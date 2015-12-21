# include "AceData.h"

void CDAceData::CheckCeAceBlock(int nTotNucNum, int nTotNonSabNucNum)
{
	for(int nuc = 1;nuc <= nTotNonSabNucNum;++nuc)
	{
		// NU block
		// MTR &  LSIG &  LAND  &  LDLW
		int NMT_4 = GetNonElMtNum(nuc);    //number of reactions excluding elastic
		if(NMT_4 == 0)   // NO MT array
		{
			p_vNuclides[nuc].MTRIndex.resize(110);     // maximum MT = 107 is requireed for reaction rate tally
			p_vNuclides[nuc].LAND.resize(3);     
			p_vNuclides[nuc].LAND[2]=int(p_vNuclides[nuc].XSS[GetLocOfLAND(nuc)]);  // elastic LAND
		}
		else
		{
			int L3,L6,L8,L10,NMT_5,MT_max_4,MT_max_5;
			NMT_5 = GetNonElMtNumWithNeu(nuc); 
			L3=GetLocOfMTR(nuc);
			L6=GetLocOfLSIG(nuc);
			L8=GetLocOfLAND(nuc);
			L10=GetLocOfLDLW(nuc);
			MT_max_4 = int(max(p_vNuclides[nuc].XSS[L3+NMT_4-1],p_vNuclides[nuc].XSS[L3]));
			MT_max_5 = int(max(p_vNuclides[nuc].XSS[L3+NMT_5-1],p_vNuclides[nuc].XSS[L3]));
			p_vNuclides[nuc].MTRIndex.resize(max(110,MT_max_4+1));  // need MT=107 when tally reaction rate 
			p_vNuclides[nuc].LSIG.resize(max(110,MT_max_4+1));      //  only containing NXS_5 MT actually
			p_vNuclides[nuc].LAND.resize(MT_max_5+3);               //  only containing NXS_5 MT actually
			p_vNuclides[nuc].LAND[2] = int(p_vNuclides[nuc].XSS[L8]);           //  elastic LAND
			p_vNuclides[nuc].LDLW.resize(MT_max_5+3);
			for(int i = 1;i <= NMT_4;i++)
			{
				int nMT_temp = int(p_vNuclides[nuc].XSS[L3+i-1]);
				if (nMT_temp <=0 )
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"negative MT = %d for nuclide %s \n",nMT_temp,p_vNuclides[nuc].p_ONucID.p_chIdStr);},
						Output.p_fpOutputFilePtr,CDOutput::_ERROR);
					continue;
				}
				if (nMT_temp >= p_vNuclides[nuc].MTRIndex.size())
				{
					//printf("MT %d >final MT %d in nuc %s \n",MT_temp,MT_max_4,nuc_ID[nuc]);
					p_vNuclides[nuc].MTRIndex.resize(nMT_temp+1);
					p_vNuclides[nuc].LSIG.resize(nMT_temp+1);
				}
				p_vNuclides[nuc].MTRIndex[nMT_temp] = i;
				p_vNuclides[nuc].LSIG[nMT_temp] = int(p_vNuclides[nuc].XSS[L6+i-1]);	
				if(i <= NMT_5)
				{
					p_vNuclides[nuc].LAND[nMT_temp]=int(p_vNuclides[nuc].XSS[L8+i]);	
					p_vNuclides[nuc].LDLW[nMT_temp]=int(p_vNuclides[nuc].XSS[L10+i-1]);
				}
			}
		}

		// check negative values for URR probability table, 20140317, 20141212
		// negative value may cause negative xs when use patble ( CDAceData::TreatURR() )
		// ptable will be closed if find negative value
		if (p_bUseProbTable)
		{
			int nLoc_lunr_table = GetLocOfLUNR(nuc);
			if (nLoc_lunr_table != 0)// probability table exists
			{
				int nNum_prob_erg = int(p_vNuclides[nuc].XSS[nLoc_lunr_table]);
				int nNum_prob_tab = int(p_vNuclides[nuc].XSS[nLoc_lunr_table + 1]);

				int nStart_loc = nLoc_lunr_table + 6;
				int nData_length = nNum_prob_erg*6*nNum_prob_tab;
				for (int i = 0; i < nData_length; i++)
				{
					if (p_vNuclides[nuc].XSS[nStart_loc + i] < 0)
					{
						Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"find negative value %f (xss[%d]) in probability table of nuclide %s. Ptable of this nuclide is disabled to avoid error.\n",
							p_vNuclides[nuc].XSS[nStart_loc + i], nStart_loc + i, p_vNuclides[nuc].p_ONucID.p_chIdStr);},Output.p_fpOutputFilePtr,CDOutput::_WARNING);
						p_vNuclides[nuc].JXS[23] = 0;// close flag of urr ptable, only this nuclide
						break;
					}
				}
			}
		}

		////  Process FissXSS and InelXSS ////
		int nNE = GetErgGridNum(nuc);
		p_vNuclides[nuc].FisXSS.resize(nNE+1);
		p_vNuclides[nuc].InelXSS.resize(nNE+1);

		bool bMt18Exist = false;
		for(int i = 1;i <= GetErgGridNum(nuc);++i)
		{
			p_vNuclides[nuc].FisXSS[i]=0;
			p_vNuclides[nuc].InelXSS[i]=0;
			int nloc = GetLocOfMTR(nuc)-1;
			for(int j = 1;j <= GetNonElMtNumWithNeu(nuc);j++)
			{
				int nMT_temp = int(p_vNuclides[nuc].XSS[nloc + j]);

				if(nMT_temp <= 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown MT number.\n   Nuc = %d, MT = XSS[%d] = %d.\n",
						p_vNuclides[nuc].p_nNucZaid, nloc + j, nMT_temp);},CDOutput::_WARNING);
				}

				int nIE_LOCA = GetLocOfSIG(nuc) + p_vNuclides[nuc].LSIG[nMT_temp] - 1;
				int nSIG_IE = int(p_vNuclides[nuc].XSS[nIE_LOCA]);
				int nSIG_NE = int(p_vNuclides[nuc].XSS[nIE_LOCA + 1]);		
				if((nSIG_IE+nSIG_NE-1) != nNE)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Abnormal cross-section of reaction MT=%d in nuc=%s \n",
						nMT_temp,p_vNuclides[nuc].p_ONucID.p_chIdStr);}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);

				}
				if (nMT_temp == 18)
				{
					bMt18Exist = true;
				}
				if( i >= nSIG_IE)
				{					
					if(nMT_temp!=18 && nMT_temp!=19 && nMT_temp!=20 && nMT_temp!=21 && nMT_temp!=38)
					{
						p_vNuclides[nuc].InelXSS[i] = p_vNuclides[nuc].InelXSS[i] + p_vNuclides[nuc].XSS[nIE_LOCA + 2 + i-nSIG_IE];  //∑«µØ–‘…¢…‰Ωÿ√Ê
					} 
					else
					{
						if(bMt18Exist && nMT_temp == 18)
						{
							p_vNuclides[nuc].FisXSS[i] = p_vNuclides[nuc].XSS[nIE_LOCA+2+i-nSIG_IE];
						}
						else if(!bMt18Exist && nMT_temp != 18)
						{
							p_vNuclides[nuc].FisXSS[i] = p_vNuclides[nuc].FisXSS[i]+p_vNuclides[nuc].XSS[nIE_LOCA+2+i-nSIG_IE];
						}
					}
				}
			}

			//Nuclides[nuc].XSS[NE + i] =   Nuclides[nuc].XSS[2*NE + i] +  Nuclides[nuc].XSS[3*NE + i] + Nuclides[nuc].FisXSS[i] + Nuclides[nuc].InelXSS[i];

		}
	}

	return;
}