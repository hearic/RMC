# include "Depth_Class.h"
# include "ME_Class.h"

void Depth_Class::CalQuantities(int step)
{
	enum QuantityType {instant = 1 , integral = 2};
    static SparseMatrixRCS LowMatrix;
	static SparseMatrixRCS UpperMatrix;
	static vector<double> MeanNt(NucNum+1);
	//// note: MeanNt is the instantaneous concentration the at burnup step end, 
	////       or the integral value over the time interval.

	//////// Calculate the beginning quantities //////////
	if(step == 1 )
	{
		CalQuantities(0);
	}

	//////// Calculate the instantaneous/integral concentration of the burnup step //////////
	if(step > 0 && (RadioactivityFlag == integral || AbsRateFlag == integral || FissionRateFlag == integral || DecayHeatFlag == integral))
	{
		for(int i = 1; i <= NucNum;i++)
		{
			MeanNt[i] = StepNt[step][i]-StepNt[step-1][i];
		}

		//LowMatrix.Clear();
		//UpperMatrix.Clear();
		LowMatrix.Reset(NucNum+1);
		UpperMatrix.Reset(NucNum+1);

		GenTranMatrix(3);
		ME.DecmpMatrixByLU(ME.TranMatrixMCS, LowMatrix, UpperMatrix);
		ME.MultiplyVecByLU(LowMatrix,UpperMatrix,MeanNt);

		for(int i = 1; i <= NucNum;i++)
		{
			if (MeanNt[i] < 1.0E-50 ) { MeanNt[i] = 0; }
		}
	}
	else
	{
		MeanNt = StepNt[step];
	}


	/////////  Calculate  Quantities  /////////
	if(RadioactivityFlag > 0)
	{
		for(int i = 1;i <= NucNum; ++i)
		{
			Radioactivity[step][i] = GetLibLamd(i)*MeanNt[i]*1.0E+24/(3.7*1.0E+10);
		}
	}

	if(AbsRateFlag > 0)
	{
		for(int i = 1;i <= NucNum; ++i)
		{
			AbsorpRate[step][i] = GetLibAbsXs(i) * MeanNt[i] * StepFlux[step] ;
		}
	}

	if(FissionRateFlag > 0)
	{
		for(int i = 1;i <= NucNum; ++i)
		{
			FissionRate[step][i] = GetLibFisXs(i) * MeanNt[i] * StepFlux[step];
		}
	}

	if(DecayHeatFlag > 0)
	{
		for(int i = 1;i <= NucNum; ++i)
		{
			DecayHeat[step][i] = GetLibLamd(i) * MeanNt[i] * 1.0E+24 * GetLibDecQ(i) * 1.0E+06 * 1.60217653E-19;
		}
	}


	return;
}