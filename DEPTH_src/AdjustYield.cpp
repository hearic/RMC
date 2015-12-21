# include "Depth_Class.h"

void Depth_Class::AdjustYield()
{
	YieldFactor = 1.0;
	NearestNeighbor = 0;

	if(AdjustFisYield == false){return;}

	if(FLUX == 0){return;}

	int MaxFisNucID = 92235;
	double FisRateSum = 0, MaxFisRate = 0;

	for(int i = ActinidePos;i <= NucNum;++i)
	{
		int FisNucID = GetNucId(i) ;
		bool HasYield = false;
		for(int j = 0 ; j < YieldData.size(); ++j)
		{
			if(FisNucID == YieldData[j].FisNucID)
			{
				HasYield = true;
				break;
			}
		}
		if(HasYield)
		{
			continue;
		}
		double FisXs = GetLibFisXs(i);
		FisRateSum = FisRateSum + StepNt[STEP-1][i]*FisXs;
		if(StepNt[STEP-1][i]*FisXs > MaxFisRate)
		{
			MaxFisRate = StepNt[STEP-1][i]*FisXs;
			MaxFisNucID = FisNucID;
		}
	}

	if(FisRateSum == 0)
	{
		YieldFactor = 1.0;
		NearestNeighbor = 0;
		return;
	}

	FindNeighbor(MaxFisNucID);

	int NeiIndex = GetNucIdIndex(NearestNeighbor);
	if(StepNt[STEP-1][NeiIndex] == 0)
	{
		YieldFactor = 1.0;
		NearestNeighbor = 0;
		return;
	}
	YieldFactor = 1.0 + FisRateSum/(StepNt[STEP-1][NeiIndex]*GetLibFisXs(NeiIndex));
}

void Depth_Class::FindNeighbor(int MaxFisNucID)
{
	int i, Distant;
	Distant = abs(MaxFisNucID - YieldData[0].FisNucID);
	for(i = 1 ; i < YieldData.size(); ++i)
	{
		if(abs(MaxFisNucID  - YieldData[i].FisNucID) > Distant){break;}
		Distant = abs(MaxFisNucID - YieldData[i].FisNucID);
	}

	NearestNeighbor = YieldData[i-1].FisNucID;
	return;
}
