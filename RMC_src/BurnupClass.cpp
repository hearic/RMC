#include "Burnup.h"
#include "Criticality.h"

CDBurnup::CDBurnup()
{

	p_bIsWithBurnup = false;
	p_bIsBurnStepBegun = false;
	p_nBurnStep = 0;            // Burnup beginning is Step 0
	p_nTotBurnStep = 0;
	p_dAbsorpFracCutoff = 0.9999;
	p_dNucDenFracCutoff = 0.999;
	p_nInnerStepNum = 10;       // Inner step time

	strcpy(p_chDepthLibForBurn,"DepthMainLib");// default depthlib

	p_nBurnSolver = 2 ;         // CRAM
	p_nBurnVaryingFlag = 0;
	p_nExcitedBranchFlag = 1;   // Excited branch

	p_nBurnStepStrategy = 0;    // Use predictor method

	p_nBurnTallyStartPos = 0;
	
	p_nXeEqFlag = 0;                           // 0: not consider Xe equilibrium  1£ºconsider Xe equilibrium
	p_dXeEqFactor = 0.1;

	p_dNAECFInput = 0;         // 0: tally neutron average energy causing fission

	////// important nuclide list from casmo-5
	int imp_nuc_id[119] =
	{340790, 360830, 360850, 380890, 380900, 390910, 400930, 400940, 400950, 400960, 
	410970, 420950, 420980, 420990, 421000, 430990, 441010, 441020, 441030, 441050, 
	441060, 451030, 451050, 471090, 471101, 471110, 501260, 511250, 511260, 511270, 
	521270, 521271, 521290, 521291, 531270, 531290, 531310, 531350, 541310, 541330, 
	541340, 541350, 541351, 541360, 551330, 551340, 551350, 551360, 551370, 561380, 
	561400, 571390, 571400, 581410, 581420, 581430, 581440, 591430, 591440, 601430, 
	601440, 601450, 601470, 601480, 601490, 611470, 611480, 611481, 611490, 621470, 
	621480, 621490, 621500, 621510, 621520, 631530, 631540, 631550, 631560, 641550, 
	641570, 922340, 922350, 922360, 922370, 922380, 922390, 922400, 932360, 932370, 
	932380, 932390, 932400, 942380, 942390, 942400, 942410, 942420, 942430, 942440, 
	952410, 952420, 952421, 952430, 952440, 962420, 962430, 962440, 962450, 962460, 
	962470, 962480, 962490, 972490, 972500, 982490, 982500, 982510, 982520};

	int imp_nuc_num = sizeof(imp_nuc_id)/sizeof(int);

	imp_nuc_num = 0;  //// comment this line to

	p_vImpNucId.resize(imp_nuc_num);
	for(int i = 0 ; i < imp_nuc_num ; ++i )
	{
		p_vImpNucId[i] = imp_nuc_id[i];
	}

	return;

}