# include "Input.h"

void CDInput::ReadConvergenceBlock(CDConvergence &cConvergence,int nINeuNumPerCyc)
{
	////////////////////// avoid redefinition ////////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["SEMESH"] = false;
	DefinedCard["ACCEFSC"] = false;

	bool bIsBlockEnd = false;
	for(;;)
	{
		char nextchar;
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}

		string InputCardStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);

		if(InputCardStr.compare("SEMESH")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CONVERGENCE block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cConvergence.p_bUseFscDiag = true;

			//auto &mesh = CConvergence.ShanEntropy.SeMesh;
			//CDMesh &OMesh =cConvergence.p_OShanEntropy.p_OSeMesh;
			ReadMeshPara(cConvergence.p_OShanEntropy.p_OSeMesh,"Shannon Entropy",bIsBlockEnd);

			if(bIsBlockEnd){ break; }
		}
		
		else if(InputCardStr.compare("FMMESH")==0)   //// allow multiple definition of fission matrix
		{
			DefinedCard[InputCardStr] = true;
			cConvergence.p_bUseDiagFm = true;

			int size = cConvergence.p_vDiagFisMatrix.size();
			cConvergence.p_vDiagFisMatrix.resize(size + 1);
			//CDMesh &mesh = cConvergence.p_vDiagFisMatrix[size].p_OFmMesh;
			//auto &mesh = CConvergence.DiagFisMatrix[size].FmMesh;
			ReadMeshPara(cConvergence.p_vDiagFisMatrix[size].p_OFmMesh,"Fission Matrix",bIsBlockEnd);

			if(bIsBlockEnd){ break; }
		}
		else if(InputCardStr.compare("NPMMESH")==0)  // calculate dominance ratio
		{
			DefinedCard[InputCardStr] = true;
			cConvergence.p_bUseDiagNpm = true;

			int size = cConvergence.p_vNpMatrix.size();
			cConvergence.p_vNpMatrix.resize(size + 1);
			//CDMesh  &mesh = cConvergence.p_vNpMatrix[size].p_ONpMesh;
			//auto &mesh = CConvergence.NpMatrix[size].NpMesh;
			ReadMeshPara(cConvergence.p_vNpMatrix[size].p_ONpMesh,"Noise Propagation Matrix",bIsBlockEnd);

			if(bIsBlockEnd){ break; }
		}
		else if(InputCardStr.compare("ACCEFSC")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in CONVERGENCE block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cConvergence.p_nFscAcceMethod = 2;  // default option
			cConvergence.p_bUseFscAcce = true;
			//////////////////////// read ACCEFSC card //////////////////////////
			vector<string> vOptions(3);
			vector<int> vParaNum(3);
			vector<vector<double> > vParas;
			vOptions[0] = "METHOD" ;    vParaNum[0] = 1 ;
			vOptions[1] = "AUTOFACTOR"; vParaNum[1] = 1 ;
			vOptions[2] = "FACTOR";     vParaNum[2] = -1 ;
			ReadCardOptions(InputCardStr,vOptions,vParaNum,vParas,bIsBlockEnd);
			///////////////////// check and assign parameters ///////////////////
			if(vParas[0].size() > 0)  //// METHOD
			{
				cConvergence.p_nFscAcceMethod = ExtratIntPara(vParas[0][0],"METHOD");
				Output.CheckInputParas(cConvergence.p_nFscAcceMethod == 1  ||  cConvergence.p_nFscAcceMethod == 2,
					[&](){sprintf(Output.p_sPrintStr,"incorrect METHOD parameters in ACCEFSC card.\n");},2);
			}

			Output.CheckInputParas(!(vParas[1].size() > 0  && vParas[2].size() > 0),
				[&](){sprintf(Output.p_sPrintStr,"FACTOR/AUTOFACTOR parameters conflict in ACCEFSC card.\n");},2);

			Output.CheckInputParas(!(vParas[1].size() == 0  && vParas[2].size() == 0),
				[&](){sprintf(Output.p_sPrintStr,"undefined FACTOR/AUTOFACTOR parameters in ACCEFSC card.\n");},2);


			vector<double> vFactor;
			vector<int> vCycle_bin;
			int nFactor_num = 0;
			if(vParas[1].size() > 0)  //// AUTOFACTOR
			{
				int inactive_cycle = ExtratIntPara(vParas[1][0],"AUTOFACOR");
				if(inactive_cycle < 30)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"convergence acceleration is closed with inactive cycles < 30.\n");},
						Output.p_fpOutputFilePtr, CDOutput::_WARNING);
					cConvergence.p_bUseFscAcce = false;
					cConvergence.p_nFscAcceMethod = -1;
					return;
				}
				nFactor_num = 4;
				vFactor.resize(nFactor_num);
				vCycle_bin.resize(nFactor_num);
				vFactor[0] = 16;     vFactor[1] = 8;      vFactor[2] = 4;      vFactor[3] = 2;    
				if((inactive_cycle <= 150))
				{
					vCycle_bin[0] = 5;
				}
				else
				{
					vCycle_bin[0] = (inactive_cycle - 70)/16 + 1;
					if(vCycle_bin[0] % 5 != 0){	vCycle_bin[0] = (vCycle_bin[0]/5 + 1)*5;	}
				}

				for(int i = 1 ; i < nFactor_num ; ++i)
				{
					vCycle_bin[i] = vCycle_bin[i-1] + 5;
				}
			}
			else if(vParas[2].size() > 0)  //// FACTOR
			{
				nFactor_num = vParas[2].size()/2;
				Output.CheckInputParas(nFactor_num >= 1 && vParas[2].size() % 2 == 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect FACTOR parameters in ACCEFSC card.\n");},2);

				vFactor.resize(nFactor_num);
				vCycle_bin.resize(nFactor_num);
				vFactor[0] = vParas[2][0];
				vCycle_bin[0] = ExtratIntPara(vParas[2][1],"FACTOR");
				for(int i = 1 ; i < nFactor_num ; ++i)
				{
					vFactor[i] = vParas[2][2*i] ;
					vCycle_bin[i] = vCycle_bin[i-1] + ExtratIntPara(vParas[2][2*i+1],"FACTOR");
				}
			}

			////////////////////check factor and cycle bin //////////////////////
			for(int i = 0 ; i < nFactor_num ; ++i)
			{
				Output.CheckInputParas(vFactor[i] >= 1  && vCycle_bin[i] > 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect FACTOR parameters in ACCEFSC card.\n");},2);
			}
			for(int i = 1 ; i < nFactor_num ; ++i)
			{
				Output.CheckInputParas(vFactor[i] < vFactor[i-1] && vCycle_bin[i] > vCycle_bin[i-1],
					[&](){sprintf(Output.p_sPrintStr,"incorrect FACTOR parameters in ACCEFSC card.\n");},2);
			}
			//int neu_per_cycle = C_Criticality.NeuNumPerCyc/int(factor[0]);
			int nNeu_per_cycle = nINeuNumPerCyc/int(vFactor[0]);
			Output.CheckInputParas(nNeu_per_cycle > 100,
				[&](){sprintf(Output.p_sPrintStr,"particles per cycle may be too few for convergence acceleration.\n");},1);


			////////////////////setup factor and cycle bin //////////////////////
			if(cConvergence.p_nFscAcceMethod == 1)
			{
				cConvergence.p_vWldtFactorList = vFactor;
				cConvergence.p_vWldtFactorCycleBin = vCycle_bin;
				cConvergence.p_dWldtFactor = cConvergence.p_vWldtFactorList[0];
			}
			else if(cConvergence.p_nFscAcceMethod == 2)
			{
				cConvergence.p_vSuphisFactorList.resize(nFactor_num);
				for(int i = 0 ; i < nFactor_num ; ++i)
				{
					cConvergence.p_vSuphisFactorList[i] = ExtratIntPara(vFactor[i],"FACTOR");
				}
				cConvergence.p_vSuphisFactorCycleBin = vCycle_bin;
				cConvergence.p_nSuphisFactor = cConvergence.p_vSuphisFactorList[0];
			}

			if(bIsBlockEnd){ break; }
		}
		//else if(InputCardStr.compare("DIAGFSC")==0)  
		//{
		//	////////////////////// check redefinition ////////////////////////////
		//	Output.CheckInputParas(!DefinedCard[InputCardStr],
		//		[&](){sprintf(Output.PrintStr,"%s card is redefined in CONVERGENCE block.\n",InputCardStr.c_str());},2);
		//	DefinedCard[InputCardStr] = true;

		//	//////////////////////// read DIAGFSC card //////////////////////////
		//	vector<string> Options(1);
		//	vector<int> ParaNum(1);
		//	vector<vector<double> > Paras;
		//	Options[0] = "METHOD" ;    ParaNum[0] = 1 ;
		//	ReadCardOptions(InputCardStr,Options,ParaNum,Paras,IsBlockEnd);
		//	///////////////////// check and assign parameters ///////////////////
		//	if(Paras[0].size() > 0)  //// METHOD
		//	{
		//		Convergence.FscDiagMethod = ExtratIntPara(Paras[0][0],"METHOD");
		//		Output.CheckInputParas(Convergence.FscDiagMethod == 1,
		//			[&](){sprintf(Output.PrintStr,"incorrect METHOD parameters in DIAGFSC card.\n");},2);
		//	}
		//}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in Tally block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}


	Output.CheckInputParas(DefinedCard["SEMESH"],
		[&](){sprintf(Output.p_sPrintStr,"SEMESH card is required in CONVERGENCE block.\n");},2);

	return;
}