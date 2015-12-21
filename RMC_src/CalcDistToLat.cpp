# include "Universe.h"
const double ZERO_DISTANCE = 1.0E-24;

double CDUniverse::CalcDistToLat(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf)
{
	enum eType {rectangle = 1, hexagon = 2};
	double dDistance = -1;

	if(p_nLatType == rectangle)    // 3D rectangle lattice
	{
		dDistance = CalcDistToLatRect(dPointPos,dPointDir,nWhichSurf);
	}
	else if(p_nLatType == hexagon)  // 2D hexagon lattice
	{
		dDistance = CalcDistToLatHex(dPointPos,dPointDir,nWhichSurf);
	}

 
	//if(dts <= ZeroDist )
	//{
	//	Output.OutputMessage([&](){sprintf(Output.PrintStr,"failed to calculate distance to lattice boundary.\n");},CD_Output::_WARNING);
	//	dts = -1 ;
	//}

	return dDistance;
}

double CDUniverse::CalcDistToLatRect(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf)
{
	///////////  Calculate distance to Rectangle lattice boundary  /////////////
    ///////////// 1:-x ,  2:+x ,  3:-y ,  4:+y , 5:-z ,  6:+z  /////////////////
	double dDistance = 1.0E+24 ;
	double dDistance_temp ;
	int nSurf_order;

	for(int i = 0 ; i < 3 ; ++i)
	{
		if(p_nScope[i] == 1 || dPointDir[i] == 0)
		{
			continue;
		}

		dDistance_temp = 0;
		if(dPointDir[i] > 0)
		{
			dDistance_temp =  (p_dPitch[i] - dPointPos[i])/dPointDir[i] ; 
			nSurf_order = 2*i + 2;
		}
		else
		{
			dDistance_temp = -dPointPos[i]/dPointDir[i] ;
			nSurf_order = 2*i + 1;
		}

		if(dDistance_temp < dDistance)
		{
			dDistance =  dDistance_temp ;
			nWhichSurf = nSurf_order ;
		}
	}

	return dDistance;
}

double CDUniverse::CalcDistToLatHex(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf)
{
	///////////  Calculate distance to Hexagon lattice boundary  /////////////
	//////////  1: FA ,  2: AB ,  3: BC ,  4: CD , 5: DE ,  6: EF  ///////////
	///////////////////  b1 = (L1, 0), b2=(0.5*L1, H) ////////////////////////

	//////////////////////////////// calculate lattice parameters ///////////////////////////////////
	double dPos1 = dPointPos[0], dPos2 = dPointPos[1];          //// point position in 2D rectangular coordinates
    double dDir1 = dPointDir[0], dDir2 = dPointDir[1];          //// point direction in 2D rectangular coordinates
	double L1 = p_dPitch[0];    // 	L2 = Pitch[1];
	double D2 = 0.5*L1*p_dCos_sita + p_dHeight*p_dSin_sita;     ////  distance between AB and DE


	/////////////////////////////// calculate distance to individual boundary //////////////////////
	double dDts_temp[7];

	///// FA & CD
	if(dDir1 > 0)
	{
	     dDts_temp[1] = -1 ;
		 dDts_temp[4] = (0.5*L1 - dPos1)/dDir1;
	}
	else
	{
	     dDts_temp[1] = (-0.5*L1 - dPos1)/dDir1 ;
		 dDts_temp[4] = -1;
	}

	///// AB & DE
	double t1 = dDir1*p_dCos_sita + dDir2*p_dSin_sita ;  
	double t2 = dPos1*p_dCos_sita + dPos2*p_dSin_sita;
	if(t1 > 0) //// DE
	{
		dDts_temp[2] = -1 ;
		dDts_temp[5] = (0.5*D2 - t2)/t1;
	}
	else //// AB
	{
		dDts_temp[2] = (-0.5*D2 - t2)/t1;
		dDts_temp[5] = -1 ;
	}

	///// BC & EF
	double t3 = dDir1*p_dCos_sita - dDir2*p_dSin_sita ;
	double t4 = dPos1*p_dCos_sita - dPos2*p_dSin_sita;
	if(t3 > 0) //// BC
	{
		dDts_temp[3] = (0.5*D2 - t4)/t3 ;
		dDts_temp[6] = -1 ;
	}
	else  //// EF
	{
		dDts_temp[3] = -1 ;
		dDts_temp[6] = (-0.5*D2 - t4)/t3;
	}

	/////////////////////////////// select minimum distance to lattice boundary //////////////////////
	double dDistance = 1.0E+24;
	for(int i = 1 ; i <= 6 ; ++i)
	{
		if(dDts_temp[i] > 0)
		{
			if(dDts_temp[i] < dDistance)
			{
				dDistance =  dDts_temp[i] ;
				nWhichSurf = i ;
			}
		}
	}

	return dDistance;
}