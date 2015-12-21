#ifndef __NULICDE__H__
#define __NULICDE__H__

# include "CommonHead.h"

using namespace std;

class CDNucId 
{// string is not good for parallel, so using char[]
public:
	char p_chIdStr[12];
};

class CDNucCs {
public:
	double p_dTot;
	double p_dAbs;
	double p_dEl;
	double p_dInel;
	double p_dFis;
	double p_dNu;
};

class CDNuclide
{
public:
	////////////////////// Simple functions to get ACE paras ////////////////////////
	int GetXSSLength()           { return NXS[ 1 ];}     // common ace: Length of second block of data ( XSS array)
	int GetNuclideZA()           { return NXS[ 2 ];}     // common ace: ZA = 1000*Z+1
	int GetErgGridNum()          { return NXS[ 3 ];}     // ce ace: Number of energies
	int GetNonElMtNum()          { return NXS[ 4 ];}     // ce ace: Number of reactions excluding elastic
	int GetNonElMtNumWithNeu()   { return NXS[ 5 ];}     // ce ace: Number of reactions having secondary neutrons excluding elastic
	int GetNPCR()                { return NXS[ 8 ];}     // ce ace: Number of delayed neutron precursor families
	int GetLocOfESZ()            { return JXS[ 1 ];}     // ce ace: ESZ = Location of energy table
	int GetLocOfNU()             { return JXS[ 2 ];}     // ce ace: NU = Location of fission NU data
	int GetLocOfMTR()            { return JXS[ 3 ];}     // ce ace: MTR = Location of MT array
	int GetLocOfLQR()            { return JXS[ 4 ];}     // ce ace: LQR = Location of Q-value array
	int GetLocOfTYR()            { return JXS[ 5 ];}     // ce ace: TYR = Location of reaction type array
	int GetLocOfLSIG()           { return JXS[ 6 ];}     // ce ace: LSIG = Location of table of cross-section locators
	int GetLocOfSIG()            { return JXS[ 7 ];}     // ce ace: SIG = Location of cross sections
	int GetLocOfLAND()           { return JXS[ 8 ];}     // ce ace: LAND = Location of table of angular distribution locators
	int GetLocOfAND()            { return JXS[ 9 ];}     // ce ace: AND = Location of angular distribution
	int GetLocOfLDLW()           { return JXS[ 10];}     // ce ace: LDLW = Location of table of energy distribution locators
	int GetLocOfDLW()            { return JXS[ 11];}     // ce ace: DLW = Location of energy distribution
	int GetLocOfFIS()            { return JXS[ 21];}     // ce ace: FIS = Location of total fission cross sections
	int GetLocOfLUNR()           { return JXS[ 23];}     // ce ace: LUNR = Location of probability tables
	int GetLocOfDNU()            { return JXS[ 24];}     // ce ace: DNU = Location of delayed nubar
	int GetLocOfBDD()            { return JXS[ 25];}     // ce ace: BDD = Location of basic delayed data
	int GetLocOfDNEDL()          { return JXS[ 26];}     // ce ace: DENDL = Location of table of energy distribution locators
	int GetLocOfDNED()           { return JXS[ 27];}     // ce ace: DNED = Location of energy distribution
	int GetEmissNeuNum(int nMT);                          // ce ace: get number of secondary neutrons
	double GetNucFisQ(int nMT);                           // ce ace: get fission Q value
	int GetSabInelMode()         { return NXS[ 2 ];}     // Sab ace: Inelastic scattering mode
	int GetSabInelDimPara()      { return NXS[ 3 ];}     // Sab ace: Inelastic dimensioning parameter
	int GetSabInelEoutNum()      { return NXS[ 4 ];}     // Sab ace: Number of inelastic exiting energies
	int GetSabElMode()           { return NXS[ 5 ];}     // Sab ace: Elastic scattering mode 
	int GetSabElDimPara()        { return NXS[ 6 ];}     // Sab ace: Elastic dimensioning parameter 
	int GetSabSecErgMode()       { return NXS[ 7 ];}     // Sab ace: Secondary energy mode 
	int GetLocOfSabInelErg()     { return JXS[ 1 ];}     // Sab ace: ITIE = Location of inelastic energy table
	int GetLocOfSabInelXs()      { return JXS[ 2 ];}     // Sab ace: ITIX = location of inelastic cross sections
	int GetLocOfSabInelErgMu()   { return JXS[ 3 ];}     // Sab ace: ITXE =location of inelastic energy/angle distributions
	int GetLocOfSabElErg()       { return JXS[ 4 ];}     // Sab ace: ITCE = location of elastic energy table
	int GetLocOfSabElXs()        { return JXS[ 5 ];}     // Sab ace: ITCX = location of elastic cross sections
	int GetLocOfSabElMu()        { return JXS[ 6 ];}     // Sab ace: ITCA = location of elastic angular distributions
	int GetMgAngVarNum()         { return NXS[ 3 ];}     // Mg ace: NLEG = number of angular distribution variables
	int GetMgEditReacNum()       { return NXS[ 4 ];}     // Mg ace: NEDIT = number of edit reactions
	int GetMgGrpNum()            { return NXS[ 5 ];}     // Mg ace: NGRP = number of groups
	int GetMgUpScatNum()         { return NXS[ 6 ];}     // Mg ace: NUS = number of upscatter groups
	int GetMgDownScatNum()       { return NXS[ 7 ];}     // Mg ace: NDS = number of downscatter groups
	int GetMgSecNum()            { return NXS[ 8 ];}     // Mg ace: NSEC = number of secondary particles
	int GetMgAngType()           { return NXS[ 9 ];}     // Mg ace: ISANG = angular distribution type
	int GetMgNubarNum()          { return NXS[ 10];}     // Mg ace: NNUBAR = number of nubars give
	int GetMgIncParId()          { return NXS[ 12];}     // Mg ace: IPT = Identifier for incident particle
	int GetMgLERG()              { return JXS[ 1 ];}     // Mg ace: LERG = location of incident particle group structure = 1
	int GetMgLTOT()              { return JXS[ 2 ];}     // Mg ace: LTOT = location of total cross-sections
	int GetMgLFIS()              { return JXS[ 3 ];}     // Mg ace: LFIS = location of fission cross-sections
	int GetMgLNU();                                      // Mg ace: LNU = location of nubar data
	int GetMgLCHI()              { return JXS[ 5 ];}     // Mg ace: LCHI = location of fission chi data
	int GetMgLABS()              { return JXS[ 6 ];}     // Mg ace: LABS = location of absorption cross-sections
	int GetMgLP0L()              { return JXS[ 13];}     // Mg ace: LP0L = location of P0 locators
	int GetMgLXPNL()             { return JXS[ 16];}     // Mg ace: LXPNL = location of XPn locators
	int GetMgLPNL()              { return JXS[ 17];}     // Mg ace: LPNL = location of Pn locators

	//int GetNucZaid();
	//NucCsClass GetNucCs();

public:
	CDNucId p_ONucID;
	double p_dNucAtomWgt;
	double p_dNucTmp,p_dNucBroadTmp;
	int p_nNucZaid;
	vector<double>  XSS;
	vector<int>   NXS;
	vector<int>   JXS;
	vector<int>   MTRIndex,LSIG,LAND,LDLW;
	vector<double>  FisXSS, InelXSS;
	CDNucCs p_ONucCs;
	int p_nNucInterpPos;
	double p_dNucInterpFrac;
	int p_nProbTableFlag;

	CDNuclide() // initialization
	{
		p_nNucZaid = 0;
		p_dNucAtomWgt = 0.0;
		p_dNucTmp = 0.0;
		p_dNucBroadTmp = 0.0;
		p_nNucInterpPos = 0;
		p_dNucInterpFrac= 0.0;
		p_nProbTableFlag = 0;
	}
};


#endif///define__NULICDE__H__