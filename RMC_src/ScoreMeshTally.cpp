# include "Tally.h"
# include "Criticality.h"

void CDTally::ScoreMeshTally(CDAceData &cAceData, CDParticleState &cParticleState, CDMaterial &cMaterial)
{	
	vector<CDMeshTrack> MeshTrack;
		
	for(int n = 0;n < p_vMeshTally.size(); ++n)  // search all mesh tally list
	{		
        ////////////////// find scored meshes  ////////////////
		p_vMeshTally[n].p_OTallyMesh.CalcMeshTrck(cParticleState.p_dPos, cParticleState.p_dDir, p_dTrackLength, MeshTrack);

		//////////////////////// Add tally Score /////////////////////////
		double dScoreTemp = cParticleState.p_dWgt * CalcTallyScore(p_vMeshTally[n].p_nTallyType, cParticleState, cMaterial, cAceData);

		for(int m = 0; m < MeshTrack.size(); ++m)
		{
			double dTrackLen = MeshTrack[m].p_dTrackLen;
			int nMeshIndex = MeshTrack[m].p_nMeshIndex;
			double dTallyAdd = dScoreTemp * MeshTrack[m].p_dTrackLen;

			/////////// Judge energy-bin existence //////////////
			int nErgBinNum = p_vMeshTally[n].p_nErgBinSize;
			if(nErgBinNum > 0)
			{
				double dErg = cParticleState.p_dErg;
				if(cAceData.p_bIsMultiGroup)
				{
					int  k = cAceData.p_nMltGrpNum - int(dErg);
					dErg = cAceData.p_vMltCentErg[k];
				}

				int nMin = 0, nMax = nErgBinNum - 1;
				int j;// Energy bin position;
				CDGlobeFun::GetIntpltPos(&p_vMeshTally[n].p_vErgBins[0], dErg, nMin, nMax, j);

				int nPtr = p_vMeshTally[n].GetMeshErgPtr(nMeshIndex,j);
				p_OMeshTallyData.p_vScore[nPtr] = p_OMeshTallyData.p_vScore[nPtr] + dTallyAdd;

				if(p_bIsPerHstry)
				{
					p_OMeshTallyData.p_setScoreIndex.insert(nPtr);
				}
			}


			int nPtr = p_vMeshTally[n].GetMeshPtr(nMeshIndex);
			p_OMeshTallyData.p_vScore[nPtr] = p_OMeshTallyData.p_vScore[nPtr] + dTallyAdd;

			if(p_bIsPerHstry)
			{
				p_OMeshTallyData.p_setScoreIndex.insert(nPtr);
			}
		}
	}

	return ;
}
