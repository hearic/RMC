# include "CalMode.h"
# include "Output.h"

void CDOutput::OutputEnding(CDCalMode &cCalMode)
{

	////////////////////// Only output when MyId==0 for parallel  ////////////////////////
	# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return ;
	}
	# endif

	//////////////////////////// Output time information at the end  //////////////////////////////
	auto FinishWallClock = time(0);         ////get system time at ending(s)
	strftime(FinishWallClockStr, sizeof(FinishWallClockStr), "%Y/%m/%d %X %A",localtime(&FinishWallClock) ); 
	
	OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nRMC Calculation Finish.\n");}, 
		p_fpOutputFilePtr, _NORMAL);

   // if(CalMode.RmcCalcMode == CalMode.PointBurnMode )
	 if(cCalMode.p_nRmcCalcMode == cCalMode.PointBurnMode )
	{
		fclose(p_fpOutputFilePtr);
		return;
	}
	
# ifdef _IS_PARALLEL_
	FinishTimeTick  = MPI_Wtime();     ////get system time at ending(s)	
	double TotCalcTime = (FinishTimeTick - StartTimeTick)/60;
# else
	FinishTimeTick  = clock();         ////get system time at ending(ms)
    double TotCalcTime = double(FinishTimeTick - StartTimeTick)/CLOCKS_PER_SEC/60;
# endif
	
	OutputMessage([&](){sprintf(Output.p_sPrintStr,"Total Calculational Time: %.4f minutes\n",TotCalcTime);}, 
		p_fpOutputFilePtr, _NORMAL);

	//if(CalMode.RmcCalcMode == CalMode.BurnupMode)
	if(cCalMode.p_nRmcCalcMode == cCalMode.BurnupMode)
	{
		OutputMessage([&](){sprintf(Output.p_sPrintStr,"Burnup Calculational Time: %.4f minutes\n",BurnupCalcTime);}, 
			p_fpOutputFilePtr, _NORMAL);
	}

	
	////////////////////////////// Output file information at the end  ///////////////////////////////
	OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nInput File = %s   Output File = %s     %s\n",p_chInputFileName,p_chOutputFileName,FinishWallClockStr);}, 
		p_fpOutputFilePtr, _NORMAL);


	///////////////////////////////////// Close all opened files  ////////////////////////////////////
	CloseFilePtrs();

	return;
}
