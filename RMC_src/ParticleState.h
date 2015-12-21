# ifndef __PARTICLESTATE__H__
# define  __PARTICLESTATE__H__

# include "CommonHead.h"

# include "Nuclide.h"

using namespace std;

class CDParticleState
{
public:
	/////////////////// basic state ///////////////////
	bool p_bIsKilled;
	double p_dWgt;
	double p_dPos[3];           // xyz position in Universe 0
	double p_dDir[3];
	double p_dExitDir[3];
	double p_dErg;              // convert to integer in case of multigroup, unit MeV    
	double p_dExitErg;          // convert to integer in case of multigroup
    double p_dLocPos[3];        // xyz position in local Universe. It changes when new Bound is found
	double p_dLocDir[3];        // uvw direction in local Universe.
	double p_dLocExitDir[3];

	/////////////// Geometry state //////////////////// 
	vector<int> p_vLocUnivs;    // Universes where the Particle is located 
	vector<int> p_vLocCells;    // The i-th cell in universe where the Particle is located. ==>  Universe[LocUnivs].ContainCells[LocCells]
	vector<int> p_vLocCellsU;   // User defined LocCells.  LocCellsU = Universe[LocUnivs].ContainCells[LocCells]
	int p_nCELL ;               // CELL in maximum-level universe where the Particle is located
	int p_nBoundLEVEL;          // Boundary is in the i-th level universe
	int p_nBoundSURF;           // Boundary surface index
	int p_nBoundINDEX;          // Boundary is the i-th surface of the CELL
	int p_nLatBoundSURF ;       // 1:-x ;  2:+x ;  3:-y;  4:+y

	///////////////// collision state ////////////////////
	int p_nMAT;                 // Current cell material
	int p_nNUC;                 // Current collision nuclide
	int p_nSabNUC;              // Sab nuclide corresponding to current collision nuclide
	int p_nColType;             // Current reaction MT number
	double p_dCellTmp;          // Current cell temperature

	///////////////// cross sections  //////////////////// 
	int p_nInterpN0;            // Eg0 interpolation position
	double p_dInterpK0;         // Eg0 interpolation fraction
	int p_nInterpN;             // Erg interpolation position
	double p_dInterpK;          // Erg interpolation fraction
	CDNucCs p_OColNucCs;        // cross-sections of collision nuclide
	double p_dMacroTotCs;
	double p_dMacroMuFissCs;  


	///////////////////////////////////////////////////////
	bool p_bIsCellChanged;
	bool p_bIsMatChanged;
	bool p_bIsCellTmpChanged;


	/////////////// Free-gas/sab state ////////////////////
	bool p_bIsSabCol;           // sab collision 
	bool p_bIsFreeGasCol;       // free gas collision
	double p_dErg_Rel;          // relative energ in free gas model
	double p_dDir_Rel[3];       // relative velocity in free gas model
	double p_dTgtVel[3];        // Target velocity in free gas model

	////////////////////////// functions ///////////////////////
	void UpdateNeuState();
	void FlyByLength(double dLength);
	//void Normalize3Array(double VectorTemp[3]);
	void ClearLevel();

	int ErgGrp()   // converte Erg to ErgGrp in case of multigroup
	{
		return int(p_dErg + 0.5);
	}
};

//extern ParticleStateClass ParticleState;

# endif