# include "Tally.h"
# include "Criticality.h"

void CDTally::TallyNucCsByTL(int nD1,int nD2,int nNuc,double dTrackLen, CDParticleState &cParticleState, CDAceData &cAceData)
{	
	double dIncidEg0 = cParticleState.p_dErg;
	double dCurrentWgt = cParticleState.p_dWgt;

	////////////////////////////// get interpolation parameters///////////////////////////
	double dInperpFrac;
	int nInperpPos;
	if( cAceData.p_vNuclides[nNuc].p_nNucInterpPos > 0 )  ///nuclide already interpolated
	{
		nInperpPos = cAceData.p_vNuclides[nNuc].p_nNucInterpPos;
		dInperpFrac = cAceData.p_vNuclides[nNuc].p_dNucInterpFrac;
	} 
	else ///interpolate nuclide by binary search 
	{
		int nMin,nMax;
		if(cAceData.p_bUseErgBinMap)
		{
			cAceData.GetErgBinMapPtr(nNuc,nMin,nMax);
		}
		else
		{
			nMin = 1 ;
			nMax = cAceData.GetErgGridNum(nNuc);
		}
		CDGlobeFun::GetIntpltPosFr(&cAceData.p_vNuclides[nNuc].XSS[0], dIncidEg0, nMin, nMax, nInperpPos,dInperpFrac);
	}


	////////////////////////////// interpolate nuc-mt reaction rate ///////////////////////////
	double dSigTemp = 0;
	int nMtNum = p_vOneGroupCsTally[nD1].p_vNucMTs[nD2].size();
	for(int d3 = 0; d3 < nMtNum; ++d3 )
	{
		int nMTTemp = p_vOneGroupCsTally[nD1].p_vNucMTs[nD2][d3];
		if(nMTTemp == -1)  // total
		{
			int nNE = cAceData.GetErgGridNum(nNuc);
			dSigTemp = CDGlobeFun::IntpltByPosFr(&cAceData.p_vNuclides[nNuc].XSS[0],nInperpPos+nNE,dInperpFrac);
		}
		else if(nMTTemp == -2)  // absorption excluding fission
		{
			int nNE = cAceData.GetErgGridNum(nNuc);
			dSigTemp = CDGlobeFun::IntpltByPosFr(&cAceData.p_vNuclides[nNuc].XSS[0],nInperpPos+2*nNE,dInperpFrac);
		}
		else if(nMTTemp == -3)  // elastic
		{
			int nNE = cAceData.GetErgGridNum(nNuc);
			dSigTemp = CDGlobeFun::IntpltByPosFr(&cAceData.p_vNuclides[nNuc].XSS[0],nInperpPos+3*nNE,dInperpFrac);
		}
		else if(nMTTemp == -6)  // fission
		{
			//if(AceData.Nuclides[nuc].ProbTableFlag != 1 )
			//{
				dSigTemp = CDGlobeFun::IntpltByPosFr(&cAceData.p_vNuclides[nNuc].FisXSS[0],nInperpPos,dInperpFrac);
			//}
			//else
			//{
			//	sig_temp = AceData.Nuclides[nuc].NucCs.Fis;
			//}
		}
		else if(nMTTemp <= 0)
		{
			return;
		}
		else // other MT reaction type
		{
			dSigTemp = cAceData.GetNucMtCs(nNuc,nMTTemp,nInperpPos,dInperpFrac);
		}

		if(dSigTemp > 0)
		{
			int nPtr = p_vOneGroupCsTally[nD1].GetNucDataPtr(nD2) + d3;
			p_OCsTallyData.p_vScore[nPtr] = p_OCsTallyData.p_vScore[nPtr] + dCurrentWgt*dTrackLen*dSigTemp;
		}
	}

	return;
}