# include "Tally.h"
# include "Criticality.h"

void CDTally::TallyCellByTL(CDGeometry &cGeometry, CDParticleState &cParticleState, CDAceData &cAceData, CDMaterial &cMaterial)
{	
	static vector<int> nCellsU;

	////////////////// convert CellsU for filter-based tally ////////////////
	for(int n = 0;n < p_vCellTally.size(); ++n)  // search all cell tally list
	{
		nCellsU = cParticleState.p_vLocCellsU;
		int nFilterSize = p_vCellTally[n].p_vFilter.size();
		if( nFilterSize > 0)
		{
			nCellsU.resize(nFilterSize);
			for (int k = 0 ; k < nFilterSize ; ++k )
			{
				if(p_vCellTally[n].p_vFilter[k] == 0)
				{
					nCellsU[k] = 0;
				}
			}
		}
		if(p_vCellTally[n].p_nFlagLevel >= 0)
		{
			int nCell = nCellsU[p_vCellTally[n].p_nFlagLevel];
			if(nCell >= p_vCellTally[n].p_vTallyLevelFlags.size() || p_vCellTally[n].p_vTallyLevelFlags[nCell] != 1)
			{
				continue;
			}
		}


		////////////////// find tally cell position ////////////////
		int i; // tally cell pos
		if(p_bUseTallyCellMap) /////// use cell map to quickly define tally cell position
		{
			long long nKey = cGeometry.GetMapKeyWord(nCellsU);

			int nMin;
			int nMax;

			//// use cell bin map to quickly define coarse bin
			if(p_bUseTallyCellBinMap) 
			{
				double dTemp = double(nKey)* p_vCellTally[n].p_OCellBinMap.p_dScale;
				int nIndex = p_vCellTally[n].p_OCellBinMap.GetBinMapIndex(dTemp);
				if(nIndex < 0 || nIndex >= p_vCellTally[n].p_vCellBinPtr.size())
				{
					continue;
				}
				nMin = p_vCellTally[n].p_vCellBinPtr[nIndex].Lower;
				nMax = p_vCellTally[n].p_vCellBinPtr[nIndex].Upper;

				//printf("a = %f, index = %d, min = %d, max = %d\n",temp,index,min,max);
			}
			else
			{
				nMin = 0;
				nMax = p_vCellTally[n].p_vCellVecListU.size() - 1;
			}
			//// use cell map to quickly define tally cell position
			if(!p_vCellTally[n].p_OCellMap.GetMapVal(nKey, i,nMin, nMax)) 
			{  
				continue ;	
			}

			///// double check
			if(p_vCellTally[n].p_vCellVecListU[i] != nCellsU )  
			{
				continue;
			}
		}
		else
		{
			int nCellSize = p_vCellTally[n].p_vCellVecListU.size();
			for(i = 0; i < nCellSize; ++i)
			{
				if(p_vCellTally[n].p_vCellVecListU[i] == nCellsU )
				{
					break;
				}
			}
			if(i >= nCellSize)
			{
				continue;
			}
		}


		/////////////////////  Integral tally case ///////////////////////
		if( p_vCellTally[n].p_vIntgrlBins.size() > 0 )
		{
			i = p_vCellTally[n].p_vIntgrlIndex[i];
		}


		//////////////////////// Add tally Score /////////////////////////
		////////// calculate tally score
		double dMultiplier = cParticleState.p_dWgt * p_dTrackLength;

		double dTallyAdd = dMultiplier * CalcTallyScore(p_vCellTally[n].p_nTallyType, cParticleState, cMaterial, cAceData);



		/////////// Judge energy-bin existing
		int nErgBinNum = p_vCellTally[n].p_nErgBinSize;
		if(nErgBinNum > 0)
		{
			double dErg = cParticleState.p_dErg;

			if(cAceData.p_bIsMultiGroup)
			{
				int  k = cAceData.p_nMltGrpNum - int(dErg);
				dErg = cAceData.p_vMltCentErg[k];
			}

			int nMin = 0, nMax = nErgBinNum - 1;
			int j;// Energy bin position;
			CDGlobeFun::GetIntpltPos(&p_vCellTally[n].p_vErgBins[0], dErg, nMin, nMax, j);

			int nPtr = p_vCellTally[n].GetCellErgPtr(i,j);
			p_OCellTallyData.p_vScore[nPtr] = p_OCellTallyData.p_vScore[nPtr] + dTallyAdd;

			if(p_bIsPerHstry)
			{
				p_OCellTallyData.p_setScoreIndex.insert(nPtr);
			}
		}

		int nPtr = p_vCellTally[n].GetCellPtr(i);
		p_OCellTallyData.p_vScore[nPtr] = p_OCellTallyData.p_vScore[nPtr] + dTallyAdd;

		if(p_bIsPerHstry)
		{
			p_OCellTallyData.p_setScoreIndex.insert(nPtr);
		}
	}

	return ;
}



double CDTally::CalcTallyScore(int nTallyType, CDParticleState &cParticleState, CDMaterial &cMaterial, CDAceData &cAceData)
{
	/// Needed to be multip
	double dErg = cParticleState.p_dErg;

	double dTallyAdd = 0;
	if(nTallyType == 1)   // flux, ¦Õ
	{
		dTallyAdd = 1;
	}
	else if(nTallyType == 2)  // Power(Fission Q), qf*¦²f*¦Õ
	{
		int nMat = cParticleState.p_nMAT;
		if(nMat != 0)
		{
			dTallyAdd = CalcMatFisQ(nMat, cMaterial, cParticleState,cAceData);	
		}
	}
	else if(nTallyType == 3)  // fission rate, ¦²f*¦Õ
	{
		int nMat = cParticleState.p_nMAT;
		if(nMat != 0)
		{
			for(int i = 0; i < cMaterial.GetMatTotNucNum(nMat); ++i )
			{
				int nNuc = cMaterial.GetMatNucIndex(nMat,i);
				dTallyAdd = dTallyAdd + cAceData.p_vNuclides[nNuc].p_ONucCs.p_dFis*cMaterial.GetMatNucAtomDen(nMat,i);
			}
		}
	}
	else if(nTallyType == 4)  // absorption rate, ¦²a*¦Õ
	{
		int nMat = cParticleState.p_nMAT;
		if(nMat != 0)
		{
			for(int i = 0; i < cMaterial.GetMatTotNucNum(nMat); ++i )
			{				
				int nNuc = cMaterial.GetMatNucIndex(nMat,i);
				if (cAceData.p_bIsMultiGroup)
				{
					cAceData.p_vNuclides[nNuc].p_ONucCs.p_dAbs = cAceData.GetMgNucAbsCs(nNuc,cParticleState.ErgGrp());
					dTallyAdd = dTallyAdd + cAceData.p_vNuclides[nNuc].p_ONucCs.p_dAbs*cMaterial.GetMatNucAtomDen(nMat,i);
				}
				else
				{
					int nInperpPos = cAceData.p_vNuclides[nNuc].p_nNucInterpPos;
					double dInperpFrac = cAceData.p_vNuclides[nNuc].p_dNucInterpFrac;
					int nNE = cAceData.GetErgGridNum(nNuc);
					cAceData.p_vNuclides[nNuc].p_ONucCs.p_dAbs = CDGlobeFun::IntpltByPosFr(&cAceData.p_vNuclides[nNuc].XSS[0],nInperpPos + 2*nNE,dInperpFrac);
					dTallyAdd = dTallyAdd + cAceData.p_vNuclides[nNuc].p_ONucCs.p_dAbs*cMaterial.GetMatNucAtomDen(nMat,i);
				}
			}
		}
	}
	else if(nTallyType == 5)  // fission neutron production rate, ¦Ô*¦²f*¦Õ
	{
		int nMat = cParticleState.p_nMAT;
		if(nMat != 0)
		{
			for(int i = 0; i < cMaterial.GetMatTotNucNum(nMat); ++i )
			{
				int nNuc = cMaterial.GetMatNucIndex(nMat,i);
				dTallyAdd = dTallyAdd + cAceData.p_vNuclides[nNuc].p_ONucCs.p_dNu * cAceData.p_vNuclides[nNuc].p_ONucCs.p_dFis*cMaterial.GetMatNucAtomDen(nMat,i);
			}
		}
	}

	return dTallyAdd;

}