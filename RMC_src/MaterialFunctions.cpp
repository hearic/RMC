# include "Material.h"


int  CDMaterial::GetTotMatNum()
{
	return p_vMatSet.size() - 1;
}

int CDMaterial::GetTotNucNumIncSab()
{
	return m_nTotNucNumIncSab;
}

int CDMaterial::GetTotNucNumExcSab()
{
	return m_nTotNucNumExcSab;
}

int CDMaterial::GetTotUsedNucNumExcSab()
{
	return m_nTotUsedNucNumExcSab;
}

int  CDMaterial::GetMatTotNucNum(int m)
{
	return p_vMatSet[m].GetMatTotNucNum();
}

int  CDMaterial::GetMatTotSabNucNum(int m)
{
	return p_vMatSet[m].GetMatTotSabNucNum();
}

CDNucId CDMaterial::GetMatNucID(int m, int i)
{
	if (!p_vMatSet[m].p_vMatNucID.empty())
	{
		return p_vMatSet[m].p_vMatNucID[i];
	}
	else
	{
		int nNucIndex = GetMatNucIndex(m,i);
		return p_vNucIDList[nNucIndex];
	}
}

CDNucId CDMaterial::GetMatSabNucID(int m, int i)
{
	return p_vMatSet[m].GetMatSabNucID(i);
}

int  CDMaterial::GetMatNucIndex(int m, int i)
{
	return p_vMatSet[m].GetMatNucIndex(i);
}

int  CDMaterial::GetMatSabNucIndex(int m, int i)
{
	return p_vMatSet[m].GetMatSabNucIndex(i);
}

int  CDMaterial::GetMatSabnucOfNuc(int m, int i)
{
	return p_vMatSet[m].GetMatSabnucOfNuc(i);
}

double  CDMaterial::GetMatNucSabesa(int m, int i)
{
	return p_vMatSet[m].GetMatNucSabesa(i);
}

double  CDMaterial::GetMatGramDen(int m)
{
	return p_vMatSet[m].p_dMatGramDen; 
}

double  CDMaterial::GetMatAtomDen(int m)
{
	return p_vMatSet[m].p_dMatAtomDen;
}

double  CDMaterial::GetMatUserDen(int m)
{
	return p_vMatSet[m].p_dMatUserDen;
}

double  CDMaterial::GetMatNucUserDen(int m, int i)
{
	if (p_vMatSet[m].p_vMatNucUserDen.empty())
	{
		return GetMatNucAtomDen(m,i);
	}
	else
	{
		return p_vMatSet[m].GetMatNucUserDen(i);  // density input by user
	}
}

double  CDMaterial::GetMatNucAtomDen(int m, int i)
{
	return p_vMatSet[m].GetMatNucAtomDen(i);  // atom density
}

double  CDMaterial::GetMatNucGramDen(int m, int i, vector<CDNuclide>& vNuclides)
{
	int nNucIndex = GetMatNucIndex(m,i);
	double dAtomWeight = vNuclides[nNucIndex].p_dNucAtomWgt;
	return p_vMatSet[m].GetMatNucAtomDen(i)*dAtomWeight/p_dAvgdn;   // gram density (g/cm^3)
}

void  CDMaterial::ClearMatNucUserInput()
{
	// clear NucID/UserDen in every single mat for saving memory
	// use swap to truly release storage
	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum;i++)
	{
		vector<CDNucId> ().swap(p_vMatSet[i].p_vMatNucID);
		vector<double> ().swap(p_vMatSet[i].p_vMatNucUserDen);
	}
}

