# include "Criticality.h"
# include "Convergence.h"
# include "FixedSource.h"

static int FisMt[5] = {18,19,20,21,38};

void CDCriticality::BankFisSourceMg(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence)
{
	//////////////////////////// Only for multi-group case  //////////////////////////////
	/////////////////////// wielandt fission bank ////////////////////////
	if(cConvergence.p_nFscAcceMethod == 1)
	{
		double dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * cParticleState.p_OColNucCs.p_dFis/cParticleState.p_OColNucCs.p_dTot/cConvergence.p_dWldtKw;  // fission bank
		GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,-1,dFisR);

		double dWielandtFisR = dFisR*cConvergence.p_dWldtKw/cConvergence.p_dWldtKe;
		cConvergence.WldtBankFissions(cNeutronTransport, cAceData,*this,cParticleState,-1,dWielandtFisR);
		return;
	}
	////////////////////// superhistory fission bank //////////////////////
	if(cConvergence.p_nFscAcceMethod == 2)
	{
		double dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * cParticleState.p_OColNucCs.p_dFis/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;

		if(cConvergence.p_nSuphisCycleCount < cConvergence.p_nSuphisFactor)
		{
			cConvergence.WldtBankFissions(cNeutronTransport, cAceData,*this,cParticleState,-1,dFisR);
		}
		else if(cConvergence.p_nSuphisCycleCount == cConvergence.p_nSuphisFactor)
		{
			GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,-1,dFisR);
		}
		return;
	}

	/////////////////////// ordinary fission bank ////////////////////////
	//double dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * cParticleState.p_OColNucCs.p_dFis/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;
	double dFisR;
	if(!p_bIsQuasiStaticD)
		dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * cParticleState.p_OColNucCs.p_dFis/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;
	else
		dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * cParticleState.p_OColNucCs.p_dFis/cParticleState.p_OColNucCs.p_dTot;

	GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,-1,dFisR);

	return;
}

void CDCriticality::BankFisSource(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,double dFisSubCs[5])
{
	/////////////////////////// Only for continuous energy case ///////////////////////////
	/////////////////////////// wielandt fission bank /////////////////////////////////////
	if(cConvergence.p_nFscAcceMethod == 1)
	{
		for(int j = 0; j <= 4; j++)
		{
			if(dFisSubCs[j] > 0)
			{
				double dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * dFisSubCs[j]/cParticleState.p_OColNucCs.p_dTot/cConvergence.p_dWldtKw;  // fission bank
				GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,FisMt[j],dFisR);

				double Wielandt_R = dFisR*cConvergence.p_dWldtKw/cConvergence.p_dWldtKe;
				cConvergence.WldtBankFissions(cNeutronTransport, cAceData,*this,cParticleState,FisMt[j],Wielandt_R);

				if(j == 0){break;}   //总裂变截面存在
			}
		}
		return;
	}
	////////////////////// superhistory fission bank //////////////////////
	if(cConvergence.p_nFscAcceMethod == 2)
	{
		for(int j = 0; j <= 4; j++)
		{
			if(dFisSubCs[j] > 0)
			{
				double dFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * dFisSubCs[j]/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;

				if(cConvergence.p_nSuphisCycleCount < cConvergence.p_nSuphisFactor)
				{
					cConvergence.WldtBankFissions(cNeutronTransport, cAceData,*this,cParticleState,FisMt[j],dFisR);
				}
				else if(cConvergence.p_nSuphisCycleCount == cConvergence.p_nSuphisFactor)
				{
					GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,FisMt[j],dFisR);
				}
				if(j == 0){break;}   //总裂变截面存在
			}
		}
		return;
	}

	/////////////////////// ordinary fission bank ////////////////////////
	for(int j = 0;j <= 4;j++)
	{
		if(dFisSubCs[j] > 0)
		{
			//double nFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * dFisSubCs[j]/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;
			double nFisR;
			if(!p_bIsQuasiStaticD)
				nFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * dFisSubCs[j]/cParticleState.p_OColNucCs.p_dTot/p_dKeffFinal;
			else
				nFisR = cParticleState.p_dWgt * cParticleState.p_OColNucCs.p_dNu * dFisSubCs[j]/cParticleState.p_OColNucCs.p_dTot;

			GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cConvergence,FisMt[j],nFisR);
			if(j == 0)
			{
				break;
			}   //总裂变截面MT=18存在
		}
	}
	return;
}


void CDCriticality::GetFissionNeuState(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,int nFisMT, double dFisWgt)
{
	int nNuc = cParticleState.p_nNUC;
	double dErg = cParticleState.p_dErg;

	int nFisNeuNum = int(dFisWgt + ORNG.Rand()); 

	double dNuDelayed = cAceData.GetDelayedNu(dErg,nNuc);
	double dBeta = dNuDelayed/cParticleState.p_OColNucCs.p_dNu;


	bool bFisNeuExist = true;

	for(int i = 0; i < nFisNeuNum;i++)
	{
		if(p_nFisBankCount >= p_vFissionBank.size())
		{
			p_vFissionBank.resize(int(1.2*p_nFisBankCount) + 1);   /// fixed bug in case of very small FisBankCount 
			p_vFissionSrc.resize(int(1.2*p_nFisBankCount) + 1);
		}
		///////////////////////////// sample prompt/delayed fission neutrons //////////////////////////////////
		if(ORNG.Rand() < dBeta)
		{
			int nNPCR = cAceData.GetNPCR(nNuc);
			int dLoc = cAceData.GetLocOfBDD(nNuc);
			double dKsi = ORNG.Rand();
			double dProbSum = 0;
			int j = 1;
			for(j = 1; j <= nNPCR; ++j)
			{
				int nNR = int(cAceData.p_vNuclides[nNuc].XSS[dLoc + 1]);
				int nNE = int(cAceData.p_vNuclides[nNuc].XSS[dLoc + 2 + 2*nNR]);

				double dYield = cAceData.GetErgFuncValue(nNuc, dLoc + 1, dErg);

				dProbSum = dProbSum + dYield;

				if(dKsi < dProbSum)
				{ 
					break;
				}
				dLoc = dLoc + 2 + 2*nNR + 2*nNE + 1;
			}

			j = std::min(j,nNPCR);
			double lambda = cAceData.p_vNuclides[nNuc].XSS[dLoc];

			if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
			{
				p_cFisNeuStrength[j].p_dScore  = p_cFisNeuStrength[j].p_dScore +1.0;

				if(p_nCurrentCYCLE>p_nInactCycNum)
				{
					if(!p_bIsQuasiStaticD)
					{
						p_dDelayedSrcStrength[j] += 1.0/(1. + lambda*p_dDeltaTimeNext);
						p_dDelayedSrcStrength[0] += 1.0/(1. + lambda*p_dDeltaTimeNext);
					}
					else
					{
						p_dDelayedSrcStrength[j] += lambda*p_dDeltaTime/(1. + lambda*p_dDeltaTime)/(1. + lambda*p_dDeltaTimeNext);
						p_dDelayedSrcStrength[0] += lambda*p_dDeltaTime/(1. + lambda*p_dDeltaTime)/(1. + lambda*p_dDeltaTimeNext);
					}

				}

				p_cAveDelayedConstant[j].p_dScore += 1.0*lambda;
			}
			

			int nLawType = 0;
			int	nLDAT = 0;
			cAceData.GetLawType(nNuc,-j,dErg,nLawType,nLDAT);

			double dExitErg;
			double dMu = 2*ORNG.Rand() - 1;
			cAceData.ReactByLaws(nNuc,-1,nLawType, nLDAT,dErg, dExitErg,dMu);

			double dPhi = 2 * 3.1415927 * ORNG.Rand();
			cParticleState.p_dExitDir[0] = dMu;
			cParticleState.p_dExitDir[1] = sqrt(1 - dMu*dMu) * cos(dPhi);
			cParticleState.p_dExitDir[2] = sqrt(1 - dMu*dMu) * sin(dPhi);

			cParticleState.p_dExitErg = dExitErg;

			if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
			{
				if(p_nCurrentCYCLE>p_nInactCycNum)
					BankDelayedNeuSource(cParticleState);

				if( p_bIsQuasiStaticD && int(ORNG.Rand()+lambda*p_dDeltaTime/(1. + lambda*p_dDeltaTime)) < 1 )
					bFisNeuExist = false;
			}
			
		}


		///////////////////////////////// Get fission neutron state //////////////////////////////////////////
		else
		{
			if(cAceData.p_bIsMultiGroup)
			{
				//determine delayed neutron or prompt neutron
				double ksi = ORNG.Rand();
				double sum=0;
				int i;
				for(i=0;i<7;i++)
				{
					sum += cAceData.p_vBeta[i];
					if(ksi<=sum) break;
				}

				if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
				{
					p_cFisNeuStrength[i].p_dScore  = p_cFisNeuStrength[i].p_dScore +1.0;
					if(i>0) 
					{
						p_cAveDelayedConstant[i].p_dScore += 1.0*cAceData.p_vLambda[i];
						if(p_nCurrentCYCLE>p_nInactCycNum)
						{
							if(!p_bIsQuasiStaticD)
							{
								p_dDelayedSrcStrength[i] += 1.0/(1. + cAceData.p_vLambda[i]*p_dDeltaTimeNext);
								p_dDelayedSrcStrength[0] += 1.0/(1. + cAceData.p_vLambda[i]*p_dDeltaTimeNext);
							}
							else
							{
								p_dDelayedSrcStrength[i] += cAceData.p_vLambda[i]*p_dDeltaTime/(1. + cAceData.p_vLambda[i]*p_dDeltaTime)/(1. + cAceData.p_vLambda[i]*p_dDeltaTimeNext);
								p_dDelayedSrcStrength[0] += cAceData.p_vLambda[i]*p_dDeltaTime/(1. + cAceData.p_vLambda[i]*p_dDeltaTime)/(1. + cAceData.p_vLambda[i]*p_dDeltaTimeNext);
							}

						}

						if(p_bIsQuasiStaticD && int(ORNG.Rand()+cAceData.p_vLambda[i]*p_dDeltaTime/(1. + cAceData.p_vLambda[i]*p_dDeltaTime)) < 1 )
							bFisNeuExist = false;
					}
				}
				

				cAceData.GetMgFisErgDir(nNuc,cParticleState.p_dExitErg,cParticleState.p_dExitDir);

				if(p_bIsQuasiStaticS || p_bIsQuasiStaticD)
				{
					if(p_nCurrentCYCLE>p_nInactCycNum && i>0)
						BankDelayedNeuSource(cParticleState);
				}
				
			}
			else
			{
				p_cFisNeuStrength[0].p_dScore = p_cFisNeuStrength[0].p_dScore + 1.0;
				cNeutronTransport.GetCeExitState(cAceData,cParticleState,nNuc,nFisMT,dErg,cParticleState.p_dDir,false,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
			}
		}


		if(bFisNeuExist)
		{
			for (int j = 0;j < 3; j++)
			{
				p_vFissionBank[p_nFisBankCount].p_dPos[j] =  cParticleState.p_dPos[j];
				p_vFissionBank[p_nFisBankCount].p_dDir[j] = cParticleState.p_dExitDir[j];
			}

			p_vFissionBank[p_nFisBankCount].p_dErg = cParticleState.p_dExitErg;

			p_nFisBankCount = p_nFisBankCount + 1;
		}
	}

	////////////////////////////////////////
	if(cConvergence.p_bUseDiagFm && p_nCurrentCYCLE > p_nInactCycNum)
	{
		if(dFisWgt > 0)
		{
			cConvergence.FscTallyFm(cParticleState.p_dPos,dFisWgt, *this);
		}
	}

	return;
}


void CDFixedSource::GetFissionNeuState(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,int FisMT)
{
	int nuc = cParticleState.p_nNUC;
	double erg = cParticleState.p_dErg;

	//int FisNeuNum = int(FisWgt + ORNG.Rand()); 

	double nu_delayed = cAceData.GetDelayedNu(erg,nuc);
	double beta = nu_delayed/cParticleState.p_OColNucCs.p_dNu;
	int FisNeuNum = int(cParticleState.p_OColNucCs.p_dNu+ORNG.Rand());

	for(int i = 0; i < FisNeuNum;i++)
	{
		if(p_nFixedSrcBankCount >= p_vFixedSrcBank.size())
		{
			p_vFixedSrcBank.resize(int(1.2*p_nFixedSrcBankCount) + 1);   /// fixed bug in case of very small FisBankCount 
			p_vFixedSrc.resize(int(1.2*p_nFixedSrcBankCount) + 1);
		}
		///////////////////////////// sample prompt/delayed fission neutrons //////////////////////////////////
		if(ORNG.Rand() < beta)
		{
			int NPCR = cAceData.GetNPCR(nuc);
			int lc = cAceData.GetLocOfBDD(nuc);
			double ksi = ORNG.Rand();
			double prob_sum = 0;
			int j = 1;
			for(j = 1; j <= NPCR; ++j)
			{
				int NR = int(cAceData.p_vNuclides[nuc].XSS[lc + 1]);
				int NE = int(cAceData.p_vNuclides[nuc].XSS[lc + 2 + 2*NR]);

				double yield = cAceData.GetErgFuncValue(nuc, lc + 1, erg);

				prob_sum = prob_sum + yield;

				if(ksi < prob_sum)
				{ 
					break;
				}
				lc = lc + 2 + 2*NR + 2*NE + 1;
			}

			j = std::min(j,NPCR);
			int LawType = 0;
			int	LDAT = 0;
			cAceData.GetLawType(nuc,-j,erg,LawType,LDAT);

			double exit_erg;
			double mu = 2*ORNG.Rand() - 1;
			cAceData.ReactByLaws(nuc,-1,LawType, LDAT,erg, exit_erg,mu);

			double phi = 2 * 3.1415927 * ORNG.Rand();
			cParticleState.p_dExitDir[0] = mu;
			cParticleState.p_dExitDir[1] = sqrt(1 - mu*mu) * cos(phi);
			cParticleState.p_dExitDir[2] = sqrt(1 - mu*mu) * sin(phi);

			cParticleState.p_dExitErg = exit_erg;
		}


		///////////////////////////////// Get fission neutron state //////////////////////////////////////////
		else
		{
			if(cAceData.p_bIsMultiGroup)
			{
				cAceData.GetMgFisErgDir(nuc,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
			}
			else
			{
				cNeutronTransport.GetCeExitState(cAceData,cParticleState,nuc,FisMT,erg,cParticleState.p_dDir,false,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
			}
		}


		for (int j = 0;j < 3; j++)
		{
			p_vFixedSrcBank[p_nFixedSrcBankCount].p_dPos[j] =  cParticleState.p_dPos[j];
			p_vFixedSrcBank[p_nFixedSrcBankCount].p_dDir[j] = cParticleState.p_dExitDir[j];
		}

		p_vFixedSrcBank[p_nFixedSrcBankCount].p_dErg = cParticleState.p_dExitErg;
		p_vFixedSrcBank[p_nFixedSrcBankCount].p_dWgt = cParticleState.p_dWgt;

		p_nFixedSrcBankCount = p_nFixedSrcBankCount + 1;
	}

	return;
}