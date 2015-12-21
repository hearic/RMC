# include "AceData.h"
# include "algorithm"

void CDAceData::InterpolateXssTab(double dErg, int nNuc, int LDAT, int &nPos, double &dFrac, int &nNumOfInterpRegion, int &nNumOfErgGrid)
{
	//// obtain number of interpolation regions and incoming energies 
	nNumOfInterpRegion = int(p_vNuclides[nNuc].XSS[LDAT]);
	int nE_grid_base = LDAT + 2*nNumOfInterpRegion + 1;
	nNumOfErgGrid = int(p_vNuclides[nNuc].XSS[nE_grid_base]);
	dFrac = 0;

	// obtain energy bin and interpolation fraction
	int nInterp_1 = nE_grid_base + 1;
	int nInterp_2 = nE_grid_base + nNumOfErgGrid;

	if (dErg <= p_vNuclides[nNuc].XSS[nInterp_1])
	{
		dFrac = 0;
		nPos = 1;
		return;
	}
	if (dErg >= p_vNuclides[nNuc].XSS[nInterp_2])
	{
		dFrac = 1;
		nPos = nNumOfErgGrid - 1;
		return;
	}


	CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nNuc].XSS[0],dErg,nInterp_1,nInterp_2,nPos,dFrac);
	nPos = nPos - nE_grid_base;

	if( nNumOfInterpRegion == 0 )
	{
		return ;		
	}


	for(int n = 1;n <= nNumOfInterpRegion; ++n)
	{
		if( nInterp_2-nE_grid_base <= int(p_vNuclides[nNuc].XSS[LDAT+n]) && int(p_vNuclides[nNuc].XSS[LDAT+nNumOfInterpRegion+n]) == 1)
		{
			dFrac = 0 ;
			return;
		}
	}

	if( int(p_vNuclides[nNuc].XSS[LDAT+nNumOfInterpRegion+nNumOfInterpRegion])==1 ) 
	{
		dFrac = 0 ;
		return;
	}
}


double CDAceData::GetErgFuncValue(int nNuc, int LDAT,double dErg)
{
	double Ti = 0;
	double dErg_1 = 0, dErg_2 = 0, dFunc_val_1 = 0, dFunc_val_2 = 0;
	int nNumOfInterpRegion = int(p_vNuclides[nNuc].XSS[LDAT]);
	int nE_grid_base = LDAT+2*nNumOfInterpRegion+1;
	int nNumOfErgGrid = int(p_vNuclides[nNuc].XSS[nE_grid_base]);

	//use the extreme value if erg is off either end of the table.
	if( dErg >= p_vNuclides[nNuc].XSS[nE_grid_base+nNumOfErgGrid] ) 
	{
		Ti = p_vNuclides[nNuc].XSS[nE_grid_base+2*nNumOfErgGrid];
		return Ti;
	}

	else if( dErg <= p_vNuclides[nNuc].XSS[nE_grid_base+1] )
	{
		Ti = p_vNuclides[nNuc].XSS[nE_grid_base+nNumOfErgGrid+1];
		return Ti;
	}

	//binary search for the location of erg in the table.
	int min = nE_grid_base+1;
	int max = nE_grid_base+nNumOfErgGrid;

	double fr;
	int nPos;
	CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nNuc].XSS[0],dErg,min,max,nPos,fr);

	dErg_1 = p_vNuclides[nNuc].XSS[nPos];
	dErg_2 = p_vNuclides[nNuc].XSS[nPos+1];
	dFunc_val_1 = p_vNuclides[nNuc].XSS[nPos+nNumOfErgGrid];
	dFunc_val_2 = p_vNuclides[nNuc].XSS[nPos+1+nNumOfErgGrid];

	//find out which kind of interpolation should be used.
	if( nNumOfInterpRegion==0 ) 
	{ 
		Ti = dFunc_val_1+(dFunc_val_2-dFunc_val_1)*(dErg-dErg_1)/(dErg_2-dErg_1);
		return Ti;
	}
	int n = 0;
	for( n = 1;n <= nNumOfInterpRegion;n++)
	{
		if( nPos+1-nE_grid_base <= int(p_vNuclides[nNuc].XSS[LDAT+n]) ) 
		{
			goto Interp;
		}
	}
	n = nNumOfInterpRegion;

	// interpolate between table entries.
Interp:
	switch( int(p_vNuclides[nNuc].XSS[LDAT+nNumOfInterpRegion+n]) )
	{
	case 1 :
		{
			Ti = dFunc_val_1;
			break;
		}
	case 2 :
		{
			Ti = dFunc_val_1+(dFunc_val_2-dFunc_val_1)*(dErg-dErg_1)/(dErg_2-dErg_1);
			break;
		}
	case 3 :
		{
			Ti = dFunc_val_1+(dFunc_val_2-dFunc_val_1)*log(dErg/dErg_1)/log(dErg_2/dErg_1);
			break;
		}
	case 4 :
		{
			Ti = dFunc_val_1*pow(dFunc_val_2/dFunc_val_1,(dErg-dErg_1)/(dErg_2-dErg_1));
			break;
		}
	case 5 :
		{
			Ti = dFunc_val_1*pow(dFunc_val_2/dFunc_val_1,log(dErg/dErg_1)/log(dErg_2/dErg_1));
			break;
		}
	}

	return Ti;
}

double CDAceData::SampleMaxwell(double T)
{
//===============================================================================
// MAXWELL_SPECTRUM samples an energy from the Maxwell fission distribution based
// on a direct sampling scheme. The probability distribution function for a
// Maxwellian is given as p(x) = 2/(T*sqrt(pi))*sqrt(x/T)*exp(-x/T). This PDF can
// be sampled using rule C64 in the Monte Carlo Sampler LA-9721-MS.
//===============================================================================

    // T: tabulated function of incoming E
    double ksi1, ksi2, ksi3;
    double c ;       // cosine of pi/2*r3

	ksi1 = ORNG.Rand();
    ksi2 = ORNG.Rand();
    ksi3 = ORNG.Rand();
    c = cos(3.1415926536/2.*ksi3);

    return -T*(log(ksi1) + log(ksi2)*c*c);
}

double CDAceData::SampleWatt(double a, double b)
{
//===============================================================================
// WATT_SPECTRUM samples the outgoing energy from a Watt energy-dependent fission
// spectrum. Although fitted parameters exist for many nuclides, generally the
// continuous tabular distributions (LAW 4) should be used in lieu of the Watt
// spectrum. This direct sampling scheme is an unpublished scheme based on the
// original Watt spectrum derivation (See F. Brown's MC lectures).
//===============================================================================

   double w  = SampleMaxwell(a);
   double ksi = ORNG.Rand();
   return w + a*a*b/4. + (2.*ksi - 1)*sqrt(a*a*b*w);
}