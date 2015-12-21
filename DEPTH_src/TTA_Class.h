# if ! defined __TTA__H__
# define  __TTA__H__

# include <vector>
# include "SparseMatrix.h"

using namespace std;

class TTA_Class
{

public:
	void SolveByTTA();
	void ResetTTA();
	void CalcOneChain();
	void PushOneChain();
	void PopOneChain();
	double CalcChainBranch(int Index);
	double CalcChainNucDen(int Index);
	double CalcChainCutOff(int Index);
	double GetEffectDecay(int m,int n);

	SparseMatrixCCS TranMatrixCCS;

	vector<vector<int> > StackChains; 
	vector<int> ChainNuclide;
	vector<double> ChainLamda;          // ChainLamda[j] = ¦Ë_eff(j) = ¦Ë(j)+ ¦Õ*¦²¦Ò(i,j)
	vector<double> ChainEffBranch;      // ChainEffBranch[j+1] = b_eff(j,j+1).  b_eff(i,j)=(b(i,j)*¦Ë(i)+ ¦Õ*¦Ò(i,j))/¦Ë_eff(i)

	double Time;
	double Step;
	vector<int> HeadNucList;
	int HeadNucNum;
	int HeadNuc;         // The first nuclide of the full chain.
	double HeadNucDen;   // Density of The first nuclide of the full chain.
	int MotherNuc;       // The current/mother nuclide of the sub chain.
	int MotherNucIndex;  // The index of the mother nuclide in the sub chain.
	int DaughterNucNum;  // daughter nuclide number of a specific nuclide
	int StackNum;
	int RepeatStackNum;          // Stack number
	int ChainNum;                // total chain number
	double CutoffFrac;
	double TTACutoff;

};

extern TTA_Class TTA;

#endif