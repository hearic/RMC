# include "Tally.h"
# include "Criticality.h"
# include "Convergence.h"

void CDCriticality::ProcessBank(CDNeutronTransport &cNeutronTransport, CDConvergence &cConvergence)
{


	///////  update fission source and Renormalize total weight ///////
# ifdef _IS_PARALLEL_
		OParallel.ParallelBank(*this);
# else
		for (int i = 0;i < p_nFisBankCount; ++i)
		{
			p_vFissionSrc[i] = p_vFissionBank[i] ;
		}
		p_nCycleNeuNum = p_nFisBankCount;	
# endif

	cNeutronTransport.p_dStartWgt = 1.0*p_dTotStartWgt/p_nTotFisBankNum ;
	
	
	////////// fission source convergence acceleration and diagnostics  /////////////
	cConvergence.DiagAcceFsc(cNeutronTransport,*this);

}

void CDCriticality::ProcessBank_QuasiStatic(CDNeutronTransport &cNeutronTransport)
{
	///////  update fission source and Renormalize total weight ///////
# ifdef _IS_PARALLEL_
	OParallel.ParallelBank_QuasiStatic(*this);
# else
	for (int i = 0;i < p_nFisBankCount; ++i)
	{
		p_vFissionSrc[i] = p_vFissionBank[i] ;
	}

	int nExternalSrcNum = p_nNeuNumPerCyc - p_nFisBankCount;

	int rnposition = ORNG.RN_POSITION;
	ORNG.RN_POSITION_PRE=-1000;
	ORNG.RN_POSITION=0;
	ORNG.GetRandSeed();

	for(int i=0;i<nExternalSrcNum;i++)
	{
		int idx = ORNG.Rand()*p_nFixedSrcSize;
		if(idx==p_nFixedSrcSize) idx = p_nFixedSrcSize-1;
		p_vFissionSrc[p_nFisBankCount+i] = p_vFixedSrc[idx];
	}

	ORNG.RN_POSITION_PRE=-1000;
	ORNG.RN_POSITION=rnposition;

	p_nCycleNeuNum = p_nNeuNumPerCyc;	
# endif

	cNeutronTransport.p_dStartWgt = 1.0;

}

void CDCriticality::ProcessDelayedNeuBank()
{
	///////  update fission source and Renormalize total weight ///////
	if(p_nCurrentCYCLE>p_nInactCycNum && p_bIsRecordDelayedNeu)
	{
# ifdef _IS_PARALLEL_
		OParallel.ParallelDelayedNeuBank(*this);
# else
		int offset = p_nDelayedNeuSrcCount;
		p_nDelayedNeuSrcCount += p_nDelayedNeuBankCount;
		if(p_nDelayedNeuSrcCount>=p_vDelayedNeuSrc.size())
		{
			p_vDelayedNeuSrc.resize(int(1.2*p_nDelayedNeuSrcCount)+1);
		}
		for (int i = 0;i < p_nDelayedNeuBankCount; ++i)
		{
			p_vDelayedNeuSrc[offset+i] = p_vDelayedNeuBank[i] ;
		}
		if(p_nDelayedNeuSrcCount>5*p_nNeuNumPerCyc) 
		{
			p_nDelayedNeuSrcCount=5*p_nNeuNumPerCyc;
			p_bIsRecordDelayedNeu=false;
		}
# endif
	}
}


void CDCriticality::ProcessTimeAbsNeuBank()
{

	///////  update fission source and Renormalize total weight ///////
	if(p_nCurrentCYCLE>p_nInactCycNum && p_bIsRecordTimeAbsNeu)
	{
# ifdef _IS_PARALLEL_
		OParallel.ParallelTimeAbsNeuBank(*this);
# else
		int offset = p_nTimeAbsNeuSrcCount;
		p_nTimeAbsNeuSrcCount += p_nTimeAbsNeuBankCount;
		if(p_nTimeAbsNeuSrcCount>=p_vTimeAbsNeuSrc.size())
		{
			p_vTimeAbsNeuSrc.resize(int(1.2*p_nTimeAbsNeuSrcCount)+1);
		}
		for (int i = 0;i < p_nTimeAbsNeuBankCount; ++i)
		{
			p_vTimeAbsNeuSrc[offset+i] = p_vTimeAbsNeuBank[i] ;
		}

		if(p_nTimeAbsNeuSrcCount>5*p_nNeuNumPerCyc) 
		{
			p_nTimeAbsNeuSrcCount=5*p_nNeuNumPerCyc;
			p_bIsRecordTimeAbsNeu=false;
		}

# endif
	}

}


void CDCriticality::ProcessFixedSrc()
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	ORNG.RN_POSITION_PRE = -1000;
	ORNG.RN_POSITION = 0;
	ORNG.GetRandSeed();

	double xxx[13];
	double yyy[13];
	for(int i=0;i<13;i++) {xxx[i]=0;yyy[i]=(i+1)*2;}

	if(p_bIsQuasiStaticD)
	{
		//combine delayed neutron source
		p_nCombDelayedNeuSrcCount = p_nWriteSrcCount;
		p_vCombDelayedNeuSrc.resize(p_nCombDelayedNeuSrcCount);
		p_dCombDelayedNeuSrcStrength = p_dDelayedSrcStrength[0]+p_vDelayedSrcStrengthPrev[0];
		double dCurrDNeuFrac = p_dDelayedSrcStrength[0]/p_dCombDelayedNeuSrcStrength;
		for(int i=0;i<p_nCombDelayedNeuSrcCount;i++)
		{
			if(ORNG.Rand()<dCurrDNeuFrac && p_nDelayedNeuSrcCount>=1)
			{
				int idx = int(ORNG.Rand()*p_nDelayedNeuSrcCount);
				if(idx==p_nDelayedNeuSrcCount) idx = p_nDelayedNeuSrcCount-1;
				p_vCombDelayedNeuSrc[i] = p_vDelayedNeuSrc[idx];
			}
			else
			{
				int idx = int(ORNG.Rand()*p_nDelayedNeuSrcCountPrev);
				if(idx==p_nDelayedNeuSrcCountPrev) idx = p_nDelayedNeuSrcCountPrev-1;
				p_vCombDelayedNeuSrc[i] = p_vDelayedNeuSrcPrev[idx];
			}
		}

		//combine the fixed source
		double dDNeuFrac = p_dCombDelayedNeuSrcStrength/(p_dCombDelayedNeuSrcStrength + p_dTimeAbsSrcStrength);
		p_nFixedSrcSize = p_nWriteSrcCount;
		vector<CDFisBank>().swap(p_vFixedSrc);
		p_vFixedSrc.resize(p_nFixedSrcSize);

		for (int i = 0;i < p_nFixedSrcSize; ++i)
		{
			if(ORNG.Rand()<dDNeuFrac || p_nTimeAbsNeuSrcCount<1)
			{
				int idx = int(ORNG.Rand()*p_nCombDelayedNeuSrcCount);
				if(idx==p_nCombDelayedNeuSrcCount) idx = p_nCombDelayedNeuSrcCount-1;
				p_vFixedSrc[i] = p_vCombDelayedNeuSrc[idx];
			}
			else
			{
				int idx = int(ORNG.Rand()*p_nTimeAbsNeuSrcCount);
				if(idx==p_nTimeAbsNeuSrcCount) idx = p_nTimeAbsNeuSrcCount-1;
				p_vFixedSrc[i] = p_vTimeAbsNeuSrc[idx];
			}
		}

		/*for(int i=0;i<p_nDelayedNeuSrcCount;i++){
			int j;
			for(j=0;j<13;j++)
			{
				if(p_vDelayedNeuSrc[i].p_dPos[0]<yyy[j]) break;
			}
			xxx[j] += 1.0;}

		for(int i=0;i<13;i++) printf("%f\n",xxx[i]);

		for(int i=0;i<13;i++) xxx[i]=0;

		for(int i=0;i<p_nTimeAbsNeuSrcCount;i++){
			int j;
			for(j=0;j<13;j++)
			{
				if(p_vTimeAbsNeuSrc[i].p_dPos[0]<yyy[j]) break;
			}
			xxx[j] += 1.0;}

		for(int i=0;i<13;i++) printf("%f\n",xxx[i]);*/
	}
	else
	{
		p_nFixedSrcSize = p_nNeuNumPerCyc;

		if(p_nFixedSrcSize>p_vFixedSrc.size()) p_vFixedSrc.resize(p_nFixedSrcSize);

		double dDNeuFrac = p_dDelayedSrcStrength[0]/(p_dDelayedSrcStrength[0] + p_dTimeAbsSrcStrength);

		if( p_nDelayedNeuSrcCount==0 ) dDNeuFrac=0.0;
		
		if( p_nTimeAbsNeuSrcCount==0 ) dDNeuFrac=1.0;
		

		for (int i = 0;i < p_nFixedSrcSize; ++i)
		{
			if(ORNG.Rand()<dDNeuFrac)
			{
				int idx = int(ORNG.Rand()*p_nDelayedNeuSrcCount);
				if(idx==p_nDelayedNeuSrcCount) idx = p_nDelayedNeuSrcCount-1;
				p_vFixedSrc[i] = p_vDelayedNeuSrc[idx];
			}
			else
			{
				int idx = int(ORNG.Rand()*p_nTimeAbsNeuSrcCount);
				if(idx==p_nTimeAbsNeuSrcCount) idx = p_nTimeAbsNeuSrcCount-1;
				p_vFixedSrc[i] = p_vTimeAbsNeuSrc[idx];
			}
		}


		/*for(int i=0;i<p_nDelayedNeuSrcCount;i++){
		int j;
		for(j=0;j<13;j++)
		{
		if(p_vDelayedNeuSrc[i].p_dPos[0]<yyy[j]) break;
		}
		xxx[j] += 1.0;}

		for(int i=0;i<13;i++) printf("%f\n",xxx[i]);

		for(int i=0;i<13;i++) xxx[i]=0;

		for(int i=0;i<p_nTimeAbsNeuSrcCount;i++){
		int j;
		for(j=0;j<13;j++)
		{
		if(p_vTimeAbsNeuSrc[i].p_dPos[0]<yyy[j]) break;
		}
		xxx[j] += 1.0;}

		for(int i=0;i<13;i++) printf("%f\n",xxx[i]);*/
	}

}