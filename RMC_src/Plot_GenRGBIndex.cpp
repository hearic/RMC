# include "Plot.h"

void CDPlot::GenRGBIndex(CDGeometry &cGeometry, CDMaterial &cMaterial)
{
	for (int i=0; i < p_vPlotInput.size(); i++)
	{
		if (p_vPlotInput[i].p_nColorType == MAT || p_vPlotInput[i].p_nColorType == MAT_SURF)
		{
			m_bIsEverMatColor = true;
		}
		if (p_vPlotInput[i].p_nColorType == CELL || p_vPlotInput[i].p_nColorType == CELL_SURF)
		{
			m_bIsEverCellColor = true;
		}
		if (p_vPlotInput[i].p_nColorType == SURF || p_vPlotInput[i].p_nColorType == MAT_SURF || p_vPlotInput[i].p_nColorType == CELL_SURF)
		{
			m_bIsEverSurfColor = true;
		}
	}

	// Generate Mat-RGB Index
	int nMatNum = cMaterial.GetTotMatNum();
	m_vMatRGBIndex.resize(nMatNum+2); //Black, White
	m_vMatRGBIndex[0] = m_OBLACKRGB;  //CELL boundary - surface, Black
	m_vMatRGBIndex[1] = m_OWHITERGB;  //mat = 0 is void, white
	
	//generate mat-RGB randomly mat = 1~MatNum
	srand(p_unColorScheme);
	for(int i = 2 ; i < m_vMatRGBIndex.size(); i++)
	{
		/*//RGB: 100~254, brighter
		RGBIndex[i].r = 100 + rand()%155;
		RGBIndex[i].g = 100 + rand()%155;
		RGBIndex[i].b = 100 + rand()%155;*/
		/*//RGB: 1~254
		RGBIndex[i].r = 1+ rand()%254;
		RGBIndex[i].g = 1 + rand()%254;
		RGBIndex[i].b = 1 + rand()%254;*/
		//RGB: 50~254
		m_vMatRGBIndex[i].r = 50+ rand()%205;
		m_vMatRGBIndex[i].g = 50 + rand()%205;
		m_vMatRGBIndex[i].b = 50 + rand()%205;
	}

	// Generate Cell-RGB Index
	int nCellNum = cGeometry.p_vCell.size() - 1;
	m_vCellRGBIndex.resize(nCellNum + 2 ); //Black, white, and nCellNum random color
	m_vCellRGBIndex[0] = m_OBLACKRGB;      //CELL boundary - surface, Black
	m_vCellRGBIndex[1] = m_OWHITERGB;      //cell = 0 (wrong cell), white

	//generate cell-RGB randomly, cell = 1 ~ cellnum
	for(int i = 2 ; i < m_vCellRGBIndex.size(); i++)
	{
		//RGB: 50~254
		m_vCellRGBIndex[i].r = 50+ rand()%205;
		m_vCellRGBIndex[i].g = 50 + rand()%205;
		m_vCellRGBIndex[i].b = 50 + rand()%205;
	}

	return;	
}