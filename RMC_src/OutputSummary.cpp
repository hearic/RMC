# include "Output.h"
# include "Criticality.h"

void CDOutput::OutputSummary(CDNeutronTransport &cNeutronTransport,CDCriticality &cCriticality,CDTally &cTally, CDMaterial &cMaterial)
{
	///////////////// Only output when MyId==0 for parallel  ///////////////////


	long long llTotCollisionCount = cCriticality.p_llTotCollisionCount;
	int  nMissParticleCount = cNeutronTransport.p_nMissParticleCount;

# ifdef _IS_PARALLEL_


	MPI_Reduce(&cCriticality.p_llTotCollisionCount,&llTotCollisionCount,1,MPI_LONG_LONG_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&cNeutronTransport.p_nMissParticleCount,&nMissParticleCount,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Allreduce(MPI_IN_PLACE,&p_nWarningCount,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif


	cTally.OutputTally(1,cCriticality, cMaterial); //// output final tally

	if(p_nWarningCount > 0)
	{
		OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n%d warning found.\n",p_nWarningCount);}, 
			p_fpOutputFilePtr, CDOutput::_NORMAL);
	}

	if(nMissParticleCount > 0)
	{

		OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n%d particle missing\n",nMissParticleCount);}, 
			p_fpOutputFilePtr, CDOutput::_NORMAL);
	}


	double dAveCollisionCount = llTotCollisionCount/(cCriticality.p_dTotStartWgt*cCriticality.p_nTotCycNum);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nAverage collisions per particle: %.2f \n", dAveCollisionCount);}, 
		p_fpOutputFilePtr, CDOutput::_NORMAL);


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nFinal Keff: %f      Standard Deviation: %f\n", cCriticality.p_dKeffCovwAve[3],cCriticality.p_dKeffCovwStd[3]);}, 
		p_fpOutputFilePtr, CDOutput::_NORMAL);
	return;
}


void CDOutput::OutputSummary(CDNeutronTransport &cNeutronTransport,CDFixedSource &cFixedSource,CDTally &CTally, CDMaterial &CMaterial)
{
	///////////////// Only output when MyId==0 for parallel  ///////////////////


	long long TotCollisionCount = cFixedSource.p_llTotCollisionCount;
	int  MissParticleCount = cNeutronTransport.p_nMissParticleCount;

# ifdef _IS_PARALLEL_


	MPI_Reduce(&cFixedSource.p_llTotCollisionCount,&TotCollisionCount,1,MPI_LONG_LONG_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&cNeutronTransport.p_nMissParticleCount,&MissParticleCount,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Allreduce(MPI_IN_PLACE,&p_nWarningCount,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif


	CTally.OutputTally(1,cFixedSource, CMaterial); //// output final tally

	if(p_nWarningCount > 0)
	{
		OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n%d warning found.\n",p_nWarningCount);}, 
			p_fpOutputFilePtr, CDOutput::_NORMAL);
	}

	if(MissParticleCount > 0)
	{

		OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n%d particle missing\n",MissParticleCount);}, 
			p_fpOutputFilePtr, CDOutput::_NORMAL);
	}


	double AveCollisionCount = TotCollisionCount/(cFixedSource.p_dTotStartWgt);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nAverage collisons per particle: %.2f \n", AveCollisionCount);}, 
		p_fpOutputFilePtr, CDOutput::_NORMAL);

	return;
}