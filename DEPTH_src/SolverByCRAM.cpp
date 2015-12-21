# include "Depth_Class.h"
# include "ME_Class.h"

void ME_Class::SolverByCRAM()
{
//// old 14 order ////
//	const complex<double> Theta[7]={
//complex<double>(+5.62314417475317895E+00, -1.19406921611247440E+00),
//complex<double>(+5.08934679728216110E+00, -3.58882439228376881E+00),
//complex<double>(+3.99337136365302569E+00, -6.00483209099604664E+00),
//complex<double>(+2.26978543095856366E+00, -8.46173881758693369E+00),
//complex<double>(-2.08756929753827868E-01, -1.09912615662209418E+01),
//complex<double>(-3.70327340957595652E+00, -1.36563731924991884E+01),
//complex<double>(-8.89777151877331107E+00, -1.66309842834712071E+01)};
//	const complex<double> Alpha[7]={
//complex<double>(-2.787519865682509130E+01, +1.021475193898859285E+02),
//complex<double>(+4.693334194385033695E+01, -4.564374483877281815E+01),
//complex<double>(-2.349827077751854175E+01, +5.808380499290905150E+00),
//complex<double>(+4.807121003130305325E+00, +1.320978069401313345E+00),
//complex<double>(-3.763610319891608210E-01, -3.351836827831888850E-01),
//complex<double>(+9.439062657932428800E-03, +1.718480882229012070E-02),
//complex<double>(-7.154321570590092450E-05, -1.436105666144070480E-04)};
//	const double Alpha0=+1.83216998528140087E-12;

//// new 14 order ////
//	const complex<double> Theta[7]={
//complex<double>(-8.8977731864688888199E+00, +1.6630982619902085304E+01),
//complex<double>(-3.7032750494234480603E+00, +1.3656371871483268171E+01),
//complex<double>(-0.2087586382501301251E+00, +1.0991260561901260913E+01),
//complex<double>(+3.9933697105785685194E+00, +6.0048316422350373178E+00),
//complex<double>(+5.0893450605806245066E+00, +3.5888240290270065102E+00),
//complex<double>(+5.6231425727459771248E+00, +1.1940690463439669766E+00),
//complex<double>(+2.2697838292311127097E+00, +8.4617379730402214019E+00)};
//	const complex<double> Alpha[7]={
//complex<double>(-7.1542880635890672853E-05, +1.4361043349541300111E-04),
//complex<double>(+9.4390253107361688779E-03, -1.7184791958483017511E-02),
//complex<double>(-3.7636003878226968717E-01, +3.3518347029450104214E-01),
//complex<double>(-2.3498232091082701191E+01, -5.8083591297142074004E+00),
//complex<double>(+4.6933274488831293047E+01, +4.5643649768827760791E+01),
//complex<double>(-2.7875161940145646468E+01, -1.0214733999056451434E+02),
//complex<double>(+4.8071120988325088907E+00, -1.3209793837428723881E+00)};
//	const double Alpha0=+1.8321743782540412751E-14;

//// new 16 order ////
	const complex<double> Theta[8]={
		complex<double>(-1.0843917078696988026E+01, +1.9277446167181652284E+01),
		complex<double>(-5.2649713434426468895E+00, +1.6220221473167927305E+01),
		complex<double>(+5.9481522689511774808E+00, +3.5874573620183222829E+00),
		complex<double>(+3.5091036084149180974E+00, +8.4361989858843750826E+00),
		complex<double>(+6.4161776990994341923E+00, +1.1941223933701386874E+00),
		complex<double>(+1.4193758971856659786E+00, +1.0925363484496722585E+01),
		complex<double>(+4.9931747377179963991E+00, +5.9968817136039422260E+00),
		complex<double>(-1.4139284624888862114E+00, +1.3497725698892745389E+01)};
	const complex<double> Alpha[8]={
		complex<double>(-5.0901521865224915650E-07, -2.4220017652852287970E-05),
		complex<double>(+2.1151742182466030907E-04, +4.3892969647380673918E-03),
		complex<double>(+1.1339775178483930527E+02, +1.0194721704215856450E+02),
		complex<double>(+1.5059585270023467528E+01, -5.7514052776421819979E+00),
		complex<double>(-6.4500878025539646595E+01, -2.2459440762652096056E+02),
		complex<double>(-1.4793007113557999718E+00, +1.7686588323782937906E+00),
		complex<double>(-6.2518392463207918892E+01, -1.1190391094283228480E+01),
		complex<double>(+4.1023136835410021273E-02, -1.5743466173455468191E-01)};
	const double Alpha0=2.1248537104952237488E-16;

	int NucNum = Depth.NucNum;
	static vector<complex<double> > MatrixA;
	static vector<int> IRC, ICFR, LUP;
	static vector<complex<double> > VectorB(NucNum+1);


	if(Step>1)//matrix has same sparse form --> use previous Symbol-Gauss-Elimination results
	{
		if(Depth.TranMatrixChanged == false) 
		{
			goto CRAM;
		}
	}

	GaussElimiSymbol( IRC, ICFR, LUP);

	MatrixA.resize(IRC.size());

CRAM:
	for(int m=0;m<8;++m)
	{
		for(int i=1;i<=NucNum;++i)
		{
			VectorB[i]=complex<double>(Depth.StepNt[Step-1][i],0);

			//////  Generate diagonal element of Matrix Theta*I+A*t
			MatrixA[i]=complex<double>(TranMatrixMCS._DiagVal[i]*Time,0)-Theta[m];

			//////  Generate lower triangle of Matrix Theta*I+A*t
			int RowIB=ICFR[i];
			int RowIC=ICFR[i+1]-1;
			int size1=TranMatrixMCS._LowerPtr[i].size();
			for(int j=RowIB;j<RowIB+size1;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(TranMatrixMCS._LowerVal[i][j-RowIB]*Time,0);
			}
			for(int j=RowIB+size1;j<=RowIC;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(0,0);
			}

			//////  Generate upper triangle of Matrix Theta*I+A*t
			int ColIB=ICFR[NucNum+i];
			int ColIC=ICFR[NucNum+i+1]-1;
			int size2 = TranMatrixMCS._UpperPtr[i].size();
			for(int j=ColIB;j<ColIB+size2;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(TranMatrixMCS._UpperVal[i][j-ColIB]*Time,0);
			}
			for(int j=ColIB+size2;j<=ColIC;++j)  // 0 -> size
			{
				MatrixA[j]=complex<double>(0,0);
			}
		}

		GaussElimi(MatrixA,  IRC, ICFR, LUP, VectorB);


		for(int i=1;i<=NucNum;i++)
		{
			if(VectorB[i]==0.0)
			{
				continue;
			}
			Depth.StepNt[Step][i] = Depth.StepNt[Step][i]+(Alpha[m].real()*VectorB[i].real()-Alpha[m].imag()*VectorB[i].imag());  //Nt = Nt -(Alphar(m) + Alphai(m)*i)*(((Thetar(m) + Thetai(m)*1i)*I + A*t)\N0)
		}
	}   // end if for(m=0;m<8;++m)

	for(int i=1;i<=NucNum;i++)
	{
		Depth.StepNt[Step][i] = Alpha0*Depth.StepNt[Step-1][i]+2*Depth.StepNt[Step][i]; 
	}

	return;
}