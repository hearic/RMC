# include "AceData.h"

double CDAceData::GetTotalNu(double dEg0, int nNuc)
{
	int nKNU = GetLocOfNU(nNuc);
	if( nKNU == 0 ) // no neutron yield
	{
		return 0;
	}   

	int nKNU_judge = int(p_vNuclides[nNuc].XSS[nKNU]);

	if(nKNU_judge < 0)  /// both prompt and total NU are given
	{
		nKNU = nKNU - nKNU_judge + 1;
	}  

	int LNU = int(p_vNuclides[nNuc].XSS[nKNU]);
	///// calculate NuTemp by evaluating a polynomial in energy /////
	if(LNU == 1)
	{
		int NC = int(p_vNuclides[nNuc].XSS[nKNU+1]) - 1;
		double dNuTemp = p_vNuclides[nNuc].XSS[nKNU+NC+2] ;
		for(int i = 1;i <= NC;i++)
		{
			dNuTemp = dNuTemp*dEg0 + p_vNuclides[nNuc].XSS[nKNU + NC + 2 - i];
		}
		return dNuTemp;
	}


	return GetErgFuncValue(nNuc, nKNU + 1, dEg0);

}


double CDAceData::GetDelayedNu(double dEg0, int nNuc)
{

	int nKNU = GetLocOfDNU(nNuc);
	if(nKNU == 0)
	{
		return 0;
	}
	
	return GetErgFuncValue(nNuc, nKNU + 1, dEg0);

}