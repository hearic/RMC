#include "Criticality.h"
#include "PointKinetics.h"

void CDPointKinetics::Adaptive_RKCK54(CDPointReactor f,std::vector<double>& x,double start_time,double end_time,double dt,double abs_eps,double rel_eps)
{
	// Solver for Pointer reactors equations.
	// ARKCK: Adaptive Runge–Kutta Cash–Karp method.
	// Reference: 1. http://en.wikipedia.org/wiki/Cash%E2%80%93Karp_method
	//            2. runge_kutta_cash_karp54 in boost
	// Writer: Yang Feng. 20141205

	///////////////////////////// constants in  RKCK54  /////////////////////////////////////////////
	const double c2  =  1./5.  , c3  =  3./10., c4  = 3./5. , c5 = 1. , c6 = 7./8.,
		         a21 =  1./5.  ,
		         a31 =  3./40. , a32 =  9./40.,
		         a41 =  3./10. , a42 = -9./10., a43 =  6./5.  ,
		         a51 = -11./54., a52 =  5./2. , a53 = -70./27., a54 = 35./27. ,
		         a61 =  1631./55296., a62 = 175./512., a63 = 575./13824., a64 = 44275./110592., a65 = 253./4096.,
		         b1  =  37./378.    , b3  = 250./621., b4  = 125./594.  , b6  = 512./1771.;
	const double db1 = b1 - 2825./27648., 
		         db3 = b3 - 18575./48384.,
		         db4 = b4 - 13525./55296.,
		         db5 =     -277./14336.,
		         db6 = b6 - 1./4.;
	///////////////////////////// constants in RKCK54  //////////////////////////////////////////////

	using namespace std;

	vector<vector<double> > k;                //  RK method needs k variables  6 steps  but 0 don't needed		
	double t_temp = start_time;               //  record the time of each step
	double h = dt;                            //  step size 
	double abs_error[7];                      //  每一步的误差计算
	vector<double> x_temp; x_temp.resize(7);  //  临时存储x

	//   initial all variables above
	k.resize(7);
	for(int i = 0; i < 7; i++)
	{
		abs_error[i] = 0.;
		k[i].resize(7);     //  7 equations 一个中子通量方程和六个缓发中子先驱核方程
	}

	bool endflag = true;              //  结尾的判断，主要是为了在endtime的时候，步长不要太大积分超出目标区间
	int nstep = 0;                    //  记录step
	const int nMAXstep = 1000000;     //  最大的step，当在积分超过该数字后报错         
	bool succueestep = false;         //  负责检查该步长下计算是否接受，即误差是否满足要求，满足要求则开始下一步积分，否则重新计算本步长

	while(t_temp <= end_time && nstep < nMAXstep && endflag) 
	{
		//  判断使最后一步积分到 end_time
		if( (t_temp + h) > end_time)
		{
			if(nstep == 0) break;
			else
			{
				if(succueestep)
				{
					h = end_time - t_temp;
					endflag = false;
				}
			}
		}// end if

		//  start Normal Runge Kutta Calculation
		f(x,k[1],t_temp);                            //  step one 
		for(int i=0;i<7;i++)
			x_temp[i] = x[i] + h*a21*k[1][i];
		f(x_temp,k[2],t_temp+c2*h);                  //  step two
		for(int i=0;i<7;i++)
			x_temp[i] = x[i] + (h*a31*k[1][i] + h*a32*k[2][i]);
		f(x_temp,k[3],t_temp+c3*h);                  //  step three
		for(int i=0;i<7;i++)
			x_temp[i] = x[i] + (h*a41)*k[1][i] + (h*a42)*k[2][i] + (h*a43)*k[3][i];
		f(x_temp,k[4],t_temp+c4*h);                  //  step four
		for(int i=0;i<7;i++)
			x_temp[i] = x[i] + h*a51*k[1][i] + h*a52*k[2][i] + h*a53*k[3][i] + h*a54*k[4][i];
		f(x_temp,k[5],t_temp+c5*h);                  //  step five
		for(int i=0;i<7;i++)
			x_temp[i] = x[i] + h*a61*k[1][i] + h*a62*k[2][i] + h*a63*k[3][i] + h*a64*k[4][i] + h*a65*k[5][i];
		f(x_temp,k[6],t_temp+c6*h);                  //  step six

		//  Estimate error as difference between fourth and fifth order methods using Cash-Karp format
		for(int i=0;i<7;i++)
		{
			x_temp[i] = x[i] + (h*b1*k[1][i]  + h*b3*k[3][i] + h*b4*k[4][i] + h*b6*k[6][i]);
			abs_error[i] = fabs(h * db1*k[1][i] + h* db3*k[3][i] + h*db4*k[4][i] + h*db5*k[5][i] + h*db6*k[6][i] );
		}

		//  Here we using algorithm from Boost to adaptive step size control for Runge-Kutta
		double v[7];
		double val = 0.;

		for(int i = 0; i < 7; i++)
		{
			//  该公式从Boost中得出
			v[i] = abs_error[i]/(abs_eps + rel_eps*( fabs(x[i] + k[1][i]*h) ));
		}
		//  val = max v[]  ,此处用误差向量的1-范数来衡量误差的大小
		val = v[0];
		for(int i = 1; i < 7; i++){
			if(v[i] > val) val = v[i];
		}

		dt = h;  // 记录本次的步长，用于更新t变量

		if(val > 1.0)     //  本次步长控制失败
		{   
			h = h * max(0.9 * pow(val,-1./3.),0.2);
			endflag = true;
			succueestep = false;
		}
		else             //  本次步长控制成功
		{
			if(val < 0.5)
			{
				val = max(pow(5.,-5.),val);
				h = h * 0.9 * pow(val,-1./5.);
			}
			for(int i = 0; i < 7; i++)
				x[i] = x_temp[i];
			t_temp += dt;
			nstep++;
			succueestep = true;		
		}

	} // end while

	if(nstep >= nMAXstep)
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"too many steps in function Boost_Adaptive_RKCK54.\n");},CDOutput::_WARNING);

	return;
}

void CDPointKinetics::getPara(std::vector<double> beta, std::vector<double> lambda, double rho, double gtime)
{
	m_beta = beta;
	m_lambda = lambda;
	m_rho = rho;
	m_gtime = gtime;
}

void CDPointKinetics::compute(std::vector<double>& x, double timestep)
{
	//create the system
		CDPointReactor f(m_beta, m_lambda, m_rho, m_gtime);

		//initial micro time step
		const double dt=1e-9;
		const double abs_error = 1.0e-10;
		const double rel_error = 1.0e-6;

		//integrate the system
		//20141205 yangfeng
	/*	boost::numeric::odeint::integrate_adaptive( boost::numeric::odeint::make_controlled( 1.0e-10 , 1.0e-6, 
			boost::numeric::odeint::runge_kutta_cash_karp54<std::vector<double > >()  ) ,
			f,x,0.0,timestep,dt);
	*/
		Adaptive_RKCK54(f,x,0.0,timestep,dt,abs_error,rel_error);
}

void CDCriticality::PointKinetics()
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}		
# endif


	if(p_bIsQuasiStaticD)
	{
		CDPointKinetics cPointKinetics;

		vector<double > x(7);
		x[0] = p_dFluxAmptitudePrev;
		for(int i=1;i<=6;i++) x[i] = p_vPrecursorConcentrationPrev[i];

		p_vPKBeta[0] = 0;
		for(int i=1;i<=6;i++) p_vPKBeta[i] = p_cBeta[i].p_dCycAve;
		p_vPKLambda[0] = 0;
		for(int i=1;i<=6;i++) p_vPKLambda[i] = p_cAveDelayedConstant[i].p_dCycAve*1e8;
		p_dRho = p_cRho.p_dCycAve;
		p_dGtime = p_cGenTime.p_dCycAve*1e-8;

		cPointKinetics.getPara(p_vPKBeta,p_vPKLambda,p_dRho,p_dGtime);
		cPointKinetics.compute(x,p_dDeltaTime*1e-8);


		p_dFluxAmptitude = x[0];
		for(int i=1;i<=6;i++) p_vPrecursorConcentration[i]=x[i];
	}
	else
	{
		p_dFluxAmptitude  = 1.0;
		for(int i=1;i<=6;i++)
		{
			p_vPrecursorConcentration[i] = (p_cBeta[i].p_dCycAve)/p_cNeuPopu.p_dCycAve/p_cAveDelayedConstant[i].p_dCycAve;
		}
	}


}
