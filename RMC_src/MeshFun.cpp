# include "Mesh.h"
# include "Output.h"

const int INFINITE = -1;
const double ZERO_TRACK_LENGTH = 1.0E-10; ;

void CDMesh::CheckMeshPara(string sInputCard)
{
	GenTotMeshNum();

	Output.CheckInputParas(GetTotMeshNum() >= 2,
		[&](){sprintf(Output.p_sPrintStr,"too few meshes for %s.\n",sInputCard.c_str());},1);

	///////////////////////// BOUND //////////////////////
	for(int i = 0 ; i < 3 ; ++i)
	{

		if(p_nMeshScope[i] == INFINITE)
		{
			continue;
		}

		Output.CheckInputParas(p_nMeshScope[i] > 0,	[&](){sprintf(Output.p_sPrintStr,"incorrect SCOPE parameters in %s card.\n",sInputCard.c_str());},2);

		p_dMeshDelt[i] = (p_dBoundMax[i] - p_dBoundMin[i])/p_nMeshNum[i];

		Output.CheckInputParas(p_dMeshDelt[i] > 0, [&](){sprintf(Output.p_sPrintStr,"incorrect BOUND parameters in %s card.\n",sInputCard.c_str());},2);

	}	
	return;
}


void CDMesh::GenTotMeshNum()
{
	p_nTotMeshNum = p_nMeshNum[0] * p_nMeshNum[1] * p_nMeshNum[2];
	return ;
}


int CDMesh::GetTotMeshNum()
{
	return p_nTotMeshNum;
}


int CDMesh::GetMeshIndex(const double dPos[3], bool bCheckScope)
{
	int nMeshIndex[3];  // start from 0
	int nIndex = 0;     // start from 0,  stored in data array
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( p_nMeshScope[i] == INFINITE)
		{
			nMeshIndex[i] = 0;
			continue;
		}
		if( dPos[i] < p_dBoundMin[i] || dPos[i] > p_dBoundMax[i])   ///// out of bound
		{	
			nIndex = -1;
			break;
		}

		nMeshIndex[i] = int((dPos[i] - p_dBoundMin[i])/p_dMeshDelt[i]);
	}

	if(nIndex == 0)
	{
		nIndex = nMeshIndex[2]*p_nMeshNum[0]*p_nMeshNum[1] + nMeshIndex[1]*p_nMeshNum[0] + nMeshIndex[0];
	}

	if(bCheckScope)
	{
		if(nIndex < 0 || nIndex >= p_nTotMeshNum)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Unknown mesh index!\n");},CDOutput::_WARNING);
		}
	}

	return nIndex;
}


void CDMesh::GetMeshIndexXYZ(int nIndex, int nIndexXyz[3])
{
	// index start from 0, index_xyz start from 1

	nIndexXyz[2] = nIndex/(p_nMeshNum[0]*p_nMeshNum[1]) + 1;

	int temp = (nIndex - (nIndexXyz[2] - 1) * p_nMeshNum[0]*p_nMeshNum[1]) - 1;

	nIndexXyz[1] = temp/p_nMeshNum[0] + 1;

	nIndexXyz[0] = temp - (nIndexXyz[1] - 1 ) * p_nMeshNum[0] + 1;

	return;
}


bool CDMesh::IsInsideMesh(const int nIndexXyz[3])
{
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( nIndexXyz[i] < 0 || nIndexXyz[i] > p_nMeshNum[i] - 1)
		{
			return false;
		}
	}
	return true;
}



void CDMesh::CalcMeshTrck(const double dPos0[3], const double dDir[3],double dTrackLen,vector<CDMeshTrack> &MeshTrack)
{

	double dPos1[3];
	dPos1[0] = dPos0[0] + dDir[0]*dTrackLen; 
	dPos1[1] = dPos0[1] + dDir[1]*dTrackLen;
	dPos1[2] = dPos0[2] + dDir[2]*dTrackLen;

	MeshTrack.resize(0);

	int nIndex0Xyz[3];  // start from 0
	int nIndex1Xyz[3];  // start from 0
	int nCrossNum = 0;
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( p_nMeshScope[i] == INFINITE)
		{
			nIndex0Xyz[i] = 0;
			nIndex1Xyz[i] = 0;
			continue;
		}

		nIndex0Xyz[i] = int(floor((dPos0[i] - p_dBoundMin[i])/p_dMeshDelt[i])) ;
		nIndex1Xyz[i] = int(floor((dPos1[i] - p_dBoundMin[i])/p_dMeshDelt[i])) ;

		nCrossNum = nCrossNum + abs(nIndex0Xyz[i] - nIndex1Xyz[i]);
	}

	////////// simple case: track length is inside single mesh
	if(	nCrossNum == 0)
	{
		if(dTrackLen > 0 && IsInsideMesh(nIndex0Xyz))
		{
			int nMeshIndex = nIndex0Xyz[2]*p_nMeshNum[0]*p_nMeshNum[1] + nIndex0Xyz[1]*p_nMeshNum[0] + nIndex0Xyz[0] ;
			MeshTrack.push_back(CDMeshTrack(nMeshIndex,dTrackLen));
		}
		return;
	}


	////////// other case: track length crosses mesh
	double dDistXyz[3];
	double dWidthXyz[3];
	for(int i = 0 ; i < 3 ; ++i)
	{
		if(p_nMeshScope[i] == INFINITE || dDir[i] == 0)
		{
			dDistXyz[i] = 1.0E+24;
			dWidthXyz[i] = 1.0E+24;
			continue;
		}

		if(dDir[i] > 0)
		{
			dDistXyz[i] =  (p_dBoundMin[i] + (nIndex0Xyz[i] + 1 )* p_dMeshDelt[i]  - dPos0[i])/dDir[i]; 
		}
		else
		{
			dDistXyz[i] =  (p_dBoundMin[i] + nIndex0Xyz[i] * p_dMeshDelt[i]  - dPos0[i])/dDir[i]; 
		}

		dWidthXyz[i] = fabs(p_dMeshDelt[i]/dDir[i]);

	}

	int n = 0;
	bool bIsInside = false;
	for(;;)
	{
		/////////////// find minimum distance ////////////////
		int nMinDistDim = 0;
		if(dDistXyz[1] < dDistXyz[0])
		{
			nMinDistDim = 1;
		}
		if(dDistXyz[2] < dDistXyz[nMinDistDim])
		{
			nMinDistDim = 2;
		}

		double dMinDist = dDistXyz[nMinDistDim];

		///////////////////  Add mesh_index - track_len pair /////////////////
		if(dMinDist > ZERO_TRACK_LENGTH)
		{
			if(IsInsideMesh(nIndex0Xyz))
			{
				int nMeshIndex = nIndex0Xyz[2]*p_nMeshNum[0]*p_nMeshNum[1] + nIndex0Xyz[1]*p_nMeshNum[0] + nIndex0Xyz[0] ;
				MeshTrack.push_back(CDMeshTrack(nMeshIndex,dMinDist));
				bIsInside = true;
			}
			else
			{
				if(bIsInside == true)
				{
					return;
				}
			}
		}


		//////////////////// offset index0_xyz ///////////////////
		if(dDir[nMinDistDim] > 0)
		{
			nIndex0Xyz[nMinDistDim] = nIndex0Xyz[nMinDistDim] + 1;
		}
		else
		{
			nIndex0Xyz[nMinDistDim] = nIndex0Xyz[nMinDistDim] - 1;
		}

		//////////////////// offset dist_xyz ///////////////////
		//if(dMinDist > 1.0E-8) //// bug? wrong location of index when pos0 and pos1 are on grid lines, 20140115, LIANG
		//{
		dTrackLen = dTrackLen - dMinDist;
		for(int i = 0 ; i < 3 ; ++i)
		{
			if(i == nMinDistDim)
			{
				dDistXyz[i] = dWidthXyz[i];
			}
			else
			{
				dDistXyz[i] = dDistXyz[i] - dMinDist;
			}
		}
		//}

		++n;
		if(n >= nCrossNum)
		{		
			if(dTrackLen > ZERO_TRACK_LENGTH && IsInsideMesh(nIndex0Xyz))
			{
				int nMeshIndex = nIndex0Xyz[2]*p_nMeshNum[0]*p_nMeshNum[1] + nIndex0Xyz[1]*p_nMeshNum[0] + nIndex0Xyz[0] ;
				MeshTrack.push_back(CDMeshTrack(nMeshIndex,dTrackLen));
			}
			return;
		}
	}

	return;
}