# include "Depth_Class.h"
# include <algorithm>

void Depth_Class::ReadOrigenXs()
{ 
	ifstream OrigenXs;
	int Flag,StrLen = 200, iTemp;
	char sTemp[200], KeyWord[20];
	long FilePos;
	string KeyWordStr;
	int NucId,NucIdTemp,Index;
	int ProdId;
	int ReactType;
	double ReactRate;
	enum LibType{Origens = 1 , Origen2 = 2 };

	vector<int> XsLibFound(NucNum + 1);

	//////////////////// Open Origens or Origen2 cross-section Library //////////////////
	OrigenXs.open(OrigenXsFile,ios::in);
	if( !OrigenXs ) 
	{
		ErrorWarning("Origen cross-section library not found.",1);
	}

	if(OrigenLibType == Origen2)
	{
		goto Read_Origen2_Xs;
	}


	////////////////////////// Read Origen-s cross-section Library /////////////////////////
	NucIdTemp = 10010;
	Index =  GetNucIdIndex(NucIdTemp);
	while(!OrigenXs.eof())
	{
		FilePos = OrigenXs.tellg();
		if((OrigenXs>>NucId) == 0 || NucId < 10010 || NucId >= (EleNum+1)*10000)
		{
			OrigenXs.clear();
			OrigenXs.seekg(FilePos,ios::beg);
			OrigenXs.getline(sTemp,StrLen);
			continue;
		}

		if(NucId != NucIdTemp)
		{
			Index = GetNucIdIndex(NucId);
			if(Index == 0)
			{
				continue;
			}
			NucIdTemp = NucId;
		}

		OrigenXs>>KeyWord;
		KeyWordStr = KeyWord;
		std::transform(KeyWordStr.begin(), KeyWordStr.end(),KeyWordStr.begin(), ::toupper);

		if(KeyWordStr.compare("TO")==0) 
		{
			if( (OrigenXs>>ProdId>>ReactRate>>sTemp>>ReactType) == 0 ) // sTemp = "mt="
			{
				ErrorWarning("Wrong transition cross-section in ORIGEN-s data.",1);
			}

			OrigenXs.getline(sTemp,StrLen);
			if(strcmp(sTemp, "  byproduct") != 0 )
			{
				GenTransitXs(NucId, ProdId, ReactType, ReactRate);;
			}
		}
		else if(KeyWordStr.compare("FISSION")==0) 
		{
			if( (OrigenXs>>ReactRate) == 0 )
			{
				ErrorWarning("Wrong fission cross-section in ORIGEN-s data.",1);
			}
			if (NucId >= FirstActinide) 
			{
				LibData[Index][22] = ReactRate;
			}
			OrigenXs.getline(sTemp,StrLen);
		}

		for(int i = 19;i <= 24; ++i)
		{
			if(LibData[Index][i] > 0)
			{
				XsLibFound[Index] = 1;
				break;
			}
		}
	}



	goto End;

	////////////////////////// Read Origen-2 cross-section Library /////////////////////////
Read_Origen2_Xs:
	while(!OrigenXs.eof())
	{
		if((OrigenXs>>Flag) == 0)
		{
			OrigenXs.getline(sTemp,StrLen);
			continue;
		}
		if(Flag == -1 || Flag == 1 || Flag == 2 || Flag == 3)
		{
			OrigenXs.getline(sTemp,StrLen);
			continue;
		}

		FilePos = OrigenXs.tellg();
		if((OrigenXs>>NucId) == 0 || NucId < 10010 || NucId >= (EleNum+1)*10000)
		{
			OrigenXs.clear();
			OrigenXs.seekg(FilePos,ios::beg);
			OrigenXs.getline(sTemp,StrLen);
			continue;
		}

		Index = GetNucIdIndex(NucId);
		if(Index == 0)
		{
			continue;
		}

		//// cross-section data
		for(int i = 19;i <= 25; ++i)
		{
			if((OrigenXs>>LibData[Index][i]) == 0)
			{
				ErrorWarning("Wrong entries in ORIGEN-2 cross-section data.",1);
			}
		}
		for(int i = 19;i <= 24; ++i)
		{
			if(LibData[Index][i] > 0)
			{
				XsLibFound[Index] = 1;
				break;
			}
		}

		//// yield data
		if(LibData[Index][25] > 0)  // End with 1.0
		{
			if((OrigenXs>>iTemp) == 0)
			{
				ErrorWarning("Wrong entries in ORIGEN-2 cross-section data.",1);
			}
			for(int i = 25;i <= 32;++i)
			{
				if((OrigenXs>>LibData[Index][i]) == 0)
				{
					ErrorWarning("Wrong entries in ORIGEN-2 yield data.",1);
				}
			}
		}
		else  // End with -1.0
		{
			LibData[Index][25] = -1.0;
		}
	}


	/////////////////// end  //////////////////////
End:
	OrigenXs.close();

	/////////////////// check number of cross-section libraries found //////////////////////
	int FoundNum = 0;
	for(int i = 1 ; i <= NucNum ; ++i)
	{
		if(XsLibFound[i] == 1 )
		{
			FoundNum = FoundNum + 1;
		}
	}

	if( FoundNum < NucNum)
	{
		sprintf(PrintMssg,"%d/%d nuclides find cross-section data.",FoundNum,NucNum);
		ErrorWarning(PrintMssg,0);
	}

	return;
}



void Depth_Class::GenTransitXs(int ReactId, int ProdId, int ReactType, double ReactRate)
{
	enum MtType{nr = 1020, nrx = 1021,n2n = 160, n2nx = 161, n3n = 17, nf = 18, na = 107, np = 103};

	int Index = GetNucIdIndex(ReactId);
	{
		if(Index == 0) return;
	}

	int ReactIdTemp = ReactId/10*10;
	int ProdIdTemp = ProdId/10*10;
	int iTemp = ProdIdTemp - ReactIdTemp;

	if( ReactType == 102 )
	{
		if(iTemp != 10)
		{
			goto End;
		}
		if(ProdId%10 == 0)  // nr
		{
			LibData[Index][19] = ReactRate;
		}
		else if(ProdId%10 == 1) // nrx
		{
			LibData[Index][23] = ReactRate;
		}
	}
	else if( ReactType ==  16 )
	{
		if(iTemp != -10)
		{
			goto End;
		}
		if(ProdId%10 == 0)  // n2n
		{
			LibData[Index][20] = ReactRate;
		}
		else if(ProdId%10 == 1)  //n2nx
		{
			LibData[Index][24] = ReactRate;
		}
	}
	else if( ReactType ==  17 )  // n3n
	{
		if(iTemp != -20)
		{
			goto End;
		}
		if (ReactId >= FirstActinide) 
		{
			LibData[Index][21] = ReactRate;
		}
	}
	else if( ReactType == 107 || ReactType == 34 ) //na
	{
		if(iTemp != -20030)
		{
			goto End;
		}
		if (ReactId < FirstActinide) 
		{
			LibData[Index][21] = ReactRate;
		}
	}
	else if( ReactType == 103 ) //np
	{
		if(iTemp != -10000)
		{
			goto End;
		}
		if (ReactId < FirstActinide) 
		{
			LibData[Index][22] = ReactRate;
		}
	}

	return ;

End:
	sprintf(PrintMssg, "Wrong MT= %d in ORIGEN-s data. Reactant= %d, Product=%d\n", ReactType, ReactId, ProdId);
	ErrorWarning(PrintMssg,0);

	return;
}