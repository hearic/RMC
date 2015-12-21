# include "Plot.h"

void CDPlot::GenPixelMatrix(CDGeometry &cGeometry)
{
	if (m_nPlotType == 2) // Box
	{
#ifdef _VTK_FILE
		m_vtkData = vtkDoubleArray::New();
		m_vtkData->SetNumberOfTuples(m_nPixel_1*m_nPixel_2*m_nPixel_3);
#else
		// resize pixel data for voxel format
		m_vPixelData3D.resize(m_nPixel_1*m_nPixel_2*m_nPixel_3);
#endif

		int nMat, nCell;
		int nPixIndex, nPixValue;
		for(int n1 = 0; n1 < m_nPixel_1; n1++)
		{
			for(int n2 = 0; n2 < m_nPixel_2; n2++)
			{
				for(int n3 = 0; n3 < m_nPixel_3; n3++)
				{
					CDVector3 OVertex_temp = m_OVertex1;
					OVertex_temp.MoveDistance(m_ODir1, (n1 + 0.5)*m_dDeltaWidth1); //move to current voxel 
					OVertex_temp.MoveDistance(m_ODir2, (n2 + 0.5)*m_dDeltaWidth2); //
					OVertex_temp.MoveDistance(m_ODir3, (n3 + 0.5)*m_dDeltaWidth3); //

					GetMatbyLocatePos(cGeometry, OVertex_temp, nMat, nCell);

					nPixIndex = n3*m_nPixel_2*m_nPixel_1 + n2*m_nPixel_1 + n1;
					if (m_nColorType == CELL)
					{
						nPixValue = nCell;
					}
					else
					{
						nPixValue = nMat;
					}

#ifdef _VTK_FILE
						m_vtkData->SetValue(nPixIndex, nPixValue);
#else
					m_vPixelData3D[nPixIndex] = nPixValue;
#endif
				}
			}
		}
		
		return;
	}
	// Slice
	//Set PixelMatrix
	m_vPixelMatrix2D.resize(m_nPixel_2);
	for (int i = 0; i < m_vPixelMatrix2D.size(); i++)
	{
		m_vPixelMatrix2D[i].resize(m_nPixel_1);
	}

	//// *** STEP 1: Tracking particle by line in width direction, generate PixelMatrix, some pixels are skipped *** ///
	vector<int> vOneLinePixels;
	vOneLinePixels.resize(m_nPixel_1);
	for (int j_height = 0; j_height < m_nPixel_2; j_height++)
	{
		//Set pos of first point in new line
		CDVector3 cVexTemp = m_OVertex1;
		cVexTemp.MoveDistance(m_ODir1, m_dDeltaWidth1/2.0);
		cVexTemp.MoveDistance(m_ODir2, j_height*m_dDeltaWidth2 + m_dDeltaWidth2/2.0);

		//check  first line: adjust vertexes of image( to output)
		if (j_height == 0)
		{
			int nAdjust = AdjustPosition(cGeometry, cVexTemp, m_ODir1);// adjust position in surface normal direction
			if (nAdjust > 0)// adjust success
			{
				m_OVertex1 = cVexTemp;//VexTemp is the position already adjusted
				m_OVertex1.MoveDistance(m_ODir1, -m_dDeltaWidth1/2.0);
				m_OVertex1.MoveDistance(m_ODir2, -m_dDeltaWidth2/2.0);
				m_OVertex2 = m_OVertex1 + m_ODir1;
				m_OVertex3 = m_OVertex2 + m_ODir2;

				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"    ");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," Vertexes are adjusted, to be not on a surface defined in Geometry.");}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			}
			else if (nAdjust < 0)// adjust failed
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"    ");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," Vertexes are on a surface defined in Geometry and adjust failed.\n");}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}

		//check points on surf or not
		bool bOnSurf = CheckOnSurf(cGeometry, cVexTemp, m_ODir1);
		//not on surf, flying this line
		if (!bOnSurf)
		{	
			FlyOneLine(cGeometry, cVexTemp, m_ODir1, m_dDeltaWidth1,m_nColorType, vOneLinePixels, m_bIsLowResolution);
		}
		//on surf, use previous line
		m_vPixelMatrix2D[j_height] = vOneLinePixels;
	}

	if (m_bIsLowResolution )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"    ");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," Image resolution for PlotID: %d is low for some surfaces.\n",m_nPlotID);}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
	}

	// *** STEP 2: for MAT / CELL color case,  auto-completion of the skipped pixels *** // 
	if (m_nColorType != SURF)
	{
		for (int j = 0; j < m_nPixel_2; j++)
		{
			if (m_vPixelMatrix2D[j][0] < 0)
			{			
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"cannot locating left point of line %d . Check if the up-left vertex is on a surface.\n",j);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
			for (int i = 1; i < m_nPixel_1; i++)
			{
				if(m_vPixelMatrix2D[j][i] < 0) m_vPixelMatrix2D[j][i] = m_vPixelMatrix2D[j][i-1];
			}
		}
	}

	// for Surf shown case: scan in Height direction
	if (m_nColorType == SURF || m_nColorType == MAT_SURF || m_nColorType == CELL_SURF)
	{
		vOneLinePixels.resize(m_nPixel_2);
		for (int i_width = 0; i_width < m_nPixel_1; i_width++)
		{
			//Set pos of first point in new line
			CDVector3 cVexTemp = m_OVertex1;
			cVexTemp.MoveDistance(m_ODir2, m_dDeltaWidth2/2.0);
			cVexTemp.MoveDistance(m_ODir1, i_width*m_dDeltaWidth1 + m_dDeltaWidth1/2.0);
			//check points on surf or not
			bool bOnSurf = CheckOnSurf(cGeometry, cVexTemp, m_ODir2);
			//not on surf, flying this line
			if (!bOnSurf)
			{	
				FlyOneLine(cGeometry, cVexTemp, m_ODir2, m_dDeltaWidth2,m_nColorType, vOneLinePixels, m_bIsLowResolution);
			}

			//Set surface value to pixel matrix 
			for (int j = 0; j < m_nPixel_2; j++)
			{
				if (vOneLinePixels[j] == 0)
				{
					m_vPixelMatrix2D[j][i_width] =  0;
				}
			}
		}
	}	
	return;
}

void CDPlot::FlyOneLine(CDGeometry &cGeometry, CDVector3 cStartPoint, CDVector3 cFlyDirection, double  dPixelWdith, int nColorType, vector<int> &vOneLinePixels, bool &bIsLowResolution )
{
	//set start position and fly direction
	m_OPlotPaticleState.p_dPos[ 0 ] = cStartPoint._x;
	m_OPlotPaticleState.p_dPos[ 1 ] = cStartPoint._y;
	m_OPlotPaticleState.p_dPos[ 2 ] = cStartPoint._z;

	m_OPlotPaticleState.p_dDir[ 0 ] = cFlyDirection._x;
	m_OPlotPaticleState.p_dDir[ 1 ] = cFlyDirection._y;
	m_OPlotPaticleState.p_dDir[ 2 ] = cFlyDirection._z;

	//Reset Pixels
	//OneLinePixels: Pixels array of one flying line
	int nPixelsNum = vOneLinePixels.size();
	for (int j = 0; j < nPixelsNum; j++)
	{
		if (m_nColorType == SURF) 
			vOneLinePixels[j] = 1;//White color, cell surface is to be changed to black
		else
			vOneLinePixels[j] = -1;//default mat color is void, need to be filled one by one
	}
	//Locate initial particle position for tracking
	m_OPlotPaticleState.ClearLevel();
	m_OPlotPaticleState.p_nCELL = cGeometry.LocateParticle(m_OPlotPaticleState, 0,m_OPlotPaticleState.p_dPos,m_OPlotPaticleState.p_dDir);
	if(m_OPlotPaticleState.p_nCELL == -1) // cannot locate position//Wrong Geometry
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to locate initial plotting point: Pos = %.15f %.15f %.15f. Check if the up-left vertex is on a defined surface (latticed)\n",
			m_OPlotPaticleState.p_dPos[0],m_OPlotPaticleState.p_dPos[1],m_OPlotPaticleState.p_dPos[2]);},CDOutput::_ERROR);			
	} 

	double dDTB = -1.0;////////// distance to cell boundary
	double dFlyLengthTot = 0.0;
	int i_pixels = 0;//pixel num of current point

	while (true)
	{
		CDVector3 cTemp(m_OPlotPaticleState.p_dPos[0]-cStartPoint._x,m_OPlotPaticleState.p_dPos[1]-cStartPoint._y,m_OPlotPaticleState.p_dPos[2]-cStartPoint._z);
		dFlyLengthTot = cTemp.GetVecLength() ;//calculate total distance to start point, to avoid numerical error, 20130705
		i_pixels = int( (dFlyLengthTot + dPixelWdith/2.0)/dPixelWdith );				
		if ( (i_pixels >= nPixelsNum) || (i_pixels <0) )break;

		//Mat Plot PixelMatrix, elements are color index, skipped cells are same with its left neighbor element
		if ( nColorType == MAT || nColorType == MAT_SURF)
		{
			m_OPlotPaticleState.p_nMAT = cGeometry.GetCellMat(m_OPlotPaticleState.p_vLocCellsU);
			if (nColorType == MAT)
			{
				vOneLinePixels[i_pixels] = m_OPlotPaticleState.p_nMAT + 1;//mat color
			}
			else if(nColorType == MAT_SURF)
			{
				if (i_pixels == 0)//first element of a new line
				{
					vOneLinePixels[i_pixels] = m_OPlotPaticleState.p_nMAT + 1;//i_width = 0
				}
				else
				{
					if (m_OPlotPaticleState.p_bIsCellChanged)vOneLinePixels[i_pixels] = 0;//Surface is Black
					if( (i_pixels+1) < nPixelsNum )vOneLinePixels[i_pixels+1] = m_OPlotPaticleState.p_nMAT + 1;//Next element is mat color
				}
			}
		}

		if ( nColorType == CELL || nColorType == CELL_SURF)
		{
			
			int nCell = m_OPlotPaticleState.p_nCELL;
			if (nColorType == CELL)
			{
				vOneLinePixels[i_pixels] = nCell + 1;//CELL color
			}
			else if(nColorType == CELL_SURF)
			{
				if (i_pixels == 0)//first element of a new line
				{
					vOneLinePixels[i_pixels] = nCell + 1;//i_width = 0
				}
				else
				{
					if (m_OPlotPaticleState.p_bIsCellChanged)vOneLinePixels[i_pixels] = 0;//Surface is Black
					if( (i_pixels+1) < nPixelsNum )vOneLinePixels[i_pixels+1] = nCell + 1;//Next element is cell color
				}
			}
		}
		//Cell Plot PixelMatrix, surfaces are black, shipped are WHITE
		if (nColorType == SURF)
		{
			if (m_OPlotPaticleState.p_bIsCellChanged)
			{
				if (i_pixels > 0)
				{	
					vOneLinePixels[i_pixels] = 0;//0 black
				}
			}
		}	

		/////////////////////////// Calculate distance to boundary ////////////////////
		if (i_pixels == 0) m_OPlotPaticleState.p_nBoundSURF = 0;  //// particle is NOT on surface at the beginning  
		dDTB = cGeometry.CalcDistToBound(m_OPlotPaticleState);	
		if(dDTB > 0)
		{
			if (bIsLowResolution == false)
			{
				if(dDTB < m_dDeltaWidth1) bIsLowResolution = true;//distance of surfaces is less than pixel width
			}
			/////////////////// Move particle forward to surface, find neighbor cell////////////////////
			m_OPlotPaticleState.FlyByLength( dDTB ); 
			cGeometry.FindNextCell(m_OPlotPaticleState);
		}
		else if(dDTB <= 0)//infinite distance, beyond the image boundary
		{
			// check points on surf or not
			CDVector3 cPosTemp(m_OPlotPaticleState.p_dPos[0],m_OPlotPaticleState.p_dPos[1],m_OPlotPaticleState.p_dPos[2]);
			bool bOnSurf = CheckOnSurf(cGeometry, cPosTemp, cFlyDirection);
			// if on surface, fly one pixel, else fly out of image
			int nFlyPixels = bOnSurf?1:nPixelsNum;
			m_OPlotPaticleState.FlyByLength( dPixelWdith * nPixelsNum ); 		
		}
	}

	return;
}