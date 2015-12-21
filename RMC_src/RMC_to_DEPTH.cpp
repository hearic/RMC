#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::RMC_to_DEPTH(CDMaterial &cMaterial,CDTally &cTally,int nBurnCellIndex)
{
	int nTallyPos = p_nBurnTallyStartPos + nBurnCellIndex ;
	int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
	int nMatNucNum = cMaterial.GetMatTotNucNum(nMat);

	/////////////////////  Passing one-group cross sections  ///////////////////////
	Depth.LibData = p_vBurnLibData ;
	for( int i = 0; i < nMatNucNum; ++i )
	{
	    ////// find nuclide nIndex in depth
		int nIndex = ConvertNucId(cMaterial.GetMatNucID(nMat,i).p_chIdStr);
    	if(nIndex < 0 ) 
		{
			continue;
		}

		//////  one-group cross sections
		for ( int j = 0 ; j < 4 ; ++j )
		{
			int nPtr = cTally.p_vOneGroupCsTally[nTallyPos].GetNucDataPtr(i) + j;
			Depth.LibData[nIndex][j + 19] = cTally.p_OCsTallyData.p_vAve[nPtr];
		}
		if(p_nExcitedBranchFlag == 1)
		{
			Depth.LibData[nIndex][23] = Depth.LibData[nIndex][19]*p_vExcitedNrBranch[nIndex];
			Depth.LibData[nIndex][19] = Depth.LibData[nIndex][19] - Depth.LibData[nIndex][23];
			Depth.LibData[nIndex][24] = Depth.LibData[nIndex][20]*p_vExcitedN2nBranch[nIndex];
			Depth.LibData[nIndex][20] = Depth.LibData[nIndex][20] - Depth.LibData[nIndex][24];
		}
	}

	//////////////////////  Passing nuclide composition  ///////////////////////////
	int nBurn_cell_index = nBurnCellIndex;
# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsParallelBurn)
	{
		nBurn_cell_index = nBurnCellIndex - OParallel.p_nStartBurnCellIndex;
	}
# endif

	Depth.N0 = p_vCellBurnNucDen[nBurn_cell_index];


	/////////////////////////   setup burnup parameters /////////////////////////////
	double dStepTime = p_vStepBurnTime[p_nBurnStep + 1];

	Depth.StepNum = p_nInnerStepNum;
	Depth.StepPower.resize(p_nInnerStepNum + 1);
	Depth.StepFlux.resize(p_nInnerStepNum + 1);
	Depth.StepMode.resize(p_nInnerStepNum + 1);
	Depth.StepTime.resize(p_nInnerStepNum + 1);

	Depth.StepTime[0] = 0;
	for ( int i = 1 ; i <= p_nInnerStepNum ; ++i)
	{
		Depth.StepPower[i] = p_vCellBurnPower[nBurnCellIndex]; 
		/*if(Depth.StepPower[i] < 1.0E-10)
		{
			Depth.StepPower[i] = 1.0E-10;// for burnable region without fissile isotopes.
		}*/
		
		Depth.StepMode[i] = p_nDepleteMode;
		
		Depth.StepTime[i] =  dStepTime*3600*24/p_nInnerStepNum;
	}
	Depth.AbsRateFlag = 1 ;

	// Transfer cell average energy. 
	Depth.AveEnergy = p_vCellBurnAveEnergy[nBurnCellIndex]; // 

	///////////////////////////////// Run Depth /////////////////////////////////      
	Depth.Initiate();

	Depth.Execute();

	//Depth.Output();

	return;
}

