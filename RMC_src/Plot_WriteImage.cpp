# include "Plot.h"
void CDPlot::WriteImage()
{
	char chBuffer[10];
	sprintf(chBuffer,"%d",m_nPlotID);
	strcpy(m_chImageFileName, Output.p_chInputFileName); 
	strcat(m_chImageFileName,"_plot_");
	strcat(m_chImageFileName,chBuffer);
	
	if (m_nPlotType == 2) // Box
	{
#ifdef _VTK_FILE
		m_vtkGrid = vtkImageData::New();
		m_vtkGrid->SetDimensions(m_nPixel_1+1, m_nPixel_2+1, m_nPixel_3+1);
		m_vtkGrid->SetOrigin(m_OVertex1._x, m_OVertex1._y, m_OVertex1._z);
		m_vtkGrid->SetSpacing(m_dDeltaWidth1, m_dDeltaWidth2, m_dDeltaWidth3);
		if (m_nColorType == CELL)
		{
			m_vtkData->SetName("Cell");
		}
		else
		{
			m_vtkData->SetName("Material");
		}
		m_vtkGrid->GetCellData()->AddArray(m_vtkData);
		m_vtkWriter = vtkXMLImageDataWriter::New();
		strcat(m_chImageFileName,".vti");
		m_vtkWriter->SetInput(m_vtkGrid);
		m_vtkWriter->SetFileName(m_chImageFileName);
		m_vtkWriter->Write();
#else // voxel file
		//open voxel image file
		strcat(m_chImageFileName,".voxel");
		fpImagefilePtr = fopen(m_chImageFileName,"w");
		if(!fpImagefilePtr)
		{
			cerr<<"Error: Open image file "<<m_chImageFileName<<" failed!"<<endl;
			exit(1);
		}
		fprintf(fpImagefilePtr, "%d %d %d\n", m_nPixel_1, m_nPixel_2, m_nPixel_3);
		fprintf(fpImagefilePtr, "%f %f %f\n", m_dDeltaWidth1, m_dDeltaWidth2, m_dDeltaWidth3);
		fprintf(fpImagefilePtr, "%f %f %f\n", m_OVertex1._x, m_OVertex1._y, m_OVertex1._z);

		if (m_nColorType == CELL)
		{
			fprintf(fpImagefilePtr, "Cell\n");
		}
		else
		{
			fprintf(fpImagefilePtr, "Material\n");
		}

		int nPixIndex;
		for(int n1 = 0; n1 < m_nPixel_1; n1++)
		{
			fprintf(fpImagefilePtr, "\n");
			for(int n2 = 0; n2 < m_nPixel_2; n2++)
			{
				for(int n3 = 0; n3 < m_nPixel_3; n3++)
				{
					nPixIndex = n3*m_nPixel_2*m_nPixel_1 + n2*m_nPixel_1 + n1;
					fprintf(fpImagefilePtr, "%d ", m_vPixelData3D[nPixIndex]);
				}
			}
		}
		fclose(fpImagefilePtr); // close file voxel
#endif

	}
	else // Slice
	{
#ifdef _PNG_IMAGE //png format
		//open png image file
		strcat(m_chImageFileName,".png");
		fpImagefilePtr = fopen(m_chImageFileName,"wb");//binary
		if(!fpImagefilePtr)
		{
			cerr<<"Error: Open image file "<<m_chImageFileName<<" failed!"<<endl;
			exit(1);
		}
		//create png structs
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
		info_ptr = png_create_info_struct(png_ptr);

		//link
		png_init_io(png_ptr, fpImagefilePtr);
		//set
		png_set_IHDR(png_ptr, info_ptr,m_nPixel_1, m_nPixel_2, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
		//write info
		png_write_info(png_ptr, info_ptr);

		//write pixels row by row
		png_byte *rowpointer = new png_byte [3*m_nPixel_1];
		unsigned char r, g, b;
		int nPixValue;
		for (int j = 0;j < m_nPixel_2;j++)
		{
			for (int i = 0;i < m_nPixel_1;i++)
			{
				nPixValue = m_vPixelMatrix2D[j][i];
				if (m_nColorType == MAT || m_nColorType == SURF || m_nColorType == MAT_SURF )
				{
					r = m_vMatRGBIndex[nPixValue].r;
					g = m_vMatRGBIndex[nPixValue].g;
					b = m_vMatRGBIndex[nPixValue].b;
				}
				else // m_nColorType == CELL || m_nColorType == CELL_SURF: Cell color case 
				{
					r = m_vCellRGBIndex[nPixValue].r;
					g = m_vCellRGBIndex[nPixValue].g;
					b = m_vCellRGBIndex[nPixValue].b;
				}
				rowpointer[3*i]   = r;
				rowpointer[3*i+1] = g;
				rowpointer[3*i+2] = b;
			}
			png_write_row(png_ptr, rowpointer);
		}
		
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);
#else 
		//open ppm image file
		strcat(m_chImageFileName,".ppm");
		fpImagefilePtr = fopen(m_chImageFileName,"w");
		if(!fpImagefilePtr)
		{
			cerr<<"Error: Open image file "<<m_chImageFileName<<" failed!"<<endl;
			exit(1);
		}
		fprintf(fpImagefilePtr, "P6\n");
		fprintf(fpImagefilePtr, "%d %d\n", m_nPixel_1, m_nPixel_2);
		fprintf(fpImagefilePtr, "255\n");
		int r, g, b, nPixValue;
		for (int j = 0;j < m_nPixel_2;j++)
		{			
			for (int i = 0;i < m_nPixel_1;i++)
			{
				nPixValue = m_vPixelMatrix2D[j][i];
				if (m_nColorType == MAT || m_nColorType == SURF || m_nColorType == MAT_SURF )
				{
					r = m_vMatRGBIndex[nPixValue].r;
					g = m_vMatRGBIndex[nPixValue].g;
					b = m_vMatRGBIndex[nPixValue].b;
				}
				else // m_nColorType == CELL || m_nColorType == CELL_SURF: Cell color case 
				{
					r = m_vCellRGBIndex[nPixValue].r;
					g = m_vCellRGBIndex[nPixValue].g;
					b = m_vCellRGBIndex[nPixValue].b;
				}
				fprintf(fpImagefilePtr, "%d %d %d ",r,g,b);
			}
			fprintf(fpImagefilePtr, "\n");
		}
#endif
		fclose(fpImagefilePtr); // close file of ppm or png
	}

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"    Image  : %s\n", m_chImageFileName);},
		Output.p_fpOutputFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

	return;
}