# include "TTA_Class.h"
# include "Depth_Class.h"
//---------------------------//
// calculate Branching Ratio //
//---------------------------//
//Index: the Index of the nuclide in the chain
//Nuclide: nuclides of the chain
//lamda: lamdas of the chain


double TTA_Class::CalcChainBranch(int Index)
{
	double Bij;
	if(Index == 0) //The beginning nuclide of the chain
	{
		Bij = 1.0;
	}
	else if (Index > 0)
	{
		Bij = GetEffectDecay(ChainNuclide[Index-1],ChainNuclide[Index])/ChainLamda[Index-1];
	}
	return Bij;
}

//-----------------------------------------------//
//  Find the reaction rate (effective constant)  //
//-----------------------------------------------//
//m: mother nuclide
//n: daughter nuclide

double TTA_Class::GetEffectDecay(int m,int n)
{


	int size,iLoc;
	size=TranMatrixCCS._ColRowPtr[m].size();
	for(iLoc=0;iLoc<size;++iLoc)
	{
		if(n==TranMatrixCCS._ColRowPtr[m][iLoc])
		{
			return TranMatrixCCS._ColRowVal[m][iLoc];
		}
	}

	Depth.ErrorWarning("Reaction rate NOT found!",1);		
	return -1;
}
