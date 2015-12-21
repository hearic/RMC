# include "Input.h"

void CDInput::CheckIOFile(int argc_temp,char *argv_temp[])
{ 	
	//check command line input and IO file
	char OutputFileName[FILE_NAME_LENGTH];

	///////////////// Only checkIOFile when MyId==0 for parallel  ///////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		goto end;
	}
# endif
	{
		/////////check command line ////////////
		if (argc_temp > 3)
		{
			cout<<"Error: too many inputs in command line!"<<endl;
			exit(0);
		}

		vector<string> vCmdlineinput(3);
		vCmdlineinput[0] = "execution address";
		vCmdlineinput[1] = "input file name";
		vCmdlineinput[2] = "output file name";

		char argv[3][FILE_NAME_LENGTH];
		int argc=argc_temp;
		for(int i = 0;i <= argc - 1;i++)
		{
			if (strlen(argv_temp[i]) > FILE_NAME_LENGTH - 20)// FILE_NAME_LENGTH = 220
			{
				cout<<"Error: too many characters of \""<<vCmdlineinput[i]<<"\" !"<<endl;
				exit(0);
			}
			strcpy(argv[i], argv_temp[i]);
		} 

		FILE *fptr_exist;
		//////////////////CD_Input///////////////////////////////////////////////////////////
		if(argc==1)   //neither input file nor output file is given
		{
			strcpy(p_chInputFileName,"inp");
			fptr_exist = fopen(p_chInputFileName,"rb");
			while(fptr_exist == NULL)
			{
				cout<<"File "<<p_chInputFileName<<" Not Found!"<<endl;
				cout<<"Input File Name: ";
				cin>>p_chInputFileName;
				fptr_exist = fopen(p_chInputFileName,"rb");
			}
			strcpy(OutputFileName,p_chInputFileName);
			strcat(OutputFileName,".out");      
		}
		else if(argc==2)    // only input file is given
		{
			strcpy(p_chInputFileName,argv[1]);
			fptr_exist = fopen(p_chInputFileName,"rb");
			while(fptr_exist == NULL)
			{
				cout<<"File "<<p_chInputFileName<<" Not Found!"<<endl;
				cout<<"Input File Name: ";
				cin>>p_chInputFileName;
				fptr_exist = fopen(p_chInputFileName,"rb");
			}
			strcpy(OutputFileName,p_chInputFileName);
			strcat(OutputFileName,".out");  	
		}
		else    // both input file and output file are give
		{
			strcpy(p_chInputFileName,argv[1]);
			strcpy(OutputFileName,argv[2]);
			fptr_exist = fopen(p_chInputFileName,"rb");
			while(fptr_exist == NULL)
			{
				cout<<"File "<<p_chInputFileName<<" Not Found!"<<endl;
				cout<<"Input File Name: ";
				cin>>p_chInputFileName;
				fptr_exist = fopen(p_chInputFileName,"rb");
				strcpy(OutputFileName,p_chInputFileName);
				strcat(OutputFileName,".out");
			}
		}

		//////////Record two different format//////////////////////////////////////////////
		char chSI_Format[6] = ".sinp";// 6 bit for 5 character's string
		//strcpy(chSI_Format,".sinp");
		int nTemp = strlen(p_chInputFileName);
		p_nInpFileFormat = 1;// default, normal format
		if (nTemp >= 6)
		{
			int nAscii = 0;
			for(int i = 0; i <= 4; i++)
			{
				nAscii = nAscii + abs(int(p_chInputFileName[nTemp - 5 + i]) - int(chSI_Format[i]));
			}
			p_nInpFileFormat = nAscii;// if last 5 characters are ".sinp", nFormat = nAscii = 0;
		}

		/////////////////////////////////////////////////////////////////////////////
		fclose(fptr_exist);
	}
# ifdef _IS_PARALLEL_
end:
	MPI_Bcast(p_chInputFileName,FILE_NAME_LENGTH,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(OutputFileName,FILE_NAME_LENGTH,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(&p_nInpFileFormat,1,MPI_INT,0,MPI_COMM_WORLD);
# endif

	strcpy(Output.p_chInputFileName,p_chInputFileName);
	strcpy(Output.p_chOutputFileName,OutputFileName);
	return;
}
