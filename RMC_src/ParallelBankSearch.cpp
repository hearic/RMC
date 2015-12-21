# include "Criticality.h"
# include "ParallelClass.h"

# ifdef _IS_PARALLEL_

void CDParallel::ParallelBankSearch(int &SendId, int &RecvId)
{
//// the MyId processor will send banks to the SendId processor
//// the MyId processor will receive banks from the RecvId processor

	int nMin, nMax , nMid, nVal;
	// search Sending location //
	nMin = 0 ;
	nMax = p_nProcsNum;
	nVal = p_vBankLoadSum[p_nMyId];
	if(nVal<=p_vSourceLoadSum[nMin])
	{
		SendId = nMin;
	}				 
	else if(nVal>=p_vSourceLoadSum[nMax])
	{
		SendId = nMax-1;	
	}
	else
	{
		while(nMax-nMin>1)
		{
			nMid=(nMin+nMax)/2;
			if(nVal>=p_vSourceLoadSum[nMid])
			{
				nMin=nMid;
			}
			else{nMax=nMid;}
		}
		SendId = nMin;
	}

	// search Receiving location //
    nMin = 0 ;
	nMax = p_nProcsNum;
	nVal = p_vSourceLoadSum[p_nMyId];
	if(nVal<=p_vBankLoadSum[nMin])
	{
		RecvId = nMin;
	}				 
	else if(nVal>=p_vBankLoadSum[nMax])
	{
		RecvId = nMax-1;	
	}
	else
	{
		while(nMax-nMin>1)
		{
			nMid=(nMin+nMax)/2;
			if(nVal>=p_vBankLoadSum[nMid])
			{
				nMin=nMid;
			}
			else{nMax=nMid;}
		}
		RecvId = nMin;
	}

	return;
}

# endif