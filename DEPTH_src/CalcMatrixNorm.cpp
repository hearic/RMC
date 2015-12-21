# include "ME_Class.h"
# include "Depth_Class.h"

void ME_Class::CalcMatrixNorm()
{
	TranMatrixNorm = 0;
	TranMatrixNormNuc = 0;

	int NucNum = Depth.NucNum;

	for(int i = 1;i <= NucNum; ++i )
	{
		if( -TranMatrixMCS._DiagVal[i] > TranMatrixNorm )
		{
			TranMatrixNorm = -TranMatrixMCS._DiagVal[i];
			TranMatrixNormNuc = i;
		}
	}

	return;
}