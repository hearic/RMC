# include "Tally.h"
# include "Criticality.h"
#include "TallyType.h"

void CDTally::TallyCsByTL(CDMaterial &cMaterial, CDGeometry &cGeometry, CDAceData &cAceData, CDParticleState &cParticleState)
{	
	//////////////////// Find the index of tallied cell ////////////////////////////
	int i ;
	if(p_bUseTallyCellMap)
	{
		long long nFindKey = cGeometry.GetMapKeyWord(cParticleState.p_vLocCellsU);
		if(CsTallyCellIndex.find(nFindKey) == CsTallyCellIndex.end()) 
		{  
			return ;	
		}
		i = CsTallyCellIndex[nFindKey];
	}
	else
	{
		int nCsTallyNum = p_vOneGroupCsTally.size();
		for(i = 0; i < nCsTallyNum; ++i )
		{
			if( p_vOneGroupCsTally[i].p_vCellVecU == cParticleState.p_vLocCellsU )
			{
				goto Found_Tallied_Cell;
			}
		}
		return;
	}


Found_Tallied_Cell:

    double dErg = cParticleState.p_dErg;
	double dCurrentWgt = cParticleState.p_dWgt;
	
	int nPtr = p_vOneGroupCsTally[i].GetFluxDataPtr();
	p_OCsTallyData.p_vScore[nPtr] = p_OCsTallyData.p_vScore[nPtr] + dCurrentWgt*p_dTrackLength; // score flux
	p_OCsTallyData.p_setScoreIndex.insert(i);

	// Tally average fission neutron energy: fission rate weighted
	int nCell_mat = cParticleState.p_nMAT;
	double dFisRateAdd = 0;
	for(int i = 0; i < cMaterial.GetMatTotNucNum(nCell_mat); ++i )
	{
		int nNuc = cMaterial.GetMatNucIndex(nCell_mat,i);
		dFisRateAdd = dFisRateAdd + cAceData.p_vNuclides[nNuc].p_ONucCs.p_dFis*cMaterial.GetMatNucAtomDen(nCell_mat,i);
	}
	if (p_vOneGroupCsTally[i].p_bIsTallyNeuAveErg)
	{
		p_vOneGroupCsTally[i].p_dNeuAveErg   += dErg*dFisRateAdd*dCurrentWgt*p_dTrackLength; // score energy 20130926, unit: Mev
		p_vOneGroupCsTally[i].p_dTallyNeuNum += dFisRateAdd*dCurrentWgt*p_dTrackLength;      // tally times
	}

	//////////////////// Tally Cs By Conventional method ////////////////////////////
	if(!p_bUseUnionCsTally)
	{
		if(cAceData.p_bUseErgBinMap) // accelerate binary search by energy bin map
		{
			cAceData.p_OAceErgBinMap.GetBinMapIndex(dErg);
		}
		int nMat = p_vOneGroupCsTally[i].p_nMat ;
		int nNucNum = cMaterial.GetMatTotNucNum(nMat) ;

		for(int j = 0; j < nNucNum; ++j )
		{
			int nNuc = cMaterial.GetMatNucIndex(nMat,j);
			TallyNucCsByTL(i,j,nNuc,p_dTrackLength, cParticleState, cAceData);
		}
	}
	/////////////////////////// Tally Cs By Union Energy Grid /////////////////////////////////
	else
	{
		int nMin = 1, nMax = cAceData.p_nUnionEgNum;
		int nInperpPos;
		CDGlobeFun::GetIntpltPos(&cAceData.p_vUnionEnergyGrid[0], cParticleState.p_dErg, nMin, nMax, nInperpPos);
		p_vOneGroupCsTally[i].p_vUnionFlux[nInperpPos] = p_vOneGroupCsTally[i].p_vUnionFlux[nInperpPos] + dCurrentWgt*p_dTrackLength;
	}

	
	return;
}