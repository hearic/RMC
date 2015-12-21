# include "Plot.h"

void CDPlot::UpdatePlotParas(CDOneImageInput &cImage)
{
	//transfer paras of one image to current plotting
	m_nPlotID = cImage.p_nPlotID;
	m_nPlotType = cImage.p_nPlotType;
	m_nColorType = cImage.p_nColorType;
	m_nPixel_1 = cImage.p_nPixel_1;
	m_nPixel_2 = cImage.p_nPixel_2;
	m_nPixel_3 = cImage.p_nPixel_3;
	m_OVertex1 = cImage.p_OVertex1;
	m_OVertex2 = cImage.p_OVertex2;
	m_OVertex3 = cImage.p_OVertex3;
	m_OVertex4 = cImage.p_OVertex4;
	m_bIsLowResolution = false;
	////////  Write Local Time  to OutputFile and Screen/////////
	char chWallClockStr[64];
	auto local_time = time(0);     //get system time at Beginning(s)
	strftime(chWallClockStr, sizeof(chWallClockStr), "%Y/%m/%d %X",localtime(&local_time) ); 	
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"  Plotting image of PlotID: %d......[ %s ] \n", m_nPlotID,chWallClockStr);}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	
	//Generate Directions
	//TrackDirection and HeightDirection
	m_ODir1 = m_OVertex2 - m_OVertex1;
	m_ODir2 = m_OVertex3 - m_OVertex2;	
	m_ODir3 = m_OVertex4 - m_OVertex3;
	//pixel size
	m_dDeltaWidth1 = m_ODir1.GetVecLength()/m_nPixel_1;
	m_dDeltaWidth2 = m_ODir2.GetVecLength()/m_nPixel_2;
	m_dDeltaWidth3 = m_ODir3.GetVecLength()/m_nPixel_3;
	//Normalize
	m_ODir1.Normalize();
	m_ODir2.Normalize();
	m_ODir3.Normalize();
	//check pixels
	if (m_nPlotType == 1 && m_dDeltaWidth1 != m_dDeltaWidth2 )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"    ");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," Image sizes are not consistent with actual geometry sizes.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
	}

	return;
}