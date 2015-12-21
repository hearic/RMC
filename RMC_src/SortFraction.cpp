#include "Burnup.h"
#include "Criticality.h"


void  CDBurnup::SortFraction(vector<double> &dArrayData, int nIb, int nIc, vector<int> &dArraySeq)
{
	int nLeft,nRight,i,j,nTemp;
	double dMiddle,dTemp;
	nLeft=nIb;
	nRight=nIc;

	i = nLeft;
	j = nRight;
	dMiddle = dArrayData[(nLeft+nRight)/2];   // middle vanue
	while(i<=j)
	{
		while(dArrayData[i]>dMiddle && i<nRight){i=i+1;}   //scan from the left              		
		while(dArrayData[j]<dMiddle && j>nLeft){j=j-1;}   //scan from the right        
		if(i<=j) //found and swap
		{
			dTemp = dArrayData[j];
			dArrayData[j] = dArrayData[i];
			dArrayData[i] = dTemp;
			nTemp = dArraySeq[j];
			dArraySeq[j] = dArraySeq[i];
			dArraySeq[i] = nTemp;
			i=i+1;
			j=j-1;
		}
	}	
	if(nLeft<j) //left recursion  
	{
		SortFraction(dArrayData,nLeft,j,dArraySeq);
		//right=j;//goto go10;
	}	
	if(nRight>i)//  right recursion  
	{
		SortFraction(dArrayData,i,nRight,dArraySeq);
		//left=i;	//goto go10; 
	}
	return;

}