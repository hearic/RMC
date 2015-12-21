# include "Universe.h"


void CDSurface::CheckSurfParas()
{
	////////////////////////// check  validity of surface parameters ///////////////////////
	auto vSurf_para = p_vParas;
	int nType = p_nType;

	////////////////////////// check  number of surface parameters ///////////////////////
	static int nPara_size[26] = {4,1,1,1,1,4,2,2,2,3,3,3,1,1,1,5,5,5,3,3,3,10,10,6,6,6};
	
	Output.CheckInputParas(int(vSurf_para.size()) == nPara_size[nType - 1],
		[&](){sprintf(Output.p_sPrintStr,"incorrect parameter entries for surface %s.\n",p_sSurfStr.c_str());},2);

	////////////////////////// check  number of surface parameters ///////////////////////
	bool bCheck_through = true;
	if(nType == 1 || nType == 2 || nType == 3 || nType == 4) 
	{
		bCheck_through = true;
	}
	if(nType == 5)
	{
		bCheck_through = (vSurf_para[0] > 0);
	}
	else if(nType == 6)
	{
		bCheck_through = (vSurf_para[3] > 0);
	}
	else if(nType == 7 || nType == 8 || nType == 9)
	{
		bCheck_through = (vSurf_para[1] > 0);
	}
	else if(nType == 10 || nType == 11 || nType == 12)
	{
		bCheck_through = (vSurf_para[2] > 0);
	}
	else if(nType == 13 || nType == 14 || nType == 15)
	{
		bCheck_through = (vSurf_para[0] > 0);
	}
	else if(nType == 16|| nType == 17 || nType == 18)
	{
		bCheck_through = (vSurf_para[3] > 0 || fabs(vSurf_para[4]) == 1);
	}
	else if(nType == 19|| nType == 20 || nType == 21)
	{
		bCheck_through = (vSurf_para[1] > 0 || fabs(vSurf_para[2]) == 1);
	}

	Output.CheckInputParas(bCheck_through,
		[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for surface %s.\n",p_sSurfStr.c_str());},2);

	return;
}