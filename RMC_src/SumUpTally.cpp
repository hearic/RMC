# include "Criticality.h"
# include "Tally.h"

void CDTally::SumUpTally(int nFlag, CDCriticality &cCriticality)
{
	enum eTallyFlag{PerHstry = 0, PerCyc = 1};

	if(cCriticality.p_nCurrentCYCLE <= cCriticality.p_nInactCycNum)
	{
		return;
	}

	//////////////////////////////// sum tally of current history/////////////////////////////////////
	if(p_bIsWithCellTally) //// Only calculate in master processor
	{
		if(nFlag == PerHstry)
		{
			p_OCellTallyData.SumTallyBin(); // sum up tally by bin index
		}
		else if(nFlag == PerCyc)
		{
			p_OCellTallyData.SumTallyAll();
		}
	}

	if(p_bIsWithMeshTally) //// Only calculate in master processor
	{
		if(nFlag == PerHstry)
		{
			p_OMeshTallyData.SumTallyBin(); // sum up tally by bin index
		}
		else if(nFlag == PerCyc)
		{
			p_OMeshTallyData.SumTallyAll();
		}
	}

	if(p_bIsWithCsTally)
	{
		if(nFlag == PerHstry)
		{
			auto ScoreIndexTemp = p_OCsTallyData.p_setScoreIndex;
			p_OCsTallyData.p_setScoreIndex.clear();
			for(auto iter = ScoreIndexTemp.begin(); iter != ScoreIndexTemp.end();++ iter)
			{
				int i = *iter;
				int nPtr = p_vOneGroupCsTally[i].p_nDataStartPtr;
				int nLen = p_vOneGroupCsTally[i].p_nDataLen;
				p_OCsTallyData.p_vScoreIndex2.push_back(nPtr);
				p_OCsTallyData.p_vScoreStride.push_back(nLen);
			}
			p_OCsTallyData.SumTallyBinStride(); // sum up tally by bin index and length
		}
		else if(nFlag == PerCyc)
		{
			p_OCsTallyData.SumTallyAll();
		}
	}

	return;
}


void CDTally::SumUpTally(int Flag, CDFixedSource &cFixedSource)
{
	enum TallyFlag{PerHstry = 0, PerCyc = 1};

	//////////////////////////////// sum tally of current history/////////////////////////////////////
	if(p_bIsWithCellTally) //// Only calculate in master processor
	{
		if(Flag == PerHstry)
			p_OCellTallyData.SumTallyBin(); // sum up tally by bin index
	}

	if(p_bIsWithMeshTally) //// Only calculate in master processor
	{
		
		p_OMeshTallyData.SumTallyBin(); // sum up tally by bin index
		
	}

	if(p_bIsWithCsTally)
	{
		auto ScoreIndexTemp = p_OCsTallyData.p_setScoreIndex;
		p_OCsTallyData.p_setScoreIndex.clear();
		for(auto iter = ScoreIndexTemp.begin(); iter != ScoreIndexTemp.end();++ iter)
		{
			int i = *iter;
			int ptr = p_vOneGroupCsTally[i].p_nDataStartPtr;
			int len = p_vOneGroupCsTally[i].p_nDataLen;
			p_OCsTallyData.p_vScoreIndex2.push_back(ptr);
			p_OCsTallyData.p_vScoreStride.push_back(len);
		}
		p_OCsTallyData.SumTallyBinStride(); // sum up tally by bin index and length
	}

	return;
}