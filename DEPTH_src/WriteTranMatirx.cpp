# include "Depth_Class.h"
# include "ME_Class.h"

void ME_Class::WriteTranMatrix()
{
	int NucNum = Depth.NucNum;
	ofstream TranMatrix;
	vector<vector<double> > FullMatrix;
	

	for(int i=1;i<=NucNum;++i)
	{
		FullMatrix.resize(NucNum+1);
		for(int j=1;j<=NucNum;++j)
		{
			FullMatrix[i].resize(NucNum+1);
		}
	}

	for(int i=1;i<=NucNum;++i)
	{
		///////// construct A+I of mixed compressed storage /////////////
		/// diagonal
		FullMatrix[i][i]=-TranMatrixMCS._DiagVal[i]*Time;  

		/// Lower matrix
		int size=TranMatrixMCS._LowerPtr[i].size();
		for(int j=0;j<size;++j)
		{
			int k = TranMatrixMCS._LowerPtr[i][j];
			FullMatrix[k][i] = -TranMatrixMCS._LowerVal[i][j]*Time;
		}
		/// Upper matrix
		size = TranMatrixMCS._UpperPtr[i].size();
		for(int j=0;j<size;++j)
		{
			int k = TranMatrixMCS._UpperPtr[i][j];
			FullMatrix[i][k] = -TranMatrixMCS._UpperVal[i][j]*Time;
		}
	}

	TranMatrix.open("TranMatrix",ios::out);

	TranMatrix.precision(4);
	TranMatrix.setf(Depth.DepthOut.uppercase|Depth.DepthOut.scientific);
	TranMatrix.setf(Depth.DepthOut.left);

	for(int i=1;i<=NucNum;++i)
	{
		for(int j=1;j<=NucNum;++j)
		{
			TranMatrix<<FullMatrix[i][j]<<"  ";
		}
		TranMatrix<<'\n';
	}

	TranMatrix.close();
	return;
}