# include "ParallelClass.h"
# include "Criticality.h"

# ifdef _IS_PARALLEL_

void CDParallel::InitiateParallel()
{

	char chProc_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm_rank(MPI_COMM_WORLD,&p_nMyId);
	MPI_Comm_size(MPI_COMM_WORLD,&p_nProcsNum);
	MPI_Get_processor_name(chProc_name,&p_nNameLen);
	
    if(p_nMyId == 0)
	{
		p_bIsMaster = true ;
	}
	else
	{
		p_bIsMaster = false ;
	}

	//////////////////////////// parallel transport calculation ///////////////////////
	p_bUseFastBank = true ;
	p_vBankLoad.resize(p_nProcsNum);
	p_vBankLoadSum.resize(p_nProcsNum + 1);
	p_vSourceLoad.resize(p_nProcsNum);
	p_vSourceLoadSum.resize(p_nProcsNum + 1);
	p_vRandNumPos.resize(p_nProcsNum);
	p_nRandNumSum = 0 ;

	//////////////////////////// parallel burnup calculation ///////////////////////
	int nBlocklens[1]={7};  //{7,12};
	MPI_Aint indices[1]={0}; //{0,7*sizeof(double)};
	MPI_Datatype oldtypes[1]={MPI_DOUBLE}; //{MPI_DOUBLE,MPI_INT};
	MPI_Type_struct(1,nBlocklens,indices,oldtypes,&MPI_BankDataType);
	MPI_Type_commit(&MPI_BankDataType);

	int nBlocklens_fix[1]={7};  //{7,12};
	MPI_Aint indices_fix[1]={0}; //{0,7*sizeof(double)};
	MPI_Datatype oldtypes_fix[1]={MPI_DOUBLE}; //{MPI_DOUBLE,MPI_INT};
	MPI_Type_struct(1,nBlocklens_fix,indices_fix,oldtypes_fix,&MPI_BankDataType_fix);
	MPI_Type_commit(&MPI_BankDataType_fix);

	setvbuf( stdout, NULL, _IONBF, 0 );   // Print immediately. 


	//////////////////////////// parallel burnup calculation ///////////////////////
	p_bIsParallelBurn = true;
	p_vBurnCellLoad.resize(p_nProcsNum);
	p_vBurnCellLoadSum.resize(p_nProcsNum + 1);


	return;

}

# endif