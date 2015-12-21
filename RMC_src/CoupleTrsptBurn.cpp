#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::CoupleTrsptBurn(CDMaterial &cMaterial,CDTally &cTally)
{
	# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsParallelBurn)
	{
		OParallel.ParallelBurn(*this, cMaterial, cTally);
		goto strategy;
	}
	# endif


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nRunning burnup-step calculation......");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nRunning DEPTH for %d burnable cells in total. ",p_nBurnCellNum);}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Progress:       ");}, CDOutput::_NORMAL);

	//////////////////////////   coupling of RMC & DEPTH    /////////////////////////////////////
	for( int nIndex = 0; nIndex < p_nBurnCellNum; ++nIndex )
	{

		if(p_nBurnStepStrategy > 0)
		{
			p_vCellBurnNucDenOld[nIndex] = p_vCellBurnNucDen[nIndex]; // Save Beginning Status at step 0 or predictor steps
		} 

		RMC_to_DEPTH(cMaterial,cTally,nIndex);

		DEPTH_to_RMC(cMaterial,cTally,nIndex);

		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\b\b\b\b\b%3d %%%%",(nIndex + 1)*100/p_nBurnCellNum);},CDOutput::_NORMAL);
	}
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," \nBurnup-step calculation finished.\n");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
		

	////////////////////////////  update status of step strategy  //////////////////////////////////
strategy:
	if(p_nBurnStepStrategy == 0)
	{
		p_nBurnStep = p_nBurnStep + 1;
	}
	else if(p_nBurnStepStrategy > 0)   // Predictor step in Predictor-Corrector
	{
		p_nBurnStepStrategy = - p_nBurnStepStrategy;  // Predictor -> Corrector
		p_vCellBurnNucDenPre = p_vCellBurnNucDen ;    // Predictor
		p_vCellBurnNucDen = p_vCellBurnNucDenOld ;    // go back to the step beginning
	}
	else if(p_nBurnStepStrategy < 0)   // Corrector step in Predictor-Corrector
	{
		p_nBurnStep = p_nBurnStep + 1;
		p_nBurnStepStrategy = - p_nBurnStepStrategy;  // Corrector -> Predictor
	}	

# ifdef _IS_PARALLEL_
	MPI_Barrier(MPI_COMM_WORLD);
# endif

	return;
}

