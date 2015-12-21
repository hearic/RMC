# include "AceData.h"

double CDAceData::GetMgNucTotCs(int nNuc, int nIncidGrp)
{
	if(GetMgLTOT(nNuc) == 0)
	{
		return 0;
	}
	return p_vNuclides[nNuc].XSS[GetMgLTOT(nNuc) + nIncidGrp - 1];
}

double CDAceData::GetMgNucFisCs(int nNuc, int nIncidGrp)
{
	if(GetMgLFIS(nNuc) == 0)
	{
		return 0;
	}
	return p_vNuclides[nNuc].XSS[GetMgLFIS(nNuc) + nIncidGrp - 1];
}


double CDAceData::GetMgNucNubar(int nNuc, int nIncidGrp)
{
	if(GetMgLFIS(nNuc) == 0)
	{
		return 0;
	}
	return p_vNuclides[nNuc].XSS[GetMgLNU(nNuc) + nIncidGrp - 1];
}

double CDAceData::GetMgNucFisChi(int nNuc, int nIncidGrp)
{
	if(GetMgLFIS(nNuc) == 0)
	{
		return 0;
	}
	return p_vNuclides[nNuc].XSS[GetMgLCHI(nNuc) + nIncidGrp - 1];
}

double CDAceData::GetMgNucAbsCs(int nNuc, int nIncidGrp)
{
	if(GetMgLABS(nNuc) == 0)
	{
		return 0;
	}
	return p_vNuclides[nNuc].XSS[GetMgLABS(nNuc) + nIncidGrp - 1];
}

double CDAceData::GetMgNucP0Cs(int nNuc, int nIncidGrp)
{
	return p_vMgP0Cs[nNuc][nIncidGrp - 1];
}

int CDAceData::GetMgNucP0Loc(int nNuc, int nIncidGrp)
{
	return p_vMgP0Loc[nNuc][nIncidGrp - 1];
}

double  CDAceData::GetMgCentErg(int nErgGrp)
{
	int  k = p_nMltGrpNum - nErgGrp;
	return p_vMltCentErg[k];
}