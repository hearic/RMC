# include "Criticality.h"
# include "Tally.h"
# include "Convergence.h"
# include "FixedSource.h"

void CDCriticality::TrackHistory(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,CDTally &cTally)
{


	if(cParticleState.p_bIsKilled)       //// found wrong cell
	{
		return;
	}

	int nMaxIter = 10000 ;
	p_nCollisionCount = 0;
	for(;;)
	{
		////////////////  save collision information //////////////////
		cNeutronTransport.OutputSrcInfo(cAceData,cParticleState,p_nCurrentCYCLE, p_nCurrentPARTICLE, p_nCollisionCount); 



		/////////////// geometry tracking : free flying ///////////////
		cNeutronTransport.GeometryTracking(*this, cGeometry,cMaterial,cAceData,cParticleState,cTally);
		if(cParticleState.p_bIsKilled) 
		{
			p_cLeakNeuStrength.p_dScore += cParticleState.p_dWgt;
			break;
		}  


		/////////////////// sample collision nuclide /////////////////
		cNeutronTransport.SampleColliNuc(*this, cMaterial,cAceData,cParticleState);  
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		} 	

		//////////////////// calculate cross-sections /////////////////
		cNeutronTransport.CalcColliNucCs(cAceData,cParticleState);  	


		////////// treat implicit capture(including fission) //////////
		cNeutronTransport.TreatImpliCapt(*this, cAceData,cParticleState,cConvergence);
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		}  


		//////////////////// sample collision type ///////////////////
		cParticleState.p_nColType = cNeutronTransport.SampleColliType_Cri(cAceData,cParticleState);
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		} 	

		////////////////////// sample exit state /////////////////////
		cNeutronTransport.GetExitState(*this,cAceData,cParticleState);
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		}  
				

		///////////////////// Update particle state //////////////////
		cParticleState.UpdateNeuState();	


		////////// error tolerance of too many collisions ////////////
		if((p_nCollisionCount++) > nMaxIter ) 
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"too many collisions.\n");},CDOutput::_WARNING);
			p_nCollisionCount = 0;
			break;
		}
	}


	p_llTotCollisionCount = p_llTotCollisionCount + p_nCollisionCount;
	return;
}

void CDFixedSource::TrackHistory(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally)
{


	if(cParticleState.p_bIsKilled)       //// found wrong cell
	{
		return;
	}

	int MaxIter = 10000 ;
	p_nCollisionCount = 0;
	for(;;)
	{
		////////////////  save collision information //////////////////
		cNeutronTransport.OutputSrcInfo(cAceData,cParticleState,0,p_nCurrentPARTICLE, p_nCollisionCount); 



		/////////////// geometry tracking : free flying ///////////////
		cNeutronTransport.GeometryTracking(*this, cGeometry,cMaterial,cAceData,cParticleState,cTally);
		if(cParticleState.p_bIsKilled) 
		{
			break;
		}  


		/////////////////// sample collision nuclide /////////////////
		cNeutronTransport.SampleColliNuc(cMaterial,cAceData,cParticleState);  
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		} 	

		//////////////////// calculate cross-sections /////////////////
		cNeutronTransport.CalcColliNucCs(cAceData,cParticleState);  	


		////////// treat implicit capture(EXcluding fission) //////////
		cNeutronTransport.TreatImpliCapt(cAceData,cParticleState);   //weight reduction
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		}  


		//////////////////// sample collision type ///////////////////
		cParticleState.p_nColType = cNeutronTransport.SampleColliType_Fix(cAceData,cParticleState);
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		} 	

		////////////////////// sample exit state /////////////////////
		cNeutronTransport.GetExitState(*this, cAceData,cParticleState);
		if(cParticleState.p_bIsKilled)  
		{
			break; 
		}  


		///////////////////// Update particle state //////////////////
		cParticleState.UpdateNeuState();	


		////////// error tolerance of too many collisions ////////////
		if((p_nCollisionCount++) > MaxIter ) 
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"too many collisions.\n");},CDOutput::_WARNING);
			p_nCollisionCount = 0;
			break;
		}
	}


	p_llTotCollisionCount = p_llTotCollisionCount + p_nCollisionCount;
	return;
}