# include "TTA_Class.h"
# include "Depth_Class.h"
//---------------------------------------------------//
//calculate the density of one nuclide in the chain  //
//---------------------------------------------------//
//Index: the Index of the nuclide in the chain
//Bn: Bn of the chain
//Lamda: lamdas of the chain
//N: result of the concentration


double TTA_Class::CalcChainNucDen(int Index)
{

	vector<double> ChainLamdaTemp(ChainLamda);

	///////Variation TTA:   check repeated effective decay constant///////
	for(;;)
	{
		int VarFlag=0;
		for(int i=1;i<=Index;++i) 
		{
			for(int j=0;j<i;++j)
			{
				if(abs(ChainLamdaTemp[j] - ChainLamdaTemp[i]) < 1.0E-6*ChainLamdaTemp[i])
				{
					ChainLamdaTemp[j] = ChainLamdaTemp[j]*1.00001;
					VarFlag=1;
				}
			}
		}
		if(VarFlag==0)
		{
			break;
		}
	}


	double NtTemp = 0;
	double bn = 1;
	for(int i = 0;i<Index + 1;++i)   //i至n的循环
	{
		double alpha = 1;
		for(int j = 0;j<Index+1;++j)//计算alpha
		{
			if(j<Index)
			{
				alpha = alpha*ChainLamdaTemp[j];
			}
			if(j!=i)
			{
				alpha = alpha/(ChainLamdaTemp[j]-ChainLamdaTemp[i]);
			}

		}
		NtTemp = NtTemp + alpha*exp(-ChainLamdaTemp[i]*Time);
		bn = bn*ChainEffBranch[i];
	}
	NtTemp = HeadNucDen*NtTemp*bn;

	return NtTemp;
}
