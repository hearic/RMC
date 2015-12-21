# include "Convergence.h"


void CDConvergence::FscTallySrc(const double dStartPos[3], double dWgt, CDCriticality& cCriticality)
{
	//////////////////// Tally mesh index for shannon entropy ///////////////
	p_OShanEntropy.TallyFisSource(dStartPos,dWgt);

	//////////////////// Tally mesh index for FM ////////////////
	if(p_bUseDiagFm && cCriticality.p_nCurrentCYCLE > cCriticality.p_nInactCycNum)
	{
		for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
		{
			p_vDiagFisMatrix[i].TallyBornSrc(dStartPos,dWgt);
		}
	}

	//////////////////// Tally mesh index for NPM matrix ////////////////
	if(p_bUseDiagNpm && cCriticality.p_nCurrentCYCLE > cCriticality.p_nInactCycNum)
	{
		for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
		{
			p_vNpMatrix[i].TallySrcVec(dStartPos,dWgt);
		}
	}

	return;
}


void CDConvergence::FscTallyFm(const double dPos[3],double dFisWgt,CDCriticality& cCriticality) 
{
	for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
	{
		p_vDiagFisMatrix[i].TallyFisMatrix(dPos, cCriticality.p_dKeffFinal * dFisWgt);
	}
	return;
}
