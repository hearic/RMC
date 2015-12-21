# include "Universe.h"
const double ZERO_DISTANCE = 1.0E-10;

int CDUniverse::FindLatIndex(const double dPointPos[3],const double dPointDir[3])
{
	enum eType {rectangle = 1, hexagon = 2};
	int nLat_index = -1;
	////////////////////////////////////////////////////////////////////////
	///////Note: Original lattice is (0, 0, 0), namely lat_index = 1 ///////
	////////////////////////////////////////////////////////////////////////

	if(p_nLatType == rectangle)  // 3D rectangle lattice
	{
		nLat_index = FindLatIndexRect(dPointPos,dPointDir);
	}
	else if(p_nLatType == hexagon)  // 2D hexagon lattice
	{
		nLat_index = FindLatIndexHex(dPointPos,dPointDir);
	}

	if(nLat_index <=0 || nLat_index > p_nFilledLatNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to locate lattice index.\n");},CDOutput::_WARNING);
		nLat_index = -1;
	}

	return nLat_index ;
}

int CDUniverse::FindLatIndexRect(const double dPointPos[3],const double dPointDir[3])
{
	int nLat_index = 0 ;
	int dXYZ[3];
	for(int i = 0 ; i < 3 ; ++i)
	{
		if(p_nScope[i] == 1)
		{
			dXYZ[i] = 0 ;
		}
		else
		{
			double dCalcTemp1 = dPointPos[i]/p_dPitch[i] ;
			int nCalcTemp2 = int(dCalcTemp1 + 0.5);

			if(fabs(dCalcTemp1 - nCalcTemp2) < ZERO_DISTANCE)
			{
				if(dPointDir[i] >=0 )
				{
					dXYZ[i] = nCalcTemp2;
				}
				else
				{
					dXYZ[i] = nCalcTemp2 - 1;
				}
			}
			else
			{
				dXYZ[i] = int(dCalcTemp1);
			}
		}	
	}
	nLat_index = 1 + dXYZ[0] + p_nScope[0]*dXYZ[1] + p_nScope[0]*p_nScope[1]*dXYZ[2];

	return nLat_index;
}

int CDUniverse::FindLatIndexHex(const double dPointPos[3],const double dPointDir[3])
{
	//////////////////////////////// calculate lattice parameters ///////////////////////////////////
	double dPos1 = dPointPos[0],dPos2 = dPointPos[1];              //// point position in 2D rectangular coordinates
    double dDir1 = dPointDir[0],dDir2 = dPointDir[1];              //// point direction in 2D rectangular coordinates
	double dLen1 = p_dPitch[0];//// dLen2 = Pitch[1];
	double dDis2 = 0.5*dLen1*p_dCos_sita + p_dHeight*p_dSin_sita;  ////  distance between AB and DE


	////////////////////////////////// find parallelogram index  ////////////////////////////////////      
	////////////////// AM = k1*b1 + k2*b2, where b1=(L1,0) and b2 = (L1/2,height)  ////////////////////
	double dK2 = dPos2/p_dHeight;                 
	double dK1 = (dPos1 - dK2*0.5*dLen1)/dLen1; 
	int i1 = int(floor(dK1)) ; //// note: ik1,ik2 may be start from -1
	int i2 = int(floor(dK2)) ;
	///////////////////// offset r1 r2 to parallelogram of index 0 //////////////////////////
	dPos1 = dPos1 - (i1*dLen1 + i2*0.5*dLen1);
	dPos2 = dPos2 - i2*p_dHeight;



	///////////////////////////////// find hexagon index ///////////////////////////////////////////
	double dSense1 = dPos1 - dLen1;
	if( fabs(dSense1) < ZERO_DISTANCE)
	{
		dSense1 = dDir1 > 0? 1 : -1 ;
	}
	if(dSense1 > 0)
	{
		double dSense2 = (dPos1 - dLen1)*p_dCos_sita + dPos2*p_dSin_sita - 0.5*dDis2;
		if(fabs(dSense2) < ZERO_DISTANCE)
		{
			double dSense_dir = dDir1*p_dCos_sita + dDir2*p_dSin_sita;
			dSense2 = dSense_dir > 0? 1 : -1 ;
		}
		i1 = i1 + 1;
		if(dSense2 > 0)
		{
			i2 = i2+1 ;
		}
	}
	else
	{
		double dSense3 = dPos1 - 0.5*dLen1 ;
		if(fabs(dSense3) < ZERO_DISTANCE)
		{
			dSense3 = dDir1>0? 1 : -1 ;
		}
		if(dSense3 > 0)
		{
			double dSense4 = ( dPos1 - dLen1 )*p_dCos_sita - dPos2*p_dSin_sita + 0.5*dDis2;
			if(fabs(dSense4) < ZERO_DISTANCE)
			{
				double dSensedir = dDir1*p_dCos_sita - dDir2*p_dSin_sita;
				dSense4 = dSensedir > 0? 1 : -1 ;
			}
			if(dSense4 > 0)
			{
				i1 = i1 + 1;
			}
			else
			{
				i2 = i2 + 1;
			}
		}
		else
		{
			double dSense5 = dPos1*p_dCos_sita + dPos2*p_dSin_sita - 0.5*dDis2;
			if(fabs(dSense5) < ZERO_DISTANCE)
			{
				double dSensedir = dDir1*p_dCos_sita + dDir2*p_dSin_sita;
				dSense5 = dSensedir > 0? 1 : -1 ;
			}
			if(dSense5 > 0)
			{
				i2 = i2 + 1;
			}
		}
	}

	/////////////////////////////// convert to single-number index //////////////////////////////////
	int nLat_index = -1;
	if(i1 >= 0 && i2 >= 0 && i1 < p_nScope[0] && i2 < p_nScope[1]) 
	{
		nLat_index = 1 + i1 + p_nScope[0]*i2;
	}

	if(nLat_index <= 0 || nLat_index > p_nFilledLatNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to locate lattice index.\n");},CDOutput::_WARNING);
		nLat_index = -1;
	}

	return nLat_index;
}