# include "AceData.h"

void CDAceData::GetErgBinMapPtr(int nNuc,int &min,int &max)
{
	int nArraySize = p_vAceErgBinPtr.size();
	int nIndex = p_OAceErgBinMap.p_nIndex;
	if(nIndex < 0 || nIndex >= nArraySize)
	{
		//printf("bin map falg\n");
		min = 1 ;
		max = GetErgGridNum(nNuc);
		return;
	}
	min =  p_vAceErgBinPtr[nIndex][nNuc].Lower;
	max =  p_vAceErgBinPtr[nIndex][nNuc].Upper;
	return;
}