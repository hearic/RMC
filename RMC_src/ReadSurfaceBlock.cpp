# include "Input.h"

void CDInput::ReadSurfaceBlock(CDIndex &cSurfaceIndex,vector<CDSurface> &vSurface)
{
	//auto &SurfaceIndex = Geometry.SurfaceIndex;
	//auto &Surface = Geometry.Surface;
	//IndexClass  &SurfaceIndex = CSurfaceIndex;
	//vector<SurfaceClass> &Surface = vSurface;

	for(;;)
	{
		char nextchar;
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3)   // surface block finished
		{
			break;
		}

		/////////////////////////////// Read surf keyword //////////////////////////////////////
		string KeyWordStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,KeyWordStr);
		Output.CheckInputParas(KeyWordStr.compare("SURF") == 0 ,
			[&](){sprintf(Output.p_sPrintStr,"unknown keyword %s defined in SURFACE block.\n",KeyWordStr.c_str());},2);

		////////////////// Read surface index and update surface index list ////////////////////
		int nSurf_index_u = -1;
		fscanf(p_fpInputFilePtr,"%d",&nSurf_index_u);
		int nSurf_index = cSurfaceIndex.AddUserIndex(nSurf_index_u) ;
		vSurface.resize(nSurf_index + 1);


		//////////////////////////////////  Get surface type /////////////////////////////////
		CDFileIO::GetKeyWord(p_fpInputFilePtr,KeyWordStr);
		vSurface[nSurf_index].GenSurfType(nSurf_index_u, KeyWordStr);
		Output.CheckInputParas(vSurface[nSurf_index].p_nType > 0 ,
			[&](){sprintf(Output.p_sPrintStr,"unknown supported surface type %s!\n",KeyWordStr.c_str());},2);

		//////////////////////////  Get surface parameters ///////////////////////////////
		int nNext_p = CDFileIO::ReadVaryVec(p_fpInputFilePtr,vSurface[nSurf_index].p_vParas,"%lf");
		if(nNext_p >= 3){ break;}
		if(nNext_p == 2){continue;} 
	
		//////////////////////////  read boundary condition //////////////////////////////
		CDFileIO::GetKeyWord(p_fpInputFilePtr,KeyWordStr);
		Output.CheckInputParas(KeyWordStr.compare("BC") == 0 ,
			[&](){sprintf(Output.p_sPrintStr,"unknown keyword %s defined in surface %d!\n",KeyWordStr.c_str(),nSurf_index_u);},2);


		CDFileIO::SkipSpecChar(p_fpInputFilePtr, '=');
		int nRead_i = -1;
		fscanf(p_fpInputFilePtr,"%d",&nRead_i);

		Output.CheckInputParas(nRead_i == 0 || nRead_i == 1,
			[&](){sprintf(Output.p_sPrintStr,"unknown boundary condition(bc=%d) for surface %d !\n",nRead_i,nSurf_index_u);},2);

		vSurface[nSurf_index].p_nBoundCond = nRead_i;
	}

	Output.CheckInputParas(vSurface.size() > 1,
		[&](){sprintf(Output.p_sPrintStr,"no surfaces are defined.\n");},2);

	return;
}