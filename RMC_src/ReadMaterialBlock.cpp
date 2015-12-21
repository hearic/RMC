# include "Input.h"
# include <numeric>

void CDInput::ReadMaterialBlock(CDMaterial &cMaterial,CDAceData &cAceData)
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
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}

		string InputCardStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);

		if(InputCardStr.compare("MAT")==0)  
		{
			ReadMatCard(bIsBlockEnd,cMaterial);
		}
		else if(InputCardStr.compare("SAB")==0)  
		{
			ReadSabCard(bIsBlockEnd,cMaterial);
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
			ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

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
			vector<string> vOptions(3);
			vector<int> vParaNum(3);
			vOptions[0] = "ERGGRP" ; vParaNum[0] = 1;
			vOptions[1] = "BETA";    vParaNum[1] = 6;
			vOptions[2] = "LAMBDA";  vParaNum[2] = 6;
			vector<vector<double> > vParas;
			ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			//////////////////////// check and assign parameters ///////////////////////
			if(vParas[0].size() > 0)
			{
				int temp = ExtratIntPara(vParas[0][0],"ERGGRP");
				Output.CheckInputParas(temp > 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect ERGGRP parameters in MGACE card.\n");},2);
				cAceData.p_bIsMultiGroup = true;
				cAceData.p_nMltGrpNum = temp;
			}	

			if(vParas[1].size()>0)
			{
				Output.CheckInputParas(vParas[1].size()==6,
					[&](){sprintf(Output.p_sPrintStr,"incorrect BETA parameters in MGACE card.\n");},2);
				for(int i=1;i<=6;i++) cAceData.p_vBeta[i] = vParas[1][i-1];
				cAceData.p_vBeta[0] = 1- std::accumulate(&cAceData.p_vBeta[1],&cAceData.p_vBeta[7],0.0);
			}

			if(vParas[2].size()>0)
			{
				Output.CheckInputParas(vParas[2].size()==6,
					[&](){sprintf(Output.p_sPrintStr,"incorrect LAMBDA parameters in MGACE card.\n");},2);
				for(int i=1;i<=6;i++) cAceData.p_vLambda[i] = vParas[2][i-1]*1e-8;
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
		[&](){sprintf(Output.p_sPrintStr,"CEACE/MGACE card conflict in CRITICALITY block.\n");},2);


	if(cMaterial.p_vMatSet.size() == 0 )
	{		
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"no material defined.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

}