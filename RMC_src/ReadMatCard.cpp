# include "Input.h"

void CDInput::ReadMatCard(bool &bIsBlockEnd,CDMaterial &cMaterial)
{

	bIsBlockEnd = false;

	/////////////////////// Read mat index defined by user //////////////////////////
	int nMat_index_u = -1;
	fscanf(p_fpInputFilePtr,"%d",&nMat_index_u);
	int nMat_index = cMaterial.p_OMatSetIndex.AddUserIndex(nMat_index_u);
	cMaterial.p_vMatSet.resize(nMat_index + 1);

	///////////////////////  Copy material £¨mat i = j£©   //////////////////////////
	char nextchar;
	CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
	if(nextchar == '=')
	{
		CDFileIO::SkipSpecChar(p_fpInputFilePtr,'=');
		int nMat_index_u2;
		if( fscanf(p_fpInputFilePtr,"%d",&nMat_index_u2) <= 0 )
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown duplicate mat index for material %d.\n", nMat_index_u);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
		cMaterial.p_vMatSet[nMat_index] = cMaterial.p_vMatSet[cMaterial.p_OMatSetIndex.GetIndex(nMat_index_u2)] ;
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			bIsBlockEnd = true;
		}
		return;
	}

	//////////////////////  Read material density  ///////////////////////////
	if(fscanf(p_fpInputFilePtr,"%lf",&cMaterial.p_vMatSet[nMat_index].p_dMatUserDen) < 1)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown density of material %d.\n",nMat_index_u);}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

/*
	Output.CheckInputParas(cMaterial.p_vMatSet[nMat_index].p_dMatUserDen != 0,
		[&](){sprintf(Output.p_sPrintStr,"incorrect density(%.3f) for material %d.\n",cMaterial.p_vMatSet[nMat_index].p_dMatUserDen,nMat_index_u);},2);	
*/


	///////////////////  Read nuclides and densities   //////////////////////
	int j = 0;
	for(;;)
	{
		int nRead_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if(nRead_p >= 2 )
		{
			if(nRead_p >= 3 ){bIsBlockEnd = true;}
			break;
		}

		char chRead_s[200];
		double dRead_f;

		Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%s %lf",chRead_s,&dRead_f) >= 2,
			[&](){sprintf(Output.p_sPrintStr,"unknown density of nuclide %s in material %d!\n",chRead_s,nMat_index_u);},2);	

		Output.CheckInputParas(strlen(chRead_s) <= 10,
			[&](){sprintf(Output.p_sPrintStr,"nuclide ID %s is too long.\n",chRead_s);},2);	

		CDNucId OReadId;
		strcpy(OReadId.p_chIdStr, chRead_s);
		cMaterial.p_vMatSet[nMat_index].p_vMatNucID.push_back(OReadId);
		cMaterial.p_vMatSet[nMat_index].p_vMatNucUserDen.push_back(dRead_f);
		// check duplicated nuclide input in material 
		for (int l = 0; l < j; l++)
		{
			if( strcmp(cMaterial.GetMatNucID(nMat_index,l).p_chIdStr,OReadId.p_chIdStr) == 0 )
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"duplicated nuclide %s in material %d.\n", OReadId.p_chIdStr, cMaterial.p_OMatSetIndex.GetIndexU(nMat_index));}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}

		++j;
	}
	cMaterial.p_vMatSet[nMat_index].p_dMatTotNucNum = j;

	Output.CheckInputParas(CDGlobeFun::CheckVecSign(cMaterial.p_vMatSet[nMat_index].p_vMatNucUserDen) != 0,
		[&](){sprintf(Output.p_sPrintStr,"inconsistent nuclide density in material %d.\n",nMat_index_u);},2);	

	// treat mat user density = 0 (input absolute density): sum up user densities
	if (cMaterial.p_vMatSet[nMat_index].p_dMatUserDen == 0)
	{
		for (int i = 0; i < cMaterial.p_vMatSet[nMat_index].p_vMatNucUserDen.size(); i++ )
		{
			cMaterial.p_vMatSet[nMat_index].p_dMatUserDen += cMaterial.p_vMatSet[nMat_index].p_vMatNucUserDen[i];
		}
	}

	return;

}

void CDInput::ReadSabCard(bool &bIsBlockEnd,CDMaterial &cMaterial)
{

	bIsBlockEnd = false;


	/////////////////////// Read sab index defined by user //////////////////////////
	int nSabmat_index_u;
	if(fscanf(p_fpInputFilePtr,"%d",&nSabmat_index_u) <=0 || nSabmat_index_u <=0 )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown sab mat index %d in material block.\n",nSabmat_index_u);}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}


	if(cMaterial.p_OMatSetIndex.GetIndex(nSabmat_index_u) <= 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown material corresponding to S(a,b) material %d !\n",nSabmat_index_u);}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	int nSabmat_index = cMaterial.p_OMatSetIndex.GetIndex(nSabmat_index_u);
	char chRead_s[200];
	char nextchar;
	for(;;)
	{
		
		int nRead_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if(nRead_p >= 2 )
		{
			if(nRead_p >= 3 ){bIsBlockEnd = true;}
			break;
		}

		fscanf(p_fpInputFilePtr,"%s",chRead_s) ;
		if(strlen(chRead_s) > 10)
		{		
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"wrong nuclide string in S(a,b) material %d !\n",nSabmat_index_u);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		CDNucId OReadId;
		strcpy(OReadId.p_chIdStr, chRead_s);
		cMaterial.p_vMatSet[nSabmat_index].p_vMatSabNucID.push_back(OReadId);
		cMaterial.p_vMatSet[nSabmat_index].p_dMatTotSabNucNum = cMaterial.p_vMatSet[nSabmat_index].p_dMatTotSabNucNum + 1;
	}
	return;
}