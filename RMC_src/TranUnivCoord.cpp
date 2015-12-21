# include "Universe.h"


void CDUniverse::TransUnivCoord(double dPos[3],double dDir[3])
{
	////// for users: transform a universe (x') into another universe (x). first rotate ,then translate. 
	////// in code: transform x to x'. first translate, then rotate.

	/////////////////  calculate x' = R * x - m /////////////////////////
	//////////////////////// translation ////////////////////////
	if(p_bIsMoved)  
	{
		for (int i = 0 ; i < 3 ; ++i )  //// translation
		{
			dPos[i] = dPos[i] - p_dOrigin[i] ;
		}
	}
	
	
	////////////////////// rotation ///////////////////////////
	if(p_bIsRotated)  
	{
		double dPos_temp[3];
		double dDir_temp[3];
		for(int i = 0 ; i < 3 ; ++i ) 
		{
			dPos_temp[i] = dPos[i] ;
			dDir_temp[i] = dDir[i] ;
		}

		for (int i = 0 ; i < 3 ; ++i )  
		{
			dPos[i] = 0;
			dDir[i] = 0;
			for(int j = 0 ; j < 3 ; ++j)
			{
				dPos[i] = dPos[i] + p_dRotation[i][j]*dPos_temp[j];
				dDir[i] = dDir[i] + p_dRotation[i][j]*dDir_temp[j];
			}
		}
	}

	return;

}

void CDUniverse::TransUnivDir(double dDir[3])
{
	/////////////////  calculate x' = R * x /////////////////////////
	if(p_bIsRotated)  //// rotation
	{
		double dDir_temp[3];
		for(int i = 0 ; i < 3 ; ++i ) 
		{
			dDir_temp[i] = dDir[i] ;
		}

		for (int i = 0 ; i < 3 ; ++i )  //// translation
		{
			dDir[i] = 0 ;
			for(int j = 0 ; j < 3 ; ++j)
			{
				dDir[i] = dDir[i] + p_dRotation[i][j]*dDir_temp[j];
			}
		}
	}

	return;
}