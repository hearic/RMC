# include "Depth_Class.h"
# include "TTA_Class.h"
# include "ME_Class.h"

void Depth_Class::GenTranMatrix(int MatrixType)
{
	/////Generate transition matrix A
	/////MatrixType:  1-Row Compressed Storage. 2-Column Compressed Storage. 3-Mixed Compressed Storage
	int i,size;
	double CalTemp = FLUX*1.0E-24;  // Because N0 unit is n/barn

	//// Generate Transition Matrix of Row compressed storage, which is usually used in A*b ////
	if(MatrixType == 1) 
	{
		ME.TranMatrixRCS.Reset(NucNum+1);

		size=DecMatrix.Size();
		for(i=0;i<size;++i)
		{
			ME.TranMatrixRCS.AddElem(DecMatrix._row[i],DecMatrix._col[i],DecMatrix._val[i]);
		}
		if(MODE!=0) // MODE=0 include  (FLUX=0 or  POWER=0) condition. See CheckInput.cpp
		{
			size=NeuMatrix.Size();
			for(i=0;i<size;++i)
			{
				ME.TranMatrixRCS.AddElem(NeuMatrix._row[i],NeuMatrix._col[i],NeuMatrix._val[i]*CalTemp);	
			}	
		}

	}

	//// Generate Transition Matrix of mixed compressed storage, which is used in TTA matrix ////
	else if(MatrixType == 2)
	{
	    TTA.TranMatrixCCS.Reset(NucNum+1);

		size=DecMatrix.Size();
		for(i=0;i<size;++i)
		{
			TTA.TranMatrixCCS.AddElem(DecMatrix._row[i],DecMatrix._col[i],DecMatrix._val[i]);
		}
		if(MODE!=0) // MODE=0 include  (FLUX=0 or  POWER=0) condition. See CheckInput.cpp
		{
			size=NeuMatrix.Size();
			for(i=0;i<size;++i)
			{
				TTA.TranMatrixCCS.AddElem(NeuMatrix._row[i],NeuMatrix._col[i],NeuMatrix._val[i]*CalTemp);	
			}	
		}
	}

	//// Generate Transition Matrix of mixed compressed storage, which is usually used in A\b ////
	else if(MatrixType == 3)
	{
		ME.TranMatrixMCS.Reset(NucNum+1);

		size = DecMatrix.Size();
		for(int i=0;i<size;++i)
		{
			ME.TranMatrixMCS.AddElem(DecMatrix._row[i],DecMatrix._col[i],DecMatrix._val[i]);
		}
		if(MODE!=0) // MODE=0 include  (FLUX=0 or  POWER=0) condition. See CheckInput.cpp
		{
			size = NeuMatrix.Size();
			for(int i=0;i<size;++i)
			{
				ME.TranMatrixMCS.AddElem(NeuMatrix._row[i],NeuMatrix._col[i],NeuMatrix._val[i]*CalTemp);	
			}	
		}
	}


	return;
}