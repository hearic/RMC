# include "Tally.h"
# include "Criticality.h"

double CDTally::CalcMatFisQ(int nMat, CDMaterial &cMaterial, CDParticleState &cParticleState, CDAceData &cAceData)
{
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
		return 0 ;
	}

	double dSumFisQ = 0;
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

	return dSumFisQ;
}


/*
double TallyClass::CalcMatFisQ2(int mat, ParticleStateClass &CParticleState)
=======
double TallyClass::CalcMatFisQ2(int mat,MaterialClass & CMaterial,AceDataClass & CAceData,ParticleStateClass &CParticleState)
>>>>>>> 3d1d08b781333aa7d7f318c42f780ebb1ccd78dc
{

	if(CParticleState.MacroMuFissCs <= 0)
	{
		return 0 ;
	}

	static int FisMt[5]= {18,19,20,21,38};
	double SumFisQ = 0;
	for(int i = 0; i < CMaterial.GetMatTotNucNum(mat); ++i )
	{
		int nuc = CMaterial.GetMatNucIndex(mat,i);

		if(CAceData.Nuclides[nuc].NucCs.Fis <=0 )
		{
			continue;
		}

		double NucFisQ = 0;
		if(CAceData.Nuclides[nuc].MTRIndex[18] > 0)//总裂变截面MT=18存在
		{
			double FisCs = CAceData.GetNucMtCs(nuc,18,CAceData.Nuclides[nuc].NucInterpPos,CAceData.Nuclides[nuc].NucInterpFrac);
			double Q = CAceData.GetNucFisQ(nuc,18);
			NucFisQ = FisCs*Q;
		} 
		else
		{
			for(int j = 1; j <= 4; ++j)
			{
				double FisCs = CAceData.GetNucMtCs(nuc,FisMt[j],CAceData.Nuclides[nuc].NucInterpPos,CAceData.Nuclides[nuc].NucInterpFrac);
				double Q = CAceData.GetNucFisQ(nuc,FisMt[j]);
				NucFisQ = NucFisQ + FisCs*Q;
			}	
		}

		SumFisQ = SumFisQ + NucFisQ*Material.GetMatNucAtomDen(mat,i);
	}

	return SumFisQ;
}
*/
