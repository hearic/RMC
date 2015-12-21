# if ! defined __CALMODE__H__
# define  __CALMODE__H__

# include "Criticality.h"
# include "Tally.h"
# include "AceData.h"
# include "Geometry.h"
# include "Material.h"
# include "Burnup.h"
# include "Output.h"
# include "Plot.h"
# include "Convergence.h"
# include "Depth_Class.h"
# include "FixedSource.h"

using namespace std;

class CDCalMode {

public:

	enum p_eMode{CriticalMode = 1, BurnupMode = 2,  PointBurnMode = 3, FixedSourceMode = 4, QuasiStaticSMode = 5, QuasiStaticDMode = 6};

	int p_nRmcCalcMode;
	bool p_bSkipCalculation;

	void RunCalculation(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData, CDGeometry &cGeometry, Depth_Class &cDepth, CDCriticality &cCriticality, CDMaterial &cMaterial, 
	CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup, CDFixedSource& cFixedSource);

private:
	void CalcCriticality(CDNeutronTransport &cNeutronTransport, CDAceData& cAceData, CDCriticality &cCriticality, CDMaterial &cMaterial, CDGeometry &cGeometry, 
	CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup);
	void CalcFixedSource(CDNeutronTransport &cNeutronTransport, CDAceData& CAceData, CDFixedSource &cFixedSource, CDMaterial &CMaterial, CDGeometry &CGeometry, 
		CDParticleState &CParticleState,CDTally &CTally);
	void CalcBurnup(CDNeutronTransport &cNeutronTransport, CDAceData& cAceData, CDCriticality &cCriticality, CDMaterial &cMaterial, CDGeometry &cGeometry, 
	CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup);
	void CalcPointBurnup(Depth_Class &cDepth);
	void CalcKinetcs();
	void CalcMultiGroup();


public:
	CDCalMode(){
		p_nRmcCalcMode = -1;
		p_bSkipCalculation = false;
	};
};

//extern CalModeClass CalMode;

#endif
