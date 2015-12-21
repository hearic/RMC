# include "Criticality.h"
# include "Convergence.h"
# include "Burnup.h"

void CDBurnup::RestBurnup(CDNeutronTransport &cNeutronTransport, CDCriticality &cCriticality,CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally,CDAceData &cAceData, CDConvergence &cConvergence)
{
	// BurnStep has begun
	p_bIsBurnStepBegun = true;

	// new burn step
	PrintStepMssg();


	////////////////////////  reset ACE data /////////////////////////////////////
    cAceData.ClearData();


	////////////////////////  reset cMaterial data ////////////////////////////////
	for(int i = 1; i <= cMaterial.GetTotMatNum(); ++i)   // MatNum + 1 ?
	{
		cMaterial.p_vMatSet[i].p_vSabnucOfNuc.clear();
		cMaterial.p_vMatSet[i].p_vMatSabNucEsa.clear();
	}


	////////////////////////  update varying cMaterial /////////////////////////////
	for(int i = 1;i <= p_nBurnVaryingFlag;i++)
	{
		int nMatTemp = cMaterial.p_OMatSetIndex.GetIndex(p_vBurnVaryingNuc[i][0]);
		int nNucId1 = p_vBurnVaryingNuc[i][1];
		if(nMatTemp <= 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown material %d defined in varying burnup card!\n",
				p_vBurnVaryingNuc[i][0]);}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
		if(nNucId1 == 0)
		{
			cMaterial.p_vMatSet[nMatTemp].p_dMatUserDen= p_vBurnVaryingNucDen[i][p_nBurnStep + 1];
			continue;
		}
		int j;
		for(j = 1; j <= cMaterial.GetMatTotNucNum(nMatTemp);j++)
		{
			int nNucId2 = atoi(cMaterial.GetMatNucID(nMatTemp,j).p_chIdStr);
			if(nNucId1 == nNucId2)
			{
				cMaterial.p_vMatSet[nMatTemp].p_vMatNucAtomDen[j] = p_vBurnVaryingNucDen[i][p_nBurnStep + 1];
				break;
			}
		}
		if(j > cMaterial.GetMatTotNucNum(nMatTemp))
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown nuclide %d defined in varying burnup card!\n",
				p_vBurnVaryingNuc[i][1]);}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	////////////////////////  Initiate cMaterial and ACE data  /////////////////////////////
	cMaterial.InitiateMatAce(cGeometry,cTally, cAceData, cAceData.p_vNuclides, cGeometry.p_vCell, *this);


	////////////////////////////  Initiate Criticality data  ////////////////////////////////
	for(int i = 0; i < 3;++i)
	{
		cCriticality.p_dKeffSum[i] = 0;
		for(int j = 0;j < 3;++j)
		{
			cCriticality.p_dKeffCrosSum[i][j] = 0;
		}
	}
	cCriticality.p_llTotCollisionCount=0;
	Output.p_nWarningCount = 0;
	cNeutronTransport.p_nMissParticleCount=0;
    //cCriticality.TrsptStartCpuClock = clock();
	//ParticleState.CELL=1;

	////////////////////////////////Initiate convergence data////////////////////////////////////
	cConvergence.p_bUseFscDiag = false;
	cConvergence.p_bUseFscAcce = false;
	cConvergence.p_bUseDiagFm = false;
	cConvergence.p_bUseAcceFm = false;
	cConvergence.p_bUseDiagNpm = false;
	cConvergence.p_bUseNpmOnFly = false;
	cConvergence.p_nFscAcceMethod = -1;

	////////////////////////////  Initiate tally data  ////////////////////////////////
	if(cTally.p_bUseUnionCsTally)
	{
		cAceData.p_vUnionEnergyGrid.clear();
	} 

	cTally.InitiateTally(cGeometry, cAceData, cMaterial);



	//printf("%d\n",cTally.CsTallyData.Sum1.size());
	return;
	
}