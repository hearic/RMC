# include "AceData.h"
# include "Criticality.h"

void CDAceData::CheckMgAceBlock(int nTotNonSabNucNum)
{
	int nGrpNum = p_nMltGrpNum;

	//////////////////////////check indentifiers ////////////////////////////////////
	for(int nNuc = 1;nNuc <= nTotNonSabNucNum;++nNuc)
	{
		//// check Identifier for incident particle
		if(GetMgIncParId(nNuc) != 1)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"multi-group data of %s is not for neutron.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		//// check multi-group number
		if(GetMgGrpNum(nNuc) != nGrpNum)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"multi-group number of %s conflicts user-defined value.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		//// check secondary particles
		//if(GetMgSecNum(nuc) != 0)
		//{
		//	printf("nuclide %s has secondary particles.\n",Nuclides[nuc].NucID.IdStr);
		//}
	}

	//////////////////////////check group structure /////////////////////////////////
	for(int nNuc = 1;nNuc <= nTotNonSabNucNum;++nNuc)
	{
		int nLERG = GetMgLERG(nNuc);
		for(int i = nGrpNum ; i >= 1 ; --i)
		{
			double dCentErg = p_vNuclides[nNuc].XSS[nLERG + i - 1];
			double dErgWidth = p_vNuclides[nNuc].XSS[nLERG + nGrpNum + i - 1];
			double dLowerBound = dCentErg - 0.5*dErgWidth;
			if(nNuc == 1)
			{
				p_vMltCentErg[nGrpNum - i] = dCentErg;     //// ascending
				p_vMltErgBins[nGrpNum - i] = dLowerBound;  //// ascending
			}
			else
			{
				if(fabs(p_vMltErgBins[nGrpNum - i] - dLowerBound) > 1.0E-6*dLowerBound)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"multi-group structure conflicts between %s and %s.\n",
						p_vNuclides[nNuc].p_ONucID.p_chIdStr,p_vNuclides[1].p_ONucID.p_chIdStr);}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
				}
			}
		}
	}


	
	////////////// generate cumulative scattering probability table /////////////////
	for(int nNuc = 1;nNuc <= nTotNonSabNucNum;++nNuc)
	{
		//// check LP0L existing
		int LP0L = GetMgLP0L(nNuc);
		if(LP0L == 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%s has not P0 block.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		//// set up P0 locators
		p_vMgP0Loc[nNuc][0] = 1;
		for(int i = 1 ; i <= nGrpNum - 1 ; ++i)
		{
			p_vMgP0Loc[nNuc][i] = p_vMgP0Loc[nNuc][i-1] + std::min(i,GetMgUpScatNum(nNuc)+1)
				+ std::min(GetMgGrpNum(nNuc)-i,GetMgDownScatNum(nNuc));
		}
		
		//// sum up P0 cross-section
		int nLP0 = int(p_vNuclides[nNuc].XSS[LP0L]);
		//int LXPN1 = int(Nuclides[nuc].XSS[GetMgLXPNL(nuc)]);
		int k = 0;
		for(int i = 1 ; i <= nGrpNum ; ++i)
		{
			int nI1 = std::max(1,i - GetMgUpScatNum(nNuc));
		    int nI2 = std::min(GetMgGrpNum(nNuc),i + GetMgDownScatNum(nNuc));
			p_vMgP0Cs[nNuc][i-1] = 0;
			for(int j = nI1 ; j <= nI2 ; ++j)
			{
				p_vMgP0Cs[nNuc][i-1] = p_vMgP0Cs[nNuc][i-1] + p_vNuclides[nNuc].XSS[nLP0 + k];
				k ++ ;
			}
			if(p_vMgP0Cs[nNuc][i-1] <= 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect P0 cross-section for %s of group %d.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr,i);}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			}
		}
		
		//// check cross-section summing up
		//for(int i = 1 ; i <= GrpNum ; ++i)
		//{
		//	double tot = GetMgNucTotCs(nuc,i);
		//	double fis = GetMgNucFisCs(nuc,i);
		//	double abs = GetMgNucAbsCs(nuc,i);
		//	double scatt = GetMgNucP0Cs(nuc,i);
		//	double temp = fis + abs + scatt - tot;

		//	if( fabs(temp) >= tot*1.0E-4)
		//	{
		//		printf("total cross-section is not equal to summed cross-sections. nuclide = %s, group = %d .\n",Nuclides[nuc].NucID.IdStr,i);
		//	}
		//}
	}

	return;
}