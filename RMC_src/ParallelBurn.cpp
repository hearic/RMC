# include "Burnup.h"
# include "ParallelClass.h"

# ifdef _IS_PARALLEL_

void CDParallel::ParallelBurn(CDBurnup& cBurnup, CDMaterial& cMaterial, CDTally& cTally)
{
	//static vector<int> BurnCellLoad(ProcsNum);
	//static vector<int> BurnCellLoadSum(ProcsNum + 1);
	//int BurnCellCount;			// average distribution		
	//int BurnCellRest;
	//static int StartCellIndex ;
	//static int EndCellIndex ;


	//////////////////////////  Get Cell index interval //////////////////////////////////////
	//if(Burnup.BurnStep == 0)    // At the beginning step
	//{
	//	BurnCellCount = Burnup.BurnCellNum/ProcsNum;			// average distribution		
	//	BurnCellRest = Burnup.BurnCellNum - BurnCellCount*ProcsNum ;
	//	if(MyId < BurnCellRest) 
	//	{
	//		BurnCellCount = BurnCellCount + 1 ;
	//	}
	//	MPI_Allgather(&BurnCellCount, 1, MPI_INTEGER, &BurnCellLoad[0], 1, MPI_INTEGER, MPI_COMM_WORLD);  

	//	// Get offsets for scatter(v)//
	//	BurnCellLoadSum[0] = 0 ;
	//	for(int i = 1;i <= ProcsNum; ++i )
	//	{
	//		BurnCellLoadSum[i] = BurnCellLoadSum[i-1] + BurnCellLoad[i-1] ;
	//	}
	//	StartCellIndex = BurnCellLoadSum[MyId];
	//	EndCellIndex = StartCellIndex + BurnCellCount;
	//}

	vector<int> vMatNucNum(cBurnup.p_nBurnCellNum,0) ;
	//////////////////////////  Run RMC-Depth //////////////////////////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nRunning DEPTH for %d burnable cells in total. Progress:       ",cBurnup.p_nBurnCellNum);},CDOutput::_NORMAL);
	for( int nIndex = p_nStartBurnCellIndex; nIndex < p_nEndBurnCellIndex; ++nIndex )
	{

		if(cBurnup.p_nBurnStepStrategy > 0)
		{
			int nIndex_temp = nIndex - p_nStartBurnCellIndex;
			cBurnup.p_vCellBurnNucDenOld[nIndex_temp] = cBurnup.p_vCellBurnNucDen[nIndex_temp]; // Save Status at predictor steps
		} 

		cBurnup.RMC_to_DEPTH(cMaterial,cTally,nIndex);
		cBurnup.DEPTH_to_RMC(cMaterial,cTally,nIndex);
	
		int nTallyPos = cBurnup.p_nBurnTallyStartPos + nIndex ;
		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		vMatNucNum[nIndex] = cMaterial.p_vMatSet[nMat].p_vMatNucID.size();  // nuclide number of current mat

		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\b\b\b\b\b%3d %%%%",(nIndex+1)*100/p_nEndBurnCellIndex);},CDOutput::_NORMAL);
	}
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n");},CDOutput::_NORMAL);


	//////////////////////////  Resize MatNucID & MatNucUserDen /////////////////////////////////
	MPI_Allreduce(MPI_IN_PLACE,&vMatNucNum[0], cBurnup.p_nBurnCellNum, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
	for( int nIndex = 0; nIndex < cBurnup.p_nBurnCellNum; ++nIndex )
	{
		int nTallyPos = cBurnup.p_nBurnTallyStartPos + nIndex ;
		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		cMaterial.p_vMatSet[nMat].p_vMatNucID.resize(vMatNucNum[nIndex]);
		cMaterial.p_vMatSet[nMat].p_vMatNucAtomDen.resize(vMatNucNum[nIndex]);
		cMaterial.p_vMatSet[nMat].p_dMatTotNucNum = vMatNucNum[nIndex];
	}
	//MPI_Barrier(MPI_COMM_WORLD);


	//////////////////////////  Bcast Material information //////////////////////////////////////
	int nSrcId = 0;
	for( int nIndex = 0; nIndex < cBurnup.p_nBurnCellNum; ++nIndex )
	{
		if(nIndex >= p_vBurnCellLoadSum[nSrcId+1])
		{
			++ nSrcId ;
		}

		int nTallyPos = cBurnup.p_nBurnTallyStartPos + nIndex ;
		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;

		///  Bcast Material Nuc ID ////
		void *pDataPtr1 = &cMaterial.p_vMatSet[nMat].p_vMatNucID[0];
		int nDataSize1 = 12*cMaterial.p_vMatSet[nMat].p_vMatNucID.size();
		MPI_Bcast(pDataPtr1, nDataSize1, MPI_CHAR, nSrcId, MPI_COMM_WORLD);

		///  Bcast Material Nuc Density ////
		void *pDataPtr2 = &cMaterial.p_vMatSet[nMat].p_vMatNucAtomDen[0];
		int nDataSize2 = cMaterial.p_vMatSet[nMat].p_vMatNucAtomDen.size();
		MPI_Bcast(pDataPtr2, nDataSize2, MPI_DOUBLE, nSrcId, MPI_COMM_WORLD);

		///  Bcast Material Density ////
		//void *DataPtr3 = &Material.MatSet[Mat].MatUserDen;
		//int DataSize3 = 1;
		//MPI_Bcast(DataPtr3, DataSize3, MPI_DOUBLE, SrcId, MPI_COMM_WORLD);

		///  Bcast CellBurnNucDen ////
		//MPI_Bcast(&Burnup.CellBurnNucDen[Index][1], Depth.NucNum, MPI_DOUBLE, SrcId, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	return;
}
# endif