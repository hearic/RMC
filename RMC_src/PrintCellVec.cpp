# include "Output.h"

char* CDOutput::PrintCellVec(vector<int> vCellVec)
{
	static char Str[200];
	static char StrTemp[10];
	int nCellVecLen = vCellVec.size();
	if(nCellVecLen >= 1)
	{
		sprintf(Str,"%d",vCellVec[0]);
		for (int i = 1 ; i < nCellVecLen; ++i )
		{
			sprintf(StrTemp," > %d",vCellVec[i]);
			strcat(Str,StrTemp);
		}
	}
	else
	{
		printf("unknown cell vector for printing!\n");
	}

	return Str;
}

char* CDOutput::PrintParticleState(CDParticleState &cParticleState)
{
	static char Str[200];

	sprintf(Str,"Pos = %f %f %f,Dir =%f %f %f",
		cParticleState.p_dPos[0],cParticleState.p_dPos[1],cParticleState.p_dPos[2],cParticleState.p_dDir[0],cParticleState.p_dDir[1],cParticleState.p_dDir[2]);

	return Str;
}