# include "Material.h"

void CDMaterial::ConvertMatNucDen(vector<CDNuclide>& vNuclides)
{
	// convert atom fraction and mass fraction.
	vector<double> vMatNucGramDen; // mass density (g/cm^3), temp variable

	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum;i++)
	{
		int nNucNum = GetMatTotNucNum(i);
		p_vMatSet[i].p_vMatNucAtomDen.resize(nNucNum);
		vMatNucGramDen.resize(nNucNum);
		double dSf = 0;
		double dSw = 0;
		for(int j = 0 ; j < nNucNum; j++ )
		{
			int nNuclide=GetMatNucIndex(i,j);
			double dA=vNuclides[nNuclide].p_dNucAtomWgt;
			if(dA <= 0 || dA > 300)
			{	
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"abnormal atomic weight of nuclide %d.",nNuclide);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
			if( p_vMatSet[i].p_vMatNucUserDen[j] < 0)   //given mass fraction.
			{
				vMatNucGramDen[j] = -p_vMatSet[i].p_vMatNucUserDen[j];
				p_vMatSet[i].p_vMatNucAtomDen[j] = -p_vMatSet[i].p_vMatNucUserDen[j]/dA;    // convert mass fractions to atom fractions.
			}
			else  // given atom fractions
			{
				p_vMatSet[i].p_vMatNucAtomDen[j] = p_vMatSet[i].p_vMatNucUserDen[j];
				vMatNucGramDen[j] = p_vMatSet[i].p_vMatNucUserDen[j]*dA;
			}
			dSf = dSf + p_vMatSet[i].p_vMatNucAtomDen[j];
			dSw = dSw + vMatNucGramDen[j];  // namely, sw = sw+MatNucAtomDen[i][j]*a; 
		}  
		// normalize the atom fraction and mass fraction.
		for(int j = 0; j < nNucNum;j++) 
		{
			p_vMatSet[i].p_vMatNucAtomDen[j] = p_vMatSet[i].p_vMatNucAtomDen[j]/dSf;
			vMatNucGramDen[j] = vMatNucGramDen[j]/dSw;
		}
		//  density of material:   den*sw/sf=rho*avgdn
		if( p_vMatSet[i].p_dMatUserDen<0)    //  mass density given
		{
			p_vMatSet[i].p_dMatGramDen = - p_vMatSet[i].p_dMatUserDen;
			p_vMatSet[i].p_dMatAtomDen = -p_vMatSet[i].p_dMatUserDen*(p_dAvgdn*dSf/dSw);
		}
		else
		{
			p_vMatSet[i].p_dMatAtomDen = p_vMatSet[i].p_dMatUserDen;
			p_vMatSet[i].p_dMatGramDen = p_vMatSet[i].p_dMatAtomDen/(p_dAvgdn*dSf/dSw);
		}

		// densities of nuclides
		for(int j = 0 ; j < nNucNum; j++ )
		{
			vMatNucGramDen[j] = vMatNucGramDen[j] * p_vMatSet[i].p_dMatGramDen;
			p_vMatSet[i].p_vMatNucAtomDen[j] = p_vMatSet[i].p_vMatNucAtomDen[j] * p_vMatSet[i].p_dMatAtomDen;
		}
	}

	ClearMatNucUserInput();
	
	return;
}