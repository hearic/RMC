# include "Material.h"
# include "Burnup.h"

void CDMaterial::InitiateMatAce(CDGeometry & CGeometry,CDTally &CTally,CDAceData& CAceData,vector<CDNuclide>& vNuclides, vector<CDSingleCell>& vCells, CDBurnup& cBurnup)
{	
	///// treat inp information before reading ACE /////////
	GenTotNucIdList(CAceData.p_bIsMultiGroup);
	CAceData.InitiateAce(p_vNucIDList);
	ConvertMatNucIndex(vNuclides);
		
	///// Read ACE libraries/////////
	CAceData.ReadAceData(GetTotNucNumIncSab(), GetTotNucNumExcSab());


	////////// use binary map to accelerate energy-grid searching /////////
	CAceData.GenErgBinMap(*this);

	
	////////// material treatment /////////
	CheckSabMaterial(CAceData);

	if (cBurnup.p_bIsBurnStepBegun)
	{
		cBurnup.UpdateBurnedMatNucDen(vNuclides, CGeometry, *this, CTally);// Update atom density in Burnup feedback 
	}
	else
	{
		ConvertMatNucDen(vNuclides);// convert gram/atom density from UserDen
	}

	if(cBurnup.p_bIsWithBurnup)
	{
		cBurnup.ExpdBurnableCell(CGeometry,*this,CTally,CAceData);
	}

	OutputMaterialFile(vNuclides);

	if(!CAceData.p_bIsMultiGroup)
	{
		DopplerBroaden(CAceData,vCells); 
	}
		
	return;
}