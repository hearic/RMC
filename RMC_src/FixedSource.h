# if ! defined __FixedSource_class__H__
# define  __FixedSource_class__H__

# include "CommonHead.h"
# include "AceData.h"
# include "Geometry.h"
# include "Material.h"
# include "Tally.h"
# include "Output.h"
# include "ParticleState.h"
# include "RNG.h"

using namespace std;

class CDFisBank;
class CDMgFisBank;
class CDConvergence;
class CDTally;

class CDFixBank {
public:
	double p_dPos[3];
	double p_dDir[3];
	double p_dErg;
	double p_dWgt;
};

class CDFixedSource
{
public:
	int p_nNeuNum;
	////////////////////// initial source //////////////////////////
	int p_nFixedSrcType;  // 0: point,  1: slab,  2: sphere  3:cyl/x  4:cyl/x  5:cyl/x
	double p_dFixedSrcErg;
	double p_dTotStartWgt;
	vector<double> p_vFixedSrcPara;
	int p_nFixedSrcBankCount;
	int p_nTotFixedSrcBankNum;  // For parallel case
	int p_nFixedSrcCount;
	vector<CDFixBank> p_vFixedSrc;
	vector<CDFixBank> p_vFixedSrcBank;
	long long p_llTotCollisionCount;
	int p_nCurrentPARTICLE;
	int p_nCollisionCount;

public:
	void CheckFixedSource();
	void InitiateAll(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDTally &cTally, 
		CDAceData& cAceData, CDParticleState &cParticleState,CDBurnup& cBurnup);
	void InitiateTrspt(CDNeutronTransport &cNeutronTransport);
	void InitiateExternalSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDAceData &cAceData,CDParticleState &cParticleState);
	void SampleFixSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDParticleState &cParticleState);
	void SampleFisSource(CDNeutronTransport &cNeutronTransport,CDGeometry &cGeometry,CDParticleState &cParticleState);
	void TrackHistory(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally);
	void TreatFission(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState);
	void GetFissionNeuState(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData,CDParticleState &cParticleState,int FisMT); // FisMt = -1 in multigroup case
};


# endif