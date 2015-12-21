# include "Plot.h"

void CDPlot::ResetPlot(CDGeometry &cGeometry)
{/*
	1. RecoverBoundSurf
	2. Clear Vector and release space
	3. Write End Time
*/
	//RecoverBoundSurf
	RecoverSurfBound(cGeometry);

	//Clear Vector and release space
	m_vPixelMatrix2D.clear();
	vector< vector< int>>().swap(m_vPixelMatrix2D);
	m_vPixelData3D.clear();
	vector< int>().swap(m_vPixelData3D);
	m_vMatRGBIndex.clear();
	vector< stRGB>().swap(m_vMatRGBIndex);
	m_vCellRGBIndex.clear();
	vector< stRGB>().swap(m_vCellRGBIndex);

	////////  Write End Time /////////
	char chWallClockStr[64];
	auto FinishWallClock = time(0);     //get system time at Beginning(s)
	strftime(chWallClockStr, sizeof(chWallClockStr), "%Y/%m/%d %X",localtime(&FinishWallClock) ); 
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Plotting finished!                 [ %s ] \n",
		chWallClockStr);}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);


# ifdef _IS_PARALLEL_
	FinishPlotTimeTick  = MPI_Wtime();     ////get system time at ending(s)	
	double TotCalcTime = (FinishPlotTimeTick - StartPlotTimeTick)/60;
# else
	FinishPlotTimeTick  = clock();         ////get system time at ending(ms)
	double TotCalcTime = double(FinishPlotTimeTick - StartPlotTimeTick)/CLOCKS_PER_SEC/60;
# endif

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Total Plotting Time: %.4f minutes\n\n",TotCalcTime);}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	fclose(Output.p_fpPlotFilePtr);

	return;
}