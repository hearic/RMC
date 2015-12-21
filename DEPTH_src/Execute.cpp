# include "Depth_Class.h"
# include "TTA_Class.h"
# include "ME_Class.h"

void Depth_Class::Execute()
{ 
	SolveTick = 0;

	//CheckInput();


	if(StepNum < 1)
	{
		return;
	}

	////////////////////////////// Calculate by burnup step //////////////////////////////////
	for(int step_i = 1; step_i <= StepNum; ++step_i)
	{
		STEP = step_i;
		MODE = StepMode[step_i];
		TIME = StepTime[step_i];

		///////////////// Calculate FLUX and POWER //////////////////////
		CalStartFluxPower();

		///////////////// Construct DecMatrix and NeuMatrix ////////////
		if(STEP == 1) ///////// Generate Decay Matrix 
		{
			TransitDec();
		}
		if(MODE > 0)  ///////// Generate Neutron Matrix 
		{
			AdjustYield();  // Yield adjustment by nearest neighbor
			InterpolateYield();
			TransitNeu();
		}

		////////////  Calculate MatrixChangeFlag to judge     ///////////
		//////////// whether the matrix need be re-generated  ///////////
		if(STEP > 1)
		{
			if(StepFlux[STEP-1] == 0 && FLUX == 0)  // Only for decay mode
			{
				TranMatrixChanged = false;
			}
			else
			{
				TranMatrixChanged = true;
			}
		}

solver:
		////////////  Solve by TTA or Matrix Exponential   //////////////
		SolveBurnEqs();

		//////// Calculate Average FLUX and POWER //////////
		CalMeanFluxPower();
		if(MeanFluxFlag==2)
		{
			for(int i = 1; i <= NucNum; ++i)
			{
				StepNt[STEP][i]=0;
			}

			goto solver;
		}

		///////// Calculate Quantities ////////////
		CalQuantities(STEP);

	}

	//////////////////////////  Output Computation Time  /////////////////////////
	//if(SolveMethod==1){cout<<"TTA Time:";}
	//else if(SolveMethod==2){cout<<"CRAM Time:";}
	//else if(SolveMethod==3){cout<<"Quadrature Time:";}
	//else if(SolveMethod==4){cout<<"Laguerre Time:";}
	//cout<<double(SolveTick)/CLOCKS_PER_SEC<<"s\n";

	return;
}