# include "Depth_Class.h"
# include "ME_Class.h"

////// XSigR=X*Sig*R, which is used to calculate FLUX or POWER ////
// X   =  amount of fissile nuclide i in fuel (g atom);
// Sig =  microscopic fission cross section for nuclide i(barn);
// R = recoverable energy per fission for nuclide i (MeV/fission)
void Depth_Class::CalMeanFluxPower()
{ 
	double CalTemp;

	static SparseMatrixRCS LowMatrix;
	static SparseMatrixRCS UpperMatrix;
	static vector<double> MeanNt(NucNum+1);

	if(MODE == 0)
	{
		return;
	}

	if(MODE == 1)
	{
		if(MeanPowerFlag == 0){return;}

		//////// Calculate the average nuclide density among the burnup step//////////
		for(int i = 1; i <= NucNum;i++)
		{
			MeanNt[i]=(StepNt[STEP][i]-StepNt[STEP-1][i])/TIME;
		}

		//LowMatrix.Clear();
		//UpperMatrix.Clear();
		LowMatrix.Reset(NucNum+1);
		UpperMatrix.Reset(NucNum+1);

		if(SolveMethod == 1)
		{
			GenTranMatrix(3);
		}
		ME.DecmpMatrixByLU(ME.TranMatrixMCS, LowMatrix, UpperMatrix);
		ME.MultiplyVecByLU(LowMatrix,UpperMatrix,MeanNt);

		//////// Calculate the average fission recoverable energy among the burnup step//////////
		double PowerFactor = CalcPowerFactor(MeanNt);

		//// Constant Flux mode /////
		MeanPOWER = StepFlux[STEP]*PowerFactor;
		StepPower[STEP] = MeanPOWER;

	}
	//// Constant Power mode /////
	else if(MODE == 2)
	{
		if(MeanFluxFlag == 0){return;}
		//////// Calculate the average nuclide density among the burnup step//////////
		for(int i = 1;i <= NucNum; i++)
		{
			MeanNt[i]=(StepNt[STEP][i]-StepNt[STEP-1][i])/TIME;
		}

		//LowMatrix.Clear();
		//UpperMatrix.Clear();
		LowMatrix.Reset(NucNum+1);
		UpperMatrix.Reset(NucNum+1);

		if(SolveMethod == 1)
		{
			GenTranMatrix(3);
		}
		ME.DecmpMatrixByLU(ME.TranMatrixMCS, LowMatrix, UpperMatrix);
		ME.MultiplyVecByLU(LowMatrix,UpperMatrix,MeanNt);

		////// Calculate the average fission recoverable energy among the burnup step//////////
		double PowerFactor = CalcPowerFactor(MeanNt);
		if (PowerFactor == 0)
		{
			MeanFlux = 0;// no heat release
		}
		else
		{
			MeanFlux = StepPower[STEP]/PowerFactor;
		}

		///// Correct the flux slope/////
		CalTemp = fabs(MeanFlux-FLUX)/FLUX;
		static int count=0;
		count++;
		if(CalTemp < MeanConvergeError)
		{
			MeanFluxFlag = 1;  // converged
			StepFlux[STEP] = MeanFlux;
		}
		else
		{
			MeanFluxFlag = 2;
			FLUX = MeanFlux;
		}
	}
	return;
}