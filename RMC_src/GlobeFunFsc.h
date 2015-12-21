# if ! defined __GlobeFunFsc__H__
# define  __GlobeFunFsc__H__

# include "CommonHead.h"

# define lg2 log10(2.0)

using namespace std;

class GlobeFunFsc {
public:
	/////////// calculate shannon entropy of a normlized distribution ////////////
	static double CalcShanEntropy(const vector<double> &vPdf)
	{
		double SE = 0;
		for( int i = 0 ; i < vPdf.size(); ++i)
		{
			if( vPdf[i] > 0 )
			{                     
				SE = SE - vPdf[i]*log10(vPdf[i]); 
			}  
		}
		SE = SE/lg2;
		return SE;
	}

	/////////// calculate relative entropy of two normlized distribution ////////////
	static double CalcRltvEntropy(const vector<double> &vPdf1,const vector<double> &vPdf2)
	{
		double KLD = 0;

		if(vPdf1.size() != vPdf2.size())
		{
			return 10000;
		}

		for( int i = 0 ; i < vPdf1.size(); ++i)
		{
			if( vPdf1[i] > 0 && vPdf2[i] > 0)
			{               
				KLD = KLD + vPdf1[i]*log10(vPdf1[i]/vPdf2[i]); 
			}  
		}

		KLD = KLD/lg2;
		return KLD;
	}

	////////// Solve eigenvalue problem by powering iteration A*v = k*v ///////////
	static void CalcEigVec(vector<vector<double> > &vA,double &dLamda, vector<double> &vV)
	{
		//////////////////// accelerate power iteration by drift ////////////////////
		int nMatrixSize = vA.size();
		double dDrift = 0;
		if(dDrift > 0)
		{
			for(int i = 0;i < nMatrixSize;++i) 
			{ 
				vA[i][i] = vA[i][i] - dDrift;
			}
		}

		////////// Calculate eigenvector by powering iteration A*v = k*v ////////////
		vector<double> vk(nMatrixSize);
		vector<double> vkk(nMatrixSize);
		vector<double> v_error(nMatrixSize);
		vector<double> z(nMatrixSize);
		double lamda = 0;
		double dError = 1E-6;
		double dErr = 0;

		for(int i = 0;i < nMatrixSize;++i)
		{
			vkk[i] = 1;
		}
		int nIter = 0;
		int nIter_max = 500;
		for(;;) 
		{ 
			double lamda_temp = lamda;   // save old eigenvalue
			lamda = 0;
			for(int i = 0;i < nMatrixSize;++i) 
			{ 
				vk[i] = vkk[i];
				z[i] = 0; 
				for(int j = 0;j < nMatrixSize;++j)
				{
					if(vA[i][j] != 0 && vkk[j] != 0)
					{
						z[i] = z[i] + vA[i][j]*vkk[j];  // z(k)=Av(k-1)
					}
				}                      
				if(z[i] > lamda)
				{
					lamda = z[i];      //m(k)=max(z(k))
				}                             
			} 

			for(int i = 0;i < nMatrixSize;++i) 
			{
				vkk[i] = z[i]/lamda;   // v(k)=z(k)/m(k)
			}

			dErr = fabs(lamda_temp - lamda)/lamda;
			if (dErr <= dError || (++nIter) > nIter_max)
			{
				break;
			}
		} 

		dLamda = lamda + dDrift;
		vV = vkk;
		//printf("iter = %d, Lamda = %f, error = %f\n",iter,Lamda,err);

		if(dDrift > 0)
		{
			for(int i = 0;i < nMatrixSize;++i) 
			{ 
				vA[i][i] = vA[i][i] + dDrift;
			}
		}

		return;
	}

	static void InvertMatrix(vector<vector<double> > &Matrix)
	{
		int i = 0;
		int j = 0;
		int k = 0;
		int m = Matrix.size();
		for(k = 0; k != m; k ++)
		{
			Matrix[k][k] = 1.0 / Matrix[k][k];
			for(i = 0; i != m; i ++)
			{
				if(i != k)
					Matrix[i][k] = - Matrix[i][k] * Matrix[k][k];
			}

			for(i = 0; i != m; i ++)
			{
				if(i != k)
				{
					for(j = 0; j != m; j ++)
					{
						if(j != k)
							Matrix[i][j] += Matrix[k][j] * Matrix[i][k];
					}
				}
			}

			for(j = 0; j != m; j ++)
			{
				if(j != k)
					Matrix[k][j] *= Matrix[k][k];
			}
		}

		return;
	}

	static void MultiplyMatrix(vector<vector<double> > &A,vector<vector<double> > &B,vector<vector<double> > &C)
	{
		// calculate C = AB
		int m = A.size();
		for(int i = 0; i != m; i ++)
		{
			for(int j = 0; j != m; j ++)
			{
				C[i][j] = 0;
				for(int k = 0; k != m; k ++)
				{
					C[i][j] = C[i][j] + A[i][k]*B[k][j];
				}
			}
		}

		return;
	}
};


#endif