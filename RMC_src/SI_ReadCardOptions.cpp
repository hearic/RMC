# include "Input.h"

void CDInput::SI_ReadCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, vector<vector<double> > &vParas, bool &bIsBlockEnd)
{
	/////////////////////// check KeyWord/ParaNum/Paras /////////////////////////////
	if(vKeyWord.size() != vParaNum.size())
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"inconsistent KeyWord-ParaNum in reading input card.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
	vParas.resize(vKeyWord.size());


	/////////////////////// Read options of the input card //////////////////////////
	bIsBlockEnd = false;
	for(;;)
	{
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		//if(nNext_p >= 2)   
		//{
		//	if(nNext_p >= 3) //current block is end
		//	{
		//		bIsBlockEnd = true;
		//	}
		//	break;
		//} 
		//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);  
		//if(nextchar == '$'||nextchar == '%') 
		if(nextchar == '$'||nextchar == '%'||nNext_p == 4) 
		{
			if(nextchar == '%'||nNext_p == 4) 
			{
			bIsBlockEnd = true;
			}
			break;
		}
		


		/////////////////////// read keyword string and find corresponding keyword //////////////
		string ReadKeyWordStr ;
		if(sInputCard.compare("InitSrc") == 0)
		{
			CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,ReadKeyWordStr);// skip "type"
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '"');
		}
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,ReadKeyWordStr);
		if(sInputCard.compare("InitSrc") == 0)
		{
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '"');
			nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
			string ReadParNameStr ;
			CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,ReadParNameStr);// skip "params"
			//	CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
		}

		int nIndex;
		for(nIndex = 0 ; nIndex < vKeyWord.size() ; ++nIndex)
		{
			if(ReadKeyWordStr == vKeyWord[nIndex])
			{
				break;
			}
		}
		if(nIndex == int(vKeyWord.size())) 
		{  
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown keyword %s in %s card.\n",
				ReadKeyWordStr.c_str(),sInputCard.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		if(vParas[nIndex].size() > 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%s is redefined in %s card.\n",
				ReadKeyWordStr.c_str(),sInputCard.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		//////////////////////////////////// read values corresponding keyword //////////////////
		CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
		double dRead_f;
		if( vParaNum[nIndex] >= 0 ) //// read parameters of fixed length
		{
			for(int i = 0 ; i < vParaNum[nIndex] ; ++i)
			{
				if(CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >=2 )
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unexpected format for %s in %s card.\n",
						ReadKeyWordStr.c_str(),sInputCard.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				if (nextchar==',') CDFileIO::SkipSpecChar(p_fpInputFilePtr, ',');
				if(fscanf(p_fpInputFilePtr,"%lf",&dRead_f) <= 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown parameters for %s in %s card.\n",
						ReadKeyWordStr.c_str(),sInputCard.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				vParas[nIndex].push_back(dRead_f);
			}
			continue;
		}
		//// read parameters of varying length
		nNext_p = CDFileIO::SI_ReadVaryVec(p_fpInputFilePtr,vParas[nIndex],"%lf");
		//if(nNext_p >= 2)   ////current block is end
		//{
		//	if(nNext_p >= 3)
		//	{
		//		bIsBlockEnd = true;
		//	}
		//	break;
		//}
		//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);  
		//if(nextchar == '$'||nextchar == '%') 
	   if(nextchar == '$'||nextchar == '%'||nNext_p == 4) 
		{
			if(nextchar == '%'||nNext_p == 4) 
			{
				bIsBlockEnd = true;
			}
			break;
		}
	}

	if(sInputCard.compare("UNIVERSE") != 0)
	{
		for(int i = 0 ; i < vParas.size(); ++i)
		{
			if(vParas[i].size() > 0)
			{
				return;
			}
		}

		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"no options/parameters found in %s card.\n",
			sInputCard.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	return;
}
