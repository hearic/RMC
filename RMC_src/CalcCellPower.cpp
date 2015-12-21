#include "Burnup.h"


void CDBurnup::CalcCellPower(CDMaterial &cMaterial,CDTally &cTally)
{
	static int nRecNucId[56] = 
	{10010, 50100, 80160, 260560, 280580, 400900, 400910, 400920, 400960, 420950, 
	430950, 441010, 451030, 451050, 471090, 541310, 541350, 551330, 551340, 601430, 
	601450, 611470, 611480, 611481, 621470, 621490, 621500, 621510, 621520, 631530, 
	631540, 631550, 902300, 902320, 902330, 912310, 912330, 922320, 922330, 922340, 
	922350, 922360, 922380, 932370, 932390, 942380, 942390, 942400, 942410, 942420, 
	942430, 952410, 952421, 952430, 962440, 962450};

	static double dFisQ[56]  = 
	{0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 
	0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 
	0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 0.0000E+00, 
	0.0000E+00, 0.0000E+00, 1.9000E+02, 1.8921E+02, 1.9000E+02, 1.9000E+02, 1.8910E+02, 2.0000E+02, 1.9129E+02, 1.9030E+02, 
	1.9402E+02, 1.9280E+02, 1.9812E+02, 1.9510E+02, 2.0000E+02, 1.9780E+02, 2.0005E+02, 1.9979E+02, 2.0222E+02, 2.0062E+02, 
	2.0000E+02, 2.0230E+02, 2.0229E+02, 2.0210E+02, 2.0000E+02, 2.0000E+02};

	static double dCapQ[56]  =
	{2.2250E+00, 2.7900E+00, 4.1430E+00, 7.6000E+00, 9.0200E+00, 7.2030E+00, 8.6350E+00, 6.7580E+00, 5.5710E+00, 9.1540E+00, 
	7.7100E+00, 9.2160E+00, 6.9990E+00, 7.0940E+00, 6.8250E+00, 8.9360E+00, 7.8800E+00, 6.7040E+00, 6.5500E+00, 7.8170E+00, 
	7.5650E+00, 5.9000E+00, 7.2660E+00, 7.2660E+00, 8.1400E+00, 7.9820E+00, 5.5960E+00, 8.2580E+00, 5.8670E+00, 6.4440E+00, 
	8.1670E+00, 6.4900E+00, 5.0100E+00, 4.7860E+00, 6.0800E+00, 5.6600E+00, 5.1970E+00, 5.9300E+00, 6.8410E+00, 5.2970E+00, 
	6.5450E+00, 5.1240E+00, 4.8040E+00, 5.4900E+00, 4.9700E+00, 5.5500E+00, 6.5330E+00, 5.2410E+00, 6.3010E+00, 5.0710E+00, 
	6.0200E+00, 5.5290E+00, 6.4260E+00, 5.3630E+00, 6.4510E+00, 6.1100E+00};


	/////////////////////////////// calculate power distribution /////////////////////////////
	vector<double> vPowerFrac(p_nBurnCellNum);
	vector<double> vHeatRateFrac(p_nBurnCellNum); // NOT multiplied by flux

	double dPowerSum = 0;
	for( int i = 0; i < p_nBurnCellNum; ++i )
	{
		vPowerFrac[i]=0;

		int nTallyPos = p_nBurnTallyStartPos + i ;

		int nMat = cTally.p_vOneGroupCsTally[nTallyPos].p_nMat;
		int nMatNucNum = cMaterial.GetMatTotNucNum(nMat);

		int nFluxLoc = cTally.p_vOneGroupCsTally[nTallyPos].GetFluxDataPtr();
		double dFlux = cTally.p_OCsTallyData.p_vAve[nFluxLoc] ;  //// cell integral flux

		double dQcSum = 0 ;
		double dQfSum = 0 ;
		for( int j = 0;j < nMatNucNum; ++j )
		{
			double dAtomDen = cMaterial.GetMatNucAtomDen(nMat,j);
			if(dAtomDen <= 0)
			{
				continue;
			}

			////////////////// convert nuclide id ///////////////////
			int nNucId = atoi(cMaterial.GetMatNucID(nMat,j).p_chIdStr);
			if( nNucId % 1000 >= 400 )  //2012-01-17      excited-state nuclide
			{ 
				nNucId = (nNucId - 400)*10 + 1 ;  
			}
			else
			{
				nNucId = nNucId*10;
			}


			//////////////////  binary search ///////////////////////
			int nMin = 0, nMax = 55, nMid, nIndex = -1;
			if(nNucId == nRecNucId[nMin])
			{
				nIndex = nMin;
			}
			else if(nNucId == nRecNucId[nMax])
			{
				nIndex = nMax;
			}
			else
			{
				while( nMax - nMin > 1)
				{
					nMid = (nMin + nMax)/2;
					if(nNucId == nRecNucId[nMid])
					{
						nIndex = nMid;
						break;
					}
					if(nNucId > nRecNucId[nMid])
					{
						nMin = nMid;
					}
					else
					{
						nMax = nMid;
					}
				}
			}


			/////////////////// get qc and qf ///////////////////////
			double dQc = 5.0;
			double dQf = 200.0;
			if(nIndex >= 0)
			{
				dQc = dCapQ[nIndex];
				dQf = dFisQ[nIndex];
			}


			/////////////////// sum-up recoverable energy ////////////////////
			int dCapLoc = cTally.p_vOneGroupCsTally[nTallyPos].GetNucDataPtr(j);
			double dCapXs = 0.0;// Use non-fission absorption xs (capture) , bug found by NPIC, fixed at 20140121, LIANG
			for (int i = 0; i < 4; i++)
			{
				dCapXs = dCapXs + cTally.p_OCsTallyData.p_vAve[dCapLoc + i];
			}

			if(nNucId >= 900000)
			{
				int nFisLoc = dCapLoc + 3;
				double dFisXs = cTally.p_OCsTallyData.p_vAve[nFisLoc];
				dQfSum = dQfSum + dQf*dFisXs*dAtomDen;

				dCapXs = dCapXs - dFisXs;// non-fission
			}
			dQcSum = dQcSum + dQc*dCapXs*dAtomDen;
		}
		vPowerFrac[i] = (dQcSum + dQfSum) * dFlux;  // flux is volume-integrated
		vHeatRateFrac[i] = (dQcSum + dQfSum);
		dPowerSum = dPowerSum + vPowerFrac[i]; 
	}

	/////////////////////////////  calculate actual power by total power////////////////////////
	int nBurnStep = p_nBurnStep < p_nTotBurnStep? p_nBurnStep + 1 : p_nTotBurnStep;
	if (p_vStepBurnPower[nBurnStep] == 0)
	{
		p_nDepleteMode = 0; // only decay mode if total power e.q. 0
		return;
	}
	
	p_nDepleteMode = 2; // constant power mode

	if (dPowerSum <= 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Power normalization failed-total raw heat release = %f.", dPowerSum);}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
	for(int i = 0; i < p_nBurnCellNum;++i)
	{
		p_vCellBurnPower[i] = p_vStepBurnPower[nBurnStep]*vPowerFrac[i]/dPowerSum;
		if (vHeatRateFrac[i] == 0 || p_vCellBurnPower[i] == 0)
		{
			p_vCellActualFlux[i] = 0;// no heat release
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Zero recoverable energy/power in burnup region no.%d.", i+1);}, 
				Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		}
		else
		{
			p_vCellActualFlux[i] = p_vCellBurnPower[i]/(vHeatRateFrac[i] * 1.60217653E-19); // Actual Flux, ¦µi = Qi/Heatf
			// unit of dAtomDen*qc = 10^24 /cm^3 * barn * Mev = 10^24*10^(-24)*M*1.60217653E-19 = 1.60217653E-19 J/cm
			// a little different from flux calculated in Depth, 20140627
		}
	}
	// p_dActualFluxRatio = p_vStepBurnPower[dBurnStep]/(dPowerSum*1.60217653E-19); // ActualFlux/TallyFlux = ActualPower/PowerSum/BarnMev

	return;
}


void CDBurnup::GetCellAveEgy(CDTally &cTally)    // Get average energy of overall burnable cells
{	
	const double THERMALENERGY = 0.0253; // ev
	const double FASTENERGY = 2.0E+06;
	const double HIGHENERGY = 1.4E+07;
	double dAveErg = 0;
	if (p_dNAECFInput <= -3)
	{
		dAveErg = HIGHENERGY;
	}
	else if (p_dNAECFInput <= -2)
	{
		dAveErg = FASTENERGY;
	}
	else if (p_dNAECFInput <= -1)
	{
		dAveErg = THERMALENERGY;
	}
	else if (p_dNAECFInput > 0)
	{
		dAveErg = p_dNAECFInput;
	}

	for( int i = 0; i < p_nBurnCellNum; ++i )
	{
		if (p_dNAECFInput == 0)
		{
			int nTallyPos = p_nBurnTallyStartPos + i ;

			p_vCellBurnAveEnergy[i] = cTally.p_vOneGroupCsTally[nTallyPos].p_dNeuAveErg*1000000; //// tallied energy, unit: ev
		}
		else
		{
			p_vCellBurnAveEnergy[i] = dAveErg; // user defined energy, unit: ev
		}
	}

	return;
}