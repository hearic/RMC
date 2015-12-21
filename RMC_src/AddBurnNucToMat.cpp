# include "Burnup.h"

void CDBurnup::AddBurnNucToMat(int nNucId, double nNucDen, CDSingleMat &mat_struct)
{
	//// convert nuclide id
	if( nNucId%10 ==1 )    // excited-state nuclide
	{
		nNucId = nNucId/10 + 400 ;
	}
	else
	{
		nNucId = nNucId/10;
	}

	//// add nuclide id and density
	char chNucStr[12];
	sprintf(chNucStr,"%d",nNucId);
	CDNucId nNucIdAce;
	strcpy(nNucIdAce.p_chIdStr,strcat(chNucStr, p_chAceLibForBurn));

	mat_struct.p_vMatNucID.push_back(nNucIdAce);
	mat_struct.p_vMatNucAtomDen.push_back(nNucDen);

	return;
}