# include "SeFmNpm.h"
# include "GlobeFun.h"


void CDFisMatrix::InitFisMatrix()
{
	int nTot_mesh_num = p_OFmMesh.GetTotMeshNum();
	if(nTot_mesh_num < 1)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect mesh for fission matrix.\n");},CDOutput::_WARNING);
	}

	p_vStartSrc.resize(nTot_mesh_num);
	p_vEigVec.resize(nTot_mesh_num);
	CDGlobeFun::Set1DVecZero(p_vStartSrc);
	CDGlobeFun::Set1DVecZero(p_vEigVec);

	CDGlobeFun::Gen2DZeroMatrix(p_vFisMatrix,nTot_mesh_num,nTot_mesh_num);

	return;
}

void CDFisMatrix::TallyBornSrc(const double dPos[3], double dWgt)
{
	p_nBornIndex = p_OFmMesh.GetMeshIndex(dPos,true);

	if(p_nBornIndex < 0 || p_nBornIndex >= p_OFmMesh.GetTotMeshNum())
	{
		return;
	}

	p_vStartSrc[p_nBornIndex] = p_vStartSrc[p_nBornIndex] + dWgt ;

	return;
}

void CDFisMatrix::TallyFisMatrix(const double dPos[3],double dKeffFisWgt)
{
	//// Note: use fission wgt instead of NeuNum, to reduce fluctuation
	int nFisIndex = p_OFmMesh.GetMeshIndex(dPos,true);
	if(nFisIndex < 0 || nFisIndex >= p_OFmMesh.GetTotMeshNum())
	{
		return;
	}
	if(p_nBornIndex < 0 || p_nBornIndex >= p_OFmMesh.GetTotMeshNum())
	{
		return;
	}

	p_vFisMatrix[nFisIndex][p_nBornIndex] = p_vFisMatrix[nFisIndex][p_nBornIndex] + dKeffFisWgt; //C_Criticality.KeffFinal * FisWgt;

	return;
}


void CDFisMatrix::GenFisMarix()
{
	////////// Generate fission matrix by F[i][j] = A[i][j]/S[j] /////////
	int nTot_mesh_num = p_vEigVec.size();
	for(int j = 0;j < nTot_mesh_num; ++j)   // set zeros
	{
		for(int i = 0;i < nTot_mesh_num; ++i)
		{
			if(p_vStartSrc[j] > 0) 
			{
				p_vFisMatrix[i][j] = p_vFisMatrix[i][j]/p_vStartSrc[j];	
			}
			else
			{
				p_vFisMatrix[i][j] = 0;
			}
		}
	}
	return;
}

void CDFisMatrix::PrintFisMatrix()
{

	fprintf(Output.p_fpFscFilePtr,"\n\n======== Fission matrix mesh: %d * %d * %d = %d ========\nData= \n",
		p_OFmMesh.p_nMeshNum[0],p_OFmMesh.p_nMeshNum[1],p_OFmMesh.p_nMeshNum[2],p_OFmMesh.GetTotMeshNum());
	int nTot_mesh_num = p_vFisMatrix.size();
	for(int j = 0;j < nTot_mesh_num; ++j)   // set zeros
	{
		for(int i = 0;i < nTot_mesh_num; ++i)
		{
			fprintf(Output.p_fpFscFilePtr,"%.6E  ",p_vFisMatrix[i][j]);
			if(i == nTot_mesh_num - 1)
			{
				fprintf(Output.p_fpFscFilePtr,"\n",p_vFisMatrix[i][j]);
			}
		}
	}

	return;
}


void CDFisMatrix::CalcFmEigen()
{
	////////// Generate fission matrix by F[i][j] = A[i][j]/S[j] /////////
	GenFisMarix();

	////////// Ouput fission matrix to file /////////
    PrintFisMatrix();

	///////////// Calculate K0 and S0 by power iteration /////////////////
	GlobeFunFsc::CalcEigVec(p_vFisMatrix,p_dEigVal,p_vEigVec); 

	///////////// Calculate K1 and DR by Wielandt's deflation ////////////
	CalcFmDR();   

	return;
}

void CDFisMatrix::CalcFmDR()
{
	////////////////////// Wielandt¡¯s deflation ///////////////////////
	int nTot_mesh_num = p_vFisMatrix.size();
	vector<double> vTemp(nTot_mesh_num);

	int nIndex = 0;
	int j = 0;
	for(j = 0;j < nTot_mesh_num; ++j)  
	{
	    if(p_vEigVec[j] == 1)
		{
			nIndex = j;
			break;
		}
	}
	if(j >= nTot_mesh_num)
	{
		printf("Wrong deflation.\n");
	}

	for(int j = 0;j < nTot_mesh_num; ++j)  
	{
		vTemp[j] = p_vFisMatrix[nIndex][j];
	}
	for(int i = 0;i < nTot_mesh_num; ++i)  
	{
		for(int j = 0;j < nTot_mesh_num; ++j)
		{
			p_vFisMatrix[i][j] = p_vFisMatrix[i][j] -   p_vEigVec[i] *  vTemp[j];
		}
	}

	p_vFisMatrix.erase(p_vFisMatrix.begin() + nIndex);
	for(int i = 0;i < nTot_mesh_num - 1; ++i)
	{
		p_vFisMatrix[i].erase(p_vFisMatrix[i].begin() + nIndex );
	}

	///////////// Calculate K1 and S1 by power iteration /////////////////
	double dEigValTemp = 0;
	vector<double> vEigVecTemp;
	GlobeFunFsc::CalcEigVec(p_vFisMatrix,dEigValTemp,vEigVecTemp); 

	/////////// Obtain DR /////////
	p_dDR = dEigValTemp/p_dEigVal;

	//printf("DR = %f/%f = %f\n",EigValTemp,EigVal,DR);
	return;
}

void CDFisMatrix::CalcFmEntropy()
{
	CDGlobeFun::NormalizeVec(p_vStartSrc);
	CDGlobeFun::NormalizeVec(p_vEigVec);

	
	p_dFmSrcSe = GlobeFunFsc::CalcShanEntropy(p_vStartSrc);  // H(S0)
	p_dFmVecSe = GlobeFunFsc::CalcShanEntropy(p_vEigVec);    // H(S1)
	p_dFmKld = GlobeFunFsc::CalcRltvEntropy(p_vEigVec,p_vStartSrc);  // H(S1|S0)

	return;
}



//void CFisMatrix::CalFmEigen2()
//{
//	int tot_mesh_num = EigVec.size();
//
//	MatrixXd FmTemp(tot_mesh_num,tot_mesh_num);
//	for(int j = 0;j < tot_mesh_num; ++j)   // set zeros
//	{
//		for(int i = 0;i < tot_mesh_num; ++i)
//		{
//			FmTemp(i,j) = FisMatrix[i][j];
//		}
//	}
//	EigenSolver<MatrixXd> es(FmTemp,false);
//
//	double eig_val0 = 0; // largest eigenvalue
//	double eig_val1 = 0; // second largest eigenvalue
//	int index = 0;
//	for(int i = 0 ; i < tot_mesh_num ; ++i)
//	{
//		if(es.eigenvalues()[i].real() > eig_val0)
//		{
//			eig_val0 = es.eigenvalues()[i].real();
//			index = i;
//		}
//	}
//	for(int i = 0 ; i < tot_mesh_num ; ++i)
//	{
//		if(i == index) continue;
//
//		if(es.eigenvalues()[i].real() > eig_val1)
//		{
//			eig_val1 = es.eigenvalues()[i].real();
//		}
//	}
//
//	printf("DR = %f/%f = %f\n",eig_val1,eig_val0,eig_val1/eig_val0);
//
//	VectorXcd v = es.eigenvectors().col(index);
//	for(int i = 0;i < tot_mesh_num; ++i)
//	{
//		EigVec[i] = v(i).real();
//	}
//
//	EigVal = eig_val0;
//
//	return;
//}