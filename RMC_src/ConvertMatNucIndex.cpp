# include "Material.h"

void CDMaterial::ConvertMatNucIndex(vector<CDNuclide>& vNuclides)
{
	int i,j,k;
	

	//////////  Generate n/sab nuclide index list in each material /////////////
	/////////  MatNucID---> MatNucIndex ;  MatSabNucID---> MatSabNucIndex  /////////
	int nTotMatNum = GetTotMatNum();
	for(i = 1; i <= nTotMatNum;i++)
	{
		p_vMatSet[i].p_vMatNucIndex.resize(GetMatTotNucNum(i));

		for(j=0;j< GetMatTotNucNum(i);j++)   // n library 注意: MatNuc的下标是从0开始
		{
			for(k=1;k<=m_nTotNucNumExcSab;k++)
			{
				if( strcmp(vNuclides[k].p_ONucID.p_chIdStr,GetMatNucID(i,j).p_chIdStr) == 0 )
				{
					p_vMatSet[i].p_vMatNucIndex[j] = k;
					break;
				}
			}
		}

		p_vMatSet[i].p_vMatSabNucIndex.resize(GetMatTotSabNucNum(i));

		for( j = 0 ;j < GetMatTotSabNucNum(i); j++) // sab library
		{
			for(k=m_nTotNucNumExcSab+1;k<=m_nTotNucNumIncSab;k++)
			{
				//if( AceData.nuc_ID[k].compare(GetMatSabNucID(i,j)) == 0 )
				if( strcmp(vNuclides[k].p_ONucID.p_chIdStr,GetMatSabNucID(i,j).p_chIdStr) == 0 )
				{
					p_vMatSet[i].p_vMatSabNucIndex[j] = k;
					break;
				}
			}
		}
	}
}
