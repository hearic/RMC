# include "mpi.h"
# include "Depth_Class.h"
# include <cstdlib>

void Depth_Class::ErrorWarning(char Message[200],int Type, bool OnlyMasterPrint)
{
	// for non-master process and only master print case, such as read
	if (OnlyMasterPrint && !IsMasterProc)
	{
		if (Type == 1)// Error for parallel case
		{
			MPI_Finalize();
			exit(1);
		}
		return; 
	}

	// Normal message
	if (Type == -1) 
	{
		cerr<<Message<<endl;
		return;
	}
	
	// Warning: OR Error:
	if(Type == 0)  // Warnings
	{
		++WarningCount;
		cerr<<"Warning: "<<Message<<endl;
		DepthRpt.open(ErrFile, ios::app);
		DepthRpt<<"Warning: "<<Message<<endl;
		DepthRpt.close();
	}
    else // if(Type==1)  // Errors 
	{
		++ErrorCount;
		cerr<<"Error: "<<Message<<endl;
		DepthRpt.open(ErrFile, ios::app);
		DepthRpt<<"Error: "<<Message<<endl;
		DepthRpt.close();
		cerr<<"\nPress ENTER to exit...";
		//getchar();
		if (IsRunInParallel)
		{
			MPI_Finalize();
		}
		exit(1);
	}
}
