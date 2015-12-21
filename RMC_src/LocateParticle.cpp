# include "Geometry.h"

int CDGeometry::LocateParticle(CDParticleState &cParticleState, int nStartUniv, const double dPointPos[3],const double dPointDir[3],bool bCheckOverlap)
{	
	///// PointPos:  在第StartUniv层Universe当中的粒子的(x,y,z)坐标.
	///// StartUniv: 从该层Universe开始,搜索粒子所在栅元位置.其上层Universe的信息依然保留在LocUnivs和LocCells当中.
	/////            定位初始源所在栅元时,StartUniv = 0. 在FindNeighborCell时,StartUniv不为0.
	///// EnterSurf: 在FindNeighborCell时,略过对交界面(-EnterSurf)的判断.
	///// LocUnivs:  存储粒子的逐层Universe.
	///// LocCells:  LocCells[i]存储粒子位于第LocUnivs[i]层Universe下的第几个栅元,从0开始编号. 
	/////            若该层Universe为Lattice,则存储LocCells[i]存储LatIndex编号,从1开始编号.

	int nUniv = nStartUniv;  // search Start Universe 
	int nLocate_cell = -1;
	double dLocPosTemp[3];
	double dLocDirTemp[3];
	for (int i = 0 ; i < 3 ; ++i )
	{
		dLocPosTemp[i] = dPointPos[i];
		dLocDirTemp[i] = dPointDir[i];
	}

	int nRoundOff = 0 ;

Search_Universe:

	nRoundOff = nRoundOff + 1;
	if( (nRoundOff++) > 99)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"terminate locating particle because of too many levels (>99).\n");},CDOutput::_WARNING);
		return -1 ;
	}
	//////////////////// The Universe is lattice structure  /////////////////////////////
	if(p_vUniverse[nUniv].p_bIsLattice)
	{
		/////////// Transform coordinate to Lattice origin ///////////
		cParticleState.p_vLocUnivs.push_back(nUniv) ;

		////////////////// Find Lattice index and Transform coordinate to Original Index ////////////////
		int nLat_index = p_vUniverse[nUniv].FindLatIndex(dLocPosTemp,dLocDirTemp);
		if(nLat_index < 0)
		{
			goto end;
		}

		cParticleState.p_vLocCells.push_back(nLat_index) ;
		cParticleState.p_vLocCellsU.push_back(nLat_index) ;
		int nLat_univ = p_vUniverse[nUniv].p_vFillLatUnivIndex[nLat_index-1];	
		p_vUniverse[nUniv].MoveToOriginLat(nLat_index, dLocPosTemp);    // Transform coordinate to Origen Index
		p_vUniverse[nLat_univ].TransUnivCoord(dLocPosTemp, dLocDirTemp);          // Transform coordinate to filled Universe

		nUniv = nLat_univ ;
		goto Search_Universe ;
	}

	//////////////////// The Universe is non-lattice structure  /////////////////////////////
	bool bCell_found = false;
	cParticleState.p_vLocUnivs.push_back(nUniv);             // store universes level by level
	for(int i = 0 ; i < p_vUniverse[nUniv].p_nContainCellNum; ++i)
	{	
		int nCell_index = p_vUniverse[nUniv].p_vFillCellsIndex[i] ;

		if( p_vCell[nCell_index].ParticleIsInCell(dLocPosTemp,dLocDirTemp,p_vSurface))   // particle is inside the cell
		{
			////////////////////Check overlapping cells /////////////////////////// 
			if(bCheckOverlap)
			{
				if(bCell_found)
				{
					int nCell_u1 = p_OCellIndex.GetIndexU(nCell_index);
					int nCell_u2 = p_OCellIndex.GetIndexU(nLocate_cell);
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"overlapping cells (%d,%d) when locating particle.Pos = %6.3E %6.3E %6.3E,Dir =%6.3E %6.3E %6.3E.\n",
						nCell_u1,nCell_u2,dPointPos[0],dPointPos[1],dPointPos[2],dPointDir[0],dPointDir[1],dPointDir[2]);},CDOutput::_ERROR);
				}
				else
				{
					bCell_found = true;
				}
			}

			//////////////////////////////// locate particle in cells ////////////////////////
			cParticleState.p_vLocCells.push_back(i) ;           // store cells level by level
			cParticleState.p_vLocCellsU.push_back(p_OCellIndex.GetIndexU(nCell_index)) ;
			//////////////////////// simple cell case ///////////////////////
			if(p_vCell[nCell_index].p_nFillUnivIndex < 0)    
			{	
				for (int j = 0 ; j < 3 ; ++j )
				{
					cParticleState.p_dLocPos[j] = dLocPosTemp[j] ;
					cParticleState.p_dLocDir[j] = dLocDirTemp[j] ;
				}

				if(bCheckOverlap)
				{
					nLocate_cell = nCell_index;
					continue;
				}
				else
				{
					return nCell_index;
				}
			}
			else
			{
				//////////////////////// complex cell case ////////////////////////
				////////// tranform coordinate to universe, goto recursion ///////////
				int nFilled_univ = p_vCell[nCell_index].p_nFillUnivIndex ; 	
				p_vUniverse[nFilled_univ].TransUnivCoord(dLocPosTemp,dLocDirTemp);         
				nUniv = nFilled_univ ;

				goto Search_Universe ; 
			}
		}
	}
	
end:
	if(nLocate_cell < 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to locate particle. Pos = %.15f %.15f %.15f,Dir =%.15f %.15f %.15f.\n",
			dPointPos[0],dPointPos[1],dPointPos[2],dPointDir[0],dPointDir[1],dPointDir[2]);},CDOutput::_WARNING);	
	}

	return nLocate_cell;

}
