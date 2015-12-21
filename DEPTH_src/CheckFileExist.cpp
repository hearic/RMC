# include "Depth_Class.h"


void Depth_Class::CheckFileExist(int argc, char argv[3][200])
{
	FILE *fptr_exist;
	if(argc==1)   //neither input file nor output file is given
	{
		strcpy(InputFile,"Inp");
		fptr_exist = fopen(InputFile,"r");
		while(fptr_exist == NULL)
		{
			cout<<"File "<<InputFile<<" Not Found!"<<endl;
			cout<<"Input File Name: ";
			cin>>InputFile;
			fptr_exist = fopen(InputFile,"r");
		}    
	}
	else if(argc==2)    // only input file is given
	{
		strcpy(InputFile,argv[1]);
		fptr_exist = fopen(InputFile,"r");
		while(fptr_exist == NULL)
		{
			cout<<"File "<<InputFile<<" Not Found!"<<endl;
			cout<<"Input File Name: ";
			cin>>InputFile;
			fptr_exist = fopen(InputFile,"r");
		}
	}
	else    // both input file and output file are give
	{
		strcpy(InputFile,argv[1]);
		strcpy(OutputFile,argv[2]);
		fptr_exist = fopen(InputFile,"r");
		while(fptr_exist == NULL)
		{
			cout<<"File "<<InputFile<<" Not Found!"<<endl;
			cout<<"Input File Name: ";
			cin>>InputFile;
			fptr_exist = fopen(InputFile,"r");
		}
	}		
	return;
}