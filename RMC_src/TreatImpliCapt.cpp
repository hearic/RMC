//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"

void CDNeutronTransport::TreatImpliCapt(CDCriticality &cCriticality, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence)
{
	////////////////////////////////// treat fission /////////////////////
	cCriticality.TreatFission(*this, cAceData,cParticleState,cConvergence);
	
	/////////////// weight reduction by implicit capture //////////////////
	cCriticality.p_cAbsNeuStrength.p_dScore += cParticleState.p_dWgt*(cParticleState.p_OColNucCs.p_dAbs + cParticleState.p_OColNucCs.p_dFis)/cParticleState.p_OColNucCs.p_dTot; 
	cParticleState.p_dWgt = cParticleState.p_dWgt*(1 - (cParticleState.p_OColNucCs.p_dAbs + cParticleState.p_OColNucCs.p_dFis)/cParticleState.p_OColNucCs.p_dTot); 

	///////////////////////// russian roulette ///////////////////////////
	double  dWeightSuv;
	if( cParticleState.p_dWgt >= p_dWgtCutOff )
	{
		return;
	}

	dWeightSuv = 2*p_dWgtCutOff;
	if (ORNG.Rand() < cParticleState.p_dWgt/dWeightSuv)
	{
		cParticleState.p_dWgt = dWeightSuv; // survival
	}
	else
	{
		cParticleState.p_bIsKilled = true;        // killed
	}

	return;
}

void CDNeutronTransport::TreatImpliCapt(CDAceData &cAceData,CDParticleState &cParticleState)
{
	/////////////// weight reduction by implicit capture //////////////////
	cParticleState.p_dWgt = cParticleState.p_dWgt*(1 - cParticleState.p_OColNucCs.p_dAbs/cParticleState.p_OColNucCs.p_dTot); 

	///////////////////////// russian roulette ///////////////////////////
	double  weight_suv;
	if( cParticleState.p_dWgt >= p_dWgtCutOff )
	{
		return;
	}

	weight_suv = 2*p_dWgtCutOff;
	if (ORNG.Rand() < cParticleState.p_dWgt/weight_suv)
	{
		cParticleState.p_dWgt = weight_suv; // survival
	}
	else
	{
		cParticleState.p_bIsKilled = true;        // killed
	}

	return;
}