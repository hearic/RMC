# include "CalMode.h"

void CDCalMode::CalcPointBurnup(Depth_Class &cDepth)
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif


	////////////////////////////////////


	cDepth.StartTick = clock();    //get system time at beginning(ms)
	strcpy(cDepth.InputFile,Output.p_chInputFileName);
	//////////////////////////////// input and initiate //////////////////////////////////



	cDepth.SetDefault();
	cDepth.ReadInput();
	cDepth.Initiate();

	/////////////////////////////// Process Data Library /////////////////////////////////



	cDepth.ConvertLib();
	cDepth.ReadLib();
	cDepth.ReadSubLib();

	//////////////////////////////// execute //////////////////////////////////

	cDepth.Execute();

	//////////////////////////////// output //////////////////////////////////

	cDepth.WriteOutput();

	//////////////////////////////// finish //////////////////////////////////

	cDepth.Finish();

	/////////Read input file and copy it to output file /////////
	fprintf(Output.p_fpOutputFilePtr,"\n------------------- Calculation Summary --------------------- \n") ;

	FILE *fpRpt_fptr;
	char cStr[200];

	fpRpt_fptr = fopen(cDepth.ErrFile,"r");

	while(!feof(fpRpt_fptr))
	{
		fgets(cStr,200,fpRpt_fptr);
		fprintf(Output.p_fpOutputFilePtr,"%s",cStr);
	}
	fprintf(Output.p_fpOutputFilePtr,"\n\n") ;

}