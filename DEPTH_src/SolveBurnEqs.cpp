# include "Depth_Class.h"
# include "TTA_Class.h"
# include "ME_Class.h"

////////////  Solve by TTA or Matrix Exponential   //////////////
// 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre

void Depth_Class::SolveBurnEqs()
{
	time_t SolveTick1, SolveTick2;
	SolveTick1 = clock();      //get clock time at beginning(ms)

	///////// TTAl method /////////////
	if(SolveMethod == 1)
	{
		GenTranMatrix(2);

		TTA.Time = TIME;
		TTA.Step = STEP;

		TTA.SolveByTTA();
	}

	///////// Matrix Exponential methods /////////////
	else if(SolveMethod >=2 )   
	{
		GenTranMatrix(3);

		//ME.CalcMatrixNorm();
	
		ME.Step = STEP;
		ME.Time = TIME;

		if(SolveMethod==2)
		{
			ME.SolverByCRAM();
		}
		else if(SolveMethod==3)
		{
			ME.SolverByQuad();
		}
		else if(SolveMethod==4)
		{
			ME.SolverByLaguerre();
		}
	}

	//// Cut off to avoid numerical instability /////
	for(int i = 1;i <= NucNum;i++)
	{
		if(StepNt[STEP][i] < 1.0E-50)
		{
			StepNt[STEP][i]=0;
		}  
	}

	SolveTick2 = clock();    //get clock time at ending(ms)
	SolveTick = SolveTick + SolveTick2 - SolveTick1;
}