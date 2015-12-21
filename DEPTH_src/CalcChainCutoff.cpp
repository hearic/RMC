# include "TTA_Class.h"
# include "Depth_Class.h"
//---------------------------------//
//calculate Cutoff for termination //
//---------------------------------//
//Index: the Index of the nuclide in the chain
//Lamda: lamdas of the chain
//Bn: Bn of the chain
double TTA_Class::CalcChainCutOff(int Index)
{

	double temp_lamda;
	double temp_b;
	double temp_bnPn;

	//��Ҫ�ĵ��������ȴ洢����
	temp_lamda = ChainLamda[Index+1];
	temp_b = ChainEffBranch[Index+1];

	ChainLamda[Index+1] = 0.0;
	ChainEffBranch[Index+1] = 1.0;
	temp_bnPn = CalcChainNucDen(Index+1);

	//���������ٻ�����
	ChainLamda[Index+1] = temp_lamda ;
	ChainEffBranch[Index+1] = temp_b;

	temp_bnPn = temp_bnPn*ChainEffBranch[Index+1];
	return temp_bnPn;
}

