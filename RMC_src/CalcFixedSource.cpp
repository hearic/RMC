# include "CalMode.h"
# include "ParallelClass.h"

void CDCalMode::CalcFixedSource(CDNeutronTransport &cNeutronTransport, CDAceData& CAceData, CDFixedSource &cFixedSource, CDMaterial &CMaterial, CDGeometry &CGeometry, 
	CDParticleState &CParticleState,CDTally &CTally)
{
	//printf("I am in CDCalMode::CalcFixedSource.\n");

	///////// Initiate All Fixedsource //////////
	CDBurnup CBurnup;

	cFixedSource.InitiateAll(cNeutronTransport,CGeometry,CMaterial,CTally, CAceData, CParticleState,CBurnup);

	int Neu;
	for (Neu = 1;Neu <= cFixedSource.p_nNeuNum; Neu++)
	{
		cFixedSource.p_nCurrentPARTICLE = Neu ;
# ifdef _IS_PARALLEL_
		if(OParallel.p_bIsMaster)
		{
			if(Neu%1000==0) printf("neutron : %d\n",Neu*OParallel.p_nProcsNum);
		}
# else
		if(Neu%1000==0) printf("neutron : %d\n",Neu);
# endif



//# ifdef _IS_PARALLEL_
		ORNG.GetRandSeed();
//# endif

		//cFixedSource.TrackOneNeu(CGeometry,CMaterial,CAceData,CParticleState,CTally);
		///////////////  sample source particle //////////////////////
		cFixedSource.SampleFixSource(cNeutronTransport,CGeometry,CParticleState); 

		//////////////////  neutron history  /////////////////////////
		for(;;)
		{
			cFixedSource.TrackHistory(cNeutronTransport, CGeometry,CMaterial,CAceData,CParticleState,CTally);
			if(cFixedSource.p_nFixedSrcBankCount>0)
			{
				//printf("fixedsrcbankcound=%d\n",p_nFixedSrcBankCount);
				cFixedSource.SampleFisSource(cNeutronTransport,CGeometry,CParticleState);
			}
			else
			{
				break;
			}
		}



		CTally.SumUpTally(0, cFixedSource);   // fixed-source calculations
	}

	CTally.ProcessTally(cFixedSource, CMaterial, CAceData);        //  process tallies at the end of each cycle;
	Output.OutputSummary(cNeutronTransport,cFixedSource,CTally,CMaterial);
	return;

}