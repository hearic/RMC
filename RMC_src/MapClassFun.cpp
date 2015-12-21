# include "MapClass.h"

void CDLongIntMap::Resize(int nSize)
{
	p_vKeyValPair.resize(nSize);
}

int CDLongIntMap::GetMapSize()
{
	return p_vKeyValPair.size();
}

long long CDLongIntMap::ConvertMapKey(const vector<int> &vec)
{
	long long nScale = 10000;
	long long nMultiper = 1 ;
	long long nSum = 0 ;
	int nSize = vec.size();

	for(int i = nSize-1 ; i >= 0 ; --i)
	{
		nSum = nSum + vec[i]*nMultiper;
		nMultiper = nMultiper * nScale;
	}

	return nSum;
}

void CDLongIntMap::AddMapPair(int i,long long nKey,int nVal)
{
	p_vKeyValPair[i].Key = nKey;
	p_vKeyValPair[i].Val = nVal;
}

bool less_key(const CDLonglongInt & m1, const CDLonglongInt & m2) 
{
	if(m1.Val != m2.Val && m1.Key == m2.Key)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"failed to generate tally map because of repeated cells.");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
	return m1.Key < m2.Key;
}

void CDLongIntMap::ResortMapKey() // sort from small to big
{
	sort(p_vKeyValPair.begin(),p_vKeyValPair.end(),less_key);
}

bool CDLongIntMap::GetMapVal(long long nKey,int &nVal,int nMin, int nMax)
{
	//int min = 0;
	//int max = KeyValPair.size() - 1;
	if( nKey < p_vKeyValPair[nMin].Key || nKey > p_vKeyValPair[nMax].Key)
	{
		return false;
	}	

	if(nKey == p_vKeyValPair[nMin].Key)
	{
		nVal = p_vKeyValPair[nMin].Val;
		return true;
	}

	if(nKey == p_vKeyValPair[nMax].Key)
	{
		nVal = p_vKeyValPair[nMax].Val;
		return true;
	}

	int nMid;
	while( nMax - nMin > 1)
	{
		nMid = (nMin + nMax)/2;
		if(nKey == p_vKeyValPair[nMid].Key)
		{
			nVal = p_vKeyValPair[nMid].Val;
			return true;
		}
		if(nKey > p_vKeyValPair[nMid].Key )
		{
			nMin = nMid;
		}
		else
		{
			nMax = nMid;
		}
	}
	return false;
}

int CDBinMap::GetBinMapIndex(double nVal)       
	{
		if(p_nHashType == 1)
		{
			int nExpTemp;
			p_nIndex = int(frexp(nVal,&nExpTemp) * 2 * p_nFraNum);
			p_nIndex = p_nIndex - p_nFraNum + p_nFraNum * (nExpTemp - p_nExpStart);
		}
		else if(p_nHashType == 2)
		{
			if(nVal < p_dBound_a || nVal > p_dBound_b)
			{
				p_nIndex = -1;
			}
			else
			{
				p_nIndex = int((nVal - p_dBound_a)/(p_dBound_b - p_dBound_a) * p_nFraNum);
			}
		}
		return p_nIndex;
	}


	////////////// Given an array of float point number, generate bin pointer array ////////////////
	void CDBinMap::GenBinMapOfArray(vector<CDBinPtr> &BinPtrArray, const double *dVec,int nStartPos, int nEndPos)  
	{
		//////////////////////////// check data array ////////////////////////
		if(nStartPos  < 0 || nStartPos > nEndPos)
		{
			printf("wrong data array to generate Bin Map.\n");
			printf("Press ENTER to exit...");
			exit(1);
		}

		///////////////////////// setup BinPtrArray size //////////////////////
		BinPtrArray.clear();
		if(p_nHashType == 1)
		{
			BinPtrArray.resize(p_nFraNum * p_nExpNum);
		}
		else if(p_nHashType == 2)
		{
			BinPtrArray.resize(p_nFraNum + 1);
		}

		////////////////////// establish bin map pointers ///////////////////////
		///////// index of the beginning element
		int nIndexBegin = GetBinMapIndex(*(dVec + nStartPos));   
		BinPtrArray[nIndexBegin].Lower = nStartPos;

		///////// go through all data of the array
		int nIndex = nIndexBegin;  //// current index
		int nIndexTemp = nIndexBegin;
		for(int nPos = nStartPos + 1; nPos <= nEndPos; nPos++)  
		{
			nIndexTemp = GetBinMapIndex(*(dVec + nPos));   //// current index
			if( nIndexTemp > nIndex)    //// increase a coarse bin (pointer pair)
			{
				BinPtrArray[nIndex].Upper = nPos;
				for(int nIx = nIndex + 1; nIx <= nIndexTemp - 1; nIx++)
				{
					BinPtrArray[nIx].Lower = BinPtrArray[nIndex].Lower;
					BinPtrArray[nIx].Upper = nPos;
				}
				BinPtrArray[nIndexTemp].Lower = nPos;
				nIndex = nIndexTemp;
			}
		}

		////////// index of the end element
		int nIndexEnd = nIndexTemp;
		BinPtrArray[nIndexEnd].Upper = nEndPos; 

		//////////  adjusting (Start_ptr, End_ptr] //////////
		for(int nIxOpt = 0; nIxOpt < BinPtrArray.size(); nIxOpt++)	
		{
			if( BinPtrArray[nIxOpt].Lower == -1 ) 
			{
				if( nIxOpt < nIndexBegin )   //// index before index_begin
				{
					BinPtrArray[nIxOpt].Lower = nStartPos;
					BinPtrArray[nIxOpt].Upper = nStartPos + 1;
				}
				else if( nIxOpt > nIndexEnd ) //// index after index_end
				{
					BinPtrArray[nIxOpt].Lower = nEndPos - 1;
					BinPtrArray[nIxOpt].Upper = nEndPos;
				}
				else
				{
					printf("failed  to generate Bin Map.\n");
					printf("Press ENTER to exit...");
					exit(1);
				}
			}
			else
			{
				if( BinPtrArray[nIxOpt].Lower != nStartPos )
				{
					BinPtrArray[nIxOpt].Lower = BinPtrArray[nIxOpt].Lower - 1;  //// (Start_ptr, End_ptr]
				}
			}
		}	  
	}