# include "Tally.h"
# include "Criticality.h"
# include "Convergence.h"

void CDCriticality::ResetCriticality()
{
	////////////  reset fission source ////////////	
	p_nFisBankCount = 0;
	p_nFisSrcCount = 0;
	p_nDelayedNeuBankCount =0;
	p_nTimeAbsNeuBankCount=0;
	////////////  reset  Keff  ////////////	

	p_dKeffWgtSum[0]=0;
	p_dKeffWgtSum[1]=0;
	p_dKeffWgtSum[2]=0;

	p_cNeuPopu.p_dScore=0;
	for(int i=0;i<7;i++) 
	{
		p_cFisNeuStrength[i].p_dScore=0;
		p_cAveDelayedConstant[i].p_dScore=0;
	}
	p_cLeakNeuStrength.p_dScore=0;
	p_cAbsNeuStrength.p_dScore=0;

	return;
}


//void CDFixedSource::ResetTrspt()
//{
//	////////////  reset fission source ////////////	
//	p_nFixedSrcBankCount = 0;
//	p_nFixedSrcCount = 0;
//
//	return;
//}