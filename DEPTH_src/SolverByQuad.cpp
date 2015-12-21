# include "Depth_Class.h"
# include "ME_Class.h"

void ME_Class::SolverByQuad()
{
	int NucNum = Depth.NucNum;
	static vector<complex<double> > Quad_z,Quad_c;
	static vector<complex<double> > MatrixA;
	static vector<int> IRC, ICFR, LUP;
	static vector<complex<double> > VectorB(NucNum+1);
	
	if(Step == 1)
	{
		CalcQuadCoeffs(Quad_z,Quad_c);
	}

	if(Step>1)//matrix has same sparse form --> use previous Symbol-Gauss-Elimination results
	{
		if(Depth.TranMatrixChanged == false) 
		{
			goto Quad;
		}
	}

	GaussElimiSymbol( IRC, ICFR, LUP);
	//size=IRC.size();
	MatrixA.resize(IRC.size());

Quad:
	for(int m=1;m<= Quad_N/2;++m)
	{
		for(int i=1;i<=NucNum;++i)
		{
			VectorB[i]=complex<double>(Depth.StepNt[Step-1][i],0);

			//////  Generate diagonal element of Matrix (Br+Bi*i)*I+A*t
			MatrixA[i]=complex<double>(TranMatrixMCS._DiagVal[i]*Time,0)-Quad_z[m];

			//////  Generate lower triangle of Matrix A*t-z(m)*I
			int RowIB=ICFR[i];
			int RowIC=ICFR[i+1]-1;
			int size1=TranMatrixMCS._LowerPtr[i].size();
			for(int j=RowIB;j<RowIB+size1;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(TranMatrixMCS._LowerVal[i][j-RowIB]*Time,0);
			}
			for(int j = RowIB+size1;j<=RowIC;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(0,0);
			}

			//////  Generate upper triangle of Matrix A*t-z(m)*I
			int ColIB=ICFR[NucNum+i];
			int ColIC=ICFR[NucNum+i+1]-1;
			int size2=TranMatrixMCS._UpperPtr[i].size();
			for(int j=ColIB;j<ColIB+size2;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(TranMatrixMCS._UpperVal[i][j-ColIB]*Time,0);
			}
			for(int j=ColIB+size2;j<=ColIC;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(0,0);
			}
		}

		GaussElimi(MatrixA,  IRC, ICFR, LUP, VectorB);


		for(int i=1;i<=NucNum;i++)
		{
			if(VectorB[i]==0.0){continue;}
			Depth.StepNt[Step][i] = Depth.StepNt[Step][i]+(Quad_c[m].real()*VectorB[i].real()-Quad_c[m].imag()*VectorB[i].imag());  
		}
	}   // end if for(m=1;m<=Quad_N/2;++m)

	for(int i=1;i<=NucNum;i++)
	{
		Depth.StepNt[Step][i]=2*Depth.StepNt[Step][i];
	}

	return;

}