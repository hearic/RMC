# include "Input.h"

void CDInput::SI_ReadCellCard(int nUniv_index,CDGeometry &cGeometry)
{
	//auto &Universe = Geometry.Universe;//CGeometry.Universe
	//auto &Cell = Geometry.Cell;  //CGeometry.Cell
	//auto &CellIndex = Geometry.CellIndex;//CGeometry.CellIndex
	//vector<UniverseClass> &Universe = vUniverse;
	//vector<C_SingleCell>  &Cell = vCell;
	//IndexClass &CellIndex = CCellIndex;

	for(;;)
	{
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar); 
		if(nextchar == '%'||nNext_p==4)//  current universe is end
		{
			break;
		}
		long lPos;
		lPos = ftell(p_fpInputFilePtr);
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'$'); 
		string KeyWordStr ;
		//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);cout<<nextchar;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,KeyWordStr);
		if (KeyWordStr.compare("UNIVERSE")==0)
		{
		fseek(p_fpInputFilePtr,lPos,SEEK_SET);
			//CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);cout<<nextchar;
	break;
		}
		//if(nextchar == '%'||nextchar == '$'||nNext_p==4)//  current universe is end
		//{
		//	break;
		//}
		
		//if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3)   // current universe is end
		//{
		//	break;
		//}

		/////////////////////////////////////// Read cell keyword ///////////////////////////////////
		/*string KeyWordStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,KeyWordStr);*/

		if(KeyWordStr.compare("CELL")!=0)  
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Keyword CELL expected in universe %d.\n",	cGeometry.p_OUniverseIndex.GetIndexU(nUniv_index));}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		/////////////////////// Read cell index and update cell index list //////////////////////////
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,':'); 
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,':'); 
		int nCell_index_u = -1;
		Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nCell_index_u) > 0,
			[&](){sprintf(Output.p_sPrintStr,"unknown cell ID.\n");},2);
		Output.CheckInputParas(nCell_index_u > 0,
			[&](){sprintf(Output.p_sPrintStr,"incorrect cell ID %d.\n",nCell_index_u);},2);

		int nCell_index = cGeometry.p_OCellIndex.AddUserIndex(nCell_index_u);		
		cGeometry.p_vUniverse[nUniv_index].p_vFillCellsIndexU.push_back(nCell_index_u);
		cGeometry.p_vCell.resize(nCell_index + 1);

		///////////////////////////////// Read Cell surface and bool ////////////////////////////////
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,KeyWordStr);//skip"bool_definition"
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'='); 
		bool bIsBlockEnd, bIsCardEnd;
		SI_ReadCellSurfBool(nCell_index,bIsBlockEnd,bIsCardEnd,cGeometry);
		if(bIsBlockEnd)
		{
			break;
		}
		if(bIsCardEnd)
		{
			continue;
		}

		////////////////////// Read Cell options£º Mat, Imp, Tmp, Vol, Fill ////////////////////////
		vector<string> Options(6);
		Options[0] = "MAT" ;
		Options[1] = "VOL";
		Options[2] = "TMP";
		Options[3] = "VOID";
		Options[4] = "FILL";
		Options[5] = "INNER";
		vector<int> vParaNum(6,1);
		vector<vector<double> > vParas;
		SI_ReadCardOptions(string("Cell"),Options,vParaNum,vParas,bIsBlockEnd);

		////////////////////////////// check and copy parameters //////////////////////////////////
		if(vParas[0].size() > 0)  //// check/copy Mat card
		{
			cGeometry.p_vCell[nCell_index].p_nMatIndexU = ExtratIntPara(vParas[0][0],"MAT");
			Output.CheckInputParas(cGeometry.p_vCell[nCell_index].p_nMatIndexU >= 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for MAT in cell %d.\n",nCell_index_u);},2);
		}

		if(vParas[1].size() > 0)  //// check/copy Vol card
		{
			cGeometry.p_vCell[nCell_index].p_dVol = vParas[1][0];
			Output.CheckInputParas(cGeometry.p_vCell[nCell_index].p_dVol > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters %f for VOL in cell %d.\n",cGeometry.p_vCell[nCell_index].p_dVol,nCell_index_u);},2);
		}

		if(vParas[2].size() > 0)  //// check/copy Tmp card
		{
			cGeometry.p_vCell[nCell_index].p_dTemp = vParas[2][0];
			Output.CheckInputParas(cGeometry.p_vCell[nCell_index].p_dTemp > 0 ,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters %f for TMP in cell %d.\n",cGeometry.p_vCell[nCell_index].p_dTemp,nCell_index_u);},2);

			cGeometry.p_vCell[nCell_index].p_dTemp = cGeometry.p_vCell[nCell_index].p_dTemp*1.3806505E-23/(1.6022E-19)*1.0E-6;   //// convert to KT(Mev)
		}

		if(vParas[3].size() > 0)  //// check/copy void card.  void = zero importance
		{
			int nTemp = ExtratIntPara(vParas[3][0],"VOID");
			Output.CheckInputParas(nTemp == 0 || nTemp == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for VOID in cell %d.\n",nCell_index_u);},2);
			if(nTemp == 1)
			{
				cGeometry.p_vCell[nCell_index].p_nImp = 0;  
			}
		}

		if(vParas[4].size() > 0)  //// check/copy fill card
		{
			cGeometry.p_vCell[nCell_index].p_nFillUnivIndexU = ExtratIntPara(vParas[4][0],"FILL");
			Output.CheckInputParas(cGeometry.p_vCell[nCell_index].p_nFillUnivIndexU > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for FILL in cell %d.\n",nCell_index_u);},2);
		}

		if(vParas[5].size() > 0)  //// check/copy inner card. 
		{
			int nTemp = ExtratIntPara(vParas[5][0],"INNER");
			Output.CheckInputParas(nTemp == 0 || nTemp == 1,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for INNER in cell %d.\n",nCell_index_u);},2);
			if(nTemp == 1)
			{
				cGeometry.p_vCell[nCell_index].p_bIsInnerCell = true;  
			}
		}

		if(bIsBlockEnd)
		{
			break;
		}

	}


	return;
}