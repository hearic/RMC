# include "Convergence.h"

void CDConvergence::AcceFsc(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality)
{
	//////////////////////////////// Accelerate FSC by AWM or ASM  ///////////////////////////////////////
	if(p_nFscAcceMethod == 1)// wielandt acceleration method
	{
		int i;
		for(i = 0 ; i < p_vWldtFactorList.size();++i)
		{
			if(cCriticality.p_nCurrentCYCLE == p_vWldtFactorCycleBin[i]){break;}
		}
		if(i == int(p_vWldtFactorList.size()) - 1)
		{	
			p_dWldtFactor=1.0;
			cCriticality.p_dTotStartWgt = 1.0*cCriticality.p_nNeuNumPerCyc;             // set TotStartWgt to original case
			p_nFscAcceMethod = -1;
		}
		else if(i < p_vWldtFactorList.size() - 1)
		{
			p_dWldtFactor=p_vWldtFactorList[i+1];
			cCriticality.p_dTotStartWgt = 1.0*cCriticality.p_nNeuNumPerCyc/p_dWldtFactor;   // set TotStartWgt by WldtFactor
		}
	}
	else if(p_nFscAcceMethod == 2)// super-history acceleration method
	{
		int i ;
		for(i = 0 ; i < p_vSuphisFactorList.size();i++)
		{
			if(cCriticality.p_nCurrentCYCLE == p_vSuphisFactorCycleBin[i]){break;}
		}
		if(i == int(p_vSuphisFactorList.size()) - 1)
		{			
			p_nSuphisFactor = 1;
			cCriticality.p_dTotStartWgt = 1.0*cCriticality.p_nNeuNumPerCyc;               // set TotStartWgt to original case
			p_nFscAcceMethod = -1;
		}
		else if(i < p_vSuphisFactorList.size() - 1)
		{
			p_nSuphisFactor = p_vSuphisFactorList[i+1];
			cCriticality.p_dTotStartWgt = 1.0*cCriticality.p_nNeuNumPerCyc/p_nSuphisFactor;  // set TotStartWgt by SuphisFactor
		}	
	}

	cNeutronTransport.p_dStartWgt = 1.0*cCriticality.p_dTotStartWgt/cCriticality.p_nTotFisBankNum;


# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif
	//////////////////////////////// Output AWM or ASM  parameters ///////////////////////////////////////
	if(p_nFscAcceMethod == 1)
	{
		printf("       AWM: factor=%.2f fisson=%d\n",p_dWldtFactor,cCriticality.p_nTotFisBankNum);
		fprintf(Output.p_fpFscFilePtr,"\n       AWM: factor=%.2f fisson=%d",p_dWldtFactor,cCriticality.p_nTotFisBankNum);
	}
	else if(p_nFscAcceMethod == 2)
	{
		printf("       ASM: factor=%d fisson=%d\n",p_nSuphisFactor,cCriticality.p_nTotFisBankNum);
		fprintf(Output.p_fpFscFilePtr,"\n       ASM: factor=%d fisson=%d",p_nSuphisFactor,cCriticality.p_nTotFisBankNum);
	}


	return;

}