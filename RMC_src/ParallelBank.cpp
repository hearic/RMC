# include "Criticality.h"
# include "ParallelClass.h"

# ifdef _IS_PARALLEL_

void CDParallel::ParallelBank(CDCriticality& cCriticality)
{
	///////////////////// Gather(MPI::Gatherv) number of Fission Banks //////////////////////////////////
	// Get Bankcount
	int nBankCount = cCriticality.p_nFisBankCount ;
	int nTotBankNum = cCriticality.p_nTotFisBankNum;

	// Get BankLoad
	MPI_Gather(&nBankCount, 1, MPI_INTEGER, &p_vBankLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// Get BankLoadSum 
	if(p_bIsMaster)
	{
		p_vBankLoadSum[0] = 0 ;
		for(int i = 1; i < p_nProcsNum; ++i )
		{
			p_vBankLoadSum[i] = p_vBankLoadSum[i-1] + p_vBankLoad[i-1] ;
		}
	}


	/////////////////////////////////// Master-salve parallel mode////////////////////////////////////
	if(!p_bUseFastBank)
	{
		// Gatherv Fission Bank DATA
		if(p_bIsMaster)
		{
			if(cCriticality.p_vFissionSrc.size() < cCriticality.p_nTotFisBankNum )
			{
				cCriticality.p_vFissionSrc.resize(cCriticality.p_nTotFisBankNum); 
			}
		}
		MPI_Gatherv(&cCriticality.p_vFissionBank[0],cCriticality.p_nFisBankCount,MPI_BankDataType,
			&cCriticality.p_vFissionSrc[0],&p_vBankLoad[0],&p_vBankLoadSum[0],MPI_BankDataType,0,MPI_COMM_WORLD);

		// Load balance
		int SourceCount = 0;				
		BalanceFisBank(SourceCount,nTotBankNum,cCriticality.p_nCycleNeuNum);

		// Scatterv Fission Bank DATA
		if(p_bIsMaster)
		{
			MPI_Scatterv(&cCriticality.p_vFissionSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType, 
				MPI_IN_PLACE,SourceCount, MPI_BankDataType,0, MPI_COMM_WORLD);	
		}
		else
		{
			MPI_Scatterv(&cCriticality.p_vFissionSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType, 
				&cCriticality.p_vFissionSrc[0],SourceCount, MPI_BankDataType,0, MPI_COMM_WORLD);	
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("Thread: %d , Fission Bank: %d / %d , RN_POSITION %d !\n",MyId,BankCount,TotBankNum,RNG.RN_POSITION);
		return;
	}

	
	/////////////////////////////// Advanced parallel fission bank algorithm ////////////////////////////////
	// Load balance
	int nSourceCount = 0;				
	BalanceFisBank(nSourceCount,nTotBankNum,cCriticality.p_nCycleNeuNum);

	if(cCriticality.p_vFissionSrc.size() < nSourceCount)
	{
		cCriticality.p_vFissionSrc.resize(nSourceCount); 
	}


	//printf("%d\n",MyId);
	//system("Pause");

	p_vBankLoadSum[p_nProcsNum] = nTotBankNum ;
	p_vSourceLoadSum[p_nProcsNum] = nTotBankNum ;
	MPI_Bcast(&p_vBankLoadSum[0], p_nProcsNum+1 , MPI_INT, 0 ,MPI_COMM_WORLD);
	MPI_Bcast(&p_vSourceLoadSum[0], p_nProcsNum+1 , MPI_INT, 0 ,MPI_COMM_WORLD);

	// search Sending/Receiving location 
	int nSendId;
	int nRecvId;
	ParallelBankSearch(nSendId,nRecvId);
	//MPI_Barrier(MPI_COMM_WORLD);

	// Send fission banks unitil the number = BankCount 
	int nSendCount = 0 ;
	int nSendLen;
	int nSendTimes = 0 ;
	int nMaxIter = 10000;
	vector<MPI_Request> vSend_request;

	for(;;)
	{
		if(nSendTimes >= nMaxIter)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many iteration for parallel sending!\n");},CDOutput::_WARNING);
		}

		if(p_vSourceLoadSum[nSendId+1] >= p_vBankLoadSum[p_nMyId+1])
		{
			nSendLen = nBankCount - nSendCount ;
			if(nSendLen > 0)
			{
				nSendTimes ++;
				vSend_request.push_back(0);
				MPI_Isend(&cCriticality.p_vFissionBank[nSendCount], nSendLen ,MPI_BankDataType,nSendId,0,MPI_COMM_WORLD,&vSend_request[nSendTimes-1]);
			}
			//printf("MyId=%d SendLen=%d, SendId=%d\n",MyId,SendLen,SendId);
			break;
		}

		nSendLen = p_vSourceLoadSum[nSendId+1] - p_vBankLoadSum[p_nMyId] - nSendCount;
		if(nSendLen > 0)
		{
			nSendTimes ++;
			vSend_request.push_back(0);
			MPI_Isend(&cCriticality.p_vFissionBank[nSendCount], nSendLen ,MPI_BankDataType,nSendId,0,MPI_COMM_WORLD,&vSend_request[nSendTimes-1]);
		}

		++nSendId;
		nSendCount = nSendCount + nSendLen ;
		//printf("MyId=%d SendLen=%d, SendId=%d\n",MyId,SendLen,SendId);
	}

	// Receive fission sources unitil the number = SourceCount 
	int nRecvCount = 0 ;
	int nRecvLen;
	int nRecvTimes = 0 ;
	vector<MPI_Request> vRecv_request;
	for(;;)
	{

		if(nRecvTimes >= nMaxIter)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many iteration for parallel receiving!\n");},CDOutput::_WARNING);
		}

		if(p_vBankLoadSum[nRecvId + 1] >= p_vSourceLoadSum[p_nMyId + 1])
		{
			nRecvLen = nSourceCount - nRecvCount ;
			if(nRecvLen > 0)
			{
				nRecvTimes ++;
				vRecv_request.push_back(0);
				MPI_Irecv(&cCriticality.p_vFissionSrc[nRecvCount], nRecvLen ,MPI_BankDataType,nRecvId,0,MPI_COMM_WORLD,&vRecv_request[nRecvTimes-1]);
			}
			//printf("MyId=%d RecvLen=%d, RecvId=%d\n",MyId,RecvLen,RecvId);
			break;
		}

		nRecvLen = p_vBankLoadSum[nRecvId+1] - p_vSourceLoadSum[p_nMyId] - nRecvCount;
		if(nRecvLen > 0)
		{
			nRecvTimes ++;
			vRecv_request.push_back(0);
			MPI_Irecv(&cCriticality.p_vFissionSrc[nRecvCount], nRecvLen ,MPI_BankDataType,nRecvId,0,MPI_COMM_WORLD,&vRecv_request[nRecvTimes-1]);
		}

		++nRecvId;
		nRecvCount = nRecvCount + nRecvLen ;
		//printf("MyId=%d RecvLen=%d, RecvId=%d\n",MyId,RecvLen,RecvId);
	}

	//printf("Thread: %d, src/bank/tot: %d/%d/%d, S/R ID %d/%d\n",MyId,SourceCount,BankCount,TotBankNum,SendId,RecvId);


	// wait and test all Send & Recv procedure
	if(vSend_request.size() > 0)
	{
		MPI_Waitall(nSendTimes, &vSend_request[0], MPI_STATUSES_IGNORE);
	}
	MPI_Waitall(nRecvTimes, &vRecv_request[0], MPI_STATUSES_IGNORE);


	// test 
	int nSendTestFlag, nRecvTestFlag;
	if(vSend_request.size() > 0)
	{
		MPI_Testall(nSendTimes,&vSend_request[0],&nSendTestFlag,MPI_STATUSES_IGNORE);
	}
	MPI_Testall(nRecvTimes,&vRecv_request[0],&nRecvTestFlag,MPI_STATUSES_IGNORE);

	if(nSendTestFlag == 0 || nRecvTestFlag == 0)
	{
		printf("Error in sending fission banks. Thread:%d, Send:%d, Recv:%d\n",p_nMyId,nSendTestFlag, nRecvTestFlag);
	}

	//MPI_Barrier(MPI_COMM_WORLD);

	return;

}


void CDParallel::ParallelBank(CDFixedSource& cFixedSource)
{
	///////////////////// Gather(MPI::Gatherv) number of Fission Banks //////////////////////////////////
	// Get Bankcount
	int nBankCount = cFixedSource.p_nFixedSrcBankCount ;
	int nTotBankNum = cFixedSource.p_nTotFixedSrcBankNum;

	// Get BankLoad
	MPI_Gather(&nBankCount, 1, MPI_INTEGER, &p_vBankLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// Get BankLoadSum 
	if(p_bIsMaster)
	{
		p_vBankLoadSum[0] = 0 ;
		for(int i = 1; i < p_nProcsNum; ++i )
		{
			p_vBankLoadSum[i] = p_vBankLoadSum[i-1] + p_vBankLoad[i-1] ;
		}
	}


	/////////////////////////////////// Master-salve parallel mode////////////////////////////////////
	if(!p_bUseFastBank)
	{
		// Gatherv Fission Bank DATA
		if(p_bIsMaster)
		{
			if(cFixedSource.p_vFixedSrc.size() < cFixedSource.p_nTotFixedSrcBankNum )
			{
				cFixedSource.p_vFixedSrc.resize(cFixedSource.p_nTotFixedSrcBankNum); 
			}
		}
		MPI_Gatherv(&cFixedSource.p_vFixedSrcBank[0],cFixedSource.p_nFixedSrcBankCount,MPI_BankDataType_fix,
			&cFixedSource.p_vFixedSrc[0],&p_vBankLoad[0],&p_vBankLoadSum[0],MPI_BankDataType_fix,0,MPI_COMM_WORLD);

		// Load balance
		int SourceCount = 0;				
		BalanceFisBank(SourceCount,nTotBankNum,cFixedSource.p_nNeuNum);

		// Scatterv Fission Bank DATA
		if(p_bIsMaster)
		{
			MPI_Scatterv(&cFixedSource.p_vFixedSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType_fix, 
				MPI_IN_PLACE,SourceCount, MPI_BankDataType_fix,0, MPI_COMM_WORLD);	
		}
		else
		{
			MPI_Scatterv(&cFixedSource.p_vFixedSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType_fix, 
				&cFixedSource.p_vFixedSrc[0],SourceCount, MPI_BankDataType_fix,0, MPI_COMM_WORLD);	
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("Thread: %d , Fission Bank: %d / %d , RN_POSITION %d !\n",MyId,BankCount,TotBankNum,RNG.RN_POSITION);
		return;
	}


	/////////////////////////////// Advanced parallel fission bank algorithm ////////////////////////////////
	// Load balance
	int nSourceCount = 0;				
	BalanceFisBank(nSourceCount,nTotBankNum,cFixedSource.p_nNeuNum);

	if(cFixedSource.p_vFixedSrc.size() < nSourceCount)
	{
		cFixedSource.p_vFixedSrc.resize(nSourceCount); 
	}


	//printf("%d\n",MyId);
	//system("Pause");

	p_vBankLoadSum[p_nProcsNum] = nTotBankNum ;
	p_vSourceLoadSum[p_nProcsNum] = nTotBankNum ;
	MPI_Bcast(&p_vBankLoadSum[0], p_nProcsNum+1 , MPI_INT, 0 ,MPI_COMM_WORLD);
	MPI_Bcast(&p_vSourceLoadSum[0], p_nProcsNum+1 , MPI_INT, 0 ,MPI_COMM_WORLD);

	// search Sending/Receiving location 
	int nSendId;
	int nRecvId;
	ParallelBankSearch(nSendId,nRecvId);
	//MPI_Barrier(MPI_COMM_WORLD);

	// Send fission banks unitil the number = BankCount 
	int nSendCount = 0 ;
	int nSendLen;
	int nSendTimes = 0 ;
	int nMaxIter = 10000;
	vector<MPI_Request> vSend_request;

	for(;;)
	{
		if(nSendTimes >= nMaxIter)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many iteration for parallel sending!\n");},CDOutput::_WARNING);
		}

		if(p_vSourceLoadSum[nSendId+1] >= p_vBankLoadSum[p_nMyId+1])
		{
			nSendLen = nBankCount - nSendCount ;
			if(nSendLen > 0)
			{
				nSendTimes ++;
				vSend_request.push_back(0);
				MPI_Isend(&cFixedSource.p_vFixedSrcBank[nSendCount], nSendLen ,MPI_BankDataType_fix,nSendId,0,MPI_COMM_WORLD,&vSend_request[nSendTimes-1]);
			}
			//printf("MyId=%d SendLen=%d, SendId=%d\n",MyId,SendLen,SendId);
			break;
		}

		nSendLen = p_vSourceLoadSum[nSendId+1] - p_vBankLoadSum[p_nMyId] - nSendCount;
		if(nSendLen > 0)
		{
			nSendTimes ++;
			vSend_request.push_back(0);
			MPI_Isend(&cFixedSource.p_vFixedSrcBank[nSendCount], nSendLen ,MPI_BankDataType_fix,nSendId,0,MPI_COMM_WORLD,&vSend_request[nSendTimes-1]);
		}

		++nSendId;
		nSendCount = nSendCount + nSendLen ;
		//printf("MyId=%d SendLen=%d, SendId=%d\n",MyId,SendLen,SendId);
	}

	// Receive fission sources unitil the number = SourceCount 
	int nRecvCount = 0 ;
	int nRecvLen;
	int nRecvTimes = 0 ;
	vector<MPI_Request> vRecv_request;
	for(;;)
	{

		if(nRecvTimes >= nMaxIter)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many iteration for parallel receiving!\n");},CDOutput::_WARNING);
		}

		if(p_vBankLoadSum[nRecvId + 1] >= p_vSourceLoadSum[p_nMyId + 1])
		{
			nRecvLen = nSourceCount - nRecvCount ;
			if(nRecvLen > 0)
			{
				nRecvTimes ++;
				vRecv_request.push_back(0);
				MPI_Irecv(&cFixedSource.p_vFixedSrc[nRecvCount], nRecvLen ,MPI_BankDataType_fix,nRecvId,0,MPI_COMM_WORLD,&vRecv_request[nRecvTimes-1]);
			}
			//printf("MyId=%d RecvLen=%d, RecvId=%d\n",MyId,RecvLen,RecvId);
			break;
		}

		nRecvLen = p_vBankLoadSum[nRecvId+1] - p_vSourceLoadSum[p_nMyId] - nRecvCount;
		if(nRecvLen > 0)
		{
			nRecvTimes ++;
			vRecv_request.push_back(0);
			MPI_Irecv(&cFixedSource.p_vFixedSrc[nRecvCount], nRecvLen ,MPI_BankDataType_fix,nRecvId,0,MPI_COMM_WORLD,&vRecv_request[nRecvTimes-1]);
		}

		++nRecvId;
		nRecvCount = nRecvCount + nRecvLen ;
		//printf("MyId=%d RecvLen=%d, RecvId=%d\n",MyId,RecvLen,RecvId);
	}

	//printf("Thread: %d, src/bank/tot: %d/%d/%d, S/R ID %d/%d\n",MyId,SourceCount,BankCount,TotBankNum,SendId,RecvId);


	// wait and test all Send & Recv procedure
	if(vSend_request.size() > 0)
	{
		MPI_Waitall(nSendTimes, &vSend_request[0], MPI_STATUSES_IGNORE);
	}
	MPI_Waitall(nRecvTimes, &vRecv_request[0], MPI_STATUSES_IGNORE);


	// test 
	int nSendTestFlag, nRecvTestFlag;
	if(vSend_request.size() > 0)
	{
		MPI_Testall(nSendTimes,&vSend_request[0],&nSendTestFlag,MPI_STATUSES_IGNORE);
	}
	MPI_Testall(nRecvTimes,&vRecv_request[0],&nRecvTestFlag,MPI_STATUSES_IGNORE);

	if(nSendTestFlag == 0 || nRecvTestFlag == 0)
	{
		printf("Error in sending fission banks. Thread:%d, Send:%d, Recv:%d\n",p_nMyId,nSendTestFlag, nRecvTestFlag);
	}

	//MPI_Barrier(MPI_COMM_WORLD);

	return;

}


void CDParallel::ParallelBank_ExternalSrc(CDCriticality& cCriticality)
{
	//broadcast p_dDeltaTime from source file
	MPI_Bcast(&cCriticality.p_dDeltaTime,1,MPI_DOUBLE,0,MPI_COMM_WORLD);


	// Load balance
	int SourceCount = 0;	
	int nTotBankNum = cCriticality.p_nNeuNumPerCyc;
	BalanceFisBank(SourceCount,nTotBankNum,cCriticality.p_nCycleNeuNum);

	// Scatterv Fission Bank DATA
	if(SourceCount>cCriticality.p_vFissionSrc.size()) cCriticality.p_vFissionSrc.resize(SourceCount);
	MPI_Scatterv(&cCriticality.p_vFixedSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType, 
		&cCriticality.p_vFissionSrc[0],SourceCount, MPI_BankDataType,0, MPI_COMM_WORLD);	



	MPI_Barrier(MPI_COMM_WORLD);


	//printf("Thread: %d , Fission Bank: %d / %d , RN_POSITION %d !\n",MyId,BankCount,TotBankNum,RNG.RN_POSITION);
	return;
}


void CDParallel::ParallelBank_QuasiStatic(CDCriticality& cCriticality)
{
	///////////////////// Gather(MPI::Gatherv) number of Fission Banks //////////////////////////////////
	// Get Bankcount
	int nBankCount = cCriticality.p_nFisBankCount ;
	int nTotBankNum = cCriticality.p_nTotFisBankNum;

	// Get BankLoad
	MPI_Gather(&nBankCount, 1, MPI_INTEGER, &p_vBankLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// Get BankLoadSum 
	if(p_bIsMaster)
	{
		p_vBankLoadSum[0] = 0 ;
		for(int i = 1; i < p_nProcsNum; ++i )
		{
			p_vBankLoadSum[i] = p_vBankLoadSum[i-1] + p_vBankLoad[i-1] ;
		}
	}


	// Gatherv Fission Bank DATA
	if(p_bIsMaster)
	{
		if(cCriticality.p_vFissionSrc.size() < cCriticality.p_nTotFisBankNum )
		{
			cCriticality.p_vFissionSrc.resize(cCriticality.p_nTotFisBankNum); 
		}
	}
	MPI_Gatherv(&cCriticality.p_vFissionBank[0],cCriticality.p_nFisBankCount,MPI_BankDataType,
		&cCriticality.p_vFissionSrc[0],&p_vBankLoad[0],&p_vBankLoadSum[0],MPI_BankDataType,0,MPI_COMM_WORLD);

	if(p_bIsMaster)
	{
		int nExternalSrcNum = cCriticality.p_nNeuNumPerCyc - cCriticality.p_nTotFisBankNum;


		ORNG.RN_POSITION_PRE=-1000;
		ORNG.RN_POSITION=0;
		ORNG.GetRandSeed();

		for(int i=0;i<nExternalSrcNum;i++)
		{
			int idx = ORNG.Rand()*cCriticality.p_nFixedSrcSize;
			if(idx==cCriticality.p_nFixedSrcSize) idx = cCriticality.p_nFixedSrcSize-1;
			cCriticality.p_vFissionSrc[cCriticality.p_nTotFisBankNum+i] = cCriticality.p_vFixedSrc[idx];
		}

		ORNG.RN_POSITION_PRE=-1000;
		ORNG.RN_POSITION=0;

	}

	// Load balance
	int SourceCount = 0;				
	BalanceFisBank(SourceCount,cCriticality.p_nNeuNumPerCyc,cCriticality.p_nCycleNeuNum);


	// Scatterv Fission Bank DATA
	if(SourceCount>cCriticality.p_vFissionSrc.size()) cCriticality.p_vFissionSrc.resize(SourceCount);
	if(p_bIsMaster)
	{
		MPI_Scatterv(&cCriticality.p_vFissionSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType, 
			MPI_IN_PLACE,SourceCount, MPI_BankDataType,0, MPI_COMM_WORLD);	
	}
	else
	{
		MPI_Scatterv(&cCriticality.p_vFissionSrc[0], &p_vSourceLoad[0], &p_vSourceLoadSum[0], MPI_BankDataType, 
			&cCriticality.p_vFissionSrc[0],SourceCount, MPI_BankDataType,0, MPI_COMM_WORLD);	
	}

	MPI_Barrier(MPI_COMM_WORLD);
	//printf("Thread: %d , Fission Bank: %d / %d , RN_POSITION %d !\n",MyId,BankCount,TotBankNum,RNG.RN_POSITION);
	return;
}

void CDParallel::ParallelTimeAbsNeuBank(CDCriticality& cCriticality)
{

	// Get Total Bankcount
	int nTotTimeAbsNeuBankCount;
	MPI_Allreduce(&cCriticality.p_nTimeAbsNeuBankCount,&nTotTimeAbsNeuBankCount,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	int nBankCount = cCriticality.p_nTimeAbsNeuBankCount;

	// Get BankLoad
	MPI_Gather(&nBankCount, 1, MPI_INTEGER, &p_vBankLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// Get BankLoadSum 
	if(p_bIsMaster)
	{
		p_vBankLoadSum[0] = 0 ;
		for(int i = 1; i < p_nProcsNum; ++i )
		{
			p_vBankLoadSum[i] = p_vBankLoadSum[i-1] + p_vBankLoad[i-1] ;
		}
	}

	// Gatherv Time absorbed neutron Bank DATA
	int offset=0;
	if(p_bIsMaster)
	{
		offset = cCriticality.p_nTimeAbsNeuSrcCount;
		cCriticality.p_nTimeAbsNeuSrcCount = offset + nTotTimeAbsNeuBankCount;
	}

	if(p_bIsMaster)
	{
		//	printf("before resize, srccount=%d\n",cCriticality.p_nTimeAbsNeuSrcCount);
		if(cCriticality.p_vTimeAbsNeuSrc.size() < cCriticality.p_nTimeAbsNeuSrcCount)
		{
			cCriticality.p_vTimeAbsNeuSrc.resize(cCriticality.p_nTimeAbsNeuSrcCount); 
		}
		//	printf("after resize, srccount=%d\n",cCriticality.p_nTimeAbsNeuSrcCount);
	}

	MPI_Bcast(&offset, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&cCriticality.p_nTimeAbsNeuSrcCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Gatherv(&cCriticality.p_vTimeAbsNeuBank[0],cCriticality.p_nTimeAbsNeuBankCount,MPI_BankDataType,
		&cCriticality.p_vTimeAbsNeuSrc[offset],&p_vBankLoad[0],&p_vBankLoadSum[0],MPI_BankDataType,0,MPI_COMM_WORLD);

	if(cCriticality.p_nTimeAbsNeuSrcCount>5*cCriticality.p_nNeuNumPerCyc) 
	{
		cCriticality.p_nTimeAbsNeuSrcCount=5*cCriticality.p_nNeuNumPerCyc;
		cCriticality.p_bIsRecordTimeAbsNeu=false;
	}


	MPI_Barrier(MPI_COMM_WORLD);
	return;

}

void CDParallel::ParallelDelayedNeuBank(CDCriticality& cCriticality)
{

	// Get Total Bankcount
	int nTotDelayedNeuBankCount;
	MPI_Allreduce(&cCriticality.p_nDelayedNeuBankCount,&nTotDelayedNeuBankCount,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	int nBankCount = cCriticality.p_nDelayedNeuBankCount;

	// Get BankLoad
	MPI_Gather(&nBankCount, 1, MPI_INTEGER, &p_vBankLoad[0], 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// Get BankLoadSum 
	if(p_bIsMaster)
	{
		p_vBankLoadSum[0] = 0 ;
		for(int i = 1; i < p_nProcsNum; ++i )
		{
			p_vBankLoadSum[i] = p_vBankLoadSum[i-1] + p_vBankLoad[i-1] ;
		}
	}

	// Gatherv Delayed neutron Bank DAT
	int offset=0;

	if(p_bIsMaster)
	{
		offset = cCriticality.p_nDelayedNeuSrcCount;
		cCriticality.p_nDelayedNeuSrcCount = offset + nTotDelayedNeuBankCount;
	}


	if(p_bIsMaster)
	{
		if(cCriticality.p_vDelayedNeuSrc.size() < cCriticality.p_nDelayedNeuSrcCount)
		{
			cCriticality.p_vDelayedNeuSrc.resize(cCriticality.p_nDelayedNeuSrcCount); 
		}
	}

	MPI_Bcast(&offset, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&cCriticality.p_nDelayedNeuSrcCount,1,MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Gatherv(&cCriticality.p_vDelayedNeuBank[0],cCriticality.p_nDelayedNeuBankCount,MPI_BankDataType,
		&cCriticality.p_vDelayedNeuSrc[offset],&p_vBankLoad[0],&p_vBankLoadSum[0],MPI_BankDataType,0,MPI_COMM_WORLD);

	if(cCriticality.p_nDelayedNeuSrcCount>5*cCriticality.p_nNeuNumPerCyc) 
	{
		cCriticality.p_nDelayedNeuSrcCount=5*cCriticality.p_nNeuNumPerCyc;
		cCriticality.p_bIsRecordDelayedNeu=false;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	return;

}

# endif