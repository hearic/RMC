#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::PrintStepMssg()
{
	string sPcStatus = "";
	if(p_nBurnStep == p_nTotBurnStep)
	{
		sPcStatus = "";
	}
	else if(p_nBurnStepStrategy > 0)
	{
		sPcStatus = "(Predictor)";
	}
	else if(p_nBurnStepStrategy < 0)
	{
		sPcStatus = "(Corrector)";
	}

	if (p_bIsBurnStepBegun)
	{
		if (p_nBurnStepStrategy >= 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n************ New burnup-step *************\n");},
				Output.p_fpOutputFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);//New burnup step
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******************************************\n");},
				Output.p_fpOutputFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);//New burnup step
		}
		
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n\n");},
			Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n\n");},
			Output.p_fpTallyFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
	}

	///////////////// Step beginning (before criticality calculation ) /////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nCurrent burnup step%s: %d/%d\nTotal step time(Day): %-9.3f  Total Burnup(MWD/KgHM): %-9.3f\n\n",
		sPcStatus.c_str(),p_nBurnStep,p_nTotBurnStep,p_vStepCmltvBurnTime[p_nBurnStep],p_vStepCmltvBurnup[p_nBurnStep]);},
		Output.p_fpOutputFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Current burnup step%s: %d/%d\nTotal step time(Day): %-9.3f  Total Burnup(MWD/KgHM): %-9.3f\n\n",
		sPcStatus.c_str(),p_nBurnStep,p_nTotBurnStep,p_vStepCmltvBurnTime[p_nBurnStep],p_vStepCmltvBurnup[p_nBurnStep]);},
		Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Current burnup step%s: %d/%d\nTotal step time(Day): %-9.3f  Total Burnup(MWD/KgHM): %-9.3f\n",
		sPcStatus.c_str(),p_nBurnStep,p_nTotBurnStep,p_vStepCmltvBurnTime[p_nBurnStep],p_vStepCmltvBurnup[p_nBurnStep]);},
		Output.p_fpTallyFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	return;
}