# if ! defined __MATERIAL__H__
# define  __MATERIAL__H__

# include "CommonHead.h"
# include "AceData.h"
# include "Output.h"
# include "IndexIndexU.h"
# include "Universe.h"

using namespace std;

class CDBurnup;
class CDSingleMat;
class CDGeometry;

class CDSingleMat {
public:
	double p_dMatUserDen;
	double p_dMatAtomDen;
	double p_dMatGramDen;
	int p_dMatTotNucNum;
	int p_dMatTotSabNucNum;
	vector<CDNucId> p_vMatNucID;// [!] will be cleared (ConvertMatNucDen.cpp, UpdateBurnedMatNucDen.cpp)
	vector<CDNucId> p_vMatSabNucID;
	vector<int> p_vMatNucIndex;
	vector<int> p_vMatSabNucIndex;
	vector<int> p_vSabnucOfNuc;  // MatNucIndex ---->
	vector<double> p_vMatSabNucEsa;  // cut-in energy of sab
	vector<double> p_vMatNucUserDen; // >0: atom density ratio, <0: mass density ratio // [!] will be cleared (ConvertMatNucDen.cpp, UpdateBurnedMatNucDen.cpp)
	vector<double> p_vMatNucAtomDen; // atom density (10^24 atom/cm^3), // mass density (g/cm^3) not saved, get from AtomDen

	int GetMatTotNucNum() {return p_dMatTotNucNum;}
	int GetMatTotSabNucNum(){return p_dMatTotSabNucNum;}
	CDNucId GetMatNucID(int i){return p_vMatNucID[i];}
	CDNucId GetMatSabNucID(int i){return p_vMatSabNucID[i];}
	int GetMatNucIndex(int i){return p_vMatNucIndex[i];}
	int GetMatSabNucIndex(int i){return p_vMatSabNucIndex[i];}
	int GetMatSabnucOfNuc(int i){return p_vSabnucOfNuc[i];}
	double GetMatNucSabesa(int i){return p_vMatSabNucEsa[i];}
	double GetMatNucUserDen(int i){return p_vMatNucUserDen[i];}
	double GetMatNucAtomDen(int i){return p_vMatNucAtomDen[i];}

	CDSingleMat()
	{
		p_dMatUserDen = 0 ;
		p_dMatAtomDen = 0 ;
		p_dMatGramDen = 0 ;
		p_dMatTotNucNum = 0 ;
		p_dMatTotSabNucNum = 0 ;
	}
};

class CDMaterial {
public:
	CDMaterial()
	{
		p_OMatSetIndex.p_sName = "material";
		p_dAneut  = 1.008664967;                   // Neutron mass in a.m.u..
		p_dAvogad = 6.022043446928244e+23;         // Avogadro's number.
		p_dAvgdn  = 1.0E-24*p_dAvogad/p_dAneut ;   // Converter
	}
	/////////public functions ///////////
	void InitiateMatAce(CDGeometry & CGeometry,CDTally &CTally, CDAceData& cAceData,vector<CDNuclide>& vNuclides, vector<CDSingleCell>& vCells, CDBurnup& cBurnup);
	void CheckUsedMat(CDGeometry & cGeometry);
	int GetTotMatNum();

	int GetTotNucNumIncSab();
	int GetTotNucNumExcSab();
	int GetTotUsedNucNumExcSab();

	int GetMatTotNucNum(int m);
	int GetMatTotSabNucNum(int m);
	CDNucId GetMatNucID(int m, int i);
	CDNucId GetMatSabNucID(int m, int i);
	int GetMatNucIndex(int m, int i);
	int GetMatSabNucIndex(int m, int i);
	int GetMatSabnucOfNuc(int m, int i);
	double GetMatNucSabesa(int m, int i);
	double GetMatGramDen(int m);
	double GetMatAtomDen(int m);
	double GetMatUserDen(int m);
	double GetMatNucUserDen(int m, int i);
	double GetMatNucAtomDen(int m, int i);
	double GetMatNucGramDen(int m, int i,vector<CDNuclide>& vNuclides);// g/cm^3
	double GetMatFuelDen(int m, vector<CDNuclide>& vNuclides);  // specified for burnup calculation
	void OutputMaterialFile(vector<CDNuclide>& vNuclides);
	void ClearMatNucUserInput();// clear NucID/UserDen in every single mat for saving memory

	/////////public variables ///////////
	vector<CDSingleMat> p_vMatSet;  // 	TotMatNum = MatSet.size() - 1;
	CDIndex p_OMatSetIndex;
	vector<CDNucId> p_vNucIDList;

	double p_dAneut;   // 1.008664967, Neutron mass in a.m.u..
	double p_dAvogad;  // 6.022043446928244e+23, Avogadro's number.	
	double p_dAvgdn;   // converter, 1.0E-24*p_dAneut/p_dAvogad. 
	                   // GramDensity = AtomDensity*dAtomWeight/p_dAvgdn

private:
	/////////private functions ///////////
	void GenTotNucIdList(bool bIsMultiGroup);
	void ConvertMatNucIndex(vector<CDNuclide>& vNuclides);
	void DopplerBroaden(CDAceData& cAceData, vector<CDSingleCell>& vCells);
	void CheckSabMaterial(CDAceData& cAceData);	
	void ConvertMatNucDen(vector<CDNuclide>& vNuclides);

	/////////private variables ///////////
	int m_nTotNucNumIncSab;
	int m_nTotNucNumExcSab;
	int m_nTotUsedNucNumExcSab;
};

//extern MaterialClass Material;

#endif
