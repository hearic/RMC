# if ! defined __CONVERGENCE__H__
# define  __CONVERGENCE__H__

# include "CommonHead.h"
# include "Criticality.h"
# include "Mesh.h"
# include "SeFmNpm.h"

using namespace std;
class CDFisBank;

class CDConvergence {

public:
	////////// public functions /////////////
	CDConvergence();
	void InitiateFsc(CDCriticality& cCriticality);
	void DiagAcceFsc(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality);
	void FscTallySrc(const double dStartPos[3], double dWgt, CDCriticality& cCriticality);
	void FscTallyFm(const double dPos[3],double dFisWgt,CDCriticality& cCriticality);
	void DiagFsc(CDCriticality& cCriticality); 
	void AcceFsc(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality);
	void ProcessSe(CDCriticality& cCriticality);
	void ProcessFm(CDCriticality& cCriticality);
	void ProcessNpm(CDCriticality& cCriticality);
	void WldtTrackHistory(CDNeutronTransport &cNeutronTransport, CDCriticality& cCriticality,CDGeometry& cGeometry,CDMaterial& cMaterial, CDAceData& cAceData, CDParticleState& cParticlestate, CDConvergence& cConvergence,CDTally &cTally);
	void WldtBankFissions(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDCriticality& cCriticality,CDParticleState &cParticleState,int nFisMT,double dWielandt_R);
	void WldtSampleSource(CDNeutronTransport &cNeutronTransport,CDCriticality& cCriticality, CDParticleState& cParticlestate, CDGeometry& cGeometry);
	void ResetFsc(CDCriticality& cCriticality);


	////////// bool values for diagnostic & acceleration of fission source convergence ///////////
	bool p_bUseFscDiag;           // Convergence diagnostic is applied
	bool p_bUseFscAcce;           // Convergence acceleration is applied
	bool p_bUseDiagFm;            // use fission matrix for diagnostic
	bool p_bUseAcceFm;            // use fission matrix for acceleration
	bool p_bUseDiagNpm;           // use noise propagation matrix for diagnostic
	bool p_bUseNpmOnFly;          // calculate DR withNPM on the fly

	bool p_bIsFsdPrint;
	bool p_bIsFmPrint;
	
	//////////////////////// Diagnostic of fission source convergence ////////////////////////////
	int p_nFscDiagMethod;
	///////////// Shannon Entropy Mesh for fission source convergence 
	CDShannonEntropy p_OShanEntropy;

	///////////// Posterior diagnostic with fission matrix
	vector<CDFisMatrix> p_vDiagFisMatrix;

	///////////// noise propagation matrix
	vector<CNpMatrix> p_vNpMatrix;

	///////////////////// Acceleration of fission source convergence ///////////////////////
	int p_nFscAcceMethod;     //// 1:wielandt, 2:superhistory, 3:Fission matrix
	//Wielandt acceleration
	double p_dWldtKw,p_dWldtKe;                  // 1/Keff=1/Kw+1/Ke
 	vector<CDFisBank> p_vWldtSourceBank;   // Fission bank of Wielandt iteration
	int p_nWldtBankCount;
	int p_nWldtSampleCount;
	double p_dWldtFactor;                       
	vector<double> p_vWldtFactorList;
	vector<int> p_vWldtFactorCycleBin;

	//super-history acceleration
	int p_nSuphisCycleCount;
	int p_nSuphisFactor;
	vector<int>  p_vSuphisSourceCycleBank;
	vector<int> p_vSuphisFactorList;
	vector<int> p_vSuphisFactorCycleBin;
	double p_dSuphisTotStartWeight;

	// Fission matrix acceleration
	CDFisMatrix p_OAcceFisMatrix;
	int p_nAcceFmCyc;
};

//extern ConvergenceClass Convergence;

#endif