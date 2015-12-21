# include "Plot.h"

void CDPlot::InitPlot(CDGeometry &cGeometry, CDMaterial &cMaterial)
{/*
	Write beginning time
	Set Reflective Surf
	Generating RGB Index
*/
	//Get Start Time
# ifdef _IS_PARALLEL_
	StartPlotTimeTick = MPI_Wtime();//(s)
# else
	StartPlotTimeTick = clock(); //get system time at Beginning(ms)
# endif

	////////  Write Beginning Time /////////
	char WallClockStr[64];
	auto local_time = time(0);     //get system time at Beginning(s)
	strftime(WallClockStr, sizeof(WallClockStr), "%Y/%m/%d %X",localtime(&local_time) ); 

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nRun Plotting.....                  [ %s ] \n",WallClockStr);}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	//Backup Surf Bound and Set 0 to non-normal surf bound condition
	BackupSurfBound( cGeometry );

	//Generating RGB for every material
	GenRGBIndex( cGeometry, cMaterial );

	return;
}
