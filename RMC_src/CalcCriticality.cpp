# include "CalMode.h"
# include "Criticality.h"

void CDCalMode::CalcCriticality(CDNeutronTransport &cNeutronTransport, CDAceData& cAceData, CDCriticality &cCriticality, CDMaterial &cMaterial, CDGeometry &cGeometry, 
	CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup)
{
	///////// Initiate All Criticality //////////


	cCriticality.InitiateAll(cNeutronTransport,cGeometry,cMaterial,cConvergence,cTally, cAceData, cParticleState,cBurnup);


	////////////////////////////////////// Cycle Start////////////////////////////////
	int nNeu,nCyc;
	for (nCyc = 1;nCyc <= cCriticality.p_nTotCycNum; nCyc++)
	{

		cCriticality.p_nCurrentCYCLE = nCyc;

		for (nNeu = 1;nNeu <= cCriticality.p_nCycleNeuNum; nNeu++)
		{
			cCriticality.p_nCurrentPARTICLE = nNeu ;

//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
//# endif


			//cCriticality.TrackOneNeu(cGeometry,cMaterial,cAceData,cParticleState,cConvergence,cTally);
			///////////////  sample source particle //////////////////////
			cCriticality.SampleFisSource(cNeutronTransport,cGeometry,cParticleState,cConvergence); 


			//////////////////  neutron history  /////////////////////////
			if(cConvergence.p_nFscAcceMethod == 1 || cConvergence.p_nFscAcceMethod == 2)
			{
				cConvergence.WldtTrackHistory(cNeutronTransport, cCriticality,cGeometry,cMaterial, cAceData, cParticleState, cConvergence,cTally);
			}
			else
			{
				cCriticality.TrackHistory(cNeutronTransport, cGeometry,cMaterial,cAceData,cParticleState,cConvergence,cTally);
			}

			if(cTally.p_bIsPerHstry)
			{
				cTally.SumUpTally(0, cCriticality);   // fixed-source calculations
			}
		}

		//Output.OutputMessage([&](){sprintf(Output.PrintStr,"    Debug info: histories in cycle %d finished!\n",Cyc);},Output.OutputFilePtr,CD_Output::_SCREEN_FILE,CD_Output::_NORMAL); 


		cCriticality.ProcessCycleEnd(cNeutronTransport,cMaterial,cConvergence,cTally, cAceData);
	}

	if((cCriticality.p_bIsQuasiStaticS || cCriticality.p_bIsQuasiStaticD))
	{
		//point kinetics calculation
		cCriticality.PointKinetics();

		cCriticality.ProcessDelayedNeuStrength();
		cCriticality.ProcessTimeAbsNeuStrength();
		cCriticality.ProcessPower();
		cCriticality.ProcessFixedSrc();
		cCriticality.WriteSrc();
	}

	Output.OutputSummary(cNeutronTransport,cCriticality,cTally,cMaterial);


	return;
}
