# include "Input.h"

void CDInput::ReadPlotBlock(CDPlot &cPlot,bool &bSkipCalculation)
{
	bool IsBlockEnd = false;

	bool SkipCalculationtmp = true;//default

	for(;;)
	{
		if(IsBlockEnd){ break; }
		char nextchar;
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}

		string InputCardStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);		

		//////////////////////////////// Read CONTINUE-CALCULATION option ////////////////////////////////
		if (InputCardStr.compare("CONTINUE-CALCULATION") == 0)
		{
			int ContinueCalc = -1;
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&ContinueCalc) > 0,
				[&](){sprintf(Output.p_sPrintStr,"unknown Continue-calculation parameters.\n");},2);
			Output.CheckInputParas(((ContinueCalc == 0)||(ContinueCalc == 1)),
				[&](){sprintf(Output.p_sPrintStr,"incorrect Continue-calculation parameters.\n");},2);

			SkipCalculationtmp = ! ContinueCalc;
		}
		//////////////////////////////// Read Color Scheme ////////////////////////////////
		else if(InputCardStr.compare("COLORSCHEME") == 0) 
		{
			int ColorSeed = -1;
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&ColorSeed) > 0 && ColorSeed > 0,
				[&](){sprintf(Output.p_sPrintStr,"unknown ColorScheme number.\n");},2);
			cPlot.p_unColorScheme = ColorSeed;
		}
		//////////////////////////////// Read Plot Input one by one  ////////////////////////////////
		else if(InputCardStr.compare("PLOTID") == 0) 
		{
			//add a new plot input
			int PlotNum = cPlot.p_vPlotInput.size();
			cPlot.p_vPlotInput.resize(PlotNum+1);

			int PlotIDinp;
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&PlotIDinp) > 0,
				[&](){sprintf(Output.p_sPrintStr,"unknown Plot ID.\n");},2);
			cPlot.p_vPlotInput[PlotNum].p_nPlotID = PlotIDinp;

			//check Plot ID redefinition
			for (int i = 0;i < PlotNum;i++)
			{
				if (cPlot.p_vPlotInput[PlotNum].p_nPlotID == cPlot.p_vPlotInput[i].p_nPlotID)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Reusing of PlotID %d in PLOT block.\n", PlotIDinp);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
			}

			//////////////////////////////// Read Plot/color type ////////////////////////////////
			// read plot type
			CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);
			if(InputCardStr.compare("TYPE") == 0) 
			{
				CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
				CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);
				if (InputCardStr.compare("SLICE") == 0)
				{
					cPlot.p_vPlotInput[PlotNum].p_nPlotType = 1;
				}
				else if (InputCardStr.compare("BOX") == 0)
				{
					cPlot.p_vPlotInput[PlotNum].p_nPlotType = 2;
				}
				else
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown Plot TYPE %s in Plot block PlotID %d.\n",InputCardStr.c_str(), PlotIDinp);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
			}
			else
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Plot TYPE is expected in Plot block PlotID %d, not %s.\n",PlotIDinp, InputCardStr.c_str());}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
			
			// read color type
			CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);
			if(InputCardStr.compare("COLOR") == 0) 
			{
				CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
				CDFileIO::GetKeyWord(p_fpInputFilePtr,InputCardStr);
				if (InputCardStr.compare("MAT") == 0)
				{
					cPlot.p_vPlotInput[PlotNum].p_nColorType = 1;
				}
				else if(InputCardStr.compare("CELL") == 0)
				{
					cPlot.p_vPlotInput[PlotNum].p_nColorType = 2;
				}else if(InputCardStr.compare("SURF") == 0)
				{
					cPlot.p_vPlotInput[PlotNum].p_nColorType = 3;
				}
				else if(InputCardStr.compare("MATSURF") == 0) 
				{
					cPlot.p_vPlotInput[PlotNum].p_nColorType = 4;
				}else if(InputCardStr.compare("CELLSURF") == 0) 
				{
					cPlot.p_vPlotInput[PlotNum].p_nColorType = 5;
				}
				else
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown Color type %s in Plot block PlotID %d.\n",InputCardStr.c_str(), PlotIDinp);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
			}
			else
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Plot COLOR type is expected in Plot block PlotID %d, not %s.\n",PlotIDinp,InputCardStr.c_str());}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

			// check BOX - MAT color type
			if (cPlot.p_vPlotInput[PlotNum].p_nPlotType == 2)// box
			{
				Output.CheckInputParas(cPlot.p_vPlotInput[PlotNum].p_nColorType <= 2,
					[&](){sprintf(Output.p_sPrintStr,"Only MAT / CELL color can be adopted for BOX plotting in PlotID %d.\n",PlotIDinp);},CDOutput::_ERROR);
			}
			

			////////////////////////// Read pixels and Vertexes ////////////////////////////
			vector<string> Options(2);
			vector<int> ParaNum(2);
			vector<vector<double> > Paras;
			Options[0] = "PIXELS" ;       ParaNum[0] = -1 ;
			Options[1] = "VERTEXES";      ParaNum[1] = -1 ;
			ReadCardOptions(InputCardStr,Options,ParaNum,Paras,IsBlockEnd);

			///////////////////// check and assign parameters ///////////////////
			Output.CheckInputParas(Paras[0].size() > 0,[&](){sprintf(Output.p_sPrintStr,"Pixels undefined in PLOT card PlotID %d %s .\n",PlotIDinp,Options[0].c_str());},2);
			Output.CheckInputParas(Paras[1].size() > 0,[&](){sprintf(Output.p_sPrintStr,"Vertexes undefined in PLOT card PlotID %d %s .\n",PlotIDinp,Options[1].c_str());},2);
			
			if (cPlot.p_vPlotInput[PlotNum].p_nPlotType == 1)// SLICE
			{
				Output.CheckInputParas(Paras[0].size() == 2,[&](){sprintf(Output.p_sPrintStr,"TWO PIXELS are expected for slice image in PLOT card PlotID %d.\n",PlotIDinp);},2);
				Output.CheckInputParas((Paras[1].size() == 6)||(Paras[1].size()==9),[&](){sprintf(Output.p_sPrintStr,"TWO/THREE VERTEXES are expected for box plotting in PLOT card PlotID %d.\n",PlotIDinp);},2);
				Output.CheckInputParas((Paras[0][0] <= 30000)&&(Paras[0][1] <= 30000),
					[&](){sprintf(Output.p_sPrintStr,"Pixels are too large(>30000) in PLOT card PlotID %d %s .\n",PlotIDinp,Options[0].c_str());},2);
				
				cPlot.p_vPlotInput[PlotNum].p_nPixel_1 = ExtratIntPara(Paras[0][0],"Pixels");
				cPlot.p_vPlotInput[PlotNum].p_nPixel_2 = ExtratIntPara(Paras[0][1],"Pixels");	
				
				CDVector3 p1,p2,p3;
				if (Paras[1].size() == 6)
				{
					p1._x = Paras[1][0]; 	p1._y  = Paras[1][1];	p1._z  = Paras[1][2];	
					p3._x = Paras[1][3]; 	p3._y  = Paras[1][4];	p3._z  = Paras[1][5];
					//Cal p2 for non-slope case
					CDVector3 Vectemp = p3 - p1;
					Output.CheckInputParas((Vectemp._x * Vectemp._y * Vectemp._z) == 0,
						[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes in PLOT card PlotID %d %s, not on same vertical surface(xy/yz/zx)!.\n",PlotIDinp,Options[1].c_str());},2);
					Output.CheckInputParas(((Vectemp._x || Vectemp._y) & (Vectemp._y || Vectemp._z) & (Vectemp._z || Vectemp._x)),
						[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes in PLOT card PlotID %d %s, not vertexes of a rectangle!.\n",PlotIDinp,Options[1].c_str());},2);
					if (Vectemp._x == 0.0)	{p2._x = p1._x; 	p2._y = p3._y; p2._z = p1._z;}
					if (Vectemp._y == 0.0)	{p2._x = p3._x; 	p2._y = p3._y; p2._z = p1._z;}
					if (Vectemp._z == 0.0)	{p2._x = p3._x; 	p2._y = p1._y; p2._z = p1._z;}
				}
				else
				{
					p1._x = Paras[1][0];	p1._y = Paras[1][1];	p1._z = Paras[1][2];
					p2._x = Paras[1][3];	p2._y = Paras[1][4];	p2._z = Paras[1][5];
					p3._x = Paras[1][6];	p3._y = Paras[1][7];	p3._z = Paras[1][8];
				}
				Output.CheckInputParas((p2-p1)*(p3-p2) == 0,
					[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes in PLOT card PlotID %d %s, not vertexes of a rectangle!.\n",PlotIDinp,Options[1].c_str());},2);
				cPlot.p_vPlotInput[PlotNum].p_OVertex1 = p1;
				cPlot.p_vPlotInput[PlotNum].p_OVertex2 = p2;	
				cPlot.p_vPlotInput[PlotNum].p_OVertex3 = p3;
			}
			else// BOX
			{
				Output.CheckInputParas(Paras[0].size()==3,[&](){sprintf(Output.p_sPrintStr,"THREE PIXELS are expected for box plotting in PLOT card PlotID %d.\n",PlotIDinp);},2);
				Output.CheckInputParas(Paras[1].size()==12,[&](){sprintf(Output.p_sPrintStr,"FOUR VERTEXES are expected for box plotting in PLOT card PlotID %d.\n",PlotIDinp);},2);
				
				cPlot.p_vPlotInput[PlotNum].p_nPixel_1 = ExtratIntPara(Paras[0][0],"Pixels");
				cPlot.p_vPlotInput[PlotNum].p_nPixel_2 = ExtratIntPara(Paras[0][1],"Pixels");
				cPlot.p_vPlotInput[PlotNum].p_nPixel_3 = ExtratIntPara(Paras[0][2],"Pixels");
				
				CDVector3 p1,p2,p3,p4;
				p1._x = Paras[1][0];	p1._y = Paras[1][1];	p1._z = Paras[1][2];
				p2._x = Paras[1][3];	p2._y = Paras[1][4];	p2._z = Paras[1][5];
				p3._x = Paras[1][6];	p3._y = Paras[1][7];	p3._z = Paras[1][8];
				p4._x = Paras[1][9];	p4._y = Paras[1][10];	p4._z = Paras[1][11];
				Output.CheckInputParas(((p2-p1)*(p3-p2) == 0) && ((p4-p3)*(p3-p2) == 0) && ((p4-p3)*(p1-p1) == 0) ,
					[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes in PLOT card PlotID %d %s, not vertexes of a box!.\n",PlotIDinp,Options[1].c_str());},2);
				cPlot.p_vPlotInput[PlotNum].p_OVertex1 = p1;
				cPlot.p_vPlotInput[PlotNum].p_OVertex2 = p2;	
				cPlot.p_vPlotInput[PlotNum].p_OVertex3 = p3;
				cPlot.p_vPlotInput[PlotNum].p_OVertex4 = p4;
			}				
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in Plot block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	if (cPlot.p_vPlotInput.size() == 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Plotting parameters not defined in Plot block.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		return;
	}

	//Set Plot flag
	cPlot.p_bIsPlot = true;
    bSkipCalculation = SkipCalculationtmp;
	return;
}