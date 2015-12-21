# include "Tally.h"
# include "Geometry.h"

void CDTally::GenTallyCellMap(CDGeometry &cGeometry)
{

	if(!p_bUseTallyCellMap)
	{
		return;
	}

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Cell-mapping method is used to accelerate tally treatment.\n");},
		Output.p_fpOutputFilePtr,CDOutput::_FILE,CDOutput::_NORMAL);

	/////////////////// Only Run once for burnup case ///////////////////
	static int nRunOnce = 0;
	if(nRunOnce == 1){return;}
	nRunOnce = 1 ;

	//////////////////// Generate cell map for cell tally //////////////////////////
	///////////////// Note: each CellTally has its own map /////////////////////
	if(p_bIsWithCellTally)
	{
		for(int i = 0 ; i < p_vCellTally.size() ; ++i )
		{
			int nCellSize = p_vCellTally[i].p_vCellVecListU.size();

			///// Generate cell map
			p_vCellTally[i].p_OCellMap.Resize(nCellSize);
			for(int j = 0 ; j < nCellSize ; ++j)
			{
				long long nKey = cGeometry.GetMapKeyWord(p_vCellTally[i].p_vCellVecListU[j]);
				p_vCellTally[i].p_OCellMap.AddMapPair(j,nKey,j);
			}
			p_vCellTally[i].p_OCellMap.ResortMapKey();

			///// Generate cell bin map
			if(p_bUseTallyCellBinMap)
			{
				vector<double> dKeyArray;
				dKeyArray.resize(nCellSize);
				double dTemp = 1.0/double(p_vCellTally[i].p_OCellMap.p_vKeyValPair[0].Key);
				p_vCellTally[i].p_OCellBinMap.p_nHashType = 2;
				p_vCellTally[i].p_OCellBinMap.p_dScale = dTemp;
				p_vCellTally[i].p_OCellBinMap.p_nFraNum = 10000;
				p_vCellTally[i].p_OCellBinMap.p_dBound_a = 1;
				p_vCellTally[i].p_OCellBinMap.p_dBound_b = 
					double(p_vCellTally[i].p_OCellMap.p_vKeyValPair[nCellSize - 1].Key) * dTemp;

				//printf("CellSize = %d a = %16.15f, b = %16.15f\n", CellSize,CellTally[i].CellBinMap.Bound_a, CellTally[i].CellBinMap.Bound_b);
		
				for(int j = 0 ; j < nCellSize ; ++j)
				{
					dKeyArray[j] =  double(p_vCellTally[i].p_OCellMap.p_vKeyValPair[j].Key) * dTemp;
				}
				p_vCellTally[i].p_OCellBinMap.GenBinMapOfArray(p_vCellTally[i].p_vCellBinPtr,&dKeyArray[0],0,nCellSize - 1);
			}
			//system("pause");
			///// Generate tally level flags
			int nLevel = p_vCellTally[i].p_nFlagLevel;
			if(nLevel >= 0)
			{
				for(int j = 0 ; j < nCellSize ; ++j)
				{
					int nCell = p_vCellTally[i].p_vCellVecListU[j][nLevel];

					if(nCell >=  p_vCellTally[i].p_vTallyLevelFlags.size())
					{
						p_vCellTally[i].p_vTallyLevelFlags.resize(nCell + 1);
					}
					p_vCellTally[i].p_vTallyLevelFlags[nCell] = 1;
				}
			}
		}	
	}

	//////////////////// Generate cell map for OneGroupCsTally ////////////////////
	///////////////// Note: all OneGroupCsTally share one map /////////////////////
	if(p_bIsWithCsTally)
	{
		CsTallyCellIndex.clear();

		int nCsTallyNum = p_vOneGroupCsTally.size();;
		for(int i = 0;i < nCsTallyNum; ++i)
		{
			long long nKey = cGeometry.GetMapKeyWord(p_vOneGroupCsTally[i].p_vCellVecU);

			if(CsTallyCellIndex.find(nKey) != CsTallyCellIndex.end()) 
			{  
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"cs tally in cell %s is redefined.\n",
					Output.PrintCellVec(p_vOneGroupCsTally[i].p_vCellVecU));}, Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
			CsTallyCellIndex[nKey] = i ;
		}
	}
}

long long CDGeometry::GetMapKeyWord(const vector<int> &vCellsU)
{
	long long llScale = 500;
	long long llMultiper = 1 ;
	long long llSum = 0 ;
	int nsize = vCellsU.size();

	for(int i = nsize - 1 ; i >= 0 ; --i)
	{
		llSum = llSum + vCellsU[i]*llMultiper;
		llMultiper = llMultiper * llScale;
	}

	return llSum;
}