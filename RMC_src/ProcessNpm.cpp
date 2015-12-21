
# include "Convergence.h"

void CDConvergence::ProcessNpm(CDCriticality& cCriticality)
{
	 ////////////////// Gather noise propagation matrix tallies at the final cycle /////////////////
# ifdef _IS_PARALLEL_
	if(p_bUseDiagNpm && cCriticality.p_nCurrentCYCLE > cCriticality.p_nInactCycNum) 
	{
		if(OParallel.p_bIsMaster)
		{
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				MPI_Reduce(MPI_IN_PLACE,&p_vNpMatrix[i].p_vS1[0], p_vNpMatrix[i].p_vS1.size(), 
					MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD ); //// Gather fission-matrix start source
			}
		}
		else
		{
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				MPI_Reduce(&p_vNpMatrix[i].p_vS1[0],&p_vNpMatrix[i].p_vS1[0], p_vNpMatrix[i].p_vS1.size(), 
					MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

#endif

# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif


	////////////////////// Solve noise propagation matrix and output results for diagnostic ///////////////////
	if(p_bUseDiagNpm && cCriticality.p_nCurrentCYCLE > cCriticality.p_nInactCycNum) 
	{
		////////////////// solve noise propagation matrix ///////////////
		if(p_bUseNpmOnFly)
		{
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				p_vNpMatrix[i].AccumTallies(cCriticality);
				if(p_bUseNpmOnFly)
				{
					p_vNpMatrix[i].CalcNpmEigen(cCriticality);
				}
				//////// Output results on the fly //////////
				printf("     DR %-2d:%f\n",i+1,p_vNpMatrix[i].p_dEigVal);
				fprintf(Output.p_fpFscFilePtr,"\n      DR %-2d %.6E",i+1,p_vNpMatrix[i].p_dEigVal);
			}
		}
		if(!p_bUseNpmOnFly && cCriticality.p_nCurrentCYCLE == cCriticality.p_nTotCycNum)
		{	
			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				p_vNpMatrix[i].CalcNpmEigen(cCriticality);
			}
		}


		////////// Output final results of noise propagation matrix //////////
		if(cCriticality.p_nCurrentCYCLE == cCriticality.p_nTotCycNum)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n\n============= DR calculation with noise propagation matrix =============\n");},Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Mesh        DR\n");},Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

			for(int i = 0 ; i < p_vNpMatrix.size(); ++i)
			{
				
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%d*%d*%d   %f\n",
					p_vNpMatrix[i].p_ONpMesh.p_nMeshNum[0],p_vNpMatrix[i].p_ONpMesh.p_nMeshNum[1],p_vNpMatrix[i].p_ONpMesh.p_nMeshNum[2],p_vNpMatrix[i].p_dEigVal);},
					Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);
			}
		}
	}

	return;
}