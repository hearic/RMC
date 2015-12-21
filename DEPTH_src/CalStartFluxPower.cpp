# include "Depth_Class.h"
const double BarnMev = 1.60217653E-19 ;// 0.60221367*1.60217653E-19;

////// XSigR=X*Sig*R, which is used to calculate FLUX or POWER ////
// X   =  amount of fissile nuclide i in fuel (g atom);
// Sig =  microscopic fission cross section for nuclide i(barn);
// R = recoverable energy per fission for nuclide i (MeV/fission)
void Depth_Class::CalStartFluxPower()
{ 
	/////////////// Decay mode  or zero Flux&Power /////////////////
	if(MODE == 0)      
	{
		FLUX = 0;
		POWER = 0;
		StepFlux[STEP] = 0;
		StepPower[STEP] = 0;
	}

	/////////////// Flux mode /////////////////
	else if(MODE == 1)
	{
		FLUX = StepFlux[STEP];		

		if(MeanPowerFlag == 0)  // if MeanPowerFlag = 1, POWER is calculated at the end of the burnup step
		{
			double PowerFactor = CalcPowerFactor(StepNt[STEP-1]);
			POWER = PowerFactor*StepFlux[STEP];
			StepPower[STEP] = POWER;
		}
		
	}

	/////////////// Power mode /////////////////
	else if(MODE == 2)
	{
	    POWER = StepPower[STEP];
		
		///// Calculate flux at the beginning of current step ////
		double PowerFactor = CalcPowerFactor(StepNt[STEP-1]);
		if (PowerFactor == 0)
		{
			StartFlux = 0;// may be caused by Oscillation
		}
		else
		{
			StartFlux = StepPower[STEP]/PowerFactor;
		}
		FLUX = StartFlux;
		StepFlux[STEP] = FLUX;
	}
	return;
}