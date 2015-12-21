# include "Input.h"

void CDInput::CheckInpBlock(CDCalMode &cCalMode,CDGeometry &cGeometry,
	CDMaterial &cMaterial,CDCriticality &cCriticality, CDFixedSource& cFixedSource,
	bool &bIsMultiGroup,bool &bIsWithCsTally,bool &bIsWithBurnup)
{

	if(cCalMode.p_nRmcCalcMode == cCalMode.PointBurnMode)
	{
		return;
	}

	/////////////////////// check necessary blocks /////////////////////
	vector<string> vBlockName;
	if(p_nInpFileFormat == 0)
	{
		vBlockName.push_back("GEOMETRY");;
	}
	else
	{
		vBlockName.push_back("UNIVERSE");
	}
	vBlockName.push_back("SURFACE");
	vBlockName.push_back("MATERIAL");
	for(int i = 0 ; i < vBlockName.size(); i++)
	{
		//auto BlockKeyWord = vBlockName[i];
	   string sBlockKeyWord = vBlockName[i];

		Output.CheckInputParas(BlockDefined[sBlockKeyWord],
			[&](){sprintf(Output.p_sPrintStr,"%s block is not defined.\n",sBlockKeyWord.c_str());},2);
	}


	///////////////////////// check conflicts //////////////////////////
//	if(AceData.IsMultiGroup && Tally.IsWithCsTally)
	if(bIsMultiGroup && bIsWithCsTally)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Multigroup calculation conflicts with CsTally.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	//if(AceData.IsMultiGroup && Burnup.IsWithBurnup)
	if(bIsMultiGroup && bIsWithBurnup)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Multigroup calculation conflicts with Burnup.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	/////////////////////////   check geometry /////////////////////////

	cGeometry.CheckGeometry(cMaterial);

	/////////////////////////   check material /////////////////////////

    cMaterial.CheckUsedMat(cGeometry);

	if(cCalMode.p_nRmcCalcMode==cCalMode.CriticalMode)
	{
		/////////////////////////   check Criticality /////////////////////////
		cCriticality.CheckCriticality();
	}
	else if(cCalMode.p_nRmcCalcMode==cCalMode.FixedSourceMode)
	{
		/////////////////////////   check Fixed source /////////////////////////
		cFixedSource.CheckFixedSource();
	}

	///////////////////////// open output files //////////////////////////
	Output.OpenFilePtrs(BlockDefined,cCalMode);

	return;
}