
# include "Convergence.h"

void CDConvergence::ProcessSe(CDCriticality& cCriticality)
{
	if(!p_bUseFscDiag)
	{
		return;
	}
	
	//////////////////////////  Gather CycMeshSrc per cycle ////////////////////////////
# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsMaster)
	{
		MPI_Reduce(MPI_IN_PLACE,&p_OShanEntropy.p_vCycMeshSrc[0], p_OShanEntropy.p_vCycMeshSrc.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
	}
	else
	{				
		MPI_Reduce(&p_OShanEntropy.p_vCycMeshSrc[0],&p_OShanEntropy.p_vCycMeshSrc[0], p_OShanEntropy.p_vCycMeshSrc.size(), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
	}
# endif
	

	# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	///////////////////////////////// Calculate and Output Shannon entropy per cycle //////////////////////
	int CurrentCYCLE = cCriticality.p_nCurrentCYCLE;
	//int InactCycNum = C_Criticality.InactCycNum;
	/////////////////////////////// Calculate Shannon entropy per cycle /////////////////////
	p_OShanEntropy.CalcCycSe(CurrentCYCLE);


	///////////////////////////////// Output Shannon entropy per cycle //////////////////////
	/////////////////////// print Shannon Entropy /////////////////////
	if( CurrentCYCLE == 1 )
	{
		fprintf(Output.p_fpFscFilePtr,"=============== shannon entropy with cycle ==============\n");
		fprintf(Output.p_fpFscFilePtr,"cycle   shannon entropy");
	}	

	printf("        SE:%-f\n",p_OShanEntropy.p_vCycSrcSE[CurrentCYCLE]);
	fprintf(Output.p_fpFscFilePtr,"\n%-6d  SE: %-.6E",CurrentCYCLE,p_OShanEntropy.p_vCycSrcSE[CurrentCYCLE]); 

	if(p_bIsFsdPrint)
	{
		fprintf(Output.p_fpFscFilePtr,"  FSD: "); 
		for(int i = 0 ; i < p_OShanEntropy.p_OSeMesh.GetTotMeshNum() ; i++)
		{
			fprintf(Output.p_fpFscFilePtr,"%-.6E  ",p_OShanEntropy.p_vCycMeshSrc[i]); 
		}
	}
	return;
}