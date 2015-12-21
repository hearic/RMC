//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"

void CDNeutronTransport::GeometryTracking(CDFixedSource &cFixedSource, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally)
{
	const bool bIS_ERG_CHANGED = true;
	double dFFL; ////////// free flight distance
	double dDTB; ////////// distance to boundary

	////////////////////////////  sample free flight distance  ////////////////////	
	dFFL = SampleFreeFlyDist(cGeometry,cMaterial,cAceData,cParticleState,bIS_ERG_CHANGED);

	/////////////////////////// Calculate distance to boundary ////////////////////
	cParticleState.p_nBoundSURF = 0;  //// particle is NOT on surface at the beginning  
	dDTB = cGeometry.CalcDistToBound(cParticleState);
	if(dDTB < 0)          //// error tolerance
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to calculate distance to boundary in cell %s. %s\n",
			Output.PrintCellVec(cParticleState.p_vLocCellsU),Output.PrintParticleState(cParticleState));},CDOutput::_WARNING);
		cParticleState.p_bIsKilled = true;
		p_nMissParticleCount++;
		return;
	}

	//////////////////////////////  ray tracking /////////////////////////////////
	int nMaxIter = 100000;
	int nIterCount = 0;
	while (dFFL >= dDTB)
	{
		////////////////////// error tolerance /////////////////////////
		if( (nIterCount++) >= nMaxIter)
		{
			cParticleState.p_bIsKilled = true;
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"too many times of surface-crossing.\n");},CDOutput::_WARNING);
			return;
		}

		////////////// keff & tally track length estimator //////////////
		//EstimateKeffTrk(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,DTB);
		cTally.TallyByTL(dDTB, cGeometry, cAceData, cFixedSource, cParticleState, cMaterial);

		/////////////////// Move particle to surface ////////////////////
		cParticleState.FlyByLength(dDTB); 

		///////// Find next cell (escaping/reflecting/crossing) /////////
		cGeometry.FindNextCell(cParticleState);
		if(cParticleState.p_nCELL == -1)          // wrong cell
		{
			++ p_nMissParticleCount;
		}

		if(cParticleState.p_bIsKilled)       //// found wrong cell
		{
			return;
		}

		////////////////// Re-calculate distance to boundary ////////////
		dDTB = cGeometry.CalcDistToBound(cParticleState);
		if(dDTB < 0)
		{
			cParticleState.ClearLevel();
			cParticleState.p_nCELL  = cGeometry.LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
			if(cParticleState.p_nCELL == -1)          //// error tolerance
			{
				cParticleState.p_bIsKilled = true;
				p_nMissParticleCount++;
				return;
			}

			cParticleState.p_nBoundSURF = 0;  
			dDTB = cGeometry.CalcDistToBound(cParticleState);
			if(dDTB < 0)          //// error tolerance
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to calculate distance to boundary in cell %s. %s\n",
					Output.PrintCellVec(cParticleState.p_vLocCellsU),Output.PrintParticleState(cParticleState));},CDOutput::_WARNING);
				cParticleState.p_bIsKilled = true;
				p_nMissParticleCount++;
				return;
			}
		}
		/////////////// Re-sample free flight distance //////////////////
		dFFL = SampleFreeFlyDist(cGeometry,cMaterial,cAceData,cParticleState,!bIS_ERG_CHANGED);
	}
	////////////////////////////  keff & tally track length estimator ////////////////////////
	//EstimateKeffTrk(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,FFL);
	cTally.TallyByTL(dFFL, cGeometry, cAceData, cFixedSource, cParticleState, cMaterial);

	//////////////////////////////////// keff collision estimator /////////////////////////////
	//EstimateKeffCol(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,cParticleState.p_dMacroTotCs);	

	////////////////////////////  Move particle to collision point ///////////////////////////
	cParticleState.FlyByLength(dFFL); 

	return;
}


void CDNeutronTransport::GeometryTracking(CDCriticality &cCriticality, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally)
{
	const bool bIS_ERG_CHANGED = true;
	double dFFL; ////////// free flight distance
	double dDTB; ////////// distance to boundary

	////////////////////////////  sample free flight distance  ////////////////////
	dFFL = SampleFreeFlyDist(cCriticality, cGeometry,cMaterial,cAceData,cParticleState,bIS_ERG_CHANGED);

	/////////////////////////// Calculate distance to boundary ////////////////////
	cParticleState.p_nBoundSURF = 0;  //// particle is NOT on surface at the beginning  
	dDTB = cGeometry.CalcDistToBound(cParticleState);
	if(dDTB < 0)          //// error tolerance
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to calculate distance to boundary in cell %s. %s\n",
			Output.PrintCellVec(cParticleState.p_vLocCellsU),Output.PrintParticleState(cParticleState));},CDOutput::_WARNING);		
		cParticleState.p_bIsKilled = true;
		p_nMissParticleCount++;
		return;
	}


	//////////////////////////////  ray tracking /////////////////////////////////
	int nMaxIter = 100000;
	int nIterCount = 0;
	while (dFFL >= dDTB)
	{
		////////////////////// error tolerance /////////////////////////
		if( (nIterCount++) >= nMaxIter)
		{
			cParticleState.p_bIsKilled = true;
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"too many times of surface-crossing.\n");},CDOutput::_WARNING);;
			return;
		}

		////////////// keff & tally track length estimator //////////////
		cCriticality.EstimateKeffTrk(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,dDTB);
		cTally.TallyByTL(dDTB, cGeometry, cAceData, cCriticality, cParticleState, cMaterial);

		///////////// inner product /////////////////////////
		if(cCriticality.p_bIsQuasiStaticS || cCriticality.p_bIsQuasiStaticD)
		{
			cCriticality.EstimateNeuPopuTrk(cAceData, cParticleState.p_dWgt, cParticleState.p_dErg, dDTB);    //neutron population estimation
			if(cCriticality.p_nCurrentCYCLE>cCriticality.p_nInactCycNum)
			{
				cCriticality.EstimatePower(cAceData, cParticleState, cMaterial,cParticleState.p_dWgt, cParticleState.p_dErg, dDTB);
			}
		}
		

		/////////////////// Move particle to surface ////////////////////
		cParticleState.FlyByLength(dDTB);

		///////// Find next cell (escaping/reflecting/crossing) /////////
		cGeometry.FindNextCell(cParticleState);
		if(cParticleState.p_nCELL == -1)          // wrong cell
		{
			++ p_nMissParticleCount;
		}

		if(cParticleState.p_bIsKilled)       //// found wrong cell
		{
			return;
		}

		////////////////// Re-calculate distance to boundary ////////////
		dDTB = cGeometry.CalcDistToBound(cParticleState);
		if(dDTB < 0)
		{
			cParticleState.ClearLevel();
			cParticleState.p_nCELL  = cGeometry.LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
			if(cParticleState.p_nCELL == -1)          //// error tolerance
			{
				cParticleState.p_bIsKilled = true;
				p_nMissParticleCount++;
				return;
			}
			cParticleState.p_nBoundSURF = 0;  
			dDTB = cGeometry.CalcDistToBound(cParticleState);
			if(dDTB < 0)          //// error tolerance
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to calculate distance to boundary in cell %s. %s\n",
					Output.PrintCellVec(cParticleState.p_vLocCellsU),Output.PrintParticleState(cParticleState));},CDOutput::_WARNING);
				cParticleState.p_bIsKilled = true;
				p_nMissParticleCount++;
				return;
			}
		}

		/////////////// Re-sample free flight distance //////////////////
		dFFL = SampleFreeFlyDist(cCriticality, cGeometry,cMaterial,cAceData,cParticleState,!bIS_ERG_CHANGED);
	}
	////////////////////////////  keff & tally track length estimator ////////////////////////
	cCriticality.EstimateKeffTrk(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,dFFL);
	cTally.TallyByTL(dFFL, cGeometry, cAceData, cCriticality, cParticleState, cMaterial);


	///////////// inner product /////////////////////////
	if(cCriticality.p_bIsQuasiStaticS || cCriticality.p_bIsQuasiStaticD)
	{
		cCriticality.EstimateNeuPopuTrk(cAceData, cParticleState.p_dWgt, cParticleState.p_dErg, dFFL);    //neutron population estimation

		if(cCriticality.p_nCurrentCYCLE>cCriticality.p_nInactCycNum)
		{
			cCriticality.EstimatePower(cAceData, cParticleState, cMaterial,cParticleState.p_dWgt, cParticleState.p_dErg, dFFL);
		}
	}
	


	//////////////////////////////////// keff collision estimator /////////////////////////////
	cCriticality.EstimateKeffCol(cParticleState.p_dWgt,cParticleState.p_dMacroMuFissCs,(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs));	

	////////////////////////////  Move particle to collision point ///////////////////////////
	cParticleState.FlyByLength(dFFL); 


	//////////////////////////////////// time absorbed neutron source bank ///////////////////////
	if((cCriticality.p_bIsQuasiStaticS || cCriticality.p_bIsQuasiStaticD) && cCriticality.p_nCurrentCYCLE>cCriticality.p_nInactCycNum)
	{
		cCriticality.BankTimeAbsNeuSource(cAceData,cParticleState);
	}

	return;
}