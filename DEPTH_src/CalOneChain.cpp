# include "TTA_Class.h"
# include "Depth_Class.h"

void TTA_Class::CalcOneChain()
{
//---------------------------//
// calculates one chain      //
//---------------------------//
	for( MotherNucIndex;;MotherNucIndex++)   
	{
		ChainNuclide.push_back(MotherNuc);  //put the first daughter into ChainNuclide
		ChainLamda.push_back(-TranMatrixCCS._DiagVal[MotherNuc]);
		ChainEffBranch.push_back(CalcChainBranch(MotherNucIndex));

		//if repeat, break;
		if(MotherNucIndex> 0)
		{
			int RepeatFlag=0;
			for( int j=0;j<MotherNucIndex;j++)
			{
				if(ChainNuclide[j]==MotherNuc)
				{
					RepeatFlag = 1;
					break;
				}
			}

			if(RepeatFlag==1)
			{
				break;
			}
		}

		double TTAresult = CalcChainNucDen(MotherNucIndex); //current nuclide in one chain
		if(ChainNuclide[MotherNucIndex]>0)
		{
			Depth.StepNt[Step][ChainNuclide[MotherNucIndex]] = Depth.StepNt[Step][ChainNuclide[MotherNucIndex]] + TTAresult;
		}
		
		DaughterNucNum=TranMatrixCCS.RowNum(MotherNuc);
		if(DaughterNucNum==0)
		{
			break;
		}
		//if the lamda put into the stack is extremelly small,break;
		//if(ChainLamda[MotherNucIndex] < 10E-40)
		//{
		//	break;
		//}

		//if Pn-1bn-1j<Cutoff£¬break;
		if(MotherNucIndex>0)
		{
			double PnBn = CalcChainCutOff(MotherNucIndex-1);
			if(PnBn < TTACutoff)
			{
				break;
			}
		}
	
		PushOneChain();//if MotherNuc has more than 1 daughter nuclides, put them into stack

		MotherNuc = TranMatrixCCS._ColRowPtr[MotherNuc][0];    // update MotherNuc
	}
}
