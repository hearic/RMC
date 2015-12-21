//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"

void CDNeutronTransport::SampleColliNuc(CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState)
{
	double dSampCut = cParticleState.p_dMacroTotCs*ORNG.Rand();


	int nMat = cParticleState.p_nMAT ;
	int i;
	double dSigtSum2 = 0;
	for(i = 0 ; i < cMaterial.GetMatTotNucNum(nMat);i++)
	{
		int nuc = cMaterial.GetMatNucIndex(nMat, i);

		dSigtSum2 = dSigtSum2 + cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot* cMaterial.GetMatNucAtomDen(nMat, i);
		if (dSampCut <= dSigtSum2)
		{
			cParticleState.p_nNUC = nuc;
			break;
		}
	}

	////////////////////////// wrong case //////////////////////////
	if(i >= cMaterial.GetMatTotNucNum(nMat))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect sampling of collision nuc.%.3f/%.3f\n",dSigtSum2,cParticleState.p_dMacroTotCs);},CDOutput::_WARNING);
		cParticleState.p_bIsKilled = true;
	}

	////////////////////////// ordinary case ////////////////////////
	cParticleState.p_nSabNUC = cMaterial.GetMatSabnucOfNuc(nMat, i);
	if(cParticleState.p_nSabNUC > 0 && cParticleState.p_dErg < cMaterial.GetMatNucSabesa(nMat, i))
	{
		cParticleState.p_bIsSabCol = true;
	}  
	else
	{
		cParticleState.p_bIsSabCol = false;
	}


	return;
}


void CDNeutronTransport::SampleColliNuc(CDCriticality &cCriticality, CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState)
{
	//absorbed by the virtual time absorbed nuclide
	cParticleState.p_dWgt = cParticleState.p_dWgt*
		cParticleState.p_dMacroTotCs/(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs);


	double dSampCut = cParticleState.p_dMacroTotCs*ORNG.Rand();


	int nMat = cParticleState.p_nMAT ;
	int i;
	double dSigtSum2 = 0;
	for(i = 0 ; i < cMaterial.GetMatTotNucNum(nMat);i++)
	{
		int nuc = cMaterial.GetMatNucIndex(nMat, i);

		dSigtSum2 = dSigtSum2 + cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot* cMaterial.GetMatNucAtomDen(nMat, i);
		if (dSampCut <= dSigtSum2)
		{
			cParticleState.p_nNUC = nuc;
			break;
		}
	}

	////////////////////////// wrong case //////////////////////////
	if(i >= cMaterial.GetMatTotNucNum(nMat))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect sampling of collision nuc.%.3f/%.3f\n",dSigtSum2,cParticleState.p_dMacroTotCs);},CDOutput::_WARNING);
		cParticleState.p_bIsKilled = true;
	}

	////////////////////////// ordinary case ////////////////////////
	cParticleState.p_nSabNUC = cMaterial.GetMatSabnucOfNuc(nMat, i);
	if(cParticleState.p_nSabNUC > 0 && cParticleState.p_dErg < cMaterial.GetMatNucSabesa(nMat, i))
	{
		cParticleState.p_bIsSabCol = true;
	}  
	else
	{
		cParticleState.p_bIsSabCol = false;
	}


	return;
}