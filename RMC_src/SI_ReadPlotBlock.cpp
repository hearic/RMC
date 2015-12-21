# include "Input.h"

void CDInput::SI_ReadPlotBlock(CDPlot &cPlot,bool &bSkipCalculation)
{
	bool IsBlockEnd = false;

	bool SkipCalculationtmp = true;//default

	for(;;)
	{
		if(IsBlockEnd){ break; }
		char nextchar;
		int nNext_p =CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) ;
		//if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		if (nNext_p==4||nextchar=='%')
		{
			break;
		}

		CDFileIO::SkipSpecChar(p_fpInputFilePtr, '$');
		string InputCardStr ;
		CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);		
		if (InputCardStr.compare("CHOOSE")==0)
		{
			for(;;)
			{
				CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) ;
				if (nNext_p==4||nextchar=='%'||nextchar=='$')
				{
					break;
				}
				CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);		
				//////////////////////////////// Read CONTINUE-CALCULATION option ////////////////////////////////
				if (InputCardStr.compare("CONTINUE-CALCULATION")==0)
				{
					int ConCal = -1;
					CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
					Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&ConCal) > 0,
						[&](){sprintf(Output.p_sPrintStr,"unknown Continue-calculation parameters.\n");},2);
					Output.CheckInputParas(((ConCal == 0)||(ConCal == 1)),
						[&](){sprintf(Output.p_sPrintStr,"incorrect Continue-calculation parameters.\n");},2);

					SkipCalculationtmp = ! ConCal;
				}
				//////////////////////////////// Read Color Scheme ////////////////////////////////
				else if(InputCardStr.compare("COLORSCHEME")==0) 
				{
					int ColorSeed = -1;
					CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
					Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&ColorSeed) > 0 && ColorSeed > 0,
						[&](){sprintf(Output.p_sPrintStr,"unknown ColorScheme number.\n");},2);
					cPlot.p_unColorScheme = ColorSeed;
				}
			}
		}
		//////////////////////////////// Read Plot Input one by one  ////////////////////////////////
		else if(InputCardStr.compare("PLOTID")==0) 
		{
			//add a new plot input
			int PlotNum = cPlot.p_vPlotInput.size();
			cPlot.p_vPlotInput.resize(PlotNum+1);
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, ':');
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, ':');
			int PlotIDinp;
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&PlotIDinp) > 0,
				[&](){sprintf(Output.p_sPrintStr,"unknown Plot ID.\n");},2);
			cPlot.p_vPlotInput[PlotNum].p_nPlotID = PlotIDinp;

			//////////////////////////////// Read Plot type ////////////////////////////////
			CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);		//skip 'colortype'
		 	CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');//skip 'colortype'
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '"');//skip '"'
			CDFileIO::SI_GetKeyWord(p_fpInputFilePtr,InputCardStr);
			CDFileIO::SkipSpecChar(p_fpInputFilePtr, '"');//skip '"'
			if(InputCardStr.compare("MATCOLOR")==0) 
			{
				cPlot.p_vPlotInput[PlotNum].p_nColorType = 1;
			}
			else if(InputCardStr.compare("CELLSURF")==0) 
			{
				cPlot.p_vPlotInput[PlotNum].p_nColorType = 2;
			}
			else if(InputCardStr.compare("MATCELL")==0) 
			{
				cPlot.p_vPlotInput[PlotNum].p_nColorType = 3;
			}
			else
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in Plot block.\n",InputCardStr.c_str());}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
			////////////////////////// Read pixels and Vertexs ////////////////////////////
			vector<string> Options(2);
			vector<int> ParaNum(2);
			vector<vector<double> > Paras;
			Options[0] = "PIXELS" ;       ParaNum[0] = 2 ;
			Options[1] = "VERTEXES";      ParaNum[1] = -1 ;
			SI_ReadCardOptions(InputCardStr,Options,ParaNum,Paras,IsBlockEnd);
			///////////////////// check and assign parameters ///////////////////
			Output.CheckInputParas(Paras[0].size() > 0,[&](){sprintf(Output.p_sPrintStr,"Pixels is undefined in PLOT card PlotID %d %s .\n",PlotIDinp,Options[0].c_str());},2);
			Output.CheckInputParas(Paras[0][0] > 50 && Paras[0][1] > 50,[&](){sprintf(Output.p_sPrintStr,"Pixels is too small(<50) in PLOT card PlotID  %d %s .\n",PlotIDinp,Options[0].c_str());},2);
			Output.CheckInputParas((Paras[0][0] < 20000)&&(Paras[0][1] < 20000),
				[&](){sprintf(Output.p_sPrintStr,"Pixels is too large(>50000) in PLOT card PlotID %d %s .\n",PlotIDinp,Options[0].c_str());},2);

			cPlot.p_vPlotInput[PlotNum].p_nPixel_1  = ExtratIntPara(Paras[0][0],"Pixels");
			cPlot.p_vPlotInput[PlotNum].p_nPixel_2 = ExtratIntPara(Paras[0][1],"Pixels");	

			Output.CheckInputParas(Paras[1].size() > 0,[&](){sprintf(Output.p_sPrintStr,"vertexes undefined in PLOT card PlotID %d %s .\n",PlotIDinp,Options[1].c_str());},2);
			Output.CheckInputParas((Paras[1].size() == 6)||(Paras[1].size() == 9),
				[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes numbers  in PLOT card PlotID %d %s.\n",PlotIDinp,Options[1].c_str());},2);
			
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
					[&](){sprintf(Output.p_sPrintStr,"incorrect points in PLOT card PlotID %d %s, not vertexes of a rectangle!.\n",PlotIDinp,Options[1].c_str());},2);
				if (Vectemp._x == 0.0)	{p2._x = p1._x; 	p2._y = p3._y; p2._z = p1._z;}
				if (Vectemp._y == 0.0)	{p2._x = p3._x; 	p2._y = p3._y; p2._z = p1._z;}
				if (Vectemp._z == 0.0)	{p2._x = p3._x; 	p2._y = p1._y; p2._z = p1._z;}
			}
			else
			{
				p1._x = Paras[1][0];		p1._y = Paras[1][1];		p1._z  = Paras[1][2];
				p2._x = Paras[1][3];		p2._y = Paras[1][4];		p2._z  = Paras[1][5];
				p3._x = Paras[1][6];		p3._y = Paras[1][7];		p3._z  = Paras[1][8];
			}
			Output.CheckInputParas((p2-p1)*(p3-p2) == 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect vertexes in PLOT card PlotID %d %s, NOT vertexes of a rectangle!.\n",PlotIDinp,Options[1].c_str());},2);
			cPlot.p_vPlotInput[PlotNum].p_OVertex1 = p1;
			cPlot.p_vPlotInput[PlotNum].p_OVertex2 = p2;	
			cPlot.p_vPlotInput[PlotNum].p_OVertex3 = p3;	
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown input card %s in Plot block.\n",InputCardStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	if (cPlot.p_vPlotInput.size() == 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Plotting image parameters not defined in Plot block.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		return;
	}

	//check Plot ID redefinition
	int  temp = 1;
	for (int i=0;i<cPlot.p_vPlotInput.size()-1;i++)
	{
		for (int j=i+1;j<cPlot.p_vPlotInput.size();j++)
		{
			temp*=(cPlot.p_vPlotInput[i].p_nPlotID-cPlot.p_vPlotInput[j].p_nPlotID);
		}
	}
	Output.CheckInputParas(temp != 0,[&](){sprintf(Output.p_sPrintStr,"Reusing of PlotID in PLOT block.\n");},2);

	//Set Plot flag
	cPlot.p_bIsPlot = true;
//	CalMode.SkipCalculation = SkipCalculationtmp;
    bSkipCalculation = SkipCalculationtmp;
	return;
}