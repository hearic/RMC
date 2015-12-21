# include "Depth_Class.h"

double Depth_Class::GetLibAbsXs(int NucIndex)
{
	double totabs = 0 ;
	for(int j = 19 ; j <= 24; ++j)
	{
		totabs = totabs + LibData[NucIndex][j] ;
	}
	return totabs;
}

double Depth_Class::GetLibFisXs(int NucIndex)
{
	return LibData[NucIndex][22];
}

double Depth_Class::GetLibDecQ(int NucIndex)
{ 
	return LibData[NucIndex][13];
}

double Depth_Class::GetLibLamd(int NucIndex)
{ 
	return LibData[NucIndex][1];
}