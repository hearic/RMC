# include "Input.h"

void CDInput::ReadCellSurfBool(int nCell_index,bool &bIsBlockEnd,bool &bIsCardEnd,CDGeometry &cGeometry)
{
	bIsBlockEnd = false;
	bIsCardEnd = false;

	//auto &Cell = Geometry.Cell; //CGeometry.Cell
	int nCell_index_u = cGeometry.p_OCellIndex.GetIndexU(nCell_index);  
	for(;;)
	{
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		if(nNext_p >= 2)   //current universe or cell finished
		{
			if(nNext_p >= 3)
			{
				bIsBlockEnd = true;
			}
			else
			{
				bIsCardEnd = true;
			}
			return;
		}   

		int nCell_surf_u;

		if(isdigit(nextchar) || nextchar == '-' || nextchar == '+')    //surface
		{
			Output.CheckInputParas(fscanf(p_fpInputFilePtr,"%d",&nCell_surf_u) > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect cell surface/boolean in cell %d.\n",nCell_index_u);},2);
			Output.CheckInputParas(!CDBoolean::IsBooleanOperator(nCell_surf_u) ,
				[&](){sprintf(Output.p_sPrintStr,"surface %d  in cell %d is not allowed.\n",nCell_surf_u,nCell_index_u);},2);

			cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(nCell_surf_u);
			//Cell[cell_index].BoundSurf.push_back(cell_surf_u);
		}
		else            // bool operator
		{
			if(nextchar == '('){cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(CDBoolean::_LEFT_PARENTHESIS);}
			else if(nextchar == ')'){cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(CDBoolean::_RIGHT_PARENTHESIS);}
			else if(nextchar == '&'){cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(CDBoolean::_BOOLEAN_INTERSECTION);}
			else if(nextchar == ':'){cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(CDBoolean::_BOOLEAN_UNION);}
			else if(nextchar == '!'){cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.push_back(CDBoolean::_BOOLEAN_COMPLEMENT);}
			else        // NOT bool operator : such as imp, tmp ,vol .....
			{
				break;
			}   
			CDFileIO::SkipChars(p_fpInputFilePtr,1);
		}
	}

		Output.CheckInputParas(cGeometry.p_vCell[nCell_index].p_vBooleanSurfaces.size() > 0,
		[&](){sprintf(Output.p_sPrintStr,"unkown surface/booleans in cell %d.\n",nCell_index_u);},2);
	return;

}