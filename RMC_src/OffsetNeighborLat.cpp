# include "Universe.h"

int CDUniverse::OffsetNeighborLat(const int nLatIndex,const int nLatBoundSURF,double dPointPos[3])
{
	enum eType {rectangle = 1, hexagon = 2};
	int nNewLatIndex = -1;
	////////////////////////////////////////////////////////////////////////
	///////Note: Original lattice is (0, 0, 0), namely lat_index = 1 ///////
	////////////////////////////////////////////////////////////////////////

	if(p_nLatType == rectangle)  // 3D rectangle lattice
	{
		nNewLatIndex = OffsetNeiLatRect(nLatIndex,nLatBoundSURF,dPointPos);
	}
	else if(p_nLatType == hexagon)  // 2D hexagon lattice
	{
		nNewLatIndex = OffsetNeiLatHex(nLatIndex,nLatBoundSURF,dPointPos);
	}

	////////////////////////////////////////////////////////////////////////
	///////Note: Original lattice is (0, 0, 0), namely lat_index = 1 ///////
	////////////////////////////////////////////////////////////////////////
	if( nNewLatIndex < 1 || nNewLatIndex > p_nFilledLatNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"offset lattice index out of range.\n");},CDOutput::_WARNING);

		nNewLatIndex = -1;
	}

	return nNewLatIndex;
}


int CDUniverse::OffsetNeiLatRect(const int nLatIndex,const int nLatBoundSURF,double dPointPos[3])
{
	/////////////  offset lattice index and particle position   //////////////
	/////////////  1:-x ,  2:+x ,  3:-y ,  4:+y , 5:-z ,  6:+z   /////////////
	int nNewLatIndex = -1;
	switch(nLatBoundSURF)
	{
	case 1 :
		{
			nNewLatIndex = nLatIndex - 1;
			dPointPos[0] = dPointPos[0] + p_dPitch[0] ;
			break;
		}
	case 2 :
		{
			nNewLatIndex = nLatIndex + 1;
			dPointPos[0] = dPointPos[0] - p_dPitch[0] ;
			break;
		}
	case 3 :
		{
			nNewLatIndex = nLatIndex - p_nScope[0];
			dPointPos[1] = dPointPos[1] + p_dPitch[1] ;
			break;
		}
	case 4 :
		{
			nNewLatIndex = nLatIndex + p_nScope[0];
			dPointPos[1] = dPointPos[1] - p_dPitch[1] ;
			break;
		}
	case 5 :
		{
			nNewLatIndex = nLatIndex - p_nScope[0]*p_nScope[1];
			dPointPos[2] = dPointPos[2] + p_dPitch[2] ;
			break;
		}
	case 6 :
		{
			nNewLatIndex = nLatIndex + p_nScope[0]*p_nScope[1];
			dPointPos[2] = dPointPos[2] - p_dPitch[2] ;
			break;
		}
	}
    return nNewLatIndex;
}


int CDUniverse::OffsetNeiLatHex(int nLatIndex,int nLatBoundSURF,double dPointPos[3])
{   
	/////////////  offset lattice index and particle position   //////////////
	//////////  1: FA ,  2: AB ,  3: BC ,  4: CD , 5: DE ,  6: EF  ///////////
	///////////////////  b1 = (L1, 0), b2=(0.5*L1, H) ////////////////////////
	//double L1 = Pitch[0], 	L2 = Pitch[1];
	int i2 = (nLatIndex - 1)/p_nScope[0];
	int i1 = nLatIndex -1 - i2*p_nScope[0];

	int nNewLatIndex = -1;
	switch(nLatBoundSURF)
	{
	case 1 :  // surface FA
		{
			//NewLatIndex = LatIndex - 1;
			i1 = i1 - 1;
			dPointPos[0] = dPointPos[0] + p_dPitch[0] ;
			break;
		}
	case 2 :  // surface AB
		{
			//NewLatIndex = LatIndex - Scope[0];
			i2 = i2 - 1;
			dPointPos[0] = dPointPos[0] + 0.5 * p_dPitch[0] ;
			dPointPos[1] = dPointPos[1] + p_dHeight ;
			break;
		}
	case 3 :   // surface BC
		{
			//NewLatIndex = LatIndex - Scope[0] + 1;
			i1 = i1 + 1;
			i2 = i2 - 1;
			dPointPos[0] = dPointPos[0] - 0.5 * p_dPitch[0] ;
			dPointPos[1] = dPointPos[1] + p_dHeight ;
			break;
		}
	case 4 :   // surface CD
		{
			//NewLatIndex = LatIndex + 1;
			i1 = i1 + 1;
			dPointPos[0] = dPointPos[0] - p_dPitch[0] ;
			break;
		}
	case 5 :   // surface DE
		{
			//NewLatIndex = LatIndex + Scope[0] ;
			i2 = i2 + 1;
			dPointPos[0] = dPointPos[0] - 0.5 * p_dPitch[0] ;
			dPointPos[1] = dPointPos[1] - p_dHeight ;
			break;
		}
	case 6 :   // surface EF
		{
			//NewLatIndex = LatIndex + Scope[0] - 1 ;
			i1 = i1 - 1;
			i2 = i2 + 1;
			dPointPos[0] = dPointPos[0] + 0.5 * p_dPitch[0] ;
			dPointPos[1] = dPointPos[1] - p_dHeight ;
			break;
		}
	}

	if(i1 >= 0 && i2 >= 0 && i1 < p_nScope[0] && i2 < p_nScope[1]) 
	{
		nNewLatIndex = 1 + i1 + p_nScope[0]*i2;
	}

	return nNewLatIndex;
}