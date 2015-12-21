# if ! defined __DEPTH__H__
# define  __DEPTH__H__
# pragma warning(disable:4996) 
# pragma warning(disable:4786) 
# pragma warning(disable:4018)
# pragma warning(disable:4244) 

# include <fstream>
# include <iostream>
# include <math.h>
# include <complex>
# include <vector>
# include <iomanip>
# include <stdio.h>
# include <string.h>
# include "SparseMatrix.h"

const unsigned int FILE_NAME_LEN = 220;

using namespace std;

class YieldClass 
{
public:
	int FisNucID;                         // fissionable actinide id
	vector<double> Energy;                // incident energies: 0.0253ev, 2Mev, 14Mev, unit: 1 ev
	vector<int> ProductId;                // fission product id
	vector<vector<double>> ProductYld;    // fission product yields at incident neutron energies
	vector<double> PrdctYld;              // fission product yield at a given neutron energy by linear interpolation
};

class Depth_Class
{
public:
    ////////////////////////////////// Functions/////////////////////////////
	////////// Basic Functions////////
    void RunDepth(int argc_temp,char *argv_temp[]);
	void CheckFileExist(int argc, char argv[3][200]);
	void SetDefault();
	void Initiate();
	void GenNucList();
	int GetNucId(int NuIndex);
	int GetNucIdIndex(int NucId);
	string GetNucName(int NucId);
	double GetLibAbsXs(int NucIndex);
	double GetLibFisXs(int NucIndex);
	double GetLibDecQ(int NucIndex);
	double GetLibLamd(int NucIndex);
	void ReadInput();
	void ScaleSqure();
	void Execute();
	void SolveBurnEqs();
	void ReadOrigenDec();
	void ReadOrigenXs();
	void GenTransitXs(int ReactId, int ProdId, int ReactType, double ReactRate);
	void ReadOrigenYld();
	void ConvertLib();
	void ReadLib();
	void ReadSubLib();
	void AdjustYield();                   // Calculate the fission product yield factor, which is used to adjust the fission product yield at each step                 
	void FindNeighbor(int FissionMaxNuc); // Find the nearest neighbor to the largest fission contributor without explicit fission product yield                  
	void TransitDec();
	void InterpolateYield();              //  Linear interpolation of yield data at a given incident neutron energy
	void TransitNeu();
	void GenTranMatrix(int MatrixType);   // MatrixType:  1-Row Compressed Storage. 2-Column Compressed Storage. 3-Mixedgdhjs Compressed Storage
	void CalStartFluxPower();  // Calculate flux at the start of the current step
	void CalMeanFluxPower();   // Calculate average flux and power of the current step
	double CalcPowerFactor(vector<double> &NucDen);
	void CalQuantities(int step);
    void WriteOutput();
	void OutputTable(vector<vector<double> > &Table);
	void ErrorWarning(char Message[200],int Type, bool OnlyMasterPrint = false);
	void Finish();

	////////////////////////////////// Variables/////////////////////////////
	//////////////////////// nuclides in burnup chain //////////////////////////
	int NucNum;
	int EleNum;
	int DataNum;
	int FirstActinide;
	int ActinidePos;
	vector<int> NucIDList;
	vector<int> NucIDIndex;
	vector<string> EleNameList;

	////////////////////////// File input & output //////////////////////////
	char InputFile[FILE_NAME_LEN];
	char OutputFile[FILE_NAME_LEN];
	char ErrFile[FILE_NAME_LEN];
	ofstream DepthOut,DepthRpt;
	char PrintMssg[800];
	int PrintFlag;
	int ErrorCount,WarningCount;
	bool IsRunInParallel, IsMasterProc; // only master print message when parallel

	//////////////////////////// Library data /////////////////////////////////
	int OrigenLibType;
	char OrigenLibFile[20];
	char OrigenDecFile[20];
	char OrigenXsFile[20];
	char OrigenYldFile[20];
	char MainLibFile[FILE_NAME_LEN];
	char SubLibFile[20];
	vector<vector<double> > LibData;
	vector<YieldClass> YieldData;
	int NearestNeighbor;
	double YieldFactor;
	SparseMatrixIS DecMatrix;
	SparseMatrixIS NeuMatrix;

	bool ReadSubLibFlag;
	bool ConvertLibFlag;
	bool TranMatrixChanged;  // 0: matrix NOT change;  1: matrix change;
	bool AdjustFisYield;     // 0: NOT Adjust fission product yield;  1: Adjust fission product yield
	
	int SolveMethod;         // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre
	int StepNum, STEP, MODE;
	double FLUX, TIME, POWER;
	double AveEnergy;       // Energy related yield data, unit: ev, same with library
	double MeanPOWER;
	double StartFlux;
	double MeanFlux;
	int MeanPowerFlag;
	int MeanFluxFlag;
	double MeanConvergeError;
	vector<double> StepFlux;
	vector<double> StepTime;
	vector<double> StepPower;
	vector<double> CmltvTime;
	vector<int> StepMode;
	int DecOnlyFlag;
	
	vector<double> N0;
	vector<vector<double> > StepNt;

	/////////////////  Quantities related to nuclide density /////////////////////
	int RadioactivityFlag;
	int AbsRateFlag;
	int FissionRateFlag;
	int DecayHeatFlag;
	vector<vector<double> > Radioactivity;
	vector<vector<double> > AbsorpRate;
	vector<vector<double> > FissionRate;
	vector<vector<double> > DecayHeat;

	time_t SolveTick;       /// Time analysis
	time_t StartTick;
	time_t FinishTick;
};

extern Depth_Class Depth;

#endif
