# if ! defined __GEOMETRY__H__
# define  __GEOMETRY__H__

# include "CommonHead.h"
# include "Universe.h"
# include "RNG.h"
# include "ParticleState.h"
# include "IndexIndexU.h"
# include "Material.h"

class CDMaterial;

using namespace std;

class CDGeometry {
public:
	CDGeometry()
	{
		p_OCellIndex.p_sName = "cell";
		p_OUniverseIndex.p_sName = "universe";
		p_OSurfaceIndex.p_sName = "surface";
	}
	////////// public functions /////////////
	void ExpdGlobalCell(vector<vector<int> > &vCellsU, vector<int> &vTempCellU, int nCell, int nUniv);
	long long GetMapKeyWord(const vector<int> &vCellsU);
	int GetCellMat(const vector<int> &vCellsU); // LatIndex > 0 :  repeated geometry with non-repeated materials
	void FindNextCell(CDParticleState &cParticleState);
	void CheckGeometry(CDMaterial &cMaterial);
	double CalcDistToBound(CDParticleState &cParticleState);
	int LocateParticle(CDParticleState &cParticleState,int nStartUniv, const double dPointPos[3],const double dPointDir[3],bool bCheckOverlap = false);
	void ReflectParticle(CDParticleState &cParticleState,int nSi);

    ////////// public variables ///////////// 	
	vector<CDSingleCell> p_vCell;
	CDIndex p_OCellIndex;
	vector<vector<vector <short int> > > p_vCellSurfNeighCell;

	vector<CDSurface> p_vSurface;	
	CDIndex p_OSurfaceIndex;

	vector<CDUniverse> p_vUniverse;
	CDIndex p_OUniverseIndex;

	vector<int> p_vLatMatInputCell;
	vector<vector<int>> p_vLatMatIndexU;
	

private:
	////////// private functions ///////////// 
	void BuildNeiCellList();
	void FindNeighborCell(CDParticleState &cParticleState);
	bool CheckParticleLocation(CDParticleState &cParticleState);
	void CheckUniverse();	
	void CheckCellComplement();
	void CheckCellSurfaces();
	void CheckCellSurfBooleans();
	void CheckCellInfo(CDMaterial &cMaterial);
	int CheckOverlap(const double dPointPos[3],const double dPointDir[3]);
	void ExpandLatMatInput(CDMaterial &cMaterial);
};

//extern GeometryClass Geometry;

#endif