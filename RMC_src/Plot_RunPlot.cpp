# include "Plot.h"

void CDPlot::RunPlot(CDGeometry &cGeometry, CDMaterial &cMaterial)
{
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif
	if (!p_bIsPlot)
	{
		return;
	}
	
	InitPlot(cGeometry, cMaterial);

	//Run Plotting image one by one
	for (int nPlotNum = 0;nPlotNum < p_vPlotInput.size(); nPlotNum++)
	{
		UpdatePlotParas(p_vPlotInput[nPlotNum]);
		GenPixelMatrix(cGeometry);
		WriteImage();
		OutputPlotFile(nPlotNum, cGeometry, cMaterial);
	}

	ResetPlot(cGeometry);

	return;
}
