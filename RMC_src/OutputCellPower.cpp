#include "Burnup.h"

void CDBurnup::OutputCellPower(CDTally &CTally)
{

	///////////////////////// output burnup information to .burn  /////////////////////////
	# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
	# endif


	/////////////////// Note: output information is from step 0 to step N /////////////////
	int nBurnStep = p_nBurnStep;
	

	////////////////////// output power and depletion distribution  ///////////////////////
	//////// open file ///////////////
	char chOutputFilePower[200];
	FILE *fpOutPower;
	strcpy(chOutputFilePower,Output.p_chInputFileName);
	strcat(chOutputFilePower,".burn.power");
	if(nBurnStep == 0)  // Beginning step
	{
		fpOutPower = fopen(chOutputFilePower,"w");	  // Note: ios = "w"
	}
	else
	{
		fpOutPower = fopen(chOutputFilePower,"a");	  // Note: ios = "a"
	}

	///////  output total burnup & time oclock ////////
	char chOutputClock[64];
	time_t tLocalTime = time(0);     //get system time at Beginning(s)
	strftime(chOutputClock, sizeof(chOutputClock), "%Y/%m/%d %X %A",localtime(&tLocalTime) );

	if(nBurnStep == 0)
	{
		fprintf(fpOutPower,"************  Power/Burnup distribution in the beginning  ******************   %s\n",chOutputClock);
	}
	else
	{
		fprintf(fpOutPower,"************  Power/Burnup distribution at the end of step %-3d  ************   %s\n",nBurnStep,chOutputClock);
	}
	fprintf(fpOutPower,"  Total step time(Day): %-12.6E , Total Burnup(MWD/KgHM): %12.6E\n",
		p_vStepCmltvBurnTime[nBurnStep],p_vStepCmltvBurnup[nBurnStep]);
	
	int nBurnStep_power = p_nBurnStep < p_nTotBurnStep? p_nBurnStep + 1 : p_nTotBurnStep;
	fprintf(fpOutPower,"Power Density(MW/KgHM): %-12.6E ,              Power(MW): %12.6E\n",
			p_vStepBurnPowerDen[nBurnStep_power]/1000,p_vStepBurnPower[nBurnStep_power]);


	/////// output power and depletion distribution ///////////
	fprintf(fpOutPower,"Cell                        Power(MW)       Burnup(MWD/KgHM)     Vol. FLux(cm/s)       NAECF(eV)\n");
	for(int i = 0;i < p_nBurnCellNum; ++i )
	{
		int nTallyPos = p_nBurnTallyStartPos + i ;
		fprintf(fpOutPower,"%-24s  %-12.6E        %-12.6E        %-12.6E        %-12.6E\n",
			Output.PrintCellVec(CTally.p_vOneGroupCsTally[nTallyPos].p_vCellVecU),
					p_vCellBurnPower[i],p_vCellCmltvBurnup[i],p_vCellActualFlux[i],p_vCellBurnAveEnergy[i]);
	}
	//fprintf(fpOutPower,"Actual Flux Ratio (ActualFlux/TallyFlux) = %-12.6E\n", p_dActualFluxRatio);

	fprintf(fpOutPower,"\n\n");
	fclose(fpOutPower);

	return;
}