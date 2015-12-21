#include "Burnup.h"
#include "Criticality.h"

void CDBurnup::GenImpNucIndex()
{
	int nImpNucNum = p_vImpNucId.size();

	p_vImpNucIndex.clear();

	for( int i = 0; i < nImpNucNum; ++i )
	{
	    ////// find nuclide index in depth
		int nNucId = p_vImpNucId[i];

    	int nIndex = Depth.GetNucIdIndex(nNucId);
		if( nIndex < 1 || nIndex > Depth.NucNum )
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown important nuclide %d.\n",nNucId);}, 
				Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			continue;
		}

		if(!p_vNucAceExist[nIndex])
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"important nuclide %d is not in ACE libraries.\n",nNucId);}, 
				Output.p_fpOutputFilePtr, CDOutput::_WARNING);
			continue;
		}

		p_vImpNucIndex.push_back(nIndex);
	}
	return;

}