# if ! defined __ParallelClass__H__
# define  __ParallelClass__H__

# include "CommonHead.h"

# ifdef _IS_PARALLEL_

using namespace std;
class CDCriticality ;
class CDFixedSource;
class CDBurnup;
class CDMaterial;
class CDTally;

class CDParallel{

public:
	void InitiateParallel();
	void ParallelCycle();
	void BalanceFisBank(int &nSourceCount, int nTotBankNum, int& nCycleNeuNum);
	void ParallelBank(CDCriticality& cCriticality);
	void ParallelBank(CDFixedSource& cFixedSource);
	void ParallelBank_ExternalSrc(CDCriticality& cCriticality);
	void ParallelBank_QuasiStatic(CDCriticality& cCriticality);
	void ParallelTimeAbsNeuBank(CDCriticality& cCriticality);
	void ParallelDelayedNeuBank(CDCriticality& cCriticality);
	void ParallelBankSearch(int &nSendIndex, int &nRecvIndex);
	void GatherCycTally(CDTally & CTally);
	void GatherHstryTally(CDTally & CTally);
	void BalanceBurnCell(int &nBurnCellCount, int nTotBurnCellNum);
	void ParallelBurn(CDBurnup& cBurnup,CDMaterial& cMaterial, CDTally& cTally);

	int p_nMyId;
	int p_nProcsNum;
	int p_nNameLen;
	bool p_bIsMaster;                    

	//////////////////////////// parallel transport calculation ///////////////////////
	bool p_bUseFastBank;             // flag of using advanced parallel fission bank algorithm
	vector<int> p_vBankLoad;             // Length of Fission bank Gathered each processor
	vector<int> p_vBankLoadSum;          // BankLoadSum[i] = sum(BankLoad[0]: BankLoad[i-1]) . Used for offset of Gatherv
	vector<int> p_vSourceLoad;           // Fission source distributed to each processor for balance
	vector<int> p_vSourceLoadSum;        // SourceLoadSum[i] = sum(SourceLoad[0]: SourceLoad[i-1]) . Used for offset of Scatterv
	vector<int> p_vRandNumPos;
	int p_nRandNumSum;                   // sum of CycleNeuNum of cycles
    MPI_Datatype MPI_BankDataType;
	MPI_Datatype MPI_BankDataType_fix;

	 

	//////////////////////////// parallel burnup calculation ///////////////////////
	bool p_bIsParallelBurn;
	vector<int> p_vBurnCellLoad;
	vector<int> p_vBurnCellLoadSum;
	int p_nStartBurnCellIndex ;
	int p_nEndBurnCellIndex ;
};

extern CDParallel OParallel;

#endif
#endif