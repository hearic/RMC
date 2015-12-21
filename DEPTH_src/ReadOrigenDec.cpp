# include "Depth_Class.h"
const double LN2 = log(2.0);
const int DataWith = 12, DataPrecision = 4;

void Depth_Class::ReadOrigenDec()
{ 
	////////////////////////////////////////////////////////////
	/////// read decay library of ORIGEN-s or ORIGEN-2 /////////
	///////  ORIGEN-s decay lib has 5 extra entries    /////////
	////////////////////////////////////////////////////////////
	ifstream OrigenDec;
	OrigenDec.open(OrigenDecFile,ios::in);
	if( !OrigenDec ) 
	{
		ErrorWarning("Failed opening Origen library file.",1);
	}
	////////////// Read Origen-2 Library ///////////////

	int Flag,StrLen = 200,NucID,Index;
	char sTemp[200];
	void *val1,*val2;
	int iu,li2,li3;
	double halfl, fb1, fp, fp1, fa, ft;         //// first line
	double fsf, fbn, q, abund, ampc, wmpc ; //// second line
	double fg, fb, fbb, fneut, fba;        //// third line (only for ORIGEN-s decay library)
	double lamd;
	double tofrac;
	enum LibType{Origens = 1 , Origen2 = 2 };
	vector<int> DecLibFound(NucNum + 1);
	while(!OrigenDec.eof())
	{
		if((OrigenDec>>Flag) == 0)   // skip lines with non-numeral
		{
			OrigenDec.getline(sTemp,StrLen);
			continue;
		}
		if(Flag != 1 && Flag != 2 && Flag != 3)  // skip lines with non-decay data
		{
			OrigenDec.getline(sTemp,StrLen);
			continue;
		}

		long FilePos = OrigenDec.tellg();
		if((OrigenDec>>NucID) == 0 || NucID < 10010 || NucID >= (EleNum+1)*10000)   //// read title
		{
			OrigenDec.clear();
			OrigenDec.seekg(FilePos,ios::beg);
			OrigenDec.getline(sTemp,StrLen);
			continue;
		}

		Index = GetNucIdIndex(NucID);

		if(Index == 0 || DecLibFound[Index] ==1)
		{
			continue;
		}
		DecLibFound[Index] = 1;

		//// read entries
		val1 = OrigenDec>>iu>>halfl>>fb1>>fp>>fp1>>fa>>ft>>li2>>fsf>>fbn>>q>>abund>>ampc>>wmpc ; 
		if(OrigenLibType == Origens)  //// ORIGEN-s has 5 extra entries
		{
			val2 = OrigenDec>>li3>>fg>>fb>>fbb>>fneut>>fba;
		}
		if(val1 == 0 || val2 == 0 )
		{
			ErrorWarning("Wrong entries in Origen decay library.",1);
		}
		OrigenDec.getline(sTemp,StrLen);


		//// convert and store q, abund, ampc, and wmpc in LibData array
		LibData[Index][13] = q;
		LibData[Index][14] = abund;
		LibData[Index][15] = ampc;
		LibData[Index][16] = wmpc;

		//// convert and store entries in LibData array
		switch(iu)
		{
		case 6:{ lamd = 0;  break; }
		case 1:{ lamd = LN2/halfl;   break; }
		case 2:{ lamd = LN2/(halfl*60);   break; }
		case 3:{ lamd = LN2/(halfl*3600);   break; }
		case 4:{ lamd = LN2/(halfl*3600*24);   break; }
		case 5:{ lamd = LN2/(halfl*3600*24*365.25);   break; }
		case 7:{ lamd = LN2/(halfl*3600*24*365.25*1.0E+3);   break; }
		case 8:{ lamd = LN2/(halfl*3600*24*365.25*1.0E+6);   break; }
		case 9:{ lamd = LN2/(halfl*3600*24*365.25*1.0E+9);   break; }
		}
		if(lamd < 1.0E-25)
		{
			continue;
		}
		if(OrigenLibType == Origens)
		{
			tofrac = fb + fb1 + fp + fp1 + ft + fa + fsf + fbn + fbb + fneut + fba;
			if(fabs(tofrac - 1.0) > 1.0E-05)
			{
				ErrorWarning("Wrong total decay fraction in Origen-s decay library.",1);
			}
		}
		else if(OrigenLibType == Origen2)
		{
			tofrac = fb1 + fp + fp1 + ft + fa + fsf + fbn;
			if(tofrac - 1.0 > 1.0E-3)
			{
				ErrorWarning("Wrong total decay fraction in Origen-2 decay library.",1);
				tofrac = 0 ;
			}
			fb = 1.0 - tofrac;
			if(fb < 0)
			{
				fb = 0 ;
			}
			fbb = 0;
			fneut = 0;
			fba = 0;
		}
		LibData[Index][1] = lamd;
		LibData[Index][2] = fb1;
		LibData[Index][3] = fp;
		LibData[Index][4] = fp1;
		LibData[Index][5] = fa;
		LibData[Index][6] = ft;
		LibData[Index][7] = fsf;
		LibData[Index][8] = fbn;
		LibData[Index][9] = fb;
		LibData[Index][10] = fbb;
		LibData[Index][11] = fneut;
		LibData[Index][12] = fba;
	}
	OrigenDec.close();


	/////////////////// check number of decay libraries found ////////////////////////
	int FoundNum = 0;
	for(int i = 1 ; i <= NucNum ; ++i)
	{
		if(DecLibFound[i] == 1 )
		{
			FoundNum = FoundNum + 1;
		}
	}

	if( FoundNum < NucNum)
	{
		sprintf(PrintMssg,"%d/%d nuclides find decay data.",FoundNum,NucNum);
		ErrorWarning(PrintMssg,0);
	}

	return;
}