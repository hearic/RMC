# include "Tally.h"
# include "Convergence.h"

void CDConvergence::WldtTrackHistory(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality,CDGeometry& cGeometry,CDMaterial& cMaterial, CDAceData& cAceData, CDParticleState& cParticlestate, CDConvergence& cConvergence,CDTally &cTally) ///Wielandt's method   2010-12-13
{
	p_nWldtBankCount = 0;
	p_nWldtSampleCount = 0;

	if(p_nFscAcceMethod == 2)
	{
		p_nSuphisCycleCount = 1;
		p_dSuphisTotStartWeight = p_dSuphisTotStartWeight + cParticlestate.p_dWgt;
	}

	for(;;)
	{
		cCriticality.TrackHistory(cNeutronTransport, cGeometry,cMaterial,cAceData,cParticlestate,cConvergence,cTally);
		
		if(p_nFscAcceMethod == 1)      //// wielandt acceleration
		{
			if(p_nWldtBankCount != p_nWldtSampleCount)    // NOTE: should be behind goingto end !!
			{
				WldtSampleSource(cNeutronTransport,cCriticality, cParticlestate, cGeometry);
				continue;
			}
			break;
		}
		else if(p_nFscAcceMethod == 2) //// wielandt acceleration
		{
			if(p_nWldtBankCount != p_nWldtSampleCount)
			{
				WldtSampleSource(cNeutronTransport,cCriticality, cParticlestate, cGeometry);
				p_dSuphisTotStartWeight = p_dSuphisTotStartWeight + cParticlestate.p_dWgt;
				continue;
			}
			break;
		}
	}

	return;
}