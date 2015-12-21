# include "AceData.h"
# include "Criticality.h"

void CDAceData::ClearData()
{
	p_vNuclides.clear();
	/*//XSS.clear();
	NXS.clear();
	JXS.clear();
	LDLW.clear();
	LAND.clear();
	LSIG.clear();
	NucAtomWgt.clear();
	NucTmp.clear();
	NucZaid.clear();
	MTRIndex.clear();
	FisXSS.clear();
	InelXSS.clear();
	NucCs.clear();*/
}

void CDAceData::InitiateAce(vector<CDNucId> vNucIDlist)
{
	int nTotNucNum = vNucIDlist.size();

	p_vNuclides.resize(nTotNucNum);
	for (int i = 1;i < nTotNucNum; ++i)
	{
		p_vNuclides[i].p_ONucID = vNucIDlist[i];
		p_vNuclides[i].NXS.resize(17);
		p_vNuclides[i].JXS.resize(33);
	}

	if(p_bIsMultiGroup)  //// use multi-group ace data
	{
		p_vMltErgBins.resize(p_nMltGrpNum);
		p_vMltCentErg.resize(p_nMltGrpNum);
		p_vMgP0Loc.resize(nTotNucNum );
		p_vMgP0Cs.resize(nTotNucNum );
		for(int i = 1;i < nTotNucNum; ++i)
		{
			p_vMgP0Loc[i].resize(p_nMltGrpNum);
			p_vMgP0Cs[i].resize(p_nMltGrpNum);
		}
	}

	/*NucAtomWgt.resize(TotNucNum+1);
	NucTmp.resize(TotNucNum+1);
	NucZaid.resize(TotNucNum+1);
	NXS.resize(TotNucNum+1);
	JXS.resize(TotNucNum+1);
	for(int i = 1;i <= TotNucNum; ++i)
	{
		NXS[i].resize(17);
		JXS[i].resize(33);
	}
	XSS.resize(TotNucNum + 1);
	NucCs.resize(TotNucNum+1);  

	if(IsMultiGroup)  //// use multi-group ace data
	{
		MltErgBins.resize(MltGrpNum);
		MltCentErg.resize(MltGrpNum);
		MgP0Loc.resize(TotNucNum + 1);
		MgP0Cs.resize(TotNucNum + 1);
		for(int i = 1;i <= TotNucNum; ++i)
		{
			MgP0Loc[i].resize(MltGrpNum);
			MgP0Cs[i].resize(MltGrpNum);
		}
	}
	else
	{
		MTRIndex.resize(TotNucNum+1);
		LSIG.resize(TotNucNum+1);
		LAND.resize(TotNucNum+1);
		LDLW.resize(TotNucNum+1);

		FisXSS.resize(TotNucNum+1);
		InelXSS.resize(TotNucNum+1);

		NucInterpPos.resize(TotNucNum+1);
		NucInterpFrac.resize(TotNucNum+1);

		ProbTableFlag.resize(TotNucNum+1);
	}*/
}