# include "Depth_Class.h"

void Depth_Class::RunDepth(int argc_temp,char *argv_temp[])
{
	/////////check inp file exist: start ////////////
	char argv[3][200];
	int argc=argc_temp;
	for(int i=0;i<=argc-1;i++){strcpy(argv[i], argv_temp[i]);} 
	CheckFileExist(argc,argv);

	////////////////////////////////////
	StartTick=clock();    //get system time at beginning(ms)

	//////////////////////////////// input and initiate //////////////////////////////////
	SetDefault();
	ReadInput();
	Initiate();

	/////////////////////////////// Process Data Library /////////////////////////////////
	ConvertLib();
	ReadLib();
	ReadSubLib();

	//////////////////////////////// execute //////////////////////////////////
	Execute();

	//////////////////////////////// output //////////////////////////////////
	WriteOutput();

	//////////////////////////////// finish //////////////////////////////////
	Finish();

}