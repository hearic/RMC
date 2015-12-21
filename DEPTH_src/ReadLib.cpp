# include "Depth_Class.h"

void Depth_Class::ReadLib()
{ 
	int cTemp, iTemp,StrLen = 200;
	int nuc_id;
	char sTemp[200];
	void *val;
	////////////////////////// read data library ////////////////////////////
	bool OnlyMasterPrint = true;
	enum eMessage_Type	{_NORMAL = -1, _WARNING, _ERROR};

	ifstream MainLib;
	MainLib.open(MainLibFile,ios::in);
	if( !MainLib ) 
	{
		sprintf(PrintMssg,"Failed opening main depletion library \"%s\".", MainLibFile);
		ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
	}
	
	sprintf(PrintMssg,"\nReading main depletion library \"%s\"...", MainLibFile);
	ErrorWarning(PrintMssg,_NORMAL,OnlyMasterPrint);

	////////////////////////// read decay and cross-section data ////////////////////////////
	while(!MainLib.eof()) //// Skip titles or comments
	{
		cTemp = MainLib.peek();
		if(cTemp == '*' || cTemp == '!') 
		{
			MainLib.getline(sTemp,StrLen);  
		}
		else
		{
			break;
		}
	}

	NucNum = 0;
	ActinidePos = 0;
	NucIDList.resize(1);// index valid from 1
	LibData.resize(1);

	int NucIndex = 0;
	while(!MainLib.eof())
	{
		MainLib>>nuc_id;
		if(nuc_id == -1)  //// end of decay and cross-section data
		{
			MainLib.getline(sTemp,StrLen); 
			break;
		}

		if (nuc_id <= 0)
		{
			sprintf(PrintMssg,"Wrong nuclide %d in decay and cross-section data.",nuc_id);
			ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
		}

		if (nuc_id < NucIDList[NucNum])
		{
			sprintf(PrintMssg,"Disordering of nuclides in library \"%s\": %d appears later than %d.",MainLibFile, nuc_id, NucIDList[NucNum]);
			ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
		}

		int ConvertId = nuc_id/10 + (nuc_id%10)*400;
		if (NucIDIndex[ConvertId] > 0)
		{
			sprintf(PrintMssg,"Nuclide %d appears again in decay and cross-section data (the last one to be used).",nuc_id);
			ErrorWarning(PrintMssg,_WARNING,OnlyMasterPrint);
		}
		else
		{
			NucNum ++;
			NucIDIndex[ConvertId] = NucNum ;
			LibData.resize(NucNum + 1);
			LibData[NucNum].resize(DataNum+1);
			NucIDList.resize(NucNum + 1);		
		}

		NucIndex = NucIDIndex[ConvertId];

		NucIDList[NucIndex] = nuc_id;
		if (nuc_id >= FirstActinide && ActinidePos <= 0)
		{
			ActinidePos = NucIndex;
		}

		if((MainLib>>sTemp) == 0)
		{
			sprintf(PrintMssg,"Unknown nuclide name for nuclide %d in decay and cross-section data.",nuc_id);
			ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
		}

		for(int j = 1;j <= 24;++j)
		{
			if((MainLib>>LibData[NucIndex][j]) == 0)
			{
				sprintf(PrintMssg,"Failed reading decay and cross-section data for nuclide %d.",nuc_id);
				ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
			}
		}

		val = (MainLib>>iTemp);
		if(iTemp != -1 || val == 0)
		{
			sprintf(PrintMssg,"End flag is expected in decay and cross-section data for nuclide %d.",nuc_id);
			ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
		}

		MainLib.getline(sTemp,StrLen);  // change line
	}

	sprintf(PrintMssg,"Finish reading decay and cross-section data for %d nuclides.", NucNum);
	ErrorWarning(PrintMssg,_NORMAL,OnlyMasterPrint);

	////////////////////////// read yield data ////////////////////////////
	int fis_nuc_num = 0;
	int product_num = 0 ;
	int erg_num = 0;
	while(!MainLib.eof()) //// Skip titles or comments
	{
		char cTemp = MainLib.peek();
		if(cTemp == '*' || cTemp == '!') 
		{
			MainLib.getline(sTemp,StrLen);  
		}
		else
		{
			break;
		}
	}

	MainLib>>fis_nuc_num;
	if(fis_nuc_num <= 0 ) 
	{
		ErrorWarning("Wrong number of fissionable nuclides in yield data.",_ERROR,OnlyMasterPrint);
	}

	YieldData.resize(fis_nuc_num);
	for(int i = 0 ; i < fis_nuc_num ; ++i)
	{
		if((MainLib>>YieldData[i].FisNucID) == 0)
		{
			ErrorWarning("Failed reading fissionable nuclide ID in yield data.",_ERROR,OnlyMasterPrint);
		}
	}
	
	for(int i = 0 ; i < fis_nuc_num ; ++i)
	{
		MainLib>>nuc_id>>product_num>>erg_num;
		if(nuc_id != YieldData[i].FisNucID || product_num <= 0 || erg_num <=0) 
		{
			sprintf(PrintMssg,"Wrong yield data for %d.",YieldData[i].FisNucID);
			ErrorWarning(PrintMssg,_ERROR,OnlyMasterPrint);
		}

		// check if fission nuclide exist in decay and xs library (read decay already)
		int ReactantIndex = GetNucIdIndex(nuc_id);
		if (ReactantIndex == 0)// not exist in decay and xs lib
		{
			sprintf(PrintMssg,"Fissionable nuclide %d in fission product yield data does not exist in decay and xs lib.",nuc_id);
			ErrorWarning(PrintMssg,_WARNING,OnlyMasterPrint);
		}		

		YieldData[i].Energy.resize(erg_num);
		for(int j = 0 ; j < erg_num ; ++j)
		{
			MainLib >> YieldData[i].Energy[j];
		}
		        	
		YieldData[i].ProductId.resize(product_num);
		YieldData[i].ProductYld.resize(erg_num);
		for(int n = 0 ; n < erg_num ; ++n)
		{
			YieldData[i].ProductYld[n].resize(product_num);
		}

		for(int j = 0 ; j < product_num ; ++j)
		{
			MainLib>>YieldData[i].ProductId[j];

			for(int n = 0 ; n < erg_num ; ++n)
			{
				MainLib>>YieldData[i].ProductYld[n][j];
			}
		}

		val = (MainLib>>iTemp);
		if(iTemp != -1 || val == 0)
		{
			ErrorWarning("End flag is expected in yield data.",_ERROR,OnlyMasterPrint);
		}
		MainLib.getline(sTemp,StrLen);  // change line
	}

	sprintf(PrintMssg,"Finish reading yield data for %d fissionable nuclides.", fis_nuc_num);
	ErrorWarning(PrintMssg,_NORMAL,OnlyMasterPrint);

	//////////////////////////////// end /////////////////////////////////////
	val = (MainLib>>iTemp);
	if(iTemp != -1 || val == 0)
	{
		ErrorWarning("End flag is expected in main library.",_ERROR,OnlyMasterPrint);
	}
	MainLib.close();

	return;
}