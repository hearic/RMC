# include "Input.h"

void CDInput::ReadFixedSourceBlock(CDFixedSource &cFixedSource)
{
	bool IsBlockEnd = false;

	////////////////// check redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["NEUTRON"] = false;

	for(;;)
	{
		if(IsBlockEnd){ break; }

		char nextchar;
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}

		string InputCardStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);

		if(InputCardStr.compare("NEUTRON")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CRITICALITY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//////////////////////// read POWERITER card /////////////////////////
			vector<string> Options(8);
			vector<int> ParaNum(8);
			vector<vector<double> > Paras;
			Options[0] = "POPULATION";ParaNum[0] = 1 ;
			Options[1] = "POINT";     ParaNum[1] = -1 ;
			Options[2] = "SLAB";      ParaNum[2] = 6 ;
			Options[3] = "SPH";       ParaNum[3] = 4 ;
			Options[4] = "CYL/X";     ParaNum[4] = 5 ;
			Options[5] = "CYL/Y";     ParaNum[5] = 5 ;
			Options[6] = "CYL/Z";     ParaNum[6] = 5 ;
			Options[7] = "ERG";       ParaNum[7] = 1 ;
			ReadCardOptions(InputCardStr,Options,ParaNum,Paras,IsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			if(Paras[0].size() > 0)
			{
				cFixedSource.p_nNeuNum = Paras[0][0];
				Output.CheckInputParas(cFixedSource.p_nNeuNum >100,
					[&](){sprintf(Output.p_sPrintStr,"Insufficient neutron population in NEUTRON card.\n");},2);
			}

			if(Paras[7].size()>0)
			{
				cFixedSource.p_dFixedSrcErg = Paras[7][0];
				Output.CheckInputParas(cFixedSource.p_dFixedSrcErg >0,
					[&](){sprintf(Output.p_sPrintStr,"Nagative neutron energy in NEUTRON card.\n");},2);
			}

			///////////////////// check and assign parameters ///////////////////
			int count = 0;
			int index = -1;
			for(int i = 1 ; i <=6; ++i)
			{
				if(Paras[i].size() > 0)
				{
					++count;
					index = i;
				}
			}
			if(count > 1 || index < 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Initial source is redefined.\n");}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

			cFixedSource.p_nFixedSrcType = index;

			if(index == 1)
			{
				Output.CheckInputParas(Paras[index].size()%3 == 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for point initial source.\n");},2);
			}

			cFixedSource.p_vFixedSrcPara = Paras[index];

			
		}
	
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in FIXEDSOURCE block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	return;
}