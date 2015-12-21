# include "Universe.h"

double CDSurface:: CalcDistToSurf(const double dPointPos[3], const double dPointDirect[3],bool bIsAtSurf)
{
	//function CalcDistToSurf: the distance of the point with respect to the surface PointToSurf
	//**************************************************
	//x,y,z is the position, si is the surface index
	//**************************************************
	enum eSurfaceType{P = 1, PX = 2, PY = 3, PZ = 4, SO = 5, S = 6, SX = 7, SY = 8, SZ = 9, 
		C_X = 10, C_Y = 11, C_Z = 12, CX = 13, CY = 14, CZ = 15, K_X = 16, K_Y = 17, 
		K_Z = 18, KX = 19, KY = 20, KZ = 21, SQ = 22, GQ = 23, TX = 24, TY = 25, TZ = 26};

	double x,y,z,u,v,w;
	x = dPointPos[0];
	y = dPointPos[1];
	z = dPointPos[2];
	u = dPointDirect[0];
	v = dPointDirect[1];
	w = dPointDirect[2];
	double t1,t2,t3,t4,a1,b1,uu;
	double tk=0;
	double d1 = -1.0;
	double d2 = -1.0;
	double t9 = 1.0e+10;

	double dts = -1;
	switch(p_nType)
	{
	case P:
		{
			t1 = p_vParas[0]*u + p_vParas[1]*v + p_vParas[2]*w;
			if(t1 == 0){return dts;}
			else
			{
				t2 = p_vParas[0]*x+p_vParas[1]*y+p_vParas[2]*z;
				dts = (p_vParas[3]-t2)/t1;
				return dts;
			}
		}
	case PX:
		{
			if(u==0){return dts;}
			else{dts=(p_vParas[0]-x)/u;return dts;}
		}
	case PY:
		{
			if(v==0){return dts;}
			else{dts=(p_vParas[0]-y)/v;return dts;}
		}
	case PZ:
		{
			if(w==0){return dts;}
			else{dts=(p_vParas[0]-z)/w;return dts;}
		}
	case SO:
		{
			a1 = x*u+y*v+z*w;
			b1 = x*x+y*y+z*z-p_vParas[0]*p_vParas[0];
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case S:
		{
			t1 = x - p_vParas[0];
			t2 = y - p_vParas[1];
			t3 = z - p_vParas[2];
			a1 = t1*u + t2*v + t3*w;
			b1 = t1*t1 + t2*t2 + t3*t3 - p_vParas[3]*p_vParas[3];
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case SX:
		{
			t1 = x - p_vParas[0];
			a1 = t1*u + y*v + z*w;
			b1 = t1*t1 + y*y + z*z - p_vParas[1]*p_vParas[1];
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}

	case SY:
		{
			t2 = y - p_vParas[0];
			a1 = x*u + t2*v + z*w;
			b1 = x*x + t2*t2 + z*z - p_vParas[1]*p_vParas[1];
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case SZ:
		{
			t3 = z - p_vParas[0];
			a1 = x*u + y*v + t3*w;
			b1 = x*x + y*y + t3*t3 - p_vParas[1]*p_vParas[1];
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case C_X:
		{
			t1 = v*v + w*w;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			t2 = y - p_vParas[0];
			t3 = z - p_vParas[1];
			a1 = (t2*v + t3*w)*t1;
			b1 = (t2*t2 + t3*t3 - p_vParas[2]*p_vParas[2])*t1;
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case C_Y:
		{
			t1 = u*u + w*w;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			t2 = x - p_vParas[0];
			t3 = z - p_vParas[1];
			a1 = (t2*u + t3*w)*t1;
			b1 = (t2*t2 + t3*t3 - p_vParas[2]*p_vParas[2])*t1;
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case C_Z:
		{
			t1 = u*u + v*v;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			t2 = x - p_vParas[0];
			t3 = y - p_vParas[1];
			a1 = (t2*u + t3*v)*t1;
			b1 = (t2*t2 + t3*t3 - p_vParas[2]*p_vParas[2])*t1;
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case CX:
		{
			t1 = v*v + w*w;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			a1 = (y*v + z*w)*t1;
			b1 = (y*y + z*z - p_vParas[0]*p_vParas[0])*t1;
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case CY:
		{
			t1 = u*u + w*w;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			a1 = (x*u + z*w)*t1;
			b1 = (x*x + z*z - p_vParas[0]*p_vParas[0])*t1;
			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case CZ:
		{
			t1 = u*u + v*v;
			if( t1==0. ) {return dts;}
			t1 = 1./t1;
			a1 = (x*u + y*v)*t1;
			b1 = (x*x + y*y - p_vParas[0]*p_vParas[0])*t1;

			return CalcDistToSurfSub(a1,b1,bIsAtSurf);
		}
	case K_X:
		{
			t4 = v*v + w*w - p_vParas[3]*u*u;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t1 = y - p_vParas[1];
			t2 = z - p_vParas[2];
			t3 = x - p_vParas[0];
			uu = u;
			tk = p_vParas[4];
			a1 = (t1*v + t2*w - t3*u*p_vParas[3])*t9;
			b1 = (t1*t1 + t2*t2 - p_vParas[3]*t3*t3)*t9;
			if( t9!=1.e10 ) {goto go200;}
			goto go100;
		}
	case K_Y:
		{
			t4 = u*u + w*w - p_vParas[3]*v*v;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t1 = x - p_vParas[0];
			t2 = z - p_vParas[2];
			t3 = y - p_vParas[1];
			uu = v;
			tk = p_vParas[4];
			a1 = (t1*u + t2*w - t3*v*p_vParas[3])*t9;
			b1 = (t1*t1 + t2*t2 - p_vParas[3]*t3*t3)*t9;
			if( t9!=1.e10 ) {goto go200;}
			goto go100;
		}
	case K_Z:
		{
			t4 = u*u + v*v - p_vParas[3]*w*w;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t1 = y - p_vParas[1];
			t2 = x - p_vParas[0];
			t3 = z - p_vParas[2];
			uu = w;
			tk = p_vParas[4];
			a1 = (t1*v + t2*u - t3*w*p_vParas[3])*t9;
			b1 = (t1*t1 + t2*t2 - p_vParas[3]*t3*t3)*t9;
			if( t9!=1.e+10 ) {goto go200;}
			goto go100;
		}
	case KX:
		{	
			t4 = v*v + w*w - p_vParas[1]*u*u;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t3 = x - p_vParas[0];
			uu = u;
			tk = p_vParas[2];
			a1 = (z*w + y*v - t3*u*p_vParas[1])*t9;
			b1 = (z*z + y*y - p_vParas[1]*t3*t3)*t9;
			if( t9!=1.e10 ) {goto go200;}
			goto go100;
		}
	case KY:
		{
			t4 = u*u + w*w - p_vParas[1]*v*v;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t3 = y - p_vParas[0];
			uu = v;
			tk = p_vParas[2];
			a1 = (x*u + z*w - t3*v*p_vParas[1])*t9;
			b1 = (x*x + z*z - p_vParas[1]*t3*t3)*t9;
			if( t9!=1.e10 ) {goto go200;}
			goto go100;
		}
	case KZ:
		{
			t4 = u*u + v*v - p_vParas[1]*w*w;
			if( fabs(t4)>1.e-10 )  {t9 = 1./t4;}
			t3 = z - p_vParas[0];
			uu = w;
			tk = p_vParas[2];
			a1 = (x*u + y*v - t3*w*p_vParas[1])*t9;
			b1 = (x*x + y*y - p_vParas[1]*t3*t3)*t9;
			if( t9!=1.e10 ) {goto go200;}
			goto go100;
		}
		//case 22:{sq}
		//case 23:{gq}
		//case 24:{tx}
		//case 25:{ty}
		//case 26:{tz}
	}

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown surface %s to calculate distance.\n",p_sSurfStr.c_str());},CDOutput::_WARNING);

	return 1;

go100 :
	// calculate a single distance.  (rare case)
	if( fabs(a1) <= 1.e-10 || bIsAtSurf) {return dts;}
	d1 = -0.5*b1/a1;
	if( d1<=0. ){return dts;}
	goto go300;

go200:  // calculate a pair of distances.
	t1 = a1*a1-b1;
	if( t1<0. )  {return dts;}
	if(bIsAtSurf) 
	{
		d1=-2*a1;
		if( d1<=0) {return dts;}
	}
	else
	{
		t2 = sqrt(t1);
		d1 = -a1+t2;
		if( d1<=0. ) {return dts;}
		d2 = -a1-t2;
	}

go300: // reject the distance to the wrong sheet of a cone.
	if( tk!=0) 
	{
		if( tk*(t3+d1*uu)<0. ) {d1 = -1;}
		if( tk*(t3+d2*uu)<0. ) {d2 = -1;}
		d1 = max(d1,d2);
		if( d1<=0. ) {return dts;}
	}

	if( d2 > 0)  {dts = d2;}//select the smallest positive distance.
	else{dts = d1;}
	return dts;
}

double CDSurface::CalcDistToSurfSub(double a1, double b1, bool bIsAtSurf)
{
	double t1 = a1*a1-b1;
	if( t1 < 0. )
	{
		return -1;
	}
	if(bIsAtSurf) 
	{
		return -2*a1;
	}
	else
	{
		double t2 = sqrt(t1);
		double d1 = -a1 + t2;
		if( d1 <= 0. )
		{
			return -1;
		}
		double d2 = -a1-t2;

		return d2 > 1.0E-10 ? d2 : d1;
	}
}