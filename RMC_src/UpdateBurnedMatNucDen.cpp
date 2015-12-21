# include "Burnup.h"

void CDBurnup::UpdateBurnedMatNucDen(vector<CDNuclide>& vNuclides,CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally)
{
	// This function update nuclide densities for burn material after depletion, including
	// 1. correct NucAtomDen which have already been updated in DEPTH_to_RMC, but are multiplied by cell volume
	// 2. calculate MatNucDen/MatGramDen of every depletion material
	// 3. output material for new transport
	// 4. clear MatNucID and MatNucUserDen for saving memory
	//
	// 5. vary material according to VaryMat input
	for( int i = 0; i < p_nBurnCellNum; ++i )
	{
		int nTallyPos = p_nBurnTallyStartPos + i ;
		int nCellIndexU = *(cTally.p_vOneGroupCsTally[nTallyPos].p_vCellVecU.end()-1);
		int nCellIndex = cGeometry.p_OCellIndex.GetIndex(nCellIndexU);
		double dVol = cGeometry.p_vCell[nCellIndex].p_dVol;
		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		int nNucNum = cMaterial.p_vMatSet[nMat].p_dMatTotNucNum;
		cMaterial.p_vMatSet[nMat].p_dMatAtomDen = 0;
		cMaterial.p_vMatSet[nMat].p_dMatGramDen = 0;
		for(int j = 0 ; j < nNucNum; j++ )
		{
			cMaterial.p_vMatSet[nMat].p_vMatNucAtomDen[j] /= dVol;
			cMaterial.p_vMatSet[nMat].p_dMatAtomDen += cMaterial.p_vMatSet[nMat].p_vMatNucAtomDen[j];
			cMaterial.p_vMatSet[nMat].p_dMatGramDen += cMaterial.GetMatNucGramDen(nMat, j, vNuclides);
		}
		cMaterial.p_vMatSet[nMat].p_dMatUserDen = cMaterial.p_vMatSet[nMat].p_dMatAtomDen;
	}

	// vary mat after burn step
	for (int i = 0; i < p_vBurnVaryMat.size(); i++)
	{
		if (p_nBurnStep == p_vBurnVaryMat[i].p_nBurnStep && p_nBurnStepStrategy >= 0) // current step(after corrector step) has vary mat
		{
			for (int j = 0; j < p_vBurnVaryMat[i].p_vVaryMats.size(); j++)
			{
				int nMatIndex = p_vBurnVaryMat[i].p_vVaryMats[j].p_nMatIndex;
				int nNewMatIndex = p_vBurnVaryMat[i].p_vVaryMats[j].p_nNewMatIndex;
				cMaterial.p_vMatSet[nMatIndex] = cMaterial.p_vMatSet[nNewMatIndex];// replace vary mat as newmat
				int nExpandeMatNum = p_vBurnVaryMat[i].p_vVaryMats[j].p_nExpandMatIndex.size();
				for (int ii = 0; ii < nExpandeMatNum; ii++)
				{
					int nExpMatIndex = p_vBurnVaryMat[ii].p_vVaryMats[j].p_nExpandMatIndex[ii];
					cMaterial.p_vMatSet[nExpMatIndex] = cMaterial.p_vMatSet[nNewMatIndex];// replace vary mat as newmat for expanded ones
				}

				// output message
				int nMatIndexU = cMaterial.p_OMatSetIndex.GetIndexU(nMatIndex);
				int nNewMatIndexU = cMaterial.p_OMatSetIndex.GetIndexU(nNewMatIndex);
				char chMatStr[1000];
				if (nExpandeMatNum > 0)
				{
					sprintf(chMatStr,"burnable material %d (expanded to %d materials)", nMatIndexU, nExpandeMatNum+1);
				}
				else
				{
					sprintf(chMatStr,"material %d", nMatIndexU);
				}
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Vary %s to be material %d at burnup step %d.\n",
					chMatStr, nNewMatIndexU, p_vBurnVaryMat[i].p_nBurnStep);},Output.p_fpOutputFilePtr,CDOutput::_NORMAL);
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Vary %s to be material %d at burnup step %d.\n",
					chMatStr, nNewMatIndexU, p_vBurnVaryMat[i].p_nBurnStep);},Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
			}
			break;
		}
	}

	cMaterial.ClearMatNucUserInput();

	return;
}