# include "Tally.h"
# include "Criticality.h"

void CDTally::ProcessCsByUEG(CDMaterial &cMaterial, CDCriticality &cCriticality, CDAceData &cAceData)
{	
	int nCsTallyNum = p_vOneGroupCsTally.size();
	for(int i = 0; i < nCsTallyNum; ++i )
	{
		int nMat = p_vOneGroupCsTally[i].p_nMat ;
		int nNucNum = cMaterial.GetMatTotNucNum(nMat) ;

		for(int j = 0; j < nNucNum; ++j )
		{
			int nNuclide = cMaterial.GetMatNucIndex(nMat,j);
			int nMtNum = p_vOneGroupCsTally[i].p_vNucMTs[j].size();
			for(int k = 0 ; k < nMtNum; k++)
			{
				int nMTTemp =  p_vOneGroupCsTally[i].p_vNucMTs[j][k];

				if(nMTTemp == -1)
				{
					int nNE=cAceData.GetErgGridNum(nNuclide);
					CalAveCsByUEG(cAceData.p_vNuclides[nNuclide].XSS, nNuclide, 1, nNE, nNE,i,j,k, cAceData);
				}
				else if(nMTTemp == -2)
				{
					int nNE=cAceData.GetErgGridNum(nNuclide);
					CalAveCsByUEG(cAceData.p_vNuclides[nNuclide].XSS, nNuclide, 1, nNE, 2*nNE,i,j,k, cAceData);
				}
				else if(nMTTemp == -3)
				{
					int nNE=cAceData.GetErgGridNum(nNuclide);
					CalAveCsByUEG(cAceData.p_vNuclides[nNuclide].XSS, nNuclide, 1, nNE, 3*nNE,i,j,k, cAceData);
				}

				else if(nMTTemp == -6)
				{
					int nNE=cAceData.GetErgGridNum(nNuclide);
					CalAveCsByUEG(cAceData.p_vNuclides[nNuclide].FisXSS, nNuclide, 1, nNE, 0,i,j,k, cAceData);
				}
				else if(nMTTemp > 0 )
				{
					int nLc = cAceData.p_vNuclides[nNuclide].MTRIndex[nMTTemp];
					if(nLc <= 0)
					{
						int nPtr = p_vOneGroupCsTally[i].GetNucDataPtr(j) + k;
						p_OCsTallyData.p_vAve[nPtr] = 0;
						continue;
					}
					int nIELoca = cAceData.GetLocOfSIG(nNuclide)+cAceData.p_vNuclides[nNuclide].LSIG[nMTTemp]-1;
					int nSIGIE = int(cAceData.p_vNuclides[nNuclide].XSS[nIELoca]);   // Energy grid index for reaction
					int nSIGNE = int(cAceData.p_vNuclides[nNuclide].XSS[nIELoca+1]); // Number of consecutive entries
					int nNE = cAceData.GetErgGridNum(nNuclide);
					if( nSIGIE > nNE || nNE-nSIGIE + 1 != nSIGNE )
					{
						Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"abnormal threshold-energy!(MT=%d;Nuclide=%s)",
							nMTTemp,cAceData.p_vNuclides[nNuclide].p_ONucID.p_chIdStr);}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
					}

					CalAveCsByUEG(cAceData.p_vNuclides[nNuclide].XSS, nNuclide, nSIGIE, nNE, nIELoca+2-nSIGIE, i, j, k, cAceData);
				}
			}
		}

	}

	//////////////////////////// normalization //////////////////
	double dTotHistory = (cCriticality.p_nCurrentCYCLE-cCriticality.p_nInactCycNum)*cCriticality.p_dTotStartWgt;
	for(int i = 0; i < nCsTallyNum; ++i )
	{
		double dFluxSum1 = 0;
		for(int n = 0 ; n < p_vOneGroupCsTally[i].p_vUnionFlux.size(); ++n)
		{
			dFluxSum1 = dFluxSum1 + p_vOneGroupCsTally[i].p_vUnionFlux[n];
		}

		int nFluxPtr = p_vOneGroupCsTally[i].GetFluxDataPtr();
		p_OCsTallyData.p_vAve[nFluxPtr] = dFluxSum1/dTotHistory;

		int nMat = p_vOneGroupCsTally[i].p_nMat ;
		int nNucNum = cMaterial.GetMatTotNucNum(nMat) ;
		for(int j = 0; j < nNucNum; ++j )
		{
			//int nuclide = Material.GetMatNucIndex(mat,j);
			int nMtNum = p_vOneGroupCsTally[i].p_vNucMTs[j].size();
			for(int k = 0 ; k < nMtNum; k++)
			{
				int nPtr = p_vOneGroupCsTally[i].GetNucDataPtr(j) + k;
				p_OCsTallyData.p_vAve[nPtr] = p_OCsTallyData.p_vAve[nPtr]/dFluxSum1;
			}
		}
		
	}
}


void CDTally::ProcessCsByUEG(CDMaterial &CMaterial, CDFixedSource &cFixedSource, CDAceData &CAceData)
{	
	int CsTallyNum = p_vOneGroupCsTally.size();
	for(int i = 0; i < CsTallyNum; ++i )
	{
		int mat = p_vOneGroupCsTally[i].p_nMat ;
		int nuc_num = CMaterial.GetMatTotNucNum(mat) ;

		for(int j = 0; j < nuc_num; ++j )
		{
			int nuclide = CMaterial.GetMatNucIndex(mat,j);
			int mt_num = p_vOneGroupCsTally[i].p_vNucMTs[j].size();
			for(int k = 0 ; k < mt_num; k++)
			{
				int MT_temp =  p_vOneGroupCsTally[i].p_vNucMTs[j][k];

				if(MT_temp == -1)
				{
					int NE=CAceData.GetErgGridNum(nuclide);
					CalAveCsByUEG(CAceData.p_vNuclides[nuclide].XSS, nuclide, 1, NE, NE,i,j,k, CAceData);
				}
				else if(MT_temp == -2)
				{
					int NE=CAceData.GetErgGridNum(nuclide);
					CalAveCsByUEG(CAceData.p_vNuclides[nuclide].XSS, nuclide, 1, NE, 2*NE,i,j,k, CAceData);
				}
				else if(MT_temp == -3)
				{
					int NE=CAceData.GetErgGridNum(nuclide);
					CalAveCsByUEG(CAceData.p_vNuclides[nuclide].XSS, nuclide, 1, NE, 3*NE,i,j,k, CAceData);
				}

				else if(MT_temp == -6)
				{
					int NE=CAceData.GetErgGridNum(nuclide);
					CalAveCsByUEG(CAceData.p_vNuclides[nuclide].FisXSS, nuclide, 1, NE, 0,i,j,k, CAceData);
				}
				else if(MT_temp > 0 )
				{
					int lc = CAceData.p_vNuclides[nuclide].MTRIndex[MT_temp];
					if(lc <= 0)
					{
						int ptr = p_vOneGroupCsTally[i].GetNucDataPtr(j) + k;
						p_OCsTallyData.p_vAve[ptr] = 0;
						continue;
					}
					int IE_LOCA = CAceData.GetLocOfSIG(nuclide)+CAceData.p_vNuclides[nuclide].LSIG[MT_temp]-1;
					int SIG_IE = int(CAceData.p_vNuclides[nuclide].XSS[IE_LOCA]);   // Energy grid index for reaction
					int SIG_NE = int(CAceData.p_vNuclides[nuclide].XSS[IE_LOCA+1]); // Number of consecutive entries
					int NE = CAceData.GetErgGridNum(nuclide);
					if( SIG_IE > NE || NE-SIG_IE + 1 != SIG_NE )
					{
						Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"abnormal threshold-energy!(MT=%d;Nuclide=%s)",
							MT_temp,CAceData.p_vNuclides[nuclide].p_ONucID.p_chIdStr);}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
					}

					CalAveCsByUEG(CAceData.p_vNuclides[nuclide].XSS, nuclide, SIG_IE, NE, IE_LOCA+2-SIG_IE, i, j, k, CAceData);
				}
			}
		}

	}

	//////////////////////////// normalization //////////////////
	double TotHistory = cFixedSource.p_dTotStartWgt;
	for(int i = 0; i < CsTallyNum; ++i )
	{
		double FluxSum1 = 0;
		for(int n = 0 ; n < p_vOneGroupCsTally[i].p_vUnionFlux.size(); ++n)
		{
			FluxSum1 = FluxSum1 + p_vOneGroupCsTally[i].p_vUnionFlux[n];
		}

		int flux_ptr = p_vOneGroupCsTally[i].GetFluxDataPtr();
		p_OCsTallyData.p_vAve[flux_ptr] = FluxSum1/TotHistory;

		int mat = p_vOneGroupCsTally[i].p_nMat ;
		int nuc_num = CMaterial.GetMatTotNucNum(mat) ;
		for(int j = 0; j < nuc_num; ++j )
		{
			//int nuclide = Material.GetMatNucIndex(mat,j);
			int mt_num = p_vOneGroupCsTally[i].p_vNucMTs[j].size();
			for(int k = 0 ; k < mt_num; k++)
			{
				int ptr = p_vOneGroupCsTally[i].GetNucDataPtr(j) + k;
				p_OCsTallyData.p_vAve[ptr] = p_OCsTallyData.p_vAve[ptr]/FluxSum1;
			}
		}

	}
}
/*
void TallyClass::CalAveCsByUEG(vector<vector<double> > &xsdata, int nuc, int StartPos, int EndPos, int Offset,int d1,int d2, int d3)

*/
void CDTally::CalAveCsByUEG(vector<double > &dXsData, int nNuc, int nStartPos, int nEndPos, int nOffset,int nD1,int nD2, int nD3, CDAceData &cAceData)
{
	int nI1 = 1;
	int nI2 = nStartPos;
	int nNE = nEndPos;
	double dSigTemp ;
	while(cAceData.p_vUnionEnergyGrid[nI1] <= cAceData.p_vNuclides[nNuc].XSS[nI2])
	{ 
		nI1 = nI1 + 1;
	}

	double dSumTemp = 0;
	while(nI2 < nNE)
	{
		double dEgTemp = cAceData.p_vUnionEnergyGrid[nI1] ;
		if(dEgTemp < cAceData.p_vNuclides[nNuc].XSS[nI2+1]) 
		{
			//sum_temp = sum_temp + OneGroupCsTally[d1].UnionFlux[i1];

			dEgTemp =  0.5 * (dEgTemp + cAceData.p_vUnionEnergyGrid[nI1+1]) ;
			double dInterpFrac = (dEgTemp - cAceData.p_vNuclides[nNuc].XSS[nI2])/(cAceData.p_vNuclides[nNuc].XSS[nI2+1] - cAceData.p_vNuclides[nNuc].XSS[nI2]);
			dSigTemp = CDGlobeFun::IntpltByPosFr(&dXsData[0],nI2 + nOffset,dInterpFrac);

			int nPtr = p_vOneGroupCsTally[nD1].GetNucDataPtr(nD2) + nD3;
			p_OCsTallyData.p_vAve[nPtr] = p_OCsTallyData.p_vAve[nPtr] +  p_vOneGroupCsTally[nD1].p_vUnionFlux[nI1]*dSigTemp;

			nI1 = nI1 + 1;
		}
		else
		{
			//sig_temp = 0.5*( AceData.Nuclides[nuc].XSS[i2 + Offset] + AceData.Nuclides[nuc].XSS[i2+ Offset + 1]);
			//OneGroupCsTally[d1].NucMtCsAve[d2][d3] = OneGroupCsTally[d1].NucMtCsAve[d2][d3] +  sum_temp*sig_temp;
			//sum_temp = 0;
			nI2 = nI2 + 1;
		}
	}

	dSumTemp = 0;
	while(nI1 <= cAceData.p_nUnionEgNum)
	{
		dSumTemp = dSumTemp + p_vOneGroupCsTally[nD1].p_vUnionFlux[nI1];
		nI1 = nI1+1;
	}
	dSigTemp = dXsData[nNE + nOffset];
	int nPtr = p_vOneGroupCsTally[nD1].GetNucDataPtr(nD2) + nD3;
	p_OCsTallyData.p_vAve[nPtr] = p_OCsTallyData.p_vAve[nPtr] + dSumTemp*dSigTemp;

	return;
}