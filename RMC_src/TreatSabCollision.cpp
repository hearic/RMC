# include "NeutronTransport.h"

void CDAceData::TreatSabColliType(int nSabColliNuc, double dSIG_sab_el, double dSIG_sab_inel,double dEin,double dDirectin[3], double &dExitErgInLab,double dDirectout[3],CDNeutronTransport &cNeutronTransport)
{
	int sab_n_el, sab_n_inel;
	double sab_k_el, sab_k_inel;
	double dMuInLab;

	////////////////// elastic scattering case.  JXS(4) = 0 : without elastic scattering ////////////////
	if(GetLocOfSabElErg(nSabColliNuc)!=0 && ORNG.Rand()*(dSIG_sab_el + dSIG_sab_inel) > dSIG_sab_inel )  
	{
		dExitErgInLab = dEin;  //存疑

		int NE_el = int(p_vNuclides[nSabColliNuc].XSS[GetLocOfSabElErg(nSabColliNuc)]);
		int min = GetLocOfSabElErg(nSabColliNuc) + 1;
		int max = GetLocOfSabElErg(nSabColliNuc) + NE_el;
		double f_min = p_vNuclides[nSabColliNuc].XSS[min];
		double f_max = p_vNuclides[nSabColliNuc].XSS[max];

		CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nSabColliNuc].XSS[0],dEin,min,max,sab_n_el,sab_k_el);   //// sab_n_el never reach max
		sab_n_el = sab_n_el - GetLocOfSabElErg(nSabColliNuc);
		int LN = GetLocOfSabElMu(nSabColliNuc) + (sab_n_el - 1)*(abs(GetSabElDimPara(nSabColliNuc)) + 1);
		int N_el_mu = GetSabElDimPara(nSabColliNuc) + 1; 
		int Loc;
		int nMode_el = GetSabElMode(nSabColliNuc);
		if( nMode_el == 2 )     ////// equally-probable angle bins.  等概率余弦表
		{
			if( ORNG.Rand() <= sab_k_el ) 
			{
				LN = LN + N_el_mu;
			} 
			double ksi = (N_el_mu - 1)*ORNG.Rand() + 1;
			int Loc = int(LN + ksi);
			dMuInLab = p_vNuclides[nSabColliNuc].XSS[Loc] + (p_vNuclides[nSabColliNuc].XSS[Loc - 1] - p_vNuclides[nSabColliNuc].XSS[Loc])*(ksi - int(ksi));
		}
		else if( nMode_el == 3 )  /////// qually-probable discrete angles.  能/角耦合的等概率离散角  （the only Mode_inel）
		{
			Loc = LN + int(N_el_mu*ORNG.Rand());
			dMuInLab = p_vNuclides[nSabColliNuc].XSS[Loc] + sab_k_el*(p_vNuclides[nSabColliNuc].XSS[Loc + N_el_mu] - p_vNuclides[nSabColliNuc].XSS[Loc]);
		}
		else if( nMode_el == 4 )  ////// 相干弹性散射
		{
			min = GetLocOfSabElXs(nSabColliNuc);
			max = GetLocOfSabElXs(nSabColliNuc) + sab_n_el;
			double pi = p_vNuclides[nSabColliNuc].XSS[max]*ORNG.Rand();
			//double f_min = Nuclides[SabColliNuc].XSS[min];
			//double f_max = Nuclides[SabColliNuc].XSS[max];
			CDGlobeFun::GetIntpltPos(&p_vNuclides[nSabColliNuc].XSS[0],pi,min,max,Loc);
			Loc = Loc + 1 - int(p_vNuclides[nSabColliNuc].XSS[GetLocOfSabElErg(nSabColliNuc)]);
			dMuInLab = 1 - 2.0 * p_vNuclides[nSabColliNuc].XSS[Loc]/dEin;
		}	 
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect elastic scattering mode(%d) in sab collision.\n",nMode_el);},CDOutput::_WARNING);
		}
	}    

	////////////////// inelastic scattering case.  JXS(4) = 0 : without elastic scattering ////////////////
	else
	{
		int NE_inel = int(p_vNuclides[nSabColliNuc].XSS[GetLocOfSabInelErg(nSabColliNuc)] + 0.5);
		int min = GetLocOfSabInelErg(nSabColliNuc) + 1;
		int max = GetLocOfSabInelErg(nSabColliNuc) + NE_inel;
		CDGlobeFun::GetIntpltPosFr(&p_vNuclides[nSabColliNuc].XSS[0],dEin,min,max,sab_n_inel,sab_k_inel);
		sab_n_inel = sab_n_inel - GetLocOfSabInelErg(nSabColliNuc);

		int LE;
		if( GetSabSecErgMode(nSabColliNuc) ==0 )
		{
			LE = int(ORNG.Rand() * GetSabInelEoutNum(nSabColliNuc));
		}   
		else
		{
			double rr = ORNG.Rand()*(GetSabInelEoutNum(nSabColliNuc) - 3);
			if(rr >= 1.0) 
			{
				LE = int(rr + 1);
			}      
			else
			{
				if(rr >= 0.5) 
				{
					LE = GetSabInelEoutNum(nSabColliNuc) - 2;
					if(rr < 0.6)
					{
						LE = LE + 1;
					}
				}            
				else
				{
					LE = 1;
					if(rr < 0.1)
					{
						LE = 0;
					}
				}
			}
		}
		int N_inel_mu = GetSabInelDimPara(nSabColliNuc) + 1;
		int LN_offset = GetSabInelEoutNum(nSabColliNuc)*(N_inel_mu + 1); // 插值点位置偏移
		int LN = (sab_n_inel - 1)*LN_offset + GetLocOfSabInelErgMu(nSabColliNuc) + LE*(N_inel_mu + 1);
		dExitErgInLab = p_vNuclides[nSabColliNuc].XSS[LN] + sab_k_inel*(p_vNuclides[nSabColliNuc].XSS[LN + LN_offset] - p_vNuclides[nSabColliNuc].XSS[LN]);
		if(dExitErgInLab <= 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit energy in sab collision is out of range.\n");},CDOutput::_WARNING);
			dExitErgInLab = cNeutronTransport.p_dEg0CutOff;
		}
		else if(dExitErgInLab <= 1.0E-11)
		{
			if(dExitErgInLab > cNeutronTransport.p_dEg0CutOff)
			{
				dExitErgInLab = 1.0E-11;
			}
		}



		LN = LN + 1;
		int Mode_inel = GetSabInelMode(nSabColliNuc);
		if( Mode_inel == 2 )  // equally-probable angle bins.  等概率余弦表
		{
			if( ORNG.Rand() <= sab_k_inel ) 
			{
				LN = LN + LN_offset;
			}  
			double ksi = (N_inel_mu - 1) * ORNG.Rand() + 1;
			int Loc = int(LN + ksi);
			dMuInLab = p_vNuclides[nSabColliNuc].XSS[Loc] + (p_vNuclides[nSabColliNuc].XSS[Loc - 1] - p_vNuclides[nSabColliNuc].XSS[Loc])*(ksi - int(ksi));
		} 
		else if( Mode_inel == 3 ) // qually-probable discrete angles.  能/角耦合的等概率离散角  （the only Mode_inel）
		{
			int Loc = LN + int(N_inel_mu*ORNG.Rand());
			dMuInLab = p_vNuclides[nSabColliNuc].XSS[Loc] + sab_k_inel*(p_vNuclides[nSabColliNuc].XSS[Loc + LN_offset] - p_vNuclides[nSabColliNuc].XSS[Loc]);
		} 
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect inelastic scattering mode(%d) in sab collision.\n",Mode_inel);},CDOutput::_WARNING);
		}
	}


	//////////// check outgoing angle and energy //////////////////////
	dMuInLab = std::min( 1.00, dMuInLab );
	dMuInLab = std::max( -1.00, dMuInLab );
	cNeutronTransport.RotateDirection(dMuInLab,dDirectin,dDirectout);



	return;

}