# include "Criticality.h"
# include "Tally.h"


void CDTally::TallyByTL(double dTrackLen, CDGeometry &cGeometry, CDAceData &cAceData, CDCriticality &cCriticality, CDParticleState &cParticleState, CDMaterial &cMaterial)
{
	if(cCriticality.p_nCurrentCYCLE <= cCriticality.p_nInactCycNum)
	{
		return;
	}

	if(p_bIsWithCellTally || p_bIsWithCsTally || p_bIsWithMeshTally)
	{
		p_dTrackLength = dTrackLen;

		if(p_bIsWithCellTally)
		{
			TallyCellByTL(cGeometry, cParticleState,cAceData,cMaterial);
		}

		if(p_bIsWithMeshTally)
		{
			ScoreMeshTally(cAceData, cParticleState, cMaterial);
		}

		if(p_bIsWithCsTally)
		{
			TallyCsByTL(cMaterial, cGeometry, cAceData, cParticleState);	
		}



	}



	return;
	
}


void CDTally::TallyByTL(double dTrackLen, CDGeometry &cGeometry, CDAceData &cAceData, CDFixedSource &cFixedSource, CDParticleState &cParticleState, CDMaterial &cMaterial)
{
	/*if(cCriticality.p_nCurrentCYCLE <= cCriticality.p_nInactCycNum)
	{
	return;
	}*/

	if(p_bIsWithCellTally || p_bIsWithCsTally || p_bIsWithMeshTally)
	{
		p_dTrackLength = dTrackLen;

		if(p_bIsWithCellTally)
		{
			TallyCellByTL(cGeometry, cParticleState,cAceData,cMaterial);
		}

		if(p_bIsWithMeshTally)
		{
			ScoreMeshTally(cAceData, cParticleState, cMaterial);
		}

		if(p_bIsWithCsTally)
		{
			TallyCsByTL(cMaterial, cGeometry, cAceData, cParticleState);	
		}



	}



	return;

}




