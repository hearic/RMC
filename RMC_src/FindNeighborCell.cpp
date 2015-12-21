# include "Geometry.h"

void CDGeometry::FindNeighborCell(CDParticleState &cParticleState)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////// Fixed bug for concave polyhedron. benchmark-pumt21a. 2012-9-2 //////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	bool bFound_neighbor = false;
	int nNeighbor_cell = -1;
	auto vPrev_cells = cParticleState.p_vLocCells;

	/////////////// Get universe information of current level(inherent from CalDistanceToBound ) ///////////
	int nLevel = cParticleState.p_nBoundLEVEL  ;
	cParticleState.p_vLocUnivs.resize(nLevel+1);
	cParticleState.p_vLocCells.resize(nLevel+1);
	cParticleState.p_vLocCellsU.resize(nLevel+1);
	int nUniv = cParticleState.p_vLocUnivs[nLevel] ;

	double dLocPosTemp[3];
	double dLocDirTemp[3];
	for (int i = 0 ; i < 3 ; ++i )
	{
		dLocPosTemp[i] = cParticleState.p_dLocPos[i];
		dLocDirTemp[i] = cParticleState.p_dLocDir[i];
	}

	///////////////////////////// The Universe is lattice //////////////////////////////////////
	///////////////////////// => change lattice index => search filled universe ////////////////
	if (p_vUniverse[nUniv].p_bIsLattice)
	{
		int nLat_index = p_vUniverse[nUniv].OffsetNeighborLat(cParticleState.p_vLocCells[nLevel],cParticleState.p_nLatBoundSURF,dLocPosTemp);
		if(nLat_index >= 0) // else found_neighbor = false;
		{
			//// If current level is lattice, LocCells[i] stores lattice index 
			cParticleState.p_vLocCells[nLevel] = nLat_index; 
			cParticleState.p_vLocCellsU[nLevel] = nLat_index;
			int nFilled_univ =  p_vUniverse[nUniv].p_vFillLatUnivIndex[nLat_index-1] ;
			p_vUniverse[nFilled_univ].TransUnivCoord(dLocPosTemp,dLocDirTemp);                                    // PosTemp: coordinate in filled Universe
			nNeighbor_cell = LocateParticle(cParticleState, nFilled_univ,dLocPosTemp,dLocDirTemp);  // locate particle in filled universe
		}
	}
	/////////////////////////////// The Universe is not lattice  //////////////////////////////////////
	///////////////// => search neighbor list => if complex cell, search filled universe //////////////
	else
	{
		int nCell_i1 = cParticleState.p_vLocCells[nLevel] ;
		int nCell_i2 = -1;
		int nBound = cParticleState.p_nBoundINDEX ;
		int nNei_cell_num = p_vUniverse[nUniv].p_vNeighborCells[nCell_i1][nBound].size();  // number of neighbor cell list

		
		for(int i = 0;i < nNei_cell_num; ++i)
		{
			nCell_i2 = p_vUniverse[nUniv].p_vNeighborCells[nCell_i1][nBound][i] ;
			nNeighbor_cell = p_vUniverse[nUniv].p_vFillCellsIndex[nCell_i2];

			if( p_vCell[nNeighbor_cell].ParticleIsInCell(dLocPosTemp,dLocDirTemp,p_vSurface))
			{
				bFound_neighbor = true;
				break;
			}
		}
		if(bFound_neighbor)  // caution: found_neighbor maybe false in concave polyhedron case.
		{
			///// If current level is not lattice, LocCells[i] stores cell index
			cParticleState.p_vLocCells[nLevel] = nCell_i2;  
			cParticleState.p_vLocCellsU[nLevel] = p_OCellIndex.GetIndexU(nNeighbor_cell);

			int nFilled_univ ;
			nFilled_univ =  p_vCell[nNeighbor_cell].p_nFillUnivIndex ;
			if(nFilled_univ > 0)  // nei_cell is a complex cell
			{
				p_vUniverse[nFilled_univ].TransUnivCoord(dLocPosTemp,dLocDirTemp); 
				nNeighbor_cell = LocateParticle(cParticleState, nFilled_univ,dLocPosTemp,dLocDirTemp);
			}
		}
	}

    ////////////  locate particle in overall universe if failed to locate particle by neighbor search. //////////
	if(!bFound_neighbor)   
	{
		//printf("Start globle neighbor cell search.\n");
		cParticleState.ClearLevel();
		nNeighbor_cell = LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	}
	

	if(nNeighbor_cell == -1) ////// failed to locate neighbour cell
	{
		return;
	}
	

	/* [DON'T DELETE!] [NOT NECESSARY YET]
	// Check if particle enters into right cell, in case crossing point coincides with interface
	if (bFound_neighbor && !CheckParticleLocation(cParticleState))
	{
		/ * // print info for debug
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"particle enters into wrong cell after crossing surface.pos=%f %f %f, dir=%f %f %f cell=%s\n",
			cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],cParticleState.p_dDir[0],cParticleState.p_dDir[1],cParticleState.p_dDir[2],
			Output.PrintCellVec(cParticleState.p_vLocCellsU));}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		* /
		cParticleState.ClearLevel();
		nNeighbor_cell = LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	}
	*/

	int nPrev_cell = cParticleState.p_nCELL;
	cParticleState.p_nCELL = nNeighbor_cell ;

	if(vPrev_cells == cParticleState.p_vLocCells)
	{
		cParticleState.p_bIsCellChanged = false ;
		cParticleState.p_bIsMatChanged = false;
		cParticleState.p_bIsCellTmpChanged = false;
	}
	else
	{
		cParticleState.p_bIsCellChanged = true ;

		int prev_mat = cParticleState.p_nMAT;
		cParticleState.p_nMAT = GetCellMat(cParticleState.p_vLocCellsU);
		if(cParticleState.p_nMAT == prev_mat)
		{
			cParticleState.p_bIsMatChanged = false;
		}
		else
		{
			cParticleState.p_bIsMatChanged = true;
		}

		if(p_vCell[nPrev_cell].p_dTemp == p_vCell[cParticleState.p_nCELL].p_dTemp)
		{
			cParticleState.p_bIsCellTmpChanged = false;
		}
		else
		{
			cParticleState.p_bIsCellTmpChanged = true;
		}

	}
	
	
	return;
}