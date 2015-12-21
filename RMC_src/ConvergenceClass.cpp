# include "Convergence.h"

CDConvergence::CDConvergence() 
{ 
	p_bUseFscDiag = false;
	p_bUseFscAcce = false;
	p_bUseDiagFm = false;
	p_bUseAcceFm = false;
	p_bUseDiagNpm = false;
	p_bUseNpmOnFly = true;

	p_bIsFsdPrint = false;
	p_bIsFmPrint = true;

	p_nFscDiagMethod = 0;
	p_nFscAcceMethod = -1;

	p_dWldtFactor = 4.0;
	p_dWldtKe = 1.5;

	return;
 }