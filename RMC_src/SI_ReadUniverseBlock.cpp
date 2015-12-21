# include "Input.h"

void CDInput::SI_ReadUniverseBlock(CDGeometry &cGeometry)
{
	//auto &UniverseIndex = Geometry.UniverseIndex;//CGeometry.UniverseIndex
	//auto &Universe = Geometry.Universe;//CGeometry.Universe
	for(;;)
	{
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar); 
		//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);  
		if(nextchar == '%'||nNext_p==4)// Geometry block finished
		{
			break;
		}
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'$'); 
		string KeyWordStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,KeyWordStr);
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,':'); 
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,':'); 

	////////////////////// Read universe index and update universe index list /////////////////////////
	static bool bIsUniv0Defined = false;
	int nUniv_index = -1;
	int nUniv_index_u = -1;
	Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nUniv_index_u) > 0,
		[&](){sprintf(Output.p_sPrintStr,"unknown universe ID.\n");},2);
	Output.CheckInputParas(nUniv_index_u >= 0,
		[&](){sprintf(Output.p_sPrintStr,"incorrect universe ID %d.\n",nUniv_index_u);},2);

	if( nUniv_index_u == 0)  ///// Build Universe 0
	{
		nUniv_index = 0 ;
		if(bIsUniv0Defined)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"universe 0 is redefinied.\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
		bIsUniv0Defined = true;
		if(cGeometry.p_vUniverse.size() < 1)
		{
			cGeometry.p_vUniverse.resize(1);
		}
	}
	else
	{
		nUniv_index = cGeometry.p_OUniverseIndex.AddUserIndex(nUniv_index_u);
		cGeometry.p_vUniverse.resize(nUniv_index + 1);
	}

	/////////////////////////////// Read universe/lattice options //////////////////////////////////
	vector<string> vOptions(7);
	vector<int> vParaNum(7);
	vector<vector<double> > vParas;
	vOptions[0] = "MOVE" ;   vParaNum[0] = 3 ;
	vOptions[1] = "ROTATE";  vParaNum[1] = 9 ;
	vOptions[2] = "LAT" ;    vParaNum[2] = 1 ;
	vOptions[3] = "SCOPE";   vParaNum[3] = -1 ; 
	vOptions[4] = "PITCH";   vParaNum[4] = -1 ;
	vOptions[5] = "SITA" ;   vParaNum[5] = 1 ;
	vOptions[6] = "FILL";    vParaNum[6] = -1 ;
	bool bIsBlockEnd;
	SI_ReadCardOptions(string("UNIVERSE"),vOptions,vParaNum,vParas,bIsBlockEnd);


	///////////////////// check and assign parameters //////////////////
	if(vParas[2].size() == 0)  //// not lattice
	{
		Output.CheckInputParas(vParas[3].size() == 0 && vParas[4].size() == 0 && vParas[5].size() == 0 && vParas[6].size() == 0,
			[&](){sprintf(Output.p_sPrintStr,"redundant lattice parameters for universe %d.\n",nUniv_index_u);},2);
	}

	if(vParas[0].size() > 0)  //// check/copy Move card
	{
		cGeometry.p_vUniverse[nUniv_index].p_bIsMoved = true;
		for(int i = 0 ; i < 3 ; ++i)
		{
			cGeometry.p_vUniverse[nUniv_index].p_dOrigin[i] = vParas[0][i];
		}
	}
	if(vParas[1].size() > 0)  //// check/copy Rotate card
	{
		cGeometry.p_vUniverse[nUniv_index].p_bIsRotated = true;
		int k = 0;
		for(int i = 0 ; i < 3 ; ++i)
		{
			double dSum = 0;
			for(int j = 0 ; j < 3 ; ++j)
			{
				cGeometry.p_vUniverse[nUniv_index].p_dRotation[i][j] = vParas[1][k];
				++k;
				dSum = dSum + cGeometry.p_vUniverse[nUniv_index].p_dRotation[i][j] * cGeometry.p_vUniverse[nUniv_index].p_dRotation[i][j];
			}
			Output.CheckInputParas(fabs(dSum - 1.0 ) < 1.0E-6,
			[&](){sprintf(Output.p_sPrintStr,"incorrect ROTATE parameters in universe %d.\n",nUniv_index_u);;},2);
		}
	}
	if(vParas[2].size() > 0)  //// check/copy Lat card
	{
		cGeometry.p_vUniverse[nUniv_index].p_bIsLattice = true ;
		cGeometry.p_vUniverse[nUniv_index].p_nLatType = ExtratIntPara(vParas[2][0], "LAT");
		Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_nLatType == 1 || cGeometry.p_vUniverse[nUniv_index].p_nLatType == 2,
			[&](){sprintf(Output.p_sPrintStr,"incorrect LAT parameters in universe %d.\n",nUniv_index_u);},2);
		if(cGeometry.p_vUniverse[nUniv_index].p_nLatType == 1)
		{
			Output.CheckInputParas(vParas[3].size() == 3 && vParas[4].size() == 3 && vParas[5].size() == 0 && vParas[6].size() > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect lattice parameters for LAT = 1 in universe %d.\n",nUniv_index_u);},2);
		}
		else if(cGeometry.p_vUniverse[nUniv_index].p_nLatType == 2)
		{
			Output.CheckInputParas(vParas[3].size() == 2 && vParas[4].size() == 2 && vParas[5].size() == 1 && vParas[6].size() > 0,
				[&](){sprintf(Output.p_sPrintStr,"insufficient lattice parameters for LAT = 2 in universe %d.\n",nUniv_index_u);},2);		
		}

	}

	if(vParas[3].size() > 0)  //// check/copy Scope card
	{
		for(int i = 0 ; i < vParas[3].size() ; ++i)
		{
			cGeometry.p_vUniverse[nUniv_index].p_nScope[i] = ExtratIntPara(vParas[3][i],"SCOPE");

			Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_nScope[i] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect SCOPE parameters in universe %d.\n",nUniv_index_u);},2);
		}
	}

	if(vParas[4].size() > 0)  //// check/copy Pitch card
	{
		for(int i = 0 ; i < vParas[4].size() ; ++i)
		{
			cGeometry.p_vUniverse[nUniv_index].p_dPitch[i] = vParas[4][i];

			Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_dPitch[i] > 0 || cGeometry.p_vUniverse[nUniv_index].p_nScope[i] == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect PITCH parameters in universe %d.\n",nUniv_index_u);},2);
		}
		if( vParas[4].size() == 2)
		{
			double L1 = cGeometry.p_vUniverse[nUniv_index].p_dPitch[0]; 
			double L2 = cGeometry.p_vUniverse[nUniv_index].p_dPitch[1]; 
			cGeometry.p_vUniverse[nUniv_index].p_dHeight = sqrt(L2*L2 - L1*L1*0.25);
		}
	}

	if(vParas[5].size() > 0)  //// check/copy Sita card
	{
		cGeometry.p_vUniverse[nUniv_index].p_dSita = vParas[5][0] ;
		Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_dSita > 0 && cGeometry.p_vUniverse[nUniv_index].p_dSita < 90,
			[&](){sprintf(Output.p_sPrintStr,"incorrect SITA parameters in universe %d.\n",nUniv_index_u);},2);

		double dSita = cGeometry.p_vUniverse[nUniv_index].p_dSita * 3.1415926536/180.0;
		cGeometry.p_vUniverse[nUniv_index].p_dCos_sita = cos(dSita);
		cGeometry.p_vUniverse[nUniv_index].p_dSin_sita = sin(dSita);
	}

	if(vParas[6].size() > 0)  //// check/copy Fill card
	{
		cGeometry.p_vUniverse[nUniv_index].p_vFillLatUnivIndexU.resize(0);
		cGeometry.p_vUniverse[nUniv_index].p_nFilledLatNum = cGeometry.p_vUniverse[nUniv_index].p_nScope[0] * cGeometry.p_vUniverse[nUniv_index].p_nScope[1];
		if(cGeometry.p_vUniverse[nUniv_index].p_nLatType == 1 )
		{
			cGeometry.p_vUniverse[nUniv_index].p_nFilledLatNum = cGeometry.p_vUniverse[nUniv_index].p_nFilledLatNum * cGeometry.p_vUniverse[nUniv_index].p_nScope[2];
		}

		Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_nFilledLatNum == int(vParas[6].size()),
			[&](){sprintf(Output.p_sPrintStr,"inconsistent SCOPE/FILL parameters in universe %d.\n",nUniv_index_u);},2);

		for(int i = 0 ; i < vParas[6].size() ; ++i)
		{
			int nFilled_univ_u = ExtratIntPara(vParas[6][i],"FILL");
			cGeometry.p_vUniverse[nUniv_index].p_vFillLatUnivIndexU.push_back(nFilled_univ_u);
		}
	}

	//////////////////////////// Read Cells inside Universe /////////////////////////////
	if(nUniv_index == 0) //// read filled-in cell in ordinary universe
	{
		Output.CheckInputParas(!cGeometry.p_vUniverse[0].p_bIsLattice && !cGeometry.p_vUniverse[0].p_bIsRotated && !cGeometry.p_vUniverse[0].p_bIsMoved,
			[&](){sprintf(Output.p_sPrintStr,"Move/Rotate/Lattite is not allowed for Universe 0.\n");},2);
	}

	//////////////////////////// Read Cells inside Universe /////////////////////////////
	if(!cGeometry.p_vUniverse[nUniv_index].p_bIsLattice) //// read filled-in cell in ordinary universe
	{
		SI_ReadCellCard(nUniv_index,cGeometry);
		cGeometry.p_vUniverse[nUniv_index].p_nContainCellNum = cGeometry.p_vUniverse[nUniv_index].p_vFillCellsIndexU.size();  // total TotCellNum

		Output.CheckInputParas(cGeometry.p_vUniverse[nUniv_index].p_nContainCellNum > 0,
			[&](){sprintf(Output.p_sPrintStr,"no cell defined in universe %d! \n",nUniv_index_u);},2);
	}


	//return;
	}
}