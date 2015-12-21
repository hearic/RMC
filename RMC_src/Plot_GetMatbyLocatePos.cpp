# include "Plot.h"

void CDPlot::GetMatbyLocatePos(CDGeometry &cGeometry, CDVector3 cVertex_temp, int &nMat, int &nCell)
{
	//set start position and fly direction
	m_OPlotPaticleState.p_dPos[ 0 ] = cVertex_temp._x;
	m_OPlotPaticleState.p_dPos[ 1 ] = cVertex_temp._y;
	m_OPlotPaticleState.p_dPos[ 2 ] = cVertex_temp._z;

	m_OPlotPaticleState.p_dDir[ 0 ] = 0.7;
	m_OPlotPaticleState.p_dDir[ 1 ] = 0.7;
	m_OPlotPaticleState.p_dDir[ 2 ] = sqrt(1.0 - 2*0.7*0.7);
	
	//Locate initial particle position for tracking
	m_OPlotPaticleState.ClearLevel();
	m_OPlotPaticleState.p_nCELL = cGeometry.LocateParticle(m_OPlotPaticleState, 0,m_OPlotPaticleState.p_dPos,m_OPlotPaticleState.p_dDir);
	if(m_OPlotPaticleState.p_nCELL == -1) // cannot locate position//Wrong Geometry
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to locate plotting point: Pos = %.15f %.15f %.15f. Check if the up-left vertex is on a defined surface (latticed)\n",
			m_OPlotPaticleState.p_dPos[0],m_OPlotPaticleState.p_dPos[1],m_OPlotPaticleState.p_dPos[2]);},CDOutput::_WARNING);
		nMat = 0;
		nCell = 0;
	}
	else
	{
		nMat = cGeometry.GetCellMat(m_OPlotPaticleState.p_vLocCellsU);
		nCell = m_OPlotPaticleState.p_nCELL;
	}
	return;
}