# include "AceData.h"

void CDAceData::DpplerBrdnNucAbsScatt(int nNuc, double dEg0)
{
	if( p_vNuclides[nNuc].p_dNucAtomWgt*dEg0 <= 500.*p_vNuclides[nNuc].p_dNucBroadTmp )   // thermal adjustment
	{
		double a2,a,b;
		int bi;
		a2 = p_vNuclides[nNuc].p_dNucAtomWgt*dEg0/p_vNuclides[nNuc].p_dNucBroadTmp;
		if( a2 >= 4.0){p_vNuclides[nNuc].p_ONucCs.p_dEl=p_vNuclides[nNuc].p_ONucCs.p_dEl*a2/(a2+0.5);}
		else
		{
			a = sqrt(a2);
			b = 25.0*a;
			bi = int(b);
			p_vNuclides[nNuc].p_ONucCs.p_dEl = p_vNuclides[nNuc].p_ONucCs.p_dEl*a/(p_dThermFunc[bi]+(b-bi)*(p_dThermFunc[bi+1]-p_dThermFunc[bi]));
		}
	}
}

void CDAceData::GetNucAbsScattCs(int nNuc,double dEg0,int nInterpPos0,double dInterpFrac0, int nInterpPos,double dInterpFrac)
{
	////// URR case: NucInterpCs[CurrentNUCLIDE][0,1,2,3,4] have been calculated in subroutine "TreatURR"/////
	if(p_vNuclides[nNuc].p_nProbTableFlag==1) 
	{
		return;
	}

	///////// Norma case: interpolate SIG (a,el,inel) //////////
	int nNE = GetErgGridNum(nNuc);
	p_vNuclides[nNuc].p_ONucCs.p_dAbs = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].XSS[0],nInterpPos0 + 2*nNE,dInterpFrac0);  // absorption, E0
	p_vNuclides[nNuc].p_ONucCs.p_dEl = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].XSS[0],nInterpPos+3*nNE,dInterpFrac);       // elastic, Er
	p_vNuclides[nNuc].p_ONucCs.p_dInel = CDGlobeFun::IntpltByPosFr(&p_vNuclides[nNuc].InelXSS[0],nInterpPos,dInterpFrac);      // inelastic, Er
	//NucCs[nuc].Fis=InterpolateCs(FisXSS[nuc],InterpPos0,InterpFrac0);    // fission, E0
		
	DpplerBrdnNucAbsScatt(nNuc, dEg0);

	return;
}

