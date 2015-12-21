# include "ME_Class.h"

const double PI=3.1415926535898;

void ME_Class::CalcQuadCoeffs(vector<complex<double> > &Quad_z,vector<complex<double> > &Quad_c)
{
	int i;
	vector<double> Quad_theta;
	vector<complex<double> > Quad_w;

	Quad_theta.resize(Quad_N/2+1);
	Quad_z.resize(Quad_N/2+1);
	Quad_w.resize(Quad_N/2+1);
	Quad_c.resize(Quad_N/2+1);


	for(i=1;i<=Quad_N/2;++i)
	{
		Quad_theta[i]=PI*(2*i-1)/Quad_N;
		Quad_z[i]=complex<double>(Quad_N*(0.1309-0.1194*Quad_theta[i]*Quad_theta[i]),Quad_N*0.2500*Quad_theta[i]);
		Quad_w[i]=complex<double>(Quad_N*(-0.1194*2*Quad_theta[i]),Quad_N*0.2500);
		Quad_c[i]=complex<double>(0,1.0/Quad_N)*exp(Quad_z[i])*Quad_w[i];
	}

	return;
}