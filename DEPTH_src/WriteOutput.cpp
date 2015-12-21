# include "Depth_Class.h"

void Depth_Class::WriteOutput()
{
	if(StepNum<1){return;}

	DepthOut.open(OutputFile,ios::out);

	DepthOut<<"///////////////Nuclide Density Table///////////////"<<'\n';
	OutputTable(StepNt);

	if(RadioactivityFlag>0)
	{
		if(RadioactivityFlag==1)
		{
		    DepthOut<<"///////////////Instantaneous Radioactivity(Curies) Table///////////////"<<'\n';
		}
		else if(RadioactivityFlag==2)
		{
			DepthOut<<"///////////////Integral Radioactivity(Curies)Table///////////////"<<'\n';
		}
		OutputTable(Radioactivity);
	}
	if(AbsRateFlag>0)
	{
		if(AbsRateFlag==1)
		{
			DepthOut<<"///////////////Instantaneous Absorption Rate(Neutrons/Sec) Table///////////////"<<'\n';
		}
		else if(AbsRateFlag==2)
		{
			DepthOut<<"///////////////Integral Absorption Rate(Neutrons/Sec) Table///////////////"<<'\n';
		}
		OutputTable(AbsorpRate);
	}
	if(FissionRateFlag>0)
	{
		if(FissionRateFlag==1)
		{
			DepthOut<<"///////////////Instantaneous Fission Rate(Fissions/Sec) Table///////////////"<<'\n';
		}
		else if(FissionRateFlag==2)
		{
			DepthOut<<"///////////////Integral Fission Rate(Fissions/Sec) Table///////////////"<<'\n';
		}
		OutputTable(FissionRate);
	}
	if(DecayHeatFlag>0)
	{
		if(DecayHeatFlag==1)
		{
			DepthOut<<"///////////////Instantaneous Decay Heat(Watts) Table///////////////"<<'\n';
		}
		else if(DecayHeatFlag==2)
		{
			DepthOut<<"///////////////Integral Decay Heat(Watts) Table///////////////"<<'\n';
		}
		OutputTable(DecayHeat);
	}

	DepthOut.close();


}

void Depth_Class::OutputTable(vector<vector<double> > &Table)
{
	DepthOut.precision(4);
	DepthOut.setf(DepthOut.uppercase|DepthOut.scientific);
	DepthOut.setf(DepthOut.left);

	if(PrintFlag==1){goto PrintAllSteps;}
	////////////////////Print End step////////////
	//// step, time, cumulative time, flux, power
	DepthOut<<setw(16)<<""<<"STEP = "<<setw(11)<<0<<"STEP = "<<setw(11)<<StepNum<<'\n'; //step
	DepthOut<<setw(16)<<""<<"T  = "<<setw(10)<<StepTime[0]<<"   "<<"T  = "<<setw(10)<<StepTime[StepNum]<<"   "<<'\n'; //t
	DepthOut<<setw(16)<<""<<"CT = "<<setw(10)<<CmltvTime[0]<<"   "<<"CT = "<<setw(10)<<CmltvTime[StepNum]<<"   "<<'\n'; //t
	DepthOut<<setw(16)<<""<<"F  = "<<setw(10)<<StepFlux[0]<<"   "<<"F  = "<<setw(10)<<StepFlux[StepNum]<<"   "<<'\n'; //f
	DepthOut<<setw(16)<<""<<"P  = "<<setw(10)<<StepPower[0]<<"   "<<"P  = "<<setw(10)<<StepPower[StepNum]<<"   "<<'\n'; //p
	
	DepthOut<<"NucID  NucName"<<'\n';
	DepthOut.precision(9);//
	for(int i = 1;i <= NucNum;++i)
	{
		int NucId = GetNucId(i);
		DepthOut<<setw(8)<<NucId<<setw(8)<<GetNucName(NucId).c_str();
		DepthOut<<setw(18)<<Table[0][i]<<setw(18)<<Table[StepNum][i]<<'\n';
	}
	return;


	///////////////////Print All Steps///////////////////
PrintAllSteps:

	DepthOut<<setw(16)<<"";
	for(int i=0;i<=StepNum;++i)
	{
		DepthOut<<"STEP = "<<setw(11)<<i;  //Step
	}
	DepthOut<<'\n';

	DepthOut<<setw(16)<<"";
	for(int i=0;i<=StepNum;++i)
	{
		DepthOut<<"T  = "<<setw(10)<<StepTime[i]<<"   ";   // time	
	}
	DepthOut<<'\n';

	DepthOut<<setw(16)<<"";
	for(int i=0;i<=StepNum;++i)
	{
		DepthOut<<"CT = "<<setw(10)<<CmltvTime[i]<<"   ";   // cumulative time	
	}
	DepthOut<<'\n';

	DepthOut<<setw(16)<<"";
	for(int i=0;i<=StepNum;++i)
	{
		DepthOut<<"F  = "<<setw(10)<<StepFlux[i]<<"   ";   // Flux;
	}
	DepthOut<<'\n';

	DepthOut<<setw(16)<<"";
	for(int i=0;i<=StepNum;++i)
	{
		DepthOut<<"P  = "<<setw(10)<<StepPower[i]<<"   ";// Power
	}
	DepthOut<<'\n';


	DepthOut<<"NucID  NucName"<<'\n';

	DepthOut.precision(9);// 6
	for(int i=1;i<=NucNum;++i)
	{
		int NucId = GetNucId(i);
		DepthOut<<setw(8)<<NucId<<setw(8)<<GetNucName(NucId).c_str();
		for(int j=0;j<=StepNum;++j)
		{
			DepthOut<<setw(18)<<Table[j][i];
		}
		DepthOut<<'\n';
	}

}