# include "Plot.h"

CDPlot::CDPlot()
{
	//Plot Class constructor
	//Const
	m_OBLACKRGB.r = 0;	m_OBLACKRGB.g = 0;	m_OBLACKRGB.b = 0;
	m_OWHITERGB.r = 255;m_OWHITERGB.g = 255;m_OWHITERGB.b = 255;

	p_bIsPlot = false;
	p_unColorScheme = 1;//Default color seed

	m_bIsLowResolution = false;

	m_dDeltaWidth1 = 0.0;
	m_dDeltaWidth2 = 0.0;

	m_vNonNormalSurfaces.resize(0);
	m_vNonNormalSurfacesBound.resize(0);
}