#if ! defined __POINT__KINETICS__H__
#define __POINT__KINETICS__H__

#include <vector>
#include <numeric>
//#include <boost/numeric/odeint.hpp>
// use runge_kutta_cash_karp54 algorithm from boost to solve point reactor equations 
// replaced by member function Adaptive_RKCK54 written by yangfeng, 20141205

class CDPointReactor
{
private:
	std::vector<double> m_beta;
	std::vector<double> m_lambda;
	double m_beta_tot;
	double m_rho;
	double m_gtime;

public:
	CDPointReactor(std::vector<double> beta, std::vector<double> lambda, double rho, double gtime)
	{
		m_beta = beta;
		m_lambda = lambda;
		m_rho = rho;
		m_gtime = gtime;
		m_beta_tot = std::accumulate(m_beta.begin(),m_beta.end(),0.);
	}

	void operator()(const std::vector<double>& x, std::vector<double>& dxdt, const double t)
	{
		dxdt[0] = (m_rho-m_beta_tot)/m_gtime*x[0] + m_lambda[1]*x[1] + m_lambda[2]*x[2] +
			m_lambda[3]*x[3] + m_lambda[4]*x[4] + m_lambda[5]*x[5] + m_lambda[6]*x[6];

		for(int i = 1; i <= 6; i++)
		{
			dxdt[i] = m_beta[i]/m_gtime*x[0] - m_lambda[i]*x[i];
		}

	}
};


class CDPointKinetics
{
private:
	std::vector<double> m_beta;
	std::vector<double> m_lambda;
	double m_rho;
	double m_gtime;  //unit:sec

public:
	void getPara(std::vector<double> beta, std::vector<double> lambda, double rho, double gtime);
	void compute(std::vector<double>& x, double timestep);
	void Adaptive_RKCK54(CDPointReactor f,std::vector<double>& x,double start_time,double end_time,double dt,double abs_eps,double rel_eps);
};


#endif //// __POINT__KINETICS__H__