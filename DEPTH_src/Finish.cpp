# include "Depth_Class.h"

void Depth_Class::Finish()
{
	FinishTick=clock();    //get system time at end(ms)
	if(ErrorCount+WarningCount > 0)
	{
		cerr<<ErrorCount<<" Errors and "<<WarningCount<<" Warnings are found!"<<endl;
	}
	cout<<"Total Time:"<< (FinishTick-StartTick)/1000.0<<"s\n";

	// Warning: OR Error:
	DepthRpt.open(ErrFile, ios::app);
	DepthRpt<<ErrorCount<<" Errors and "<<WarningCount<<" Warnings are found."<<endl;
	if(SolveMethod==1){DepthRpt<<"TTA Time:";}
	else if(SolveMethod==2){DepthRpt<<"CRAM Time:";}
	else if(SolveMethod==3){DepthRpt<<"Quadrature Time:";}
	else if(SolveMethod==4){DepthRpt<<"Laguerre Time:";}
	DepthRpt<< SolveTick/1000.0<<"s\n";
	DepthRpt<<"Total Time:"<< (FinishTick-StartTick)/1000.0<<"s\n";
	DepthRpt.close();
}