# include "TTA_Class.h"
# include "Depth_Class.h"

//-----------------------------------
//This function calculate all chains
//from one nuclide
//-----------------------------------
void TTA_Class::SolveByTTA()
{
	//////////////////////// Generate  HeadNucList ///////////////////////////
	HeadNucList.clear();
	for(int i = 1 ; i <= Depth.NucNum; ++i)
	{
        if(Depth.StepNt[Step-1][i] > 0)
		{
			HeadNucList.push_back(i);

		}
	}
	HeadNucNum = HeadNucList.size();

	//////////////////////////reserve memory //////////////////////////////
	ChainNuclide.reserve(1000);
	ChainLamda.reserve(1000);
	ChainEffBranch.reserve(1000);
	StackChains.reserve(1000);

	//////////////////////////// Solve linear chain //////////////////////////
	//cout<<"STEP="<<Step<<"   HeadNucNum="<<HeadNucNum<<endl;
	for(int i=0;i<HeadNucNum;++i)
	{
		ResetTTA();

		HeadNuc = HeadNucList[i];

		TTACutoff = CutoffFrac*Depth.StepNt[Step-1][HeadNuc];
		TTACutoff = std::max(TTACutoff,1.0E-20);
		HeadNucDen = Depth.StepNt[Step-1][HeadNuc];

		MotherNuc = HeadNuc;
		/////////calculate all the chains corresponding to the head nuclide
		for(;;)
		{
			//calculate one chain of the mother nuclide
			CalcOneChain();	

			if(StackNum==0)//if the stack is empty, break; all chains complete
			{
				break;
			}

			PopOneChain();//else pop the outter chain from the stack chains
		}

		//cout<<i+1<<"/"<<HeadNucNum<<" Head nuclides finished! "<<"Total chains of "<<Depth.NucIDList[HeadNuc]<<" = "<<ChainNum<<endl;
	}


}