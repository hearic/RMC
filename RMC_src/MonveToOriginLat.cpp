# include "Universe.h"

void CDUniverse::MoveToOriginLat(const int nLatIndex, double dPointPos[3])
{
	//////////// Given lattice index, move particle to original lattice //////////////
	if(p_nLatType == 1)     ////// 3D rectangle
	{
		/////// vector basis: x=(1,0,0), y=(0,1,0), z=(0,0,1) ///////

		int n_xyz[3];
		n_xyz[2] = (nLatIndex -1)/(p_nScope[0]*p_nScope[1]) ;
		n_xyz[1] = (nLatIndex - 1 - p_nScope[0]*p_nScope[1]*n_xyz[2])/p_nScope[0]  ;
		n_xyz[0] =  nLatIndex - 1 - p_nScope[0]*p_nScope[1]*n_xyz[2] - p_nScope[0]*n_xyz[1] ;

		for(int i = 0; i < 3; ++i)
		{
			if(p_nScope[i] != 1)
			{
				dPointPos[i] = dPointPos[i] - p_dPitch[i] * n_xyz[i];
			}
		}
	}

	else if(p_nLatType == 2)  /////// 2D hexagon
	{
		/////// vector basis: b1=(L1,0) and b2 = (L1/2,height) ///////
		double nLen1 = p_dPitch[0]; // 	L2 = Pitch[1];
		int i2 = (nLatIndex - 1)/p_nScope[0];
		int i1 = nLatIndex -1 - i2*p_nScope[0];

		dPointPos[0] = dPointPos[0] - (i1*nLen1 + i2*0.5*nLen1);
		dPointPos[1] = dPointPos[1] - i2*p_dHeight;
	}

	return;
}
