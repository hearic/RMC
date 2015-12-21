# include "Depth_Class.h"
# include "ME_Class.h"
# include "TTA_Class.h"
# include <algorithm>
/////////Key word list////////

void Depth_Class::ReadInput()
{
	ifstream Input;
	int i,StrLen = 200,NucID, Index;
	double fTemp;
	char KeyWord[20], sTemp[200], cTemp;
	string KeyWordStr;
	void *val;
	long FilePos;


	////////////// Read Input File ///////////////
	Input.open(InputFile,ios::in);
	if( !Input ) 
	{
		ErrorWarning("Failed opening input file.",1);
	}

	for(;;)
	{
		val=(Input>>KeyWord);
		if(Input.eof()){break;}
		if(val==0){ErrorWarning("Failed reading  input file.",1);}
		KeyWordStr = KeyWord;
		std::transform(KeyWordStr.begin(), KeyWordStr.end(),KeyWordStr.begin(), ::toupper);

DFP:
		if(KeyWordStr.compare("DECAY")==0 || KeyWordStr.compare("FLUX") == 0 || KeyWordStr.compare("POWER") == 0) 
		{
			++StepNum;
			StepFlux.resize(StepNum+1);
			StepPower.resize(StepNum+1);
			StepTime.resize(StepNum+1);
			StepMode.resize(StepNum+1);
			if(KeyWordStr.compare("DECAY") == 0) 
			{
				StepMode[StepNum]=0;
			}
			else if(KeyWordStr.compare("FLUX") == 0) 
			{
				val=(Input>>StepFlux[StepNum]);
				if(val==0){ErrorWarning("Failed reading burnup flux.",1);}
				StepMode[StepNum]=1;
			}
			else if(KeyWordStr.compare("POWER") == 0) 
			{
				val=(Input>>StepPower[StepNum]);
				if(val==0){ErrorWarning("Failed reading burnup power.",1);}
				StepMode[StepNum]=2;
			}

			// Read time of Decay | Flux | Power mode
			val=(Input>>sTemp);
			if(val==0){ErrorWarning("Failed reading burnup time.",1);}
			fTemp=atof(sTemp);
			cTemp=toupper(sTemp[strlen(sTemp)-1]);
			if(cTemp=='S'){StepTime[StepNum]=fTemp;}
			else if(cTemp=='M'){StepTime[StepNum]=fTemp*60;}
			else if(cTemp=='H'){StepTime[StepNum]=fTemp*3600;}
			else if(cTemp=='D'){StepTime[StepNum]=fTemp*3600*24;}
			else if(cTemp=='Y'){StepTime[StepNum]=fTemp*3600*24*365.25;}
			else{ErrorWarning("Wrong time  unit.",1);}

			//// Read repeated parameter
			if((Input>>sTemp) != 0 )
			{
				string StrTemp;
				StrTemp = sTemp;
				std::transform(StrTemp.begin(), StrTemp.end(),StrTemp.begin(), ::toupper);
				if(StrTemp.compare("*")!=0) 
				{
					KeyWordStr = StrTemp; 
					goto DFP;
				}

				int RepeatNum ;
				val=(Input>>RepeatNum);
				if(val == 0 || RepeatNum < 1)
				{
					ErrorWarning("Failed reading repeated parameters.",1);
				}

				for(int i = 2 ; i <= RepeatNum; ++i)
				{
					StepFlux.push_back(StepFlux[StepNum]);
					StepPower.push_back(StepPower[StepNum]);
					StepTime.push_back(StepTime[StepNum]);
					StepMode.push_back(StepMode[StepNum]);
				}
				StepNum = StepNum + RepeatNum - 1 ;
			}  
		}

		else if(KeyWordStr.compare("DENSITY") == 0) 
		{
			N0.resize(NucNum+1);
			i=0;
			for(;;)
			{
				FilePos=Input.tellg();
				val=(Input>>NucID);
				if(val==0)
				{
					Input.clear();
					Input.seekg(FilePos,ios::beg);
					break;
				}
				else
				{
					Index = GetNucIdIndex(NucID);
					val=(Input>>N0[Index]);
					if(val==0){ErrorWarning("Failed reading nuclide density.",1);}
					++i;
				}
			}
			if(i==0){ErrorWarning("Failed reading initial composition.",1);}
		}  

		else if(KeyWordStr.compare("MAINLIB") == 0) 
		{
			val=(Input>>MainLibFile);
			if(val==0){ErrorWarning("Failed reading file name of main library.",1);}	
		}  

		else if(KeyWordStr.compare("CONVERTLIB") == 0) 
		{
			if((Input>>OrigenLibType) == 0 || (OrigenLibType!=1 && OrigenLibType!=2))
			{
				ErrorWarning("Unknown ORIGEN library type.",1);
			}

			if(OrigenLibType == 1) // Origen-s library type
			{
				Input>>OrigenDecFile>>OrigenXsFile>>OrigenYldFile;
			}
			else if(OrigenLibType == 2)  // Origen-2 library type
			{
				Input>>OrigenDecFile;
				strcpy(OrigenXsFile,OrigenDecFile);
				strcpy(OrigenYldFile,OrigenDecFile);
			}
			if((Input>>MainLibFile) ==0)
			{
				ErrorWarning("Failed reading file name of main library.",1);
			}	
			ConvertLibFlag = true;
		}  

		else if(KeyWordStr.compare("SUBLIB") == 0) 
		{
			val=(Input>>SubLibFile);
			if(val==0){ErrorWarning("Failed reading file name of substitute library.",1);}
			ReadSubLibFlag = true;
		} 

		else if(KeyWordStr.compare("SOLVER") == 0) 
		{
			val=(Input>>SolveMethod);  // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre
			if(val==0 || SolveMethod < 1 || SolveMethod>4)
			{
				ErrorWarning("Wrong burnup solver.",1);
			}
		}  
		else if(KeyWordStr.compare("LPAM") == 0) 
		{
			val=(Input>>ME.Laguerre_a>>ME.Laguerre_Tao>>ME.Laguerre_N);  // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre
			if(val==0)
			{
				ErrorWarning("Wrong LPAM parameters.",1);
			}
		}  
		else if(KeyWordStr.compare("TTA") == 0) 
		{
			val=(Input>>TTA.CutoffFrac);  // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre
			if(val==0)
			{
				ErrorWarning("Wrong TTA parameters.",1);
			}
		}  
		else if(KeyWordStr.compare("QRAM") == 0) 
		{
			val=(Input>>ME.Quad_N);  // 1: TTA   2: CRAM  3: Quadrature  4£ºLaguerre
			if(val==0)
			{
				ErrorWarning("Wrong QRAM parameters.",1);
			}
		} 

		else if(KeyWordStr.compare("PRINT") == 0) 
		{
			// Option of printing concentration: 0--Print Last step  1--Print All steps
			val=(Input>>PrintFlag);  
			if(PrintFlag!=0 && PrintFlag!=1){ErrorWarning("Wrong print option. 0--Print end step, 1-- Print all steps.",1);}
		} 

		else if(KeyWordStr.compare("RADIOACTIVITY") == 0) 
		{
			val=(Input>>RadioactivityFlag);  
			if(RadioactivityFlag!=0 && RadioactivityFlag!=1 && RadioactivityFlag!=2)
			{
				ErrorWarning("Wrong output option of radioactivity. 0--None 1--instantaneous, 2--integral.",1);
			}
		} 

		else if(KeyWordStr.compare("ABSORPRATE") == 0) 
		{
			val=(Input>>AbsRateFlag);  
			if(AbsRateFlag!=0 && AbsRateFlag!=1 && AbsRateFlag!=2)
			{
				ErrorWarning("Wrong output option of neutron absorption rate. 0--None 1--instantaneous, 2--integral.",1);
			}
		} 
		else if(KeyWordStr.compare("FISSIONRATE") == 0) 
		{
			val=(Input>>FissionRateFlag);  
			if(FissionRateFlag!=0 && FissionRateFlag!=1 && FissionRateFlag!=2)
			{
				ErrorWarning("Wrong output option of fission rate. 0--None 1--instantaneous, 2--integral.",1);
			}
		} 
		else if(KeyWordStr.compare("DECAYHEAT") == 0) 
		{
			val=(Input>>DecayHeatFlag);  
			if(DecayHeatFlag!=0 && DecayHeatFlag!=1 && DecayHeatFlag!=2)
			{
				ErrorWarning("Wrong output option of thermal power. 0--None 1--instantaneous, 2--integral.",1);
			}
		} 
		else if(KeyWordStr.compare("FLUXPOWER") == 0) 
		{
			val=(Input>>MeanPowerFlag>>MeanFluxFlag);  
			if((MeanPowerFlag!=0 && MeanPowerFlag!=1) || (MeanFluxFlag!=0 && MeanFluxFlag!=1))
			{
				ErrorWarning("Wrong Flux&Power Flag. 0--Use Starting Value 1--Use Mean Value",1);
			}
		} 

		else if(KeyWordStr.compare("ENERGY") == 0) 
		{
			Input>>AveEnergy;
			if(AveEnergy <= 0)
			{
				ErrorWarning("Wrong neutron energy.",1);
			}
		}

		else
		{
			Input.getline(sTemp,StrLen);  // change line
		}

	}  // end for(;;)

	return;

	///////////////////////////////////////////

}