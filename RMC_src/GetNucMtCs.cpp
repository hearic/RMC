# include "AceData.h"

double CDAceData::GetNucMtCs(int nNuc, int MT, int nInterpPos, double dInterpFrac)
{

	if(MT >= p_vNuclides[nNuc].LSIG.size()) /// fix bug: 2013-07-13
	{
		return 0;
	}


	if( p_vNuclides[nNuc].LSIG[MT] <= 0 )  // mt number is not found
	{
		return 0;
	}

	int nIE_LOCA = GetLocOfSIG(nNuc) + p_vNuclides[nNuc].LSIG[MT]-1;
	int nSIG_IE = int(p_vNuclides[nNuc].XSS[nIE_LOCA]);
	if(nInterpPos < nSIG_IE)
	{
		return 0;
	}
	else
	{
		return CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].XSS[0],nIE_LOCA + 2 + nInterpPos - nSIG_IE,dInterpFrac);
	}

}