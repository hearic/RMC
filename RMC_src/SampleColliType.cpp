# include "NeutronTransport.h"
//# include "FixedSource.h"

int CDNeutronTransport::SampleColliType_Cri(CDAceData &cAceData,CDParticleState &cParticleState)
{
	////////////////////////////  multi-group case  //////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		return 0;
	}

	/////////////////////////// continuous energy case ///////////////////////////
	//////////////////////  sab collision case  //////////////////////////
	if(cParticleState.p_bIsSabCol)  //// treat sab collision case 
	{
		return 0;
	}
	
	//////////////////////// elastic scattering  //////////////////////////
	for(;;)
	{
		if (ORNG.Rand()*(cParticleState.p_OColNucCs.p_dEl + cParticleState.p_OColNucCs.p_dInel) - cParticleState.p_OColNucCs.p_dEl <= 0)
		{
			int nMt = 2;
			return nMt;
		}  

		/////////////////////// inelastic scattering  /////////////////////////
		int nColliNuc = cParticleState.p_nNUC ;
		double dSum = 0;
		double dKsi = ORNG.Rand()*cParticleState.p_OColNucCs.p_dInel;
		int dLoc = cAceData.GetLocOfMTR(nColliNuc) - 1;
		int nMtNum = cAceData.GetNonElMtNumWithNeu(nColliNuc);
		for (int i = 1;i <= nMtNum;i++)  
		{
			int nMt = int(cAceData.p_vNuclides[nColliNuc].XSS[dLoc + i]);
			if(nMt == 18 || nMt == 19  || nMt == 20  || nMt == 21  || nMt == 38)
			{
				continue;
			}
			double dCs = cAceData.GetNucMtCs(nColliNuc, nMt, cParticleState.p_nInterpN,cParticleState.p_dInterpK);
			if(dCs > 0)
			{
				dSum = dSum + dCs;
				if(dKsi <= dSum)
				{
					return nMt;
				}
			}
		}

		if(cParticleState.p_OColNucCs.p_dInel == dSum)
		{
			break;
		}

		cParticleState.p_OColNucCs.p_dInel = dSum;  //// try re-sampling
	    printf("re-sampling inel collision.\n");
	}


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect sampling of collision type. nuc = %s.\n",cAceData.p_vNuclides[cParticleState.p_nNUC].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
	cParticleState.p_bIsKilled = true;

	return 0;
}


int CDNeutronTransport::SampleColliType_Fix(CDAceData &cAceData,CDParticleState &cParticleState)
{
	////////////////////////////  multi-group case  //////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		double Scat =  cParticleState.p_OColNucCs.p_dTot - cParticleState.p_OColNucCs.p_dFis - cParticleState.p_OColNucCs.p_dAbs;
		if (ORNG.Rand()*(Scat + cParticleState.p_OColNucCs.p_dFis) - Scat <= 0)
		{
			int mt = 2;
			return mt;
		}
		else
		{
			int mt = 19;
			return mt;
		}
		//return 0;
	}

	/////////////////////////// continuous energy case ///////////////////////////
	//////////////////////  sab collision case  //////////////////////////
	if(cParticleState.p_bIsSabCol)  //// treat sab collision case 
	{
		return 0;
	}

	
	///////////////////// fission is treated as inelastic scattering ////////////////
	cParticleState.p_OColNucCs.p_dInel += cParticleState.p_OColNucCs.p_dFis;

	for(;;)
	{
		//////////////////////// elastic scattering  //////////////////////////
		if (ORNG.Rand()*(cParticleState.p_OColNucCs.p_dEl + cParticleState.p_OColNucCs.p_dInel) - cParticleState.p_OColNucCs.p_dEl <= 0)
		{
			int mt = 2;
			return mt;
		}  

		/////////////////////// inelastic scattering  /////////////////////////
		int ColliNuc = cParticleState.p_nNUC ;
		double sum = 0;
		double ksi = ORNG.Rand()*cParticleState.p_OColNucCs.p_dInel;
		int loc = cAceData.GetLocOfMTR(ColliNuc) - 1;
		int MtNum = cAceData.GetNonElMtNumWithNeu(ColliNuc);

		///////////////////// URR treatment ///////////////////////
		double ff=1;
		if(cAceData.p_vNuclides[cParticleState.p_nNUC].p_nProbTableFlag == 1)
		{
			double cf = cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0]+cParticleState.p_dInterpK0*(cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0+1]-cAceData.p_vNuclides[cParticleState.p_nNUC].FisXSS[cParticleState.p_nInterpN0]);
			if(cf != 0)
			{
				ff = cParticleState.p_OColNucCs.p_dFis/cf;
			}
		}

		for (int i = 1;i <= MtNum;i++)  
		{
			int mt = int(cAceData.p_vNuclides[ColliNuc].XSS[loc + i]);

			double cs;
			if(mt==18 || mt==19 || mt==20 || mt==21 || mt==38)
			{
				cs = cAceData.GetNucMtCs(ColliNuc, mt, cParticleState.p_nInterpN0,cParticleState.p_dInterpK0);
				cs = cs*ff;
			}
			else
			{
				cs = cAceData.GetNucMtCs(ColliNuc, mt, cParticleState.p_nInterpN,cParticleState.p_dInterpK);
			}

			if(cs > 0)
			{
				sum = sum + cs;
				if(ksi <= sum)
				{
					return mt;
				}
			}
		}

		if(cParticleState.p_OColNucCs.p_dInel == sum)
		{
			break;
		}

		cParticleState.p_OColNucCs.p_dInel = sum;  //// try re-sampling
		printf("re-sampling inel collision.\n");
	}


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect sampling of collision type. nuc = %s.\n",cAceData.p_vNuclides[cParticleState.p_nNUC].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
	cParticleState.p_bIsKilled = true;

	return 0;
}