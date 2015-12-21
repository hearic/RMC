# include "Plot.h"


void CDPlot::OutputPlotFile(int nPlotTimes, CDGeometry &cGeometry, CDMaterial &cMaterial)
{
	if (nPlotTimes == 0)
	{
		//Output Colors
		fprintf(Output.p_fpPlotFilePtr,"1.  Plot Colors (Scheme %d):\n", p_unColorScheme);
		fprintf(Output.p_fpPlotFilePtr,"    ----------------------------------------\n");
		fprintf(Output.p_fpPlotFilePtr,"    DESCRIPTION          COLOR(R,G,B)\n");
		fprintf(Output.p_fpPlotFilePtr,"    ----------------------------------------\n");
		if (m_bIsEverSurfColor)fprintf(Output.p_fpPlotFilePtr,"    cell boundary        %-3d, %-3d, %-3d(black)\n",0,0,0);
		if (m_bIsEverMatColor)
		{
			fprintf(Output.p_fpPlotFilePtr,"    mat  0               %-3d, %-3d, %-3d(white)\n",255,255,255);
			for (int i = 2; i < m_vMatRGBIndex.size(); i++)
			{
				int nMat = i - 1;
				fprintf(Output.p_fpPlotFilePtr,"    mat  %-3d             %-3d, %-3d, %-3d\n",cMaterial.p_OMatSetIndex.GetIndexU(nMat), m_vMatRGBIndex[i].r,m_vMatRGBIndex[i].g,m_vMatRGBIndex[i].b);
			}
		}
		fprintf(Output.p_fpPlotFilePtr,"\n");
		if (m_bIsEverCellColor)
		{
			for (int i = 2; i < m_vCellRGBIndex.size(); i++)
			{
				int nCell = i - 1 ;
				fprintf(Output.p_fpPlotFilePtr,"    Cell %-3d             %-3d, %-3d, %-3d\n",cGeometry.p_OCellIndex.GetIndexU(nCell), m_vCellRGBIndex[i].r,m_vCellRGBIndex[i].g,m_vCellRGBIndex[i].b);
			}
		}
		fprintf(Output.p_fpPlotFilePtr,"    ----------------------------------------\n");
		fprintf(Output.p_fpPlotFilePtr,"\n\n");

		//Output Image Parameters
		fprintf(Output.p_fpPlotFilePtr,"2.  Image Parameters:\n");
	}
	
	// write image specific information
	vector<string> srTypeDescrp, srColorDescrp; 
	srTypeDescrp.push_back("Slice");
	srTypeDescrp.push_back("Box");
	srColorDescrp.push_back("MAT - Materials colored"); 
	srColorDescrp.push_back("CELL - Cells colored"); 
	srColorDescrp.push_back("SURF - Cell boundaries showed"); 
	srColorDescrp.push_back("MATSURF - Materials colored and cell boundaries showed"); 
	srColorDescrp.push_back("CELLSURF - Cells colored and cell boundaries showed"); 

	fprintf(Output.p_fpPlotFilePtr,"PlotID:  %d\n",m_nPlotID);
	fprintf(Output.p_fpPlotFilePtr,"    Plot type            :  %s\n",srTypeDescrp[m_nPlotType-1].c_str());
	fprintf(Output.p_fpPlotFilePtr,"    Color type           :  %s\n",srColorDescrp[m_nColorType-1].c_str());
	fprintf(Output.p_fpPlotFilePtr,"    Image file           :  %s\n", m_chImageFileName);
	
	if (m_nPlotType == 1) // Slice
	{	
		fprintf(Output.p_fpPlotFilePtr,"    Vertex1(top-left)    : %11.6f %11.6f %11.6f \n", m_OVertex1._x,m_OVertex1._y,m_OVertex1._z);
		fprintf(Output.p_fpPlotFilePtr,"    Vertex2(top-right)   : %11.6f %11.6f %11.6f \n", m_OVertex2._x,m_OVertex2._y,m_OVertex2._z);
		fprintf(Output.p_fpPlotFilePtr,"    Vertex3(bottom-right): %11.6f %11.6f %11.6f \n", m_OVertex3._x,m_OVertex3._y,m_OVertex3._z);
		fprintf(Output.p_fpPlotFilePtr,"    Image width          : %11d\n", m_nPixel_1);
		fprintf(Output.p_fpPlotFilePtr,"    Image height         : %11d\n", m_nPixel_2);
		fprintf(Output.p_fpPlotFilePtr,"    1 pixel width        : %11.6f\n", m_dDeltaWidth1);
		fprintf(Output.p_fpPlotFilePtr,"    1 pixel height       : %11.6f\n", m_dDeltaWidth2);
	}
	
	fprintf(Output.p_fpPlotFilePtr,"\n");

	return;
}