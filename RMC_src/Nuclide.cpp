# include "Nuclide.h"


int CDNuclide::GetEmissNeuNum(int nMT)
{
	if(nMT == 2) { return 1; }
	return int(XSS[GetLocOfTYR() + MTRIndex[nMT]-1]);
}

double CDNuclide::GetNucFisQ(int nMT)     // LQR = Location of Q-value array
{
	if(MTRIndex[nMT] <= 0) { return 0; }
	return XSS[GetLocOfLQR() + MTRIndex[nMT] - 1];
}

int CDNuclide::GetMgLNU()   // Mg ace: LNU = location of nubar data
{
	if( GetMgNubarNum() > 1)
	{
		return JXS[4] + GetMgGrpNum();
	}
	return JXS[4];
}
/*
inline int CD_Nuclide::GetXSSLength()       // Length of second block of data ( XSS array)
{
	return NXS[1];
}

inline int CD_Nuclide::GetNuclideZA()       // ZA=1000*Z+1
{
	return NXS[2];
}

inline int CD_Nuclide::GetErgGridNum()      // ce ace: Number of energies
{
	return NXS[3];
}

inline int CD_Nuclide::GetNonElMtNum()      // ce ace: Number of reactions excluding elastic
{
	return NXS[4];
}

inline int CD_Nuclide::GetNonElMtNumWithNeu() // ce ace: Number of reactions having secondary neutrons excluding elastic
{
	return NXS[5];
}

inline int CD_Nuclide::GetNPCR()
{
	return NXS[8];
}

inline int CD_Nuclide::GetLocOfESZ()        // ce ace: ESZ = Location of energy table
{
	return JXS[1];
}

inline int CD_Nuclide::GetLocOfNU()         // ce ace: NU = Location of fission NU data
{
	return JXS[2];
}

inline int CD_Nuclide::GetLocOfMTR()        // ce ace: MTR = Location of MT array
{
	return JXS[3];
}

inline int CD_Nuclide::GetLocOfLQR()        // ce ace: LQR = Location of Q-value array
{
	return JXS[4];
}

inline int CD_Nuclide::GetLocOfTYR()        // ce ace: TYR = Location of reaction type array
{
	return JXS[5];
}

inline int CD_Nuclide::GetLocOfLSIG()       // ce ace: LSIG = Location of table of cross-section locators
{
	return JXS[6];
}

inline int CD_Nuclide::GetLocOfSIG()        // ce ace: SIG = Location of cross sections
{
	return JXS[7];
}

inline int CD_Nuclide::GetLocOfLAND()      // ce ace: LAND = Location of table of angular distribution locators
{
	return JXS[8];
}

inline int CD_Nuclide::GetLocOfAND()       // ce ace: AND = Location of angular distribution
{
	return JXS[9];
}

inline int CD_Nuclide::GetLocOfLDLW()      // ce ace: LDLW = Location of table of energy distribution locators
{
	return JXS[10];
}

inline int CD_Nuclide::GetLocOfDLW()       // ce ace: DLW = Location of energy distribution
{
	return JXS[11];
}

inline int CD_Nuclide::GetLocOfFIS()       // ce ace: FIS = Location of total fission cross sections
{
	return JXS[21];
}

inline int CD_Nuclide::GetLocOfLUNR()      // ce ace: LUNR = Location of probability tables
{
	return JXS[23];
}

inline int CD_Nuclide::GetLocOfDNU()              // ce ace: DNU = Location of delayed nubar
{
	return JXS[24];
}

inline int CD_Nuclide::GetLocOfBDD()              // ce ace: BDD = Location of basic delayed data
{
	return JXS[25];
}

inline int CD_Nuclide::GetLocOfDNEDL()            // ce ace: DENDL = Location of table of energy distribution locators
{
	return JXS[26];
}

inline int CD_Nuclide::GetLocOfDNED()             // ce ace: DNED = Location of energy distribution
{
	return JXS[27];
}

inline int CD_Nuclide::GetSabInelMode()      // Sab ace: Inelastic scattering mode
{
	return NXS[2];
}

inline int CD_Nuclide::GetSabInelDimPara()   // Sab ace: Inelastic dimensioning parameter
{
	return NXS[3];
}

inline int CD_Nuclide::GetSabInelEoutNum()   // Sab ace: Number of inelastic exiting energies
{
	return NXS[4];
}

inline int CD_Nuclide::GetSabElMode()        // Sab ace: Elastic scattering mode 
{
	return NXS[5];
}

inline int CD_Nuclide::GetSabElDimPara()     // Sab ace: Elastic dimensioning parameter 
{
	return NXS[6];
}

inline int CD_Nuclide::GetSabSecErgMode()    // Sab ace: Secondary energy mode 
{
	return NXS[7];
}

inline int CD_Nuclide::GetLocOfSabInelErg()     // Sab ace: ITIE = Location of inelastic energy table
{
	return JXS[1];
}

inline int CD_Nuclide::GetLocOfSabInelXs()     // Sab ace: ITIX = location of inelastic cross sections
{
	return JXS[2];
}

inline int CD_Nuclide::GetLocOfSabInelErgMu()     // Sab ace: ITXE =location of inelastic energy/angle distributions
{
	return JXS[3];
}

inline int CD_Nuclide::GetLocOfSabElErg()     // Sab ace: ITCE = location of elastic energy table
{
	return JXS[4];
}

inline int CD_Nuclide::GetLocOfSabElXs()     // Sab ace: ITCX = location of elastic cross sections
{
	return JXS[5];
}

inline int CD_Nuclide::GetLocOfSabElMu()     // Sab ace: ITCA = location of elastic angular distributions
{
	return JXS[6];
}

inline int CD_Nuclide::GetMgAngVarNum()    // Mg ace: NLEG = number of angular distribution variables
{
	return NXS[3];
}

inline int CD_Nuclide::GetMgEditReacNum()  // Mg ace: NEDIT = number of edit reactions
{
	return NXS[4];
}

inline int CD_Nuclide::GetMgGrpNum()       // Mg ace: NGRP = number of groups
{
	return NXS[5];
}

inline int CD_Nuclide::GetMgUpScatNum()    // Mg ace: NUS = number of upscatter groups
{
	return NXS[6];
}

inline int CD_Nuclide::GetMgDownScatNum()  // Mg ace: NDS = number of downscatter groups
{
	return NXS[7];
}

inline int CD_Nuclide::GetMgSecNum()       // Mg ace: NSEC = number of secondary particles
{
	return NXS[8];
}

inline int CD_Nuclide::GetMgAngType()      // Mg ace: ISANG = angular distribution type
{
	return NXS[9];
}

inline int CD_Nuclide::GetMgNubarNum()     // Mg ace: NNUBAR = number of nubars give
{
	return NXS[10];
}

inline int CD_Nuclide::GetMgIncParId()     // Mg ace: IPT = Identifier for incident particle
{
	return NXS[12];
}

inline int CD_Nuclide::GetMgLERG()  // Mg ace: LERG = location of incident particle group structure = 1
{
	return JXS[1];
}

inline int CD_Nuclide::GetMgLTOT()  // Mg ace: LTOT = location of total cross-sections
{
	return JXS[2];
}

inline int CD_Nuclide::GetMgLFIS()  // Mg ace: LFIS = location of fission cross-sections
{
	return JXS[3];
}
*/



/*
inline int CD_Nuclide::GetMgLCHI()  // Mg ace: LCHI = location of fission chi data
{
	return JXS[5];
}

inline int CD_Nuclide::GetMgLABS()  // Mg ace: LABS = location of absorption cross-sections
{
	return JXS[6];
}

inline int CD_Nuclide::GetMgLP0L()  // Mg ace: LP0L = location of P0 locators
{
	return JXS[13];
}

inline int CD_Nuclide::GetMgLXPNL() // Mg ace: LXPNL = location of XPn locators
{
	return JXS[16];
}

inline int CD_Nuclide::GetMgLPNL()  // Mg ace: LPNL = location of Pn locators
{
	return JXS[17];
}*/

/*
inline int CD_Nuclide::GetNucZaid()
{
	return NucZaid;
}

NucCsClass CD_Nuclide::GetNucCs()
{
	return NucCs;
}*/

