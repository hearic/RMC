# include "Depth_Class.h"

int Depth_Class::GetNucId(int NuIndex)
{ 
	if(NuIndex < 1 || NuIndex > NucNum)
	{
		ErrorWarning("Nuclide index exceeds bound.",1);
		return -1;
	}

	return NucIDList[NuIndex];

}

int Depth_Class::GetNucIdIndex(int NucId)
{ 
	if(NucId < 10010 || NucId >= (EleNum+1)*10000)// 1001~1190000
	{
		sprintf(PrintMssg,"Nuclide ID %d exceeds bound.",NucId);
		ErrorWarning(PrintMssg,0);
		return 0;
	}

	int NucIdTemp = NucId/10 + (NucId%10)*400;

	return NucIDIndex[NucIdTemp];
}

string Depth_Class::GetNucName(int NuId)
{ 
	string NucName;
	int EleID = NuId/10000;
	int WeightNum = (NuId - EleID*10000)/10;
	char sWeightNum[5];


	sprintf(sWeightNum,"%d",WeightNum) ;
	(NucName+=EleNameList[EleID])+=sWeightNum;
	if(NuId%10==1)
	{
		NucName+="m";
	}

	return NucName;
}