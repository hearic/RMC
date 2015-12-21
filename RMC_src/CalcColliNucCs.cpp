//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"


void CDNeutronTransport::CalcColliNucCs(CDAceData &cAceData,CDParticleState &cParticleState)
{
	int nColNuc = cParticleState.p_nNUC;

	//////////////////////////////// multigroup case  /////////////////////////////////////
	/////////////////// Note fission is not included in absorption  ///////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		cParticleState.p_OColNucCs.p_dTot = cAceData.p_vNuclides[nColNuc].p_ONucCs.p_dTot;
		cParticleState.p_OColNucCs.p_dFis = cAceData.p_vNuclides[nColNuc].p_ONucCs.p_dFis;
		cParticleState.p_OColNucCs.p_dNu = cAceData.p_vNuclides[nColNuc].p_ONucCs.p_dNu;
		cParticleState.p_OColNucCs.p_dAbs = cAceData.GetMgNucAbsCs(nColNuc,cParticleState.ErgGrp());
		return;
	}


	////////////////////////////////// S(a,b) case  ///////////////////////////////////////
	////////////////////// Nuclides[ColNuc].NucCs have been interpolated /////////////////////////
	if(cParticleState.p_bIsSabCol) 
	{
		cParticleState.p_OColNucCs = cAceData.p_vNuclides[nColNuc].p_ONucCs;
		return;
	}


	///////////////////////// ordinary continuous energy case  ///////////////////////////
	/////////////////// Note fission is not included in absorption  ///////////////////////
	cParticleState.p_nInterpN0 = cAceData.p_vNuclides[nColNuc].p_nNucInterpPos;
	cParticleState.p_dInterpK0 = cAceData.p_vNuclides[nColNuc].p_dNucInterpFrac;

	//////// free gas treatment:sample the velocity of target nucleus //////////
	if((cParticleState.p_dCellTmp > 1.0E-24) && (cParticleState.p_dErg < 400*cParticleState.p_dCellTmp || cAceData.p_vNuclides[nColNuc].p_dNucAtomWgt <= 1.5))  
	{
		cParticleState.p_bIsFreeGasCol = true;
		if(cAceData.p_vNuclides[nColNuc].p_nProbTableFlag == 1) 
		{
			cParticleState.p_OColNucCs = cAceData.p_vNuclides[nColNuc].p_ONucCs;
			return;
		}
		//printf("Free gas treatment.\n");
		TreatFreeGasModel(cParticleState,cParticleState.p_dCellTmp,cAceData.p_vNuclides[nColNuc].p_dNucAtomWgt, cParticleState.p_dErg, cParticleState.p_dErg_Rel);
		CDGlobeFun::GetIntpltPosFr(&cAceData.p_vNuclides[nColNuc].XSS[0],cParticleState.p_dErg_Rel,1,cAceData.GetErgGridNum(nColNuc),cParticleState.p_nInterpN,cParticleState.p_dInterpK);
	}
	else
	{
		cParticleState.p_bIsFreeGasCol = false;
		cParticleState.p_dErg_Rel = cParticleState.p_dErg;
		cParticleState.p_nInterpN = cParticleState.p_nInterpN0;	
		cParticleState.p_dInterpK = cParticleState.p_dInterpK0;
	}

	//////////////////////// interpolate SIG (a,el,inel) ////////////////////////
	//////////////////// SIG (t,f) have been interpolated ///////////////////////
	cAceData.GetNucAbsScattCs(nColNuc,cParticleState.p_dErg,cParticleState.p_nInterpN0,cParticleState.p_dInterpK0,cParticleState.p_nInterpN,cParticleState.p_dInterpK);

	cParticleState.p_OColNucCs = cAceData.p_vNuclides[nColNuc].p_ONucCs;
	return;
}