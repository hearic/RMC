# include "SeFmNpm.h"
# include "GlobeFun.h"
# include "Criticality.h"

void CNpMatrix::InitNpMatrix()
{
	int nTot_mesh_num = p_ONpMesh.GetTotMeshNum();
	if(nTot_mesh_num < 1)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect mesh for noise propagation matrix.\n");},CDOutput::_WARNING);
	}

	p_vS0.resize(nTot_mesh_num);
	p_vS1.resize(nTot_mesh_num);
	p_vSm.resize(nTot_mesh_num);
	CDGlobeFun::Set1DVecZero(p_vS0);
	CDGlobeFun::Set1DVecZero(p_vS1);
	CDGlobeFun::Set1DVecZero(p_vSm);
	CDGlobeFun::Gen2DZeroMatrix(p_vLm0,nTot_mesh_num,nTot_mesh_num);
	CDGlobeFun::Gen2DZeroMatrix(p_vLm1,nTot_mesh_num,nTot_mesh_num);
	CDGlobeFun::Gen2DZeroMatrix(p_vA0,nTot_mesh_num,nTot_mesh_num);

	return;
}

void CNpMatrix::TallySrcVec(const double dPos[3], double dWgt)
{
	int nIndex = p_ONpMesh.GetMeshIndex(dPos,true);

	if(nIndex < 0 || nIndex >= p_ONpMesh.GetTotMeshNum())
	{
		return;
	}

	p_vS1[nIndex]  = p_vS1[nIndex] + dWgt ;

	return;
}

void CNpMatrix::AccumTallies(CDCriticality& cCriticality)
{
	p_nCycNum = cCriticality.p_nCurrentCYCLE - cCriticality.p_nInactCycNum;

	CDGlobeFun::NormalizeVec(p_vS1);

	int nTot_mesh_num = p_vS1.size();
	////////////////////// Lm0 = sum(Si*Si'), i = 1 : m ////////////////////
	for(int i = 0 ; i < nTot_mesh_num ; ++i)
	{
		for(int j = 0 ; j < nTot_mesh_num ; ++j)
		{
			p_vLm0[i][j] = p_vLm0[i][j] + p_vS1[i]*p_vS1[j];
		}
	}

	//////////////////////// Sm = sum(Si), i = 2 : m ///////////////////////
	for(int i = 0 ; i < nTot_mesh_num ; ++i)
	{
			p_vSm[i] = p_vSm[i] + p_vS1[i];
	}

	////////////////////// Lm1 = sum(Si*S(i-1)'), i = 2 : m ////////////////
	if(p_nCycNum >= 2)
	{
		for(int i = 0 ; i < nTot_mesh_num ; ++i)
		{
			for(int j = 0 ; j < nTot_mesh_num ; ++j)
			{
				p_vLm1[i][j] = p_vLm1[i][j] + p_vS1[i]*p_vS0[j];
			}
		}
	}


	p_vS0 = p_vS1;

	CDGlobeFun::Set1DVecZero(p_vS1);

	return;
}

void CNpMatrix::GenNpMatrix(CDCriticality& cCriticality)
{
	auto Lm1_temp = p_vLm1;
	auto Lm0_temp = p_vLm0;
	auto Sm_temp = p_vSm;

	//////////////// find and erase zero elements ///////////////
	for(;;)
	{
		////////////////// find zero index /////////////
		int nIndex = 0;
		int nSize = Sm_temp.size();
		for(nIndex = 0; nIndex < nSize; nIndex++)
		{
			if(Sm_temp[nIndex] == 0)
			{
				break;
			}
		}

		if(nIndex == nSize)
		{
			break;
		}

		////////////////// erase zero elements /////////////
		if(Sm_temp.size() == 0)
		{
			return;
		}

		Sm_temp.erase(Sm_temp.begin() + nIndex);
		Lm0_temp.erase(Lm0_temp.begin() + nIndex);
		Lm1_temp.erase(Lm1_temp.begin() + nIndex);
		for(int i = 0;i < Lm0_temp.size(); ++i)
		{
			Lm0_temp[i].erase(Lm0_temp[i].begin() + nIndex );
			Lm1_temp[i].erase(Lm1_temp[i].begin() + nIndex );
		}
	}

	//////////////// calculate noise propagation matrix ///////////////
	//// A0 = ((m-m0)/(m-m0-1)*Lm0 - 1/(m-m0)*Sm*SmT) * inv(Lm0) //////
	vector<vector<double> > vMatrixTemp;
	p_nCycNum = cCriticality.p_nCurrentCYCLE - cCriticality.p_nInactCycNum;
	double s = double(p_nCycNum);

	int nSize = Lm1_temp.size();
	CDGlobeFun::Gen2DZeroMatrix(vMatrixTemp,nSize,nSize);
	CDGlobeFun::Gen2DZeroMatrix(p_vA0,nSize,nSize);

	for(int i = 0 ; i < nSize ; i++)
	{
		for(int j = 0 ; j < nSize ; j++)
		{
			vMatrixTemp[i][j] = s/(s-1.0) * Lm1_temp[i][j] - 1.0/s * Sm_temp[i] * Sm_temp[j];
		}
	}

	GlobeFunFsc::InvertMatrix(Lm0_temp);
	GlobeFunFsc::MultiplyMatrix(vMatrixTemp,Lm0_temp,p_vA0);

	return;
}

void CNpMatrix::CalcNpmEigen(CDCriticality& cCriticality)
{
	////////////// Generate noise propagation matrix ///////////////////
	GenNpMatrix(cCriticality);

	////////////// calculate matrix eigenvalue, namely DR ///////////////
	if(p_vA0.size() < 2)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to calculate DR with noise propagation matrix.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		p_dEigVal = -1;
		return;
	}

	vector<double> vTemp;
	GlobeFunFsc::CalcEigVec(p_vA0,p_dEigVal,vTemp);

	return;
}