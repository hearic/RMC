# include "Material.h"

void CDMaterial::OutputMaterialFile(vector<CDNuclide>& vNuclides)
{
	const double dAneut  = 1.008664967;                   //  Neutron mass in a.m.u..
	const double dAvogad = 6.022043446928244e+23;         //  Avogadro's number.
	const double dAvgdn  = 1.0E-24*dAvogad/dAneut ;       //  1.e-24*Avogadro's number/neutron mass.

	// convert AtomDen to GramDen, UserDen == AtomDen
	// mass = atom * A / dAvgdn

	////////////////////////// output material information ////////////////////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	if(!Output.p_bIsMatPrint)
	{
		fprintf(Output.p_fpMatFilePtr,"\nMaterial printing is switched off.\n") ;
		return;
	}

	bool bIsPrintUserDen = false;
	if (!p_vMatSet[1].p_vMatNucUserDen.empty()) // UserDen are not cleared yet
	{
		bIsPrintUserDen = true;
	}

	fprintf(Output.p_fpMatFilePtr,"====================== Material Information Start ======================\n") ;
	fprintf(Output.p_fpMatFilePtr,"Material    Nuclide       Mass Density     Atom Density") ;
	if (bIsPrintUserDen)
	{
		fprintf(Output.p_fpMatFilePtr,"     User Input") ;
	}
	fprintf(Output.p_fpMatFilePtr,"\n") ;
	fprintf(Output.p_fpMatFilePtr,"                           (g/cm^3)      (10^24atom/cm^3)\n") ;

	int nTotMatNum = GetTotMatNum();
	for(int i = 1;i <= nTotMatNum; i++)
	{
		fprintf(Output.p_fpMatFilePtr,"%-4d          Sum         %.5E      %.5E",
			p_OMatSetIndex.GetIndexU(i),GetMatGramDen(i),GetMatAtomDen(i));
		if (bIsPrintUserDen) 
			fprintf(Output.p_fpMatFilePtr,"      %.5E",GetMatUserDen(i));
		fprintf(Output.p_fpMatFilePtr,"\n") ;
		for(int j = 0; j < GetMatTotNucNum(i); j++)
		{
			int nNuclide = GetMatNucIndex(i,j);
			fprintf(Output.p_fpMatFilePtr,"           %-10s     %.5E      %.5E",
				vNuclides[nNuclide].p_ONucID.p_chIdStr,GetMatNucGramDen(i,j,vNuclides),GetMatNucAtomDen(i,j));
			if (bIsPrintUserDen) 
				fprintf(Output.p_fpMatFilePtr,"      %.5E",GetMatNucUserDen(i,j));
			fprintf(Output.p_fpMatFilePtr,"\n");
		}
	}
	fprintf(Output.p_fpMatFilePtr,"======================= Material Information End =======================\n\n");

	return;
}