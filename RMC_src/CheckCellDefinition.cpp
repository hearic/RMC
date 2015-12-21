# include "Geometry.h"
# include "Boolean.h"
# include "Material.h"

void CDGeometry::CheckCellComplement()
{

    int nTotCellNum = p_vCell.size() - 1 ;
	for(int i = 1; i <= nTotCellNum; i++ )   // Cell[0] never be used
	{
		////////////////////////////  Expand boolean complement cells: !c1 !c2 /////////////////////////////	
		int nCell_index_u = p_OCellIndex.GetIndexU(i);


		int nPos = -1;
		int nEnd_pos = p_vCell[i].p_vBooleanSurfaces.size() - 1;
		for(;;)
		{
			nPos++;
			if(nPos > nEnd_pos)
			{
				break;
			}

			int nSurf_bool = p_vCell[i].p_vBooleanSurfaces[nPos];
			if(nSurf_bool == CDBoolean::_BOOLEAN_COMPLEMENT)
			{
				Output.CheckInputParas(nPos <= nEnd_pos - 1,
					[&](){sprintf(Output.p_sPrintStr,"unknown boolean complement in cell %d.\n",nCell_index_u);},2);

				int nNext_surf_bool = p_vCell[i].p_vBooleanSurfaces[nPos + 1];

				/////  complement of surfaces
				if(nNext_surf_bool == CDBoolean::_LEFT_PARENTHESIS)  
				{
					continue;
				}

				/////  complement of cell
				Output.CheckInputParas(!CDBoolean::IsBooleanOperator(nNext_surf_bool),
					[&](){sprintf(Output.p_sPrintStr,"unknown boolean complement in cell %d.\n",nCell_index_u);},2);

				int nComplement_cell_index_u = nNext_surf_bool;
				int nComplement_cell_index = p_OCellIndex.GetIndex(nComplement_cell_index_u) ;
				Output.CheckInputParas(nComplement_cell_index > 0 ,[&](){sprintf(Output.p_sPrintStr,
					"Complement cell %d in Cell %d not defined!\n",nComplement_cell_index,nCell_index_u);},2);

				
				p_vCell[i].p_vBooleanSurfaces.erase(p_vCell[i].p_vBooleanSurfaces.begin() + nPos + 1);
				p_vCell[i].p_vBooleanSurfaces.insert(p_vCell[i].p_vBooleanSurfaces.begin() + nPos + 1, CDBoolean::_LEFT_PARENTHESIS);
				p_vCell[i].p_vBooleanSurfaces.insert(p_vCell[i].p_vBooleanSurfaces.begin() + nPos + 2, CDBoolean::_RIGHT_PARENTHESIS);

				p_vCell[i].p_vBooleanSurfaces.insert(p_vCell[i].p_vBooleanSurfaces.begin() + nPos + 2, 
					p_vCell[nComplement_cell_index].p_vBooleanSurfaces.begin(),p_vCell[nComplement_cell_index].p_vBooleanSurfaces.end());

				///////// check booleans/surfaces size /////////////
				if(p_vCell[i].p_vBooleanSurfaces.size() > 10000)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Incorrect booleans/surfaces in Cell %d.\n",nCell_index_u);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}

				nEnd_pos = p_vCell[i].p_vBooleanSurfaces.size() - 1;
				continue ;
			}
		}



		////////////////////////////  Treat boolean surfaces: !(s1 s2 s3) /////////////////////////////
		nPos = -1;
		nEnd_pos = p_vCell[i].p_vBooleanSurfaces.size() - 1;
		for(;;)
		{
			nPos++;
			if(nPos > nEnd_pos)
			{
				break;
			}

			int surf_bool = p_vCell[i].p_vBooleanSurfaces[nPos];
			if(surf_bool != CDBoolean::_BOOLEAN_COMPLEMENT)
			{
				continue;
			}

			p_vCell[i].p_vBooleanSurfaces.erase(p_vCell[i].p_vBooleanSurfaces.begin() + nPos);
			nPos = nPos - 1;
			nEnd_pos = p_vCell[i].p_vBooleanSurfaces.size() - 1;

			int nLeft_parenth_count = 0;
			int nRight_parenth_count = 0;
			for(;;)
			{
				nPos++;
				if(nPos > nEnd_pos)
				{
					//pos = -1;
					break;
				}

				int nSurf_bool_in_complement = p_vCell[i].p_vBooleanSurfaces[nPos];

				if(nSurf_bool_in_complement == CDBoolean::_LEFT_PARENTHESIS)
				{
					nLeft_parenth_count = nLeft_parenth_count + 1;
				}
				else if(nSurf_bool_in_complement == CDBoolean::_RIGHT_PARENTHESIS)
				{
					nRight_parenth_count = nRight_parenth_count + 1;

					if(nRight_parenth_count == nLeft_parenth_count)
					{
						//pos = -1;
						break;
					}
				}
				else if(nSurf_bool_in_complement == CDBoolean::_BOOLEAN_INTERSECTION)
				{
					p_vCell[i].p_vBooleanSurfaces[nPos] = CDBoolean::_BOOLEAN_UNION;
				}
				else if(nSurf_bool_in_complement == CDBoolean::_BOOLEAN_UNION)
				{
					p_vCell[i].p_vBooleanSurfaces[nPos] = CDBoolean::_BOOLEAN_INTERSECTION;
				}
				else if(nSurf_bool_in_complement != CDBoolean::_BOOLEAN_COMPLEMENT)
				{
					p_vCell[i].p_vBooleanSurfaces[nPos]= - p_vCell[i].p_vBooleanSurfaces[nPos];
				}
			}

			nPos = -1;
		}
	}

	return;

}

void CDGeometry::CheckCellSurfaces()
{
	//////////////////// check cell surface and convert surface index ////////////////////
	int nTotCellNum = p_vCell.size() - 1 ;
	for(int i = 1; i <= nTotCellNum; i++ )   // Cell[0] never be used
	{
		
		p_vCell[i].p_vBoundSurf.resize(0);
		int nSurf_bool_num = p_vCell[i].p_vBooleanSurfaces.size() ;
		for(int j = 0;j < nSurf_bool_num ; j++ )
		{
			int nSurf_bool = p_vCell[i].p_vBooleanSurfaces[j];
			if(CDBoolean::IsBooleanOperator(nSurf_bool))
			{
				continue ;
			}

			int nSurf_index_u = abs(nSurf_bool);
			int nSurf_index = p_OSurfaceIndex.GetIndex(nSurf_index_u);
			Output.CheckInputParas(nSurf_index > 0 && nSurf_index <= p_OSurfaceIndex.GetIndexNum(),[&](){sprintf(Output.p_sPrintStr,
				"unknown surface %d defined in cell %d.\n",nSurf_index_u,p_OCellIndex.GetIndexU(i));},2);

			p_vCell[i].p_vBooleanSurfaces[j] = p_vCell[i].p_vBooleanSurfaces[j] > 0 ? nSurf_index : -nSurf_index ;

			if(std::find(p_vCell[i].p_vBoundSurf.begin(),p_vCell[i].p_vBoundSurf.end(),p_vCell[i].p_vBooleanSurfaces[j])  == p_vCell[i].p_vBoundSurf.end())
			{
				p_vCell[i].p_vBoundSurf.push_back(p_vCell[i].p_vBooleanSurfaces[j]);
			}
		}

	}
	return;
}


void CDGeometry::CheckCellSurfBooleans()
{
	//////////////////// check cell surface booleans //////////////////
	int nTotCellNum = p_vCell.size() - 1 ;
	for(int cc = 1; cc <= nTotCellNum; cc++ )   // Cell[0] never be used
	{
		int nCellU = p_OCellIndex.GetIndexU(cc);
		vector<int> vBoolExpression(p_vCell[cc].p_vBooleanSurfaces);

		///////////////// check size of BoolExpression /////////////////
		Output.CheckInputParas(vBoolExpression.size() > 0,
			[&](){sprintf(Output.p_sPrintStr,"undefined surface/booleans in cell %d.\n",nCellU);},2);


		//////////// check consistency of parentheses /////////////////
		int nLeftParanthCount = 0, nRightParanthCount = 0;
		for(int i = 0 ; i < vBoolExpression.size(); ++i)
		{
			if(vBoolExpression[i] == CDBoolean::_LEFT_PARENTHESIS)
			{
				nLeftParanthCount++;
			}
			else if(vBoolExpression[i] == CDBoolean::_RIGHT_PARENTHESIS)
			{
				nRightParanthCount++;
			}
			Output.CheckInputParas(nLeftParanthCount >= nRightParanthCount,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parentheses in surface/booleans in cell %d.\n",nCellU);},2);
		}
		Output.CheckInputParas(nRightParanthCount == nLeftParanthCount,
			[&](){sprintf(Output.p_sPrintStr,"incorrect parentheses in surface/booleans in cell %d.\n",nCellU);},2);



		//////////// check boolean character //////////////////////
		for(;;)
		{
			int nStartPos = 0;
			int nEndPos = vBoolExpression.size() - 1;
			bool bFind_parenth = false;

			for(int i = 0 ; i < vBoolExpression.size() ; ++i)
			{
				if(vBoolExpression[i] == CDBoolean::_LEFT_PARENTHESIS)
				{
					nStartPos = i;
				}
				else if(vBoolExpression[i] == CDBoolean::_RIGHT_PARENTHESIS)
				{
					nEndPos = i;
					bFind_parenth = true;
					break;
				}
			}
						
			///////// check the surface&booleans between of a pair of parentheses ////////
			int nStart_check_pos = nStartPos, end_check_pos = nEndPos;
			if(bFind_parenth) 
			{
				nStart_check_pos = nStart_check_pos + 1;   // exclude left parenthsis
				end_check_pos = end_check_pos - 1;       // exclude right parenthsis
			}
			int nSurf_count = 0, bool_count = 0;
			for(int j = nStart_check_pos ; j <= end_check_pos ; ++j)
			{
				int nSurf_bool = vBoolExpression[j];
				if(CDBoolean::IsBooleanOperator(nSurf_bool))
				{
					bool_count++;
				}
				else
				{
					nSurf_count++;
				}
				Output.CheckInputParas(nSurf_count == bool_count || nSurf_count == bool_count + 1,
					[&](){sprintf(Output.p_sPrintStr,"Incorrect booleans/surfaces in cell %d.\n",nCellU);},2);
			}
			Output.CheckInputParas(nSurf_count - 1 == bool_count,
				[&](){sprintf(Output.p_sPrintStr,"Incorrect booleans/surfaces in cell %d.\n",nCellU);},2);

			/////////////////////////  recursion  //////////////////////
			if(!bFind_parenth)
			{
				break;
			}
			else
			{
				vBoolExpression.erase(vBoolExpression.begin()+nStartPos,vBoolExpression.begin()+nEndPos+1);
				vBoolExpression.insert(vBoolExpression.begin()+nStartPos,1);
				continue;
			}
		}

		////////////////////////////  Generate Parentheses positions ////////////////
		CDBoolean::GenParenthPos(p_vCell[cc].p_vBooleanSurfaces,p_vCell[cc].p_vParenthPos);

	}

	return;
}


void CDGeometry::CheckCellInfo(CDMaterial &cMaterial)
{
	////////////////////// Convert the user index into the system Index : Mat/Surf/Univ @ Cell ////////////////////
	int nTotCellNum = p_vCell.size() - 1 ;
	int nVoidCellNum = 0;
	for(int i = 1; i <= nTotCellNum; i++ )   // Cell[0] never be used
	{
		///////////// count void cell
		if (p_vCell[i].p_nImp == 0)
		{
			nVoidCellNum++;
		}		

		///////////////// convert filled material index ///////////////////
		if(p_vCell[i].p_nMatIndexU != 0) 
		{
			int mat_index_u = p_vCell[i].p_nMatIndexU;
			int mat_index = cMaterial.p_OMatSetIndex.GetIndex(mat_index_u);

			Output.CheckInputParas(mat_index > 0 ,[&](){sprintf(Output.p_sPrintStr,
				"unknown material %d defined in cell %d.\n",mat_index_u,p_OCellIndex.GetIndexU(i));},2);

			p_vCell[i].p_nMatIndex = mat_index;
		}
/*
		else // p_nMatIndexU = 0
		{
			Output.CheckInputParas(p_vCell[i].p_nFillUnivIndexU != -1 || p_vCell[i].p_bIsExpdMat,[&](){sprintf(Output.p_sPrintStr,
				"no material defined in bottom cell %d.\n",p_OCellIndex.GetIndexU(i));},2);
		}
*/

		///////////////// convert filled universe index ///////////////////
		int nUniv_index_u = p_vCell[i].p_nFillUnivIndexU ;
		if(nUniv_index_u == 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"universe 0 can not be filled.\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
		else if (nUniv_index_u > 0)
		{
			int nUniv_index = p_OUniverseIndex.GetIndex(nUniv_index_u) ;
			int nTot_univ_num = p_vUniverse.size();
			Output.CheckInputParas(nUniv_index > 0 && nUniv_index <= nTot_univ_num,[&](){sprintf(Output.p_sPrintStr,
				"unknown universe %d is filled in cell %d.\n",nUniv_index_u,p_OCellIndex.GetIndexU(i));},2);

			p_vCell[i].p_nFillUnivIndex = nUniv_index ;
		}
	}

	// check if void cell defined
	Output.CheckInputParas(nVoidCellNum > 0,
		[&](){sprintf(Output.p_sPrintStr,"no void cell(VOID = 1) defined! It is recommended to check geometry. Transport of particles may go wrong if there is undefined space.\n");},CDOutput::_WARNING);
	Output.CheckInputParas(nVoidCellNum < nTotCellNum ,
		[&](){sprintf(Output.p_sPrintStr,"all cells are defined to be void! It is recommended to check geometry.\n");},CDOutput::_ERROR);

	return;
}