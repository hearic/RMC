# include "NeutronTransport.h"
# include "FixedSource.h"
# include "Criticality.h"

void CDNeutronTransport::OutputSrcInfo(CDAceData &cAceData,CDParticleState &cParticleState, int nCycle, int nCurrParticle,
	int nCollisionCount)
{	 
	if(Output.p_bIsSrcPrint == false)
	{
		return;

	}

	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	if(p_nCurrentPARTICLE == 1 && nCollisionCount == 0)
	{
		if(nCycle>0) fprintf(Output.p_fpSourceFilePtr,"Cycle: %d\n",nCycle);	
	}
	if(nCollisionCount == 0)
	{
		fprintf(Output.p_fpSourceFilePtr,"Particle: %d\n",nCurrParticle);	
	}

	if(cAceData.p_bIsMultiGroup)
	{
		fprintf(Output.p_fpSourceFilePtr,"  C = %d, r = (%6.4E %6.4E %6.4E), erg = %d, w = %6.4E\n",
			nCollisionCount,cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],
			cParticleState.ErgGrp(),cParticleState.p_dWgt);	
	}
	else
	{
		fprintf(Output.p_fpSourceFilePtr,"  C = %d, r = (%6.4E %6.4E %6.4E), e = %6.4E, w = %6.4E\n",
			nCollisionCount,cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],
			cParticleState.p_dErg,cParticleState.p_dWgt);	
	}

	return;
}


void CDCriticality::WriteSrc()
{	 

	if(Output.p_bIsSrcWrite == false)
	{
		return;

	}

	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif
	char SourceFileName[FILE_NAME_LENGTH];

	strcpy(SourceFileName, Output.p_chInputFileName);       
	strcat(SourceFileName,".source"); 
	FILE* p_fpSrcFilePtr = fopen(SourceFileName,"w");
	if (p_fpSrcFilePtr == NULL)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nOpen Source print file \"%s\" failed.\n",SourceFileName);},CDOutput::_ERROR);
	}

	fprintf(p_fpSrcFilePtr,"time step (sec):%10.4E \n\n\n",p_dDeltaTimeNext*1e-8);

	fprintf(p_fpSrcFilePtr,"initial state of point equation: \n");
	fprintf(p_fpSrcFilePtr,"T(0):%6.4E \n",p_dFluxAmptitude);
	for(int i=1;i<=6;i++)
	{
		fprintf(p_fpSrcFilePtr,"c%d(0):%6.4E \n",i,p_vPrecursorConcentration[i]);
	}


	fprintf(p_fpSrcFilePtr,"\n\ndelayed neutron strength: \n");
	for(int i=1;i<=6;i++)
	{
		fprintf(p_fpSrcFilePtr,"group:%d, %12.4E \n",i, p_dDelayedSrcStrength[i]+p_vDelayedSrcStrengthPrev[i]);
	}

	fprintf(p_fpSrcFilePtr,"\n\naverage decay constant (1/sec): \n");
	for(int i=1;i<=6;i++)
	{
		fprintf(p_fpSrcFilePtr,"group:%d, %12.4E \n",i, 1e+8*p_cAveDelayedConstant[i].p_dCycAve);
	}

	////////////////////// print the above information to innerproduct ////////////////////
	fprintf(Output.p_fpInnerProductPtr,"\n\ntime step (sec):%10.4E \n\n",p_dDeltaTimeNext*1e-8);

	fprintf(Output.p_fpInnerProductPtr,"initial state of point equation: \n");
	fprintf(Output.p_fpInnerProductPtr,"T(0):%6.4E \n",p_dFluxAmptitude);
	for(int i=1;i<=6;i++)
	{
		fprintf(Output.p_fpInnerProductPtr,"c%d(0):%6.4E \n",i,p_vPrecursorConcentration[i]);
	}


	fprintf(Output.p_fpInnerProductPtr,"\n\ndelayed neutron strength: \n");
	for(int i=1;i<=6;i++)
	{
		fprintf(Output.p_fpInnerProductPtr,"group:%d, %12.4E \n",i, p_dDelayedSrcStrength[i]+p_vDelayedSrcStrengthPrev[i]);
	}

	fprintf(Output.p_fpInnerProductPtr,"\n\naverage decay constant (1/sec): \n");
	for(int i=1;i<=6;i++)
	{
		fprintf(Output.p_fpInnerProductPtr,"group:%d, %12.4E \n",i, 1e+8*p_cAveDelayedConstant[i].p_dCycAve);
	}



	//////////////////// write the neutron source /////////////////////////////////
	fprintf(p_fpSrcFilePtr,"\n\ndelayed_neutron_points:%d \n",p_nDelayedNeuSrcCount);
	for(int i=0;i<p_nDelayedNeuSrcCount;i++)
	{
		fprintf(p_fpSrcFilePtr,"Particle:%7d",i);
		fprintf(p_fpSrcFilePtr,"  pos = (%12.4E %12.4E %12.4E), dir = (%12.4E %12.4E %12.4E), erg = %12.4E \n",
			p_vDelayedNeuSrc[i].p_dPos[0],p_vDelayedNeuSrc[i].p_dPos[1],p_vDelayedNeuSrc[i].p_dPos[2],
			p_vDelayedNeuSrc[i].p_dDir[0],p_vDelayedNeuSrc[i].p_dDir[1],p_vDelayedNeuSrc[i].p_dDir[2],
			p_vDelayedNeuSrc[i].p_dErg);
	}


	fprintf(p_fpSrcFilePtr,"\n\n\n\nfixed_src_points:%d \n",p_nFixedSrcSize);
	for(int i=0;i<p_nFixedSrcSize;i++)
	{
		fprintf(p_fpSrcFilePtr,"Particle:%7d ",i);
		fprintf(p_fpSrcFilePtr,"  pos = (%12.4E %12.4E %12.4E), dir = (%12.4E %12.4E %12.4E), erg = %12.4E \n",
			p_vFixedSrc[i].p_dPos[0],p_vFixedSrc[i].p_dPos[1],p_vFixedSrc[i].p_dPos[2],
			p_vFixedSrc[i].p_dDir[0],p_vFixedSrc[i].p_dDir[1],p_vFixedSrc[i].p_dDir[2],
			p_vFixedSrc[i].p_dErg);
	}

	fclose(p_fpSrcFilePtr);
	return;
}