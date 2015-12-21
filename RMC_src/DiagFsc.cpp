# include "Convergence.h"

void CDConvergence::DiagFsc(CDCriticality& cCriticality)  
{

    ProcessSe(cCriticality);

	ProcessNpm(cCriticality);

	ProcessFm(cCriticality);

	return;
}