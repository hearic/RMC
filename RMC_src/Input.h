
# if ! defined __INPUT__H__
# define  __INPUT__H__

# include "CommonHead.h"
# include "ParallelClass.h"
# include "FileIO.h"
# include "ParticleState.h"
# include "Mesh.h"
# include "Output.h"
# include "Geometry.h"
# include "Material.h"
# include "Criticality.h"
# include "FixedSource.h"
# include "Tally.h"
# include "Convergence.h"
# include "Burnup.h"
# include "Plot.h"
# include "CalMode.h"

using namespace std;

class CDInput {

public:

	CDInput()
	{
		BlockDefined["DEPLETION"] = false;
		BlockDefined["UNIVERSE"] = false;
		BlockDefined["SURFACE"] = false;
		BlockDefined["MATERIAL"] = false;
		BlockDefined["CRITICALITY"] = false;
		BlockDefined["TALLY"] = false;
		BlockDefined["BURNUP"] = false;
		BlockDefined["PRINT"] = false;
		BlockDefined["CONVERGENCE"] = false;
		BlockDefined["PLOT"] = false;
		BlockDefined["GEOMETRY"] = false;
	}

	
	void CheckIOFile(int argc_temp,char *argv_temp[]);
   // void ReadInuptBlocks();                ////// read input blocks and return RmcCalcMode

	void ReadInuptBlocks(CDCalMode &cCalMode,CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,
		CDCriticality &cC_Criticality,CDTally &cTally,CDConvergence &cConvergence,CDBurnup &cBurnup,CDPlot &cPlot, CDFixedSource &cFixedSource);

	void ReadUniverseBlock(CDGeometry &cGeometry);
	void ReadCellCard(int nUniv_index,CDGeometry &cGeometry);
	void ReadCellSurfBool(int nCell_index,bool &bIsBlockEnd,bool &bIsCardEnd,CDGeometry &cGeometry);
	void ReadSurfaceBlock(CDIndex &cSurfaceIndex,vector<CDSurface> &vSurface);
	void ReadMaterialBlock(CDMaterial &cMaterial,CDAceData &cAceData);
	void ReadMatCard(bool &bIsBlockEnd,CDMaterial &cMaterial);
	void ReadSabCard(bool &bIsBlockEnd,CDMaterial &cMaterial);
	void ReadCriticalityBlock(CDCriticality &cC_Criticality);
	void ReadFixedSourceBlock(CDFixedSource &cFixedSource);
	void ReadQuasiStaticSBlock(CDCriticality &cC_Criticality);
	void ReadQuasiStaticDBlock(CDCriticality &cC_Criticality);
	void ReadConvergenceBlock(CDConvergence &cConvergence,int nINeuNumPerCyc);
	void ReadTallyBlock(CDTally &cTally,CDGeometry &cGeometry,CDMaterial &cMaterial);
	void ReadCellTallyCard(bool &bIsBlockEnd,vector<CDCellTally> &vCellTally,bool &bIsWithCellTally,CDGeometry &cGeometry);
	void ReadMeshTallyCard(bool &bIsBlockEnd,vector<CDMeshTally> &vMeshTally,bool &bIsWithMeshTally);
	void ReadErgBins(vector<double> &ErgBins,bool &IsBlockEnd,bool &IsCardEnd);

	bool ReadTallyCells(vector<vector<int> > &vTallyCells,bool &bIsBlockEnd, bool &bIsCardEnd,CDGeometry &cGeometry);
	void ReadCsTallyCard(bool &bIsBlockEnd,vector<CDCsTally> &vOneGroupCsTally,bool &bIsWithCsTally,
		CDGeometry &cGeometry,CDMaterial &cMaterial);
	void ReadBurnupBlock(CDBurnup &cBurnup,bool &bIsWithCsTally,CDGeometry &cGeometry);
	void ReadPrintBlock();
	bool ReadBurnVaryMat(vector<CDBurnVaryMat> &vBurnVaryMat, bool &bIsBlockEnd);
    void ReadPlotBlock(CDPlot &cPlot,bool &bSkipCalculation);

    void CheckInpBlock(CDCalMode &cCalMode,CDGeometry &cGeometry,CDMaterial &cMaterial,CDCriticality &cC_Criticality,
		 CDFixedSource& cFixedSource, bool &bIsMultiGroup,bool &bIsWithCsTally,bool &bIsWithBurnup);

	void SI_ReadInuptBlocks(CDCalMode &cCalMode,CDGeometry &cGeometry,CDMaterial &cMaterial,CDAceData &cAceData,
		CDCriticality &cC_Criticality,CDTally &cTally,CDConvergence &cConvergence,CDBurnup &cBurnup,CDPlot &cPlot, CDFixedSource &cFixedSource);
	void SI_ReadUniverseBlock(CDGeometry &cGeometry);
	void SI_ReadCellCard(int nUniv_index,CDGeometry &cGeometry);
	void SI_ReadCellSurfBool(int nCell_index,bool &bIsBlockEnd,bool &bIsCardEnd,CDGeometry &cGeometry);
	void SI_ReadSurfaceBlock(CDIndex &cSurfaceIndex,vector<CDSurface> &vSurface);
	void SI_ReadMaterialBlock(CDMaterial &cMaterial,CDAceData &cAceData);
	void SI_ReadMatCard(bool &bIsBlockEnd,CDMaterial &cMaterial);
	void SI_ReadSabCard(bool &bIsBlockEnd,CDMaterial &cMaterial);
	void SI_ReadCriticalityBlock(CDCriticality &cC_Criticality);
	void SI_ReadConvergenceBlock(CDConvergence &cConvergence,int nINeuNumPerCyc);
	void SI_ReadTallyBlock(CDTally &cTally,CDGeometry &cGeometry,CDMaterial &cMaterial);
	void SI_ReadCellTallyCard(bool &bIsBlockEnd,vector<CDCellTally> &vCellTally,bool &bIsWithCellTally,CDGeometry &cGeometry);
	void SI_ReadMeshTallyCard(bool &bIsBlockEnd,vector<CDMeshTally> &vMeshTally,bool &bIsWithMeshTally);
	void SI_ReadErgBins(vector<double> &ErgBins,bool &IsBlockEnd,bool &IsCardEnd);
	bool SI_ReadTallyCells(vector<vector<int> > &vTallyCells,bool &bIsBlockEnd, bool &bIsCardEnd,CDGeometry &cGeometry);
	void SI_ReadCsTallyCard(bool &bIsBlockEnd,vector<CDCsTally> &vOneGroupCsTally,bool &bIsWithCsTally,
		CDGeometry &cGeometry,CDMaterial &cMaterial);
	void SI_ReadBurnupBlock(CDBurnup &cBurnup,bool &bIsWithCsTally,CDGeometry &cGeometry);
	void SI_ReadPrintBlock();
	void SI_ReadPlotBlock(CDPlot &cPlot,bool &bSkipCalculation);


	////////////////// IO files and pointers ////////////
	char p_chInputFileName[FILE_NAME_LENGTH];
	FILE *p_fpInputFilePtr;
    map<string,bool> BlockDefined;
	int p_nInpFileFormat;

private:
	////// read all options in one input card and store them in double format

	void ReadCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, 
		               vector<vector<double> > &vParas, bool &bIsBlockEnd);  
	void ReadFixSeqCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, 
		vector<vector<double> > &vParas, bool &bIsBlockEnd); 

	void ReadMeshPara(CDMesh &cMesh, string sKeyWord, bool &bIsBlockEnd);  

	void SI_ReadCardOptions(string sInputCard, vector<string> vKeyWord, vector<int> vParaNum, 
		vector<vector<double> > &vParas, bool &bIsBlockEnd);  

	void SI_ReadMeshPara(CDMesh &cMesh, string sKeyWord, bool &bIsBlockEnd);  

	//// extrat an integer para from double format

	int ExtratIntPara(double dPara,string sCardStr);    
	long long ExtratLongPara(double dPara,string sCardStr); 


};

#endif//// __INPUT__H__
