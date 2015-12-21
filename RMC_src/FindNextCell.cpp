# include "Geometry.h"

void CDGeometry::FindNextCell(CDParticleState &cParticleState)
{

	int nUniv = cParticleState.p_vLocUnivs[cParticleState.p_nBoundLEVEL] ; //// Currently located universe 

	if (p_vUniverse[nUniv].p_bIsLattice)  //// Lattice case
	{
		FindNeighborCell(cParticleState);
	}
	else //// Ordinary case
	{
		enum eBoundCondType{crossing = 0,reflective = 1,white = 2};

		int si = abs(cParticleState.p_nBoundSURF);
		switch(p_vSurface[si].p_nBoundCond)
		{
		case reflective: 
			{
				/* Fixed bug for concave polyhedron with reflective surfaces. (found by SXT). 2014-01-13 */
				/* Find neighbor cell firstly to judge whether cell is changed */
				/* Only reflecting particle when bound_cell (cell of bound surf) is changed!*/

				CDParticleState cParticleStateTemp = cParticleState;
				FindNeighborCell(cParticleState);							 
				// cell vectors not changed: it is a fake surface in concave polyhedron.
				// Do NOT reflect particle
				if ( ! cParticleState.p_bIsCellChanged )
				{					
					break;
				}
				
				// cell vectors changed: judge whether bound cell changed
				// To account for complex geometry case, may be only bottom cell changed in repeat structure
				// like 1>2>3 to 1>2>4 where cell 2 in level 2 not change, this reflective surface is still fake 
				bool IsBoundCellChanged = true;
				int nLevel = cParticleStateTemp.p_nBoundLEVEL ;// level of bound surf, maybe the outer one for overlap case
				int nOld_Bound_Cell = cParticleStateTemp.p_vLocCells[nLevel];
				if ( cParticleState.p_vLocCells.size() > nLevel )// else: bound_cell must be changed
				{
					int nNew_Bound_Cell = cParticleState.p_vLocCells[nLevel];//same level with previous one
					IsBoundCellChanged = ( nOld_Bound_Cell != nNew_Bound_Cell );
				}

				// reflect particle if bound_cell is changed
				if ( IsBoundCellChanged )
				{
					cParticleState = cParticleStateTemp ;// recover particle state
					ReflectParticle(cParticleState,si);  // reflect particle(with check)
				}
				
				break;
			}  

		case crossing: 
			{
				FindNeighborCell(cParticleState);
				break;
			}
		default:
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"wrong boundary condition.\n");}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

		}// switch½áÊø
	}


	if(cParticleState.p_nCELL == -1)          // wrong cell
	{
		//++ C_Criticality.MissParticleCount;
		cParticleState.p_bIsKilled = true;
		return;
	}

	if(p_vCell[cParticleState.p_nCELL].p_nImp == 0)  // vacuum/black/wrong cell
	{
		cParticleState.p_bIsKilled = true;
		return;
	}
}