# if ! defined __OUTPUT__H__
# define  __OUTPUT__H__

# include "CommonHead.h"
# include "ParallelClass.h"
# include "ParticleState.h"

using namespace std;

class CDCalMode;
class CDCriticality;
class CDFixedSource;
class CDNeutronTransport;
class CDTally;
class CDMaterial;
class CDGeometry;
class CDAceData;
class CDConvergence;
class CDBurnup;
class CDPlot;

class CDOutput {

public:

	CDOutput()
	{
		p_nErrorCount = 0 ;
		p_nWarningCount = 0 ;

		p_bIsMatPrint = true ;
		p_bIsKeffPrint = true ;
		p_bIsSrcPrint = false;
		p_bIsSrcWrite = true;
		p_bIsCellTallyPrint = true ;
		p_bIsMeshTallyPrint = true ;
		p_bIsCsTallyPrint = true ;
		p_bIsTallyPrintPerCyc = false;
		p_bIsInputFilePrint = false; // print formatted input file
	}

	enum ePrint_Dest
	{
		_SCREEN = -1, _SCREEN_FILE, _FILE
	};

	enum eMessage_Type
	{
		_NORMAL = 0, _WARNING, _ERROR
	};

	void OpenFilePtrs(std::map<string,bool> &BlockDefined,CDCalMode &cCalMode);
	void CloseFilePtrs();
	void OutputHeading(CDCriticality &cCriticality);
	void OutputSummary(CDNeutronTransport &cNeutronTransport,CDCriticality &cCriticality,CDTally &cTally, CDMaterial &cMaterial);
	void OutputSummary(CDNeutronTransport &cNeutronTransport,CDFixedSource &cFixedSource,CDTally &cTally, CDMaterial &cMaterial);
	void OutputEnding(CDCalMode &cCalMode);
	void GenerateInpFile(CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,CDCriticality &cCriticality,
		CDTally &cTally,CDConvergence &cConvergence,CDBurnup &cBurnup, CDFixedSource &cFixedSource);

    template<typename PrintMssgType>
	void CheckInputParas(bool bCheck, PrintMssgType PrintMssg, int nMssgType)
	{
		if(! bCheck)
		{
			PrintMssg();
			PrintFile(p_fpOutputFilePtr,0,nMssgType);
		}
	}
	
	//// print message to file and/or screen ///////
    template<typename PrintMssgType>
	void OutputMessage(PrintMssgType PrintMssg, FILE* FilePtr, int nPrintDest, int nMssgType)
	{
		PrintMssg();
		PrintFile(FilePtr,nPrintDest,nMssgType);
	}

	//// print message to file and screen ///////
    template<typename PrintMssgType>
	void OutputMessage(PrintMssgType PrintMssg, FILE* FilePtr, int nMssgType) 
	{
		PrintMssg();
		PrintFile(FilePtr, 0, nMssgType);
	}

	//// print message to screen ///////
    template<typename PrintMssgType>
	void OutputMessage(PrintMssgType PrintMssg, int nMssgType) 
	{
		PrintMssg();
		PrintFile(p_fpOutputFilePtr, -1, nMssgType);
	}


	char* PrintCellVec(vector<int> vCellVec);
	char* PrintParticleState(CDParticleState &cParticleState);
	void PrintFile(FILE *fptr,int nPrintDest, int nMsgType);

	
	////////////////// IO files and pointers ////////////
	char p_chInputFileName[FILE_NAME_LENGTH];
	char p_chOutputFileName[FILE_NAME_LENGTH];

	FILE *p_fpOutputFilePtr;
	FILE *p_fpSourceFilePtr;  
	FILE *p_fpFscFilePtr;
	FILE *p_fpMatFilePtr;
	FILE *p_fpTallyFilePtr;
	FILE *p_fpCycTallyFilePtr;
	FILE *p_fpPlotFilePtr;
	FILE *p_fpInnerProductPtr;


	//////////////// time information ///////////////////
# ifdef _IS_PARALLEL_
	double StartTimeTick;
	double FinishTimeTick;
# else
	time_t StartTimeTick;
	time_t FinishTimeTick;
# endif

	char StartWallClockStr[64]; 
	char FinishWallClockStr[64]; 
	double BurnupCalcTime;
	
	int p_nErrorCount;
	int p_nWarningCount;


	/////////////////// print options///////////////////
	bool p_bIsMatPrint;
	bool p_bIsKeffPrint ;
	bool p_bIsSrcPrint;
	bool p_bIsSrcWrite;
	bool p_bIsCellTallyPrint;
	bool p_bIsMeshTallyPrint;
	bool p_bIsCsTallyPrint;
	bool p_bIsTallyPrintPerCyc;
	bool p_bIsInputFilePrint;// print formatted input file

	////////////////////////////// 
	char p_sPrintStr[1000];   // used by PrintMsg


};

extern CDOutput Output;

#endif//// __OUTPUT__H__