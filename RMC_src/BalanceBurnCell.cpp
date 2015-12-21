# include "Criticality.h"
# include "ParallelClass.h"

# ifdef _IS_PARALLEL_

void CDParallel::BalanceBurnCell(int &nBurnCellCount, int nTotBurnCellNum)
{
	nBurnCellCount = nTotBurnCellNum/p_nProcsNum;			// average distribution		
	int BurnCellRest = nTotBurnCellNum - nBurnCellCount*p_nProcsNum ;
	if(p_nMyId < BurnCellRest) 
	{
		nBurnCellCount = nBurnCellCount + 1 ;
	}
	MPI_Allgather(&nBurnCellCount, 1, MPI_INTEGER, &p_vBurnCellLoad[0], 1, MPI_INTEGER, MPI_COMM_WORLD);  

	// Get offsets for scatter(v)//
	p_vBurnCellLoadSum[0] = 0 ;
	for(int i = 1;i <= p_nProcsNum; ++i )
	{
		p_vBurnCellLoadSum[i] = p_vBurnCellLoadSum[i-1] + p_vBurnCellLoad[i-1] ;
	}

	p_nStartBurnCellIndex = p_vBurnCellLoadSum[p_nMyId];
	p_nEndBurnCellIndex = p_nStartBurnCellIndex + nBurnCellCount ;

	return;
}

#endif