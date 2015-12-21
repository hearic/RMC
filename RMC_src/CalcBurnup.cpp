# include "CalMode.h"
# include "Criticality.h"

void CDCalMode::CalcBurnup(CDNeutronTransport &cNeutronTransport, CDAceData& cAceData, CDCriticality &cCriticality, CDMaterial &cMaterial, CDGeometry &cGeometry, 
	CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup)
{
	Output.BurnupCalcTime = 0 ;

	cBurnup.PrintStepMssg();

	cCriticality.InitiateAll(cNeutronTransport,cGeometry,cMaterial,cConvergence,cTally, cAceData, cParticleState,cBurnup);

	cBurnup.InitiateBurnup(cGeometry,cMaterial,cTally);

	//////////////////////////// criticality calculation /////////////////////////
	for(;;)
	{		
		////////////////// Cycle Start ////////////////////
		for (int nCyc = 1;nCyc <= cCriticality.p_nTotCycNum;nCyc++)
		{
			cCriticality.p_nCurrentCYCLE=nCyc;

			for (int nNeu = 1;nNeu <= cCriticality.p_nCycleNeuNum;nNeu++)
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


			cCriticality.ProcessCycleEnd(cNeutronTransport,cMaterial,cConvergence,cTally, cAceData);
		}

		Output.OutputSummary(cNeutronTransport,cCriticality,cTally,cMaterial);
		

# ifdef _IS_PARALLEL_
		double dTime1 = MPI_Wtime();
# else
		time_t nTime1  = clock();
# endif
		
		//////////////////////////// CBurnup calculation /////////////////////////
		////////// calculate power distribution  //////////
		cBurnup.CalcCellPower(cMaterial,cTally);

		////////// get cell average energy  //////////
		cBurnup.GetCellAveEgy(cTally);

		//////// Output CBurnup Calculation Results ////////
		cBurnup.OutputBurnup(cMaterial,cTally,cCriticality,cAceData.p_vNuclides);


		////////// Couple criticality and burnup //////////
		if(cBurnup.p_nBurnStep < cBurnup.p_nTotBurnStep)
		{

			cBurnup.CoupleTrsptBurn(cMaterial,cTally);

# ifdef _IS_PARALLEL_
			double dTime2 = MPI_Wtime();
			Output.BurnupCalcTime = Output.BurnupCalcTime + (dTime2 - dTime1)/60;
# else
			time_t nTime2  = clock();
			Output.BurnupCalcTime = Output.BurnupCalcTime + double(nTime2 - nTime1)/CLOCKS_PER_SEC/60;
# endif

			cBurnup.RestBurnup(cNeutronTransport,cCriticality,cGeometry,cMaterial,cTally,cAceData, cConvergence);

			////////////// Generate Input File////////////////
			CDFixedSource cTempFixsource;
			Output.GenerateInpFile(cGeometry, cMaterial, cAceData, cCriticality, cTally, cConvergence, cBurnup, cTempFixsource);
			
			continue;
		}

# ifdef _IS_PARALLEL_
		double dTime2 = MPI_Wtime();
		Output.BurnupCalcTime = Output.BurnupCalcTime + (dTime2 - dTime1)/60;
# else
		long long nTime2  = time(0);
		Output.BurnupCalcTime = Output.BurnupCalcTime + difftime(nTime2,nTime1)/60;
# endif

		break;
	}


	///////// Print Keffs of all burnup steps /////////
	cBurnup.PrintKeffTable();
		
	return;
}