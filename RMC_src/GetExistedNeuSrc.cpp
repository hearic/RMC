# include "Criticality.h"

void CDCriticality::GetExistedNeuSource()
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
		return;
#endif

	char ExistedNeuSrcFileName[FILE_NAME_LENGTH];

	strcpy(ExistedNeuSrcFileName, Output.p_chInputFileName);       
	strcat(ExistedNeuSrcFileName,".source"); 
	FILE* p_fpExistedNeuSrcFilePtr = fopen(ExistedNeuSrcFileName,"r");
	if (p_fpExistedNeuSrcFilePtr == NULL)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nOpen mat print file \"%s\" failed.\n",ExistedNeuSrcFileName);},CDOutput::_ERROR);
	}

	double readTemp;
	char   sss[1000];
	int temp;


	fscanf(p_fpExistedNeuSrcFilePtr,"time step (sec):%lf",&readTemp);
	p_dDeltaTime = readTemp*1e+8;
	/*if(fabs(p_dDeltaTimeNext-readTemp*1e+8)>1e-6) 
	{
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\ntime step is not correct!\n");},CDOutput::_ERROR);
	}*/

	fscanf(p_fpExistedNeuSrcFilePtr," initial state of point equation:");
	fscanf(p_fpExistedNeuSrcFilePtr," T(0):%lf",&p_dFluxAmptitudePrev);
	for(int i=1;i<=6;i++)
	{
		fscanf(p_fpExistedNeuSrcFilePtr," c%d(0):%lf",&temp, &p_vPrecursorConcentrationPrev[i]);
	}



	fscanf(p_fpExistedNeuSrcFilePtr," delayed neutron strength:");
	for(int i=1;i<=6;i++)
	{
		fscanf(p_fpExistedNeuSrcFilePtr," group:%d, %lf",&temp, &p_vDelayedSrcStrengthPrev[i]);
	}


	fscanf(p_fpExistedNeuSrcFilePtr," average decay constant (1/sec):");
	for(int i=1;i<=6;i++)
	{
		fscanf(p_fpExistedNeuSrcFilePtr," group:%d, %lf",&temp, &p_vAveDelayedConstantPrev[i]);
		p_vAveDelayedConstantPrev[i] /= 1e+8;
	}


	p_vDelayedSrcStrengthPrev[0] = 0;
	for(int i=1;i<=6;i++)
	{
		p_vDelayedSrcStrengthPrev[i] = p_vDelayedSrcStrengthPrev[i]/(1+p_vAveDelayedConstantPrev[i]*p_dDeltaTimeNext);
		p_vDelayedSrcStrengthPrev[0] += p_vDelayedSrcStrengthPrev[i];
	}


	fscanf(p_fpExistedNeuSrcFilePtr," delayed_neutron_points:%d",&p_nDelayedNeuSrcCountPrev);
	p_vDelayedNeuSrcPrev.resize(p_nDelayedNeuSrcCountPrev);
	for(int i=0;i<p_nDelayedNeuSrcCountPrev;i++)
	{
		fscanf(p_fpExistedNeuSrcFilePtr," Particle:%d  pos = (%lf %lf %lf), dir = (%lf %lf %lf), erg = %lf",
			&temp, &p_vDelayedNeuSrcPrev[i].p_dPos[0], &p_vDelayedNeuSrcPrev[i].p_dPos[1], &p_vDelayedNeuSrcPrev[i].p_dPos[2],
			&p_vDelayedNeuSrcPrev[i].p_dDir[0], &p_vDelayedNeuSrcPrev[i].p_dDir[1], &p_vDelayedNeuSrcPrev[i].p_dDir[2],
			&p_vDelayedNeuSrcPrev[i].p_dErg);
	}


	fscanf(p_fpExistedNeuSrcFilePtr," fixed_src_points:%d",&p_nWriteSrcCount);
	if(p_nNeuNumPerCyc/p_nWriteSrcCount==0 || p_nNeuNumPerCyc%p_nWriteSrcCount!=0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nfixed source bank count is not correct!\n");},CDOutput::_ERROR);
	}

	CDFisBank tempbank;
	p_nFixedSrcSize = p_nNeuNumPerCyc;
	if(p_nFixedSrcSize>p_vFixedSrc.size()) p_vFixedSrc.resize(p_nFixedSrcSize);
	for(int i = 0;i<p_nWriteSrcCount;i++)
	{

		fscanf(p_fpExistedNeuSrcFilePtr," Particle:%d  pos = (%lf %lf %lf), dir = (%lf %lf %lf), erg = %lf",
			&temp, &tempbank.p_dPos[0], &tempbank.p_dPos[1], &tempbank.p_dPos[2],
			&tempbank.p_dDir[0], &tempbank.p_dDir[1], &tempbank.p_dDir[2],
			&tempbank.p_dErg);
		for(int j=0;j<p_nFixedSrcSize/p_nWriteSrcCount;j++)
		{
			p_vFixedSrc[p_nFixedSrcSize/p_nWriteSrcCount*i+j] = tempbank;
		}
	}

	fclose(p_fpExistedNeuSrcFilePtr);
	return;
}