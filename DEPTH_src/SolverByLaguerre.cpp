# include "Depth_Class.h"
# include "ME_Class.h"

//// solve exp(-A*tao)
void ME_Class::SolverByLaguerre()
{
	// reference;
	// 1. Orthogonal polynomial expansions for the matrix exponential
	// exp(At)= sum(Pn(At/tao)*Ln(tao)) 
	// Pn = (-1)^n * (At/tao)^n * (I-At/tao)^(-n-a-1) ,  namly,
	// Pn = (-A)^n * [(I*tao/t-A)^(-1)]^n * (tao/t)^(a+1) * [(I*tao/t-A)^(-1)]^(a+1)  
	// or   Pn = exp(-tao/2) * [-(0.5I*tao/t+A)]^n * [(0.5I*tao/t-A)^(-1)]^n * (tao/t)^(a+1) * [(0.5I*tao/t-A)^(-1)]^(a+1)



	//static vector<double> AReal, AImag;
    int NucNum = Depth.NucNum;
	static SparseMatrixRCS MatrixA1;     // MatrixA1 = -TranMatrix*t/Laguerre_Tao        // MatrixA1 =  - 0.5*I*tao/t - A
	static SparseMatrixMCS MatrixA2;     // MatrixA2 = I - TranMatrix*t/Laguerre_Tao    // MatrixA2 = 0.5*I*tao/t - A
	static SparseMatrixRCS LowMatrix;
	static SparseMatrixRCS UpperMatrix;
	static vector<double> L_n(Laguerre_N+2);
	static vector<double> C_n(NucNum+1); // C_n = C_n(A)*n0
	static vector<double > VectorTemp(NucNum+1);
	double Tao_t = Laguerre_Tao/Time ;

	if( Step == 1 )
	{
		MatrixA1.Reset(NucNum+1);
		MatrixA2.Reset(NucNum+1);
		//////// calculate Laguerre polynomials//////
		L_n[0] = 1;
		L_n[1] = 1 + Laguerre_a - Laguerre_Tao;
		for(int n = 1;n < Laguerre_N;++n)
		{
			L_n[n+1]=((2*n + 1 + Laguerre_a - Laguerre_Tao)*L_n[n]-(n + Laguerre_a)*L_n[n-1])/(n+1);
		}
		double caltemp = exp(-0.5*Laguerre_Tao); 
		//for(int n = 0;n <= Laguerre_N;++n)
		//{
		//	L_n[n] = L_n[n] * caltemp;
		//}
	}

	else //matrix has same sparse form --> use previous LUdecomposition results
	{
		if(Depth.TranMatrixChanged == false)
		{
			goto Laguerre;
		}
	}

	///////////////// construct A1 = -0.5*I*tao/t - A and A2 = 0.5*I*tao/t - A ///////////////////
	Depth.GenTranMatrix(1);   /// construct A

	for(int i = 1;i <= NucNum;++i)
	{
		///////// construct A1 of row compressed storage /////////////
		int size = TranMatrixRCS.ColNum(i);
		MatrixA1._RowColPtr[i].resize(size);
		MatrixA1._RowColVal[i].resize(size);
		for(int j = 0;j < size;++j)
		{
			int Col = TranMatrixRCS._RowColPtr[i][j];
			MatrixA1._RowColPtr[i][j] = Col;
			if(Col == i)  // diagonal elements
			{
				MatrixA1._RowColVal[i][j] = -TranMatrixRCS._RowColVal[i][j];
			}
			else
			{
				MatrixA1._RowColVal[i][j] = -TranMatrixRCS._RowColVal[i][j];
			}
		}

		///////// construct A2 of mixed compressed storage /////////////
		/// diagonal
		MatrixA2._DiagVal[i] = 1.0*Tao_t - TranMatrixMCS._DiagVal[i];  
		/// Lower matrix
		size = TranMatrixMCS._LowerPtr[i].size();
		MatrixA2._LowerPtr[i].resize(size);
		MatrixA2._LowerVal[i].resize(size);
		for(int j = 0;j < size; ++j)
		{
			MatrixA2._LowerPtr[i][j] = TranMatrixMCS._LowerPtr[i][j];
			MatrixA2._LowerVal[i][j] = -TranMatrixMCS._LowerVal[i][j];
		}
		/// Upper matrix
		size = TranMatrixMCS._UpperPtr[i].size();
		MatrixA2._UpperPtr[i].resize(size);
		MatrixA2._UpperVal[i].resize(size);
		for(int j = 0;j < size; ++j)
		{
			MatrixA2._UpperPtr[i][j] = TranMatrixMCS._UpperPtr[i][j];
			MatrixA2._UpperVal[i][j] = -TranMatrixMCS._UpperVal[i][j];
		}
	}

	//LowMatrix.Clear();
	//UpperMatrix.Clear();
	LowMatrix.Reset(NucNum+1);
	UpperMatrix.Reset(NucNum+1);
	
	//// Perform LU Decomposition of matrix A2
	DecmpMatrixByLU(MatrixA2, LowMatrix, UpperMatrix);

Laguerre:
	////////////////// get a copy of nuclide densities ///////////////
	for(int i = 1;i <= NucNum; i++)
	{
		C_n[i] = Depth.StepNt[Step-1][i];
	}


	////////////// n = 0 , (tao/t)^(a+1) * [(0.5I*tao/t-A)^(-1)]^(a+1) ////////////////
	MultiplyVecByLU(LowMatrix, UpperMatrix,C_n); // C_n, n=0
	for(int i = 0 ; i < Laguerre_a ; ++i)
	{
		MultiplyVecByLU(LowMatrix, UpperMatrix,C_n); //generalized Laguerre polynomials
		Tao_t = Tao_t * Laguerre_Tao/Time;
	}
	for(int i = 1;i <= NucNum;i++)
	{
		C_n[i] = C_n[i] * Tao_t ;
		Depth.StepNt[Step][i] = C_n[i]*L_n[0]; 
	}

	////////////// n=1,2,3... ////////////////
	for(int n = 1;n <= Laguerre_N; ++n)
	{
		///// calculate C_n(n)=A1*[A2\C_n[n-1]] /////////
		///b = A2\C_n[n-1]///
		MultiplyVecByLU(LowMatrix, UpperMatrix,C_n);

		///C_n(n) = A1*b///
		for(int i = 1;i <= NucNum;i++)
		{
			VectorTemp[i] = C_n[i];
		}
		for(int i = 1;i <= NucNum;i++)
		{
			C_n[i] = 0;
			int size = MatrixA1._RowColPtr[i].size();
			for(int j = 0;j < size;++j)  
			{
				int col = MatrixA1._RowColPtr[i][j];
				C_n[i] = C_n[i] + MatrixA1._RowColVal[i][j]*VectorTemp[col]; 
			}
		}

		/////// calculate exp(At) /////
		for(int i = 1;i <= NucNum;i++)
		{
			Depth.StepNt[Step][i] = Depth.StepNt[Step][i] + C_n[i]*L_n[n]; 
		}
	}

	return;
}