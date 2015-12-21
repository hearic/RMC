# include "Output.h"

void CDOutput::PrintFile(FILE *fptr,int nPrintDest, int nMssgType)
{   
	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		if (nMssgType == CDOutput::_ERROR)
		{
			MPI_Finalize();
			exit(1);
		}	
		return;
	}
# endif


	//////////////// check and print destination and message type //////////////
	const int nWarningMsg = 1 ;
	if(nMssgType == nWarningMsg)
	{
		p_nWarningCount++;
	}

	//////////////// check and print destination and message type //////////////
	if(nPrintDest != _SCREEN && nPrintDest != _SCREEN_FILE && nPrintDest != _FILE)
	{
		printf("Warning: unknown destination to print.\n");
		return;
	}	
	if(nMssgType != _NORMAL && nMssgType != _WARNING && nMssgType != _ERROR)
	{
		printf("Warning: unknown message to print.\n");
		return;
	}

	///////////////// add prefix according to MssgType /////////////////////
	char chPrefix[3][200] = {"","Warning: ","Error: "};
	char *pStr = strcat(chPrefix[nMssgType],p_sPrintStr);
	//strcpy(Str,Prefix[MssgType]);
	/////////////////////// Print on screen/file  /////////////////////////////
	if(nPrintDest == -1 )  // only on screen
	{
		printf(pStr);
	}
	else if(nPrintDest == 1)  // only on file
	{
		fprintf(fptr,pStr);
	}
	else  // print on both screen and file
	{
		printf(pStr);
		fprintf(fptr,pStr);
	}
	////////////////////////// exit in case of error  ///////////////////////
	if(nMssgType == 2)   //  Error. Exit
	{
		printf("\nPress ENTER to exit...");
		//getchar();
# ifdef _IS_PARALLEL_
		MPI_Finalize();			
# endif
		exit(1);
	}

	return;
}