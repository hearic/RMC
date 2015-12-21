# if ! defined __CELLMAP__H__
# define  __CELLMAP__H__

# include "CommonHead.h"
# include "Output.h"

using namespace std;

typedef map<long long, int> LonglongIntMap;

class CDLonglongInt 
{
public:
	long long Key;
	int Val;
};

class CDLongIntMap   
{
///////////////////////////////////////////////////////////////
////// This class is used to generate LongLong-int map   //////
////// It is used for cell mapping method in cell tally  //////
///////////////////////////////////////////////////////////////
public:
	vector<CDLonglongInt> p_vKeyValPair;

public:
	void Resize(int size);
	int GetMapSize();
	long long ConvertMapKey(const vector<int> &vec);
	void AddMapPair(int i,long long key,int val);
	void ResortMapKey();
	bool GetMapVal(long long key,int &val,int min, int max); 
};


class CDBinPtr 
{
public:
	int Lower;
	int Upper;
	CDBinPtr()
	{
		Lower = -1;
		Upper = -1;
	}
};

class CDBinMap   
{
//////////////////////////////////////////////////////////////////////////////////////////
////// This class is used to generate pointer map corresponding to a given array    //////
////// It is used to speed up binary search by deciding the coarse bin of the value //////
////// Hash Function 1 frexp :  x -> s * 2^t,  s is between [0.5, 1), t is integer  //////
////// Hash Function 2          x[a,b]  ->  x= (x - a)/(b - a)  -> index = 1000*x   //////
//////////////////////////////////////////////////////////////////////////////////////////
public:
	int p_nHashType;    /// type 1: exp   type 2: linear

	int p_nFraNum;
	int p_nExpNum;
	int p_nExpStart;
	double p_dBound_a;
	double p_dBound_b;
	double p_dScale;

	int p_nIndex;

	/////////////// Given a float point number, Get the index of BinPtrArray ///////////////
	int GetBinMapIndex(double val);

	////////////// Given an array of float point number, generate bin pointer array ////////////////
	void GenBinMapOfArray(vector<CDBinPtr> &BinPtrArray, const double *vec,int start_pos, int end_pos);
};

#endif