# include "Geometry.h"
# include "Boolean.h"

bool CDSingleCell::ParticleIsInCell(const double dPointPos[3],const double dPointDir[3], vector<CDSurface> &vSurface)
{	
	//////// PointPos: 粒子xyz坐标
	//////// CellIndex: 栅元编号
	//////// EnterSurf: 粒子从该面进入栅元

	int nSurf_bool_num;
	int nSurf_bool;
	int nSurf_index;
	double nSense_satisfied;

	static vector<int> vBoolExpress;

	nSurf_bool_num = p_vBooleanSurfaces.size();
	vBoolExpress.resize(nSurf_bool_num) ;
	for(int i = 0;i < nSurf_bool_num;  ++i )
	{
		nSurf_bool = p_vBooleanSurfaces[i];

		///// copy logical symbol /////
		if(CDBoolean::IsBooleanOperator(nSurf_bool))
		{
			vBoolExpress[i] = nSurf_bool;  
			continue ;
		}


		///// calculate surface sense and change it to bool value /////
		/////  the opposite side of entering surface
/*		if( surf_bool == - EnterSurf)     //// could cause bug  2013-06-04
		{
			BoolExpress[i] = 1 ;
			continue ;
		}*/	
		nSurf_index = abs(nSurf_bool);
		nSense_satisfied = nSurf_bool * vSurface[nSurf_index].CalcSurfSense(dPointPos,dPointDir);
		if (nSense_satisfied == 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Particle is on surface: surf = %d, pos = %f %f %f, dir = %f %f %f\n",
				nSurf_bool, dPointPos[0],dPointPos[1],dPointPos[2],dPointDir[0],dPointDir[1],dPointDir[2]);}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		}
		else
		{
			vBoolExpress[i] = nSense_satisfied > 0 ?  1 : 0 ;  
		}
	}


	if( CDBoolean::CalcBoolExpression(vBoolExpress,p_vParenthPos) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

}
