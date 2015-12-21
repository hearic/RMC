# include "AceData.h"
# include "Material.h"

void CDAceData::GenErgBinMap(CDMaterial &cMaterial)
{
	if(p_bIsMultiGroup || !p_bUseErgBinMap)
	{
		return;
	}
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Energy-bin method is used to accelerate ACE treatment.\n");},
		Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	////////////////////////// setup parameters of energy bin map //////////////////////////
	p_OAceErgBinMap.p_nHashType = 1;
	p_OAceErgBinMap.p_nFraNum = 100;
	p_OAceErgBinMap.p_nExpNum = 61;
	p_OAceErgBinMap.p_nExpStart = -40;

	///////////////////// establish the energy bin map for all nuclides /////////////////////
	vector<vector<CDBinPtr> > vAceErgBinPtrTemp;

	int nTotNucNum = cMaterial.GetTotNucNumExcSab();
	vAceErgBinPtrTemp.resize(nTotNucNum+1);
	
	for(int nNuc = 1; nNuc <= nTotNucNum; ++nNuc)   
	{
		int NE = GetErgGridNum(nNuc);
		int Loc = GetLocOfESZ(nNuc);   //  Loc = 1
		p_OAceErgBinMap.GenBinMapOfArray(vAceErgBinPtrTemp[nNuc],&p_vNuclides[nNuc].XSS[0],Loc,Loc + NE - 1);
	}	

	/////////////////// exchange dimensions of bin map to better using cache  ///////////////
	int nSize = vAceErgBinPtrTemp[1].size();
	p_vAceErgBinPtr.resize(nSize);
	for(int i = 0; i < nSize ; ++i)
	{
		p_vAceErgBinPtr[i].resize(nTotNucNum+1);

		for(int nuc = 1; nuc <= nTotNucNum; ++nuc)   
		{
			p_vAceErgBinPtr[i][nuc] = vAceErgBinPtrTemp[nuc][i];
		}
	}
	return;
}
