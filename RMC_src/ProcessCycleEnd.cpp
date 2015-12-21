# include "Tally.h"
# include "Criticality.h"
# include "Convergence.h"


void CDCriticality::ProcessCycleEnd(CDNeutronTransport &cNeutronTransport, CDMaterial &cMaterial,CDConvergence &cConvergence,CDTally &cTally, CDAceData& cAceData)
{

	ProcessKeff(cConvergence);


	if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
	{
		ProcessNeuPopu();
		ProcessFisNeuStrength();
	}


	if(!p_bIsQuasiStaticD)
		ProcessBank(cNeutronTransport,cConvergence);
	else
		ProcessBank_QuasiStatic(cNeutronTransport);

	if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
	{
		ProcessDelayedNeuBank();

		ProcessTimeAbsNeuBank();
	}

	

	ResetCriticality();                //  reset parameters for next cycle 


	cTally.ProcessTally(*this, cMaterial, cAceData);        //  process tallies at the end of each cycle;  
}
