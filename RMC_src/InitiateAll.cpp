# include "Criticality.h"
# include "Tally.h"
# include "Material.h"
# include "Convergence.h"
# include "Burnup.h"
# include "FixedSource.h"


void CDCriticality::InitiateAll(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDConvergence &cConvergence,CDTally &cTally, 
	CDAceData& cAceData, CDParticleState &cParticleState,CDBurnup& cBurnup)
{

#ifdef _IS_PARALLEL_
	if((p_bIsQuasiStaticS || p_bIsQuasiStaticD)) OParallel.p_bUseFastBank = false;
#endif

	cMaterial.InitiateMatAce(cGeometry,cTally, cAceData, cAceData.p_vNuclides, cGeometry.p_vCell, cBurnup);   
	
	//Geometry.BuildNeighborCells();

	InitiateTrspt(cNeutronTransport);

	//////// Initiate Fission Source Convergence Block /////

	cConvergence.InitiateFsc(*this);

	cTally.InitiateTally(cGeometry, cAceData, cMaterial);

	
	////////// initial source /////////
	InitiateFisSource(cNeutronTransport,cGeometry, cAceData, cParticleState);
	
	return;
}


void CDFixedSource::InitiateAll(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial, CDTally &cTally, 
	CDAceData& cAceData, CDParticleState &cParticleState,CDBurnup& cBurnup)
{


	cMaterial.InitiateMatAce(cGeometry,cTally, cAceData, cAceData.p_vNuclides, cGeometry.p_vCell, cBurnup);   

	//Geometry.BuildNeighborCells();

	InitiateTrspt(cNeutronTransport);

	cTally.InitiateTally(cGeometry, cAceData, cMaterial);
	cTally.p_bIsPerHstry = true;
	cTally.p_bIsPerCyc = false;

	InitiateExternalSource(cNeutronTransport,cGeometry, cAceData, cParticleState);

	return;
}