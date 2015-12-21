#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::InitiateBurnup(CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally)
{
	char chBurnSolver[][80]={"None","TTA",  "CRAM","QRAM","LPAM"};

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nInitiating burnup calculation...\nBurnup regions: %d   Burnup Solver: %s\n",
		p_nBurnCellNum,chBurnSolver[p_nBurnSolver]);}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	//////////////////////// Set burnup step to zero (burnup beginning) ////////////////////////
	p_nBurnStep = 0; 

	///////////////////////// Set default parameters for point-burnup //////////////////////////
	strcpy(Depth.InputFile,Output.p_chInputFileName);
	Depth.SetDefault();

# ifdef _IS_PARALLEL_
	Depth.IsRunInParallel = true;
	if (!OParallel.p_bIsMaster)	Depth.IsMasterProc = false;// set Depth master or not for print message
# endif
	Depth.SolveMethod = p_nBurnSolver;

	///////////////////////// Read point-burnup library and check ACE library //////////////////////
	//////////////////////// Read point-burnup library ////////////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Check burnup libraries...");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	strcpy(Depth.MainLibFile,p_chDepthLibForBurn); 
	Depth.ReadLib();

	p_vBurnLibData = Depth.LibData;

	int nTotBurnNucNum = Depth.NucNum;

	///////////////// Calculate fraction of branching ratio //////////////////

	if(p_nExcitedBranchFlag == 1)
	{
		p_vExcitedNrBranch.resize(nTotBurnNucNum + 1);
		p_vExcitedN2nBranch.resize(nTotBurnNucNum + 1);
		for(int i = 1 ; i <= nTotBurnNucNum ; ++i ) 
		{
			if(p_vBurnLibData[i][23] != 0)
			{
				p_vExcitedNrBranch[i] = p_vBurnLibData[i][23]/(p_vBurnLibData[i][23] + p_vBurnLibData[i][19]);
			}
			if(p_vBurnLibData[i][24] != 0)
			{
				p_vExcitedN2nBranch[i] = p_vBurnLibData[i][24]/(p_vBurnLibData[i][24] + p_vBurnLibData[i][20]);
			}
		}
	}

	/////////// check ACE library for the nuclide used in burnup ///////////////
	CheckAceForBurn();

	//////////  check material/nuclide in depletion cell: natural isotopes or other non-depletion nuclides
	CheckDepletionNuc(cGeometry, cMaterial);

	/////////////////////////////////  initiate vector size ////////////////////////////////////
	p_vCellBurnPower.resize(p_nBurnCellNum);
	p_vCellBurnAveEnergy.resize(p_nBurnCellNum);
	p_vCellActualFlux.resize(p_nBurnCellNum);
	p_vCellCmltvBurnup.resize(p_nBurnCellNum);
	CDGlobeFun::Set1DVecZero(p_vCellCmltvBurnup);

	int nBurnCellCount = p_nBurnCellNum;
	int nStartBurnCellIndex = 0;
	int nEndBurnCellIndex = p_nBurnCellNum;

# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsParallelBurn)
	{
		OParallel.BalanceBurnCell(nBurnCellCount,p_nBurnCellNum);
		nStartBurnCellIndex = OParallel.p_nStartBurnCellIndex;
		nEndBurnCellIndex = OParallel.p_nEndBurnCellIndex;
	}
# endif
	
	/////////////////////////// save initial material composition ///////////////////////////////
	p_vCellInitNucID.resize(nBurnCellCount);
	p_vCellInitNucDen.resize(nBurnCellCount);

	p_vCellBurnNucDen.resize(nBurnCellCount);
	p_vCellBurnNucDenPre.resize(nBurnCellCount);
	p_vCellBurnNucDenOld.resize(nBurnCellCount);

	p_vXeEqDen.resize(nBurnCellCount);

	for(int i = 0; i < nBurnCellCount; ++i)
	{
		p_vCellBurnNucDen[i].resize(nTotBurnNucNum + 1);
		p_vCellBurnNucDenPre[i].resize(nTotBurnNucNum + 1);
		p_vCellBurnNucDenOld[i].resize(nTotBurnNucNum + 1);

		int nTallyPos = p_nBurnTallyStartPos + i + nStartBurnCellIndex;
		int nCellIndexU = *(cTally.p_vOneGroupCsTally[nTallyPos].p_vCellVecU.end()-1);
		int nCellIndex = cGeometry.p_OCellIndex.GetIndex(nCellIndexU);
		double dVol = cGeometry.p_vCell[nCellIndex].p_dVol;
		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		int nMatNucNum = cMaterial.GetMatTotNucNum(nMat);
		
		p_vCellInitNucID[i].resize(nMatNucNum);
		p_vCellInitNucDen[i].resize(nMatNucNum);

		for(int j = 0 ; j < nMatNucNum ; ++j )
		{
			p_vCellInitNucID[i][j] = cMaterial.GetMatNucID(nMat,j);
			p_vCellInitNucDen[i][j] = dVol * cMaterial.GetMatNucAtomDen(nMat,j);// volume is important!

			int index = ConvertNucId(cMaterial.GetMatNucID(nMat,j).p_chIdStr);
			if(index < 0 ) 
			{
				continue;
			}
			p_vCellBurnNucDen[i][index] =  dVol*cMaterial.GetMatNucAtomDen(nMat,j);
		}
	}


	//////////////////////////////////  build output cell index ////////////////////////////////
	CheckOutputCells(cGeometry,cTally);

	//////////////////////////////  build important nuclide index //////////////////////////////
	GenImpNucIndex();

	return;
}

