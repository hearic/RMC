# include "Criticality.h"
# include "FixedSource.h"
# include "Tally.h"
# include "Convergence.h"

void CDCriticality::CheckCriticality()
{
	    //// check Population Block //
	if ( p_dKeffFinal <= 0.1 || p_dKeffFinal > 10)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial Keff is too large or too small!\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
	if(p_nInactCycNum >= p_nTotCycNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"insufficient active cycles!\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}


	return;
}

void CDFixedSource::CheckFixedSource()
{

}