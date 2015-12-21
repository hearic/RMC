# include "Input.h"

void CDInput::ReadCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, vector<vector<double> > &vParas, bool &bIsBlockEnd)
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
		if(nNext_p >= 2)   
		{
			if(nNext_p >= 3) //current block is end
			{
				bIsBlockEnd = true;
			}
			break;
		} 

		/////////////////////// read keyword string and find corresponding keyword //////////////
		string ReadKeyWordStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,ReadKeyWordStr);

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
		nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,vParas[nIndex],"%lf");
		if(nNext_p >= 2)   ////current block is end
		{
			if(nNext_p >= 3)
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

int CDInput::ExtratIntPara(double dPara,string cCardStr)
{
	int Para_int = int(dPara);

	if(fabs(dPara - Para_int) > 1.0E-10)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect parameter(%.3f) in %s card.\n",
			dPara,cCardStr.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	return Para_int;
}

long long CDInput::ExtratLongPara(double dPara,string cCardStr)
{
	long long Para_int = (long long)(dPara);

	if(fabs(dPara - Para_int) > 1.0E-10)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect parameter(%.3f) in %s card.\n",
			dPara,cCardStr.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	return Para_int;
}

void CDInput::ReadFixSeqCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, vector<vector<double> > &vParas, bool &bIsBlockEnd)
{
	// read fix sequence card options, filepointer before new keyword after return
	// such as,
	//     mat = 1, matden = 2.0, nuc = 1001 92235, den = 1.5 8.7
	//     mat = 2
	// file pointer before second "mat"

	/////////////////////// check KeyWord/ParaNum/Paras /////////////////////////////
	if(vKeyWord.size() != vParaNum.size())
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"inconsistent KeyWord-ParaNum in reading input card.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	int nNum = vKeyWord.size();

	vParas.resize(nNum);

	/////////////////////// Read options of the input card //////////////////////////
	bIsBlockEnd = false;
	for(int j = 0; j < nNum;)
	{
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if(nNext_p >= 2) 
		{
			if(nNext_p >= 3) //current block is end
			{
				bIsBlockEnd = true;
			}
			break;
		} 

		/////////////////////// read keyword string and find corresponding keyword //////////////
		string ReadKeyWordStr ;
		long lPos = ftell(p_fpInputFilePtr);
		CDFileIO::GetKeyWord(p_fpInputFilePtr,ReadKeyWordStr);

		int nIndex;
		for(nIndex = j ; nIndex < nNum ; ++nIndex)
		{
			if(ReadKeyWordStr == vKeyWord[nIndex])
			{
				j = nIndex;
				break;
			}
		}
		if(nIndex == int(vKeyWord.size())) // not find
		{
			fseek(p_fpInputFilePtr,lPos,SEEK_SET);
			break;
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
		nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,vParas[nIndex],"%lf");
		if(nNext_p >= 2)   ////current block is end
		{
			if(nNext_p >= 3)
			{
				bIsBlockEnd = true;
			}
			break;
		}
	}
/*
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
*/

	return;
}