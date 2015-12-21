# include "Criticality.h"

void CDCriticality::OutputInnerProduct()
{
	//////////////////////// Only output when MyId==0 for parallel  /////////////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif


	//////////////////////////////////// output to file ////////////////////////////////////
	if( p_nCurrentCYCLE == p_nInactCycNum + 1 )
	{	
		fprintf(Output.p_fpInnerProductPtr,"================== inner product ==========================\n");
	}

	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		fprintf(Output.p_fpInnerProductPtr,"%-6d  %-f  %-f  %-f\n",
			p_nCurrentCYCLE,p_cNeuPopu.p_dCycAve,p_cNeuPopu.p_dCycAve,p_cNeuPopu.p_dCycStd); 
	}

	return;
}
