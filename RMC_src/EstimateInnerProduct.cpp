# include "Criticality.h"


void CDCriticality::EstimateNeuPopuTrk(CDAceData &cAceData, double dWgt, double dErg, double track_len)
{
	double dNeuMass = 939.58;
	double dSlight = 299.7925;

	if(cAceData.p_bIsMultiGroup)
	{
		dErg = cAceData.GetMgCentErg(int(dErg+0.5));
	}

	double dVel = dSlight*sqrt(dErg*(dErg+2*dNeuMass))/(dErg+dNeuMass);
	p_cNeuPopu.p_dScore += dWgt*track_len/dVel;
}


void CDCriticality::EstimatePower(CDAceData &cAceData, CDParticleState &cParticleState, CDMaterial &cMaterial, double dWgt, double dErg, double track_len)
{
	int nMat=cParticleState.p_nMAT;
	double dSumFisQ = 0;

	static map<int, double> NucFisQ;
	static bool bFirstRun = true;

	if(bFirstRun)
	{
		bFirstRun = false;
		NucFisQ[90232] = 171.91 ;  
		NucFisQ[91233] = 175.57 ;
		NucFisQ[92233] = 180.84 ; 
		NucFisQ[92234] = 179.45 ;
		NucFisQ[92235] = 180.88 ; 
		NucFisQ[92236] = 179.50 ;
		NucFisQ[92237] = 180.40 ; 
		NucFisQ[92238] = 181.31 ;
		NucFisQ[92239] = 180.40 ; 
		NucFisQ[92240] = 180.40 ;
		NucFisQ[93237] = 183.67 ; 
		NucFisQ[94238] = 186.65 ;
		NucFisQ[94239] = 189.44 ; 
		NucFisQ[94240] = 186.36 ;
		NucFisQ[94241] = 188.99 ; 
		NucFisQ[94242] = 185.98 ;
		NucFisQ[94243] = 187.48 ; 
		NucFisQ[95241] = 190.83 ;
		NucFisQ[95242] = 190.54 ; 
		NucFisQ[95243] = 190.25 ;
		NucFisQ[96242] = 190.49 ; 
		NucFisQ[96244] = 190.49 ;
	}

	if(cParticleState.p_dMacroMuFissCs <= 0)
	{
		dSumFisQ = 0 ;
	}
	else
	{
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nMat); ++i )
		{
			int nuc = cMaterial.GetMatNucIndex(nMat,i);

			if(cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis <=0 )
			{
				continue;
			}

			double FisQ;
			int nFindKey = cAceData.GetNuclideZA(nuc);
			if(NucFisQ.find(nFindKey) == NucFisQ.end()) 
			{  
				FisQ = 180 ;	
			}
			else
			{
				FisQ = NucFisQ[nFindKey];
			}

			dSumFisQ = dSumFisQ + FisQ*cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis*cMaterial.GetMatNucAtomDen(nMat,i);
		}

	}

	p_dPower += dWgt*track_len*dSumFisQ;
}