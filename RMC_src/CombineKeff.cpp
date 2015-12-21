# include "Criticality.h"

void CDCriticality::CombineKeff()
{
	// Reference for covariance-weighted combination of keff & std. £º 
	// max halperin, "almost linearly-optimum combination of unbiased
	// estimates," amer. stat. assn. j. 56, p. 36 - 43 (1961).
	//
	//    mc = number of cycles over which to average.
	//    KeffSum[3] = sum(KeffCyc(i)).
	//    KeffCrosSum[3][3] = sum(KeffCyc(i)*KeffCyc(j)].
	//    KeffIndAve[3], KeffIndStd[3] = averages and std.devs. of (col,abs,tl) individually.
	//    dKeffSimpAve[4], dKeffSimpStd[4] = simple combined averages and std. devs.
	//    KeffCovwAve[4], KeffCovwStd[4] = covariance-weighted combined averages and std. devs.
	//    KeffCorr[3] = Correlations.

	//	Combined sequence: (col/abs), (abs,tl), (tl,col), (col/abs/tl)

	double  dKeffSimpAve[4], dKeffSimpStd[4];       // simple combined averages and std. devs
	double  dKeffCorr[3];                          // Correlation

	if(p_nCurrentCYCLE <= p_nInactCycNum)
	{
		return;
	}

	int i,j,k,nMc;
	double d0,d1,d2,dTemp;
	double  KeffCov[3][3], KeffCovSum;


	nMc = p_nCurrentCYCLE - p_nInactCycNum ;
	for(i=0;i<3;++i)
	{
		p_dKeffSum[i] = p_dKeffSum[i] + p_dKeffCyc[i];
		for(j=0;j<3;++j)
		{
			p_dKeffCrosSum[i][j] = p_dKeffCrosSum[i][j] + p_dKeffCyc[i]*p_dKeffCyc[j];
		}
	}

	for(i=0;i<4;++i)
	{
		dKeffSimpAve[i] = 0 ;
		dKeffSimpStd[i] = 0 ;
		p_dKeffCovwAve[i] = 0 ;
		p_dKeffCovwStd[i] = 0 ;
	}

	/// Calculate the individual average, std. dev., as well as the covariance ////.
	KeffCovSum = 0;
	d0 = nMc ;
	d1 = sqrt(d0/max(1,nMc-1)) ;
	for( i = 0 ; i < 3 ; ++i )
	{
		p_dKeffIndAve[i] = p_dKeffSum[i]/nMc;
		dKeffCorr[i] = 0.;
		for( j = 0 ; j < 3 ; ++j )
		{
			KeffCov[i][j] = p_dKeffCrosSum[i][j] - p_dKeffSum[i]*p_dKeffSum[j]/nMc ;
			KeffCovSum = KeffCovSum + KeffCov[i][j] ;
		}
		p_dKeffIndStd[i] = d1*sqrt(fabs(KeffCov[i][i]))/nMc ;
	}

	/// calculate simple/covariance-weighted combined averages and std. devs.  ///
	/// Combined sequence 0 - 3 : (col/abs), (abs,tl), (tl,col), (col/abs/tl)  ///
	if( nMc <= 2 ) { return; }
	d2 = sqrt(d0/(d0-2.)) ;
	for ( i = 0 ; i < 3 ; ++i )
	{
		j = (i+1)%3 ;
		dKeffSimpAve[i] = 0.5*( p_dKeffIndAve[i] + p_dKeffIndAve[j] ) ;
		dKeffSimpStd[i] = 0.5*d1*sqrt(fabs(KeffCov[i][i] + KeffCov[j][j] + 2*KeffCov[i][j]))/nMc ;
		p_dKeffCovwAve[i] = p_dKeffIndAve[i] ;
		p_dKeffCovwStd[i] = p_dKeffIndStd[i] ;
		dTemp = KeffCov[i][i] + KeffCov[j][j] - 2*KeffCov[i][j] ;
		if( dTemp > 1.e-10*(fabs(KeffCov[i][i]) + fabs(KeffCov[i][j]) + fabs(KeffCov[j][j])) )
		{
			p_dKeffCovwAve[i] = p_dKeffIndAve[i]-(p_dKeffIndAve[i]-p_dKeffIndAve[j])*(KeffCov[i][i] - KeffCov[j][j])/dTemp ;
			p_dKeffCovwStd[i] = d2*sqrt(fabs((KeffCov[i][i]*KeffCov[j][j] - KeffCov[i][j]*KeffCov[i][j])*
				(dTemp+d0*(p_dKeffIndAve[i]-p_dKeffIndAve[j])*(p_dKeffIndAve[i]-p_dKeffIndAve[j]))/(dTemp*dTemp)))/nMc ;
		}
		if( KeffCov[i][i]*KeffCov[j][j] > 0 ) 
		{
			dKeffCorr[i] = KeffCov[i][j]/sqrt(fabs(KeffCov[i][i]*KeffCov[j][j])) ;
		}
	}
	dKeffSimpAve[3] = (p_dKeffIndAve[0] + p_dKeffIndAve[1] + p_dKeffIndAve[2])/3 ;
	dKeffSimpStd[3] = d1*sqrt(fabs(KeffCovSum))/(3*nMc) ;

	/// calculate the combined average (col/abs/tl), zc(4). //
	if( nMc==3 ) { return ;}
	double a = 0 , al ;
	double s1 = 0 ;
	double s2 = 0 ;
	double s3 = 0 ;
	k = 2 ;
	if( fabs(dKeffCorr[0]-1.0) < 1.0E-5 ){goto go60;}
	for (i = 0 ; i < 3 ; ++i )
	{
		j = (i+1)%3 ;
		k = (j+1)%3 ;
		al = KeffCov[j][j]*KeffCov[k][k] - KeffCov[j][j]*KeffCov[i][k] - KeffCov[k][k]*KeffCov[i][j] + 
			KeffCov[j][k]*(KeffCov[i][j] + KeffCov[i][k] - KeffCov[j][k]) ;
		a = a+al ;
		p_dKeffCovwAve[3] = p_dKeffCovwAve[3] + al*p_dKeffIndAve[i] ;
		s1 = s1 + al*KeffCov[i][0] ;
		s2 = s2+(KeffCov[j][j] + KeffCov[k][k] - 2*KeffCov[j][k])*(p_dKeffIndAve[i]*p_dKeffIndAve[i]) ;
		s3 = s3+(KeffCov[k][k] + KeffCov[i][j] - KeffCov[j][k] - KeffCov[i][k])*p_dKeffIndAve[i]*p_dKeffIndAve[j] ;
	}
	if( a==0 || p_dKeffCovwAve[3] == 0. ) { goto go60 ;}
	p_dKeffCovwAve[3] = p_dKeffCovwAve[3]/a ;
	p_dKeffCovwStd[3] = sqrt(fabs(s1*(1.0 + d0*(s2-s3-s3)/a)/(a*d0*(d0-3.0)))) ;
	p_dKeffFinal = p_dKeffCovwAve[3];
	return ;

go60:
	p_dKeffCovwAve[3] =  p_dKeffCovwAve[k] ;
	p_dKeffCovwStd[3] =  p_dKeffCovwStd[k] ;
	p_dKeffFinal = p_dKeffCovwAve[3];
	return ;
}