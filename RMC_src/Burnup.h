# if ! defined __Burn__H__
# define  __Burn__H__
 
# include "CommonHead.h"
# include "Output.h"
# include "Tally.h"
#include "Depth_Class.h"

using namespace std;

class CDVaryOneMat{
public:
	int p_nMatIndex; // VaryMat
	int p_nNewMatIndex;// NewMatIndex
	vector<int> p_nExpandMatIndex; // expanded vary mats, for example mat 1 expand to 5-10
};

class CDBurnVaryMat{
public:
	int p_nBurnStep;
	vector<CDVaryOneMat> p_vVaryMats;
};

class CDBurnup {
public:
	////////// Public functions/////////////
	CDBurnup();
	void ExpdBurnableCell(CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally,CDAceData &cAceData); 
	void CheckDepletionNuc(CDGeometry &cGeometry,CDMaterial &cMaterial);

	void InitiateBurnup(CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally);
	void CheckOutputCells(CDGeometry &cGeometry,CDTally &cTally);
	void CalcCellPower(CDMaterial &cMaterial,CDTally &cTally);    // Calculate power distribution of overall burnable cells
	void GetCellAveEgy(CDTally &cTally);    // Get average energy of overall burnable cells

	void CoupleTrsptBurn(CDMaterial &cMaterial,CDTally &cTally);
	void RMC_to_DEPTH(CDMaterial &cMaterial,CDTally &cTally,int nBurnCellIndex);
	void DEPTH_to_RMC(CDMaterial &cMaterial,CDTally &cTally,int nBurnCellIndex);
	void AddBurnNucToMat(int nNucId, double dNucDen, CDSingleMat &stMatStruct);
    void SortFraction(vector<double> &dArrayData, int nIb, int nIc, vector<int> &dArraySeq);
	void RestBurnup(CDNeutronTransport &cNeutronTransport,CDCriticality &cCriticality,CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally,CDAceData &cAceData,CDConvergence &cConvergence);
	void UpdateBurnedMatNucDen(vector<CDNuclide>& vNuclides,CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally);

	void OutputBurnup(CDMaterial &cMaterial,CDTally &cTally,CDCriticality & cCriticality,vector<CDNuclide>& vNuclides);
	void OutputCellPower(CDTally &cTally);
	void OutputCellNucDen(CDMaterial &cMaterial,CDTally &cTally,vector<CDNuclide>& vNuclides);

	
	void PrintStepMssg();
	void PrintKeffTable();
	///////////// Public variables /////////////
    bool p_bIsWithBurnup;
	bool p_bIsBurnStepBegun;
	int p_nBurnStep;
	int	p_nTotBurnStep;

	int p_nBurnTallyStartPos;             // OneGroupCsTally[ Start : End ] is for burnup calculation
	int p_nBurnCellNum;
	vector<int> p_vBurnableCell;          // burnable cells input by users (for expansion)
	vector<double> p_vStepBurnTime;       // Burnup time of each step
	vector<double> p_vStepCmltvBurnTime;  // Cumulative Burnup time of each step
	vector<double> p_vStepBurnPowerDen;   // Burnup power density (W/gHM) of each step
	vector<double> p_vStepBurnPower;      // Burnup power (MW) of each step calculated from the user defined power density
	vector<double> p_vStepCmltvBurnup;    // Accumulative total Burnup
	vector<double> p_vStepFinalKeff;

	vector<double> p_vCellBurnPower;      // Current power of individual cell
	vector<double> p_vCellCmltvBurnup;    // Accumulative depletion of individual cell
	vector<double> p_vCellInitFuelMass;   // Initial fuel mass (Kg) of individual cell
	
	vector<double> p_vCellActualFlux;       // Current Actual flux (absolute) of individual cell
	//double p_dActualFluxRatio;              // Ratio of ActualFlux/TallyFlux

	// NAECF: neutron average energy causing fission, using for calculating fission yield
	// NAECF Input: 
	// <=-3: High Energy; <=-2: Fast neutron; <=-1: Thermal neutron; 0(default): tally fission rate weighted energy; >0: user defined average energy(eV)
	double p_dNAECFInput;
	vector<double> p_vCellBurnAveEnergy;   // Current average energy of individual cell, unit: eV

	vector<vector<double> > p_vBurnLibData;
	vector<vector<double> > p_vCellBurnNucDen;     // [][1500]

	vector<vector<CDNucId> > p_vCellInitNucID;
	vector<vector<double> > p_vCellInitNucDen;     // Specifically for natural isotopes
	vector<vector<double> > p_vCellBurnNucDenOld;
	vector<vector<double> > p_vCellBurnNucDenPre;  // Predictor Step

	int p_nInnerStepNum;

	double p_dAbsorpFracCutoff;
	double p_dNucDenFracCutoff;

	vector<int> p_vImpNucId;
	vector<int> p_vImpNucIndex;

	char p_chAceLibForBurn[10];
	char p_chDepthLibForBurn[FILE_NAME_LEN];
	int p_nExcitedBranchFlag;
	
	vector<CDBurnVaryMat> p_vBurnVaryMat;
	int p_nBurnVaryingFlag;
	vector<vector<int> > p_vBurnVaryingNuc;
	vector<vector<double> > p_vBurnVaryingNucDen;

	vector<double> p_vExcitedNrBranch;
	vector<double> p_vExcitedN2nBranch;
	vector<bool> p_vNucAceExist;                  
	
    int p_nBurnStepStrategy;                   //  burnup step strategy.     0£ºpredictor  1£ºpredictor-corrector
	int p_nBurnSolver;                         // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre

	int p_nXeEqFlag;                           // 0: not consider Xe equilibrium  1£ºconsider Xe equilibrium
	double p_dXeEqFactor;
	vector<double> p_vXeEqDen;                 // Xe equilibrium densities

	int p_nDepleteMode;
	////// cells of which the nuclide densities are output /////
	vector<vector<int> > p_vCellsForOutput;
	vector<int> p_vCellIndexForOutput;

private:
    int ConvertNucId(const char chIdStr[12]);
	void CheckAceForBurn();
	void GenImpNucIndex();
	void OutputTotNucDen();
};

//extern BurnupClass Burnup;

#endif
