# include "Geometry.h"
# include "Tally.h"
# include "TallyType.h"
# include "Material.h"
# include "AceData.h"



void CDTallyData::Initiate()  // expand size of tally data
{
	p_vScore.resize(0);
	p_vSum1.resize(0);
	p_vSum2.resize(0);
	p_vAve.resize(0);
	p_vRe.resize(0);
	return;
}

void CDTallyData::SetZero()  // expand size of tally data
{
	CDGlobeFun::Set1DVecZero(p_vScore); 
	CDGlobeFun::Set1DVecZero(p_vSum1); 
	CDGlobeFun::Set1DVecZero(p_vSum2); 
	CDGlobeFun::Set1DVecZero(p_vAve); 
	CDGlobeFun::Set1DVecZero(p_vRe); 
	return;
}

int CDTallyData::GetSize()  // expand size of tally data
{
	return p_vSum1.size();
}


void CDTallyData::ExpdSize(int nAddedSize)  // expand size of tally data
{
	int nOldSize = p_vSum1.size();
	int nNewSize = nOldSize + nAddedSize;
	p_vScore.resize(nNewSize);
	p_vSum1.resize(nNewSize);
	p_vSum2.resize(nNewSize);
	p_vAve.resize(nNewSize);
	p_vRe.resize(nNewSize);

	return;
}


void CDTallyData::SumTallyBin()
{

	for(auto iter = p_setScoreIndex.begin(); iter != p_setScoreIndex.end();++ iter)
	{
		int i = *iter;
		p_vSum1[i] = p_vSum1[i] + p_vScore[i];
		p_vSum2[i] = p_vSum2[i] + p_vScore[i]*p_vScore[i];
		p_vScore[i] = 0;
	}
	p_setScoreIndex.clear();

	return;
}

void CDTallyData::SumTallyBinStride()
{
	for(int j = 0 ; j < p_vScoreIndex2.size(); ++ j)
	{
		int nStartPos = p_vScoreIndex2[j];
		int n = p_vScoreStride[j];
		for(int i = nStartPos ; i < nStartPos + n ; ++i)
		{
			p_vSum1[i] = p_vSum1[i] + p_vScore[i];
			p_vSum2[i] = p_vSum2[i] + p_vScore[i]*p_vScore[i];
			p_vScore[i] = 0;
		}
	}
	p_setScoreIndex.clear();
	p_vScoreIndex2.clear();
	p_vScoreStride.clear();
	return;
}


void CDTallyData::SumTallyAll()
{
	for(int i = 0; i < p_vScore.size();++i)
	{
		p_vSum1[i] = p_vSum1[i] + p_vScore[i];
		p_vSum2[i] = p_vSum2[i] + p_vScore[i]*p_vScore[i];
		p_vScore[i] = 0;
	}

	return;
}


void CDTallyData::CalcAveRe(double dM, double dDiv)
{
/*  Note:
M; Number of observations, namely number of total batches/cycles/histories.
Div: Division. Tally average and relative error are calculated per history.
*/
	double dTemp2 = 1.0/dDiv;
	if(dM == 1.0)
	{
		for(int i = 0 ; i < p_vSum1.size(); ++i)
		{
			p_vAve[i]  = p_vSum1[i]*dTemp2; //Sum1[i]/(M * Div); 
			p_vRe[i]  = 0;
		}
	}
	else
	{
		double dTemp1 = 1.0/(dM - 1);
		for(int i = 0 ; i < p_vSum1.size(); ++i)
		{
			p_vAve[i]  = p_vSum1[i]/dM/dDiv; //*temp2; //Sum1[i]/(M * Div); 
			p_vRe[i]  = p_vSum1[i] == 0? 0 : sqrt(p_vSum2[i]/(p_vSum1[i]*p_vSum1[i])*dM*dTemp1 - dTemp1);
		}
	}

	return;
}


void CDCellTally::SetupCellTally(CDAceData &cAceData,CDTallyData &cCellTallyData)
{
	////// setup integral bins and indices in case of integral tally /////
	p_nCellBinSize = p_vCellVecListU.size(); // number of tally cells
	if ( p_vIntgrlBins.size() > 0)
	{
		p_vIntgrlIndex.resize(p_nCellBinSize);
		int i = 0;
		for(int j = 0 ; j < p_nCellBinSize ; ++j)
		{
			if(j + 1 > p_vIntgrlBins[i])
			{
				++i;
			}
			p_vIntgrlIndex[j] = i ;
		}

		p_nCellBinSize = p_vIntgrlBins.size(); // number of integral cell bins
	}

	////////////////////// setup multigroup energy bins ///////////////////////
	/////////////////// setup energy bins /////////////////
	if(p_vErgBins.size() > 0)
	{
		if(p_vErgBins.size() == 1 && p_vErgBins[0] == -1)
		{
			p_vErgBins = cAceData.p_vMltErgBins;
		}
		else
		{
			p_vErgBins.insert(p_vErgBins.begin(),0);
		}
	}
	p_nErgBinSize = p_vErgBins.size();  // number of energy bins

	///////////////////////// setup tally data size ///////////////////////////
	p_nDataLen = p_nCellBinSize * (p_nErgBinSize + 1); // number of total tally bins
	p_nDataStartPtr = cCellTallyData.GetSize();
	cCellTallyData.ExpdSize(p_nDataLen);

	return;
}
	
int CDCellTally::GetCellPtr(int nCellIndex)
{
	return p_nDataStartPtr + nCellIndex * (p_nErgBinSize + 1) + p_nErgBinSize;
}

int CDCellTally::GetCellErgPtr(int nCellIndex, int nErgBinIndex)
{
	return p_nDataStartPtr + nCellIndex * (p_nErgBinSize + 1) + nErgBinIndex;
}


void CDMeshTally::SetupMeshTally(CDAceData &cAceData, CDTallyData &cMeshTallyData)
{
	/////////////////// setup energy bins /////////////////
	if(p_vErgBins.size() > 0)
	{
		if(p_vErgBins.size() == 1 && p_vErgBins[0] == -1)
		{
			p_vErgBins = cAceData.p_vMltErgBins;
		}
		else
		{
			p_vErgBins.insert(p_vErgBins.begin(),0);
		}
	}
	p_nErgBinSize = p_vErgBins.size();


	///////////////////////// setup tally data size /////////////////////////
	p_nTotMeshNum = p_OTallyMesh.GetTotMeshNum();  // number of meshes
	
	p_nDataLen = p_nTotMeshNum * (p_nErgBinSize + 1);           // number of total tally bins
	p_nDataStartPtr = cMeshTallyData.GetSize();
	cMeshTallyData.ExpdSize(p_nDataLen);

	return;
}
	
int CDMeshTally::GetMeshPtr(int nMeshIndex)
{
	return p_nDataStartPtr + nMeshIndex * (p_nErgBinSize + 1) + p_nErgBinSize;
}

int CDMeshTally::GetMeshErgPtr(int nMeshIndex, int nErgBinIndex)
{
	return p_nDataStartPtr + nMeshIndex * (p_nErgBinSize + 1) + nErgBinIndex;
}

void CDCsTally::SetupCsTally(CDAceData &cAceData, CDMaterial &cMaterial, CDGeometry &cGeometry,CDTallyData &cCsTallyData, bool bUseUnionCsTally)
{
	/////////// setup material & MT for burnup case /////////////////
	if(p_bIsForBurnup)
	{
		SetupCsTallyBurn(cMaterial, cGeometry);
	}

	///////////// clear average energy tally /////////////////
	p_dNeuAveErg = 0.0;
	p_dTallyNeuNum = 0.0;

	///////////// Calculate total length of cs tally /////////////////
	int nNucNum = cMaterial.GetMatTotNucNum(p_nMat);

	p_nDataLen = 1;    // flux tally
	p_nDataStartPtr = cCsTallyData.GetSize();
	p_vNucPtr.resize(nNucNum);

	for(int i = 0; i < nNucNum ; ++i )
	{
		p_vNucPtr[i] = p_nDataStartPtr + p_nDataLen; // data location of each nuclide
		int nMtNum = p_vNucMTs[i].size();
		p_nDataLen = p_nDataLen + nMtNum;
	}
	
	cCsTallyData.ExpdSize(p_nDataLen);

	////////////////////// setup UnionEnergyGrid ////////////////////
	if(bUseUnionCsTally)
	{
		p_vUnionFlux.resize(cAceData.p_vUnionEnergyGrid.size() + 1);
		CDGlobeFun::Set1DVecZero(p_vUnionFlux); 
	}
	return;
}

void CDCsTally::SetupCsTallyBurn(CDMaterial &cMaterial, CDGeometry &cGeometry)
{
	p_nMat = cGeometry.GetCellMat(p_vCellVecU); /// NOTE Lattice case
	int nNucNum = cMaterial.GetMatTotNucNum(p_nMat);
	p_vNucMTs.resize(nNucNum);
	for(int i = 0; i < nNucNum ; ++i)
	{
		int nMtNum = 4;
		p_vNucMTs[i].resize(nMtNum) ;
		int nNucZ = atoi(cMaterial.GetMatNucID(p_nMat,i).p_chIdStr)/1000;   // Z = ZA/1000
		if(nNucZ >= 90)   // actinide
		{
			p_vNucMTs[i][0] = 102 ;  // (n,r)
			p_vNucMTs[i][1] = 16 ;   // (n,2n)
			p_vNucMTs[i][2] = 17 ;   // (n,3n)
			p_vNucMTs[i][3] = -6 ;   // (n,f)
		}
		else              // light nuclides
		{
			p_vNucMTs[i][0] = 102 ;  // (n,r)
			p_vNucMTs[i][1] = 16 ;   // (n,2n)
			p_vNucMTs[i][2] = 107 ;  // (n,a)
			p_vNucMTs[i][3] = 103 ;  // (n,p)
		}	
	}

	return;
}

int CDCsTally::GetFluxDataPtr()
{
	return p_nDataStartPtr;
}

int CDCsTally::GetNucDataPtr(int nNuc)
{
	return p_vNucPtr[nNuc];
}


void CDCsTally::DivideByFlux(CDTallyData &cCsTallyData)
{
	double dFlux = cCsTallyData.p_vAve[p_nDataStartPtr];
	if(dFlux == 0)
	{
		return;
	}

	double dT = 1/dFlux;
	for(int i = p_nDataStartPtr + 1; i < p_nDataStartPtr + p_nDataLen; ++i) // skip p_nDataStartPtr location which is flux itself
	{
		cCsTallyData.p_vAve[i] = dT * cCsTallyData.p_vAve[i];
	}
	return;
}