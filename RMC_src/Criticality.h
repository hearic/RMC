# if ! defined __Criticality_class__H__
# define  __Criticality_class__H__

# include "CommonHead.h"
# include "AceData.h"
# include "Geometry.h"
# include "Material.h"
# include "Tally.h"
# include "Output.h"
# include "ParticleState.h"
# include "RNG.h"
# include "NeutronTransport.h"


using namespace std;

class CDFisBank;
class CDMgFisBank;
class CDConvergence;
class CDTally;


class CDInnerProduct
{
public:
	double p_dScore;
	double p_dCycVal;
	double p_dCycSum1;
	double p_dCycSum2;
	double p_dCycAve;
	double p_dCycStd;

public:
	CDInnerProduct()
	{
		p_dScore=0;
		p_dCycVal=0;
		p_dCycSum1=0;
		p_dCycSum2=0;
		p_dCycAve=0;
		p_dCycStd=0;
	}
};

class CDCriticality {
public:
	///////////////////// public functions //////////////////////
	CDCriticality()
	{
		p_dKeffFinal = 1.0;
		p_nNeuNumPerCyc = 0;
		p_bUseBatch = false;

		p_bIsQuasiStaticS = false;
		p_bIsQuasiStaticD = false;
	}

	void CheckCriticality();
	void InitiateAll(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDConvergence &cConvergence,CDTally &cTally, 
	                 CDAceData& cAceData, CDParticleState &cParticleState,CDBurnup& cBurnup);
	//void TrackOneNeu(CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,CDTally &cTally);
	void TrackHistory(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,CDTally &cTally);
	
	
    
	
	
	void EstimateKeffTrk(double dWgt, double dMacroFisXs, double track_len);              // Track length estimator
	void EstimateKeffCol(double dWgt, double dMacroFisXs, double dMacroTotXs);            // collision estimator
	void EstimateKeffAbs(double dWgt,double dNu,double dMicroFisXs,double dMicroTotXs);   // absorption estimator
	void ProcessCycleEnd(CDNeutronTransport &cNeutronTransport, CDMaterial &cMaterial,CDConvergence &cConvergence,CDTally &cTally, CDAceData& cAceData);
    void ProcessKeff(CDConvergence &cConvergence);
	void ProcessBank(CDNeutronTransport &cNeutronTransport, CDConvergence &cConvergence);
	void CombineKeff();
	void OutputKeff();	
	void SampleFisSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDParticleState &cParticleState,CDConvergence &cConvergence);
	
	
	void ResetCriticality();


	///////////////////// public variables //////////////////////
# ifdef _IS_PARALLEL_
	double TrsptStartTimeTick;
# else
	time_t TrsptStartTimeTick;
# endif

	
	//////////////////// transport tracking  //////////////////
	int p_nNeuNumPerCyc;
	int p_nTotCycNum;
	int p_nInactCycNum;
	int p_nActCycNum;

	bool p_bUseBatch;
	int p_nBatchNum;
	int p_nBatchLen;
	double p_dBatchKeff;

	////////////////////// initial source //////////////////////////
	int p_nKsrcType;  // 0: point,  1: slab,  2: sphere  3:cyl/x  4:cyl/x  5:cyl/x
	vector<double> p_vKsrcPara;

	int p_nCycleNeuNum, p_nCurrActiveCYCLE;
	int p_nCurrentCYCLE, p_nCurrentPARTICLE;
	double  p_dKeffFinal;
	double  p_dKeffWgtSum[3],p_dKeffCyc[3];             // 0: Collision estimator,  1: Absorption estimator,  2:Track Length estimator
	double  p_dKeffSum[3], p_dKeffCrosSum[3][3];        // KeffSum = cumulative sums, KeffCrosSum = cross product sums.
	double  p_dKeffIndAve[3], p_dKeffIndStd[3];         // Averages and std. devs. of individual estimators
	double  p_dKeffCovwAve[4], p_dKeffCovwStd[4];       // covariance-weighted combined averages and std. devs
	double  p_dTotStartWgt;	

	
	long long p_llTotCollisionCount;  
	int p_nCollisionCount;
	
	int p_nFisBankCount;
	int p_nTotFisBankNum;  // For parallel case
	int p_nFisSrcCount;
	vector<CDFisBank> p_vFissionSrc;
	vector<CDFisBank> p_vFissionBank;

public:
	
	void TreatFission(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence);    
	void BankFisSource(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,double dFisSubCs[5]);//void BankFisSource(double FissCs[5]);
	void BankFisSourceMg(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence);//void BankFisSourceMg();
	void GetFissionNeuState(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence,int nFisMT,double dFisWgt); // FisMt = -1 in multigroup case
	void InitiateFisSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDAceData &cAceData,CDParticleState &cParticleState);
	void InitiateTrspt(CDNeutronTransport &cNeutronTransport);



	////////////////////////////////////////////////////////////////////////////
	////////////////////////////// PCQS ////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// inner products //
	CDInnerProduct p_cNeuPopu;                     //<1,phi/v>/totStartWgt
	CDInnerProduct p_cFisNeuStrength[7];           //[0] denotes prompt neutron fraction
	CDInnerProduct p_cAveDelayedConstant[7];
	CDInnerProduct p_cBeta[7];
	CDInnerProduct p_cLeakNeuStrength;
	CDInnerProduct p_cAbsNeuStrength;
	double         p_dPower;

	CDInnerProduct p_cRho;
	CDInnerProduct p_cFis;    //For reactivity
	CDInnerProduct p_cAbs;	  //For reactivity
	CDInnerProduct p_cLeak;	  //For reactivity

	CDInnerProduct p_cGenTime;

	//  time absorbed neutron source and bank //
	bool   p_bIsRecordTimeAbsNeu;
	double p_dTimeAbsSrcStrength;
	int p_nTimeAbsNeuBankCount;
	int p_nTimeAbsNeuSrcCount;
	vector<CDFisBank> p_vTimeAbsNeuBank;
	vector<CDFisBank> p_vTimeAbsNeuSrc;

	// delayed neutron source and bank //
	bool   p_bIsRecordDelayedNeu;
	double p_dDelayedSrcStrength[7];
	int p_nDelayedNeuBankCount;
	int p_nDelayedNeuSrcCount; 
	vector<CDFisBank> p_vDelayedNeuBank;
	vector<CDFisBank> p_vDelayedNeuSrc;

	// fixed source and bank //
	int p_nFixedSrcSize;
	vector<CDFisBank> p_vFixedSrc;

	double p_dDeltaTime;
	double p_dDeltaTimeNext;
	double p_dTimeAbsMacroCs;
	bool   p_bIsQuasiStaticD;
	bool   p_bIsQuasiStaticS;


	// point kinetics //
	double p_dFluxAmptitude;
	double p_vPrecursorConcentration[7];
	vector<double> p_vPKLambda;
	vector<double> p_vPKBeta;
	double p_dRho;
	double p_dGtime;


	// combined delayed neutron source //
	int p_nCombDelayedNeuSrcCount;
	vector<CDFisBank> p_vCombDelayedNeuSrc;
	double p_dCombDelayedNeuSrcStrength;


	// previous read from *.source //
	double p_vDelayedSrcStrengthPrev[7];
	double p_vAveDelayedConstantPrev[7];
	double p_dFluxAmptitudePrev;
	double p_vPrecursorConcentrationPrev[7];
	int    p_nDelayedNeuSrcCountPrev;
	vector<CDFisBank> p_vDelayedNeuSrcPrev;
	int    p_nWriteSrcCount;


	void GetExistedNeuSource();
	void EstimateNeuPopuTrk(CDAceData &cAceData, double dWgt, double dErg, double track_len);
	void EstimatePower(CDAceData &cAceData,  CDParticleState &cParticleState, CDMaterial &cMaterial,double dWgt, double dErg, double track_len);
	void BankTimeAbsNeuSource(CDAceData &cAceData, CDParticleState &cParticleState);
	void ProcessNeuPopu();
	void ProcessFisNeuStrength();
	void ProcessBank_QuasiStatic(CDNeutronTransport &cNeutronTransport);
	void ProcessTimeAbsNeuBank();
	void ProcessDelayedNeuBank();
	void BankDelayedNeuSource(CDParticleState &cParticleState);

	void PointKinetics();
	void ProcessDelayedNeuStrength();
	void ProcessTimeAbsNeuStrength();
	void ProcessPower();
	void ProcessFixedSrc();
	void WriteSrc();
	void OutputInnerProduct();
};

class CDFisBank {
public:
	double p_dPos[3];
	double p_dDir[3];
	double p_dErg;
};

class CDMgFisBank {
public:
	double p_dPos[3];
	double p_dDir[3];
	int p_dErgGrp;
};

//extern CD_Criticality C_Criticality;

#endif///__Criticality_class__H__
