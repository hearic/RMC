# include "Material.h"

double CDMaterial::GetMatFuelDen(int m, vector<CDNuclide>& vNuclides)
{
	int nNucNum = p_vMatSet[m].p_dMatTotNucNum;
	double nSum = 0;

	for(int i = 0 ; i < nNucNum; ++i )
	{
		int nNuc = p_vMatSet[m].p_vMatNucIndex[i];
		int nZA = vNuclides[nNuc].p_nNucZaid;
		if(nZA >= 90000)
		{
			nSum = nSum + GetMatNucGramDen(m, i, vNuclides);
		}
	}
	return nSum;
}