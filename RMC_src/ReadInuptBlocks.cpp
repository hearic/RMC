# include "Input.h"


void CDInput::ReadInuptBlocks(CDCalMode &cCalMode,CDGeometry &cGeometry,CDMaterial &cMaterial,
	CDAceData &cAceData,CDCriticality &cCriticality,CDTally &cTally,CDConvergence &cConvergence,CDBurnup &cBurnup,CDPlot &cPlot, CDFixedSource &cFixedSource)
{
	////////////////////////// Read input file: start ////////////////////////////////
	if (p_nInpFileFormat ==0)
	{
		SI_ReadInuptBlocks(cCalMode,cGeometry,cMaterial,cAceData,cCriticality,cTally,cConvergence,cBurnup,cPlot, cFixedSource); 
		return;
	}
	
	char nextchar,key_word_read[200];

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Reading input file...\n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	p_fpInputFilePtr = fopen(p_chInputFileName,"rb");

	///////////////////////// read input blocks by key words //////////////////////////////
	for(;;)
	{
		//////////////////  read block key word  ///////////////////
		CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		int nFileEndFlag = fscanf(p_fpInputFilePtr,"%s",key_word_read);
		if(nFileEndFlag == -1)    //File end
		{
			break;
		}  
		else if(nFileEndFlag == 0) //blank line
		{
			continue;
		}   

		string sBlockKeyWord = key_word_read;
		std::transform(sBlockKeyWord.begin(), sBlockKeyWord.end(),sBlockKeyWord.begin(), ::toupper);


		///////////////// depletion(point burnup) block ///////////////////
		if(sBlockKeyWord.compare("DEPLETION")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.PointBurnMode ;
			fclose(p_fpInputFilePtr);
			break;
		}

		///////////////////////// universe block //////////////////////////
		if(sBlockKeyWord.compare("UNIVERSE")==0)    
		{

			BlockDefined[sBlockKeyWord] = true;
			ReadUniverseBlock(cGeometry);
		}

		///////////////////////// surface block ///////////////////////////
		else if(sBlockKeyWord.compare("SURFACE")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;


			ReadSurfaceBlock(cGeometry.p_OSurfaceIndex, cGeometry.p_vSurface);
		}

		/////////////////////// Material block ////////////////////////////
		else if(sBlockKeyWord.compare("MATERIAL")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			ReadMaterialBlock(cMaterial,cAceData);
		}

		/////////////////////// criticality block ///////////////////////////
		else if(sBlockKeyWord.compare("CRITICALITY")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.CriticalMode ;

			ReadCriticalityBlock(cCriticality);
		}	

		/////////////////////// fixed source block ///////////////////////////
		else if(sBlockKeyWord.compare("FIXEDSOURCE")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.FixedSourceMode ;

			ReadFixedSourceBlock(cFixedSource);
		}	

		/////////////////////// QUASISTATIC_S block ///////////////////////////
		else if(sBlockKeyWord.compare("QUASISTATIC_S")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.QuasiStaticSMode ;

			ReadQuasiStaticSBlock(cCriticality);
		}

		/////////////////////// QUASISTATIC_D block ///////////////////////////
		else if(sBlockKeyWord.compare("QUASISTATIC_D")==0)
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.QuasiStaticDMode;

			ReadQuasiStaticDBlock(cCriticality);
		}

		//////////////////////////// Tally block //////////////////////////
		else if(sBlockKeyWord.compare("TALLY")==0)    //Flux and Power tally
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			ReadTallyBlock(cTally,cGeometry,cMaterial);
		}

		///////////////////////// convergence block ////////////////////////
		else if(sBlockKeyWord.compare("CONVERGENCE")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			ReadConvergenceBlock(cConvergence,cCriticality.p_nNeuNumPerCyc);
		}

		///////////////////////////  BURNUP block /////////////////////////
		else if(sBlockKeyWord.compare("BURNUP")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;
			
			cCalMode.p_nRmcCalcMode = cCalMode.BurnupMode ;

			ReadBurnupBlock(cBurnup,cTally.p_bIsWithCsTally,cGeometry);
		}

		//////////////////////////// print block ///////////////////////////

		else if(sBlockKeyWord.compare("PRINT") == 0)
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			ReadPrintBlock();
		}

		//////////////////////////// plot block ///////////////////////////

		else if(sBlockKeyWord.compare("PLOT") == 0)
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;
			
			ReadPlotBlock(cPlot,cCalMode.p_bSkipCalculation);
		}

		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown block keyword %s.\n",sBlockKeyWord.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

	}

	fclose(p_fpInputFilePtr);

	/////////////////////////////  check RMC input blocks /////////////////////////////
	CDInput::CheckInpBlock(cCalMode,cGeometry,cMaterial,cCriticality, cFixedSource, cAceData.p_bIsMultiGroup,cTally.p_bIsWithCsTally,cBurnup.p_bIsWithBurnup);

	return;
	////////////////////////// Read input file: finished ////////////////////////////////

}
