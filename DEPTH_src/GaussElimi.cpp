# include "Depth_Class.h"
# include "ME_Class.h"

/// Solve A\b  MatrixA\VectorB
void ME_Class::GaussElimi(vector<complex<double> > &MatrixA, vector<int> &IRC, vector<int> &ICFR,  vector<int> &LUP, 
		                           vector<complex<double> > &VectorB)
{
	// reference:  杨绍祺，稀疏矩阵——算法及其程序实现，$6.3.1，P176
	// reference:  吴建平，稀疏线性方程组的高效求解与并行计算，$2.1，P26
	int NucNum = Depth.NucNum;
	int LUPCount = 1;
	int LUPpos;
	
	complex<double> ElimiFactor;

	for(int k=1;k<NucNum;++k)
	{
		////////////////////////// Forward step of Gauss Elimination////////////////////////////
		//if(abs(MatrixA[k])==0)
		//{
		//	Depth.ErrorWarning("Wrong Gauss Elimination.",1);
		//}

		int RowIB = ICFR[k];       
		int RowIC = ICFR[k+1]-1;  
		int ColIB = ICFR[NucNum+k];    
		int ColIC = ICFR[NucNum+k+1]-1;  
		for(int i=RowIB;i<=RowIC;++i)     
		{
			int row=IRC[i];     /// Find the row need to be eliminated
			if(row<=k)
			{
				Depth.ErrorWarning("wrong lower-triangle pointers of Matrix A.",1);
			}

			ElimiFactor=MatrixA[i]/MatrixA[k];

			//cout<<MatrixA[i].real()<<endl<<MatrixA[i].imag()<<endl;

			/////// Elimination and filling-in of  Matrix A and its pointers////////
			for(int j=ColIB;j<=ColIC;++j)    /// Eliminate the columns of the given row
			{			
				int col=IRC[j];
				if(col<=k)
				{
					Depth.ErrorWarning("Wrong upper-triangle pointers of Matrix A.",1);
				}

				LUPpos=LUP[LUPCount];

				MatrixA[LUPpos]=MatrixA[LUPpos]-MatrixA[j]*ElimiFactor;
				++LUPCount;	
				
			} // end for(j=0;j<size2;++j)
			/////// Elimination and filling-in of  vector b////////
			if(VectorB[k] != 0.0)
			{
				VectorB[row]=VectorB[row]-VectorB[k]*ElimiFactor;
			}
		}// end for(i=0;i<size1;++i)
	}// endffor(k=1;k<=NucNum-1;++k)


	////////////////////////// Backward step of Gauss Elimination////////////////////////////
	for(int i=NucNum;i>=1;--i)
	{
		int ColIB = ICFR[NucNum+i];       
		int ColIC = ICFR[NucNum+i+1]-1;  
		//cout<<k<<"   "<<size1<<"   "<<size2<<'\n';
		for(int j=ColIB;j<=ColIC;++j)  
		{
			int col = IRC[j];
			if(col<=i)
			{
				Depth.ErrorWarning("wrong lower-triangle pointers of MatrixA.",1);
			}

			VectorB[i]=VectorB[i]-MatrixA[j]*VectorB[col];
		}
		VectorB[i]=VectorB[i]/MatrixA[i];
	}

	return;
}