# include "Material.h"

void CDMaterial::CheckSabMaterial(CDAceData& cAceData)
{
	//// step4:  MatNucIndex +  MatSabNucIndex ------>   SabnucOfNuc
	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum;i++)
	{
		p_vMatSet[i].p_vSabnucOfNuc.resize(GetMatTotNucNum(i));
		p_vMatSet[i].p_vMatSabNucEsa.resize(GetMatTotNucNum(i));

		for(int k = 0 ; k < GetMatTotSabNucNum(i); k++)
		{
			bool bSab_nuc_found = false; 
			for(int j = 0 ; j < GetMatTotNucNum(i) ; j++)
			{
				if(cAceData.p_vNuclides[GetMatNucIndex(i,j)].p_nNucZaid == cAceData.p_vNuclides[GetMatSabNucIndex(i,k)].p_nNucZaid )
				{
					int nSab_nuclide=GetMatSabNucIndex(i,k);
					p_vMatSet[i].p_vSabnucOfNuc[j] = nSab_nuclide;
					double dEsa1 = cAceData.p_vNuclides[nSab_nuclide].XSS[cAceData.GetLocOfSabInelErg(nSab_nuclide) + 
						int(cAceData.p_vNuclides[nSab_nuclide].XSS[cAceData.GetLocOfSabInelErg(nSab_nuclide)])];

					p_vMatSet[i].p_vMatSabNucEsa[j] = dEsa1;
					if( cAceData.GetLocOfSabElErg(nSab_nuclide) != 0 && cAceData.GetSabElMode(nSab_nuclide) != 4 ) 
					{
						double dEsa2 = cAceData.p_vNuclides[nSab_nuclide].XSS[cAceData.GetLocOfSabElErg(nSab_nuclide) + 
							int(cAceData.p_vNuclides[nSab_nuclide].XSS[cAceData.GetLocOfSabElErg(nSab_nuclide)])];
						p_vMatSet[i].p_vMatSabNucEsa[j] = std::min(dEsa1,dEsa2);
					}
					bSab_nuc_found = true;
					//break;
				}
			}
			if(!bSab_nuc_found)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown nuclide corresponding to S(a,b) %s in material %d !\n",
					cAceData.p_vNuclides[GetMatSabNucIndex(i,k)].p_ONucID.p_chIdStr,p_OMatSetIndex.GetIndexU(i));}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}
	}
}