//# include "Tally.h"
//# include "Criticality.h"
//
//void CD_Input::ReadErgBins(vector<double> &ErgBins,bool &IsBlockEnd,bool &IsCardEnd)
//{
//	// ERGBIN card which has not been prepared
//
//
//	//IsBlockEnd = false; 
//	//IsCardEnd  = false; 
//
//	//auto &CellTally = Tally.CellTally;
//	/////////////////////// Get key word of energy bins ////////////////////
//	//char nextchar;
//	//FileIO::GetNextChar(InputFilePtr,nextchar);
//
//	//////////////////////// use existing energy bins /////////////////////
//	//if(nextchar == '#')
//	//{
//	//	FileIO::SkipSpecChar(InputFilePtr,'#');
//	//	int read_i;
//	//	if(fscanf(InputFilePtr,"%d",&read_i) < 1)
//	//	{
//	//		OutputMessage([&](){sprintf(Output.PrintStr,"unknown ID defined for tally energy bin.\n");}, 
//	//			OutputFilePtr, CD_Output::_ERROR);
//	//	}
//	//	int TallyNum = CellTally.size();
//	//	int i;
//	//	for(i = 0 ; i < TallyNum - 1 ; ++i )  //// Note: "TallyNum - 1"
//	//	{
//	//		if(CellTally[i].TallyID == read_i)
//	//		{
//	//			ErgBins = CellTally[i].ErgBins;
//	//			if(ErgBins.size() == 0)
//	//			{
//	//				OutputMessage([&](){sprintf(Output.PrintStr,"tally (ID=%d) has no energy bins.\n",read_i);}, 
//	//					OutputFilePtr, CD_Output::_ERROR);
//	//			}
//	//			break;
//	//		}
//	//	}
//	//	if( i == TallyNum - 1)
//	//	{
//	//		OutputMessage([&](){sprintf(Output.PrintStr,"undefined ID for tally energy bin.\n");}, 
//	//			OutputFilePtr, CD_Output::_ERROR);
//	//	}
//
//	//	return;
//	//}
//
//
//	//////////////////////// use multi-group bins /////////////////////////
//	////if(nextchar == 'm')   // use -1 instead
//	////{
//	////	FileIO::SkipSpecChar(InputFilePtr,'m');
//	////	if(!AceData.IsMultiGroup)
//	////	{	
//	////		OutputMessage([&](){sprintf(Output.PrintStr,"unexpected multi-group keyword for tally energy bin.\n");}, 
//	////			OutputFilePtr, CD_Output::_ERROR);
//	////	}
//
//	////	ErgBins.push_back(-999);
//	////    return ;
//	////}
//	//
//	//
//	//////////////////////// read ordinary energy bins ////////////////////
//	//ErgBins.resize(0);
//	//ErgBins.push_back(0);
//
//	//for(;;)
//	//{
//	//	int next_p = FileIO::GetNextChar(InputFilePtr,nextchar);
//	//	if(next_p >= 2)   
//	//	{
//	//		IsCardEnd = true;
//	//		if(next_p >= 3){ IsBlockEnd = true; }
//	//		break;
//	//	}
//
//	//	double read_f = -1;
//	//	if (fscanf(InputFilePtr,"%lf",&read_f) < 1)
//	//	{
//	//		break;
//	//	}
//	//	CheckInputParas(read_f > 0,[&](){sprintf(PrintStr,"incorrect tally energy bin.\n");},2);
//	//	ErgBins.push_back(read_f);
//	//}
//
//	return;
//
//}