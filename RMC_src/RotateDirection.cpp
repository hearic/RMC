#include "NeutronTransport.h"

void CDNeutronTransport::RotateDirection(double dMu,const double dDirOld[3],double dDirNew[3]) 
{
	double dksi1,dksi2;
	double r,s,t;

	if ( fabs(dMu) >= 1.)   // special handling for 1.0 <= fabs(Mu) <= 1.0001 or more.   
	{
		dDirNew[0] = dMu*dDirOld[0];
		dDirNew[1] = dMu*dDirOld[1];
		dDirNew[2] = dMu*dDirOld[2];
		return;
	}
	do
	{
		dksi1 = 2.0*ORNG.Rand() - 1.0;
		dksi2 = 2.0*ORNG.Rand() - 1.0;
		r = (dksi1*dksi1) + (dksi2*dksi2);
	}while( r>1.0);
	r = sqrt((1.0-(dMu*dMu))/r);
	dksi1 = dksi1*r;
	dksi2 = dksi2*r;

	if( fabs(dDirOld[2])<=0.9)// Two ranges for fabs(Mu) < 1.0 .
	{
		s = sqrt(dDirOld[0]*dDirOld[0]+dDirOld[1]*dDirOld[1]);
		t = 1.0/s;
		dDirNew[0] = dDirOld[0]*dMu + (dksi1*dDirOld[0]*dDirOld[2]-dksi2*dDirOld[1])*t;
		dDirNew[1] = dDirOld[1]*dMu + (dksi1*dDirOld[1]*dDirOld[2]+dksi2*dDirOld[0])*t;
		dDirNew[2] = dDirOld[2]*dMu - dksi1*s;                   
	} //renormalize every 50 calls to prevent error buildup.	{}
	else
	{
		//special handling for the case of exceptionally large Dir_Old[2].
		s = sqrt(dDirOld[0]*dDirOld[0]+dDirOld[2]*dDirOld[2]);
		t = 1.0/s;
		dDirNew[0] = dDirOld[0]*dMu+(dksi1*dDirOld[0]*dDirOld[1] + dksi2*dDirOld[2])*t;
		dDirNew[1] = dDirOld[1]*dMu-dksi1*s;
		dDirNew[2] = dDirOld[2]*dMu+(dksi1*dDirOld[2]*dDirOld[1] - dksi2*dDirOld[0])*t;
	}

	return;
}
