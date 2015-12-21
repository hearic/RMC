# include "AceData.h"

void CDAceData::GetNucTotFisCs(int nNuc, int nSabNuc, double dEg0, double dCell_T, bool bUseProbTable)
{
	enum eAdjustCsByPTOrNot{ NotAdjustCsByPT = 0, AdjustCsByPT = 1};

	int nNE = GetErgGridNum(nNuc);

	/////////////////////// Cal NU /////////////////////////////
    p_vNuclides[nNuc].p_ONucCs.p_dNu = GetTotalNu(dEg0,nNuc);

	//////// binary search for Interpolation parameters ///////// 
	int min,max;
	if(p_bUseErgBinMap)
	{
		GetErgBinMapPtr(nNuc,min,max);
		//AceErgBinMap[nuc].GetBinMapPtr(ErgBinMapIndex,min,max);
	}
	else
	{
		min = 1 ;
		max = GetErgGridNum(nNuc);
	}
	CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nNuc].XSS[0],dEg0,min,max,p_vNuclides[nNuc].p_nNucInterpPos,p_vNuclides[nNuc].p_dNucInterpFrac);
	//goto go_urs;

	if(bUseProbTable)
	{
		p_vNuclides[nNuc].p_nProbTableFlag = NotAdjustCsByPT;
		int nL = GetLocOfLUNR(nNuc);
		if(nL!=0)// probability tables exist
		{
			if( dEg0 > p_vNuclides[nNuc].XSS[nL + 6] && dEg0 < p_vNuclides[nNuc].XSS[nL + 5 + int(p_vNuclides[nNuc].XSS[nL])]) 
			{
				//double lower_energy =  Nuclides[nuc].XSS[L + 6];
				//double upper_energy = Nuclides[nuc].XSS[L + 5 + int(Nuclides[nuc].XSS[L])];
				TreatURR(nNuc,dEg0);
				p_vNuclides[nNuc].p_nProbTableFlag = AdjustCsByPT;
				return;
			}
		}   
	}

//tot_cs:
	if( nSabNuc == 0 )   
	{	
		p_vNuclides[nNuc].p_ONucCs.p_dTot = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].XSS[0],p_vNuclides[nNuc].p_nNucInterpPos + nNE,p_vNuclides[nNuc].p_dNucInterpFrac); 
		p_vNuclides[nNuc].p_ONucCs.p_dFis = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].FisXSS[0],p_vNuclides[nNuc].p_nNucInterpPos,p_vNuclides[nNuc].p_dNucInterpFrac); 

		DpplrBrdnNuc_totcs(nNuc, dCell_T,dEg0);
		return;
	}

	////////////////  sab case ///////////////////
	if(nSabNuc != 0)
	{
		InterpolateSab(nNuc,nSabNuc,dEg0);
		//sabflag = 1;
		return;
	}

	
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"fail to calculate total&fission xs of nuclide %d.\n",nNuc);}, 
		Output.p_fpOutputFilePtr, CDOutput::_WARNING);

	return;
}

void CDAceData::DpplrBrdnNuc_totcs(int nNuc, double dCell_T, double dEg0)
{
	double a,b,a2,f;
	int bi,j;
	if( p_vNuclides[nNuc].p_dNucBroadTmp == 0.  && p_vNuclides[nNuc].p_dNucBroadTmp != dCell_T)  //th_ad:  thermal adjustment
	{
		a2 = p_vNuclides[nNuc].p_dNucAtomWgt*dEg0;
		if( a2 <= 500.*dCell_T ) 
		{
			if( a2 >= 4.*dCell_T ) 	{f = .5*dCell_T/a2;}
			else
			{
				a = sqrt(a2/dCell_T);
				b = 25.*a;
				bi = int(b);
				f = (p_dThermFunc[bi] + (b-bi)*(p_dThermFunc[bi+1] - p_dThermFunc[bi]))/a - 1.;
			}
			j = p_vNuclides[nNuc].p_nNucInterpPos + 3*GetErgGridNum(nNuc);
			p_vNuclides[nNuc].p_ONucCs.p_dTot = p_vNuclides[nNuc].p_ONucCs.p_dTot + f*(p_vNuclides[nNuc].XSS[j] + p_vNuclides[nNuc].p_dNucInterpFrac*(p_vNuclides[nNuc].XSS[j+1]-p_vNuclides[nNuc].XSS[j]));
		}
	}
}