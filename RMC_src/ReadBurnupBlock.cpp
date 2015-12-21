# include "Input.h"

void CDInput::ReadBurnupBlock(CDBurnup &cBurnup,bool &bIsWithCsTally,CDGeometry &cGeometry)
{
	//Burnup.IsWithBurnup = true;
	//Tally.IsWithCsTally = true ;
	cBurnup.p_bIsWithBurnup = true;
	bIsWithCsTally = true ;

	////////////////// check redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["BURNCELL"] = false;
	DefinedCard["TIMESTEP"] = false;
	DefinedCard["POWER"] = false;
	DefinedCard["ACELIB"] = false;
	DefinedCard["DEPTHLIB"] = false;
	DefinedCard["SUBSTEP"] = false;
	DefinedCard["INHERENT"] = false;
	DefinedCard["STRATEGY"] = false;
	DefinedCard["SOLVER"] = false;
	DefinedCard["PARALLEL"] = false;
	DefinedCard["OUTPUTCELL"] = false;
	DefinedCard["IMPNUC"] = false;
	DefinedCard["XEEQ"] = false;
	DefinedCard["NAECF"] = false;
	DefinedCard["VARYMAT"] = false;
	//DefinedCard["OUTPUTNUC"] = false;   //// not prepared yet


	for(;;)
	{
		char nextchar;
		if(CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) // current MODULE finished
		{
			break;
		}   
		string InputCardStr ; 
		CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);
		//////////////////////////////   Burnup cells   ////////////////////////////////
		/////// Important note: Burnup cells are in the form of simple cell. 
		///////               They will be automatically expanded  
		if(InputCardStr.compare("BURNCELL")==0)   
		{	
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,cBurnup.p_vBurnableCell,"%d");

			//// check existence/validation of burnable cells(Bug?)
			for(int i = 0; i < cBurnup.p_vBurnableCell.size(); ++i)
			{
				int nBurnableCellU = cBurnup.p_vBurnableCell[i];
				Output.CheckInputParas(nBurnableCellU > 0 && nBurnableCellU <= cGeometry.p_OCellIndex.GetIndexUNum(),
					[&](){sprintf(Output.p_sPrintStr,"invalid burnable cell %d in %s card defined in BURNUP block.\n",nBurnableCellU, InputCardStr.c_str());},CDOutput::_ERROR);
				int nBurnableCell = cGeometry.p_OCellIndex.GetIndex(nBurnableCellU);
				Output.CheckInputParas(nBurnableCell > 0,
					[&](){sprintf(Output.p_sPrintStr,"invalid burnable cell %d in %s card defined in BURNUP block.\n",nBurnableCellU, InputCardStr.c_str());},CDOutput::_ERROR);
				Output.CheckInputParas(cGeometry.p_vCell[nBurnableCell].p_nImp != 0,
					[&](){sprintf(Output.p_sPrintStr,"invalid burnable cell %d with 0 importance in %s card defined in BURNUP block.\n",nBurnableCellU, InputCardStr.c_str());},CDOutput::_ERROR);
				//check if Vol is set, print warning if vol =1.0;
				Output.CheckInputParas(cGeometry.p_vCell[nBurnableCell].p_dVol != 1.0,
					[&](){sprintf(Output.p_sPrintStr,"[!] volume of burnable cell %d is not input(using default value 1.0 cm^3). Actual volume are highly suggested for burnup calculation.\n",nBurnableCellU);},CDOutput::_WARNING);
			}

			if(nNext_p >= 3)   ////current block is end
			{
				break;
			}
		}

		///////////////////////////   Burnup Time  ////////////////////////////////
		else if(InputCardStr.compare("TIMESTEP")==0)   
		{	
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_vStepBurnTime.resize(1);
			cBurnup.p_vStepBurnTime[0] = 0;
			int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,cBurnup.p_vStepBurnTime,"%lf");

			for(int i = 1; i < cBurnup.p_vStepBurnTime.size(); ++i)
			{
				Output.CheckInputParas(cBurnup.p_vStepBurnTime[i] > 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
			}
			if(nNext_p >= 3)   ////current block is end
			{
				break;
			}
			//TotBurnStep = i;
		}

		///////////////////////////   Burnup Power  ////////////////////////////////
		else if(InputCardStr.compare("POWER")==0)   
		{	
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_vStepBurnPowerDen.resize(1);
			cBurnup.p_vStepBurnPowerDen[0] = 0;
			int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,cBurnup.p_vStepBurnPowerDen,"%lf");
			for(int i = 1; i < cBurnup.p_vStepBurnPowerDen.size(); ++i)
			{
				Output.CheckInputParas(cBurnup.p_vStepBurnPowerDen[i] >= 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
			}
			if(nNext_p >= 3)   ////current block is end
			{
				break;
			}
		}

		////////////////////////// temperature - ace library /////////////////////////////
		else if(InputCardStr.compare("ACELIB")==0)   
		{			
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			fscanf(p_fpInputFilePtr,"%s",cBurnup.p_chAceLibForBurn);

		}

		////////////////////////// Depletion library Depthlib /////////////////////////////
		else if(InputCardStr.compare("DEPTHLIB")==0)   
		{			
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			fscanf(p_fpInputFilePtr,"%s",cBurnup.p_chDepthLibForBurn);
		}

		///////////////////////// other input card /////////////////////////////
		else if(InputCardStr.compare("SUBSTEP")==0)    ///// inner step of point depletion calculation
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_nInnerStepNum = -1;
			fscanf(p_fpInputFilePtr,"%d",&cBurnup.p_nInnerStepNum);
			Output.CheckInputParas(cBurnup.p_nInnerStepNum > 0  &&  cBurnup.p_nInnerStepNum < 10000,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
		}
		else if(InputCardStr.compare("INHERENT")==0)  ///// absorption fraction of inherent important nuclides
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//Burnup.AbsorpFracCutoff = -1;
			//Burnup.NucDenFracCutoff = -1;
			fscanf(p_fpInputFilePtr,"%lf %lf",&cBurnup.p_dAbsorpFracCutoff,&cBurnup.p_dNucDenFracCutoff);
			Output.CheckInputParas(cBurnup.p_dAbsorpFracCutoff > 0.9  &&  cBurnup.p_dAbsorpFracCutoff < 1.0 
				&& cBurnup.p_dNucDenFracCutoff > 0.9  &&  cBurnup.p_dNucDenFracCutoff < 1.0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
		}
		else if(InputCardStr.compare("IMPNUC")==0)  ///// absorption fraction of inherent important nuclides
		{
            ////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			vector<int> vImp_nuc_id;
			vImp_nuc_id.resize(0);
			int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,vImp_nuc_id,"%d");

			for(int i = 0; i < vImp_nuc_id.size(); ++i)
			{
				Output.CheckInputParas(vImp_nuc_id[i] > 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);

				auto p = find(cBurnup.p_vImpNucId.begin(),cBurnup.p_vImpNucId.end(),vImp_nuc_id[i]);
				if(p == cBurnup.p_vImpNucId.end())
				{
					cBurnup.p_vImpNucId.push_back(vImp_nuc_id[i]);
				}
			}

			if(nNext_p >= 3)   ////current block is end
			{
				break;
			}
		}

		else if(InputCardStr.compare("STRATEGY")==0) ///// Burnup strategy  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_nBurnStepStrategy = -1;
			fscanf(p_fpInputFilePtr,"%d",&cBurnup.p_nBurnStepStrategy);
			Output.CheckInputParas(cBurnup.p_nBurnStepStrategy == 0  ||  cBurnup.p_nBurnStepStrategy == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);

			//Burnup.BurnStepStrategy = 0 ;  //// 2013-03-22
		}
		else if(InputCardStr.compare("SOLVER")==0)  ///// Point depletion solver
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_nBurnSolver = -1;
			fscanf(p_fpInputFilePtr,"%d",&cBurnup.p_nBurnSolver);
			Output.CheckInputParas(cBurnup.p_nBurnSolver > 0  &&  cBurnup.p_nBurnSolver < 5,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
		}
		else if(InputCardStr.compare("PARALLEL")==0) ///// parallel burnup
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			int nRead_i = -1;
			fscanf(p_fpInputFilePtr,"%d",&nRead_i);
			Output.CheckInputParas(nRead_i == 0  ||  nRead_i == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
# ifdef _IS_PARALLEL_
			OParallel.p_bIsParallelBurn = (nRead_i == 1);
# endif
		}

		///////////////////////////   varying mat during burnup  ///////////////////////////////
		else if(InputCardStr.compare("VARYMAT")==0)   
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			//////////////////////// read VaryMat card /////////////////////////
			bool bIsBlockEnd;
			bool bIsReadSuccess = ReadBurnVaryMat(cBurnup.p_vBurnVaryMat,bIsBlockEnd);
			Output.CheckInputParas(bIsReadSuccess,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);
			if(bIsBlockEnd)
			{
				break;
			}

		}

		else if(InputCardStr.compare("OUTPUTCELL")==0) ///// 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			bool bIsBlockEnd,bIsCardEnd;

			bool bIsReadSuccess = ReadTallyCells(cBurnup.p_vCellsForOutput,bIsBlockEnd,bIsCardEnd,cGeometry);
			Output.CheckInputParas(bIsReadSuccess,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);

			if(bIsBlockEnd)
			{
				break;
			}
		}
		else if(InputCardStr.compare("XEEQ")==0)  ///// Xenon equilibrium
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;
		
			cBurnup.p_dXeEqFactor = -1;
			fscanf(p_fpInputFilePtr,"%lf",&cBurnup.p_dXeEqFactor);
			Output.CheckInputParas(cBurnup.p_dXeEqFactor >= 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect %s card defined in BURNUP block.\n",InputCardStr.c_str());},2);

			cBurnup.p_nXeEqFlag = 1;
		}
		else if(InputCardStr.compare("NAECF")==0)  ///// neutron average energy causing fission for calculating fission yield
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[InputCardStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in BURNUP block.\n",InputCardStr.c_str());},2);
			DefinedCard[InputCardStr] = true;

			cBurnup.p_dNAECFInput = 0;// default
			fscanf(p_fpInputFilePtr,"%lf",&cBurnup.p_dNAECFInput);
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in BURNUP block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

	}

	//////////////////////// check necessary card for burnup blcok ///////////////////////
	Output.CheckInputParas(DefinedCard["BURNCELL"],
		[&](){sprintf(Output.p_sPrintStr,"BURNCELL card is not defined in BURNUP block.\n");},2);

	Output.CheckInputParas(DefinedCard["TIMESTEP"],
		[&](){sprintf(Output.p_sPrintStr,"TIMESTEP card is not defined in BURNUP block.\n");},2);

	Output.CheckInputParas(DefinedCard["POWER"],
		[&](){sprintf(Output.p_sPrintStr,"POWER card is not defined in BURNUP block.\n");},2);

	Output.CheckInputParas(DefinedCard["ACELIB"],
		[&](){sprintf(Output.p_sPrintStr,"ACELIB card is not defined in BURNUP block.\n");},2);

	//////////////////////// check inconsistency for burnup block ///////////////////////
	int nTot_burn_step = cBurnup.p_vStepBurnTime.size() - 1;
	int nTot_power_step = cBurnup.p_vStepBurnPowerDen.size() - 1;
	Output.CheckInputParas(nTot_power_step == nTot_burn_step && nTot_burn_step > 0,
		[&](){sprintf(Output.p_sPrintStr,"inconsistent TIMESTEP/POWER parameters in BURNUP block.\n");},2);


	////////////////////   Cal total depletion of each step  ///////////////////////////
	cBurnup.p_vStepCmltvBurnTime.resize(nTot_burn_step + 1);
	cBurnup.p_vStepCmltvBurnup.resize(nTot_burn_step + 1);
	cBurnup.p_vStepCmltvBurnTime[1] = cBurnup.p_vStepBurnTime[1];
	cBurnup.p_vStepCmltvBurnup[1] = cBurnup.p_vStepBurnTime[1]*cBurnup.p_vStepBurnPowerDen[1]*1.0E-3;
	for(int i = 2;i <= nTot_burn_step; ++i)
	{
		cBurnup.p_vStepCmltvBurnTime[i] = cBurnup.p_vStepCmltvBurnTime[i-1] + cBurnup.p_vStepBurnTime[i];
		cBurnup.p_vStepCmltvBurnup[i] = cBurnup.p_vStepCmltvBurnup[i-1] + cBurnup.p_vStepBurnTime[i]*cBurnup.p_vStepBurnPowerDen[i]*1.0E-3; // Wd/gHM -> Mwd/KgHM
	}

	cBurnup.p_vStepFinalKeff.resize(nTot_burn_step + 1);
	cBurnup.p_nTotBurnStep = nTot_burn_step ;
	return;
}