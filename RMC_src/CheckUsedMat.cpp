# include "Material.h"
# include "Geometry.h"

void CDMaterial::CheckUsedMat(CDGeometry &cGeometry)
{
	static vector<int> vMat_used_flag(GetTotMatNum() + 1);

	int nTotCellNum = cGeometry.p_vCell.size() - 1 ;
	for(int i = 1;i <= nTotCellNum;i++)
	{
		int nMat_index = cGeometry.p_vCell[i].p_nMatIndex ;
		vMat_used_flag[nMat_index] = 1 ;
	}

	for (int i = 0;i < cGeometry.p_vLatMatInputCell.size(); ++i)
	{
		for (int i_mat = 0; i_mat < cGeometry.p_vLatMatIndexU[i].size(); i_mat++)
		{
			int nMatIndex_temp = p_OMatSetIndex.GetIndex(cGeometry.p_vLatMatIndexU[i][i_mat]);
			//check lattice material used or not
			if (vMat_used_flag[nMatIndex_temp])
			{
				int nMatIndexU = cGeometry.p_vLatMatIndexU[i][i_mat];
				int nCellIndexU = cGeometry.p_OCellIndex.GetIndexU(cGeometry.p_vLatMatInputCell[i]);
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr," material %d in lattice cell %d is already used in other cell.\n",
					nMatIndexU,nCellIndexU);}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

			vMat_used_flag[nMatIndex_temp] = 1 ;
		}		 
	}

	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum;i++)
	{
		if(vMat_used_flag[i]==0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"material %d is not used in any cell.\n",
				p_OMatSetIndex.GetIndexU(i));},Output.p_fpOutputFilePtr, CDOutput::_FILE,CDOutput::_WARNING);
		}
	}

	return;
}