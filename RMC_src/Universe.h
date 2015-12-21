# ifndef __UNIVERSE__H__
# define  __UNIVERSE__H__

# include "CommonHead.h"

# include "Boolean.h"

typedef std::map<long long, int> LonglongIntMap;

using namespace std;

class CDSurface 
{
public:
	CDSurface()
	{
		p_nBoundCond = 0;
	}

	string p_sKeyWord; 
	int p_nType;
	vector<double> p_vParas;
	int p_nBoundCond;
	string p_sSurfStr;

	void GenSurfType(int nSurf_index_u, string sSurf_key_word);
	void CheckSurfParas();
	int CalcSurfSense(const double dPointPos[3],const double dPointDir[3]);  //used in plot checking, LIANG 20130624
	void GetSurfNormVec(const double dPointPos[3],double dSurfNorm[3]);
	double CalcDistToSurf(const double dPointPos[3], const double dPointDirect[3],bool bIsAtSurf);
	double CalcDistToSurfSub(double a1, double b1, bool bIsAtSurf);
	void ReflectParticle(CDParticleState &cParticleState);
};


class CDSingleCell 
{
public:
	int p_nFillUnivIndexU;
	int p_nFillUnivIndex;      // filled universe index (0 means simple cell)
	vector<int> p_vBoundSurf;  // boundary surfaces (stored as system index)
	vector<int> p_vBooleanSurfaces;
	vector<CDParenthPos> p_vParenthPos;
	int p_nMatIndexU;
	int p_nMatIndex;
	int p_nImp;         // cell importance
	double p_dTemp;     // cell temperature
	double p_dVol;      // cell volume

	bool p_bIsInnerCell;   // cell is absolutely inside the universe

	////////////////// lattice material: specified for lattice burnup   /////////////////////  
    //////////  Given CellsU = C1 > C2 > C3..., where C1/C2/C3 is user-defined Index  ///////
	//////////  LatMatMap[CellsU]return material index                                ///////
	bool p_bIsExpdMat;
	LonglongIntMap LatMatMap;
	int p_nLatMatIndex;  // the first material index of LatMatMap
	int p_nLatMatNum;

	CDSingleCell()
	{
		p_nFillUnivIndexU = -1 ;
		p_nFillUnivIndex = -1 ;
		p_nMatIndexU = 0 ;
		p_nMatIndex = 0 ;
		p_nLatMatNum = 0 ;
		p_bIsExpdMat = false ;
		p_nImp = 1 ;
		p_dTemp = 2.53E-8 ;
		p_dVol = 1.0 ;         // cm^3
		p_bIsInnerCell = false ;
	}

	bool ParticleIsInCell(const double dPointPos[3], const double dPointDir[3], vector<CDSurface> &vSurface);

};


class CDUniverse 
{
public:
	////////////////////////// Filling information ////////////////////////////
	int p_nContainCellNum;
	vector<int> p_vFillCellsIndexU;    
	vector<int> p_vFillCellsIndex;
	vector<vector<vector<int> > > p_vNeighborCells; // Cell - Surface - NeighCell;

	//////////////// Coordinate transforming x' = R (x + m) ///////////////////
	double p_dOrigin[3];
	double p_dRotation[3][3];
	bool p_bIsMoved;
	bool p_bIsRotated;

	/////////////////////// Parameters for lattice  ////////////////////////////
	bool p_bIsLattice;
	int p_nLatType;                 // Type 1: 3D rectangle;  Type 2: 2D hexagon lattice
	double p_dPitch[3];             // 3D rectangle: xLen, yLen, zLen ; 2D hexagon: L1, L2
	int p_nScope[3];                // 3D rectangle: xNum, yNum, zNum ; 2D hexagon: b1Num, b2Num 
	double p_dSita;                 // 2D hexagon angle: <FA,AB> = sita
	double p_dSin_sita,p_dCos_sita;
	double p_dHeight;               // 2D hexagon height£º H = sqrt(L2^2 - L1^2*0.25)
	vector<int> p_vFillLatUnivIndexU;
	vector<int> p_vFillLatUnivIndex;
	int p_nFilledLatNum ;

	/////////////////// hexagon lattice ///////////////////////
	///             E                                       ///
	///             *               / b1 = (L1, 0)          ///
	///           *   *            /                        ///
	///      F  *       * D       /                         ///
	///         *   O   *        /------> b2=(0.5*L1, H)    ///
	///      A  *       * C                                 ///
	///           *   *                                     ///
	///             *            <FA,AB> = sita             ///
	///             B                                       ///
	///  1: FA ,  2: AB ,  3: BC ,  4: CD , 5: DE ,  6: EF  ///
	///////////////////////////////////////////////////////////


	CDUniverse()
	{
		p_nContainCellNum = 0;

		p_bIsMoved = false;
		p_bIsRotated = false;
		
		p_bIsLattice = false ;
	}

    void TransUnivCoord(double dPos[3],double dDir[3]);
	void TransUnivDir(double dDir[3]);
	int FindLatIndex(const double dPointPos[3],const double dPointDir[3]) ;
	int FindLatIndexRect(const double dPointPos[3],const double dPointDir[3]) ;
	int FindLatIndexHex(const double dPointPos[3],const double dPointDir[3]) ;
	void MoveToOriginLat(int nLatIndex, double dPointPos[3]);
	double CalcDistToLat(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf);
	double CalcDistToLatRect(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf);
	double CalcDistToLatHex(const double dPointPos[3], const double dPointDir[3], int &nWhichSurf);
	int OffsetNeighborLat(int nLatIndex,int nLatBoundSURF,double dPointPos[3]);
	int OffsetNeiLatRect(int nLatIndex,int nLatBoundSURF,double dPointPos[3]);
	int OffsetNeiLatHex(int nLatIndex,int nLatBoundSURF,double dPointPos[3]);
};

# endif