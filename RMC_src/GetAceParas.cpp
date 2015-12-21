# include "AceData.h"

/*
inline int AceDataClass::GetXSSLength(int nuclide)       // Length of second block of data ( XSS array)
{
	return Nuclides[nuclide].GetXSSLength();
}

inline int AceDataClass::GetNuclideZA(int nuclide)       // ZA=1000*Z+1
{
	return Nuclides[nuclide].GetNuclideZA();
}

inline int AceDataClass::GetErgGridNum(int nuclide)      // ce ace: Number of energies
{
	return Nuclides[nuclide].GetErgGridNum();
}

inline int AceDataClass::GetNonElMtNum(int nuclide)      // ce ace: Number of reactions excluding elastic
{
	return Nuclides[nuclide].GetNonElMtNum();
}

inline int AceDataClass::GetNonElMtNumWithNeu(int nuclide) // ce ace: Number of reactions having secondary neutrons excluding elastic
{
	return Nuclides[nuclide].GetNonElMtNumWithNeu();
}

inline int AceDataClass::GetNPCR(int nuclide)
{
	return Nuclides[nuclide].GetNPCR();
}

inline int AceDataClass::GetLocOfESZ(int nuclide)        // ce ace: ESZ = Location of energy table
{
	return Nuclides[nuclide].GetLocOfESZ();
}

inline int AceDataClass::GetLocOfNU(int nuclide)         // ce ace: NU = Location of fission NU data
{
	return Nuclides[nuclide].GetLocOfNU();
}

inline int AceDataClass::GetLocOfMTR(int nuclide)        // ce ace: MTR = Location of MT array
{
	return Nuclides[nuclide].GetLocOfMTR();
}

inline int AceDataClass::GetLocOfLQR(int nuclide)        // ce ace: LQR = Location of Q-value array
{
	return Nuclides[nuclide].GetLocOfLQR();
}

inline int AceDataClass::GetLocOfTYR(int nuclide)        // ce ace: TYR = Location of reaction type array
{
	return Nuclides[nuclide].GetLocOfTYR();
}

inline int AceDataClass::GetLocOfLSIG(int nuclide)       // ce ace: LSIG = Location of table of cross-section locators
{
	return Nuclides[nuclide].GetLocOfLSIG();
}

inline int AceDataClass::GetLocOfSIG(int nuclide)        // ce ace: SIG = Location of cross sections
{
	return Nuclides[nuclide].GetLocOfSIG();
}

inline int AceDataClass::GetLocOfLAND(int nuclide)      // ce ace: LAND = Location of table of angular distribution locators
{
	return Nuclides[nuclide].GetLocOfLAND();
}

inline int AceDataClass::GetLocOfAND(int nuclide)       // ce ace: AND = Location of angular distribution
{
	return Nuclides[nuclide].GetLocOfAND();
}

inline int AceDataClass::GetLocOfLDLW(int nuclide)      // ce ace: LDLW = Location of table of energy distribution locators
{
	return Nuclides[nuclide].GetLocOfLDLW();
}

inline int AceDataClass::GetLocOfDLW(int nuclide)       // ce ace: DLW = Location of energy distribution
{
	return Nuclides[nuclide].GetLocOfDLW();
}

inline int AceDataClass::GetLocOfFIS(int nuclide)       // ce ace: FIS = Location of total fission cross sections
{
	return Nuclides[nuclide].GetLocOfFIS();
}

inline int AceDataClass::GetLocOfLUNR(int nuclide)      // ce ace: LUNR = Location of probability tables
{
	return Nuclides[nuclide].GetLocOfLUNR();
}

inline int AceDataClass::GetLocOfDNU(int nuclide)              // ce ace: DNU = Location of delayed nubar
{
	return Nuclides[nuclide].GetLocOfDNU();
}

inline int AceDataClass::GetLocOfBDD(int nuclide)              // ce ace: BDD = Location of basic delayed data
{
	return Nuclides[nuclide].GetLocOfBDD();
}

inline int AceDataClass::GetLocOfDNEDL(int nuclide)            // ce ace: DENDL = Location of table of energy distribution locators
{
	return Nuclides[nuclide].GetLocOfDNEDL();
}

inline int AceDataClass::GetLocOfDNED(int nuclide)             // ce ace: DNED = Location of energy distribution
{
	return Nuclides[nuclide].GetLocOfDNED();
}

inline int AceDataClass::GetEmissNeuNum(int nuclide, int mt)
{
	/ *if(mt == 2) { return 1; }
	return int(Nuclides[nuclide].XSS[GetLocOfTYR(nuclide) + MTRIndex[nuclide][mt]-1]);* /
	return Nuclides[nuclide].GetEmissNeuNum( mt);
}

double AceDataClass::GetNucFisQ(int nuclide, int mt)     // LQR = Location of Q-value array
{
	/ *if(MTRIndex[nuc][mt] <= 0) { return 0; }
	return Nuclides[nuc].XSS[GetLocOfLQR(nuc) + MTRIndex[nuc][mt] - 1];* /
	return Nuclides[nuclide].GetNucFisQ( mt);
}

inline int AceDataClass::GetSabInelMode(int SabNuclide)      // Sab ace: Inelastic scattering mode
{
	return Nuclides[SabNuclide].GetSabInelMode();
}

inline int AceDataClass::GetSabInelDimPara(int SabNuclide)   // Sab ace: Inelastic dimensioning parameter
{
	return Nuclides[SabNuclide].GetSabInelDimPara();
}

inline int AceDataClass::GetSabInelEoutNum(int SabNuclide)   // Sab ace: Number of inelastic exiting energies
{
	return Nuclides[SabNuclide].GetSabInelEoutNum();
}

inline int AceDataClass::GetSabElMode(int SabNuclide)        // Sab ace: Elastic scattering mode 
{
	return Nuclides[SabNuclide].GetSabElMode();
}

inline int AceDataClass::GetSabElDimPara(int SabNuclide)     // Sab ace: Elastic dimensioning parameter 
{
	return Nuclides[SabNuclide].GetSabElDimPara();
}

inline int AceDataClass::GetSabSecErgMode(int SabNuclide)    // Sab ace: Secondary energy mode 
{
	return Nuclides[SabNuclide].GetSabSecErgMode();
}

inline int AceDataClass::GetLocOfSabInelErg(int SabNuclide)     // Sab ace: ITIE = Location of inelastic energy table
{
	return Nuclides[SabNuclide].GetLocOfSabInelErg();
}

inline int AceDataClass::GetLocOfSabInelXs(int SabNuclide)     // Sab ace: ITIX = location of inelastic cross sections
{
	return Nuclides[SabNuclide].GetLocOfSabInelXs();
}

inline int AceDataClass::GetLocOfSabInelErgMu(int SabNuclide)     // Sab ace: ITXE =location of inelastic energy/angle distributions
{
	return Nuclides[SabNuclide].GetLocOfSabInelErgMu();
}

inline int AceDataClass::GetLocOfSabElErg(int SabNuclide)     // Sab ace: ITCE = location of elastic energy table
{
	return Nuclides[SabNuclide].GetLocOfSabElErg();
}

inline int AceDataClass::GetLocOfSabElXs(int SabNuclide)     // Sab ace: ITCX = location of elastic cross sections
{
	return Nuclides[SabNuclide].GetLocOfSabElXs();
}

inline int AceDataClass::GetLocOfSabElMu(int SabNuclide)     // Sab ace: ITCA = location of elastic angular distributions
{
	return Nuclides[SabNuclide].GetLocOfSabElMu();
}

inline int AceDataClass::GetMgAngVarNum(int nuclide)    // Mg ace: NLEG = number of angular distribution variables
{
	return Nuclides[nuclide].GetMgAngVarNum();
}

inline int AceDataClass::GetMgEditReacNum(int nuclide)  // Mg ace: NEDIT = number of edit reactions
{
	return Nuclides[nuclide].GetMgEditReacNum();
}

inline int AceDataClass::GetMgGrpNum(int nuclide)       // Mg ace: NGRP = number of groups
{
	return Nuclides[nuclide].GetMgGrpNum();
}

inline int AceDataClass::GetMgUpScatNum(int nuclide)    // Mg ace: NUS = number of upscatter groups
{
	return Nuclides[nuclide].GetMgUpScatNum();
}

inline int AceDataClass::GetMgDownScatNum(int nuclide)  // Mg ace: NDS = number of downscatter groups
{
	return Nuclides[nuclide].GetMgDownScatNum();
}

inline int AceDataClass::GetMgSecNum(int nuclide)       // Mg ace: NSEC = number of secondary particles
{
	return Nuclides[nuclide].GetMgSecNum();
}

inline int AceDataClass::GetMgAngType(int nuclide)      // Mg ace: ISANG = angular distribution type
{
	return Nuclides[nuclide].GetMgAngType();
}

inline int AceDataClass::GetMgNubarNum(int nuclide)     // Mg ace: NNUBAR = number of nubars give
{
	return Nuclides[nuclide].GetMgNubarNum();
}

inline int AceDataClass::GetMgIncParId(int nuclide)     // Mg ace: IPT = Identifier for incident particle
{
	return Nuclides[nuclide].GetMgIncParId();
}

inline int AceDataClass::GetMgLERG(int nuclide)  // Mg ace: LERG = location of incident particle group structure = 1
{
	return Nuclides[nuclide].GetMgLERG();
}

inline int AceDataClass::GetMgLTOT(int nuclide)  // Mg ace: LTOT = location of total cross-sections
{
	return Nuclides[nuclide].GetMgLTOT();
}

inline int AceDataClass::GetMgLFIS(int nuclide)  // Mg ace: LFIS = location of fission cross-sections
{
	return Nuclides[nuclide].GetMgLFIS();
}

inline int AceDataClass::GetMgLNU(int nuclide)   // Mg ace: LNU = location of nubar data
{
	/ *if( GetMgNubarNum(nuc) > 1)
	{
		return JXS[nuc][4] + GetMgGrpNum(nuc);
	}
	return JXS[nuc][4];* /
	return Nuclides[nuclide].GetMgLNU();
}

inline int AceDataClass::GetMgLCHI(int nuclide)  // Mg ace: LCHI = location of fission chi data
{
	return Nuclides[nuclide].GetMgLCHI();
}

inline int AceDataClass::GetMgLABS(int nuclide)  // Mg ace: LABS = location of absorption cross-sections
{
	return Nuclides[nuclide].GetMgLABS();
}

inline int AceDataClass::GetMgLP0L(int nuclide)  // Mg ace: LP0L = location of P0 locators
{
	return Nuclides[nuclide].GetMgLP0L();
}

inline int AceDataClass::GetMgLXPNL(int nuclide) // Mg ace: LXPNL = location of XPn locators
{
	return Nuclides[nuclide].GetMgLXPNL();
}

inline int AceDataClass::GetMgLPNL(int nuclide)  // Mg ace: LPNL = location of Pn locators
{
	return Nuclides[nuclide].GetMgLPNL();
}*/
/*

//New
int AceDataClass::GetNucZaid(int nuclide)
{
	return Nuclides[nuclide].GetNucZaid();
}

NucCsClass AceDataClass::GetNucCs(int nuclide)
{
	return Nuclides[nuclide].GetNucCs();
}*/

