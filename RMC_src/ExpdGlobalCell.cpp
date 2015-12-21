# include "Geometry.h"

void CDGeometry::ExpdGlobalCell(vector<vector<int> > &vCellsU, vector<int> &vTempCellU, int nCellIndexU, int nUnivIndex)
{
	/// recursively find the given CellIndexU under universe[UnivIndex] ///
	/// CellsU storages the user-index cell vector sets like 1 > 2 > 3 ///
	// Note the user index and the system Index 

	if (p_vUniverse[nUnivIndex].p_bIsLattice)  //lattice case
	{
		for (int j = 0 ; j < p_vUniverse[nUnivIndex].p_nFilledLatNum ; ++j )
		{
			int nUniv_Index = p_OUniverseIndex.GetIndex(p_vUniverse[nUnivIndex].p_vFillLatUnivIndexU[j]) ;  
			// Note User index has not been converted BEFORE CheckGeometry

			vTempCellU.push_back(j+1);
			ExpdGlobalCell(vCellsU, vTempCellU, nCellIndexU, nUniv_Index);
			vTempCellU.pop_back();
		}
	}
	else  //ordinary case
	{
		for (int j = 0 ; j < p_vUniverse[nUnivIndex].p_nContainCellNum ; ++j )
		{
			int ncell_indexU = p_vUniverse[nUnivIndex].p_vFillCellsIndexU[j] ;   		
			int ncell_index = p_OCellIndex.GetIndex(ncell_indexU);
			// Note User index has not been converted BEFORE CheckGeometry

			if(ncell_indexU == nCellIndexU) // Find one cell
			{
				vTempCellU.push_back(ncell_indexU);
				vCellsU.push_back(vTempCellU);
				vTempCellU.pop_back();
				return;
			}

			int nUniv_indexU = p_vCell[ncell_index].p_nFillUnivIndexU;
			
			if (nUniv_indexU <= 0)
			{
				continue;
			}
			else
			{
				int nUniv_index = p_OUniverseIndex.GetIndex(nUniv_indexU); // Note User index has not been converted BEFORE CheckGeometry
				vTempCellU.push_back(ncell_indexU);
				ExpdGlobalCell(vCellsU, vTempCellU, nCellIndexU, nUniv_index);
				vTempCellU.pop_back();
			}
		}
	}
}