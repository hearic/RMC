# include "Geometry.h"

void CDGeometry::BuildNeiCellList()
{
	///////////build neighbor cell for each universe////

	int nUniv;
	int i1,i2,j1,j2;
	int nCell_num;
	int nCell1,nCell2;
	int nSurf_num1, nSurf_num2;
	int nSurf1,nSurf2;

	int nTotUnivNum = p_vUniverse.size();
	for(nUniv = 0 ; nUniv < nTotUnivNum ; ++nUniv )
	{
		if(p_vUniverse[nUniv].p_bIsLattice)
		{
			continue;
		}
		nCell_num = p_vUniverse[nUniv].p_nContainCellNum ;
		p_vUniverse[nUniv].p_vNeighborCells.resize(nCell_num) ;
		for ( i1 = 0 ; i1 < nCell_num ; ++i1 )
		{
			nCell1 = p_vUniverse[nUniv].p_vFillCellsIndex[i1] ;  // select one cell
			nSurf_num1 = p_vCell[nCell1].p_vBoundSurf.size() ;
			p_vUniverse[nUniv].p_vNeighborCells[i1].resize(nSurf_num1);
			for (j1 = 0 ; j1  < nSurf_num1 ; ++j1 )
			{
				nSurf1 = p_vCell[nCell1].p_vBoundSurf[j1] ;   // select one bound surface of the cell

				for ( i2 = 0 ; i2 < nCell_num ; ++i2 ) // find neighbor cell corresponding to the selected surface of the cell
				{
					if ( i2 == i1){continue ;}

					nCell2 = p_vUniverse[nUniv].p_vFillCellsIndex[i2] ;
					nSurf_num2 = p_vCell[nCell2].p_vBoundSurf.size() ;
					for (j2 = 0 ; j2  < nSurf_num2 ; ++j2 )
					{
						nSurf2 = p_vCell[nCell2].p_vBoundSurf[j2] ;
						if (nSurf2 == -nSurf1)          //  common and opposite surface found -->  neighbor cell 
						{
							p_vUniverse[nUniv].p_vNeighborCells[i1][j1].push_back(i2);   // Note!!! : i2
							break ;
						}
					}
				}
			}
		}
	}
}