# include "Burnup.h"

void CDBurnup::CheckDepletionNuc(CDGeometry &cGeometry,CDMaterial &cMaterial)
{
	///////////////////////  check material/nuclide in depletion cell  /////
	// 1. check non-depletion nuclides
	// 2. check heat release

	/////////////////// Only Run once for burnup case ///////////////////
	static int nRunOnlyOnce = 0;
	if(nRunOnlyOnce == 1){return;}
	nRunOnlyOnce = 1 ;
	
	if(p_nBurnStep != 0 )
	{
		return;
	}

	int nTotBurnNucNum = Depth.NucNum ;
	vector<double> vCellBurnNucDen;
	vCellBurnNucDen.resize(nTotBurnNucNum + 1);

	for(int i = 0;i < p_vBurnableCell.size(); ++i)
	{
		int nCellIndexU = p_vBurnableCell[i];
		int nCellIndex = cGeometry.p_OCellIndex.GetIndex(nCellIndexU);
		if (cGeometry.p_vCell[nCellIndex].p_bIsExpdMat)
		{
			continue; // NO check if lattice mat input
		}
		int nMatIndexU = cGeometry.p_vCell[nCellIndex].p_nMatIndexU;
		int nMatIndex = cMaterial.p_OMatSetIndex.GetIndex(nMatIndexU);
		int nMatNucNum = cMaterial.GetMatTotNucNum(nMatIndex);
		double dVol = cGeometry.p_vCell[nCellIndex].p_dVol;

		CDGlobeFun::Set1DVecZero(vCellBurnNucDen);

		int nBurnNucNum = 0;
		for(int j = 0 ; j < nMatNucNum ; ++j )
		{
			int index = ConvertNucId(cMaterial.GetMatNucID(nMatIndex,j).p_chIdStr);
			if(index > 0 ) // normal
			{
				vCellBurnNucDen[index] = dVol*cMaterial.GetMatNucAtomDen(nMatIndex,j);
				nBurnNucNum ++; 
			}
			else if (index == -1000)//Current nuclide is natural isotope, warning
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Natural nuclide %s of mat %d in cell %d will not be depleted.\n",cMaterial.GetMatNucID(nMatIndex,j).p_chIdStr, nMatIndexU, nCellIndexU);}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);// only master print
			}
			else// Current nuclide not in Depth isotope chain,, warning
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Nuclide %s of mat %d in cell %d will not be depleted.\n",cMaterial.GetMatNucID(nMatIndex,j).p_chIdStr, nMatIndexU, nCellIndexU);}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			}			
		}
		/*1. check non-depletion nuclides*/
		if (nBurnNucNum == 0)//No isotope to be depleted
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"No isotope is to be depleted in burnable cell %d (mat = %d ). Check nuclides or cancel this cell to be burnable.\n", nCellIndexU, cMaterial.p_OMatSetIndex.GetIndexU(nMatIndex));}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
		/*2. check heat release*/
		double dPowerFactor = Depth.CalcPowerFactor(vCellBurnNucDen);
		if (dPowerFactor <= 0)//No heat release in this region
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"No heat release in burnable cell %d (mat = %d ). Check nuclides or cancel this cell to be burnable.\n", nCellIndexU, cMaterial.p_OMatSetIndex.GetIndexU(nMatIndex));}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}
	return;
}