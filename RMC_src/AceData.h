# if ! defined __ACEDATA__H__
# define  __ACEDATA__H__

# include "CommonHead.h"
# include "Output.h"
# include "RNG.h"
# include "GlobeFun.h"
# include "MapClass.h"
# include "Nuclide.h"

using namespace std;

class CDNucId;
class CDNucCs;
class CDMaterial;

class CDAceData {
public:
	CDAceData()
	{
		p_bUseErgBinMap = true;
		p_bUseProbTable = true;  // use Probability table TreatURR resonance
		p_bIsMultiGroup = false;
	}

	//////////////////////////  read Ace data into arrays ///////////////////////////
	void InitiateAce(vector<CDNucId> NucID_list);
	void ClearData();
	void ReadAceData(int nTotNucNum, int nTotNonSabNucNum);  //

	////////////////////// Simple functions to get ACE paras ////////////////////////
	int GetXSSLength(int nNuc)           { return p_vNuclides[nNuc].GetXSSLength() ;}             // common ace: Length of second block of data ( XSS array)
	int GetNuclideZA(int nNuc)           { return p_vNuclides[nNuc].GetNuclideZA() ;}             // common ace: ZA = 1000*Z+1
	int GetErgGridNum(int nNuc)          { return p_vNuclides[nNuc].GetErgGridNum() ;}            // ce ace: Number of energies
	int GetNonElMtNum(int nNuc)          { return p_vNuclides[nNuc].GetNonElMtNum() ;}            // ce ace: Number of reactions excluding elastic
	int GetNonElMtNumWithNeu(int nNuc)   { return p_vNuclides[nNuc].GetNonElMtNumWithNeu() ;}     // ce ace: Number of reactions having secondary neutrons excluding elastic
	int GetNPCR(int nNuc)                { return p_vNuclides[nNuc].GetNPCR() ;}                  // ce ace: Number of delayed neutron precursor families
	int GetLocOfESZ(int nNuc)            { return p_vNuclides[nNuc].GetLocOfESZ() ;}              // ce ace: ESZ = Location of energy table
	int GetLocOfNU(int nNuc)             { return p_vNuclides[nNuc].GetLocOfNU() ;}               // ce ace: NU = Location of fission NU data
	int GetLocOfMTR(int nNuc)            { return p_vNuclides[nNuc].GetLocOfMTR() ;}              // ce ace: MTR = Location of MT array
	int GetLocOfLQR(int nNuc)            { return p_vNuclides[nNuc].GetLocOfLQR() ;}              // ce ace: LQR = Location of Q-value array
	int GetLocOfTYR(int nNuc)            { return p_vNuclides[nNuc].GetLocOfTYR() ;}              // ce ace: TYR = Location of reaction type array
	int GetLocOfLSIG(int nNuc)           { return p_vNuclides[nNuc].GetLocOfLSIG() ;}             // ce ace: LSIG = Location of table of cross-section locators
	int GetLocOfSIG(int nNuc)            { return p_vNuclides[nNuc].GetLocOfSIG() ;}              // ce ace: SIG = Location of cross sections
	int GetLocOfLAND(int nNuc)           { return p_vNuclides[nNuc].GetLocOfLAND() ;}             // ce ace: LAND = Location of table of angular distribution locators
	int GetLocOfAND(int nNuc)            { return p_vNuclides[nNuc].GetLocOfAND() ;}              // ce ace: AND = Location of angular distribution
	int GetLocOfLDLW(int nNuc)           { return p_vNuclides[nNuc].GetLocOfLDLW() ;}             // ce ace: LDLW = Location of table of energy distribution locators
	int GetLocOfDLW(int nNuc)            { return p_vNuclides[nNuc].GetLocOfDLW() ;}              // ce ace: DLW = Location of energy distribution
	int GetLocOfFIS(int nNuc)            { return p_vNuclides[nNuc].GetLocOfFIS() ;}              // ce ace: FIS = Location of total fission cross sections
	int GetLocOfLUNR(int nNuc)           { return p_vNuclides[nNuc].GetLocOfLUNR() ;}             // ce ace: LUNR = Location of probability tables
	int GetLocOfDNU(int nNuc)            { return p_vNuclides[nNuc].GetLocOfDNU() ;}              // ce ace: DNU = Location of delayed nubar
	int GetLocOfBDD(int nNuc)            { return p_vNuclides[nNuc].GetLocOfBDD() ;}              // ce ace: BDD = Location of basic delayed data
	int GetLocOfDNEDL(int nNuc)          { return p_vNuclides[nNuc].GetLocOfDNEDL() ;}            // ce ace: DENDL = Location of table of energy distribution locators
	int GetLocOfDNED(int nNuc)           { return p_vNuclides[nNuc].GetLocOfDNED() ;}             // ce ace: DNED = Location of energy distribution
	int GetEmissNeuNum(int nNuc, int nMT) { return p_vNuclides[nNuc].GetEmissNeuNum(nMT) ;};        // ce ace: get number of secondary neutrons
	double GetNucFisQ(int nNuc, int nMT)  { return p_vNuclides[nNuc].GetNucFisQ(nMT) ;};	         // ce ace: get fission Q value
	int GetSabInelMode(int SabNuc)      { return p_vNuclides[SabNuc].GetSabInelMode() ;};       // Sab ace: Inelastic scattering mode
	int GetSabInelDimPara(int SabNuc)   { return p_vNuclides[SabNuc].GetSabInelDimPara() ;};    // Sab ace: Inelastic dimensioning parameter
	int GetSabInelEoutNum(int SabNuc)   { return p_vNuclides[SabNuc].GetSabInelEoutNum() ;};    // Sab ace: Number of inelastic exiting energies
	int GetSabElMode(int SabNuc)        { return p_vNuclides[SabNuc].GetSabElMode() ;};         // Sab ace: Elastic scattering mode 
	int GetSabElDimPara(int SabNuc)     { return p_vNuclides[SabNuc].GetSabElDimPara() ;};      // Sab ace: Elastic dimensioning parameter 
	int GetSabSecErgMode(int SabNuc)    { return p_vNuclides[SabNuc].GetSabSecErgMode() ;};     // Sab ace: Secondary energy mode 
	int GetLocOfSabInelErg(int SabNuc)  { return p_vNuclides[SabNuc].GetLocOfSabInelErg() ;};   // Sab ace: ITIE = Location of inelastic energy table
	int GetLocOfSabInelXs(int SabNuc)   { return p_vNuclides[SabNuc].GetLocOfSabInelXs() ;};    // Sab ace: ITIX = location of inelastic cross sections
	int GetLocOfSabInelErgMu(int SabNuc){ return p_vNuclides[SabNuc].GetLocOfSabInelErgMu() ;}; // Sab ace: ITXE =location of inelastic energy/angle distributions
	int GetLocOfSabElErg(int SabNuc)    { return p_vNuclides[SabNuc].GetLocOfSabElErg() ;};     // Sab ace: ITCE = location of elastic energy table
	int GetLocOfSabElXs(int SabNuc)     { return p_vNuclides[SabNuc].GetLocOfSabElXs() ;};      // Sab ace: ITCX = location of elastic cross sections
	int GetLocOfSabElMu(int SabNuc)     { return p_vNuclides[SabNuc].GetLocOfSabElMu() ;};      // Sab ace: ITCA = location of elastic angular distributions
	int GetMgAngVarNum(int nNuc)         { return p_vNuclides[nNuc].GetMgAngVarNum() ;}           // Mg ace: NLEG = number of angular distribution variables
	int GetMgEditReacNum(int nNuc)       { return p_vNuclides[nNuc].GetMgEditReacNum() ;}         // Mg ace: NEDIT = number of edit reactions
	int GetMgGrpNum(int nNuc)            { return p_vNuclides[nNuc].GetMgGrpNum() ;}              // Mg ace: NGRP = number of groups
	int GetMgUpScatNum(int nNuc)         { return p_vNuclides[nNuc].GetMgUpScatNum() ;}           // Mg ace: NUS = number of upscatter groups
	int GetMgDownScatNum(int nNuc)       { return p_vNuclides[nNuc].GetMgDownScatNum() ;}         // Mg ace: NDS = number of downscatter groups
	int GetMgSecNum(int nNuc)            { return p_vNuclides[nNuc].GetMgSecNum() ;}              // Mg ace: NSEC = number of secondary particles
	int GetMgAngType(int nNuc)           { return p_vNuclides[nNuc].GetMgAngType() ;}             // Mg ace: ISANG = angular distribution type
	int GetMgNubarNum(int nNuc)          { return p_vNuclides[nNuc].GetMgNubarNum() ;}            // Mg ace: NNUBAR = number of nubars give
	int GetMgIncParId(int nNuc)          { return p_vNuclides[nNuc].GetMgIncParId() ;}            // Mg ace: IPT = Identifier for incident particle
	int GetMgLERG(int nNuc)              { return p_vNuclides[nNuc].GetMgLERG() ;}                // Mg ace: LERG = location of incident particle group structure = 1
	int GetMgLTOT(int nNuc)              { return p_vNuclides[nNuc].GetMgLTOT() ;}                // Mg ace: LTOT = location of total cross-sections
	int GetMgLFIS(int nNuc)              { return p_vNuclides[nNuc].GetMgLFIS() ;}                // Mg ace: LFIS = location of fission cross-sections
	int GetMgLNU(int nNuc)               { return p_vNuclides[nNuc].GetMgLNU() ;}                 // Mg ace: LNU = location of nubar data
	int GetMgLCHI(int nNuc)              { return p_vNuclides[nNuc].GetMgLCHI() ;}                // Mg ace: LCHI = location of fission chi data
	int GetMgLABS(int nNuc)              { return p_vNuclides[nNuc].GetMgLABS() ;}                // Mg ace: LABS = location of absorption cross-sections
	int GetMgLP0L(int nNuc)              { return p_vNuclides[nNuc].GetMgLP0L() ;}                // Mg ace: LP0L = location of P0 locators
	int GetMgLXPNL(int nNuc)             { return p_vNuclides[nNuc].GetMgLXPNL() ;}               // Mg ace: LXPNL = location of XPn locators
	int GetMgLPNL(int nNuc)              { return p_vNuclides[nNuc].GetMgLPNL() ;}                // Mg ace: LPNL = location of Pn locators
	//New
	//int GetNucZaid(int nNuc);                //
	//NucCsClass GetNucCs(int nNuc);

	/////////////////////// functions for continuous-energy calculation /////////////////////////
	void CheckCeAceBlock(int nTotNucNum, int nTotNonSabNucNum);  //
	void CalcThermGfun();
	int DpplrBrdnNucTmp(int nNuc, double dTmp);  // CellMatIndex MatNucIndex MatTotNucNum
	double Erf2Function(double a);
	void GetNucTotFisCs(int nNuc, int nSabNuc, double dEg0, double dCell_T, bool bUseProbTable);
	void GetNucAbsScattCs(int nNuc, double dEg0, int nInterpPos0, double dInterpFrac0, int nInterpPos, double dInterpFrac);
	double GetNucMtCs(int nNuc, int nMT, int nInterpPos,double dInterpFrac);
	void InterpolateSab(int nNuc_n,int nNuc_sab, double dEg0);
	void TreatURR(int nNuc,double dEg0);
	void GetCeExitErgMu(int nNuc,int nMT,double dIncidErg,double &dExitErg_Lab,double &dMu_Lab);
	double GetScattCosine(int nNuc,int nMT,double dIncidErg);
	void GetLawType(int nNuc,int nMT,double nIncidErg,int &nLawType, int &LDAT);
	void ReactByLaws(int nNuc,int nMT,int nLawType, int LDAT,double dEin, double &dExitErgInCm,double &dMuInCm);
	void TransformCmToLab(int nNuc,int nMT,double dEin,double dExitErgInCm,double dMuInCm,double &dExitErgInLab,double &dMuInLab);
	/*void TreatSabColliType(int nSabColliNuc, double dSIG_sab_el, double dSIG_sab_inel,double dEin,double dDirectin[3], double &dExitErgInLab,double dDirectout[3],CDCriticality &cCriticality);
	void TreatSabColliType(int SabColliNuc, double SIG_sab_el, double SIG_sab_inel,double Ein,double Directin[3], double &ExitErgInLab,double Directout[3],CDFixedSource &cFixedSource);*/
	void TreatSabColliType(int nSabColliNuc, double dSIG_sab_el, double dSIG_sab_inel,double dEin,double dDirectin[3], double &dExitErgInLab,double dDirectout[3],CDNeutronTransport &cNeutronTransport);
	double GetTotalNu(double dEg0, int nNuc);
	double GetDelayedNu(double dEg0, int nNuc);
	void InterpolateXssTab(double dErg, int nNuc, int LDAT,int &nPos,double &dFrac,int &NR, int &NE);
	double GetErgFuncValue(int nNuc, int L,double dErg);
	double SampleMaxwell(double T);
	double SampleWatt(double a, double b);
	void GenUnionErgGrid(CDMaterial &cMaterial);
	void GenErgBinMap(CDMaterial &cMaterial);  // use binary map based on fexp function.  
	void GetErgBinMapPtr(int nNuc,int &min,int &max);

	/////////////////////////// functions for multi-group case calculation ////////////////////////////
	void CheckMgAceBlock(int nTotNonSabNucNum);
	double GetMgNucTotCs(int nNuc, int nIncidGrp);
	double GetMgNucFisCs(int nNuc, int nIncidGrp);
	double GetMgNucNubar(int nNuc, int nIncidGrp);
	double GetMgNucFisChi(int nNuc, int nIncidGrp);
	double GetMgNucAbsCs(int nNuc, int nIncidGrp);
	double GetMgNucP0Cs(int nNuc, int nIncidGrp);
	int GetMgNucP0Loc(int nNuc, int nIncidGrp);
	void GetMgExitErgMu(int nNuc, double dIncidErg, double &dExitErg, double &dExitMu); 
	void GetMgFisErgDir(int nNuc, double &dExitErg, double dDir[3]); 
	double  GetMgCentErg(int nErgGrp);

public:
	vector< CDNuclide> p_vNuclides;
/*
	vector<NucIdClass> NucID;
	vector<double> NucAtomWgt;
	vector<double> NucTmp,NucBroadTmp;
	vector<int> NucZaid;
	vector<vector<double> > XSS;
	vector<vector<int> >  NXS;
	vector<vector<int> >  JXS;
	vector<vector<int> >  MTRIndex,LSIG,LAND,LDLW;
	vector<vector<double> > FisXSS, InelXSS;
	vector<NucCsClass> NucCs;
	vector<int> NucInterpPos;
	vector<double> NucInterpFrac;
	vector<int> ProbTableFlag;
*/
	double p_dThermFunc[52];

	//////uniform energy gird////////
	vector<double> p_vUnionEnergyGrid;
	int p_nUnionEgNum;  // number of union energy grid

	////////// use binary map to accelerate energy-grid searching /////////
	bool p_bUseProbTable;
	bool p_bUseErgBinMap;
	vector<vector<CDBinPtr> > p_vAceErgBinPtr;
	CDBinMap p_OAceErgBinMap;

	////////////////////// Multigroup ACE data ////////////////////////////
	bool p_bIsMultiGroup;
	int p_nMltGrpNum;	
    vector<vector<int> > p_vMgP0Loc;      // Start location of P0 block for individual group
	vector<vector<double> > p_vMgP0Cs;    // Total P0 scattering cross-section for individual group
	vector<double> p_vMltErgBins;         // Energy lower bound for individual group.
	vector<double> p_vMltCentErg;


	//precursor properties
	double p_vBeta[7];
	double p_vLambda[7];

	///////////////////freegas treatment/////////////////////
	void DpplrBrdnNuc_totcs(int nNuc, double dCell_T,double dEg0);
	void DpplerBrdnNucAbsScatt(int nNuc, double dEg0);
};

//extern AceDataClass AceData;

#endif
