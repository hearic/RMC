# if ! defined __TALLY__H__
# define  __TALLY__H__

# include "CommonHead.h"
# include "TallyType.h"
# include "GlobeFun.h"
# include "AceData.h"
# include "Geometry.h"
# include "Criticality.h"
# include "FixedSource.h"

using namespace std;

class CDTally {
public:
	///////////////////// public functions //////////////////////
	CDTally()
	{
		p_bIsPerHstry = false;
		p_bIsPerCyc =  true;
		p_bIsWithCellTally = false;
		p_bIsWithMeshTally = false;
		p_bIsWithCsTally = false;
		p_bUseTallyCellMap = true ;
		p_bUseTallyCellBinMap = false;
		p_bUseUnionCsTally = false; 
	}

	void ReadCsTallyInput(FILE *fptr);
	void InitiateTally(CDGeometry &cGeometry, CDAceData &cAceData, CDMaterial &cMaterial);
	void TallyByTL(double dTrackLen, CDGeometry &cGeometry, CDAceData &cAceData, CDCriticality &cCriticality, CDParticleState &cParticleState, CDMaterial &cMaterial);
	void TallyByTL(double dTrackLen, CDGeometry &cGeometry, CDAceData &cAceData, CDFixedSource &cFixedSource, CDParticleState &cParticleState, CDMaterial &cMaterial);
	void SumUpTally(int nFlag, CDCriticality &cCriticality);
	void SumUpTally(int nFlag, CDFixedSource &cFixedSource);
	void ProcessTally(CDCriticality &cCriticality, CDMaterial &cMaterial, CDAceData &cAceData);
	void ProcessTally(CDFixedSource &cFixedSource, CDMaterial &CMaterial, CDAceData &CAceData);
	void OutputTally(int nPrintFlag, CDCriticality &cCriticality, CDMaterial &cMaterial);
	void OutputTally(int print_flag, CDFixedSource &cFixedSource, CDMaterial &CMaterial);
	void GenTallyCellMap(CDGeometry &cGeometry);


	/////////////////////////  Tally type: cell tally & Mesh tally & Cs tally ///////////////////// 
	double p_dTrackLength;

	bool p_bIsPerHstry;
	bool p_bIsPerCyc;
	bool p_bIsWithCellTally;
	bool p_bIsWithMeshTally;
	bool p_bIsWithCsTally;  

	///////////// cell tally data structure //////////
	vector<CDCellTally> p_vCellTally;
	CDTallyData p_OCellTallyData;

	////////////// xs tally data structure ///////////
	vector<CDCsTally> p_vOneGroupCsTally; 
	CDTallyData p_OCsTallyData;

	////////////// mesh tally data structure ///////////
	vector<CDMeshTally> p_vMeshTally; 
	CDTallyData p_OMeshTallyData;

	///////////////////////////////// tally cell map ////////////////////////////////////
	bool p_bUseTallyCellMap;
	bool p_bUseTallyCellBinMap;
	LonglongIntMap CsTallyCellIndex;

	////////// tally reaction rate by flux spectrum(super-fine energy bin) //////////////
	bool p_bUseUnionCsTally;        
	

private:
	void TallyCellByTL(CDGeometry &cGeometry, CDParticleState &cParticleState, CDAceData &cAceData, CDMaterial &cMaterial);
	void ScoreMeshTally(CDAceData &cAceData, CDParticleState &cParticleState, CDMaterial &cMaterial);
	double CalcTallyScore(int nTallyType, CDParticleState &cParticleState, CDMaterial &cMaterial, CDAceData &cAceData); 


	double CalcMatFisQ(int nMat, CDMaterial &cMaterial, CDParticleState &cParticleState, CDAceData &cAceData);
	double CalcMatFisQ2(int mat, CDParticleState &cParticleState);


	void TallyCsByTL(CDMaterial &cMaterial, CDGeometry &cGeometry, CDAceData &cAceData, CDParticleState &cParticleState);
	void TallyNucCsByTL(int nD1,int nD2,int nNuclide,double dTrackLength, CDParticleState &cParticleState, CDAceData &cAceData);
	void ProcessCsByUEG(CDMaterial &cMaterial, CDCriticality &cCriticality, CDAceData &cAceData);
	void ProcessCsByUEG(CDMaterial &CMaterial, CDFixedSource &cFixedSource, CDAceData &CAceData);
	void CalAveCsByUEG(vector<double > &dXsData, int nNuc, int nStartPos, int nEndPos, int nOffset,int nD1,int nD2, int nD3, CDAceData &cAceData);
	void OutputCellTally(int nPrintFlag, CDCriticality &cCriticality);
	void OutputCellTally(int print_flag, CDFixedSource &cFixedSource);
	void OutputMeshTally(int nPrintFlag);
	void OutputCsTally(int nPrinFlag, CDMaterial &cMaterial);
};

//extern TallyClass Tally;

#endif