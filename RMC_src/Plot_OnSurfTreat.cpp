# include "Plot.h"


int CDPlot::AdjustPosition(CDGeometry &cGeometry, CDVector3 &cVertex_temp, CDVector3 cDir_temp)
{
	/*
	check the input point if on one surface, then adjust the position(most 10 times half pixel)
	return value
	 0: no adjust
	 1: adjust success
	-1: adjust failed
	*/
/*
	Vector3Class Vertex_temp = Vertex1;
	Vertex_temp.MoveDistance(TrackDirection, DeltaWidth/2.0);
	Vertex_temp.MoveDistance(HeightDirection, DeltaHeight/2.0);
*/

	double dPos[3], dDir[3];	
	dDir[0] = cDir_temp._x;	dDir[1] = cDir_temp._y;	dDir[2] = cDir_temp._z;

	int nSense_satisfied;
	int nSurf_num = cGeometry.p_vSurface.size();
	int nSurf_i;
	int nAdjust_time  = 0;
	while(nAdjust_time < 10)
	{
		dPos[0] = cVertex_temp._x;	dPos[1] = cVertex_temp._y;	dPos[2] = cVertex_temp._z;
		for (nSurf_i = 1; nSurf_i < nSurf_num; nSurf_i++)
		{
			nSense_satisfied = cGeometry.p_vSurface[nSurf_i].CalcSurfSense(dPos,dDir);
			if (nSense_satisfied == 0)
			{
				double dNormVec[3];
				cGeometry.p_vSurface[nSurf_i].GetSurfNormVec(dPos,dNormVec);

				CDVector3 cAdjustDir(dNormVec);//define adjust direction by normalize vector
				cVertex_temp.MoveDistance(cAdjustDir, m_dDeltaWidth2/20.0);//adjust 1/20 of pixel one time
				nAdjust_time++;

				break;//break current check, continue next cycle check
			}
		}
		if (nSurf_i == nSurf_num ) 
		{
			if (nAdjust_time == 0)return 0;//no need to adjust
			else return 1;//adjust success
		}
	}
	return -1;//adjust failed
}



bool CDPlot::CheckOnSurf(CDGeometry &cGeometry, CDVector3 cVertex_temp, CDVector3 cDir_temp)
{
	double dPos[3], dDir[3];	
	dPos[0] = cVertex_temp._x;	dPos[1] = cVertex_temp._y;	dPos[2] = cVertex_temp._z;
	dDir[0] = cDir_temp._x;	dDir[1] = cDir_temp._y;	dDir[2] = cDir_temp._z;

	int nSense_satisfied;
	int nSurf_num = cGeometry.p_vSurface.size();
	int nSurf_i;
	for (nSurf_i = 1; nSurf_i < nSurf_num; nSurf_i++)
	{
		nSense_satisfied = cGeometry.p_vSurface[nSurf_i].CalcSurfSense(dPos,dDir);
		if (nSense_satisfied == 0)//On surfaces
		{
			return true;
		}
	}

	return false;
}
