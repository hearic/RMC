# include "Criticality.h"
# include "Convergence.h"
# include "FixedSource.h"


void CDCriticality::SampleFisSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDParticleState &cParticleState,CDConvergence &cConvergence)
{
	///////////////  flag of kill particle //////////////////////

	cParticleState.p_bIsKilled = false;

	//////////////////// Get position, direction, energy and weight ////////////////////////////
	for(int j = 0; j < 3; ++j)
	{


		cParticleState.p_dPos[j] = p_vFissionSrc[p_nFisSrcCount].p_dPos[j];   // Position
		cParticleState.p_dDir[j] = p_vFissionSrc[p_nFisSrcCount].p_dDir[j];   //Direction
	}	


	cParticleState.p_dErg = p_vFissionSrc[p_nFisSrcCount].p_dErg;	    
	cParticleState.p_dWgt = cNeutronTransport.p_dStartWgt;
	++ p_nFisSrcCount;    /// fix bug 2012-12-01

	/////////////////////// Locate fission source ///////////////////////



	cParticleState.ClearLevel();
	cParticleState.p_nCELL = cGeometry.LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	if(cParticleState.p_nCELL == -1) // wrong initial source
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed locate fission source.\n");},CDOutput::_WARNING);

		++ cNeutronTransport.p_nMissParticleCount;

		cParticleState.p_bIsKilled = true;

		return;
	} 


	if(cGeometry.p_vCell[cParticleState.p_nCELL].p_nImp == 0) // 0 importance cell
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial source located in 0 importance cell.\n");},CDOutput::_WARNING);
		printf("x=%6.4E, y=%6.4E, z=%6.4E\n",cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2]);

		cParticleState.p_dWgt = 0;

		cParticleState.p_bIsKilled=true;
	} 


	cParticleState.p_nMAT = cGeometry.GetCellMat(cParticleState.p_vLocCellsU);


	///////////// tally fission source for Shannon entropy & fission matrix //////////////

	if(cConvergence.p_bUseFscDiag)
	{

		cConvergence.FscTallySrc(cParticleState.p_dPos,cParticleState.p_dWgt, *this);
	}


	return;
}



void CDFixedSource::SampleFixSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDParticleState &cParticleState)
{
	///////////////  flag of kill particle //////////////////////

	cParticleState.p_bIsKilled = false;

	//////////////////// Get position, direction, energy and weight ////////////////////////////
	for(int j = 0; j < 3; ++j)
	{


		cParticleState.p_dPos[j] = p_vFixedSrc[p_nFixedSrcCount].p_dPos[j];   // Position
		cParticleState.p_dDir[j] = p_vFixedSrc[p_nFixedSrcCount].p_dDir[j];   //Direction
	}	


	cParticleState.p_dErg = p_vFixedSrc[p_nFixedSrcCount].p_dErg;	    
	cParticleState.p_dWgt = cNeutronTransport.p_dStartWgt;
	++ p_nFixedSrcCount;    /// fix bug 2012-12-01

	/////////////////////// Locate fission source ///////////////////////



	cParticleState.ClearLevel();
	cParticleState.p_nCELL = cGeometry.LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	if(cParticleState.p_nCELL == -1) // wrong initial source
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed locate fission source.\n");},CDOutput::_WARNING);

		++ cNeutronTransport.p_nMissParticleCount;

		cParticleState.p_bIsKilled = true;

		return;
	} 


	if(cGeometry.p_vCell[cParticleState.p_nCELL].p_nImp == 0) // 0 importance cell
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial source located in 0 importance cell.\n");},CDOutput::_WARNING);

		cParticleState.p_dWgt = 0 ;
	} 


	cParticleState.p_nMAT = cGeometry.GetCellMat(cParticleState.p_vLocCellsU);

	return;
}


void CDFixedSource::SampleFisSource(CDNeutronTransport &cNeutronTransport,CDGeometry &cGeometry,CDParticleState &cParticleState)
{
	///////////////  flag of kill particle //////////////////////

	cParticleState.p_bIsKilled = false;

	//////////////////// Get position, direction, energy and weight ////////////////////////////
	for(int j = 0; j < 3; ++j)
	{


		cParticleState.p_dPos[j] = p_vFixedSrcBank[p_nFixedSrcBankCount-1].p_dPos[j];   // Position
		cParticleState.p_dDir[j] = p_vFixedSrcBank[p_nFixedSrcBankCount-1].p_dDir[j];   //Direction
	}	


	cParticleState.p_dErg = p_vFixedSrcBank[p_nFixedSrcBankCount-1].p_dErg;	    
	cParticleState.p_dWgt = p_vFixedSrcBank[p_nFixedSrcBankCount-1].p_dWgt;
	p_nFixedSrcBankCount--;    /// fix bug 2012-12-01

	/////////////////////// Locate fission source ///////////////////////



	cParticleState.ClearLevel();
	cParticleState.p_nCELL = cGeometry.LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	if(cParticleState.p_nCELL == -1) // wrong initial source
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed locate fission source.\n");},CDOutput::_WARNING);

		++ cNeutronTransport.p_nMissParticleCount;

		cParticleState.p_bIsKilled = true;

		return;
	} 


	if(cGeometry.p_vCell[cParticleState.p_nCELL].p_nImp == 0) // 0 importance cell
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial source located in 0 importance cell.\n");},CDOutput::_WARNING);

		cParticleState.p_dWgt = 0 ;
	} 


	cParticleState.p_nMAT = cGeometry.GetCellMat(cParticleState.p_vLocCellsU);

	return;
}


