# include "Depth_Class.h"
const double THERMAL_ENERGY = 0.0253;
const double FAST_ENERGY = 2.0E+06;
const double HIGH_ENERGY = 1.4E+07;

void Depth_Class::InterpolateYield()
{
	for(int i = 0 ; i < YieldData.size(); ++i)
	{
		int erg_num = YieldData[i].Energy.size();
		int product_num = YieldData[i].ProductId.size();
		YieldData[i].PrdctYld.resize(product_num);

		if(erg_num == 1)
		{
			YieldData[i].PrdctYld = YieldData[i].ProductYld[0];
		}
		else if(erg_num == 2)
		{
			if(AveEnergy <= YieldData[i].Energy[0])
			{
				YieldData[i].PrdctYld = YieldData[i].ProductYld[0];
			}
			else if(AveEnergy >= YieldData[i].Energy[1])
			{
				YieldData[i].PrdctYld = YieldData[i].ProductYld[1];
			}
			else
			{
				double frac = (AveEnergy - YieldData[i].Energy[0])/(YieldData[i].Energy[1] - YieldData[i].Energy[0]);
				for(int j = 0 ; j < product_num ; ++j)
				{
					YieldData[i].PrdctYld[j] = frac * YieldData[i].ProductYld[0][j] + (1 - frac) * YieldData[i].ProductYld[1][j];
				}
			}
		}
		else if(erg_num == 3)
		{
			if(AveEnergy <= YieldData[i].Energy[0])
			{
				YieldData[i].PrdctYld = YieldData[i].ProductYld[0];
			}
			else if(AveEnergy >= YieldData[i].Energy[2])
			{
				YieldData[i].PrdctYld = YieldData[i].ProductYld[2];
			}
			else if(AveEnergy <= YieldData[i].Energy[1])
			{
				double frac = (AveEnergy - YieldData[i].Energy[0])/(YieldData[i].Energy[1] - YieldData[i].Energy[0]);
				for(int j = 0 ; j < product_num ; ++j)
				{
					YieldData[i].PrdctYld[j] = frac * YieldData[i].ProductYld[0][j] + (1 - frac) * YieldData[i].ProductYld[1][j];
				}
			}
			else
			{
				double frac = (AveEnergy - YieldData[i].Energy[1])/(YieldData[i].Energy[2] - YieldData[i].Energy[1]);
				for(int j = 0 ; j < product_num ; ++j)
				{
					YieldData[i].PrdctYld[j] = frac * YieldData[i].ProductYld[1][j] + (1 - frac) * YieldData[i].ProductYld[2][j];
				}
			}
		}
	}

	return;
}