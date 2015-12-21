# include "Input.h"

void CDInput::ReadCsTallyCard(bool &bIsBlockEnd,vector<CDCsTally> &vOneGroupCsTally,bool &bIsWithCsTally,CDGeometry &cGeometry,CDMaterial &cMaterial)
{
	bIsBlockEnd = false;
	bool bIsCardEnd = false;
	
	//auto &OneGroupCsTally = Tally.OneGroupCsTally;
	int nTallySize = vOneGroupCsTally.size();
	vOneGroupCsTally.resize(nTallySize + 1) ;

	//////////////////////////////// Read cs tally ID ////////////////////////////////
	int nTally_id ;
	Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nTally_id) > 0,
		[&](){sprintf(Output.p_sPrintStr,"unknown cs tally ID.\n");},2);
	vOneGroupCsTally[nTallySize].p_nTallyID = nTally_id;


	////////////////// avoid redefinition ////////////////////
	map<string,bool> DefinedCard;
	DefinedCard["CELL"] = false;
	DefinedCard["MAT"] = false;
	DefinedCard["MT"] = false;

	////////////////////////////// Read cs tally options /////////////////////////////
	for(;;)
	{
		if(bIsBlockEnd || bIsCardEnd){ break; }

		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if(nNext_p >= 2)   
		{
			if(nNext_p >= 3) { bIsBlockEnd = true; }
			break;
		} 

		string KeyWordStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,KeyWordStr);

		if(KeyWordStr.compare("CELL") == 0)  //// Read Tally Cell 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cs tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');

			vector<vector<int> > vTallyCells;
			Output.CheckInputParas(ReadTallyCells(vTallyCells,bIsBlockEnd,bIsCardEnd,cGeometry) && vTallyCells.size() == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect CELL card in cs tally %d.\n",nTally_id);},2);

			vOneGroupCsTally[nTallySize].p_vCellVecU = vTallyCells[0];

			//////////////////// check cstally cell is bottom cell or not ///////////
			int nCell_index_u = vTallyCells[0][vTallyCells[0].size() - 1];
			int nCell_index = cGeometry.p_OCellIndex.GetIndex(nCell_index_u);
			if(nCell_index < 0 || cGeometry.p_vCell[nCell_index].p_nFillUnivIndexU > 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect or non-bottom CELL defined in cs tally %d.\n",nTally_id);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}

		else if(KeyWordStr.compare("MAT") == 0)  //// Read Tally material 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cs tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			int nRead_i = -1;
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nRead_i) > 0 && nRead_i > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect material in cs tally %d.\n",nTally_id);},2);
			vOneGroupCsTally[nTallySize].p_nMat = cMaterial.p_OMatSetIndex.GetIndex(nRead_i);
		}
		else if(KeyWordStr.compare("MT") == 0)  //// Read Tally MT arrays 
		{
			////////////////////// check redefinition ////////////////////////////
			Output.CheckInputParas(!DefinedCard[KeyWordStr],
				[&](){sprintf(Output.p_sPrintStr,"%s card is redefined in cs tally %d.\n",KeyWordStr.c_str(),nTally_id);},2);
			DefinedCard[KeyWordStr] = true;

			CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
			for(;;)
			{
				vector<int> vMTs;
				int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,vMTs,"%d");

				vOneGroupCsTally[nTallySize].p_vNucMTs.push_back(vMTs);
				if(nNext_p >= 2)   
				{
					bIsCardEnd = true;
					if(nNext_p >= 3) { bIsBlockEnd = true;}
					break;
				}
				nextchar = ' ';
				nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
				if(nextchar != ',')   
				{
					break;
				} 
				else
				{
					CDFileIO::SkipSpecChar(p_fpInputFilePtr,',');
				}
			}
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown keyword %s in cs tally %d!\n",KeyWordStr.c_str(),nTally_id);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

	}

	////////////////////////// Check necessary keyword for xs tally/////////////////////////
	Output.CheckInputParas(DefinedCard["CELL"],
		[&](){sprintf(Output.p_sPrintStr,"CELL card is not defined in xs tally %d.\n",nTally_id);},2);

	Output.CheckInputParas(DefinedCard["MAT"],
		[&](){sprintf(Output.p_sPrintStr,"MAT card is not defined in xs tally %d.\n",nTally_id);},2);

	Output.CheckInputParas(DefinedCard["MT"],
		[&](){sprintf(Output.p_sPrintStr,"MT card is not defined in xs tally %d.\n",nTally_id);},2);


	//////////////////////////////// Check cs tally ///////////////////////////////////
	int nNucNum = cMaterial.GetMatTotNucNum(vOneGroupCsTally[nTallySize].p_nMat);
	Output.CheckInputParas(int(vOneGroupCsTally[nTallySize].p_vNucMTs.size()) == nNucNum,
		[&](){sprintf(Output.p_sPrintStr,"inconsistent MAT/MT parameters in cs tally %d!\n",nTally_id);},2);
	for(int i = 0 ; i < nNucNum ; ++i)
	{
		for(int j = 0 ; j < int(vOneGroupCsTally[nTallySize].p_vNucMTs[i].size());++j)
		{
			int nMt = vOneGroupCsTally[nTallySize].p_vNucMTs[i][j];
			Output.CheckInputParas(nMt == -1 || nMt == -2 || nMt == -3 || nMt == -6 || (nMt > 0 && nMt < 1000),
				[&](){sprintf(Output.p_sPrintStr,"incorrect MT %d in cs tally %d!\n",nMt,nTally_id);},2);
		}
	}



	//Tally.IsWithCsTally = true;
	bIsWithCsTally = true;

	return;

}