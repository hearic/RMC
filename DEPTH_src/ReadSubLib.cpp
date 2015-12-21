# include "Depth_Class.h"
const int DataWith=12, DataPrecision=4;

void Depth_Class::ReadSubLib()
{ 
	if(!ReadSubLibFlag)
	{
		return;
	}

	int NucID,iTemp,StrLen=200,count=0;
	char sTemp[200], cTemp;
	void *val;
	ifstream SubLib;
	SubLib.open(SubLibFile,ios::in);

	if( !SubLib ) 
	{
		ErrorWarning("Failed opening substitute library file.",1);
	}

	while(!SubLib.eof()) //// Skip titles or comments
	{
		cTemp = SubLib.peek();
		if(cTemp == '*' || cTemp == '!') 
		{
			SubLib.getline(sTemp,StrLen);  
		}
		else
		{
			break;
		}
	}
	while(!SubLib.eof())
	{
		SubLib>>NucID;
		if(NucID == -1)  //// end of decay and cross-section data
		{
			SubLib.getline(sTemp,StrLen); 
			break;
		}

		int index = GetNucIdIndex(NucID);
		if(index == 0)
		{
			continue;
		}

		if((SubLib>>sTemp) == 0)
		{
			ErrorWarning("Failed reading nuclide name in substitute library.",0);
			continue;
		}
		for(int j = 19;j <= 25;++j)  //// substitute library only has cross-section/yield data
		{
			if((SubLib>>LibData[index][j]) ==0 )
			{
				ErrorWarning("Failed reading cross-section in substitute library.",0);
				continue;
			}
		}
		if(int(LibData[index][25]) != -1)
		{
			for(int j = 26;j <= 32;++j)
			{
				if((SubLib>>LibData[index][j]) == 0)
				{
					ErrorWarning("Failed reading fission yields in substitute library.",1);
				}
			}

			val=(SubLib>>iTemp);
			if(iTemp != -1 || val == 0)
			{
				ErrorWarning("Failed reading end flag in main library.",1);
				continue;
			}
		}
		SubLib.getline(sTemp,StrLen);  // change line
	}


	SubLib.close();

	return;
}