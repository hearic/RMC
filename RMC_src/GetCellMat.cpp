# include "Geometry.h"

int CDGeometry::GetCellMat(const vector<int> &vCellsU) // CellU: user defined cell
{
	int nCellLevel = vCellsU.size();
	int nCell_index_u = vCellsU[nCellLevel-1];
	int nCell_index = p_OCellIndex.GetIndex(nCell_index_u);

	if(!p_vCell[nCell_index].p_bIsExpdMat)
	{
		return p_vCell[nCell_index].p_nMatIndex;
	}

	long long llFindKey = GetMapKeyWord(vCellsU);
	if(p_vCell[nCell_index].LatMatMap.find(llFindKey) == p_vCell[nCell_index].LatMatMap.end()) 
	{  
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown material in cell %d!\n",nCell_index_u);},CDOutput::_WARNING);

		return 0 ; // vacuum
	}

	return p_vCell[nCell_index].LatMatMap[llFindKey];
}