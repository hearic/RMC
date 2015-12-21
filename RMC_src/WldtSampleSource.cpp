# include "Convergence.h"

void CDConvergence::WldtSampleSource(CDNeutronTransport &cNeutronTransport,CDCriticality& cCriticality, CDParticleState& cParticlestate, CDGeometry& cGeometry)
{
	///////////////  flag of kill particle //////////////////////
	cParticlestate.p_bIsKilled = false;

	//////////////////// Get position, direction, energy and weight ////////////////////////////
	for(int j=0;j<3;++j)
	{
		cParticlestate.p_dPos[j] = p_vWldtSourceBank[p_nWldtSampleCount].p_dPos[j];   //Î»ÖÃ
		cParticlestate.p_dDir[j] = p_vWldtSourceBank[p_nWldtSampleCount].p_dDir[j];
	}	

	cParticlestate.p_dErg = p_vWldtSourceBank[p_nWldtSampleCount].p_dErg;	    
    cParticlestate.p_dWgt = 1.0; //Wielandt_weight_bank[WldtSampleCount];

	/////////////////////// Locate fission source ///////////////////////
    cParticlestate.ClearLevel();
	cParticlestate.p_nCELL = cGeometry.LocateParticle(cParticlestate, 0,cParticlestate.p_dPos,cParticlestate.p_dDir);
	if(cParticlestate.p_nCELL == -1) // wrong initial source
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed locate fission source.\n");},CDOutput::_WARNING);

		++ cNeutronTransport.p_nMissParticleCount;
		cParticlestate.p_bIsKilled = true;
		return;
	} 

	if(cGeometry.p_vCell[cParticlestate.p_nCELL].p_nImp == 0) // 0 importance cell
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial fission source is in 0 importance cell.\n");},CDOutput::_WARNING);
		cParticlestate.p_dWgt = 0 ;
	} 
	cParticlestate.p_nMAT = cGeometry.GetCellMat(cParticlestate.p_vLocCellsU);

	if(p_nFscAcceMethod == 2)
	{
		p_nSuphisCycleCount = p_vSuphisSourceCycleBank[p_nWldtSampleCount];
	} 

	p_nWldtSampleCount = p_nWldtSampleCount+1;

	return;
}