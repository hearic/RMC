# include "Criticality.h"
# include "Tally.h"

#include "Burnup.h"

void CDTally::InitiateTally(CDGeometry &cGeometry, CDAceData &cAceData, CDMaterial &cMaterial)
{
	
	//////////// variable for tallies ////////////
	if(p_bIsWithCellTally)
	{
		p_OCellTallyData.Initiate();    ////  Clear all cell tally data

		for(int i = 0 ; i < p_vCellTally.size() ; ++i )
		{
			p_vCellTally[i].SetupCellTally(cAceData,p_OCellTallyData);
		}

		p_OCellTallyData.SetZero();
	}

	if(p_bIsWithMeshTally)
	{
		p_OMeshTallyData.Initiate();    ////  Clear all cell tally data

		for(int i = 0 ; i < p_vMeshTally.size() ; ++i )
		{
			p_vMeshTally[i].SetupMeshTally(cAceData, p_OMeshTallyData);
		}

		p_OMeshTallyData.SetZero();
	}


	if(p_bIsWithCsTally)
	{
		p_OCsTallyData.Initiate();    ////  Clear cs cell tally data

		if(p_bUseUnionCsTally)
		{
			cAceData.GenUnionErgGrid(cMaterial);
		}

		int nCsTallyNum = p_vOneGroupCsTally.size();;
		for(int i = 0;i < nCsTallyNum; ++i)
		{
			p_vOneGroupCsTally[i].SetupCsTally(cAceData, cMaterial, cGeometry,p_OCsTallyData, p_bUseUnionCsTally);
		}

		p_OCsTallyData.SetZero();
	}


	/////////////// use map to locate tally cell position ///////////////
	GenTallyCellMap(cGeometry);

	return;
}