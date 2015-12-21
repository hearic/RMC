# include "Criticality.h"
# include "float.h"

void CDCriticality::ProcessNeuPopu()
{
# ifdef _IS_PARALLEL_
	MPI_Allreduce(MPI_IN_PLACE,&p_cNeuPopu.p_dScore,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
# endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	int nMc = p_nCurrentCYCLE-p_nInactCycNum;

	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		p_cNeuPopu.p_dCycVal = p_cNeuPopu.p_dScore/p_dTotStartWgt;

		p_cNeuPopu.p_dCycSum1 += p_cNeuPopu.p_dCycVal;

		p_cNeuPopu.p_dCycSum2 += p_cNeuPopu.p_dCycVal*p_cNeuPopu.p_dCycVal;

		p_cNeuPopu.p_dCycAve = p_cNeuPopu.p_dCycSum1/(nMc);

		p_cNeuPopu.p_dCycStd = sqrt(fabs(p_cNeuPopu.p_dCycSum2 - p_cNeuPopu.p_dCycSum1*p_cNeuPopu.p_dCycSum1/nMc))/nMc ;
	}


	//////////////////////////////////// output to file ////////////////////////////////////
	if( p_nCurrentCYCLE == p_nInactCycNum + 1 )
	{	
		fprintf(Output.p_fpInnerProductPtr,"================== cycle  cycleVal  cycleAve  cycleStd ==========================\n");
	}

	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		fprintf(Output.p_fpInnerProductPtr,"%-6d  NeuPopu(sec):%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cNeuPopu.p_dCycVal*1e-8,p_cNeuPopu.p_dCycAve*1e-8,p_cNeuPopu.p_dCycStd*1e-8); 
	}

	return;

}



void CDCriticality::ProcessFisNeuStrength()
{
# ifdef _IS_PARALLEL_
	for(int i=0;i<7;i++)
	{
		MPI_Allreduce(MPI_IN_PLACE,&p_cFisNeuStrength[i].p_dScore,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
		MPI_Allreduce(MPI_IN_PLACE,&p_cAveDelayedConstant[i].p_dScore,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
	}
	MPI_Allreduce(MPI_IN_PLACE,&p_cLeakNeuStrength.p_dScore,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
	MPI_Allreduce(MPI_IN_PLACE,&p_cAbsNeuStrength.p_dScore,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
# endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	int nMc = p_nCurrentCYCLE-p_nInactCycNum;

	if(p_nCurrentCYCLE > p_nInactCycNum)
	{
		for(int i=0;i<7;i++)
		{
			p_cFisNeuStrength[i].p_dCycVal = p_cFisNeuStrength[i].p_dScore/p_dTotStartWgt;

			p_cFisNeuStrength[i].p_dCycSum1 += p_cFisNeuStrength[i].p_dCycVal;

			p_cFisNeuStrength[i].p_dCycSum2 += p_cFisNeuStrength[i].p_dCycVal*p_cFisNeuStrength[i].p_dCycVal;

			p_cFisNeuStrength[i].p_dCycAve = p_cFisNeuStrength[i].p_dCycSum1/(nMc);

			p_cFisNeuStrength[i].p_dCycStd = sqrt(fabs(p_cFisNeuStrength[i].p_dCycSum2 - p_cFisNeuStrength[i].p_dCycSum1*p_cFisNeuStrength[i].p_dCycSum1/nMc))/nMc ;
		}

		for(int i=1;i<7;i++)
		{

			p_cAveDelayedConstant[i].p_dCycVal = p_cAveDelayedConstant[i].p_dScore/p_dTotStartWgt/p_cFisNeuStrength[i].p_dCycVal;

			if( /*_isnan(p_cAveDelayedConstant[i].p_dCycVal)*/p_cAveDelayedConstant[i].p_dCycVal!=p_cAveDelayedConstant[i].p_dCycVal) 
			{
				p_cAveDelayedConstant[i].p_dCycVal = p_cAveDelayedConstant[i].p_dCycAve;
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"delayed constant is a nan.\n");},CDOutput::_WARNING);
			}

			p_cAveDelayedConstant[i].p_dCycSum1 += p_cAveDelayedConstant[i].p_dCycVal;

			p_cAveDelayedConstant[i].p_dCycSum2 += p_cAveDelayedConstant[i].p_dCycVal*p_cAveDelayedConstant[i].p_dCycVal;

			p_cAveDelayedConstant[i].p_dCycAve = p_cAveDelayedConstant[i].p_dCycSum1/(nMc);

			p_cAveDelayedConstant[i].p_dCycStd = sqrt(fabs(p_cAveDelayedConstant[i].p_dCycSum2 - p_cAveDelayedConstant[i].p_dCycSum1*p_cAveDelayedConstant[i].p_dCycSum1/nMc))/nMc ;

			if(p_cAveDelayedConstant[i].p_dCycStd/p_cAveDelayedConstant[i].p_dCycAve < 1.0e4) p_cAveDelayedConstant[i].p_dCycStd=0;
		}

		double sum=0;
		for(int i=0; i<7;i++) sum += p_cFisNeuStrength[i].p_dCycVal;
		for(int i=0;i<7;i++)
		{
			p_cBeta[i].p_dCycVal = p_cFisNeuStrength[i].p_dCycVal/sum;

			p_cBeta[i].p_dCycSum1 += p_cBeta[i].p_dCycVal;

			p_cBeta[i].p_dCycSum2 += p_cBeta[i].p_dCycVal*p_cBeta[i].p_dCycVal;

			p_cBeta[i].p_dCycAve = p_cBeta[i].p_dCycSum1/(nMc);

			p_cBeta[i].p_dCycStd = sqrt(fabs(p_cBeta[i].p_dCycSum2 - p_cBeta[i].p_dCycSum1*p_cBeta[i].p_dCycSum1/nMc))/nMc ;
		}



		p_cFis.p_dCycVal = sum/p_cNeuPopu.p_dCycVal;
		p_cFis.p_dCycSum1 += p_cFis.p_dCycVal;
		p_cFis.p_dCycSum2 += p_cFis.p_dCycVal*p_cFis.p_dCycVal;
		p_cFis.p_dCycAve = p_cFis.p_dCycSum1/(nMc);
		p_cFis.p_dCycStd = sqrt(fabs(p_cFis.p_dCycSum2 - p_cFis.p_dCycSum1*p_cFis.p_dCycSum1/nMc))/nMc ;

		p_cAbs.p_dCycVal = p_cAbsNeuStrength.p_dScore/p_dTotStartWgt/p_cNeuPopu.p_dCycVal;
		p_cAbs.p_dCycSum1 += p_cAbs.p_dCycVal;
		p_cAbs.p_dCycSum2 += p_cAbs.p_dCycVal*p_cAbs.p_dCycVal;
		p_cAbs.p_dCycAve = p_cAbs.p_dCycSum1/(nMc);
		p_cAbs.p_dCycStd = sqrt(fabs(p_cAbs.p_dCycSum2 - p_cAbs.p_dCycSum1*p_cAbs.p_dCycSum1/nMc))/nMc ;

		p_cLeak.p_dCycVal = p_cLeakNeuStrength.p_dScore/p_dTotStartWgt/p_cNeuPopu.p_dCycVal;
		p_cLeak.p_dCycSum1 += p_cLeak.p_dCycVal;
		p_cLeak.p_dCycSum2 += p_cLeak.p_dCycVal*p_cLeak.p_dCycVal;
		p_cLeak.p_dCycAve = p_cLeak.p_dCycSum1/(nMc);
		p_cLeak.p_dCycStd = sqrt(fabs(p_cLeak.p_dCycSum2 - p_cLeak.p_dCycSum1*p_cLeak.p_dCycSum1/nMc))/nMc ;


		p_cRho.p_dCycVal = (sum-p_cAbsNeuStrength.p_dScore/p_dTotStartWgt-p_cLeakNeuStrength.p_dScore/p_dTotStartWgt)/sum;
		p_cRho.p_dCycSum1 += p_cRho.p_dCycVal;
		p_cRho.p_dCycSum2 += p_cRho.p_dCycVal*p_cRho.p_dCycVal;
		p_cRho.p_dCycAve = p_cRho.p_dCycSum1/(nMc);
		p_cRho.p_dCycStd = sqrt(fabs(p_cRho.p_dCycSum2 - p_cRho.p_dCycSum1*p_cRho.p_dCycSum1/nMc))/nMc ;


		p_cGenTime.p_dCycVal = p_cNeuPopu.p_dCycVal/sum;
		p_cGenTime.p_dCycSum1 += p_cGenTime.p_dCycVal;
		p_cGenTime.p_dCycSum2 += p_cGenTime.p_dCycVal*p_cGenTime.p_dCycVal;
		p_cGenTime.p_dCycAve = p_cGenTime.p_dCycSum1/(nMc);
		p_cGenTime.p_dCycStd = sqrt(fabs(p_cGenTime.p_dCycSum2 - p_cGenTime.p_dCycSum1*p_cGenTime.p_dCycSum1/nMc))/nMc ;

		for(int i=1; i<7;i++)
		{
			fprintf(Output.p_fpInnerProductPtr,"%-6d  Lambda %d (1/sec):%6.4E  %6.4E  %6.4E\n",p_nCurrentCYCLE,i,p_cAveDelayedConstant[i].p_dCycVal*1e+8,
				p_cAveDelayedConstant[i].p_dCycAve*1e8, p_cAveDelayedConstant[i].p_dCycStd*1e8);
		}


		for(int i=0; i<7;i++)
		{
			fprintf(Output.p_fpInnerProductPtr,"%-6d  Beta %d:%6.4E  %6.4E  %6.4E\n",
				p_nCurrentCYCLE, i,p_cBeta[i].p_dCycVal, p_cBeta[i].p_dCycAve, p_cBeta[i].p_dCycStd);
		}

		fprintf(Output.p_fpInnerProductPtr,"%-6d  Fis_rho:%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cFis.p_dCycVal, p_cFis.p_dCycAve, p_cFis.p_dCycStd);

		fprintf(Output.p_fpInnerProductPtr,"%-6d  Abs_rho:%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cAbs.p_dCycVal, p_cAbs.p_dCycAve, p_cAbs.p_dCycStd);

		fprintf(Output.p_fpInnerProductPtr,"%-6d  Leak_rho:%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cLeak.p_dCycVal, p_cLeak.p_dCycAve, p_cLeak.p_dCycStd);

		fprintf(Output.p_fpInnerProductPtr,"%-6d  Rho_rho:%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE, (p_cFis.p_dCycAve-p_cAbs.p_dCycAve-p_cLeak.p_dCycAve)/p_cFis.p_dCycAve, 0.0,0.0);


		fprintf(Output.p_fpInnerProductPtr,"%-6d  Reactivity:%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cRho.p_dCycVal, p_cRho.p_dCycAve, p_cRho.p_dCycStd);

		fprintf(Output.p_fpInnerProductPtr,"%-6d  GenTime(sec):%6.4E  %6.4E  %6.4E\n",
			p_nCurrentCYCLE,p_cGenTime.p_dCycVal*1e-8, p_cGenTime.p_dCycAve*1e-8, p_cGenTime.p_dCycStd*1e-8);
	}

}


void CDCriticality::ProcessTimeAbsNeuStrength()
{
# ifdef _IS_PARALLEL_
	MPI_Allreduce(MPI_IN_PLACE,&p_dTimeAbsSrcStrength,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
# endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	fprintf(Output.p_fpInnerProductPtr,"\n\n");
	fprintf(Output.p_fpInnerProductPtr,"============= time absorbed neutron strength ==============\n");
	p_dTimeAbsSrcStrength = p_dTimeAbsSrcStrength/p_dTotStartWgt/p_nActCycNum/p_cNeuPopu.p_dCycAve*p_dFluxAmptitude;
	fprintf(Output.p_fpInnerProductPtr,"practical strength:%6.4E\n",p_dTimeAbsSrcStrength);


	fprintf(Output.p_fpInnerProductPtr,"\n\n");
	fprintf(Output.p_fpInnerProductPtr,"============= time absorbed neutron population ==============\n");
	fprintf(Output.p_fpInnerProductPtr,"time absorbed neutron population per %6.4E src neutrons:%d\n",p_dTotStartWgt*p_nActCycNum,p_nTimeAbsNeuSrcCount);

}


void CDCriticality::ProcessPower()
{
# ifdef _IS_PARALLEL_
	MPI_Allreduce(MPI_IN_PLACE,&p_dPower,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
# endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	fprintf(Output.p_fpInnerProductPtr,"\n\n");
	fprintf(Output.p_fpInnerProductPtr,"============= Power ==============\n");
	p_dPower = p_dPower/p_dTotStartWgt/p_nActCycNum/p_cNeuPopu.p_dCycAve*p_dFluxAmptitude;
	fprintf(Output.p_fpInnerProductPtr,"Power:%6.4E\n",p_dPower);


}


void CDCriticality::ProcessDelayedNeuStrength()
{
# ifdef _IS_PARALLEL_
	MPI_Allreduce(MPI_IN_PLACE,&p_dDelayedSrcStrength,7, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
# endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif

	fprintf(Output.p_fpInnerProductPtr,"\n\n");
	fprintf(Output.p_fpInnerProductPtr,"============= delayed neutron strength ==============\n");


	for(int i=0; i<7;i++) p_dDelayedSrcStrength[i] = p_dDelayedSrcStrength[i]/p_dTotStartWgt/(p_nActCycNum)/p_cNeuPopu.p_dCycAve*p_dFluxAmptitude;
	for(int i=0; i<7;i++)
	{
		fprintf(Output.p_fpInnerProductPtr,"group:%d,strength_curr:%6.4E, strength_prev:%6.4E\n",i,p_dDelayedSrcStrength[i],p_vDelayedSrcStrengthPrev[i]);
	}


	fprintf(Output.p_fpInnerProductPtr,"\n\n");
	fprintf(Output.p_fpInnerProductPtr,"============= delayed neutron population ==============\n");
	fprintf(Output.p_fpInnerProductPtr,"delayed neutrons per %6.4E src neutrons:%d\n",p_dTotStartWgt*p_nActCycNum,p_nDelayedNeuSrcCount);

}
