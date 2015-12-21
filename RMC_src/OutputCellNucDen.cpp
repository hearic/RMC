#include "Burnup.h"

void CDBurnup::OutputCellNucDen(CDMaterial &CMaterial,CDTally &CTally,vector<CDNuclide>& vNuclides)
{	
	///////////////////////// output burnup information to .burn  /////////////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	int nBurnStep = p_nBurnStep;

	/////////////////////////// output isotope density of indicaed cells /////////////////////////
	int nOutputCellNum = int(p_vCellIndexForOutput.size());
	if(nOutputCellNum == 0)
	{
		return;
	}
	/////////// Open output file //////////
	char chOutputFileNuc[200];
	FILE *fpOutputNuc;
	strcpy(chOutputFileNuc,Output.p_chInputFileName);
	strcat(chOutputFileNuc,".burn.den");

	if(nBurnStep == 0)  // Beginning step
	{
		fpOutputNuc = fopen(chOutputFileNuc,"w");	  // Note: ios = "w"
	}
	else
	{
		fpOutputNuc = fopen(chOutputFileNuc,"a");	  // Note: ios = "a"
	}

     /////  output total burnup & time oclock /////
	char chOutputClock[64];
	time_t tLocalTime = time(0);     //get system time at Beginning(s)
	strftime(chOutputClock, sizeof(chOutputClock), "%Y/%m/%d %X %A",localtime(&tLocalTime) ); 

	if(nBurnStep == 0)
	{
		fprintf(fpOutputNuc,"********  Cell nuclide densities in the beginning ********   %s\n",chOutputClock);
	}
	else
	{
		fprintf(fpOutputNuc,"\n\n********  Cell nuclide at the end of step %d  ********   %s\n",nBurnStep,chOutputClock);
	}
	fprintf(fpOutputNuc,"Total step time(Day): %12.6E , Total Burnup(MWD/KgHM): %12.6E\n",
		p_vStepCmltvBurnTime[nBurnStep],p_vStepCmltvBurnup[nBurnStep]);

	
	////// output cell nuclide density ////////////
	for(int i = 0 ; i < nOutputCellNum; ++i)
	{
		int nTallyPos = p_nBurnTallyStartPos + p_vCellIndexForOutput[i] ;
		int nMat = CTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		
		fprintf(fpOutputNuc,"cell = %-s  mat = %d\n",
			Output.PrintCellVec(CTally.p_vOneGroupCsTally[nTallyPos].p_vCellVecU),CMaterial.p_OMatSetIndex.GetIndexU(nMat));

		fprintf(fpOutputNuc,"           Nuclide     Mass Density     Atom Density\n") ;
        fprintf(fpOutputNuc,"             Sum       %.5E      %.5E\n",
			CMaterial.GetMatGramDen(nMat),CMaterial.GetMatAtomDen(nMat)); 
		
		
		for(int j = 0; j < CMaterial.GetMatTotNucNum(nMat); j++)
		{
			int nNucId = atoi(CMaterial.GetMatNucID(nMat,j).p_chIdStr);

			if(nNucId % 1000 == 0 ) //// skip natural isotopes
			{
				continue;
			}
			if( nNucId % 1000 >= 400 )  //2012-01-17      excited-state nuclide
			{ 
				nNucId = (nNucId - 400)*10 + 1 ;  
			}
			else
			{
				nNucId = nNucId*10;
			}

			fprintf(fpOutputNuc,"           %-10d  %.5E      %.5E\n",
				nNucId,CMaterial.GetMatNucGramDen(nMat,j,vNuclides),CMaterial.GetMatNucAtomDen(nMat,j)); 
		}

	}
		

	fclose(fpOutputNuc);

	return;
}