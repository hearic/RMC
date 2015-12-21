# include "Burnup.h"

void CDBurnup::OutputTotNucDen()
{

	///////////////////////////// sumup total isotope density  //////////////////////////
	int nTotBurnNucNum = Depth.NucNum ;

	vector<double> vTotNucDen(nTotBurnNucNum + 1);  

	for(int j = 1; j <= nTotBurnNucNum ; ++j)
	{
		vTotNucDen[j] = 0;
		for(int i = 0; i < p_vCellBurnNucDen.size(); ++i)  
		{
			vTotNucDen[j] = vTotNucDen[j] + p_vCellBurnNucDen[i][j];
		}
	}
	
	///////////////////////// MPI reduce of total isotope density /////////////////////////
# ifdef _IS_PARALLEL_
	///////// Reduce density of all processors
	//printf("myid = %d,  size = %d\n",Parallel.MyId,tot_nuc_den.size());
	if(OParallel.p_bIsParallelBurn)
	{
		if(OParallel.p_bIsMaster)
		{
			MPI_Reduce(MPI_IN_PLACE,&vTotNucDen[1], nTotBurnNucNum, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}
		else
		{				
			MPI_Reduce(&vTotNucDen[1],&vTotNucDen[1], nTotBurnNucNum, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	if(!OParallel.p_bIsMaster)
	{
		return;
	}

# endif
		

	/////////////////////////// output isotope density of indicaed cells /////////////////////////
	int nBurnStep = p_nBurnStep;
	/////////// Open output file //////////
	char chOutputFileDenTot[FILE_NAME_LENGTH];
	FILE *fpOutDenTot;
	strcpy(chOutputFileDenTot,Output.p_chInputFileName);
	strcat(chOutputFileDenTot,".burn.den_tot");

	if(nBurnStep == 0)  // Beginning step
	{
		fpOutDenTot = fopen(chOutputFileDenTot,"w");	  // Note: ios = "w"
	}
	else
	{
		fpOutDenTot = fopen(chOutputFileDenTot,"a");	  // Note: ios = "a"
	}

    /////  output total burnup & time oclock /////
	char chOutputClock[64];
	time_t tLocalTime = time(0);     //get system time at Beginning(s)
	strftime(chOutputClock, sizeof(chOutputClock), "%Y/%m/%d %X %A",localtime(&tLocalTime) ); 	

	if(nBurnStep == 0)
	{
		fprintf(fpOutDenTot,"********  Total isotope densities in the beginning ********   %s\n",chOutputClock);
	}
	else
	{
		fprintf(fpOutDenTot,"********  Total isotope densities at the end of step %d  ********   %s\n",nBurnStep,chOutputClock);
	}
	fprintf(fpOutDenTot,"Total step time(Day): %12.6E , Total Burnup(MWD/KgHM): %12.6E\n",
		p_vStepCmltvBurnTime[nBurnStep],p_vStepCmltvBurnup[nBurnStep]);

	
	/////////// output Densities //////////
	fprintf(fpOutDenTot,"Isotope     Name      Density(atom/barn/cm)\n");
	for(int j = 1;j <= Depth.NucNum;j++)
	{
		int nNucId = Depth.GetNucId(j);
		fprintf(fpOutDenTot,"%-10d  %-6s",nNucId,Depth.GetNucName(nNucId).c_str());  // composition
		fprintf(fpOutDenTot,"    %-18.6E\n",vTotNucDen[j]);

	}
	fprintf(fpOutDenTot,"\n");
	fclose(fpOutDenTot);

	return;
}