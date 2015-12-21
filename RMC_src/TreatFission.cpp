# include "Criticality.h"
# include "Convergence.h"
# include "FixedSource.h"

static int FissMt[5]= {18,19,20,21,38};

void CDCriticality::TreatFission(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence)
{
	//////////////////////// collision nuclide is not fissionable /////////////////////
	if(cParticleState.p_OColNucCs.p_dFis <=0 )
	{
		return;
	}

	if(cAceData.p_bIsMultiGroup)
	{
		EstimateKeffAbs(cParticleState.p_dWgt,cParticleState.p_OColNucCs.p_dNu,cParticleState.p_OColNucCs.p_dFis,cParticleState.p_OColNucCs.p_dTot);
		BankFisSourceMg(cNeutronTransport, cAceData,cParticleState,cConvergence); 
		return;
	}

	///////////////////////// calculate fission cross-section /////////////////////////
	double dFisSubCs[5];
	for(int j = 0;j <= 4;j++)
	{
		dFisSubCs[j] = 0;
	}
	if(cAceData.p_vNuclides[cParticleState.p_nNUC].LSIG[18] > 0)//总裂变截面MT=18存在
	{
		dFisSubCs[0] = cAceData.GetNucMtCs(cParticleState.p_nNUC,18,cParticleState.p_nInterpN0,cParticleState.p_dInterpK0);
	} 
	else
	{
		for(int j = 1;j <= 4;j++)
		{
			dFisSubCs[j] = cAceData.GetNucMtCs(cParticleState.p_nNUC,FissMt[j],cParticleState.p_nInterpN0,cParticleState.p_dInterpK0);
		}	
	}

	///////////////////// URR treatment ///////////////////////
	if(cAceData.p_vNuclides[cParticleState.p_nNUC].p_nProbTableFlag == 1)
	{
		double dTemp = cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0]+cParticleState.p_dInterpK0*(cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0+1]-cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0]);
		if(dTemp != 0)
		{
			double ff = cParticleState.p_OColNucCs.p_dFis/dTemp;
			for(int j = 0;j <= 4;j++)
			{
				dFisSubCs[j] = dFisSubCs[j]*ff;
			}
		}

		cParticleState.p_OColNucCs.p_dFis = dFisSubCs[0] + dFisSubCs[1] + dFisSubCs[2] + dFisSubCs[3] + dFisSubCs[4];
	}


	////////////////////////// treat keff absorption estimator //////////////////////////
	EstimateKeffAbs(cParticleState.p_dWgt,cParticleState.p_OColNucCs.p_dNu,cParticleState.p_OColNucCs.p_dFis,cParticleState.p_OColNucCs.p_dTot);

	//////////////////////////////Bank fission sites ////////////////////////////////////
	BankFisSource(cNeutronTransport, cAceData,cParticleState,cConvergence,dFisSubCs); 

	return;
	
}


void CDFixedSource::TreatFission(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState)
{

	if(cAceData.p_bIsMultiGroup)
	{
		//EstimateKeffAbs(cParticleState.p_dWgt,cParticleState.p_OColNucCs.p_dNu,cParticleState.p_OColNucCs.p_dFis,cParticleState.p_OColNucCs.p_dTot);
		//BankFisSourceMg(cAceData,cParticleState); 
		GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,-1);
		return;
	}

	

	////////////////////////// treat keff absorption estimator //////////////////////////
	//EstimateKeffAs(cParticleState.p_dWgt,cParticleState.p_OColNucCs.p_dNu,cParticleState.p_OColNucCs.p_dFis,cParticleState.p_OColNucCs.p_dTot);

	//////////////////////////////Bank fission sites ////////////////////////////////////
	//BankFisSource(cAceData,cParticleState,FisSubCs); 
	GetFissionNeuState(cNeutronTransport, cAceData,cParticleState,cParticleState.p_nColType);

	return;

}