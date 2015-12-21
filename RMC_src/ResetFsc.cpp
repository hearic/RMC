
# include "Convergence.h"

void CDConvergence::ResetFsc(CDCriticality& cCriticality)
{

	


	if(p_bUseFscDiag)
	{
		for( int i = 0 ; i < p_OShanEntropy.p_vCycMeshSrc.size(); ++i)
		{
			p_OShanEntropy.p_vCycMeshSrc[i] = 0;
		}
	}
	if(p_bUseDiagNpm) 
	{
		if(cCriticality.p_nCurrentCYCLE >= cCriticality.p_nInactCycNum)
		{
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				CDGlobeFun::Set1DVecZero(p_vNpMatrix[i].p_vS1);
			}
		}
	}


	if(p_nFscAcceMethod == 1)
	{	
		p_dWldtKe = p_dWldtFactor/(p_dWldtFactor-1)*cCriticality.p_dKeffFinal;
		p_dWldtKw =1/(1/cCriticality.p_dKeffFinal - 1/p_dWldtKe);
	}

	else if(p_nFscAcceMethod == 2)
	{
		p_dSuphisTotStartWeight = 0;
		//printf("%d\n",C_Criticality.BankFissionCount);
	}


}