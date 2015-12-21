# include "Input.h"

void CDInput::SI_ReadInuptBlocks(CDCalMode &cCalMode,CDGeometry &cGeometry,CDMaterial &cMaterial,
	CDAceData &cAceData,CDCriticality &cC_Criticality,CDTally &cTally,CDConvergence &cConvergence,CDBurnup &cBurnup,CDPlot &cPlot, CDFixedSource &cFixedSource)
{
	////////////////////////// Read input file: start ////////////////////////////////

	char nextchar,key_word_read[200];


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Reading input file...\n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	p_fpInputFilePtr = fopen(p_chInputFileName,"rb");

	///////////////////////// read input blocks by key words //////////////////////////////
	for(;;)
	{
		//////////////////  read block key word  ///////////////////
		if(CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) == 4 )
		{
			break;
		}
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'%'); 
		int nFileEndFlag = fscanf(p_fpInputFilePtr,"%s",key_word_read);
		if(nFileEndFlag == -1)    //File end
		{
			break;
		}  
		else if(nFileEndFlag == 0) //blank line
		{
			continue;
		}   
		//string sBlockKeyWordfirst = key_word_read;
		//string sBlockKeyWord = sBlockKeyWordfirst.erase(0,1);
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
		if(sBlockKeyWord.compare("GEOMETRY")==0) 
		//if(sBlockKeyWord.compare("UNIVERSE")==0)    
		{
			BlockDefined[sBlockKeyWord] = true;
			SI_ReadUniverseBlock(cGeometry);
		}

		///////////////////////// surface block ///////////////////////////
		else if(sBlockKeyWord.compare("SURFACE")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			SI_ReadSurfaceBlock(cGeometry.p_OSurfaceIndex, cGeometry.p_vSurface);
		}

		/////////////////////// Material block ////////////////////////////
		else if(sBlockKeyWord.compare("MATERIAL")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			//ReadMaterialBlock(Material.MatSet,Material.MatSetIndex,Material.MatSet,Material.MatSetIndex);
			SI_ReadMaterialBlock(cMaterial,cAceData);
		}

		/////////////////////// criticality block ///////////////////////////
		else if(sBlockKeyWord.compare("CRITICALITY")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.CriticalMode ;

			//ReadCriticalityBlock(CC_Criticality,CRNG,Parallel.UseFastBank);
			SI_ReadCriticalityBlock(cC_Criticality);
		}	

		//////////////////////////// Tally block //////////////////////////
		else if(sBlockKeyWord.compare("TALLY")==0)    //Flux and Power tally
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			SI_ReadTallyBlock(cTally,cGeometry,cMaterial);
		}

		///////////////////////// convergence block ////////////////////////
		else if(sBlockKeyWord.compare("CONVERGENCE")==0)  
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			SI_ReadConvergenceBlock(cConvergence,cC_Criticality.p_nNeuNumPerCyc);
		}

		///////////////////////////  BURNUP block /////////////////////////
		else if(sBlockKeyWord.compare("BURNUP")==0)    
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			cCalMode.p_nRmcCalcMode = cCalMode.BurnupMode ;
			SI_ReadBurnupBlock(cBurnup,cTally.p_bIsWithCsTally,cGeometry);
		}

		//////////////////////////// print block ///////////////////////////
		else if(sBlockKeyWord.compare("PRINT") == 0)
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			SI_ReadPrintBlock();
		}

		//////////////////////////// plot block ///////////////////////////
		else if(sBlockKeyWord.compare("PLOT") == 0)
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!BlockDefined[sBlockKeyWord],
				[&](){sprintf(Output.p_sPrintStr,"%s block is redefined.\n",sBlockKeyWord.c_str());},2);
			BlockDefined[sBlockKeyWord] = true;

			SI_ReadPlotBlock(cPlot,cCalMode.p_bSkipCalculation);
		}

		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown block keyword %s.\n",sBlockKeyWord.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

	}

	fclose(p_fpInputFilePtr);

	/////////////////////////////  check RMC input blocks /////////////////////////////
	CDInput::CheckInpBlock(cCalMode,cGeometry,cMaterial,cC_Criticality,cFixedSource, cAceData.p_bIsMultiGroup,cTally.p_bIsWithCsTally,cBurnup.p_bIsWithBurnup);

	return;
	////////////////////////// Read input file: finished ////////////////////////////////

}
