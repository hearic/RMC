# include "ParallelClass.h"
# include "Tally.h"

# ifdef _IS_PARALLEL_

void CDParallel::GatherCycTally(CDTally & CTally)
{

	/////////////////////////  MPI_Reduce Cell Tally ///////////////////////////
	if(CTally.p_bIsWithCellTally)
	{
		if(p_bIsMaster)
		{
			MPI_Reduce(MPI_IN_PLACE,&CTally.p_OCellTallyData.p_vScore[0], CTally.p_OCellTallyData.p_vScore.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}
		else
		{				
			MPI_Reduce(&CTally.p_OCellTallyData.p_vScore[0],&CTally.p_OCellTallyData.p_vScore[0], CTally.p_OCellTallyData.p_vSum1.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	/////////////////////////  MPI_Reduce mesh Tally ///////////////////////////
	if(CTally.p_bIsWithMeshTally)
	{
		if(p_bIsMaster)
		{
			MPI_Reduce(MPI_IN_PLACE,&CTally.p_OMeshTallyData.p_vScore[0], CTally.p_OMeshTallyData.p_vScore.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}
		else
		{				
			MPI_Reduce(&CTally.p_OMeshTallyData.p_vScore[0],&CTally.p_OMeshTallyData.p_vScore[0], CTally.p_OMeshTallyData.p_vSum1.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	/////////////////////////  MPI_Reduce Cs Tally ///////////////////////////
	if(CTally.p_bIsWithCsTally)
	{
		if(!CTally.p_bUseUnionCsTally)
		{
			MPI_Allreduce(MPI_IN_PLACE,&CTally.p_OCsTallyData.p_vScore[0], CTally.p_OCsTallyData.p_vScore.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );	
		}
	}

	return;
	
}

void CDParallel::GatherHstryTally(CDTally & CTally)
{

	/////////////////////////  MPI_Sum Parallel Cell Tally ///////////////////////////
	if(CTally.p_bIsWithCellTally)
	{
		if(p_bIsMaster)
		{
			MPI_Reduce(MPI_IN_PLACE,&CTally.p_OCellTallyData.p_vSum1[0], CTally.p_OCellTallyData.p_vSum1.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
			MPI_Reduce(MPI_IN_PLACE,&CTally.p_OCellTallyData.p_vSum2[0], CTally.p_OCellTallyData.p_vSum2.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}
		else
		{				
			MPI_Reduce(&CTally.p_OCellTallyData.p_vSum1[0],&CTally.p_OCellTallyData.p_vSum1[0], CTally.p_OCellTallyData.p_vSum1.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
			MPI_Reduce(&CTally.p_OCellTallyData.p_vSum2[0],&CTally.p_OCellTallyData.p_vSum2[0], CTally.p_OCellTallyData.p_vSum2.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}


	/////////////////////////  MPI_Reduce Cs Tally ///////////////////////////
	if(CTally.p_bIsWithCsTally)
	{
		if(CTally.p_bUseUnionCsTally)
		{
			int nCsTallyNum = CTally.p_vOneGroupCsTally.size();
			for(int i = 0; i < nCsTallyNum; ++i )
			{
				int nSize = CTally.p_vOneGroupCsTally[i].p_vUnionFlux.size();
				MPI_Allreduce(MPI_IN_PLACE,&CTally.p_vOneGroupCsTally[i].p_vUnionFlux[0], nSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );		
			}
		}	
		else
		{
			MPI_Allreduce(MPI_IN_PLACE,&CTally.p_OCsTallyData.p_vSum1[0], CTally.p_OCsTallyData.p_vSum1.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );	
			MPI_Allreduce(MPI_IN_PLACE,&CTally.p_OCsTallyData.p_vSum2[0], CTally.p_OCsTallyData.p_vSum2.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );	
		}
	}

	return;
	
}
# endif