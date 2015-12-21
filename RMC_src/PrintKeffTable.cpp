#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::PrintKeffTable()
{

	////////////////////// Only output when MyId==0 for parallel  ////////////////////////
	# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return ;
	}
	# endif

	////////////// Print keff table at burnup end  /////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n***********Summary of burnup calculation***********\n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Step    Time(Day)   Burnup(MWD/KgHM)    Keff \n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	for( int i = 0; i <= p_nTotBurnStep; ++i )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%-6d   %-9.3f       %-9.3f    %-9.6f\n",
			i,p_vStepCmltvBurnTime[i],p_vStepCmltvBurnup[i],p_vStepFinalKeff[i]);}, 
			Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	}

	return;
}