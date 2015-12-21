# include "TTA_Class.h"
# include "Depth_Class.h"

//////////////////////////////////////
// Push one chain into the stack
/////////////////////////////////////
void TTA_Class::PushOneChain()
{
	if(DaughterNucNum<=1)
	{
		return;
	}

	for(int iLoc = DaughterNucNum - 1;iLoc >=1 ;--iLoc)
	{	
		int DaughterNuclide = TranMatrixCCS._ColRowPtr[MotherNuc][iLoc];
		ChainNuclide.push_back(DaughterNuclide);
		ChainLamda.push_back(-TranMatrixCCS._DiagVal[DaughterNuclide]);
		ChainEffBranch.push_back(CalcChainBranch(MotherNucIndex+1)); 
		//TTAresult = TTA_CalNucDen(MotherNucIndex,ChainLamda,ChainEffBranch);

		double PnBn = CalcChainCutOff(MotherNucIndex);
		if(PnBn > TTACutoff) 
		{
			///////check repeating, and put one Daughter Nuclide into the stack////
			int RepeatFlag  = 0;

			for(int i =0;i<ChainNuclide.size() - 1;i++)
			{
				if(ChainNuclide[i]==DaughterNuclide)
				{
					RepeatFlag++;
					//RepeatStackNum++;
					//break;
				}
			}
			if(RepeatFlag <= 1)
			{
				StackChains.push_back(ChainNuclide);
				StackNum++;
				//cout<<StackNum<<PnBn<<endl;
			}
		}

		ChainNuclide.pop_back();
		ChainLamda.pop_back();
		ChainEffBranch.pop_back();
	}
}