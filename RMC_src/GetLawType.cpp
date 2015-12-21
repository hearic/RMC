# include "AceData.h"

void CDAceData::GetLawType(int nNuc,int nMT,double dIncidErg,int &nLawType, int &LDAT)
{	
	double E1,E0,Pi = 0,ksi;
	int LNW,IDAT,NR,NE;
	int which_Pi = 1;

	int LDIS = nMT > 0? GetLocOfDLW(nNuc) : GetLocOfDNED(nNuc);
	int LOCC = nMT > 0? LDIS + p_vNuclides[nNuc].LDLW[nMT] : LDIS + int(p_vNuclides[nNuc].XSS[GetLocOfDNEDL(nNuc) - nMT - 1]);


	LNW = int(p_vNuclides[nNuc].XSS[LOCC - 1]);
	nLawType = int(p_vNuclides[nNuc].XSS[LOCC]);
	if(nLawType <= 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown Law type %d. \nNuc = %d, MT = %d, Law_type = XSS[%d] = %9.5E.\n",
			nLawType,p_vNuclides[nNuc].p_nNucZaid,nMT, LOCC,p_vNuclides[nNuc].XSS[LOCC]);},CDOutput::_WARNING);
	}

	IDAT = int(p_vNuclides[nNuc].XSS[LOCC + 1]);
	if(LNW == 0)
	{
		LDAT = LDIS + IDAT - 1;

		//if(LawType != 3 && LawType != 4  && LawType != 44 )
		//{
		//    printf("law = %d\n",LawType);
		//}
		return ;
	}

	NR = int(p_vNuclides[nNuc].XSS[LOCC+2]);
	NE = int(p_vNuclides[nNuc].XSS[LOCC+3+2*NR]);

	if(dIncidErg <= p_vNuclides[nNuc].XSS[LOCC+4+2*NR])
	{
		which_Pi = 1;
		Pi = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+NE];
	}
	else if(dIncidErg >= p_vNuclides[nNuc].XSS[LOCC+4+2*NR+NE-1])
	{
		which_Pi = NE;
		Pi = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+NE+NE-1];
	}
	else
	{
		for (int j=1;j<=NE-1;j++)
		{
			E1 = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+j];
			if(dIncidErg <= E1)
			{
				E0 = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+j-1];
				if(ORNG.Rand() < (dIncidErg-E0)/(E1-E0))    //  En  or  En+1
				{
					which_Pi = j+1;
					Pi = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+NE+j];
				}
				else
				{
					which_Pi = j;
					Pi = p_vNuclides[nNuc].XSS[LOCC+4+2*NR+NE+j-1];
				}
				break;
			}
		}
	}

	ksi = ORNG.Rand();
	while(ksi > Pi)   //未抽中当前law，考虑抽下一个law
	{
		Pi = Pi + p_vNuclides[nNuc].XSS[LDIS+LNW+4+2*NR+NE+which_Pi-1];
		if(ksi<=Pi)
		{
			nLawType = int(p_vNuclides[nNuc].XSS[LDIS+LNW]);
			IDAT = int(p_vNuclides[nNuc].XSS[LDIS+LNW+1]);
			break;
		}
		LNW = int(p_vNuclides[nNuc].XSS[LDIS+LNW-1]);
		if(LNW == 0)
		{
			break;
		}
	}

	LDAT = LDIS+IDAT-1;

	return;
}