# include "Material.h"

void CDMaterial::DopplerBroaden(CDAceData& cAceData, vector<CDSingleCell>& vCells)
{

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"===================== Cross-section doppler broaden ====================\n") ;},
		Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	int nCount = 0;
	for (int i = 0; i < cAceData.p_vNuclides.size(); i++)
	{
		cAceData.p_vNuclides[i].p_dNucBroadTmp = cAceData.p_vNuclides[i].p_dNucTmp;
	}
	//AceData.NucBroadTmp=AceData.NucTmp;
	cAceData.CalcThermGfun();
	/////// adjust elastic and total cross-section //////////
	int nTotCellNum = vCells.size() - 1 ;
	for(int nNuc = 1;nNuc <= GetTotUsedNucNumExcSab();nNuc++)
	{
		double dTmp = 0;
		int np = 0;
		for(int k = 1;k <= nTotCellNum;k++)
		{
			//m = vCells[k].MatIndex;
			int nStartIndex;
			int nEndIndex;

			if(vCells[k].p_bIsExpdMat)
			{
				nStartIndex= vCells[k].p_nLatMatIndex;
				nEndIndex = nStartIndex + vCells[k].p_nLatMatNum -1;
			}
			else
			{
				nStartIndex = vCells[k].p_nMatIndex;
				nEndIndex = nStartIndex;
			}
			for(int m = nStartIndex ; m <= nEndIndex ; ++m )
			{
				if( m == 0 ){continue;}
				for(int j = 0; j < GetMatTotNucNum(m);j++)
				{
					if(nNuc == GetMatNucIndex(m,j))
					{
						if(np==0) //// the first temperature selected
						{
							dTmp = vCells[k].p_dTemp;
							np = 1;
						}  
						if( fabs(dTmp-vCells[k].p_dTemp) > 0.01*dTmp )  
						{
							dTmp = 0;
							goto go_ad;
						} // two different temperature
					}
				}
			}
		}

go_ad:
		nCount = nCount + cAceData.DpplrBrdnNucTmp(nNuc, dTmp);
	}

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Doppler broaden applied to %d nuclide.\n",nCount);},
		Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"========================================================================\n");},
		Output.p_fpMatFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);
}