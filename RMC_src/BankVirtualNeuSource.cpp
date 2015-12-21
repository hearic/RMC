# include "Criticality.h"

void CDCriticality::BankTimeAbsNeuSource(CDAceData &cAceData, CDParticleState &cParticleState)
{
	//used only in the last active cycle
	//or, can be used in the last several cycles to reduce the variance

	double dNeuMass = 939.58;
	double dSlight = 299.7925;

	double dVel=0;
	if(cAceData.p_bIsMultiGroup)
	{
		double dErg = cAceData.GetMgCentErg(int(cParticleState.p_dErg+0.5));
		dVel = dSlight*sqrt(dErg*(dErg+2*dNeuMass))/(dErg+dNeuMass);
	}
	else
	{
		dVel = dSlight*sqrt(cParticleState.p_dErg*(cParticleState.p_dErg+2*dNeuMass))/(cParticleState.p_dErg+dNeuMass);
	}


	double dTimeAbsWgt = cParticleState.p_dWgt/dVel/p_dDeltaTimeNext/(cParticleState.p_dMacroTotCs+p_dTimeAbsMacroCs);

	p_dTimeAbsSrcStrength += dTimeAbsWgt;

	int nNeuNum = int(dTimeAbsWgt/10.0 + ORNG.Rand()); 
	if(nNeuNum>50) printf("warning: time step may be too small!\n");

	for(int i=0;i<nNeuNum;i++)
	{
		if(p_bIsRecordTimeAbsNeu)
		{
			if(p_nTimeAbsNeuBankCount>=p_vTimeAbsNeuBank.size())
			{
				p_vTimeAbsNeuBank.resize(int(1.2*p_nTimeAbsNeuBankCount)+1);
			}

			for (int j = 0;j < 3; j++)
			{
				p_vTimeAbsNeuBank[p_nTimeAbsNeuBankCount].p_dPos[j] =  cParticleState.p_dPos[j];
				p_vTimeAbsNeuBank[p_nTimeAbsNeuBankCount].p_dDir[j] =  cParticleState.p_dDir[j];
			}
			p_vTimeAbsNeuBank[p_nTimeAbsNeuBankCount].p_dErg = cParticleState.p_dErg;

			p_nTimeAbsNeuBankCount = p_nTimeAbsNeuBankCount + 1;
		}

		//p_dTimeAbsSrcStrength += 1;
	}

}

void CDCriticality::BankDelayedNeuSource(CDParticleState &cParticleState)
{
	if(p_bIsRecordDelayedNeu)
	{
		if(p_nDelayedNeuBankCount>=p_vDelayedNeuBank.size())
		{
			p_vDelayedNeuBank.resize(int(1.2*p_nDelayedNeuBankCount)+1);
		}

		for (int j = 0;j < 3; j++)
		{
			p_vDelayedNeuBank[p_nDelayedNeuBankCount].p_dPos[j] =  cParticleState.p_dPos[j];
			p_vDelayedNeuBank[p_nDelayedNeuBankCount].p_dDir[j] = cParticleState.p_dExitDir[j];
		}

		p_vDelayedNeuBank[p_nDelayedNeuBankCount].p_dErg = cParticleState.p_dExitErg;

		p_nDelayedNeuBankCount = p_nDelayedNeuBankCount + 1;
	}

}