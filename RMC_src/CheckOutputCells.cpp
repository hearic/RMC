#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::CheckOutputCells(CDGeometry &cGeometry,CDTally &cTally)
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	int nSize =  int(p_vCellsForOutput.size());
	if(nSize == 0)
	{
		//CellIndexForOutput.resize(BurnCellNum);  // print all cells by default
		//for(int i = 0 ; i < BurnCellNum ; ++i)
		//{
		//	CellIndexForOutput[i] = i;
		//}
		return;
	}

	for(int k = 0 ; k < nSize; ++k)
	{
		int i = 0;
		if(cTally.p_bUseTallyCellMap)
		{
			auto llFindKey = cGeometry.GetMapKeyWord(p_vCellsForOutput[k]);
			if(cTally.CsTallyCellIndex.find(llFindKey) == cTally.CsTallyCellIndex.end()) 
			{  
				continue ;	
			}
			i = cTally.CsTallyCellIndex[llFindKey] - p_nBurnTallyStartPos;
		}
		else
		{
			for(i = 0;i < p_nBurnCellNum; ++i )
			{
				int nTallyPos = p_nBurnTallyStartPos + i ;
				if( cTally.p_vOneGroupCsTally[nTallyPos].p_vCellVecU == p_vCellsForOutput[k] )
				{
					break;
				}
			}
			continue;
		}

		p_vCellIndexForOutput.push_back(i);
	}

	if(p_vCellIndexForOutput.size() < nSize)
	{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"%d/%d cells are effective for output in burnup calculations.\n",
				int(p_vCellIndexForOutput.size()),nSize);}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	}

	
	return;
}