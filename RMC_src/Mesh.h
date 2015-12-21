# if ! defined __MESH__H__
# define  __MESH__H__

# include "CommonHead.h"


using namespace std;

class CDMeshTrack
{
public:
	int p_nMeshIndex;
	double p_dTrackLen;

	CDMeshTrack(int mesh_index = 0, double track_len = 0)
	{
		p_nMeshIndex = mesh_index;
		p_dTrackLen = track_len;
	}
};


class CDMesh 
{
public:
	int p_nMeshScope[3];     /// read from scope card
	int p_nMeshNum[3];       /// convert from scope card (note: "-1")
	double p_dBoundMin[3];
	double p_dBoundMax[3];
	double p_dMeshDelt[3];
	int p_nTotMeshNum;

	CDMesh()
	{
		for(int i = 0 ; i < 3 ; ++i)
		{
			p_dBoundMin[i] = -1.0E+24;
			p_dBoundMax[i] = 1.0E+24;
			p_dMeshDelt[i]=0;
			p_nMeshScope[i] = 1;
			p_nMeshNum[i] = 1;	
		}
		p_nTotMeshNum = 1 ;
		return;
	}

	//MeshClass(const MeshClass &MeshTemp)
	//{
	//	for(int i = 0 ; i < 3 ; ++i)
	//	{
	//		BoundMin[i] = MeshTemp.BoundMin[i];
	//		BoundMax[i] = MeshTemp.BoundMax[i];
	//		MeshDelt[i] = MeshTemp.MeshDelt[i];
	//		MeshNum[i] = MeshTemp.MeshNum[i];
	//        MeshScope[i] = MeshTemp.MeshScope[i];
	//	}
	//	TotMeshNum = MeshTemp.TotMeshNum ;
	//	return;
	//}


	void CheckMeshPara(string sInputCard);
	void GenTotMeshNum();
	int GetTotMeshNum();
	int GetMeshIndex(const double dPos[3], bool bCheckScope);
	void GetMeshIndexXYZ(int nIndex, int nIndexXyz[3]);
	void CalcMeshTrck(const double dPos[3], const double dDir[3],double dTrackLen,vector<CDMeshTrack> &MeshTrack);
	bool IsInsideMesh(const int nIndexXyz[3]);
	
};

#endif