# include "AceData.h"

void CDAceData::TreatURR(int nNuc, double dEg0)  // Treat Unresolved Resonance Range
{
	double dEl_factor,dFis_factor,dC_factor,dInterp_value,dSmooth_total,sInel_balance,dAbs_balance;
	int nIdx,nIdx_s,nIdx_e,nNum_e;

	int nNum_erg_grid  = GetErgGridNum(nNuc);
	int nLoc_lunr_table  = GetLocOfLUNR(nNuc);
	int nNum_prob_table_incident_erg = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table]);
	int nLoc_incident_erg = nLoc_lunr_table + 6;
	int nLength_table = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table+1]);
	int nIncident_erg_idx = nLoc_incident_erg + 1;
	
	for(;;)
	{
		if( dEg0 < p_vNuclides[nNuc].XSS[nIncident_erg_idx] )  
		{
			break;
		}
		nIncident_erg_idx = nIncident_erg_idx+1;
	}
	
	//// interpolate between prob tables
	double dRatio  = (dEg0 - p_vNuclides[nNuc].XSS[nIncident_erg_idx-1])/(p_vNuclides[nNuc].XSS[nIncident_erg_idx] - p_vNuclides[nNuc].XSS[nIncident_erg_idx-1]);
	int nErg_idx = nIncident_erg_idx - nLoc_incident_erg;
	int nLoc_prob_table = nLoc_incident_erg + nNum_prob_table_incident_erg + (nErg_idx-1)*nLength_table*6;    // 
	int nj1 = nLoc_prob_table;
	int nj2 = nLoc_prob_table + nLength_table*6;
	
	double dRand_num = ORNG.Rand();
	for(;;)
	{
		if(p_vNuclides[nNuc].XSS[nj1] >= dRand_num){break;}
		nj1 = nj1 + 1;
	}
	for(;;)
	{
		if(p_vNuclides[nNuc].XSS[nj2] >= dRand_num){break;}
		nj2 = nj2 + 1;
	}
	if( nj1 - nLoc_prob_table + 1 > nLength_table || nj2 - (nLoc_prob_table + nLength_table*6) + 1 > nLength_table) 
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"prob table not normed in nuc %s.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
		return;
	}


	///Interpolation parameter between tables  =2 lin-lin; =5 log-log
	int nDistribution_type = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table+2]); 
	if( nDistribution_type!=5 || dRatio <= 0. || dRatio >= 1. ) //// lin-lin interpolation.
	{
		p_vNuclides[nNuc].p_ONucCs.p_dAbs = p_vNuclides[nNuc].XSS[nj1 + nLength_table*4] + dRatio*(p_vNuclides[nNuc].XSS[nj2 + nLength_table*4] - p_vNuclides[nNuc].XSS[nj1 + nLength_table*4]) ; //absorption (capture only)
		p_vNuclides[nNuc].p_ONucCs.p_dEl  = p_vNuclides[nNuc].XSS[nj1 + nLength_table*2] + dRatio*(p_vNuclides[nNuc].XSS[nj2 + nLength_table*2] - p_vNuclides[nNuc].XSS[nj1 + nLength_table*2]) ;  //elastic
		p_vNuclides[nNuc].p_ONucCs.p_dFis = p_vNuclides[nNuc].XSS[nj1 + nLength_table*3] + dRatio*(p_vNuclides[nNuc].XSS[nj2 + nLength_table*3] - p_vNuclides[nNuc].XSS[nj1 + nLength_table*3]) ; //fission (18 only)
		// heating no. + nLength_table*5
	}
	else if(nDistribution_type == 5)// log-log interpolation. 
	{
		//xs data maybe negative, resulting Nan(Not a Number), add check when reading
		double g = log(dEg0/p_vNuclides[nNuc].XSS[nIncident_erg_idx-1])/log(p_vNuclides[nNuc].XSS[nIncident_erg_idx]/p_vNuclides[nNuc].XSS[nIncident_erg_idx-1]);
		int i = nLength_table*4;
		p_vNuclides[nNuc].p_ONucCs.p_dAbs = 0;
		if( p_vNuclides[nNuc].XSS[nj1 + i] != 0 )
		{
			p_vNuclides[nNuc].p_ONucCs.p_dAbs = exp(log(p_vNuclides[nNuc].XSS[nj1+i]) + g*log(p_vNuclides[nNuc].XSS[nj2+i]/p_vNuclides[nNuc].XSS[nj1+i]));
		}
		i =  nLength_table*2;
		p_vNuclides[nNuc].p_ONucCs.p_dEl = exp(log(p_vNuclides[nNuc].XSS[nj1+i]) + g*log(p_vNuclides[nNuc].XSS[nj2+i]/p_vNuclides[nNuc].XSS[nj1+i]));
		i = nLength_table*3;
		p_vNuclides[nNuc].p_ONucCs.p_dFis = 0;
		if( p_vNuclides[nNuc].XSS[nj1 + i] != 0 )
		{ 
			p_vNuclides[nNuc].p_ONucCs.p_dFis = exp(log(p_vNuclides[nNuc].XSS[nj1+i]) + g*log(p_vNuclides[nNuc].XSS[nj2+i]/p_vNuclides[nNuc].XSS[nj1+i]));
		}
	}
	else
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown interpolation type in pTable of nuc %s.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
	}
                                                                                                                                                                                                                                             	
	// calculate cross   sections
	int nFactor_flag = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table+5]);  // Factors flag
	int nInel_competition_flag = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table+3]);  // Inelastic competition flag
	int nOther_abs_flag = int(p_vNuclides[nNuc].XSS[nLoc_lunr_table+4]);  // Other absorption flag
	if( nFactor_flag!=0 || nInel_competition_flag == 0 ) 
	{
		int nInterp_pos = p_vNuclides[nNuc].p_nNucInterpPos;
		dEl_factor = p_vNuclides[nNuc].XSS[nInterp_pos+3*nNum_erg_grid] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nInterp_pos+3*nNum_erg_grid+1] - p_vNuclides[nNuc].XSS[nInterp_pos+3*nNum_erg_grid]);
		dFis_factor = p_vNuclides[nNuc].FisXSS[nInterp_pos] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].FisXSS[nInterp_pos+1] - p_vNuclides[nNuc].FisXSS[nInterp_pos]);
		dFis_factor = 0;
		int i = GetLocOfFIS(nNuc);
		if( i!=0 ) 
		{
			int nInterp_i = p_vNuclides[nNuc].p_nNucInterpPos-int(p_vNuclides[nNuc].XSS[i])+1;
			if( nInterp_i>=1 ) 
			{
				nInterp_i = nInterp_i + i + 1;
				dFis_factor = p_vNuclides[nNuc].XSS[nInterp_i]+p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nInterp_i+1]-p_vNuclides[nNuc].XSS[nInterp_i]);
			}
		}
	}
	
	/////// get other absorption factor.
	if( nFactor_flag !=0  || nOther_abs_flag == 0. ) 
	{
		dC_factor = 0;
		int i = GetLocOfMTR(nNuc)-1;  // MTR block
		for(nIdx = 1;nIdx <= GetNonElMtNum(nNuc);nIdx++)
		{
			if( int(p_vNuclides[nNuc].XSS[i+nIdx])==102 ) 
			{
				nIdx_s = GetLocOfSIG(nNuc)+int(p_vNuclides[nNuc].XSS[GetLocOfLSIG(nNuc)+nIdx-1]); // JXS(7)+LOCA : Location of NE
				nIdx_e = int(p_vNuclides[nNuc].XSS[nIdx_s-1]);    //  int(p_vNuclides[nuc].XSS[iq-1])  :IE
				nNum_e = int(p_vNuclides[nNuc].XSS[nIdx_s]);
				int ic = p_vNuclides[nNuc].p_nNucInterpPos+1-nIdx_e;
				if( ic>=1 && ic<=nNum_e ) 
				{
					if( ic==nNum_e)
					{
						dC_factor=p_vNuclides[nNuc].XSS[nIdx_s+nNum_e];
					}
					else
					{
						dC_factor=p_vNuclides[nNuc].XSS[nIdx_s+ic]+p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nIdx_s+ic+1]-p_vNuclides[nNuc].XSS[nIdx_s+ic]);
					}
				}
				break;				
			}
		} 
		
		
		if(nFactor_flag!=0) 
		{
			p_vNuclides[nNuc].p_ONucCs.p_dEl = p_vNuclides[nNuc].p_ONucCs.p_dEl*dEl_factor;      // elastic
			p_vNuclides[nNuc].p_ONucCs.p_dFis = p_vNuclides[nNuc].p_ONucCs.p_dFis*dFis_factor;    // fission   (18 or 19/20/21/38)
			p_vNuclides[nNuc].p_ONucCs.p_dAbs = p_vNuclides[nNuc].p_ONucCs.p_dAbs*dC_factor;    // absorption  (102 only)
		}
	}
	
	// calculate prob table total
	// p_vNuclides[nuc].p_ONucCs.p_dTot = p_vNuclides[nuc].p_ONucCs.p_dEl+p_vNuclides[nuc].p_ONucCs.p_dFis+p_vNuclides[nuc].p_ONucCs.p_dAbs;  // near total, excluding inelastic and other absorption
    // SIG_interp[nuc][102] = p_vNuclides[nuc].p_ONucCs.p_dAbs;  //  absorption  (102 only)
	
	
	sInel_balance = 0;
	dAbs_balance = 0;
	
	
	if( nInel_competition_flag == 0 || nOther_abs_flag==0 ) 
	{
		int ii = p_vNuclides[nNuc].p_nNucInterpPos;
		dInterp_value = p_vNuclides[nNuc].XSS[ii+2*nNum_erg_grid] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[ii+2*nNum_erg_grid+1] - p_vNuclides[nNuc].XSS[ii+2*nNum_erg_grid]);
	}
	
	if( nInel_competition_flag > 0 ) 
	{
		// ILF>0.  calculate inelastic 
		int i = GetLocOfMTR(nNuc) - 1 ;
		for(nIdx = 1;nIdx <= GetNonElMtNum(nNuc);++nIdx)
		{
			if( int(p_vNuclides[nNuc].XSS[i+nIdx]) == nInel_competition_flag )  
			{
				break;
			}
		} 
		if(nIdx > GetNonElMtNum(nNuc))
		{
			
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"total inelastic MT is not in table. \n");},CDOutput::_WARNING);
		}

		nIdx_s = GetLocOfSIG(nNuc) + int(p_vNuclides[nNuc].XSS[GetLocOfLSIG(nNuc)+nIdx-1]);
		nIdx_e = int(p_vNuclides[nNuc].XSS[nIdx_s-1]);
		nNum_e = int(p_vNuclides[nNuc].XSS[nIdx_s]);
		int idx_c = p_vNuclides[nNuc].p_nNucInterpPos+1-nIdx_e;
		if( idx_c >= 1 && idx_c <= nNum_e ) 
		{
			if( idx_c == nNum_e)
			{
				sInel_balance = p_vNuclides[nNuc].XSS[nIdx_s+nNum_e];
			}
			else
			{
				sInel_balance = p_vNuclides[nNuc].XSS[nIdx_s+idx_c] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nIdx_s+idx_c+1]-p_vNuclides[nNuc].XSS[nIdx_s+idx_c]);
			}
		}
	}
	// ILF=0
	else if(nInel_competition_flag == 0) 
	{

		int nIdx_c = p_vNuclides[nNuc].p_nNucInterpPos;
		dSmooth_total = p_vNuclides[nNuc].XSS[nIdx_c+nNum_erg_grid] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nIdx_c+nNum_erg_grid+1]-p_vNuclides[nNuc].XSS[nIdx_c+nNum_erg_grid]);
		sInel_balance = dSmooth_total - dEl_factor - dFis_factor - dInterp_value;
	}
	//p_vNuclides[nuc].p_ONucCs.p_dInel=r1;  // 2010-11-04

	
	// check flag for other absorption to include in abs and total.
	// obtain other absorption by smooth (abs-102).
	if( nOther_abs_flag > 0 )  // jxs(23,iex)+4 contains the mt of other absorption.
	{
		int i = GetLocOfMTR(nNuc)-1 ;
		for(nIdx = 1;nIdx <= GetNonElMtNum(nNuc); ++nIdx)
		{
			if( int(p_vNuclides[nNuc].XSS[i+nIdx])==nOther_abs_flag ) 
			{
				break;
			}
		}
		if(nIdx > GetNonElMtNum(nNuc))
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"other absorption mt not in table. \n");},CDOutput::_WARNING);
		}

	    nIdx_s = GetLocOfSIG(nNuc)+int(p_vNuclides[nNuc].XSS[GetLocOfLSIG(nNuc)+nIdx-1]);
		nIdx_e = int(p_vNuclides[nNuc].XSS[nIdx_s-1]);
		nNum_e = int(p_vNuclides[nNuc].XSS[nIdx_s]);
		int idx_c = p_vNuclides[nNuc].p_nNucInterpPos+1-int(p_vNuclides[nNuc].XSS[nIdx_s-1]);
		if( idx_c>=1 && idx_c<=nNum_e ) 
		{
			if( idx_c==nNum_e){dAbs_balance = p_vNuclides[nNuc].XSS[nIdx_s+nNum_e];}
			else{dAbs_balance = p_vNuclides[nNuc].XSS[nIdx_s+idx_c]+p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[nIdx_s+idx_c+1]-p_vNuclides[nNuc].XSS[nIdx_s+idx_c]);}
		}
	}
	else if(nOther_abs_flag == 0) 
	{
		dAbs_balance = dInterp_value - dC_factor;
	}
	
	// increment cross sections for inelastic and other absorption.
	p_vNuclides[nNuc].p_ONucCs.p_dAbs = p_vNuclides[nNuc].p_ONucCs.p_dAbs + dAbs_balance ;    // absorption=capture +  other absorption
	if (p_vNuclides[nNuc].p_ONucCs.p_dAbs < 0)
	{
		int NE = GetErgGridNum(nNuc);
		p_vNuclides[nNuc].p_ONucCs.p_dAbs = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].XSS[0],p_vNuclides[nNuc].p_nNucInterpPos + 2*NE,p_vNuclides[nNuc].p_dNucInterpFrac);  // absorption, E0
	}

	p_vNuclides[nNuc].p_ONucCs.p_dInel = sInel_balance;
	p_vNuclides[nNuc].p_ONucCs.p_dTot =  p_vNuclides[nNuc].p_ONucCs.p_dAbs + p_vNuclides[nNuc].p_ONucCs.p_dEl + p_vNuclides[nNuc].p_ONucCs.p_dInel + p_vNuclides[nNuc].p_ONucCs.p_dFis;

	// Finally, get  total,absorption,elastic,fission
	
	return;

}