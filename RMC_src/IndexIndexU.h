# if ! defined __Index_IndexU__H__
# define  __Index_IndexU__H__

# include "CommonHead.h"

using namespace std;

class CDIndex {
public:
	string p_sName;
	vector<int> p_vIndex;    // Index[i]: system index responding to user index i
	vector<int> p_vIndexU;   // IndexU[i]: user index responding to system index i

	CDIndex()
	{
		p_vIndex.resize(1);
		p_vIndexU.resize(1);
	}
	int AddUserIndex(int nIndexU)
	{
		Output.CheckInputParas(nIndexU > 0 ,
			[&](){sprintf(Output.p_sPrintStr,"Wrong %s ID %d.",p_sName.c_str(),nIndexU);},2);

		if(nIndexU >= p_vIndex.size())
		{
			p_vIndex.resize(nIndexU + 1);
		}
		else // if(Index[nIndexU] != 0) //// redefined index
		{
			Output.CheckInputParas(p_vIndex[nIndexU] == 0 ,
				[&](){sprintf(Output.p_sPrintStr,"%s %d is redefined.",p_sName.c_str(),nIndexU);},2);
		}

		p_vIndexU.push_back(nIndexU);
		p_vIndex[nIndexU]  = p_vIndexU.size() - 1;

		return p_vIndex[nIndexU]; ////successfully added
	}

	int GetIndex(int nIndexU)
	{
		if(nIndexU >= p_vIndex.size() || nIndexU < 0)
		{
			printf("Wrong %s index %d.\n",p_sName.c_str(),nIndexU);
			printf("\nPress ENTER to exit...");
			exit(1);
		}
		return p_vIndex[nIndexU];
	}

	int GetIndexU(int nIndex)
	{
		if(nIndex >= p_vIndexU.size())
		{
			printf("Wrong %s index %d.\n",p_sName.c_str(),nIndex);
			printf("\nPress ENTER to exit...");
			exit(1);
		}
		return p_vIndexU[nIndex];
	}

	int GetIndexNum()
	{
		return p_vIndexU.size() - 1;
	}

	int GetIndexUNum()
	{
		return p_vIndex.size() - 1;
	}
};




#endif