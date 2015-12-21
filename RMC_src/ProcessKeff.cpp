# include "Criticality.h"
# include "Convergence.h"

void CDCriticality::ProcessKeff(CDConvergence &cConvergence)
{
	////////////  Reduce KeffCyc[i],FisBankCount,SuphisTotStartWeight for parallel////////////
# ifdef _IS_PARALLEL_
		MPI_Allreduce(MPI_IN_PLACE,p_dKeffWgtSum,3, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
		MPI_Allreduce(&p_nFisBankCount,&p_nTotFisBankNum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
		MPI_Allreduce(MPI_IN_PLACE,&cConvergence.p_dSuphisTotStartWeight,1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );		
# else
		p_nTotFisBankNum = p_nFisBankCount ;
# endif

	////////////  update fission source ////////////
	if( p_nTotFisBankNum < 5 )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Insufficient fission source to be sampled.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	////////////  process KeffCyc[i] ////////////
	if( cConvergence.p_nFscAcceMethod == 1 )
	{
		for(int i = 0 ; i < 3 ; ++i )
		{
			double Kw = p_dKeffWgtSum[i]/p_dTotStartWgt;
			p_dKeffCyc[i] = 1/(1/Kw+1/cConvergence.p_dWldtKe);
		}
	}
	else if( cConvergence.p_nFscAcceMethod == 2 )
	{
		for(int i = 0 ; i < 3 ; ++i )
		{
			p_dKeffCyc[i] = p_dKeffWgtSum[i]/cConvergence.p_dSuphisTotStartWeight;
		}
	}
	else
	{		
		for(int i = 0 ; i < 3 ; ++i )
		{
			p_dKeffCyc[i] = p_dKeffWgtSum[i]/p_dTotStartWgt;
		}
	}
	
	////////////  combine Keff ////////////
	p_dKeffFinal = p_dKeffCyc[0];
	CombineKeff();


	////////////  output Keff ////////////
	OutputKeff();
		
}