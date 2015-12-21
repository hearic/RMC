# include "Burnup.h"

void CDBurnup::ExpdBurnableCell(CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally,CDAceData &cAceData)
{
	/////////////////// Only Run once for burnup case ///////////////////
	static int nRunOnce = 0;
	if(nRunOnce == 1){return;}
	nRunOnce = 1 ;

	if(p_nBurnStep != 0 )
	{
		return;
	}

	// check vary mat input
	for (int nStep = 0; nStep < p_vBurnVaryMat.size(); nStep++)
	{
		if (p_vBurnVaryMat[nStep].p_nBurnStep > p_nTotBurnStep )
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"STEP %d too large in VARYMAT card in BURNUP block.\n", 
				p_vBurnVaryMat[nStep].p_nBurnStep);},Output.p_fpOutputFilePtr,CDOutput::_ERROR);
		}
		for (int nMat = 0; nMat < p_vBurnVaryMat[nStep].p_vVaryMats.size(); nMat++)
		{
			int nMatInput = p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nMatIndex;
			int nMatIndex = cMaterial.p_OMatSetIndex.GetIndex(nMatInput);
			if (nMatIndex <= 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"varying mat %d not exist for STEP %d in VARYMAT card in BURNUP block.\n", 
					nMatInput, p_vBurnVaryMat[nStep].p_nBurnStep);},Output.p_fpOutputFilePtr,CDOutput::_ERROR);
			}
			p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nMatIndex = nMatIndex;

			int nNewMatInput = p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nNewMatIndex;
			int nNewMatIndex = cMaterial.p_OMatSetIndex.GetIndex(nNewMatInput);
			if (nNewMatIndex <= 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"varying new mat %d not exist for STEP %d in VARYMAT card in BURNUP block.\n", 
					nNewMatInput, p_vBurnVaryMat[nStep].p_nBurnStep);},Output.p_fpOutputFilePtr,CDOutput::_ERROR);
			}
			p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nNewMatIndex = nNewMatIndex;
		}
	}

	vector<vector<int> > vAllExpdCells;
	vector<vector<int> > vTempExpdCells;
	vector<int> vTempCells;
	double dTotFuelGram = 0;
	// max material number before expanding
	int nPreMaxMatIndexU = cMaterial.p_OMatSetIndex.p_vIndex.size() - 1 ;

	for(int i = 0;i < p_vBurnableCell.size(); ++i)
	{
		int nCellIndexU = p_vBurnableCell[i] ;
		int nCellIndex = cGeometry.p_OCellIndex.GetIndex(nCellIndexU);

		if(nCellIndex < 0 || cGeometry.p_vCell[nCellIndex].p_nFillUnivIndex > 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect or non-bottom CELL defined in BURNUP block.\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		///////////////////////// expand cell material ////////////////////////////
		int nMatIndex = cMaterial.p_OMatSetIndex.GetIndex(cGeometry.p_vCell[nCellIndex].p_nMatIndexU);

		vTempCells.resize(0);
		vTempExpdCells.resize(0);
		cGeometry.ExpdGlobalCell(vTempExpdCells,vTempCells,nCellIndexU,0);
		int nExpdMatNum = vTempExpdCells.size();
		
		if (nExpdMatNum <= 0)
		{
			continue;
		}
		int nTotMatNumTemp;
		if(nExpdMatNum >= 1 && !cGeometry.p_vCell[nCellIndex].p_bIsExpdMat)// material may have been expanded already using lattice material input, 20140108
		{
			cGeometry.p_vCell[nCellIndex].p_bIsExpdMat = true ;
			int nMatIndexU = cMaterial.p_OMatSetIndex.p_vIndex.size() - 1 ;
			cGeometry.p_vCell[nCellIndex].p_nLatMatNum = nExpdMatNum ;

			nTotMatNumTemp = cMaterial.GetTotMatNum();
			for(int j = 1 ; j <= nExpdMatNum ; ++j )
			{
				cMaterial.p_OMatSetIndex.p_vIndex.push_back(nTotMatNumTemp + j);
				cMaterial.p_OMatSetIndex.p_vIndexU.push_back(nMatIndexU + j );
				cMaterial.p_vMatSet.push_back(cMaterial.p_vMatSet[nMatIndex]);

				long long llKey = cGeometry.GetMapKeyWord(vTempExpdCells[j-1]);
				if(cGeometry.p_vCell[nCellIndex].LatMatMap.find(llKey) != cGeometry.p_vCell[nCellIndex].LatMatMap.end()) 
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to generate material map for burnup calculation. Cell index may be too large!");}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				cGeometry.p_vCell[nCellIndex].LatMatMap[llKey] = nTotMatNumTemp + j ;
			}
			cGeometry.p_vCell[nCellIndex].p_nLatMatIndex = nTotMatNumTemp + 1 ;
		}

		//////////////////////// sum up OneGroupCsTally for burnup///////////////////
		vAllExpdCells.insert(vAllExpdCells.end(),vTempExpdCells.begin(),vTempExpdCells.end());

		
		/////////////////////// sum up total/individual fuel quantity///////////////////////////
		for (int i = 0;i < nExpdMatNum; i++)
		{
			double dVol = cGeometry.p_vCell[nCellIndex].p_dVol;
			int nMat_index = cGeometry.GetCellMat(vTempExpdCells[i]);
			double dDen = cMaterial.GetMatFuelDen(nMat_index, cAceData.p_vNuclides);
			dTotFuelGram = dTotFuelGram + dDen * dVol ;
			p_vCellInitFuelMass.push_back(dDen * dVol * 1.0E-3);  // KgHM
		}

		// expand varying mat input if same with expand mat, save to p_nExpandMatIndex
		for (int nStep = 0; nStep < p_vBurnVaryMat.size(); nStep++)
		{
			for (int nMat = 0; nMat < p_vBurnVaryMat[nStep].p_vVaryMats.size(); nMat++)
			{
				if(nMatIndex == p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nMatIndex)
				{
					for (int j = 1 ; j <= nExpdMatNum ; ++j) // push expanded mat
					{
						p_vBurnVaryMat[nStep].p_vVaryMats[nMat].p_nExpandMatIndex.push_back(nTotMatNumTemp+j);
					}

					//Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"burnable varying mat %d is expanded as %-%d mat for STEP %d in VARYMAT card in BURNUP block.\n",
					//	cGeometry.p_vCell[nCellIndex].p_nMatIndexU, nTotMatNumTemp+1, nTotMatNumTemp+nExpdMatNum, p_vBurnVaryMat[nStep].p_nBurnStep);},CDOutput::_WARNING);

				}
			}
		}
	}

	/////////////////////// check number of burnable cells for burnup calculation ///////////////////
	p_nBurnTallyStartPos = cTally.p_vOneGroupCsTally.size();
	p_nBurnCellNum = vAllExpdCells.size() ;
	if(p_nBurnCellNum == 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown cell defined for burnup calculation!");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
	// new expanded material number
	int nPostMaxMatIndexU = cMaterial.p_OMatSetIndex.p_vIndex.size() - 1 ;

	if (p_nBurnCellNum == 1 )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"New material %d is created to trace nuclide transmutation in the burnup cell.\n",
			nPreMaxMatIndexU+1);},Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	}
	else // > 1
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"New materials %d to %d are created to trace nuclide transmutation in %d burnable regions.\n",
			nPreMaxMatIndexU+1, nPostMaxMatIndexU, p_nBurnCellNum);},Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	}	

	/////////////////////// set all OneGroupCsTally for burnup //////////////////////////
	cTally.p_vOneGroupCsTally.resize(p_nBurnTallyStartPos + p_nBurnCellNum);
	for(int i = p_nBurnTallyStartPos ; i < p_nBurnTallyStartPos + p_nBurnCellNum ; ++i )
	{
		cTally.p_vOneGroupCsTally[i].p_bIsForBurnup = true ;
		if (p_dNAECFInput == 0) cTally.p_vOneGroupCsTally[i].p_bIsTallyNeuAveErg = true;
		cTally.p_vOneGroupCsTally[i].p_vCellVecU = vAllExpdCells[i - p_nBurnTallyStartPos] ;
	}

	//////////// calculate actual power according to user defined power density ///////////
	int nSize = p_vStepBurnPowerDen.size();
	p_vStepBurnPower.resize(nSize);
	for(int i = 0 ; i < nSize ; ++i)
	{
		p_vStepBurnPower[i] = dTotFuelGram * p_vStepBurnPowerDen[i] * 1.0E-6;  // MW
	}

	return;
}