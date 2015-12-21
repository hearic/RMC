# if ! defined __TALLYSUB__H__
# define  __TALLYSUB__H__

# include "CommonHead.h"
# include "MapClass.h"
# include "Material.h"
# include "Geometry.h"
# include <set>
# include "Mesh.h"

class CDTallyData
{
/* Note:
1. Tallies are scored per batch/cycle for criticality calculations, while they are 
   scored per particle history for fixed-source calculation
2. Tally data are stored in a 1D array for the convenience of parallel communication.
3. Tally average and relative error are calculated per history
*/
public:
	vector<double> p_vScore;        //// temporary score
	vector<double> p_vSum1;   
	vector<double> p_vSum2;
	vector<double> p_vAve;
	vector<double> p_vRe;

	set<int> p_setScoreIndex;         // scored indices during present histroy

	vector<int> p_vScoreIndex2;     // scored indices during present histroy
	vector<int> p_vScoreStride;     // scored stride during present histroy

	void Initiate();
	void SetZero();
	int GetSize();
	void ExpdSize(int nAddedSize);         // expand size of tally data
	void SumTallyBin();                   // sum up tally by score bin (used for tally per history)
	void SumTallyBinStride();             // sum up tally by score bin and stride (used for tally per history)
	void SumTallyAll();                   // sum up tally by overall scan (used for tally per batch/cycle)
	void CalcAveRe(double dM, double dDiv); // M; Number of observations (number of total batch/cycle/history)
};


//////// Tally types : cell tally, mesh tally, cs tally //////////
class CDCellTally
{
public:
	///////////////////////////Identifier and pointer//////////////////////////////
	int p_nTallyID;               
	int p_nTallyType;      // 1: flux, 2: fission energy deposit, 3: fission, 4: absorption, 5: fission neutron production(¦Ô¦²f*¦Õ)
	int p_nDataStartPtr;  // Start location in the cell tally data array
	int p_nDataLen;       // Data length of present cell tally  DataLen = CellBinSize + CellBinSize * ErgBinSize;
	int p_nCellBinSize;
	int p_nErgBinSize;

	///////////////////////////// list of tally cell vectors //////////////////////
	vector<vector<int> > p_vCellVecListU;   // stores list of cells
	vector<double> p_vErgBins;

	///////////////// use map to handle massive number of cells ///////////////////
	CDLongIntMap p_OCellMap;             //  use map to store tally cells

	vector<CDBinPtr> p_vCellBinPtr;      //  use bin map to accelerate binary search
	CDBinMap p_OCellBinMap;

	int p_nFlagLevel;                       //  use tally-level-flag
	vector<int> p_vTallyLevelFlags;

	////////////////// specified paras for integral tally case ////////////////////
	vector<int> p_vFilter;        // Filter is specified for high level cell 
	vector<int> p_vIntgrlBins;    // integral bins are used to combine cell vectors
	vector<int> p_vIntgrlIndex;   // integral index for each cell vector

	void SetupCellTally(CDAceData &cAceData,CDTallyData &cCellTallyData);
	int GetCellPtr(int nCell);
	int GetCellErgPtr(int nCell, int nErgGrp);

};


class CDMeshTally
{
public:
	///////////////////////////Identifier and pointer//////////////////////////////
	int p_nTallyID;               
	int p_nTallyType;      // 1: flux, 2: fission Energy deposit, 3: fission, 4: absorption, 5: fission neutron production(¦Ô¦²f*¦Õ)
	int p_nDataStartPtr;   // Start location in the mesh tally data array
	int p_nDataLen;        // Data length of present mesh tally  DataLen = TotMeshNum * ErgBinSize;
	CDMesh p_OTallyMesh;

	int p_nTotMeshNum;
	int p_nErgBinSize;
	vector<double> p_vErgBins;

	void SetupMeshTally(CDAceData &cAceData, CDTallyData &cMeshTallyData);
    int GetMeshPtr(int nMeshIndex);
	int GetMeshErgPtr(int nMeshIndex, int nErgBinIndex);
};


class CDCsTally 
{
public:
	CDCsTally()
	{
		p_nTallyID = -1;
		p_bIsForBurnup = false;
		p_nMat = -1;
		p_bIsTallyNeuAveErg = false;
		p_dTallyNeuNum = 0.0;
		p_dNeuAveErg = 0.0;
	}
	int p_nTallyID;
	int p_nDataStartPtr;  // Start location in the cs tally data array
	int p_nDataLen;       // Data length of present cs tally

	bool p_bIsTallyNeuAveErg;
	double p_dNeuAveErg;   // record average energy
	double p_dTallyNeuNum; // tallied times(include weight and tracking length), for calculating average energy, 20130926

	bool p_bIsForBurnup;
	vector<int> p_vCellVecU;        // Cell vector defined by user
	int p_nMat;                     // system index of tallied material       
	vector<vector<int> > p_vNucMTs; // tallied reaction type of each nuclide
	vector<int> p_vNucPtr;

	vector<double> p_vUnionFlux;    // flux of union energy grid 

	void SetupCsTally(CDAceData &cAceData, CDMaterial &cMaterial, CDGeometry &cGeometry,CDTallyData &cCsTallyData, bool bUseUnionCsTally);
	void SetupCsTallyBurn(CDMaterial &cMaterial, CDGeometry &cGeometry);
	int GetFluxDataPtr();
	int GetNucDataPtr(int nNuc);
	void DivideByFlux(CDTallyData &cCsTallyData);          // calculate cross-section
};


#endif
