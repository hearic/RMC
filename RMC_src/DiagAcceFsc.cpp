
# include "Convergence.h"

void CDConvergence::DiagAcceFsc(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality)
{
	////////////////////// no diagnostic and acceleration ////////////////////
	if(!p_bUseFscDiag && !p_bUseFscAcce)
	{
		return;
	}


	////// ///////////////////Diagnose of source convergence //////////////////////////
	DiagFsc(cCriticality); /// Diagnose by SE, FM and NPM


	////////////////////////// Acceleration source convergence  /////////////////////////
	AcceFsc(cNeutronTransport, cCriticality);


	ResetFsc(cCriticality);
	return;
}