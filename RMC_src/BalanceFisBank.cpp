# include "Criticality.h"
# include "ParallelClass.h"

# ifdef _IS_PARALLEL_

void CDParallel::BalanceFisBank(int &SourceCount, int TotBankNum, int& nCycleNeuNum)
{

	//////////////////  Balancing the Fission Banks(including initial source) ////////////////////////

	//////////////////////// Get source number per processor /////////////////////////
	SourceCount = TotBankNum/p_nProcsNum;	    // average distribution		
	int nSourceCountRest = TotBankNum - SourceCount*p_nProcsNum ;
	if(p_nMyId < nSourceCountRest) 
	{
		SourceCount = SourceCount + 1 ;
	}
	nCycleNeuNum = SourceCount;    // particles for next cycle

	//cerr<<" 4 ok";

	MPI_Gather(&SourceCount, 1, MPI_INTEGER, &p_vSourceLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD);  
	//cerr<<" 5 ok";

	//////////////// Get accumulated source number before current processor /////////////////////
	///////////////////////////////// Get offsets for scatter(v) ////////////////////////////////
	if(p_bIsMaster)
	{
		p_vSourceLoadSum[0] = 0 ;
		p_vRandNumPos[0] = p_nRandNumSum	;  ///// accumulate offset: RN_POSITION
		for(int i = 1;i < p_nProcsNum; ++i )
		{
			p_vSourceLoadSum[i] = p_vSourceLoadSum[i-1] + p_vSourceLoad[i-1] ;
			p_vRandNumPos[i] = p_vRandNumPos[i-1] + p_vSourceLoad[i-1];
		}
		p_nRandNumSum = p_nRandNumSum + TotBankNum ; 
	}
	///////////////////////////////// Adjust Rand Number Position  ////////////////////////////////
	MPI_Scatter(&p_vRandNumPos[0], 1, MPI_INTEGER, &ORNG.RN_POSITION, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	return;

}
#endif