# include "Depth_Class.h"
# include "TTA_Class.h"
# include "ME_Class.h"

void Depth_Class::Initiate()
{ 
	/////////////////// Check Burnup step/time/flux/power /////////////////////
	if(StepNum < 1)
	{
		ErrorWarning("None burnup step!",0);
		return;
	}

	CmltvTime.resize(StepNum + 1);
	CmltvTime[0] = 0 ;
	for(int i = 1;i <= StepNum;++i)// Check Time Flux Power
	{
		if(StepFlux[i] < 0)
		{
			ErrorWarning("Wrong depletion flux!",1);
		}
		if(StepPower[i] < 0)
		{
			ErrorWarning("Wrong depletion power!",1);
		}
		if(StepTime[i] < 0)
		{
			ErrorWarning("Wrong depletion time!",1);
		}
		if(StepFlux[i] == 0 && StepPower[i] == 0)
		{
			StepMode[i] = 0;          // zero Flux&Power  =  Decay
		}   

		CmltvTime[i] = CmltvTime[i-1] + StepTime[i] ;
	}


	////////////////// initiate Nt and other output vectors ////////////////
	StepNt.clear();
	FissionRate.clear();
	Radioactivity.clear();
	FissionRate.clear();
	DecayHeat.clear();
	StepNt.resize(StepNum+1);
	if(RadioactivityFlag>0)
	{
		Radioactivity.resize(StepNum+1);
	}
	if(AbsRateFlag>0)
	{
		AbsorpRate.resize(StepNum+1);
	}
	if(FissionRateFlag>0)
	{
		FissionRate.resize(StepNum+1);
	}
	if(DecayHeatFlag>0)
	{
		DecayHeat.resize(StepNum+1);
	}
	for(int i=0;i<=StepNum;++i)
	{
		StepNt[i].resize(NucNum+1);
		if(RadioactivityFlag>0)
		{
			Radioactivity[i].resize(NucNum+1);
		}
		if(AbsRateFlag>0)
		{
			AbsorpRate[i].resize(NucNum+1);
		}
		if(FissionRateFlag>0)
		{
			FissionRate[i].resize(NucNum+1);
		}
		if(DecayHeatFlag>0)
		{
			DecayHeat[i].resize(NucNum+1);
		}
	}

	for(int i=1;i<=NucNum;++i)
	{
		if(N0[i]<0)   // gram unit is given
		{
			N0[i] = 0.60221367*N0[i]/((GetNucId(i)%10000)/10);    // transfer to atomic unit ( n/barn )
		}
		StepNt[0][i]=N0[i];
	}	

	return;
}