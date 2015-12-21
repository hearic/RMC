
# include "Convergence.h"

void CDConvergence::ProcessFm(CDCriticality& cCriticality)
{
	 ////////////////// Gather fission matrix tallies at the final cycle /////////////////
# ifdef _IS_PARALLEL_
	if(p_bUseDiagFm && cCriticality.p_nCurrentCYCLE == cCriticality.p_nTotCycNum) 
	{
		if(OParallel.p_bIsMaster)
		{
			for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
			{
				for(int j = 0; j < p_vDiagFisMatrix[i].p_vFisMatrix.size(); ++j)
				{
					//// Gather fission matrix
					MPI_Reduce(MPI_IN_PLACE,&p_vDiagFisMatrix[i].p_vFisMatrix[j][0],
						p_vDiagFisMatrix[i].p_vFisMatrix[j].size(),MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD ); 
				}
                //// Gather fission-matrix start source
				MPI_Reduce(MPI_IN_PLACE,&p_vDiagFisMatrix[i].p_vStartSrc[0], p_vDiagFisMatrix[i].p_vStartSrc.size(), 
					MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD ); 
			}
		}
		else
		{
			for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
			{
				for(int j = 0; j < p_vDiagFisMatrix[i].p_vFisMatrix.size(); ++j)
				{
					MPI_Reduce(&p_vDiagFisMatrix[i].p_vFisMatrix[j][0],&p_vDiagFisMatrix[i].p_vFisMatrix[j][0],
						p_vDiagFisMatrix[i].p_vFisMatrix[j].size(),MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
				}
				MPI_Reduce(&p_vDiagFisMatrix[i].p_vStartSrc[0],&p_vDiagFisMatrix[i].p_vStartSrc[0], 
					p_vDiagFisMatrix[i].p_vStartSrc.size(),MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
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

	//////////////////////// Solve fission matrix and output results for diagnostic ///////////////////////
	int CurrentCYCLE = cCriticality.p_nCurrentCYCLE;
	int TotCycNum = cCriticality.p_nTotCycNum;
	
	if(p_bUseDiagFm && CurrentCYCLE == TotCycNum)
	{
		///////////////////////////// Solve fission matrix /////////////////////////
		for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
		{
			p_vDiagFisMatrix[i].CalcFmEigen();
			p_vDiagFisMatrix[i].CalcFmEntropy();
		}


		/////// Output Shannon entropy and relative entropy of fission matrix ///////
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n\n=================== Fission matrix diagnostic ===================\n");},
			Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Mesh        K0         DR       H(src)     H(eigv)    H(src/eigv)\n");},
			Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);

		for(int i = 0 ; i < p_vDiagFisMatrix.size(); ++i)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%d*%d*%d   %f   %f   %f   %f     %f\n",
				p_vDiagFisMatrix[i].p_OFmMesh.p_nMeshNum[0],p_vDiagFisMatrix[i].p_OFmMesh.p_nMeshNum[1],p_vDiagFisMatrix[i].p_OFmMesh.p_nMeshNum[2],
				p_vDiagFisMatrix[i].p_dEigVal,p_vDiagFisMatrix[i].p_dDR,p_vDiagFisMatrix[i].p_dFmSrcSe,p_vDiagFisMatrix[i].p_dFmVecSe,p_vDiagFisMatrix[i].p_dFmKld);},
				Output.p_fpFscFilePtr,CDOutput::_SCREEN_FILE,CDOutput::_NORMAL);
		}

		///////// Output detailed source distribution of of fission matrix //////////
		fprintf(Output.p_fpFscFilePtr,"\n================ Fission matrix source distribution  ================\n");
		for(int m = 0 ; m < p_vDiagFisMatrix.size(); ++m)
		{
			fprintf(Output.p_fpFscFilePtr,"\nMesh        K0         DR       H(src)     H(eigv)    H(src/eigv)\n");
			fprintf(Output.p_fpFscFilePtr,"%d*%d*%d   %f   %f   %f   %f     %f\n",
				p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[0],p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[1],p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[2],
				p_vDiagFisMatrix[m].p_dEigVal,p_vDiagFisMatrix[m].p_dDR,p_vDiagFisMatrix[m].p_dFmSrcSe,p_vDiagFisMatrix[m].p_dFmVecSe,p_vDiagFisMatrix[m].p_dFmKld);

			fprintf(Output.p_fpFscFilePtr,"i(x)     j(y)     k(z)         S0           S1         Diff\n");

			int x_num = p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[0];
			int y_num = p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[1];
			int z_num = p_vDiagFisMatrix[m].p_OFmMesh.p_nMeshNum[2];
			for(int k = 1 ; k <= z_num; ++k)
			{
				for(int j = 1 ; j <= y_num; ++j)
				{
					for(int i = 1 ; i <= x_num; ++i)
					{
						int n = (k-1) * x_num * y_num + (j-1) * x_num + i - 1 ;
						double f0 = p_vDiagFisMatrix[m].p_vStartSrc[n];
						double f1 = p_vDiagFisMatrix[m].p_vEigVec[n];
						double diff = f0 == 0? (f1 == 0? 0 : fabs(f1-f0)/f1) : fabs(f1-f0)/f0;
						fprintf(Output.p_fpFscFilePtr,"%-6d   %-6d   %-6d    %.4E     %.4E    %.4E\n",i,j,k,f0,f1,diff);
					}
				}
			}
		}
	}


	return;
}