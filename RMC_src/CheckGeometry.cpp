# include "Geometry.h"

void CDGeometry::CheckGeometry(CDMaterial &cMaterial)
{
	//////////////////////////////////// Check surface parameters ///////////////////////////////////////////
	for(int i = 1; i < p_vSurface.size();++i)
	{
		p_vSurface[i].CheckSurfParas();
	}

	///////////////////////////////////// Check Universe  //////////////////////////////////////////////////
	CheckUniverse();


	///////////////////////////////// check and convert cell definition //////////////////////////////////
	CheckCellComplement();
	CheckCellSurfBooleans();
	CheckCellSurfaces();

	////////////////////////////////// Construct Lattice material ///////////////////////////////
	ExpandLatMatInput(cMaterial);

	////////////////////////////////// Convert and convert  Mat/Univ @ Cell ///////////////////////////////
	CheckCellInfo(cMaterial);

	//////////////////////  Build neighbor cell list ///////////////////////
	BuildNeiCellList();

	return;
}

void CDGeometry::CheckUniverse()
{

   ///////////////////////////////////// Check Universe 0 //////////////////////////////////////////////////
	Output.CheckInputParas(p_vUniverse[0].p_nContainCellNum > 0 ,
		[&](){sprintf(Output.p_sPrintStr,"Universe 0 is not defined!\n");},2);


	////////////////////// Convert the user index into the system Index : Cell @ Universe ////////////////////
	int nTotUnivNum = p_vUniverse.size();
	for(int i = 0 ; i < nTotUnivNum ; ++i )
	{
		p_vUniverse[i].p_vFillCellsIndex.resize(p_vUniverse[i].p_nContainCellNum);
		for (int j = 0 ; j < p_vUniverse[i].p_nContainCellNum ; ++j )
		{
			int nCell_index_u = p_vUniverse[i].p_vFillCellsIndexU[j] ;
			int nCell_index = p_OCellIndex.GetIndex(nCell_index_u) ;

			Output.CheckInputParas(nCell_index > 0 ,[&](){sprintf(Output.p_sPrintStr,
				"Cell %d in universe %d not defined!\n",nCell_index_u,p_OUniverseIndex.GetIndexU(i));},2);

			p_vUniverse[i].p_vFillCellsIndex[j] = nCell_index;
		}

		//////////////// convert fillings in lattice ///////////// 
		if (p_vUniverse[i].p_bIsLattice)
		{
			p_vUniverse[i].p_vFillLatUnivIndex.resize(p_vUniverse[i].p_nFilledLatNum);
			for (int j = 0 ; j < p_vUniverse[i].p_nFilledLatNum ; ++j )
			{
				int nUniv_index_u = p_vUniverse[i].p_vFillLatUnivIndexU[j] ;
				int nUniv_index = p_OUniverseIndex.GetIndex(nUniv_index_u) ;

				Output.CheckInputParas(nUniv_index > 0 ,[&](){sprintf(Output.p_sPrintStr,
					"unknown universe %d is filled in universe %d.\n",nUniv_index_u,p_OUniverseIndex.GetIndexU(i));},2);

				p_vUniverse[i].p_vFillLatUnivIndex[j] = nUniv_index;
			}
		}
	}
	return;
}
