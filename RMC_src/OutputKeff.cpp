# include "Criticality.h"

void CDCriticality::OutputKeff()
{
	//////////////////////// Only output when MyId==0 for parallel  /////////////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	//////////////////////////////////// output to scree ////////////////////////////////////
# ifdef _IS_PARALLEL_
	double CycleTimeTick = MPI_Wtime();
	double CycCpuTime = (CycleTimeTick - TrsptStartTimeTick)/60;
# else
    time_t CycleTimeTick = clock(); //get system time at Beginning(ms)
	double CycCpuTime = double(CycleTimeTick - TrsptStartTimeTick)/CLOCKS_PER_SEC/60.0 ;
# endif
	
	
	if( p_nCurrentCYCLE == p_nInactCycNum + 1 )
	{	
		printf("************* Start Active Cycle *************\n");
		printf("Cycle      Active       Keff        Average        Std      Time(min)\n");
 	}
	else if( p_nCurrentCYCLE == 1 )
	{
		printf("\n************ Start Inactive Cycle ************\n");
		printf("Cycle        Keff      Time(min)\n");
	}	

	if(p_nCurrentCYCLE <= p_nInactCycNum)
	{
		printf("%-6d     %-f     %-.4f\n",p_nCurrentCYCLE,p_dKeffCyc[0],CycCpuTime);
	}
	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		printf("%-6d     %-6d     %-f     %-f     %-f     %-.4f\n",
			p_nCurrentCYCLE,p_nCurrentCYCLE-p_nInactCycNum,p_dKeffCyc[0],p_dKeffIndAve[0],p_dKeffIndStd[0],CycCpuTime);
	}


	//////////////////////////////////// output to file ////////////////////////////////////
	if( p_nCurrentCYCLE == p_nInactCycNum + 1 )
	{	
        fprintf(Output.p_fpOutputFilePtr,"================== start active cycles ==================================================================================================\n");
	}
	else if( p_nCurrentCYCLE == 1 )
	{
		fprintf(Output.p_fpOutputFilePtr,"\n=========================================================================================================================================\n");
		fprintf(Output.p_fpOutputFilePtr,"                      keff estimators by cycle             individual average keff and deviation           combined average keff \n");
		fprintf(Output.p_fpOutputFilePtr,"cycle   history      k(col)    k(abs)     k(tl)     k(col)   st dev     k(abs)  st dev      k(tl) st dev     k(c/a/t)  st dev    Time(min) \n");
	}	
	else if( p_nCurrentCYCLE % 10 == 1 )
	{
		fprintf(Output.p_fpOutputFilePtr,"=========================================================================================================================================\n");
	}


	if(p_nCurrentCYCLE <= p_nInactCycNum)
	{
		fprintf(Output.p_fpOutputFilePtr,"%-6d  %-9d | %-f  %-f  %-f |                                                                                %-.4f\n",
			p_nCurrentCYCLE,p_nTotFisBankNum,p_dKeffCyc[0],p_dKeffCyc[1],p_dKeffCyc[2],CycCpuTime);       //write Keff file  
	}
	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		fprintf(Output.p_fpOutputFilePtr,"%-6d  %-9d | %-f  %-f  %-f | %-f %-f  %-f %-f  %-f %-f | %-f %-f |  %-.4f\n",
			p_nCurrentCYCLE,p_nTotFisBankNum,p_dKeffCyc[0],p_dKeffCyc[1],p_dKeffCyc[2],
			p_dKeffIndAve[0],p_dKeffIndStd[0],p_dKeffIndAve[1],p_dKeffIndStd[1],p_dKeffIndAve[2],p_dKeffIndStd[2],
			p_dKeffCovwAve[3],p_dKeffCovwStd[3],CycCpuTime); 
	}

	return;
}