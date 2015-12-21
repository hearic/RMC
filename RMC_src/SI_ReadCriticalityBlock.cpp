# include "Input.h"

void CDInput::SI_ReadCriticalityBlock(CDCriticality &cC_Criticality)
{

	bool bIsBlockEnd = false;

	////////////////// check redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["POWERITER"] = false;
	DefinedCard["INITSRC"] = false;
	DefinedCard["RNG"] = false;
	DefinedCard["PARALLELBANK"] = false;

	for(;;)
	{
		if(bIsBlockEnd){ break; }

		char nextchar;
		//if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		//{
		//	break;
		//}
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar); 
		if(nextchar == '%'||nNext_p==4)//  current universe is end
		{
			break;
		}

		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'$'); 

		string InputCardStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);

		if(InputCardStr.compare("POWERITER")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CRITICALITY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//////////////////////// read POWERITER card /////////////////////////
			vector<string> vOptions(3);
			vector<int> vParaNum(3);
			vector<vector<double> > vParas;
			vOptions[0] = "KEFF0" ;     vParaNum[0] = 1 ;
			vOptions[1] = "POPULATION"; vParaNum[1] = 3 ;
			vOptions[2] = "BATCH"; vParaNum[2] = 1 ;
			SI_ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			if(vParas[0].size() > 0)
			{
				cC_Criticality.p_dKeffFinal = vParas[0][0];
				Output.CheckInputParas(cC_Criticality.p_dKeffFinal > 0.1 && cC_Criticality.p_dKeffFinal < 10.0 ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect KEFF0 parameters in POWERITER card.\n");},2);
			}

			Output.CheckInputParas(vParas[1].size() > 0,[&](){sprintf(Output.p_sPrintStr,"POPULATION is undefined in POWERITER card.\n");},2);

			cC_Criticality.p_nNeuNumPerCyc = ExtratIntPara(vParas[1][0],"POPULATION");
			cC_Criticality.p_nInactCycNum = ExtratIntPara(vParas[1][1],"POPULATION");
			cC_Criticality.p_nTotCycNum = ExtratIntPara(vParas[1][2],"POPULATION");
			cC_Criticality.p_nActCycNum = cC_Criticality.p_nTotCycNum - cC_Criticality.p_nInactCycNum;

			Output.CheckInputParas(cC_Criticality.p_nNeuNumPerCyc > 0 &&  cC_Criticality.p_nInactCycNum >= 0 && cC_Criticality.p_nTotCycNum > cC_Criticality.p_nInactCycNum,
				[&](){sprintf(Output.p_sPrintStr,"incorrect POPULATION parameters in POWERITER card.\n");},2);
			Output.CheckInputParas(cC_Criticality.p_nActCycNum > 3,[&](){sprintf(Output.p_sPrintStr,"too few active cycles.\n");},2);

			if(vParas[2].size() > 0)
			{
				cC_Criticality.p_nBatchNum = 0;
				cC_Criticality.p_nBatchNum = ExtratIntPara(vParas[2][0],"BATCH");
				
				Output.CheckInputParas(cC_Criticality.p_nBatchNum > 0 && cC_Criticality.p_nActCycNum % cC_Criticality.p_nBatchNum == 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect BATCH parameters in POWERITER card.\n");},2);

				cC_Criticality.p_nBatchLen = cC_Criticality.p_nActCycNum/cC_Criticality.p_nBatchNum;

				cC_Criticality.p_bUseBatch = true;
			}

		}
		else if(InputCardStr.compare("INITSRC")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CRITICALITY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			////////////////////////// read InitSrc card ////////////////////////////
			vector<string> vOptions(6);
			vector<int> vParaNum(6);
			vector<vector<double> > vParas;
			vOptions[0] = "POINT" ;    vParaNum[0] = -1 ;
			vOptions[1] = "SLAB";      vParaNum[1] = 6 ;
			vOptions[2] = "SPH";       vParaNum[2] = 4 ;
			vOptions[3] = "CYL/X";     vParaNum[3] = 5 ;
			vOptions[4] = "CYL/Y";     vParaNum[4] = 5 ;
			vOptions[5] = "CYL/Z";     vParaNum[5] = 5 ;

			SI_ReadCardOptions(string("InitSrc"),vOptions,vParaNum,vParas,bIsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			int nCount = 0;
			int nIndex = -1;
			for(int i = 0 ; i < vParas.size(); ++i)
			{
				if(vParas[i].size() > 0)
				{
					++nCount;
					nIndex = i;
				}
			}
			if(nCount > 1 || nIndex < 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Initial source is redefined.\n");}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

			cC_Criticality.p_nKsrcType = nIndex + 1;
			if(nIndex == 0)
			{
				Output.CheckInputParas(vParas[0].size()%3 == 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for point initial source.\n");},2);
			}
			cC_Criticality.p_vKsrcPara = vParas[nIndex];
		}
		else if(InputCardStr.compare("RNG")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CRITICALITY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			////////////////////////// read RNG card ////////////////////////////
			vector<string> vOptions(3);
			vector<int> vParaNum(3);
			vector<vector<double> > vParas;
			vOptions[0] = "TYPE" ;    vParaNum[0] = 1 ;
			vOptions[1] = "SEED";     vParaNum[1] = 1 ;
			vOptions[2] = "STRIDE";   vParaNum[2] = 1 ;
			SI_ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			if(vParas[0].size() > 0)
			{
				int nRandType = ExtratIntPara(vParas[0][0],"RNG");
				Output.CheckInputParas(nRandType >= 1 &&  nRandType <= 3 ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect TYPE parameters in RNG card.\n");},2);
				ORNG.SetRngParas(nRandType);
			}
			if(vParas[1].size() > 0)
			{
				long long llSeed_temp = ExtratLongPara(vParas[1][0],"RNG");
				Output.CheckInputParas(llSeed_temp > 0 && llSeed_temp % 2 == 1 ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect SEED parameters in RNG card. It should be positive odd.\n");},2);
				ORNG.RN_SEED0 = unsigned(llSeed_temp);
			}
			if(vParas[2].size() > 0)
			{
				long long llStride_temp = ExtratLongPara(vParas[2][0],"RNG");
				Output.CheckInputParas(llStride_temp > 200 ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect STRIDE parameters in RNG card. Stride should be > 50.\n");},2);

				ORNG.RN_STRIDE = unsigned(llStride_temp);
			}

		}
		else if(InputCardStr.compare("PARALLELBANK")==0)  
		{
		
			string SkipStr ;
			CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,SkipStr);//skip "flag"
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CRITICALITY block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			int nRead_i = -1;
			fscanf(p_fpInputFilePtr,"%d",&nRead_i);
			Output.CheckInputParas(nRead_i == 0 || nRead_i == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters in PARALLELBANK card.\n");},2);
# ifdef _IS_PARALLEL_
			OParallel.p_bUseFastBank = (nRead_i == 1);
#endif
		}
		//else if(InputCardStr.compare("CUTOFF")==0)    // Weight and Energy cutoff
		//{

		//}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in CRITICALITY block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}


	return;

}