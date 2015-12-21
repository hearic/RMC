# include "Input.h"

bool CDInput::SI_ReadTallyCells(vector<vector<int> > &vTallyCells,bool &bIsBlockEnd, bool &bIsCardEnd,CDGeometry &cGeometry)
{
	bIsCardEnd = false;
	bIsBlockEnd = false;        // flag used to judge block end

	vector<int> vStartCells;
	vector<int> vEndCells;
	vector<int> vTempCells;


	/////////////////////////////////// Read Tally Cells  //////////////////////////////
	vTallyCells.resize(0);
	vStartCells.resize(0);
	vEndCells.resize(0);

	for(;;)
	{
		/////////////////////////////// NO more cells ///////////////////////////////
		char nextchar;
		int nNext_p = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);
		/*if(nNext_p >= 2)   
		{
		bIsCardEnd = true;
		if(nNext_p >= 3){ bIsBlockEnd = true; }
		break;
		}*/
		if(nNext_p == 4||nextchar=='%'||nextchar=='$')   
		{
			bIsCardEnd = true;
		if(nNext_p == 4||nextchar=='%')   { bIsBlockEnd = true; }
			break;
		}

		//////////////////// read and add global-expanded cell  /////////////////////
		int nRead_i = -1;
		if( nextchar == '*')
		{
			CDFileIO::SkipChars(p_fpInputFilePtr,1);
			if(fscanf(p_fpInputFilePtr,"%d",&nRead_i) < 1 || nRead_i < 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect cell vector.\n");}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);
				return false;
			}

			vTempCells.resize(0);
			cGeometry.ExpdGlobalCell(vTallyCells,vTempCells,nRead_i,0);
			continue;
		}

		////////////// read simple cell(1>2>3) and multi cell(1>1:289>3) ///////////
		if(fscanf(p_fpInputFilePtr,"%d",&nRead_i) < 1)
		{
			break;
		}
		if(nRead_i < 0)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect cell vector.\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			return false;
		}

		int nNext_p1 = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);	
		int nRead_ii;
		if (nextchar == ':')  //// multi-cell case
		{
			CDFileIO::SkipChars(p_fpInputFilePtr,1);
			if(fscanf(p_fpInputFilePtr,"%d",&nRead_ii) < 1 || nRead_ii < 0 || nRead_ii <= nRead_i)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect cell vector.\n");}, 
					Output.p_fpOutputFilePtr, CDOutput::_WARNING);
				return false;
			}
		}
		else //// simple cell case
		{
			nRead_ii = nRead_i ;
		}

		vStartCells.push_back(nRead_i);   //// start of  multi-cell
		vEndCells.push_back(nRead_ii);    //// end of multi-cell 

		int nNext_p2 = CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar);	
		if (nextchar == '>')  //// Continue reading current cell
		{
			CDFileIO::SkipChars(p_fpInputFilePtr,1);
			continue;
		}

		//////////////////////// finish reading one cell input //////////////////////
		vTempCells = vStartCells ;

AddCell:
		////////////////////////// Add one simple cell /////////////////////////////
		vTallyCells.push_back(vTempCells);

		///////////////////// check and expand multi-cells /////////////////////////
		int nMaxLevel = vStartCells.size()-1;
		for(int i = nMaxLevel ; i >= 0 ; --i)
		{
			if( vTempCells[i] < vEndCells[i] )
			{
				++ vTempCells[i] ;
				for(int j = nMaxLevel ; j > i ; --j)
				{
					vTempCells[j] = vStartCells[j];  // reset to 1
				}
				goto AddCell;
			}
		}

		///////////////////////// prepare reading next cell ///////////////////////
		vStartCells.resize(0);
		vEndCells.resize(0);

		if(nNext_p1 >= 2 || nNext_p2 >= 2)   
		{
			bIsCardEnd = true;
			if(nNext_p1 >= 3 || nNext_p2 >= 3)
			{
				bIsBlockEnd = true;
			}
			break;
		} 
	}


	return true;
}