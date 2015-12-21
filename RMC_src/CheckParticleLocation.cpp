# include "Geometry.h"

bool CDGeometry::CheckParticleLocation(CDParticleState &cParticleState)
{
	//// Check if particle located in right cell vectors
	
	double dLocPosTemp[3],dLocDirTemp[3];
	for (int i = 0 ; i < 3 ; ++i )
	{
		dLocPosTemp[i] = cParticleState.p_dPos[i];
		dLocDirTemp[i] = cParticleState.p_dDir[i];
	}
	int nTot_level = cParticleState.p_vLocCells.size(); 
	for(int nLevel = 0 ; nLevel < nTot_level; ++nLevel )
	{
		int nUniv = cParticleState.p_vLocUnivs[nLevel] ; //// lower universe
		// Transform coordinate to inside universe
		p_vUniverse[nUniv].TransUnivCoord(dLocPosTemp,dLocDirTemp); 
		// Transform coordinate to Origin Index
		if (p_vUniverse[nUniv].p_bIsLattice)
		{
			int nLat_index = p_vUniverse[nUniv].FindLatIndex(dLocPosTemp,dLocDirTemp);
			if (nLat_index != cParticleState.p_vLocCells[nLevel])
			{
				return false;
			}			
			p_vUniverse[nUniv].MoveToOriginLat(nLat_index, dLocPosTemp);
		}
		else
		{
			// check if particle is inside the cell
			int nCell_Index = cParticleState.p_vLocCells[nLevel];
			int nCell = p_vUniverse[nUniv].p_vFillCellsIndex[nCell_Index];
			if( !p_vCell[nCell].ParticleIsInCell(dLocPosTemp,dLocDirTemp,p_vSurface))
			{
				return false;
			}
		}
	}
	
	return true;
}
