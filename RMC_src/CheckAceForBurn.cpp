#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::CheckAceForBurn()
{
	//////////////////////// check ACE library for the nuclide used in burnup ///////////////////////////
	FILE *fpXsdir;
	char chNucIdLib[10];

	//////////////////////// check ACE library exist ////////////////////
	fpXsdir = fopen("xsdir","r");
	if (fpXsdir == NULL)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Library index file xsdir not exist!");}, 
			Output.p_fpOutputFilePtr,CDOutput::_ERROR);
	}	

	//////////////////////// find corresponding ACE libraries for burnup isotopes ////////////////////
	int nNucId;
	int nFind = 0;
	char chTemp[200];
	int nTotBurnNucNum = Depth.NucNum ;
	p_vNucAceExist.resize(nTotBurnNucNum + 1);
	for(int i = 1 ; i <= nTotBurnNucNum ; ++i ) 
	{	
		fseek(fpXsdir,0,SEEK_SET);

		p_vNucAceExist[i] = false;

		nNucId = Depth.GetNucId(i)/10 ;
		if((Depth.GetNucId(i)%10)==1)    // excited-state nuclide
		{
			nNucId = nNucId + 400 ;
		}
		//nuc_id_lib = _itoa(nNucId, str,10) ;
		sprintf(chNucIdLib,"%d",nNucId) ;

		strcat(chNucIdLib,p_chAceLibForBurn);

		while(!feof(fpXsdir))
		{
			fscanf(fpXsdir,"%s",chTemp);
			if(strcmp(chNucIdLib, chTemp) == 0)//
			{
				p_vNucAceExist[i] = true;
				++nFind;
				break;
			}
			fgets(chTemp,200,fpXsdir);
		}
	}
	fclose(fpXsdir);

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Find %d/%d isotopes having corresponding ACE libraries.\n",nFind,nTotBurnNucNum);}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	return;
}