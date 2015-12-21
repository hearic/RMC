# include "Criticality.h"
# include "FixedSource.h"

void CDCriticality::InitiateTrspt(CDNeutronTransport &cNeutronTransport)
{

	/////////////////////////////////////////////////////
	# ifdef _IS_PARALLEL_
	if(OParallel.p_bUseFastBank)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Peer-to-peer fission bank gathering is used.\n");},
			Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
	}
	# endif

	/////////////////////////////////////
	//if(!UseProbTableBool)
	//{
	//	printf("Probability table for unresolved resonance is not used!\n");
	//	fprintf(Output.OutputFilePtr,"Probability table for unresolved resonance is not used!\n");
	//}


	//////////// variable for tracking ////////////
	p_nCycleNeuNum = p_nNeuNumPerCyc;
	p_dTotStartWgt = 1.0*p_nCycleNeuNum;

	p_nFisBankCount = 0;
	p_nFisSrcCount = 0;
	p_nDelayedNeuBankCount = 0;
	p_nTimeAbsNeuBankCount = 0;
	p_nDelayedNeuSrcCount = 0;
	p_nTimeAbsNeuSrcCount = 0;    
	p_dTimeAbsSrcStrength = 0;
	for(int i=0;i<7;i++) 
	{
		p_dDelayedSrcStrength[i]=0;
		p_vDelayedSrcStrengthPrev[i]=0;
		p_vAveDelayedConstantPrev[i]=0;
	}

	p_bIsRecordDelayedNeu=true;
	p_bIsRecordTimeAbsNeu=true;
		
	p_llTotCollisionCount = 0;
	cNeutronTransport.p_nMissParticleCount = 0;

	for(int i = 0; i < 3; ++i)
	{
		p_dKeffWgtSum[i] = 0;
		p_dKeffSum[i] = 0;
		for(int j = 0;j < 3; ++j)
		{
			p_dKeffCrosSum[i][j] = 0;
		}
	}

	//////////// variable for bank ////////////
	int nBankSize = 0;

# ifdef _IS_PARALLEL_
	if(OParallel.p_bUseFastBank)
	{
		nBankSize = int(1.2*p_nNeuNumPerCyc/OParallel.p_nProcsNum);
	}
	else
	{
		nBankSize = int(1.2*p_nNeuNumPerCyc);
	}
#else
	nBankSize = int(1.2*p_nNeuNumPerCyc);
# endif

	p_vFissionSrc.resize(nBankSize);
	p_vFissionBank.resize(nBankSize);

	p_vPKLambda.resize(7);
	p_vPKBeta.resize(7);

	p_dPower=0;
	
	return;
}


void CDFixedSource::InitiateTrspt(CDNeutronTransport &cNeutronTransport)
{
	/////////////////////////////////////////////////////
# ifdef _IS_PARALLEL_
	if(OParallel.p_bUseFastBank)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Peer-to-peer fission bank gathering is used.\n");},
			Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
	}
# endif

	/////////////////////////////////////
	//if(!UseProbTableBool)
	//{
	//	printf("Probability table for unresolved resonance is not used!\n");
	//	fprintf(Output.OutputFilePtr,"Probability table for unresolved resonance is not used!\n");
	//}


	//////////// variable for tracking ////////////
	p_dTotStartWgt = 1.0*p_nNeuNum;

	p_nFixedSrcBankCount = 0;
	p_nFixedSrcCount = 0;

	p_llTotCollisionCount = 0;
	cNeutronTransport.p_nMissParticleCount = 0;

	//////////// variable for bank ////////////
	int bank_size = 0;

# ifdef _IS_PARALLEL_
	if(OParallel.p_bUseFastBank)
	{
		bank_size = int(1.2*p_nNeuNum/OParallel.p_nProcsNum);
	}
	else
	{
		bank_size = int(1.2*p_nNeuNum);
	}
#else
	bank_size = int(1.2*p_nNeuNum);
# endif

	p_vFixedSrc.resize(bank_size);
	p_vFixedSrcBank.resize(bank_size);

	return;
}