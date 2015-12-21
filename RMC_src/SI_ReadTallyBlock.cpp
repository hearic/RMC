# include "Input.h"

void CDInput::SI_ReadTallyBlock(CDTally &cTally,CDGeometry &cGeometry,CDMaterial &cMaterial)
{

	bool bIsBlockEnd = false;

	////////////////// check redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["ACCETALLY"] = false;
	for(;;)
	{
		if(bIsBlockEnd){ break; }
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if (nextchar=='%'||nNext_p==4)
		//if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}
		CDFileIO::SkipSpecChar(p_fpInputFilePtr, '$');
		string InputCardStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);
		CDFileIO::SkipSpecChar(p_fpInputFilePtr, ':');
		CDFileIO::SkipSpecChar(p_fpInputFilePtr, ':');

		if(InputCardStr.compare("CELLTALLY")==0)  
		{
			SI_ReadCellTallyCard(bIsBlockEnd,cTally.p_vCellTally,cTally.p_bIsWithCellTally,cGeometry);
		}
		else if(InputCardStr.compare("MESHTALLY")==0)  
		{
			SI_ReadMeshTallyCard(bIsBlockEnd,cTally.p_vMeshTally,cTally.p_bIsWithMeshTally );
		}
		else if(InputCardStr.compare("CSTALLY")==0)  
		{
			SI_ReadCsTallyCard(bIsBlockEnd,cTally.p_vOneGroupCsTally,cTally.p_bIsWithCsTally,cGeometry,cMaterial);
		}
		else if(InputCardStr.compare("ACCETALLY")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in TALLY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//////////////////////// read POWERITER card /////////////////////////
			vector<string> vOptions(2);
			vector<int> vParaNum(2);
			vector<vector<double> > vParas;
			vOptions[0] = "MAP" ;  vParaNum[0] = 1 ;
			vOptions[1] = "UNION"; vParaNum[1] = 1 ;
			SI_ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			if(vParas[0].size() > 0)
			{
				int temp = ExtratIntPara(vParas[0][0], "MAP");
				Output.CheckInputParas(temp == 0 || temp == 1 || temp == 2,
					[&](){sprintf(Output.p_sPrintStr,"incorrect MAP parameters in ACCETALLY card.\n");},2);
				cTally.p_bUseTallyCellMap = (temp >= 1);
				cTally.p_bUseTallyCellBinMap = (temp == 2);
			}
			if(vParas[1].size() > 0)
			{
				int temp = ExtratIntPara(vParas[1][0],"UNION");
				Output.CheckInputParas(temp == 0 || temp == 1,
					[&](){sprintf(Output.p_sPrintStr,"incorrect UNION parameters in ACCETALLY card.\n");},2);
				cTally.p_bUseUnionCsTally = (temp == 1);
			}
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in Tally block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	return;
}