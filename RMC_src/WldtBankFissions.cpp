
# include "Convergence.h"

# define lg2 log10(2.0)


void CDConvergence::WldtBankFissions(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDCriticality& cCriticality,CDParticleState &cParticleState,int nFisMT,double dWielandt_R)
{
	int nWldtBankNeuNum = int(dWielandt_R + ORNG.Rand());
	for(int i = 0;i < nWldtBankNeuNum; i++)
	{

		if(p_nWldtBankCount >= p_vWldtSourceBank.size())
		{
			p_vWldtSourceBank.resize(2*p_nWldtBankCount);
			if(p_nFscAcceMethod == 2)
			{
				p_vSuphisSourceCycleBank.resize(2*p_nWldtBankCount);
			}
		}

		if(p_nFscAcceMethod == 2)
		{
			p_vSuphisSourceCycleBank[p_nWldtBankCount] = p_nSuphisCycleCount + 1;
		}

		///////////////////////////// Get fission neutron state //////////////////////////////////
		if(cAceData.p_bIsMultiGroup)
		{
			cAceData.GetMgFisErgDir(cParticleState.p_nNUC,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
		}
		else
		{
			cNeutronTransport.GetCeExitState(cAceData,cParticleState,cParticleState.p_nNUC,nFisMT,cParticleState.p_dErg,cParticleState.p_dDir,
				false,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
		}


		for (int j=0;j<3;j++)
		{
			p_vWldtSourceBank[p_nWldtBankCount].p_dPos[j] =  cParticleState.p_dPos[j];
			p_vWldtSourceBank[p_nWldtBankCount].p_dDir[j] = cParticleState.p_dExitDir[j];
		}
		p_vWldtSourceBank[p_nWldtBankCount].p_dErg = cParticleState.p_dExitErg;

		p_nWldtBankCount = p_nWldtBankCount + 1;	
	}
	return;
}
