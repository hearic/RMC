#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::OutputBurnup(CDMaterial &cMaterial,CDTally &cTally,CDCriticality & cCriticality,vector<CDNuclide>& vNuclides)
{
	////////////output burnup results at the beginning of predictor step ////////////////
	if(p_nBurnStepStrategy < 0 )  
	{
		return;
	}


	////////////////////// output power and depletion distribution  ///////////////////////
	OutputCellPower(cTally);
	
	/////////////////////////// output total isotope densities  ///////////////////////////
	OutputTotNucDen();

	//////////////////////// output isotope density of indicated cells //////////////////////
	OutputCellNucDen(cMaterial,cTally,vNuclides);


	////////////////// update depletion distribution among cells ///////////////
	int nBurnStep = p_nBurnStep;
	if(nBurnStep < p_nTotBurnStep)
	{
		for(int i = 0;i < p_nBurnCellNum; ++i )
		{
			p_vCellCmltvBurnup[i] += ( p_vCellInitFuelMass[i] > 0 ) ? ( p_vCellBurnPower[i]*p_vStepBurnTime[nBurnStep + 1]/p_vCellInitFuelMass[i] ) : 0.0;
		}
	}

	///////////////////// updata keff of current burnup step //////////////////
	p_vStepFinalKeff[p_nBurnStep] = cCriticality.p_dKeffCovwAve[3];

	return;
}

