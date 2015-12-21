# include "TTA_Class.h"

//----------------------------------------------
// Pop out one chain from the stack
//----------------------------------------------
void TTA_Class::PopOneChain()
{
	++ChainNum;

	ChainNuclide.clear();
	ChainLamda.clear();
	ChainEffBranch.clear();

	//do not store last nuclide in the chain of the outter stack
	//put it as a MotherNuc
	int size=StackChains[StackNum-1].size();
	int NucTemp,NucTempIndex;
	for(NucTempIndex = 0;NucTempIndex < size-1; NucTempIndex++) 
	{
		NucTemp = StackChains[StackNum-1][NucTempIndex];
		ChainNuclide.push_back(NucTemp);
		ChainLamda.push_back(-TranMatrixCCS._DiagVal[NucTemp]);
		ChainEffBranch.push_back(CalcChainBranch(NucTempIndex));
	}

	MotherNuc=StackChains[StackNum-1][StackChains[StackNum-1].size()-1];
	MotherNucIndex = StackChains[StackNum-1].size()-1;

	StackChains.pop_back();
	StackNum--; 

}