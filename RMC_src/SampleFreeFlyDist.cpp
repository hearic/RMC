//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"


double CDNeutronTransport::SampleFreeFlyDist(CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,bool IsErgChanged)
{
	/////////////////////// use previous MacroTotCs if available  ///////////////////////////
	if(!IsErgChanged) 
	{
		if(!cParticleState.p_bIsCellChanged)
		{
			return -log(ORNG.Rand())/cParticleState.p_dMacroTotCs;
		}

		if(!cParticleState.p_bIsMatChanged && !cParticleState.p_bIsCellTmpChanged)
		{
			return -log(ORNG.Rand())/cParticleState.p_dMacroTotCs;
		}
	}


	////////////////////////////////////// ordinary cases //////////////////////////////////////

	//////////////////////////////////  vacuum cell case   /////////////////////////////////
	int nCurrentMAT = cParticleState.p_nMAT;
	if(nCurrentMAT == 0)// vacuum
	{
		cParticleState.p_dMacroTotCs = 1.0E-24;
		cParticleState.p_dMacroMuFissCs = 0 ;
		return -log(ORNG.Rand())/cParticleState.p_dMacroTotCs;
	}  


	/////////// recalulate Macro cross-section and sample free fight distance ///////////
	cParticleState.p_dMacroTotCs = 0;
	cParticleState.p_dMacroMuFissCs = 0;


	///////////////////////////// multigroup  case ///////////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nuc = cMaterial.GetMatNucIndex(nCurrentMAT, i);
			int IncidGrp = cParticleState.ErgGrp();
			double NucDen = cMaterial.GetMatNucAtomDen(nCurrentMAT, i);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot = cAceData.GetMgNucTotCs(nuc,IncidGrp);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis = cAceData.GetMgNucFisCs(nuc,IncidGrp);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dNu =cAceData.GetMgNucNubar(nuc,IncidGrp);
			cParticleState.p_dMacroTotCs = cParticleState.p_dMacroTotCs + cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot * NucDen;
			cParticleState.p_dMacroMuFissCs = cParticleState.p_dMacroMuFissCs + cAceData.p_vNuclides[nuc].p_ONucCs.p_dNu * cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis * NucDen;
		}
	}
	/////////////////////// continuous energy case //////////////////////
	else
	{
		if(IsErgChanged)  //// use last interpolation information if energy is not changed
		{
			for(int nNuc = 1; nNuc <= cMaterial.GetTotNucNumExcSab(); ++nNuc)
			{
				cAceData.p_vNuclides[nNuc].p_nNucInterpPos = -1;
			}
			if(cAceData.p_bUseErgBinMap) ////  accelerate binary search by energy bin map 
			{
				cAceData.p_OAceErgBinMap.GetBinMapIndex(cParticleState.p_dErg);
			}
		}

		////////// calculate total cross section of each nuclide /////////////
		cParticleState.p_dCellTmp = cGeometry.p_vCell[cParticleState.p_nCELL].p_dTemp;
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nNuclide = cMaterial.GetMatNucIndex(nCurrentMAT, i);   //

			int nSabNuclide = cMaterial.GetMatSabnucOfNuc(nCurrentMAT, i);

			if(cAceData.p_vNuclides[nNuclide].p_nNucInterpPos > 0) //// Already interpolated before crossing surface  (caution different tmp!)
			{
				if(nSabNuclide ==0 && !cParticleState.p_bIsCellTmpChanged)
				{
					continue;
				}
			} 

			if(nSabNuclide <= 0 || cParticleState.p_dErg >= cMaterial.GetMatNucSabesa(nCurrentMAT, i))   // MatSabNucEsa[CurrentMAT][nuclide] =0  for none-sab-nuclide
			{
				nSabNuclide = 0 ;
			}

			cAceData.GetNucTotFisCs(nNuclide, nSabNuclide,cParticleState.p_dErg,cParticleState.p_dCellTmp, cAceData.p_bUseProbTable);
		}


		///////////////////////////// sum up MacroTotCs ///////////////////////////
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nNuclide = cMaterial.GetMatNucIndex(nCurrentMAT, i);
			double dNucDen = cMaterial.GetMatNucAtomDen(nCurrentMAT, i);

			cParticleState.p_dMacroTotCs = cParticleState.p_dMacroTotCs + cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dTot * dNucDen;

			if(cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dFis <= 0)
			{
				continue;
			}
			cParticleState.p_dMacroMuFissCs = cParticleState.p_dMacroMuFissCs + cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dNu * cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dFis *dNucDen;
		}
	}



	//////////////////////////////// sample free flight distance /////////////////////////
	if(cParticleState.p_dMacroTotCs <= 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect macro total cross-section of material %d.\n",cMaterial.p_OMatSetIndex.GetIndexU(nCurrentMAT));},CDOutput::_WARNING);
		//cAceData.TreatURR(cMaterial.GetMatNucIndex(CurrentMAT, 0),cParticleState.Erg);
		cParticleState.p_dMacroTotCs = 1.0E-24;
		cParticleState.p_dMacroMuFissCs = 0 ;
	}



	return -log(ORNG.Rand())/cParticleState.p_dMacroTotCs;
}



double CDNeutronTransport::SampleFreeFlyDist(CDCriticality &cCriticality, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,bool IsErgChanged)
{
	//calculate the virtual total cross section
	double dNeuMass = 939.58;
	double dSlight = 299.7925;
	double dErg=0;

	if(cAceData.p_bIsMultiGroup)
	{
		dErg = cAceData.GetMgCentErg(int(cParticleState.p_dErg+0.5));
	}
	else
	{
		dErg = cParticleState.p_dErg;
	}

	double dVel = dSlight*sqrt(dErg*(dErg+2*dNeuMass))/(dErg+dNeuMass);
	if(cCriticality.p_bIsQuasiStaticD)
		cCriticality.p_dTimeAbsMacroCs = 1.0/dVel/cCriticality.p_dDeltaTime;
	else
		cCriticality.p_dTimeAbsMacroCs=0;



	/////////////////////// use previous MacroTotCs if available  ///////////////////////////
	if(!IsErgChanged) 
	{
		if(!cParticleState.p_bIsCellChanged)
		{
			return -log(ORNG.Rand())/(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs);
		}

		if(!cParticleState.p_bIsMatChanged && !cParticleState.p_bIsCellTmpChanged)
		{
			return -log(ORNG.Rand())/(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs);
		}
	}


	////////////////////////////////////// ordinary cases //////////////////////////////////////

	//////////////////////////////////  vacuum cell case   /////////////////////////////////
	int nCurrentMAT = cParticleState.p_nMAT;
	if(nCurrentMAT == 0)// vacuum
	{
		cParticleState.p_dMacroTotCs = 1.0E-24;
		cParticleState.p_dMacroMuFissCs = 0 ;
		return -log(ORNG.Rand())/(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs);
	}  


	/////////// recalulate Macro cross-section and sample free fight distance ///////////
	cParticleState.p_dMacroTotCs = 0;
	cParticleState.p_dMacroMuFissCs = 0;


	///////////////////////////// multigroup  case ///////////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nuc = cMaterial.GetMatNucIndex(nCurrentMAT, i);
			int IncidGrp = cParticleState.ErgGrp();
			double NucDen = cMaterial.GetMatNucAtomDen(nCurrentMAT, i);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot = cAceData.GetMgNucTotCs(nuc,IncidGrp);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis = cAceData.GetMgNucFisCs(nuc,IncidGrp);
			cAceData.p_vNuclides[nuc].p_ONucCs.p_dNu =cAceData.GetMgNucNubar(nuc,IncidGrp);
			cParticleState.p_dMacroTotCs = cParticleState.p_dMacroTotCs + cAceData.p_vNuclides[nuc].p_ONucCs.p_dTot * NucDen;
			cParticleState.p_dMacroMuFissCs = cParticleState.p_dMacroMuFissCs + cAceData.p_vNuclides[nuc].p_ONucCs.p_dNu * cAceData.p_vNuclides[nuc].p_ONucCs.p_dFis * NucDen;
		}
	}
	/////////////////////// continuous energy case //////////////////////
	else
	{
		if(IsErgChanged)  //// use last interpolation information if energy is not changed
		{
			for(int nNuc = 1; nNuc <= cMaterial.GetTotNucNumExcSab(); ++nNuc)
			{
				cAceData.p_vNuclides[nNuc].p_nNucInterpPos = -1;
			}
			if(cAceData.p_bUseErgBinMap) ////  accelerate binary search by energy bin map 
			{
				cAceData.p_OAceErgBinMap.GetBinMapIndex(cParticleState.p_dErg);
			}
		}

		////////// calculate total cross section of each nuclide /////////////
		cParticleState.p_dCellTmp = cGeometry.p_vCell[cParticleState.p_nCELL].p_dTemp;
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nNuclide = cMaterial.GetMatNucIndex(nCurrentMAT, i);   //

			int nSabNuclide = cMaterial.GetMatSabnucOfNuc(nCurrentMAT, i);

			if(cAceData.p_vNuclides[nNuclide].p_nNucInterpPos > 0) //// Already interpolated before crossing surface  (caution different tmp!)
			{
				if(nSabNuclide ==0 && !cParticleState.p_bIsCellTmpChanged)
				{
					continue;
				}
			} 

			if(nSabNuclide <= 0 || cParticleState.p_dErg >= cMaterial.GetMatNucSabesa(nCurrentMAT, i))   // MatSabNucEsa[CurrentMAT][nuclide] =0  for none-sab-nuclide
			{
				nSabNuclide = 0 ;
			}

			cAceData.GetNucTotFisCs(nNuclide, nSabNuclide,cParticleState.p_dErg,cParticleState.p_dCellTmp, cAceData.p_bUseProbTable);
		}


		///////////////////////////// sum up MacroTotCs ///////////////////////////
		for(int i = 0; i < cMaterial.GetMatTotNucNum(nCurrentMAT); ++i )
		{
			int nNuclide = cMaterial.GetMatNucIndex(nCurrentMAT, i);
			double dNucDen = cMaterial.GetMatNucAtomDen(nCurrentMAT, i);

			cParticleState.p_dMacroTotCs = cParticleState.p_dMacroTotCs + cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dTot * dNucDen;

			if(cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dFis <= 0)
			{
				continue;
			}
			cParticleState.p_dMacroMuFissCs = cParticleState.p_dMacroMuFissCs + cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dNu * cAceData.p_vNuclides[nNuclide].p_ONucCs.p_dFis *dNucDen;
		}
	}



	//////////////////////////////// sample free flight distance /////////////////////////
	if(cParticleState.p_dMacroTotCs <= 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect macro total cross-section of material %d.\n",cMaterial.p_OMatSetIndex.GetIndexU(nCurrentMAT));},CDOutput::_WARNING);
		//cAceData.TreatURR(cMaterial.GetMatNucIndex(CurrentMAT, 0),cParticleState.Erg);
		cParticleState.p_dMacroTotCs = 1.0E-24;
		cParticleState.p_dMacroMuFissCs = 0 ;
	}



	return -log(ORNG.Rand())/(cParticleState.p_dMacroTotCs+cCriticality.p_dTimeAbsMacroCs);
}