# include "AceData.h"

int CDAceData::DpplrBrdnNucTmp(int nNuc, double nTmp)
{
	//// adjust elastic and total cross-section
	if( fabs(p_vNuclides[nNuc].p_dNucTmp-nTmp) <= 0.01*nTmp ) {return 0;}  // no adjustment
	double b = 500.*fabs(nTmp-p_vNuclides[nNuc].p_dNucTmp)/p_vNuclides[nNuc].p_dNucAtomWgt;
	for(int j = 1;j <= GetErgGridNum(nNuc);++j)
	{
		if( p_vNuclides[nNuc].XSS[j]>b ) {break;}
		///////////////calculate f1 //////////////
		double df1 = 1.0;
		if( p_vNuclides[nNuc].p_dNucTmp!=0. ) 
		{
			double a = sqrt(p_vNuclides[nNuc].p_dNucAtomWgt*p_vNuclides[nNuc].XSS[j]/p_vNuclides[nNuc].p_dNucTmp);
			if( a>=2.0) {df1=1.0+0.5/(a*a);}
			int i = int(a/0.04);
			if( a<2.0) {df1=(p_dThermFunc[i]+(a/0.04-i)*(p_dThermFunc[i+1]-p_dThermFunc[i]))/a;}
		}
		///////////////calculate f2 //////////////
		double df2 = 1.0;
		if( nTmp!=0. ) 
		{
			double a = sqrt(p_vNuclides[nNuc].p_dNucAtomWgt*p_vNuclides[nNuc].XSS[j]/nTmp);
			if( a>=2.0){df2=1.0+0.5/(a*a);}
			int i = int(a/.04);
			if( a<2.0){df2=(p_dThermFunc[i]+(a/0.04-i)*(p_dThermFunc[i+1]-p_dThermFunc[i]))/a;}
		}
		///////////////calculate a //////////////
		double a = p_vNuclides[nNuc].XSS[j+3*GetErgGridNum(nNuc)]*(df2-df1)/df1;

		///////////////calculate xs //////////////
		p_vNuclides[nNuc].XSS[j+GetErgGridNum(nNuc)]= p_vNuclides[nNuc].XSS[j+GetErgGridNum(nNuc)]+a;
		p_vNuclides[nNuc].XSS[j+3*GetErgGridNum(nNuc)]=p_vNuclides[nNuc].XSS[j+3*GetErgGridNum(nNuc)]+a;
	} 
	p_vNuclides[nNuc].p_dNucBroadTmp = nTmp;
	
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%10s temperature = %5.4E adjusted to %5.4E\n",p_vNuclides[nNuc].p_ONucID.p_chIdStr,p_vNuclides[nNuc].p_dNucTmp,p_vNuclides[nNuc].p_dNucBroadTmp);},
		Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	return 1;

}
void CDAceData::CalcThermGfun()
{
	//calculate the thermal g-function table.
	double b = 1.0/sqrt(3.1415926);
	p_dThermFunc[0] = 2.0*b;
	for(int i = 1; i<=50; i++)
	{
		double a = i*0.04;
		p_dThermFunc[i] = (a+0.5/a)*Erf2Function(a)+b*exp(-a*a);
	}
}
double CDAceData::Erf2Function(double a)
{
	// return the value of the error function of a.
	const double WF = 1.1283791670955;

	double ds1[13] = {0.000,        -.0490461212346920,   -.142261205103710,    .0100355821876000,  
		-.000576876469976750, .000027419931252196, -.000001104317550734, .000000038488755420,
		-.000000001180858253, .000000000032334215, -.000000000000799101, .000000000000017990,
		-.000000000000000371 };
	double ds2[22] = { 0.000     ,  -.0696013466023100,    -.0411013393626210,   .00391449586668960, 
		-.000490639565054900, .000071574790013770,  -.000011530716341312, .000001994670590201,
		-.000000364266647159, .000000069443726100,  -.000000013712209021, .000000002788389661,
		-.000000000581416472, .000000000123892049,  -.000000000026906391, .000000000005942614,
		-.000000000001332386, .000000000000302804,  -.000000000000069666, .000000000000016208,
		-.000000000000003809, .000000000000000904 };
	double ds3[23] = { 0.000   ,   .0715179310202930,      -.0265324343376070,   .00171115397792090, 
		-.000163751663458510, .000019871293500549,  -.000002843712412769, .000000460616130901,
		-.000000082277530261, .000000015921418724,  -.000000003295071356, .000000000722343973,
		-.000000000166485584, .000000000040103931,  -.000000000010048164, .000000000002608272,
		-.000000000000699105, .000000000000192946,  -.000000000000054704, .000000000000015901,
		-.000000000000004729, .000000000000001432,  -.000000000000000439 };

	double dErf2 = 0;
	if( fabs(a)<=8.429370e-8 ) 
	{
		dErf2 = a*WF;
		return dErf2;
	}
	double b =a*a;
	double b0 = 0;
	double b1 = 0;
	double b2 = 0;
	double c = 0;
	if( b <= 1.0) 
	{
		for(int i = 1;i<=12;i++)
		{
			b2 = b1;
			b1 = b0;
			b0 = b1*(4.0*b-2.0)-b2+ds1[13-i];
		}
		dErf2 = a*(1.0+0.5*(b0-b2));
	}
	else
	{
		c = 1.0;
		dErf2 = 1.0;
		if(a < 0){dErf2 = -1.0;}
		if( b <= 4.0) 
		{
			c = (16.0/b - 10.0)/3.0;
			for(int i = 1;i <= 21;i++)
			{
				b2 = b1;
				b1 = b0;
				b0 = b1*c-b2+ds2[22-i];
			}
		}
		else
		{
			if( b > 32.6987 )  {return dErf2;}
			c = 16.0/b-2.0;
			for(int i = 1;i<=22;i++)
			{
				b2 = b1;
				b1 = b0;
				b0 = b1*c - b2 + ds3[23-i];
			}
		}
		dErf2 = dErf2 - exp(-b)*(1.0 + b0 - b2)/(2.0*a);
	}
	return dErf2;
}