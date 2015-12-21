# if ! defined _FisMatrixClass_h_
# define  _FisMatrixClass_h_

# include "CommonHead.h"

# include "Output.h"
# include "Mesh.h"
# include "GlobeFunFsc.h"
# include "GlobeFun.h"

using namespace std;


class CDShannonEntropy 
{
/*
	Given a mesh grid, calculate Shannon Entropy per cycle.
	Shannon Entropy can be used as indicator of fission source convergence
*/
public:
	CDMesh p_OSeMesh;             // Mesh grid. Index = (z-1) * x_num * y_num + (y-1)*x_num + x -1 ;
	vector<double> p_vCycMeshSrc;    // Mesh tally of fission source during the current cycle.
	vector<double> p_vCycSrcSE;      // Shannon entropy of fission source of each cycle

	void Initiate(int nSize)
	{
		p_vCycMeshSrc.resize(p_OSeMesh.GetTotMeshNum());
		p_vCycSrcSE.resize(nSize);
		CDGlobeFun::Set1DVecZero(p_vCycMeshSrc);
		CDGlobeFun::Set1DVecZero(p_vCycSrcSE);
		return;
	}

	int TallyFisSource(const double dPos[3], double dWgt)
	{
		int nIndex = p_OSeMesh.GetMeshIndex(dPos,true);

		if(nIndex < 0 || nIndex >= p_OSeMesh.GetTotMeshNum())
		{
			return -1;
		}

		p_vCycMeshSrc[nIndex] = p_vCycMeshSrc[nIndex] + dWgt ;

		return nIndex;
	}

	void CalcCycSe(int nCyc)
	{
		CDGlobeFun::NormalizeVec(p_vCycMeshSrc);
		p_vCycSrcSE[nCyc] = GlobeFunFsc::CalcShanEntropy(p_vCycMeshSrc);
		return;
	}

};

class CDFisMatrix 
{
	/////  F[i][j] = A[i][j]/S[j] ;   F * v = Keff * v 
	/////  F[i][j] is the number of fission neutrons born in region i 
	/////  due to one average fission neutron starting in region j
public:
	CDMesh p_OFmMesh; 

	vector<vector<double> > p_vFisMatrix;    //// Fij
	vector<double> p_vStartSrc;              //// Sj
	vector<double> p_vEigVec;                //// S

	double p_dEigVal;                        //// keff
	double p_dDR;                            //// Dominance ratio
	
	double p_dFmSrcSe;               // S0: Shannon entropy of fission-matrix start source
	double p_dFmVecSe;               // S1: Shannon entropy of fission-matrix eigen vector
	double p_dFmKld;                 // H(S1|S0): Relative entropy (K-L distance)

	int p_nBornIndex; 

	void InitFisMatrix();
	void TallyBornSrc(const double dPos[3], double dWgt);
	void TallyFisMatrix(const double dPos[3],double dFisWgt);
	void GenFisMarix();
	void PrintFisMatrix();
	void CalcFmEigen();
	void CalcFmDR();
	void CalcFmEntropy();
};


class CNpMatrix  // noise propagation matrix
{
/*
	A0 = ((m-m0)/(m-m0-1)*Lm1 - 1/(m-m0)*Sm*SmT) * inv(Lm0) 
*/
public:
	CDMesh p_ONpMesh; 

	vector<vector<double> > p_vA0;   
	vector<vector<double> > p_vLm0;  // correlation matrix  Lm0 = sum(Si*Si'), i = 1 : m
	vector<vector<double> > p_vLm1;  // correlation matrix  Lm1 = sum(Si*S(i-1)'), i = 2 : m
	vector<double> p_vS1;  // source vector of present cycle
	vector<double> p_vS0;  // source vector of previous cycle
	vector<double> p_vSm;   
	double p_dEigVal;
	 

	int p_nCycNum;

    void InitNpMatrix();
	void TallySrcVec(const double dPos[3], double dWgt);
	void AccumTallies(CDCriticality& cCriticality);
	void GenNpMatrix(CDCriticality& cCriticality);
	void CalcNpmEigen(CDCriticality& cCriticality);
};


#endif