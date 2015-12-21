# include "Geometry.h"
# include "Boolean.h"
# include "Material.h"

void CDGeometry::ExpandLatMatInput(CDMaterial &cMaterial)
{
	vector<vector<int> > vAllExpdCells;
	vector<vector<int> > vTempExpdCells;
	vector<int> vTempCells;

	if (p_vLatMatInputCell.size() != p_vLatMatIndexU.size() )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," wrong lattice material input.\n");}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	for(int i = 0;i < p_vLatMatInputCell.size(); ++i)
	{
		int nCellIndex = p_vLatMatInputCell[i] ;
		int nCellIndexU = p_OCellIndex.GetIndexU(nCellIndex);

		if(p_vCell[nCellIndex].p_nFillUnivIndex > 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"non-bottom CELL %d used for lattice material input.\n", nCellIndexU);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		///////////////////////// expand cell material ////////////////////////////
		vector<int> nMatIndex;
		for (int i_mat = 0; i_mat < p_vLatMatIndexU[i].size(); i_mat++)
		{
			int nMatIndex_temp = cMaterial.p_OMatSetIndex.GetIndex(p_vLatMatIndexU[i][i_mat]);
			Output.CheckInputParas(nMatIndex_temp > 0 ,[&](){sprintf(Output.p_sPrintStr,
				"unknown material %d defined in lattice material input of cell %d.\n",nMatIndex_temp,nCellIndexU);},2);

			nMatIndex.push_back(nMatIndex_temp);
		}

		vTempCells.resize(0);
		vTempExpdCells.resize(0);
		ExpdGlobalCell(vTempExpdCells,vTempCells,nCellIndexU,0);

		if (nMatIndex.size() != vTempExpdCells.size())
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,
				"number of lattice materials input for CELL %d is not consistent with number of expanded cells, which should be %d.\n", nCellIndexU, vTempExpdCells.size());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		int nExpdCellNum = vTempExpdCells.size();

		if(nExpdCellNum > 1)
		{
			p_vCell[nCellIndex].p_bIsExpdMat = true ;
			p_vCell[nCellIndex].p_nLatMatNum = nExpdCellNum ;

			p_vCell[nCellIndex].p_nLatMatIndex = nMatIndex[0] ; // first lat mat index
			for(int j_cell = 0 ; j_cell < nExpdCellNum ; ++j_cell )
			{
				long long llKey = GetMapKeyWord(vTempExpdCells[j_cell]);
				if(p_vCell[nCellIndex].LatMatMap.find(llKey) != p_vCell[nCellIndex].LatMatMap.end()) 
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to generate material map for lattice input. Cell index may be too large!");}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				p_vCell[nCellIndex].LatMatMap[llKey] = nMatIndex[j_cell] ;
			}			
		}
	}
	if (p_vLatMatInputCell.size() > 0)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Lattice material input method is used.\n");},
			Output.p_fpOutputFilePtr,CDOutput::_NORMAL);
	}
}
