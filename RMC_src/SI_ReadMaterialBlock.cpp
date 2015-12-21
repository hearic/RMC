# include "Input.h"


void CDInput::SI_ReadMaterialBlock(CDMaterial &cMaterial,CDAceData &cAceData)
{
	bool bIsBlockEnd = false;

	////////////////// check redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["CEACE"] = false;
	DefinedCard["MGACE"] = false;

	for(;;)
	{
		if(bIsBlockEnd){break;}

		char nextchar;
		int nNext_p=CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		//if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) == 4) 
		//{
		//	break;
		//}
		//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);  
		//if(nextchar == '%')// material block finished
		//{
		//	break;
		//}
		if( nNext_p== 4||nextchar == '%')// material block finished
		{
			break;
		}


		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'$'); 

		string InputCardStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);

		if(InputCardStr.compare("MAT")==0)  
		{
			SI_ReadMatCard(bIsBlockEnd,cMaterial);
		}
		else if(InputCardStr.compare("SAB")==0)  
		{
			SI_ReadSabCard(bIsBlockEnd,cMaterial);
		}
		else if(InputCardStr.compare("CEACE")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in MATERIAL block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//////////////////////// read CEACE card /////////////////////////
			vector<string> vOptions(2);
			vOptions[0] = "ERGBINHASH" ;
			vOptions[1] = "PTABLE";
			vector<int> vParaNum(2,1);
			vector<vector<double> > vParas;
			SI_ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			///////////////////// check and assign parameters //////////////////
			if(vParas[0].size() > 0)
			{
				int nTemp = ExtratIntPara(vParas[0][0],"ERGBINHASH");
				Output.CheckInputParas(nTemp == 0 || nTemp ==1,
					[&](){sprintf(Output.p_sPrintStr,"incorrect ERGBINHASH parameters in CEACE card.\n");},2);
				cAceData.p_bUseErgBinMap = (nTemp == 1);
			}
			if(vParas[1].size() > 0)
			{
				int nTemp = ExtratIntPara(vParas[1][0],"PTABLE");
				Output.CheckInputParas(nTemp == 0 || nTemp ==1 ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect PTABLE parameters in CEACE card.\n");},2);
				cAceData.p_bUseProbTable = (nTemp == 1);
			}
		}
		else if(InputCardStr.compare("MGACE")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in MATERIAL block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

            ///////////////////////// read MGACE card options //////////////////////////
			vector<string> vOptions(1);
			vOptions[0] = "ERGGRP" ;
			vector<int> vParaNum(1,1);
			vector<vector<double> > vParas;
			SI_ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			//////////////////////// check and assign parameters ///////////////////////
			if(vParas[0].size() > 0)
			{
				int temp = ExtratIntPara(vParas[0][0],"ERGGRP");
				Output.CheckInputParas(temp > 1,
					[&](){sprintf(Output.p_sPrintStr,"incorrect ERGGRP parameters in MGACE card.\n");},2);
				cAceData.p_bIsMultiGroup = true;
				cAceData.p_nMltGrpNum = temp;
			}	
			
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in material block.\n",InputCardStr.c_str());},
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	///////////////////////////////// check material block ////////////////////////
	Output.CheckInputParas(!(DefinedCard["CEACE"] && DefinedCard["MGACE"]),
		[&](){sprintf(Output.p_sPrintStr,"CEACE/MGACE card conflict in material block.\n");},2);


	if(cMaterial.p_vMatSet.size() == 0 )
	{		
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"no material defined.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

}