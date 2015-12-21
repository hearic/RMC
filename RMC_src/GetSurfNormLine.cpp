# include "Universe.h"
# include "GlobeFun.h"

void CDSurface::GetSurfNormVec(const double dPointPos[3],double dSurfNorm[3])
{
	enum eSurfaceType{P = 1, PX = 2, PY = 3, PZ = 4, SO = 5, S = 6, SX = 7, SY = 8, SZ = 9, 
		C_X = 10, C_Y = 11, C_Z = 12, CX = 13, CY = 14, CZ = 15, K_X = 16, K_Y = 17, 
		K_Z = 18, KX = 19, KY = 20, KZ = 21, SQ = 22, GQ = 23, TX = 24, TY = 25, TZ = 26};

	double x = dPointPos[0];
	double y = dPointPos[1];
	double z = dPointPos[2];

	switch(p_nType)
	{
	case P:
		{
			dSurfNorm[0]=p_vParas[0];
			dSurfNorm[1]=p_vParas[1];
			dSurfNorm[2]=p_vParas[2];
			break;
		}
	case PX:
		{
			dSurfNorm[0]=1;
			dSurfNorm[1]=0;
			dSurfNorm[2]=0;
			break;
		}
	case PY:
		{
			dSurfNorm[0]=0;
			dSurfNorm[1]=1;
			dSurfNorm[2]=0;
			break;
		}
	case PZ:
		{
			dSurfNorm[0]=0;
			dSurfNorm[1]=0;
			dSurfNorm[2]=1;
			break;
		}
	case SO:
		{
			dSurfNorm[0]=x;
			dSurfNorm[1]=y;
			dSurfNorm[2]=z;
			break;
		}
	case S:
		{
			dSurfNorm[0]=x-p_vParas[0];
			dSurfNorm[1]=y-p_vParas[1];
			dSurfNorm[2]=z-p_vParas[2];
			break;
		}
	case SX:
		{
			dSurfNorm[0]=x-p_vParas[0];
			dSurfNorm[1]=y;
			dSurfNorm[2]=z;
			break;
		}
	case SY:
		{
			dSurfNorm[0]=x;
			dSurfNorm[1]=y-p_vParas[0];
			dSurfNorm[2]=z;
			break;
		}
	case SZ:
		{
			dSurfNorm[0]=x;
			dSurfNorm[1]=y;
			dSurfNorm[2]=z-p_vParas[0];
			break;
		}
	case C_X:
		{
			dSurfNorm[0]=0;
			dSurfNorm[1]=y-p_vParas[0];
			dSurfNorm[2]=z-p_vParas[1];
			break;
		}
	case C_Y:
		{
			dSurfNorm[0]=x-p_vParas[0];
			dSurfNorm[1]=0;
			dSurfNorm[2]=z-p_vParas[1];
			break;
		}
	case C_Z:
		{
			dSurfNorm[0]=x-p_vParas[0];
			dSurfNorm[1]=y-p_vParas[1];
			dSurfNorm[2]=0;
			break;
		}
	case CX:
		{
			dSurfNorm[0]=0;
			dSurfNorm[1]=y;
			dSurfNorm[2]=z;
			break;
		}
	case CY:
		{
			dSurfNorm[0]=x;
			dSurfNorm[1]=0;
			dSurfNorm[2]=z;
			break;
		}
	case CZ:
		{
			dSurfNorm[0]=x;
			dSurfNorm[1]=y;
			dSurfNorm[2]=0;
			break;
		}
	case K_X:
		{
			dSurfNorm[0]=-p_vParas[4]*sqrt(p_vParas[3]);
			dSurfNorm[1]=(y-p_vParas[1])/sqrt((y-p_vParas[1])*(y-p_vParas[1])
				+(z-p_vParas[2])*(z-p_vParas[2]));
			dSurfNorm[2]=(z-p_vParas[2])/sqrt((y-p_vParas[1])*(y-p_vParas[1])
				+(z-p_vParas[2])*(z-p_vParas[2]));
			break;
		}
	case K_Y:
		{
			dSurfNorm[0]=(x-p_vParas[0])/sqrt((x-p_vParas[0])*(x-p_vParas[0])
				+(z-p_vParas[2])*(z-p_vParas[2]));
			dSurfNorm[1]=-p_vParas[4]*sqrt(p_vParas[3]);
			dSurfNorm[2]=(z-p_vParas[2])/sqrt((x-p_vParas[0])*(x-p_vParas[0])
				+(z-p_vParas[2])*(z-p_vParas[2]));
			break;
		}
	case K_Z:
		{
			dSurfNorm[0]=(x-p_vParas[0])/sqrt((x-p_vParas[0])*(x-p_vParas[0])
				+(y-p_vParas[1])*(y-p_vParas[1]));
			dSurfNorm[1]=(y-p_vParas[1])/sqrt((x-p_vParas[0])*(x-p_vParas[0])
				+(y-p_vParas[1])*(y-p_vParas[1]));
			dSurfNorm[2]=-p_vParas[4]*sqrt(p_vParas[3]);
			break;
		}
	case KX:
		{
			dSurfNorm[0]=-p_vParas[2]*sqrt(p_vParas[1]);
			dSurfNorm[1]=y/sqrt(y*y+z*z);
			dSurfNorm[2]=z/sqrt(y*y+z*z);
			break;
		}
	case KY:
		{
			dSurfNorm[0]=x/sqrt(x*x+z*z);
			dSurfNorm[1]=-p_vParas[2]*sqrt(p_vParas[1]);
			dSurfNorm[2]=z/sqrt(x*x+z*z);
			break;
		}
	case KZ:
		{
			dSurfNorm[0]=x/sqrt(x*x+y*y);
			dSurfNorm[1]=y/sqrt(x*x+y*y);
			dSurfNorm[2]=-p_vParas[2]*sqrt(p_vParas[1]);
			break;
		}
	case SQ:
		{
			dSurfNorm[0]=p_vParas[0]*(x-p_vParas[7])+p_vParas[3];
			dSurfNorm[1]=p_vParas[1]*(y-p_vParas[8])+p_vParas[4];
			dSurfNorm[2]=p_vParas[2]*(z-p_vParas[9])+p_vParas[5];
			break;
		}
	case GQ:
		{
			dSurfNorm[0]=2*p_vParas[0]*x+p_vParas[3]*y+p_vParas[5]*z+p_vParas[6];
			dSurfNorm[1]=2*p_vParas[1]*y+p_vParas[3]*x+p_vParas[4]*z+p_vParas[7];
			dSurfNorm[2]=2*p_vParas[2]*z+p_vParas[4]*y+p_vParas[5]*x+p_vParas[8];
			break;
		}
	case TX:
		{
			dSurfNorm[0]=(x-p_vParas[0])/(p_vParas[4]*p_vParas[4]);
			dSurfNorm[1]=(sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]))-p_vParas[3])*(y-p_vParas[1])/(p_vParas[5]*p_vParas[5])
				/sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]));
			dSurfNorm[2]=(sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]))-p_vParas[3])*(z-p_vParas[2])/(p_vParas[5]*p_vParas[5])
				/sqrt((y-p_vParas[1])*(y-p_vParas[1])+(z-p_vParas[2])*(z-p_vParas[2]));
			break;
		}
	case TY:
		{
			dSurfNorm[0]=(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]))-p_vParas[3])*(x-p_vParas[0])/(p_vParas[5]*p_vParas[5])
				/sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]));
			dSurfNorm[1]=(y-p_vParas[1])/(p_vParas[4]*p_vParas[4]);
			dSurfNorm[2]=(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]))-p_vParas[3])*(z-p_vParas[2])/(p_vParas[5]*p_vParas[5])
				/sqrt((x-p_vParas[0])*(x-p_vParas[0])+(z-p_vParas[2])*(z-p_vParas[2]));
			break;
		}
	case TZ:
		{
			dSurfNorm[0]=(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]))-p_vParas[3])*(x-p_vParas[0])/(p_vParas[5]*p_vParas[5])
				/sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]));
			dSurfNorm[1]=(sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]))-p_vParas[3])*(y-p_vParas[1])/(p_vParas[5]*p_vParas[5])
				/sqrt((x-p_vParas[0])*(x-p_vParas[0])+(y-p_vParas[1])*(y-p_vParas[1]));
			dSurfNorm[2]=(z-p_vParas[2])/(p_vParas[4]*p_vParas[4]);
			break;
		}
	default:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown normal line of surface %s.\n",p_sSurfStr.c_str());}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}



	CDGlobeFun::Normalize3Array(dSurfNorm);
	return;
}