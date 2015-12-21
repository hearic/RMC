# if ! defined __ME_Class__
# define  __ME_Class__

# include <math.h>
# include <complex>
# include <vector>
# include "SparseMatrix.h"

using namespace std;

class ME_Class   // solvers of matrix exponential
{
public:
	////////// Matrix Exponential Functions////////
	void WriteTranMatrix();
	void CalcMatrixNorm();
	void SolverByCRAM();
	void SolverByLaguerre();
	void SolverByQuad();
	void CalcQuadCoeffs(vector<complex<double> > &Quad_z,vector<complex<double> > &Quad_c);
	void DecmpMatrixByLU(SparseMatrixMCS &MatrixA, SparseMatrixRCS &LowMatrix, SparseMatrixRCS &UpperMatrix);
	void MultiplyVecByLU(SparseMatrixRCS &LowMatrix, SparseMatrixRCS &UpperMatrix, vector<double> &VectorB);
	void GaussElimiSymbol(vector<int> &IRC, vector<int> &ICFR,  vector<int> &LUP);
	void GaussElimi(vector<complex<double> > &MatrixA, vector<int> &IRC, vector<int> &ICFR,  vector<int> &LUP, 
		                           vector<complex<double> > &VectorB);


	int Step;
	double Time;

	/////////////////// Matrix Exponential Variables //////////////////
	SparseMatrixMCS TranMatrixMCS; // row&col mixed store strategy	
	SparseMatrixRCS TranMatrixRCS;


	///////////// generalized Laguerre polynomials L(a,Tao) ////////////
	int Laguerre_N;
	int Laguerre_a; 
	double Laguerre_Tao;

	int Quad_N;
	int ScaleFlag;
	double TranMatrixNorm;
	int TranMatrixNormNuc;

};

extern ME_Class ME;

#endif