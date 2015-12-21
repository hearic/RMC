# include "Output.h"
# include "CalMode.h"

void CDOutput::OpenFilePtrs(std::map<string,bool> &BlockDefined,CDCalMode &cCalMode)
{
	char SourceFileName[FILE_NAME_LENGTH];
	char FscFileName[FILE_NAME_LENGTH];
	char MatFileName[FILE_NAME_LENGTH];
	char TallyFileName[FILE_NAME_LENGTH];
	char CycTallyFileName[FILE_NAME_LENGTH];
	char PlotFileName[FILE_NAME_LENGTH];
	char InnerProductFileName[FILE_NAME_LENGTH];

	///////////////// Only output when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return ;
	}
# endif

	///////////////////////// If it is point burnup, return  ////////////////////
	//if(CalMode.RmcCalcMode == CalMode.PointBurnMode)
	if(cCalMode.p_nRmcCalcMode == cCalMode.PointBurnMode)
	{
		return;
	}

	///////////////////////// Open all kinds of output files  ////////////////////
	if(BlockDefined["MATERIAL"] == true)
	{
		strcpy(MatFileName, p_chInputFileName);       
	    strcat(MatFileName,".material"); 
		p_fpMatFilePtr = fopen(MatFileName,"w");
		if (p_fpMatFilePtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen mat print file \"%s\" failed.\n",MatFileName);},CDOutput::_ERROR);
		}
	} 

	if(BlockDefined["TALLY"] == true || BlockDefined["BURNUP"] == true)
	{
		strcpy(TallyFileName, p_chInputFileName);       
	    strcat(TallyFileName,".Tally"); 
		p_fpTallyFilePtr = fopen(TallyFileName,"w");
		if (p_fpTallyFilePtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen Tally print file \"%s\" failed.\n",TallyFileName);},CDOutput::_ERROR);
		}

		if(p_bIsTallyPrintPerCyc)
		{
			strcpy(CycTallyFileName, p_chInputFileName);    
			strcat(CycTallyFileName,".CycTally");
			p_fpCycTallyFilePtr = fopen(CycTallyFileName,"w");
			if (p_fpCycTallyFilePtr == NULL)
			{
				OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen CycTally print file \"%s\" failed.\n",CycTallyFileName);},CDOutput::_ERROR);
			}
		}
	} 

	if(BlockDefined["CONVERGENCE"] == true)
	{
		strcpy(FscFileName, p_chInputFileName);       
		strcat(FscFileName,".fsc"); 
		p_fpFscFilePtr = fopen(FscFileName,"w");
		if (p_fpFscFilePtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen Convergence print file \"%s\" failed.\n",FscFileName);},CDOutput::_ERROR);
		}
	}

	if(p_bIsSrcPrint == true)
	{
		strcpy(SourceFileName, p_chInputFileName);       
	    strcat(SourceFileName,".source"); 
		p_fpSourceFilePtr = fopen(SourceFileName,"w");
		if (p_fpSourceFilePtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen Source print file \"%s\" failed.\n",SourceFileName);},CDOutput::_ERROR);
		}
	} 

	if(BlockDefined["PLOT"] == true)
	{
		strcpy(PlotFileName, p_chInputFileName);       
		strcat(PlotFileName,".plot"); 
		p_fpPlotFilePtr = fopen(PlotFileName,"w");
		if (p_fpPlotFilePtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen Plot print file \"%s\" failed.\n",p_fpPlotFilePtr);},CDOutput::_ERROR);
		}
	}

	if(cCalMode.p_nRmcCalcMode == cCalMode.QuasiStaticSMode || cCalMode.p_nRmcCalcMode == cCalMode.QuasiStaticDMode)
	{
		strcpy(InnerProductFileName, p_chInputFileName);       
		strcat(InnerProductFileName,".innerproduct"); 
		p_fpInnerProductPtr = fopen(InnerProductFileName,"w");
		if (p_fpInnerProductPtr == NULL)
		{
			OutputMessage([&](){sprintf(p_sPrintStr,"\nOpen InnerProduct print file \"%s\" failed.\n",p_fpInnerProductPtr);},CDOutput::_ERROR);
		}
	}

	return;
}