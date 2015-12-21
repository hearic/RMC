# include "Depth_Class.h"
# include "ME_Class.h"
# include "TTA_Class.h"

void Depth_Class::SetDefault()
{ 
	NucNum = 0;     // defined when depletion library
	EleNum = 118;
	DataNum = 24;     // 18 decay entries + 6 cross-section entries
	FirstActinide = 900000 ;
	ActinidePos = 0; // Z > 89, defined when reading library
	StepNum = 0;

	//////////////Initiate Nuclide List&Name/////////////////
	GenNucList();

	//////////////Initiate Default  Parameters/////////////////
	ConvertLibFlag = false;
	ReadSubLibFlag = false;
	TranMatrixChanged = true;
	DecOnlyFlag = 0;
	AdjustFisYield = true;   // Default: Adjust FPY 
	PrintFlag = 1;
	RadioactivityFlag = 0;
	AbsRateFlag = 0;
	FissionRateFlag = 0;
	DecayHeatFlag = 0;

	MeanPowerFlag = 0;        // Default: NOT Calculate average power for constant flux mode
	MeanFluxFlag = 0;         // Default: NOT use linear Flux approximation for constant power mode  
	MeanConvergeError = 1.0E-6;

	///////////////////// file IO  //////////////////
	strcpy(OutputFile,InputFile);
	strcat(OutputFile,".burn"); 

	strcpy(ErrFile,InputFile);
	strcat(ErrFile,".depth.error"); 
	DepthRpt.open(ErrFile, ios::out);// new error file
	DepthRpt.close();

	strcpy(OrigenLibFile, "tape9.inp"); 
	strcpy(MainLibFile, "DepthMainLib");

	IsRunInParallel = false;// default: only one process run depth
	IsMasterProc = true;
	enum eMessage_Type
	{
		_NORMAL = -1, _WARNING, _ERROR
	};

	ErrorCount = 0;
	WarningCount = 0;

	SolveMethod = 2;  //Default: CRAM
	AveEnergy = 0.0253;

	ME.Laguerre_N = 18;   // 20;
	ME.Laguerre_Tao = 20; // 20;
	ME.Laguerre_a = 20;   //18 ; //generalized Laguerre polynomials

	ME.Quad_N = 32;

	TTA.CutoffFrac = 1.0E-15 ;
	return;
}
