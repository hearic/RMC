# include "AceData.h"

void CDAceData::GetMgExitErgMu(int nNuc, double dIncidErg, double &dExitErg, double &dExitMu)
{
	/////////////////////////// sample exit energy group ///////////////////////////////////
	int nGrpNum = GetMgGrpNum(nNuc);
	int nIncidGrp = int(dIncidErg);	
	int nExitGrp;
	int I1 = std::max(1,nIncidGrp - GetMgUpScatNum(nNuc));
	int I2 = std::min(nGrpNum,nIncidGrp + GetMgDownScatNum(nNuc));
	int LP0 = int(p_vNuclides[nNuc].XSS[GetMgLP0L(nNuc)]);
	int P0Loc = LP0 + GetMgNucP0Loc(nNuc,nIncidGrp) - 1;
	double r = ORNG.Rand() * GetMgNucP0Cs(nNuc,nIncidGrp);
	double dSum = 0 ;
	int k = 0;
	for(nExitGrp = I1 ; nExitGrp <= I2 ; ++nExitGrp)
	{
		dSum = dSum + p_vNuclides[nNuc].XSS[P0Loc + k];
		if (dSum >= r)
		{
			break;
		}
		k++;
	}

	if(nExitGrp > nGrpNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown energy group of exit neutron. nuclide = %s.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
		nExitGrp = I2 ;
	}
	dExitErg = double(nExitGrp) ;

	///////////////////////////////// sample exit mu   /////////////////////////////////////
	/////////// tread XPNL block /////////////
	int LXPN = int(p_vNuclides[nNuc].XSS[GetMgLXPNL(nNuc)]);
	int PnLoc = LXPN + GetMgNucP0Loc(nNuc,nIncidGrp) -1 + k ;
	int LPND = int(p_vNuclides[nNuc].XSS[PnLoc]);
	if(LPND == 0) //// isotropic
	{
		dExitMu = 2 * ORNG.Rand() - 1 ;
		return;
	}
	if(LPND == -1)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"impossible scattering from group %d to group % d in nuclide %s.\n",nIncidGrp,nExitGrp,p_vNuclides[nNuc].p_ONucID.p_chIdStr);},CDOutput::_WARNING);
		dExitMu = 2 * ORNG.Rand() - 1 ;
		return;
	}

	/////////// tread PNL block /////////////
	int LPN = int(p_vNuclides[nNuc].XSS[GetMgLPNL(nNuc)]);
	if(LPN == 0) //// isotropic
	{
		dExitMu = 2 * ORNG.Rand() - 1 ;
		return;
	}

	/////////// tread PN block /////////////
	int nPndLoc = LPN + LPND -1 ;
	int NLEG = GetMgAngVarNum(nNuc);
	if(GetMgAngType(nNuc) == 0) //// equiproble cosine bins
	{
		if( NLEG > 1)
		{
			double rr = ORNG.Rand()*(NLEG - 1);
			int i = int(rr);
			dExitMu = p_vNuclides[nNuc].XSS[nPndLoc + i] + (rr - i)*(p_vNuclides[nNuc].XSS[nPndLoc + i + 1] - p_vNuclides[nNuc].XSS[nPndLoc + i]);
		}
		else //// special treatment for one moment
		{
			if( p_vNuclides[nNuc].XSS[nPndLoc] >= 0)
			{
				dExitMu = 1 - 2*ORNG.Rand()*(1 - p_vNuclides[nNuc].XSS[nPndLoc]);
			}
			else
			{
				dExitMu = -1 + 2*ORNG.Rand()*(1 + p_vNuclides[nNuc].XSS[nPndLoc]);
			}
		}
	}
	else //// discrete cosine 
	{
		int n = (NLEG - 1)/2;
		double rr = ORNG.Rand();
		int i;
		for(i = 1 ; i <= n ; ++i)
		{
			if(p_vNuclides[nNuc].XSS[nPndLoc + i - 1] > rr)
			{
				break;
			}
		}
		dExitMu = p_vNuclides[nNuc].XSS[nPndLoc + n + i - 1];
	}

	////////////// Check ExitMu /////////////
	if(!(dExitMu >= -1.000001 && dExitMu <= 1.000001))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit mu out of range. nuclide = %s group %d to group % d.\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr,nIncidGrp,nExitGrp);},CDOutput::_WARNING);
		dExitMu = 2*ORNG.Rand()-1;
	}

	return;
}

void CDAceData::GetMgFisErgDir(int nNuc, double &dExitErg, double dDir[3])
{
	//// Note: energy group is stored as "double"
	double r = ORNG.Rand();
	double dSum = 0 ;
	int nLoc = GetMgLCHI(nNuc);
	int nGrpNum = GetMgGrpNum(nNuc);

	///////////////////////// sample fission neutron energy group //////////////////////////////
	int nExitGrp ;
	for(nExitGrp = 1 ; nExitGrp <= nGrpNum ; ++nExitGrp)
	{
		dSum = dSum + p_vNuclides[nNuc].XSS[nLoc + nExitGrp -1];
		if (dSum >= r)
		{
			break;
		}
	}

	if(nExitGrp > nGrpNum)
	{
		//Output.OutputMessage([&](){sprintf(Output.PrintStr,"unknown energy group of fission neutron. nuclide = %s, sum = %f.\n",Nuclides[nuc].NucID.IdStr,sum);},CD_Output::_WARNING);
		nExitGrp = nGrpNum ;
	}

	dExitErg = double(nExitGrp);

	/////////////////////////// sample fission neutron direction /////////////////////////////
	double ksi1 = ORNG.Rand();
	double ksi2 = ORNG.Rand();  
	dDir[0] = 2*ksi2 - 1;   
	dDir[1] = sqrt(1 - dDir[0]*dDir[0])*cos(2*3.14159*ksi1);
	dDir[2] = sqrt(1 - dDir[0]*dDir[0])*sin(2*3.14159*ksi1);
}