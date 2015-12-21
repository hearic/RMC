# include "Output.h"

void CDOutput::CloseFilePtrs()
{
	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return ;
	}
# endif


	///////////////////////// Open all kinds of output files  ////////////////////
	fclose(p_fpOutputFilePtr);

	if(p_fpMatFilePtr != NULL)
	{
		fclose(p_fpMatFilePtr);
	} 

	if(p_fpTallyFilePtr != NULL)
	{
		fclose(p_fpTallyFilePtr);

		if(p_fpCycTallyFilePtr != NULL)
		{
			fclose(p_fpCycTallyFilePtr);
		}
	} 

	if(p_fpFscFilePtr != NULL)
	{
		fclose(p_fpFscFilePtr);
	}

	if(p_fpSourceFilePtr != NULL)
	{
		fclose(p_fpSourceFilePtr);
	} 

	
	return;
}