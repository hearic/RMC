# include "Geometry.h"

////////////////////////////////////////////////////////////
//////////  reflection treatment for ray tracking //////////
////////////////////////////////////////////////////////////
void CDGeometry::ReflectParticle(CDParticleState &cParticleState,int si)
{
	p_vSurface[si].ReflectParticle(cParticleState);// only direction is changed

	if (CheckParticleLocation(cParticleState))
	{
		cParticleState.p_bIsCellChanged = false ;
		cParticleState.p_bIsMatChanged = false ;
		cParticleState.p_bIsCellTmpChanged = false ;
		return;
	}

	// particle enters into new cell, reflective surface is coincident with another surface
    
	// output info for debug
	/*
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"particle enters into different cell after reflecting.\n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_WARNING);
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Reflective surface: %d.\n",p_OSurfaceIndex.GetIndexU(si));}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"origin:\npos=%f %f %f, dir=%f %f %f\ncell=%s\n",
		cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],cParticleState.p_dDir[0],cParticleState.p_dDir[1],cParticleState.p_dDir[2],
		Output.PrintCellVec(cParticleState.p_vLocCellsU));},Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	*/

	// re-locate particle
	auto vPrev_cells = cParticleState.p_vLocCells;
	int nPrev_cell = cParticleState.p_nCELL;
	cParticleState.ClearLevel();
	cParticleState.p_nCELL = LocateParticle(cParticleState, 0,cParticleState.p_dPos,cParticleState.p_dDir);
	if( vPrev_cells == cParticleState.p_vLocCells)
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

	// output warnings for debug
	/*
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"news: \npos=%f %f %f, dir=%f %f %f\ncell=%s\n",
		cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],cParticleState.p_dDir[0],cParticleState.p_dDir[1],cParticleState.p_dDir[2],
		Output.PrintCellVec(cParticleState.p_vLocCellsU));},Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	*/
	return;
}


void CDSurface::ReflectParticle(CDParticleState &cParticleState)    
{
	//////////////////////////////// get line_normal ///////////////////////////////////
	double dSurfNorm[3];
	GetSurfNormVec(cParticleState.p_dPos,dSurfNorm);

	//////////////////////////////// reflect direction /////////////////////////////////
	double dTemp1 = cParticleState.p_dDir[0]*dSurfNorm[0] + cParticleState.p_dDir[1]*dSurfNorm[1] + cParticleState.p_dDir[2]*dSurfNorm[2];
	if(dTemp1 < 0)
	{
		dSurfNorm[0] = -dSurfNorm[0];
		dSurfNorm[1] = -dSurfNorm[1];
		dSurfNorm[2] = -dSurfNorm[2];
		dTemp1 = -dTemp1;
	}
	dSurfNorm[0] = 2*dTemp1*dSurfNorm[0];
	dSurfNorm[1] = 2*dTemp1*dSurfNorm[1];
	dSurfNorm[2] = 2*dTemp1*dSurfNorm[2];	

	cParticleState.p_dDir[0] = cParticleState.p_dDir[0] - dSurfNorm[0];
	cParticleState.p_dDir[1] = cParticleState.p_dDir[1] - dSurfNorm[1];
	cParticleState.p_dDir[2] = cParticleState.p_dDir[2] - dSurfNorm[2];	

	
	cParticleState.p_dLocDir[0] = cParticleState.p_dLocDir[0] - dSurfNorm[0];
	cParticleState.p_dLocDir[1] = cParticleState.p_dLocDir[1] - dSurfNorm[1];
	cParticleState.p_dLocDir[2] = cParticleState.p_dLocDir[2] - dSurfNorm[2];

	CDGlobeFun::Normalize3Array(cParticleState.p_dDir);
	CDGlobeFun::Normalize3Array(cParticleState.p_dLocDir);

	return;

	///////////////////////////// locate cell avoiding mistakes //////////////////////////////
}