# include "Plot.h"

void CDPlot::BackupSurfBound(CDGeometry &cGeometry)
{
	for (int i =0; i < cGeometry.p_vSurface.size(); i++)
	{
		if (cGeometry.p_vSurface[i].p_nBoundCond != 0 )
		{
			m_vNonNormalSurfaces.push_back(i);
			m_vNonNormalSurfacesBound.push_back(cGeometry.p_vSurface[i].p_nBoundCond);
			cGeometry.p_vSurface[i].p_nBoundCond=0;
		}
	}
	return;
}


void CDPlot::RecoverSurfBound(CDGeometry &cGeometry)
{
	for (int i =0; i < m_vNonNormalSurfaces.size(); i++)
	{
		int nSurfaceIndex = m_vNonNormalSurfaces[i];
		cGeometry.p_vSurface[ nSurfaceIndex ].p_nBoundCond = m_vNonNormalSurfacesBound[i];
	}
	return;
}