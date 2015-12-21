# include "Geometry.h"
# include "Criticality.h"

const double OVERLAP_ERROR = 1.0E-08; // prefer to outer boundary in case of surface overlap. bug if 1.0E-06
const double ZERO_DISTANCE = 1.0E-10;

double CDGeometry::CalcDistToBound(CDParticleState &cParticleState)
{
	double dDistance_min =1.0E+22;
	int nAtSurf = abs(cParticleState.p_nBoundSURF);
	cParticleState.p_nBoundSURF = 0;
	cParticleState.p_nLatBoundSURF = -1 ;
	
	double dLocPosTemp[3];
	double dLocDirTemp[3];

	//////////////////////////// INNER case: start from the current universe //////////////////////////  
	if(p_vCell[cParticleState.p_nCELL].p_bIsInnerCell)
	{
		int nLevel = cParticleState.p_vLocCells.size() - 1  ;
		int nUniv = cParticleState.p_vLocUnivs[nLevel] ;
		int nCell_index = cParticleState.p_vLocCells[nLevel] ;
		int nCell = p_vUniverse[nUniv].p_vFillCellsIndex[nCell_index] ;

		//// transform direction from universe 0 to current universe /////
		for (int i = 0 ; i < 3 ; ++i )
		{
			dLocDirTemp[i] = cParticleState.p_dDir[i];
		}
		int nTot_level = cParticleState.p_vLocCells.size(); 
		for(int nLevel = 0 ; nLevel < nTot_level; ++nLevel )
		{
			int univ = cParticleState.p_vLocUnivs[nLevel] ; //// lower universe
			p_vUniverse[univ].TransUnivDir(dLocDirTemp); //// Transform coordinate to inside universe
		}
		
		for (int i = 0 ; i < 3 ; ++i )
		{
			cParticleState.p_dLocDir[i] = dLocDirTemp[i];
		}

		for(int pj = 0;pj < p_vCell[nCell].p_vBoundSurf.size();pj++)
		{
			int nSurf_index = p_vCell[nCell].p_vBoundSurf[pj];          // surface index

			int nSurf_index0 = abs(nSurf_index);
			bool bIsAtSurf = (nSurf_index0 == nAtSurf);
			double dDistance_temp = p_vSurface[nSurf_index0].CalcDistToSurf(cParticleState.p_dLocPos,cParticleState.p_dLocDir,bIsAtSurf);
			if(dDistance_temp > ZERO_DISTANCE)
			{
				if(dDistance_min -dDistance_temp > OVERLAP_ERROR)    // prefer to outer boundary in case of surface overlap
				{
					dDistance_min = dDistance_temp;
					cParticleState.p_nBoundSURF = nSurf_index;
					cParticleState.p_nLatBoundSURF = 0;
					cParticleState.p_nBoundINDEX = pj;      // crossing-surface
					cParticleState.p_nBoundLEVEL = nLevel ;
				}
			}
		}
	}
	////////////////////////// Normal case: start from the universe 0 ////////////////////////
	else
	{
		for (int i = 0 ; i < 3 ; ++i )
		{
			dLocPosTemp[i] = cParticleState.p_dPos[i];
			dLocDirTemp[i] = cParticleState.p_dDir[i];
		}

		////////////////////// get position & direction of universe 0 ////////////////////////
		int nTot_level = cParticleState.p_vLocCells.size(); 
		for(int nLevel = 0 ; nLevel < nTot_level; ++nLevel )
		{
			int nUniv = cParticleState.p_vLocUnivs[nLevel] ; //// lower universe
			/////////////////// Transform coordinate to inside universe ////////////////////////
			p_vUniverse[nUniv].TransUnivCoord(dLocPosTemp,dLocDirTemp); 

			/////////////////////  calculate distance to lattice boundary //////////////////////
			if (p_vUniverse[nUniv].p_bIsLattice)
			{
				int nLat_index = cParticleState.p_vLocCells[nLevel]  ;
				p_vUniverse[nUniv].MoveToOriginLat(nLat_index, dLocPosTemp);    // Transform coordinate to Origin Index and
				int nLat_bound_surf;
				double dDistance_temp = p_vUniverse[nUniv].CalcDistToLat(dLocPosTemp,dLocDirTemp,nLat_bound_surf);
				if(dDistance_temp < 0) // wrong lattice boundary
				{
					break;
				}

				if(dDistance_min - dDistance_temp > OVERLAP_ERROR)  ////prefer to outer boundary in case of surface overlap
				{
					dDistance_min = dDistance_temp;
					cParticleState.p_nBoundLEVEL = nLevel ;
					cParticleState.p_nLatBoundSURF = nLat_bound_surf ;
					cParticleState.p_nBoundSURF = 0 ;
					for(int i = 0 ; i < 3 ; i++)
					{
						cParticleState.p_dLocPos[i] = dLocPosTemp[i];
						cParticleState.p_dLocDir[i] = dLocDirTemp[i];
					}
				}
				continue ;
			}

			/////////////////////////  calculate distance to cell boundary ///////////////////////////
			int nCell_index = cParticleState.p_vLocCells[nLevel] ;
			int nCell = p_vUniverse[nUniv].p_vFillCellsIndex[nCell_index] ;

			int nSize = p_vCell[nCell].p_vBoundSurf.size();
			for(int pj = 0;pj < nSize; ++pj)
			{
				int si = p_vCell[nCell].p_vBoundSurf[pj];     // surface cell_index
				int si0 = abs(si);
				bool bIsAtSurf = (si0 == nAtSurf);

				double dDistance_temp = p_vSurface[si0].CalcDistToSurf(dLocPosTemp,dLocDirTemp,bIsAtSurf);
				if(dDistance_temp <= ZERO_DISTANCE)
				{
					continue;
				}

				if(dDistance_min -dDistance_temp > OVERLAP_ERROR)    //prefer to outer boundary in case of surface overlap
				{
					dDistance_min = dDistance_temp;
					cParticleState.p_nBoundSURF = si;
					cParticleState.p_nLatBoundSURF = 0 ;
					cParticleState.p_nBoundINDEX = pj;      // crossing-surface
					cParticleState.p_nBoundLEVEL = nLevel ;
					for(int i = 0 ; i < 3 ; i++)
					{
						cParticleState.p_dLocPos[i] = dLocPosTemp[i];
						cParticleState.p_dLocDir[i] = dLocDirTemp[i];
					}
				}
			}
		}
	}


	////////////////////////////////// check errors ////////////////////////////////////////
	if((cParticleState.p_nBoundSURF == 0 && cParticleState.p_nLatBoundSURF == -1) || dDistance_min <= 0 )
	{
		//sprintf(Output.PrintStr,"failed to calculate distance to boundary in cell %s. %s\n",
		//	Output.PrintCellVec(ParticleState.LocCellsU),RMCIO.PrintParticleState(ParticleState));

		//RMCIO.PrintScrn(1);

		//++C_Criticality.MissParticleCount;
		//C_Criticality.IsParticleKilled = true;

		dDistance_min = -1;
	}

	return dDistance_min;
}