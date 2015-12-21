# include "AceData.h"
# include "Criticality.h"

const double PI = 3.1415926536;
const double ZeroErg = 1.0E-25;
const int MaxIter = 100000;

void CDAceData::ReactByLaws(int nNuc,int nMT,int nLawType, int LDAT,double dEin, double &dExitErgInCm,double &dMuInCm)
{
	int NR, NE, NET, LN;
	int nPos;
	double dFrac;

	int nIterCount = 0 ;

	if(nLawType == 1) // TABULAR EQUIPROBABLE ENERGY BINS
	{
		InterpolateXssTab(dEin,nNuc,LDAT,nPos,dFrac,NR,NE);
		LN = 2*NR + NE + 2 ;
		NET = int(p_vNuclides[nNuc].XSS[LDAT + LN]);

		//// Sample outgoing energy bin 
		double ksi1 = ORNG.Rand();
		int k = 1 + int(ksi1 * NET + 1);

		//// calculate E_1 and E_K 
		int LC = LDAT + LN + NR + NET * (nPos-1);
		double E_i_1 = p_vNuclides[nNuc].XSS[LC + 1];
		double E_i_K = p_vNuclides[nNuc].XSS[LC + NET];
		LC = LC + NET;
		double E_i1_1 = p_vNuclides[nNuc].XSS[LC + 1];
		double E_i1_K = p_vNuclides[nNuc].XSS[LC + NET];
		double E_1 = E_i_1 + dFrac*(E_i1_1 - E_i_1);
		double E_K = E_i_K + dFrac*(E_i1_K - E_i_K);

		//// Sample outgoing table for incoming energy E_i and E_(i+1) 
		double ksi3 = ORNG.Rand();
		int nPos_smpl = ksi3 < dFrac ? nPos + 1 : nPos;

		//// Calculate E_l_k, E_l_k+1 and E' 
		LC = LDAT + LN + (nPos_smpl - 1)*NET;
		double E_l_k  = p_vNuclides[nNuc].XSS[LC + k];
		double E_l_k1 = p_vNuclides[nNuc].XSS[LC + k + 1];
		double ksi2 = ORNG.Rand();
		double E_pie = E_l_k + ksi2*(E_l_k1 - E_l_k);

		//// scaled interpolation between tables
		if(nPos_smpl == nPos)
		{
			dExitErgInCm = E_1 + (E_pie - E_i_1)*(E_K - E_1)/(E_i_K - E_i_1);
		}
		else
		{
			dExitErgInCm = E_1 + (E_pie - E_i1_1)*(E_K - E_1)/(E_i1_K - E_i1_1);
		}
	}


	else if(nLawType == 3)  // INELASTIC LEVEL SCATTERING
	{
		dExitErgInCm = p_vNuclides[nNuc].XSS[LDAT+1] * (dEin - p_vNuclides[nNuc].XSS[LDAT]);  //ExitErgInCm
	}


	else if(nLawType == 5) // GENERAL EVAPORATION SPECTRUM
	{
		NR = int(p_vNuclides[nNuc].XSS[LDAT]);
		NE = int(p_vNuclides[nNuc].XSS[LDAT + 2*NR + 1]);
		LN = 2*(NR + NE + 1);
		double TI = GetErgFuncValue(nNuc,LDAT,dEin);
		int NET = int(p_vNuclides[nNuc].XSS[LDAT + LN]);
		int  i = LDAT + LN + 1 + int(ORNG.Rand()*(NET-1));
		dExitErgInCm = TI*(p_vNuclides[nNuc].XSS[i]+ORNG.Rand()*(p_vNuclides[nNuc].XSS[i+1]-p_vNuclides[nNuc].XSS[i]));
	}


	else if(nLawType == 7) // MAXWELL FISSION SPECTRUM
	{
		NR = int(p_vNuclides[nNuc].XSS[LDAT]);
		NE = int(p_vNuclides[nNuc].XSS[LDAT + 2*NR + 1]);
		double T = GetErgFuncValue(nNuc,LDAT,dEin);
		double U = p_vNuclides[nNuc].XSS[LDAT + 2*( NR + NE + 1)];
		double CalTemp = dEin - U;
		if(CalTemp > 0 )
		{
			nIterCount = 0 ;
			for(;;)
			{
				dExitErgInCm = SampleMaxwell(T);
				if (dExitErgInCm <= CalTemp)
				{
					break;
				}
				if((nIterCount++) >= MaxIter)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many samples. Nuc=%s, LawType=%d, Ein=%f, U=%f, T=%f\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr,nLawType,dEin,U,T);},CDOutput::_WARNING);
				}
			}
		}
		else
		{
			//Output.OutputMessage([&](){sprintf(Output.PrintStr,"Waring: Ein %f < U %f.Nuc=%s, LawType=%d!\n",Ein,U, Nuclides[nuc].NucID,LawType);},CD_Output::_WARNING);
			dExitErgInCm = ZeroErg;
		}
	}


	else if(nLawType == 9) // EVAPORATION SPECTRUM
	{
		NR = int(p_vNuclides[nNuc].XSS[LDAT]);
		NE = int(p_vNuclides[nNuc].XSS[LDAT + 2*NR + 1]);
		LN = 2*(NR+NE+1);
		double T = GetErgFuncValue(nNuc,LDAT,dEin);
		double U = p_vNuclides[nNuc].XSS[LDAT+LN];
		double dCalcTemp = dEin - U ;
		if(dCalcTemp > 0)
		{
			nIterCount = 0;
			for(;;)
			{
				double ksi1 = ORNG.Rand();
				double ksi2 = ORNG.Rand();
				dExitErgInCm = -T*log(ksi1*ksi2);
				if(dExitErgInCm <= dCalcTemp)
				{
					break;
				}
				//if((IterCount++) >= MaxIter)
				//{
				//	Output.OutputMessage([&](){sprintf(Output.PrintStr,"Waring: too many samples. Nuc=%s, LawType=%d, Ein=%f, U=%f, T=%f\n",Nuclides[nuc].NucID.IdStr,LawType,Ein,U,T);},CD_Output::_WARNING);
				//}
			}
		}
		else
		{
			//Output.OutputMessage([&](){sprintf(Output.PrintStr,"Waring: Ein %f < U %f.Nuc=%s, LawType=%d!\n",Ein,U, Nuclides[nuc].NucID,LawType);},CD_Output::_WARNING);
			dExitErgInCm = ZeroErg;
		}
	}


	else if(nLawType == 11) // ENERGY-DEPENDENT WATT SPECTRUM
	{
		int NR1 = int(p_vNuclides[nNuc].XSS[LDAT]);
		int NE1 = int(p_vNuclides[nNuc].XSS[LDAT + 2*NR1 + 1]);
		int LN1 = 2*(NR1 + NE1 + 1);
		double a = GetErgFuncValue(nNuc,LDAT,dEin);  // watt parameter a
		double b = GetErgFuncValue(nNuc,LDAT + LN1,dEin); // watt parameter b

		int NR2 = int(p_vNuclides[nNuc].XSS[LDAT + LN1]);
		int NE2 = int(p_vNuclides[nNuc].XSS[LDAT + LN1 + 2*NR2 + 1]);
		int LN2 = 2 + 2*(NR2 + NE2) ;	
		double U = p_vNuclides[nNuc].XSS[LDAT + LN1 + LN2]; // restriction energy
		double dCalTemp = dEin - U;
		if(dCalTemp > 0 )
		{
			nIterCount = 0;
			for(;;)
			{
				dExitErgInCm = SampleWatt(a,b);
				if(dExitErgInCm <= dCalTemp)
				{
					break;
				}
				if((nIterCount++) >= MaxIter)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many samples of LawType %d!\n",nLawType);},CDOutput::_WARNING);
				}
			}
		}
		else
		{
			//Output.OutputMessage([&](){sprintf(Output.PrintStr,"Waring: Ein %f < U %f.Nuc=%s, LawType=%d!\n",Ein,U, Nuclides[nuc].NucID,LawType);},CD_Output::_WARNING);
			dExitErgInCm = ZeroErg;
		}
	}

	else if(nLawType == 22)
	{
		InterpolateXssTab(dEin,nNuc,LDAT,nPos,dFrac,NR,NE);
		LN = 2*NR + NE + 2 ;
		int ie = GetLocOfDLW(nNuc) - 1 + int(p_vNuclides[nNuc].XSS[LDAT + LN + nPos - 1]);
		int NF = int(p_vNuclides[nNuc].XSS[ie]);

		nIterCount = 0;
		do{
			LDAT = ie;
			double fr = ORNG.Rand();
			do{
				LDAT = LDAT + 1;
				fr = fr - p_vNuclides[nNuc].XSS[LDAT]; 
			}while( fr > 0. );

			if( (nIterCount++) >= MaxIter)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many samples of LawType %d!\n",nLawType);},CDOutput::_WARNING);
			}
		}while( LDAT > ie + NF ) ;

		dExitErgInCm = p_vNuclides[nNuc].XSS[LDAT + 2*NF]*(dEin - p_vNuclides[nNuc].XSS[LDAT + NF]);
	}

	else if(nLawType == 24)
	{
		InterpolateXssTab(dEin,nNuc,LDAT,nPos,dFrac,NR,NE);
		LN = 2*NR + NE + 2 ;
		NET = int(p_vNuclides[nNuc].XSS[LDAT+LN]);
		int k = int(ORNG.Rand()*(NET-1));
		int i = LDAT + LN + 1 + NET*(nPos - 1) + k;
		dExitErgInCm = dEin*(p_vNuclides[nNuc].XSS[i]+ORNG.Rand()*(p_vNuclides[nNuc].XSS[i+1]-p_vNuclides[nNuc].XSS[i]));
	}

	else if(nLawType ==4 || nLawType == 44 || nLawType == 61)
	{
		// law 4 (from endf law 1) -- continuous erg tabular distribution.
		// law 44 (from endf law 1) -- kalbach-87 correlated formalism.
		// law 61 (from endf law 1) -- correlated tab energy-angle dist.
		InterpolateXssTab(dEin,nNuc,LDAT,nPos,dFrac,NR,NE);
		//if(NR > 0)
		//{
		//	printf("Multiple interpolation regions not supported while attempting to sample Kalbach-Mann distribution!\n");
		//}

		LN = 2*NR + NE + 2 ;

		//// Sample between the ith and (i+1)th bin
		int nPos_smpl = dFrac > ORNG.Rand()? nPos + 1 : nPos;

		//// obtain endpoints on grid i
		int LDIS = GetLocOfDLW(nNuc);
		if(nMT == -1)
		{
			LDIS = GetLocOfDNED(nNuc);
		}

		int LC1 = LDIS + int(p_vNuclides[nNuc].XSS[LDAT - 1 + LN + nPos]) ;

		int NP1 = int(p_vNuclides[nNuc].XSS[LC1]);
		double E_i_1 = p_vNuclides[nNuc].XSS[LC1 + 1];
		double E_i_K = p_vNuclides[nNuc].XSS[LC1 + NP1];

		//// obtain endpoints on grid i+1
		int LC2 = LDIS + int(p_vNuclides[nNuc].XSS[LDAT - 1 + LN + nPos + 1]);
		int NP2 = int(p_vNuclides[nNuc].XSS[LC2]);
		double E_i1_1 = p_vNuclides[nNuc].XSS[LC2 + 1];
		double E_i1_K = p_vNuclides[nNuc].XSS[LC2 + NP2];

		double E_1 = E_i_1 + dFrac*(E_i1_1 - E_i_1);
		double E_K = E_i_K + dFrac*(E_i1_K - E_i_K);

		//// obtain location of outgoing energies, pdf, cdf for E(l)
		int LC = nPos_smpl == nPos? LC1 : LC2;

		//// obtain type of interpolation and number of discrete lines
		int INTTp = int(p_vNuclides[nNuc].XSS[LC -1]);
		int NP  = int(p_vNuclides[nNuc].XSS[LC]);
		
		int INTT = 0;
		int ND = 0;
		if (INTTp > 10) 
		{
			INTT = INTTp%10;
			ND = (INTTp - INTT)/10;
		}
		else
		{
			INTT = INTTp;
			ND = 0;
		}

		if (ND > 0) 
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"wrong discrete lines in continuous tabular.\n");},CDOutput::_WARNING);
			goto End;
		}

		//printf("INTTP = %d, INTT = %d, NP = %d\n",INTTp,INTT,NP);
		//// sample outgoing energy bin
		double r1 = ORNG.Rand();
		int ic = LC + 2*NP + 1;
		int ib = LC + 3*NP;
		int ih;
		while(ib - ic>1)
		{
			ih = (ic+ib)/2;
			if(r1 >= p_vNuclides[nNuc].XSS[ih])
			{
				ic = ih;
			}
			else
			{
				ib = ih;
			}
		}
		int k = ic - (LC + 2*NP);
		k = std::min(k, NP - 1);

		double c_k = p_vNuclides[nNuc].XSS[ic];  
		double E_l_k = p_vNuclides[nNuc].XSS[LC + k];
		double p_l_k = p_vNuclides[nNuc].XSS[LC + NP + k];
		double KM_R, KM_A;
		if( INTT == 1 ) // Histogram interpolation
		{
			if (p_l_k > 0) 
			{
				dExitErgInCm = E_l_k + (r1 - c_k)/p_l_k;
			}
			else
			{
				dExitErgInCm = E_l_k ;
			}
			if(nLawType == 44 )
			{
				KM_R = p_vNuclides[nNuc].XSS[LC + 3*NP + k];
				KM_A = p_vNuclides[nNuc].XSS[LC + 4*NP + k];
			}
		}
		else if(INTT == 2) //Linear-linear interpolation
		{
			double E_l_k1 = p_vNuclides[nNuc].XSS[LC + k +1];
			double p_l_k1 = p_vNuclides[nNuc].XSS[LC + NP + k +1];

			double fr = (p_l_k1 - p_l_k)/(E_l_k1 - E_l_k);
			if (fr == 0) 
			{
				dExitErgInCm = E_l_k + (r1 - c_k)/p_l_k ;
			}
			else
			{
				dExitErgInCm = E_l_k + (sqrt(std::max(0.0, p_l_k*p_l_k + 2*fr*(r1 - c_k))) - p_l_k)/fr ;
			}

			//// Determine Kalbach-Mann parameters
			if(nLawType == 44 )
			{
				double R_k  = p_vNuclides[nNuc].XSS[LC + 3*NP + k];
				double R_k1 = p_vNuclides[nNuc].XSS[LC + 3*NP + k + 1];
				double A_k  = p_vNuclides[nNuc].XSS[LC + 4*NP + k];
				double A_k1 = p_vNuclides[nNuc].XSS[LC + 4*NP + k + 1];

				KM_R = R_k + (R_k1 - R_k)*(dExitErgInCm - E_l_k)/(E_l_k1 - E_l_k);
				KM_A = A_k + (A_k1 - A_k)*(dExitErgInCm - E_l_k)/(E_l_k1 - E_l_k);
			}
		}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown interpolation type.MT = %d, LDIS = %d, LDAT = %d ,INTTP = %d, INTT = %d, NP = %d\n",
				nMT, LDIS, LDAT,INTTp,INTT,NP);},CDOutput::_WARNING);
			goto End;
		}

		//// interpolate between incident energy bins i and i + 1
		if (nPos_smpl == nPos) 
		{
			dExitErgInCm = E_1 + (dExitErgInCm - E_i_1)*(E_K - E_1)/(E_i_K - E_i_1);
		}
		else
		{
			dExitErgInCm = E_1 + (dExitErgInCm - E_i1_1)*(E_K - E_1)/(E_i1_K - E_i1_1);
		}

		///// sample angular distribution for law 44 & law 61 ////
		if(nLawType == 44 )  //// Sampled correlated angle from Kalbach-Mann parameters
		{
			double ksi3 = ORNG.Rand();
			double ksi4 = ORNG.Rand();
			if (ksi3 > KM_R)
			{
				double T = (2*ksi4 - 1)*sinh(KM_A);
				dMuInCm = log(T + sqrt((T*T) + 1))/KM_A;
			}
			else
			{
				dMuInCm = log(ksi4*exp(KM_A) + (1 - ksi4)*exp(-KM_A))/KM_A;
			}
			return;
		}
		else if(nLawType == 61 ) // sample tabulated angular distribution for law 61
		{
			int LB = ic + NP; // LC + 3*NP + k
			if( INTT!=1 && (p_vNuclides[nNuc].XSS[ib]-r1 < r1-p_vNuclides[nNuc].XSS[ic]) )  
			{
				LB = LB + 1;
			}
			int LM = int(p_vNuclides[nNuc].XSS[LB]);

			if (LM == 0) // isotropic
			{
				dMuInCm = 2*ORNG.Rand()-1;
				return ;
			}
			LC = GetLocOfDLW(nNuc) + abs(LM) - 2;
			// interpolation type and number of points in angular distribution
			int JJ = int(p_vNuclides[nNuc].XSS[LC + 1]);
			int NP = int(p_vNuclides[nNuc].XSS[LC + 2]);

			//// determine outgoing cosine bin
			double r3 = ORNG.Rand();
			double c_k = p_vNuclides[nNuc].XSS[LC + 2 + 2*NP + 1];
			int k;
			for( k = 1; k <= NP - 1 ; ++k)
			{
				double temp = p_vNuclides[nNuc].XSS[LC + 2 + 2*NP + k+1];
				if (r3 < temp) 
				{
					break ;
				}
				c_k = temp;
			}
			k = std::min(k, NP - 1);

			double p_k  = p_vNuclides[nNuc].XSS[LC + 2 + NP + k];
			double mu_k = p_vNuclides[nNuc].XSS[LC + 2 + k];
			if (JJ == 1) //Histogram interpolation
			{
				if (p_k > 0) 
				{
					dMuInCm = mu_k + (r3 - c_k)/p_k;
				}
				else
				{
					dMuInCm = mu_k ;
				}
			}

			else if (JJ == 2) // Linear-linear interpolation 
			{
				double p_k1  = p_vNuclides[nNuc].XSS[LC + 2 + NP + k + 1];
				double mu_k1 = p_vNuclides[nNuc].XSS[LC + 2 + k + 1];

				double fr = (p_k1 - p_k)/(mu_k1 - mu_k);
				if (fr == 0) 
				{
					dMuInCm = mu_k + (r3 - c_k)/p_k;
				}
				else
				{
					dMuInCm = mu_k + (sqrt(p_k*p_k + 2*fr*(r3 - c_k))-p_k)/fr;
				}
			}
			else
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown interpolation type.\n");},CDOutput::_WARNING);
				goto End;
			}
		}
	}

	else if(nLawType == 66)  // N-BODY PHASE SPACE DISTRIBUTION
	{
		int NPSX = int(p_vNuclides[nNuc].XSS[LDAT]); // number of bodies in phase space
		double Ap = p_vNuclides[nNuc].XSS[LDAT+1];  // total mass ratio
		double aw = p_vNuclides[nNuc].p_dNucAtomWgt;  // atomic weight
		double Q = GetNucFisQ(nNuc,nMT);//Nuclides[nuc].XSS[GetLocOfLQR(nuc) + Nuclides[nuc].MTRIndex[mt]-1]; // Q-value
		double E_max = ((Ap-1)/Ap)*(dEin*aw/(aw+1)+Q); 

		double lx = SampleMaxwell(1);
		double ly;

		if(NPSX == 3)
		{
			ly = SampleMaxwell(1);
		}
		else if(NPSX == 4)
		{
			double temp = ORNG.Rand()*ORNG.Rand()*ORNG.Rand();
			ly = log(temp);
		}
		else
		{
			double r1 = ORNG.Rand();
			double r2 = ORNG.Rand();
			double r3 = ORNG.Rand();
			double r4 = ORNG.Rand();
			double r5 = ORNG.Rand();
			double r6 = ORNG.Rand();
			ly = -log(r1*r2*r3*r4) - log(r5) * cos(PI/2.*r6)* cos(PI/2.*r6);
		}
		double V = lx/(lx+ly);	
		dExitErgInCm = V*E_max;
		dMuInCm = 2*ORNG.Rand()-1.;
	}


	else if(nLawType == 67) // LABORATORY ENERGY-ANGLE LAW
	{
		InterpolateXssTab(dEin,nNuc,LDAT,nPos,dFrac,NR,NE);
		LN = LDAT + 2*NR + NE + 2 ;

		///// go to appropriate table for incident energy ////
		int LC[3];   // p2-49    l,  i,   i+1
		LC[1] = GetLocOfDLW(nNuc) + int(p_vNuclides[nNuc].XSS[LN + nPos - 1]) - 1 ;
		LC[2] = 0 ;

		int ix = 1;
		int ir = 0;
		if(dFrac != 0)
		{
			LC[2] = GetLocOfDLW(nNuc) + int(p_vNuclides[nNuc].XSS[LN + nPos]) - 1;
			ir = 1;
			if(ORNG.Rand() <= dFrac)
			{
				ix = 2;
			}
		}

		double E_i_1[3],E_i_K[3];
		for(int i = 1;i <= 2;i++)
		{
			if(LC[i] == 0)
			{
				continue;
			}
			int le = LC[i];
			int INTMU = int(p_vNuclides[nNuc].XSS[le]);
			int NMU = int(p_vNuclides[nNuc].XSS[le + 1]);
			int LMU = 0;
			
			for (int j = 1;j <= NMU - 1;j++)
			{
				if(dMuInCm <= p_vNuclides[nNuc].XSS[le + j + 2])
				{
					LMU = j;
					break;
				}
			}
			if(INTMU == 2) // lin-lin
			{
				ir = ir + 1;
				if(ORNG.Rand() < (dMuInCm - p_vNuclides[nNuc].XSS[le + LMU + 1])/(p_vNuclides[nNuc].XSS[le + LMU + 2] - p_vNuclides[nNuc].XSS[le + LMU + 1]))
				{
					LMU = LMU + 1;
				}
			}
			else if(INTMU != 1) // lin-lin
			{
				//printf("i=%d le=%d INTMU=%d NMU=%d\n",i,le,INTMU,NMU);
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown INTMU interpolation type %d.\n",INTMU);},CDOutput::_WARNING);
				goto End;
			}

			// sample from tabulated energy distribution
			int lb = GetLocOfDLW(nNuc) + int(p_vNuclides[nNuc].XSS[le + LMU + NMU + 1]);
			int INTEP = int(p_vNuclides[nNuc].XSS[lb - 1]);
			int NPEP = int(p_vNuclides[nNuc].XSS[lb]);
			E_i_1[i] = p_vNuclides[nNuc].XSS[lb + 1];
			E_i_K[i] = p_vNuclides[nNuc].XSS[lb + NPEP];
			if(i != ix)
			{
				continue;
			}

			ir = ir + 1;
			double ksi4 = ORNG.Rand();

			int ic =  lb + 2*NPEP + 1, ib = lb + 3*NPEP;
			while(ib - ic>1)
			{
				int ih = (ic + ib)/2;
				if(ksi4 >= p_vNuclides[nNuc].XSS[ih]){ic = ih;}
				else{ib = ih;}
			}

			double E_l_m_k = p_vNuclides[nNuc].XSS[ic - 2*NPEP];
			double E_l_m_kk = p_vNuclides[nNuc].XSS[ic - 2*NPEP + 1];
			double p_l_m_k = p_vNuclides[nNuc].XSS[ic - NPEP];
			double p_l_m_kk = p_vNuclides[nNuc].XSS[ic - NPEP + 1];
			double c_l_m_k = p_vNuclides[nNuc].XSS[ic];  

			if(INTEP == 1)  //histogram
			{
				dExitErgInCm = E_l_m_k + (ksi4 - c_l_m_k)/p_l_m_k;
			}
			else if(INTEP == 2) //lin-lin
			{
				double cal_temp1 = (p_l_m_kk - p_l_m_k)/(E_l_m_kk - E_l_m_k);
				if(cal_temp1 == 0)
				{
					dExitErgInCm = E_l_m_k + (ksi4 - c_l_m_k)/p_l_m_k;
				}
				else
				{
					double cal_temp2 = sqrt(std::max(0.0, (p_l_m_k * p_l_m_k) + 2*cal_temp1*(ksi4-c_l_m_k)));
					dExitErgInCm = E_l_m_k + (cal_temp2 - p_l_m_k)/cal_temp1;
				}
			}
			else
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown interpolation type.\n");},CDOutput::_WARNING);
				goto End;
			}
		}

		if( dFrac == 0)
		{
			return;
		}

		double E_1 = E_i_1[1] + dFrac*(E_i_1[2] - E_i_1[1]);
		double E_K = E_i_K[1] + dFrac*(E_i_K[2] - E_i_K[1]);
		dExitErgInCm = E_1 + (dExitErgInCm - E_i_1[ix])*(E_K - E_1)/(E_i_K[ix] - E_i_1[ix]);
	}

	else
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown Law type.Law type = %d.\n",nLawType);},CDOutput::_WARNING);
		dExitErgInCm = ZeroErg;
	}



End:
    if(nMT == -1 && nLawType != 44 && nLawType != 61)
	{
		return;
	}

	if (p_vNuclides[nNuc].LAND[nMT] < 0 && nLawType != 44 && nLawType != 61 && nLawType !=66 )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"angular distribution is not processed for LAND[MT] = -1. LAW = %d\n",nLawType);},CDOutput::_WARNING);
		dExitErgInCm = ZeroErg;
	}


	if(dExitErgInCm == 0)  // MT = 91, law = 61, Erg = 0  ??
	{
		dExitErgInCm = ZeroErg;//
	}
	else if( !(dExitErgInCm > 0 && dExitErgInCm < 100))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit energy_cm out of range.\n        nuc=%s, MT=%d, Law=%d, Erg=%9.6E \n",
			p_vNuclides[nNuc].p_ONucID.p_chIdStr,nMT,nLawType,dExitErgInCm);},CDOutput::_WARNING);
		dExitErgInCm = ZeroErg;
	}


	if( !(dMuInCm >= -1.000001 && dMuInCm <= 1.000001))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit mu_cm out of range. nuc=%d, MT=%d, Law=%d, Mu=%f \n",nNuc,nMT,nLawType,dMuInCm);},CDOutput::_WARNING);
		dMuInCm = 2*ORNG.Rand()-1.;
	}
}