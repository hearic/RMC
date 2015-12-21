# include "Burnup.h"

int CDBurnup::ConvertNucId(const char chIdStr[12])
{

	int nNucId = atoi(chIdStr);
	int nIndex;

	if(nNucId % 1000 == 0 ) //// skip natural isotopes
	{
		nIndex = -1000;
		return nIndex;
	}

	if( nNucId % 1000 >= 400 )  //2012-01-17      excited-state nuclide
	{ 
		nNucId = (nNucId - 400)*10 + 1 ;  
	}
	else
	{
		nNucId = nNucId*10;
	}

	nIndex = Depth.GetNucIdIndex(nNucId);
	if( nIndex < 1 || nIndex > Depth.NucNum )
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown nuclide index %d converted from RMC to DEPTH.\n",nNucId);}, 
			Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		nIndex = -1;
	}

	return nIndex;
}