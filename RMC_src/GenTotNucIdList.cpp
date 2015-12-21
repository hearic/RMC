# include "Material.h"

void CDMaterial::GenTotNucIdList(bool bIsMultiGroup)
{
	////////////  Generate total nuclide ID list: NucID //////////////////
	//////////       TotNucNumExcSab:  number of n libraries  	 //////////
    /////// TotNucNumIncSab:  number of n libraries and sab libraries  //////
	int nM = 1;
	m_nTotNucNumIncSab = 1;

	p_vNucIDList.resize( m_nTotNucNumIncSab + 1 ) ;
	p_vNucIDList[1] = GetMatNucID(nM,0) ; //

	////////////////////// Deal with n library //////////////////////
	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum;i++)    
	{
		for(int j = 0; j < GetMatTotNucNum(i); j++ )
		{
			int k;
			for(k = 1; k <= m_nTotNucNumIncSab; k++)
			{
				if( strcmp(p_vNucIDList[k].p_chIdStr,GetMatNucID(i,j).p_chIdStr) == 0 )
				{
					break;
				}
			}
			if(k > m_nTotNucNumIncSab)
			{
				m_nTotNucNumIncSab = m_nTotNucNumIncSab + 1;
				p_vNucIDList.push_back(GetMatNucID(i,j));
			}
		}
	}
	m_nTotUsedNucNumExcSab = m_nTotNucNumIncSab;

	///////////////////// Deal with sab library ////////////////////////
	m_nTotNucNumExcSab = m_nTotNucNumIncSab; 
	for(int i = 1;i <= nTotMatNum;++i)  // deal with sab library
	{
		if(GetMatTotSabNucNum(i)==0){continue;}
		
		for (int j = 0; j < GetMatTotSabNucNum(i) ; j++)
		{
			int k;
			for(k = m_nTotNucNumExcSab + 1;k <= m_nTotNucNumIncSab;++k)
			{
				//printf("%s %s\n",NucIDList[k].IdStr,GetMatSabNucID(i,j).IdStr);
				if( strcmp(p_vNucIDList[k].p_chIdStr,GetMatSabNucID(i,j).p_chIdStr) == 0 )
				{
					break;
				}
			}
			if(k > m_nTotNucNumIncSab)   
			{
				//printf("mat=%d, sabnum=%d k =%d/%d sab=%s\n",i,GetMatTotSabNucNum(i),k,TotNucNumIncSab,GetMatSabNucID(i,j).IdStr);
				m_nTotNucNumIncSab = m_nTotNucNumIncSab + 1;
				p_vNucIDList.push_back(GetMatSabNucID(i,j));
			}
		}
	}

	if(bIsMultiGroup && m_nTotNucNumExcSab != m_nTotNucNumIncSab)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"s(a,b) conflicts with multi-group ace data.\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
	}

}