# include "AceData.h"

void CDAceData::InterpolateSab(int nNuclide_n, int nNuclide_sab, double dEg0)
{
	unsigned int min,max;
	int nNE_el,nNE_inel;
	int nsab_n_el,nsab_n_inel;
	double dSIG_sab_el, dSIG_sab_inel,dsab_k_el,dsab_k_inel;

	/////////////// calculate the s(a,b) inelastic scattering cross section ///////////////
	nNE_inel = int(p_vNuclides[nNuclide_sab].XSS[GetLocOfSabInelErg(nNuclide_sab)]);
	min = GetLocOfSabInelErg(nNuclide_sab) + 1;
	max = GetLocOfSabInelErg(nNuclide_sab) + nNE_inel;
	CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nNuclide_sab].XSS[0],dEg0,min,max,nsab_n_inel,dsab_k_inel);

	dSIG_sab_inel = p_vNuclides[nNuclide_sab].XSS[nNE_inel + nsab_n_inel] + 
		          dsab_k_inel*(p_vNuclides[nNuclide_sab].XSS[nNE_inel + nsab_n_inel + 1] - p_vNuclides[nNuclide_sab].XSS[nNE_inel + nsab_n_inel]);


	/////////////// calculate the s(a,b) elastic scattering cross section ///////////////
	if(GetLocOfSabElErg(nNuclide_sab)==0) 
	{
		dSIG_sab_el = 0;
	}
	else
	{
		nNE_el = int(p_vNuclides[nNuclide_sab].XSS[GetLocOfSabElErg(nNuclide_sab)]);
		min = GetLocOfSabElErg(nNuclide_sab) + 1;

		if(GetSabElMode(nNuclide_sab) == 4 && dEg0 <= p_vNuclides[nNuclide_sab].XSS[min])
		{
			dSIG_sab_el = 0;
		}
		else
		{
			max = GetLocOfSabElErg(nNuclide_sab) + nNE_el;
			CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nNuclide_sab].XSS[0],dEg0,min,max,nsab_n_el,dsab_k_el);
			if(GetSabElMode(nNuclide_sab) == 4)
			{
				dSIG_sab_el = p_vNuclides[nNuclide_sab].XSS[nNE_el + nsab_n_el]/dEg0;
			}
			else
			{
				dSIG_sab_el = p_vNuclides[nNuclide_sab].XSS[nNE_el + nsab_n_el] +
					dsab_k_el*(p_vNuclides[nNuclide_sab].XSS[nNE_el + nsab_n_el + 1] - p_vNuclides[nNuclide_sab].XSS[nNE_el + nsab_n_el]);
			}
		}
	}

	//总截面 = 总吸收from c  +   散射from sab

	//if(Nuclides[nuclide_sab].ProbTableFlag==1){printf("TreatURR conflict with sab \n");}
	int nn = p_vNuclides[nNuclide_n].p_nNucInterpPos;
	double kk = p_vNuclides[nNuclide_n].p_dNucInterpFrac;
	int NE = GetErgGridNum(nNuclide_n);
	p_vNuclides[nNuclide_n].p_ONucCs.p_dAbs = p_vNuclides[nNuclide_n].XSS[nn + 2*NE] + kk*(p_vNuclides[nNuclide_n].XSS[nn + 2*NE + 1] - p_vNuclides[nNuclide_n].XSS[nn + 2*NE]);
	p_vNuclides[nNuclide_n].p_ONucCs.p_dTot = p_vNuclides[nNuclide_n].p_ONucCs.p_dAbs + dSIG_sab_el + dSIG_sab_inel;
	p_vNuclides[nNuclide_n].p_ONucCs.p_dEl = dSIG_sab_el;
	p_vNuclides[nNuclide_n].p_ONucCs.p_dInel = dSIG_sab_inel;
	p_vNuclides[nNuclide_n].p_ONucCs.p_dFis = 0;

	return;
}