# if ! defined __Transport_class__H__
# define  __Transport_class__H__

# include "CommonHead.h"
# include "AceData.h"
# include "Geometry.h"
# include "Material.h"
# include "ParticleState.h"
# include "RNG.h"
# include "Convergence.h"

using namespace std;

class CDFisBank;
class CDMgFisBank;
class CDNeutronTransport {
public:
	///////////////////// public functions //////////////////////
	CDNeutronTransport()
	{
		p_dWgtCutOff = 0.25;
		p_dEg0CutOff = 1.0E-20;
	}
	
	int p_nCurrentPARTICLE;
	int p_nMissParticleCount;

	double  p_dStartWgt,p_dWgtCutOff,p_dEg0CutOff;	
	

public:
	///////////////////// private functions //////////////////////
	void GeometryTracking(CDCriticality &cCriticality, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally);
	void GeometryTracking(CDFixedSource &cFixedSource, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,CDTally &cTally);
	double SampleFreeFlyDist(CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,bool IsErgChanged);	
	double SampleFreeFlyDist(CDCriticality &cCriticality, CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState,bool IsErgChanged);	
	void SampleColliNuc(CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState);
	void SampleColliNuc(CDCriticality &cCriticality, CDMaterial &cMaterial,CDAceData &cAceData,CDParticleState &cParticleState);
	void CalcColliNucCs(CDAceData &cAceData,CDParticleState &cParticleState);
	void TreatFreeGasModel(CDParticleState &cParticleState,double dCellTmp, double dNucWgt, double dErg0, double &dErgRel);
	void RotateDirection(double dMu,const double dDirOld[3],double dDirNew[3]);
	void GetCeExitState(CDAceData &cAceData,CDParticleState &cParticleState,int nColliNuc,int dReactType,double dIncidErg,const double *dIncidDir,bool bTreatedFreeGas, double &dExitErg,double *dExitDir);
	void RotationFreeGas(CDParticleState &cParticleState,double dDirRel[3],double dNucAtomWgt,double dTmp,
		        double Mu_Lab,double* ExitDir, int ColliNuc, double ExitErg_Lab, double& ExitErg);
	void GetExitState(CDCriticality &cCriticality, CDAceData &cAceData,CDParticleState &cParticleState);
	void GetExitState(CDFixedSource &cFixedSource, CDAceData &cAceData,CDParticleState &cParticleState);
	int  SampleColliType_Cri(CDAceData &cAceData,CDParticleState &cParticleState);
	int  SampleColliType_Fix(CDAceData &cAceData,CDParticleState &cParticleState);
	void OutputSrcInfo(CDAceData &cAceData,CDParticleState &cParticleState, int nCycle, int nCurrParticle, int nCollisionCount);   
	void TreatImpliCapt(CDCriticality &cCriticality, CDAceData &cAceData,CDParticleState &cParticleState,CDConvergence &cConvergence);
	void TreatImpliCapt(CDAceData &cAceData,CDParticleState &cParticleState);
};


//extern CDNeutronTransport ONeutronTransport;

#endif
