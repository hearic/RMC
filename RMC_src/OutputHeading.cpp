# include "Output.h"
# include "Criticality.h"
# include "Version.h"

void CDOutput::OutputHeading(CDCriticality &cCriticality)
{
	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return ;
	}
# endif


	char s[200];
	FILE *inp_fptr;
	p_fpOutputFilePtr = fopen(p_chOutputFileName,"w"); 

	////////  Write Beginning Time /////////	
	auto StartWallClock = time(0);     //get system time at Beginning(s)
	strftime(StartWallClockStr, sizeof(StartWallClockStr), "%Y/%m/%d %X %A",localtime(&StartWallClock) ); 

# ifdef _IS_PARALLEL_
	StartTimeTick = MPI_Wtime();
# else
    StartTimeTick = clock(); //get system time at Beginning(ms)
# endif
	//C_Criticality.TrsptStartTimeTick = StartTimeTick; //get system time at Beginning(ms)
	cCriticality.TrsptStartTimeTick = StartTimeTick; 
	
	////////////////////////// Write RMC LOGO///////////////////////
	printf("Copyrights Reserved by Reactor Engineering Analysis Laboratory(REAL).\n\n");
	printf("      RRRRRRRR       MM         MM        CCCCCCCC   \n");
	printf("      RRRRRRRRR      MMM       MMM      CCCCCCCCC    \n");
	printf("      RRR    RRR     MMMM     MMMM     CCC           \n");
	printf("      RRR    RR      MMMMM   MMMMM    CCC            \n");
	printf("      RRRRRRRR       MM MMM MMM MM    CCC            \n");
	printf("      RRR   RRR      MM  MMMMM  MM    CCC            \n");
	printf("      RRR    RRR     MM   MMM   MM     CCCCCCCCCC    \n");
	printf("      RRR     RRR   MMM         MMM      CCCCCCCCC   \n");
	printf("\nCode version: %s\n", CODE_VERSION);
	printf("\nRelease date: %s %s\n",__TIME__,__DATE__);
	printf("\n");
	fprintf(p_fpOutputFilePtr,"Copyrights Reserved by Reactor Engineering Analysis Laboratory(REAL).\n\n");
	fprintf(p_fpOutputFilePtr,"      RRRRRRRR       MM         MM        CCCCCCCC   \n");
	fprintf(p_fpOutputFilePtr,"      RRRRRRRRR      MMM       MMM      CCCCCCCCC    \n");
	fprintf(p_fpOutputFilePtr,"      RRR    RRR     MMMM     MMMM     CCC           \n");
	fprintf(p_fpOutputFilePtr,"      RRR    RR      MMMMM   MMMMM    CCC            \n");
	fprintf(p_fpOutputFilePtr,"      RRRRRRRR       MM MMM MMM MM    CCC            \n");
	fprintf(p_fpOutputFilePtr,"      RRR   RRR      MM  MMMMM  MM    CCC            \n");
	fprintf(p_fpOutputFilePtr,"      RRR    RRR     MM   MMM   MM     CCCCCCCCCC    \n");
	fprintf(p_fpOutputFilePtr,"      RRR     RRR   MMM         MMM      CCCCCCCCC   \n");
	fprintf(p_fpOutputFilePtr,"\nCode version: %s\n", CODE_VERSION);
	fprintf(p_fpOutputFilePtr,"\nRelease date: %s %s\n",__TIME__,__DATE__);
	fprintf(p_fpOutputFilePtr,"\n");

	////////////////////////// Start time and input file name///////////////////////
	printf("RMC Calculation Start.\nInput File = %s     %s \n\n",p_chInputFileName,StartWallClockStr); 
	fprintf(p_fpOutputFilePtr,"RMC Calculation Start.\nInput File = %s     %s \n\n",p_chInputFileName,StartWallClockStr); 
	
	//////////////////Read input file and copy it to output file ////////////////
	fprintf(p_fpOutputFilePtr,"------------------ Input File Start -------------------- \n") ;

	inp_fptr=fopen(p_chInputFileName,"r");

	while(!feof(inp_fptr))
	{
		if(fgets(s,200,inp_fptr))
		{
			fprintf(p_fpOutputFilePtr,"%s",s);
		}
	}
	fprintf(p_fpOutputFilePtr,"\n---------------------- Input File End------------------------ \n\n") ;

	fclose(inp_fptr);
}