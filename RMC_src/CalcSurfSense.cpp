# include "Universe.h"
const double ZERO_SENSE = 1.0E-10; 

int CDSurface::CalcSurfSense(const double dPointPos[3],const double dPointDir[3])
{
	//Calculate the sense of the point with respect to the surface
	//**************************************************
	//x,y,z is the position, si is the surface index number
	//**************************************************
	enum eSurfaceType{P = 1, PX = 2, PY = 3, PZ = 4, SO = 5, S = 6, SX = 7, SY = 8, SZ = 9, 
		C_X = 10, C_Y = 11, C_Z = 12, CX = 13, CY = 14, CZ = 15, K_X = 16, K_Y = 17, 
		K_Z = 18, KX = 19, KY = 20, KZ = 21, SQ = 22, GQ = 23, TX = 24, TY = 25, TZ = 26};

	double dSense = 1.0;
	double x = dPointPos[0];
	double y = dPointPos[1];
	double z = dPointPos[2];

	switch(p_nType)
	{
	case P:
		{
			dSense = p_vParas[0]*x+p_vParas[1]*y+p_vParas[2]*z-p_vParas[3];
			break;
		}
	case PX:{
		dSense=x-p_vParas[0];
		break;
			}
	case PY:
		{
			dSense=y-p_vParas[0];
			break;
		}
	case PZ:
		{
			dSense=z-p_vParas[0];
			break;
		}
	case SO:
		{
			dSense=x*x+y*y+z*z-p_vParas[0]*p_vParas[0];
			break;
		}
	case S:
		{
			dSense=(x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1])+
				(z-p_vParas[2])*(z-p_vParas[2])-p_vParas[3]*p_vParas[3];
			break;
		}
	case SX:
		{
			dSense=(x-p_vParas[0])*(x-p_vParas[0])+y*y+z*z-p_vParas[1]*p_vParas[1];
			break;
		}
	case SY:
		{
			dSense=x*x+(y-p_vParas[0])*(y-p_vParas[0])+z*z-p_vParas[1]*p_vParas[1];
			break;
		}
	case SZ:
		{
			dSense=x*x+y*y+(z-p_vParas[0])*(z-p_vParas[0])-p_vParas[1]*p_vParas[1];
			break;
		}
	case C_X:
		{
			dSense=(y-p_vParas[0])*(y-p_vParas[0])+(z-p_vParas[1])*(z-p_vParas[1])-
				p_vParas[2]*p_vParas[2];
			break;
		}
	case C_Y:
		{
			dSense=(x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[1])*(z-p_vParas[1])-
				p_vParas[2]*p_vParas[2];
			break;
		}
	case C_Z:
		{
			dSense=(x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1])-
				p_vParas[2]*p_vParas[2];
			break;
		}
	case CX:
		{
			dSense=y*y+z*z-p_vParas[0]*p_vParas[0];
			break;
		}
	case CY:
		{
			dSense=x*x+z*z-p_vParas[0]*p_vParas[0];
			break;
		}
	case CZ:
		{
			dSense=x*x+y*y-p_vParas[0]*p_vParas[0];
			break;
		}
	case K_X:
		{
			dSense=sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]))
				-p_vParas[4]*sqrt(p_vParas[3])*(x-p_vParas[0]);
			break;
		}
	case K_Y:
		{
			dSense=sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]))
				-p_vParas[4]*sqrt(p_vParas[3])*(y-p_vParas[1]);
			break;
		}
	case K_Z:
		{
			dSense=sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]))
				-p_vParas[4]*sqrt(p_vParas[3])*(z-p_vParas[2]);
			break;
		}
	case KX:
		{
			dSense=sqrt(y*y+z*z)-p_vParas[2]*sqrt(p_vParas[1])*(x-p_vParas[0]);
			break;
		}
	case KY:
		{
			dSense=sqrt(x*x+z*z)-p_vParas[2]*sqrt(p_vParas[1])*(y-p_vParas[0]);
			break;
		}
	case KZ:
		{
			dSense=sqrt(x*x+y*y)-p_vParas[2]*sqrt(p_vParas[1])*(z-p_vParas[0]);
			break;
		}
	case SQ:
		{
			dSense=p_vParas[0]*(x-p_vParas[7])*(x-p_vParas[7])+p_vParas[1]*
				(y-p_vParas[8])*(y-p_vParas[8])+p_vParas[2]*(z-p_vParas[9])*
				(z-p_vParas[9])+2*p_vParas[3]*(x-p_vParas[7])+2*p_vParas[4]*
				(y-p_vParas[8])+2*p_vParas[5]*(z-p_vParas[9])+p_vParas[6];
			break;
		}
	case GQ:
		{
			dSense=p_vParas[0]*x*x+p_vParas[1]*y*y+p_vParas[2]*z*z+p_vParas[3]
			*x*y+p_vParas[4]*y*z+p_vParas[5]*x*z+p_vParas[6]*x+p_vParas[7]*
				y+p_vParas[8]*z+p_vParas[9];
			break;
		}
	case TX:
		{
			dSense=(x-p_vParas[0])*(x-p_vParas[0])/(p_vParas[4]*p_vParas[4])+
				pow(sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]))
				-p_vParas[3],2)/(p_vParas[5]*p_vParas[5])-1;
			break;
		}
	case TY:
		{
			dSense=(y-p_vParas[1])*(y-p_vParas[1])/(p_vParas[4]*p_vParas[4])+
				pow(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]))
				-p_vParas[3],2)/(p_vParas[5]*p_vParas[5])-1;
			break;
		}
	case TZ:
		{
			dSense=(z-p_vParas[2])*(z-p_vParas[2])/(p_vParas[4]*p_vParas[4])+
				pow(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]))-
				p_vParas[3],2)/(p_vParas[5]*p_vParas[5])-1;
			break;
		}
	default:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown surface %s to calculate sense.\n",p_sSurfStr.c_str());},CDOutput::_WARNING);
		}
	}



	if(fabs(dSense) < ZERO_SENSE)
	{
		double dNormVec[3];
		GetSurfNormVec(dPointPos,dNormVec);
		dSense = 0 ;
		for (int i = 0 ; i < 3 ; i++)
		{
			dSense = dSense + dPointDir[i]*dNormVec[i];
		}
	}

	if(dSense > 0)
	{
		return 1;
	}
	else if(dSense < 0)
	{
		return -1;
	}
	else
	{
		return 0 ;
	}

}