# if ! defined __FUNCTIONS__H__
# define  __FUNCTIONS__H__

# include "CommonHead.h"
using namespace std;

class CDGlobeFun {
public:

	static void NormalizeVec(vector<double> &dVec)
	{
		double dSum = 0;
		for( int i = 0 ; i < dVec.size(); ++i)
		{
			dSum = dSum + dVec[i]; 
		}

		if(dSum == 0) return;

		for( int i = 0 ; i < dVec.size(); ++i)
		{
			dVec[i] = dVec[i]/dSum; 
		}

		return;
	}

	static double SumupVec(vector<double> &dVec)
	{
		double dSum = 0;
		for( int i = 0 ; i < dVec.size(); ++i)
		{
			dSum = dSum + dVec[i]; 
		}		

		return dSum;
	}

	static void Normalize3Array(double dArrayTemp[3])
	{
		double dLength_temp;
		dLength_temp = 1.0/sqrt(dArrayTemp[0]*dArrayTemp[0]+dArrayTemp[1]*dArrayTemp[1]+dArrayTemp[2]*dArrayTemp[2]);
		dArrayTemp[0] = dArrayTemp[0]*dLength_temp;
		dArrayTemp[1] = dArrayTemp[1]*dLength_temp;
		dArrayTemp[2] = dArrayTemp[2]*dLength_temp;
	}

	template <typename T> static double IntpltByPosFr(const T *vec, int nPos, double fr)
	{
		return *(vec + nPos) + (*(vec + nPos + 1) - *(vec + nPos))*fr;
	}


	template <typename T> static void GetIntpltPos(const T *vec, T val, int min, int max, int &nPos)
	{
		if( val <= *(vec + min) )
		{
			nPos = min;
			return;
		}				 
		if(val >= *(vec + max) )
		{
			nPos = max ;   //// note : this place is different with GetIntpltPosFr
			return;
		}

		while(max - min > 1)
		{
			int mid = (min + max)/2;
			if(val >= *(vec + mid) )
			{
				min = mid;
			}
			else
			{
				max = mid;
			}
		}
		nPos = min;
		return;
	}
	
	template <typename T> static void GetIntpltPosFr (const T *vec, T val, int min, int max, int &nPos, double &fr)
	{
		if( val <= *(vec+min) )
		{
			nPos = min;
			fr = 0;
			return;
		}				 
		if(val >= *(vec+max) )
		{
			nPos = max-1;	
			fr = 1;
			return;
		}

		while(max - min > 1)
		{
			int mid = (min + max)/2;
			if(val >= *(vec + mid) )
			{
				min = mid;
			}
			else
			{
				max = mid;
			}
		}

		nPos = min;
		if(*(vec + min + 1) ==  *(vec + min))
		{
			fr = 0 ;
		}
		else
		{
			fr = (val - *(vec + min))/( *(vec + min + 1) -  *(vec + min));
		}
		return;
	}


	template <typename T> static void Set1DVecZero(vector<T> &vVec)
	{
		for(int i = 0;i < vVec.size(); ++i)
		{
			vVec[i] = 0;
		}
		return;
	}

	template <typename T> static void Set2DVecZero(vector<vector<T> > &vVec)
	{
		for(int i = 0;i < vVec.size(); ++i)
		{
			for(int j = 0 ; j < vVec[i].size(); ++j)
			{
				vVec[i][j] = 0;
			}
		}
		return;
	}

	template <typename T> static void Gen2DZeroMatrix(vector<vector<T> > &vVec,int d1, int d2)
	{
		vVec.resize(d1);
		for(int i = 0;i < d1; ++i)
		{
			vVec[i].resize(d2);
			for(int j = 0 ; j < d2; ++j)
			{
				vVec[i][j] = 0;
			}
		}
		return;
	}

	template <typename T> static void SortTwoVec(vector<T> &vData, int nLeft, int nRight, vector<int> &vIndex)  
	{
		//////// descending sort two vectors by the first vector
		int i = nLeft;
		int j = nRight;
		T middle = vData[(nLeft+nRight)/2];   // middle vanue
		while(i<=j)
		{
			while(vData[i] > middle && i < nRight){ i = i + 1;}   //scan from the left              		
			while(vData[j] < middle && j > nLeft){ j = j - 1;}   //scan from the right        
			if(i <=j ) //found and swap
			{
				T rTemp = vData[j];
				vData[j] = vData[i];
				vData[i] = rTemp;

				int iTemp = vIndex[j];
				vIndex[j] = vIndex[i];
				vIndex[i] = iTemp;
				i=i+1;
				j=j-1;
			}
		}	
		if(nLeft < j) //left recursion  
		{
			SortTwoVec(vData,nLeft,j,vIndex);
			//right=j;//goto go10;
		}	
		if(nRight > i)//  right recursion  
		{
			SortTwoVec(vData,i,nRight,vIndex);
			//left=i;	//goto go10; 
		}
		return;
	}


	template <typename T> static int CheckVecSign(vector<T> &vVec)
	{
		double dFind_negative = false;
		double dFind_positive = false;
		for(int i = 0;i < vVec.size(); ++i)
		{
			if(vVec[i] >= 0)
			{
				dFind_positive = true;
			}
			if(vVec[i] <= 0)
			{
				dFind_negative = true;
			}
		}

		if(dFind_positive && !dFind_negative)
		{
			return 1;
		}
		if(!dFind_positive && dFind_negative)
		{
			return -1;
		}
		return 0;
	}
};


#endif