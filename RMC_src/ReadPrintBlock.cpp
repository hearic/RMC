# include "Input.h"

void CDInput::ReadPrintBlock()
{
char nextchar;

	for(;;)
	{
		if (CDFileIO::GetNextChar(p_fpInputFilePtr,nextchar) >= 3) 
		{
			break;
		}
	 
		string KeyWordStr ;
		CDFileIO::GetKeyWord(p_fpInputFilePtr,KeyWordStr);

		int nRead_i = -1;
		fscanf(p_fpInputFilePtr,"%d",&nRead_i);
		Output.CheckInputParas(nRead_i == 0 || nRead_i == 1,
			[&](){sprintf(Output.p_sPrintStr,"incorrect parameters in PRINT block.\n");},2);

		if(KeyWordStr.compare("MAT")==0) 
		{
			Output.p_bIsMatPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("KEFF")==0)  
		{
			Output.p_bIsKeffPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("SOURCE")==0)  
		{
			Output.p_bIsSrcPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("CELLTALLY")==0)  
		{
			Output.p_bIsCellTallyPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("MESHTALLY")==0)  
		{
			Output.p_bIsMeshTallyPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("CSTALLY")==0)  
		{
			Output.p_bIsCsTallyPrint = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("CYCTALLY")==0)  ///// print tally per cycle
		{
			Output.p_bIsTallyPrintPerCyc = (nRead_i == 1) ? true : false;
		}
		else if(KeyWordStr.compare("INPFILE")==0)  ///// print formatted input file
		{
			Output.p_bIsInputFilePrint = (nRead_i == 1) ? true : false;
		}
		
		//else if(KeyWordStr.compare("BURNNUC")==0)  
		//{
		//	IsBurnNucPrint = (read_i == 1) ? true : false;
		//}
		//else if(KeyWordStr.compare("BURNPOWER")==0)  
		//{
		//	IsBurnPowerPrint = (read_i == 1) ? true : false;
		//}
		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown keyword %s in PRINT card!\n",KeyWordStr.c_str());}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}
	return;
}