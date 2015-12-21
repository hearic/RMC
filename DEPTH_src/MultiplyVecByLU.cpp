# include "ME_Class.h"
# include "Depth_Class.h"

void ME_Class::MultiplyVecByLU(SparseMatrixRCS &LowMatrix, SparseMatrixRCS &UpperMatrix, vector<double> &VectorB)
{
	///// solve LUx=b:  Ly=b, Ux=y 
	////////step 1: Ly=b ///////////////
    int NucNum = Depth.NucNum;

	for(int i = 1;i <= NucNum; ++i)
	{
		int size = LowMatrix._RowColPtr[i].size();
		for(int j = 0;j < size;++j)  
		{
			int col = LowMatrix._RowColPtr[i][j];
			VectorB[i] = VectorB[i] - LowMatrix._RowColVal[i][j]*VectorB[col];
		}
	}


	////////step 2: Ux=y ///////////////
	for(int i = NucNum;i>=1;--i)
	{
		int size = UpperMatrix._RowColPtr[i].size();
		for(int j = 1;j < size;++j)  
		{
			int col = UpperMatrix._RowColPtr[i][j];
			VectorB[i] = VectorB[i] - UpperMatrix._RowColVal[i][j]*VectorB[col];
		}
		if(UpperMatrix._RowColVal[i][0] != 0)
		{
			VectorB[i] = VectorB[i]/UpperMatrix._RowColVal[i][0];
		}
	}

	return;
}