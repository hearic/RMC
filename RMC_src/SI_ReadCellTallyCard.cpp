# include "Input.h"

void CDInput::SI_ReadCellTallyCard(bool &bIsBlockEnd,vector<CDCellTally> &vCellTally,bool &bIsWithCellTally,CDGeometry &cGeometry)
{

	bIsBlockEnd = false;
	bool bIsCardEnd = false;
	//auto  &CellTally = Tally.CellTally;
//	vector<CDCellTally>  &vCellTally = vCellTally;
	int nTallySize = vCellTally.size();
	vCellTally.resize(nTallySize + 1) ;

	
	//////////////////////////////// Read cell tally ID ////////////////////////////////
	int nTally_id ;
	Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nTally_id) > 0,
					[&](){sprintf(Output.p_sPrintStr,"unknown cell tally ID.\n");},2);
	vCellTally[nTallySize].p_nTallyID = nTally_id;

	vCellTally[nTallySize].p_nFlagLevel = -1;

	////////////////////////////// Read cell tally options /////////////////////////////
	////////////////// avoid redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["TYPE"] = false;
	DefinedCard["ENERGY"] = false;
	DefinedCard["FILTER"] = false;
	DefinedCard["INTEGRAL"] = false;
	DefinedCard["FLAG"] = false;
	DefinedCard["CELL"] = false;

	for(;;)
	{
		if(bIsBlockEnd || bIsCardEnd){ break; }

		char nextchar;
		int next_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		//if(next_p >= 2)   
		//{
		//	if(next_p >= 3) { bIsBlockEnd = true; }
		//	break;
		//} 
		if(nextchar=='$'||nextchar=='%'||next_p == 4)   
		{
			if(nextchar=='%'||next_p == 4)   { bIsBlockEnd = true; }
			break;
		} 
		
		string KeyWordStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,KeyWordStr);

		if(KeyWordStr.compare("TYPE") == 0) //// read tally type: 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			////  1:flux  2: power 3:fission  4: absorption
			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			int nRead_i = -1;
			fscanf(p_fpInputFilePtr,"%d",&nRead_i);
			Output.CheckInputParas(nRead_i >= 1 && nRead_i <= 5,
					[&](){sprintf(Output.p_sPrintStr,"unknown tally type for cell tally %d.\n",nTally_id);},2);

			vCellTally[nTallySize].p_nTallyType = nRead_i;
		}
		else if(KeyWordStr.compare("ENERGY") == 0) //// Read energy bins 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			///////////// (-,e0),(e0,e1), (e1,e2),...,(en,+) ////////////////////
			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			int nNext_p = CDFileIO::SI_ReadVaryVec(p_fpInputFilePtr,vCellTally[nTallySize].p_vErgBins,"%lf");
			if(nNext_p >= 2)   
			{
				bIsCardEnd = true;
				if(nNext_p >= 3) { bIsBlockEnd = true;}
			}
		}
		else if (KeyWordStr.compare("FILTER")==0) //// Read tally filter
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			int nNext_p = CDFileIO::SI_ReadVaryVec(p_fpInputFilePtr,vCellTally[nTallySize].p_vFilter,"%d");
			if(nNext_p >= 2)   
			{
				bIsCardEnd = true;
				if(nNext_p >= 3) { bIsBlockEnd = true;}
			}
			for(int i = 0; i < vCellTally[nTallySize].p_vFilter.size(); ++i)
			{
				Output.CheckInputParas(vCellTally[nTallySize].p_vFilter[i] == 0 || vCellTally[nTallySize].p_vFilter[i] == 1,
					            [&](){sprintf(Output.p_sPrintStr,"incorrect FILTER parameters for cell tally %d.\n",nTally_id);},2);
			}
		}
		else if (KeyWordStr.compare("INTEGRAL")==0)  //// Read integral tally
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			int nNext_p = CDFileIO::SI_ReadVaryVec(p_fpInputFilePtr,vCellTally[nTallySize].p_vIntgrlBins,"%d");
			if(nNext_p >= 2)   ////current block is end
			{
				bIsCardEnd = true;
				if(nNext_p >= 3) { bIsBlockEnd = true; }
			}
		}
		else if(KeyWordStr.compare("CELL") == 0)  //// Read Tally Cells 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');

			vector<vector<int> > vTallyCells;
			Output.CheckInputParas(SI_ReadTallyCells(vTallyCells,bIsBlockEnd,bIsCardEnd,cGeometry),// read tally cells
				[&](){sprintf(Output.p_sPrintStr,"incorrect CELL card defined in cell tally %d.\n",nTally_id);},2);

			vCellTally[nTallySize].p_vCellVecListU = vTallyCells;
		}
		else if(KeyWordStr.compare("FLAG") == 0)  //// Read Tally level flag 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cell tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');

			int nRead_i = -1;
			fscanf(p_fpInputFilePtr,"%d",&nRead_i);
			Output.CheckInputParas(nRead_i >= 0,
					[&](){sprintf(Output.p_sPrintStr,"unknown tally level flag for cell tally %d.\n",nTally_id);},2);

			vCellTally[nTallySize].p_nFlagLevel = nRead_i;
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown keyword %s in cell tally %d!\n",KeyWordStr.c_str(),nTally_id);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	////////////////////////// Check necessary keyword for cell tally/////////////////////////
	Output.CheckInputParas(DefinedCard["TYPE"],
		[&](){sprintf(Output.p_sPrintStr,"TYPE card is not defined in cell tally %d.\n",nTally_id);},2);

	Output.CheckInputParas(DefinedCard["CELL"],
		[&](){sprintf(Output.p_sPrintStr,"CELL card is not defined in cell tally %d.\n",nTally_id);},2);



	///////////////////////Check Energy/Filter/Integral cell tally ////////////////////////////
	int nCellSize = vCellTally[nTallySize].p_vCellVecListU.size();
	int nErgSize = vCellTally[nTallySize].p_vErgBins.size();
	int nFilterSize = vCellTally[nTallySize].p_vFilter.size();
	int nIntgrlBinSize = vCellTally[nTallySize].p_vIntgrlBins.size();

	if(nErgSize > 0) // check Energy
	{

		if(!(nErgSize == 1 && vCellTally[nTallySize].p_vErgBins[0] == -1))  /// multigroup case
		{
			Output.CheckInputParas(vCellTally[nTallySize].p_vErgBins[0] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect ENERGY parameters for cell tally %d.\n",nTally_id);},2);

			for(int i = 0 ; i < nErgSize - 1; ++i)
			{
				Output.CheckInputParas(vCellTally[nTallySize].p_vErgBins[i] < vCellTally[nTallySize].p_vErgBins[i+1],
					[&](){sprintf(Output.p_sPrintStr,"incorrect ENERGY parameters for cell tally %d.\n",nTally_id);},2);
			}
		}
	}


	if(nFilterSize > 0) // check Filter
	{
		for(int i = 0 ; i < nCellSize; ++i)
		{

			Output.CheckInputParas(int(vCellTally[nTallySize].p_vCellVecListU[i].size()) == nFilterSize,
				[&](){sprintf(Output.p_sPrintStr,"incorrect FILTER parameters for cell tally %d.\n",nTally_id);},2);

			for(int j = 0 ; j < nFilterSize ; ++j)
			{
				Output.CheckInputParas(!(vCellTally[nTallySize].p_vFilter[j] == 0 && vCellTally[nTallySize].p_vCellVecListU[i][j] != 0),
					[&](){sprintf(Output.p_sPrintStr,"incorrect FILTER parameters for cell tally %d.\n",nTally_id);},2);
			}
		}
	}
	
	if(nIntgrlBinSize > 0) /// check Integral
	{
		int nSum = vCellTally[nTallySize].p_vIntgrlBins[0];
		for(int i = 1 ; i < nIntgrlBinSize ; ++i)
		{
			nSum = nSum + vCellTally[nTallySize].p_vIntgrlBins[i];
			vCellTally[nTallySize].p_vIntgrlBins[i] = vCellTally[nTallySize].p_vIntgrlBins[i] + vCellTally[nTallySize].p_vIntgrlBins[i-1];
		}

		if(nSum != nCellSize)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect integral parameters in TALLY card (ID=%d).\n",nTally_id);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	if(vCellTally[nTallySize].p_nFlagLevel >= 0) /// check Flag
	{
		Output.CheckInputParas(vCellTally[nTallySize].p_vCellVecListU[0].size() > vCellTally[nTallySize].p_nFlagLevel,
			[&](){sprintf(Output.p_sPrintStr,"incorrect FLAG parameters for cell tally %d.\n",nTally_id);},2);
	}

	////// End ///////
	//Tally.IsWithCellTally = true;
	bIsWithCellTally = true;


	return;
}