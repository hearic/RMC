# include "Depth_Class.h"
# include "TTA_Class.h"

void CallDepth()
{

	////////////////////////////////////
	//Depth.StartTick=clock();    //get system time at beginning(ms)

	////////////////////////////////// input and initiate //////////////////////////////////
	//Depth.SetDefault();
	////Depth.Input();

	/////////////////   set parameters from outside  //////////////
	//Depth.StepNum = 1;
	//Depth.StepPower.resize(2);
	//Depth.StepFlux.resize(2);
	//Depth.StepMode.resize(2);
	//Depth.StepTime.resize(2);
	//Depth.StepPower[1] = 1.00E-02;
	//Depth.StepMode[1] = 2;
	//Depth.StepTime[1] = 100*3600*24;
	//
	//int Index = Depth.NucIDIndex[80160];
	//Depth.N0[Index] = 2;
	//Index = Depth.NucIDIndex[922350];
	//Depth.N0[Index] = 0.04;
	//Index = Depth.NucIDIndex[922380];
	//Depth.N0[Index] = 0.36;
	//
	/////////////////////////////////////////

	//Depth.InitiateCalc();

	////////////////////////////////// Process Data Library ///////////////////////////////////
	//if(Depth.ConvertLibFlag==1){Depth.ConvertLib();}
	//Depth.ReadLib();
	//if(Depth.ReadSubLibFlag==1){Depth.ReadSubLib();}

	////////////////////////////////// execute //////////////////////////////////
	//Depth.Execute();


	//Depth.N0 = Depth.StepNt[Depth.StepNum];

	//Depth.InitiateCalc();


	//Depth.Execute();

	////////////////////////////////// output //////////////////////////////////
	//Depth.Output();

	////////////////////////////////// finish //////////////////////////////////
	//Depth.Finish();

}

