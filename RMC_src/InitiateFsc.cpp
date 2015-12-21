# include "Convergence.h"


void CDConvergence::InitiateFsc(CDCriticality& cCriticality)
{
	///////////////// Write convergence information ////////////////////
	if(p_bUseFscDiag || p_bUseFscAcce)
	{
		char Diagnostic[][80]={"Shannon Entropy","Shannon Entropy and Fission Matrix"};
		char Accelerate[][80]={"None","Wielandt",  "Super History","Fission Matrix"};

		int DiagMethod = 0;
		if(p_bUseDiagFm)
		{
			DiagMethod = 1;
		}

		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Fission source convergence: \nDiagnostics=%s   Acceleration=%s\n",Diagnostic[DiagMethod],Accelerate[p_nFscAcceMethod]);},
			Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
	}


	/////////////////// initiate convergence diagnostic //////////////////////
	if(p_bUseFscDiag)
	{
		/////////////// initiate shannon entropy ////////////////
		p_OShanEntropy.Initiate(cCriticality.p_nTotCycNum + 1);


		/////////////// initiate fission matrix ////////////////
		if(p_bUseDiagFm)
		{
			for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
			{
				p_vDiagFisMatrix[i].InitFisMatrix();
			}
		}
		if(p_bUseDiagNpm)
		{
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				p_vNpMatrix[i].InitNpMatrix();
			}
		}
	}

	/////////////////// Initiate convergence acceleration //////////////////////
	//  Wielandt or superhistory method //
	if(p_nFscAcceMethod == 1 || p_nFscAcceMethod == 2)
	{
		int tot_acce_cycle = 0;


		if(p_nFscAcceMethod == 1)
		{
			cCriticality.p_nCycleNeuNum = int(cCriticality.p_nNeuNumPerCyc/p_dWldtFactor);
			tot_acce_cycle = p_vWldtFactorCycleBin[p_vWldtFactorCycleBin.size() - 1];
		}
		else if(p_nFscAcceMethod == 2)
		{
			cCriticality.p_nCycleNeuNum = cCriticality.p_nNeuNumPerCyc/p_nSuphisFactor;
			tot_acce_cycle = p_vSuphisFactorCycleBin[p_vSuphisFactorCycleBin.size() - 1];
		}
		cCriticality.p_dTotStartWgt = 1.0*cCriticality.p_nCycleNeuNum;


		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Total accelerating cycle is %d.\n",tot_acce_cycle);},Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

		Output.CheckInputParas(tot_acce_cycle <= cCriticality.p_nInactCycNum,
			[&](){sprintf(Output.p_sPrintStr,"total accelerating cycle(%d) succeed inactive cycle(%d).\n",
			tot_acce_cycle,cCriticality.p_nInactCycNum);},1);

		Output.CheckInputParas(p_dWldtFactor >= 1.0,
			[&](){sprintf(Output.p_sPrintStr,"Ke is less than Keff for Wielandt's acceleration.\n");},2);

		double Keff = cCriticality.p_dKeffFinal;
		p_dWldtKe = p_dWldtFactor/(p_dWldtFactor-1)*Keff;
		p_dWldtKw = 1/(1/Keff-1/p_dWldtKe);
		int Wielandt_bank_size = 10000;
		p_vWldtSourceBank.resize(Wielandt_bank_size);
		p_vSuphisSourceCycleBank.resize(Wielandt_bank_size);
		p_dSuphisTotStartWeight = 0;
	}


	//  Fission matrix method //
}