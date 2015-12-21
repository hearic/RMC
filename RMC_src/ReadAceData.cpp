# include "AceData.h"
# include "Input.h"

void CDAceData::ReadAceData(int nTotNucNum, int nTotNonSabNucNum)
{

	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\nReading XSDIR/ACE library...");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
	
	////////////////////// check xsdir existence  //////////////////////////
	FILE *fptr_xsdir, *fptr_ace;
	fptr_xsdir = fopen("xsdir","r");
	if (fptr_xsdir == NULL)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Library index file \"xsdir\" not exist!");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}	

	char chDataPath[200];
	char chFullPath[200];
	char chAccessRoute[70]; // Actually not used
	char chNucIDTemp[200];
	char chread_s[200];
	vector<int> vAceFileType(nTotNucNum+1);
	vector<int> vAceAddress(nTotNucNum+1);
	vector<string> vAceFileName(nTotNucNum+1);
	vector<bool> vNucIsInXsdir(nTotNucNum+1);
	vector<int> vNucSeqInXsdir;  // nuclide sequence appearing in xsdir file

	for(int i = 1; i <= nTotNucNum; ++i)
	{
		vNucIsInXsdir[i] = false;
		vAceAddress[i] = 0 ;
	}

	///////////////// read xdir information for all nuclides /////////////
	string  sdata_path_key_word;
	CDFileIO::GetKeyWord(fptr_xsdir,sdata_path_key_word);
	Output.CheckInputParas(sdata_path_key_word.compare("DATAPATH") == 0,
		[&](){sprintf(Output.p_sPrintStr,"DATAPATH is expected in xsdir head!\n");},2);

	char chnextchar;
	CDFileIO::GetNextChar(fptr_xsdir,chnextchar);
	CDFileIO::SkipSpecChar(fptr_xsdir, '=');

	fscanf(fptr_xsdir,"%s",chDataPath);

	if(chDataPath[0] == '/')
	{
		strcat(chDataPath,"/");
	}
	else
	{
		strcat(chDataPath,"\\");
	}

	for(;;)
	{
		fscanf(fptr_xsdir,"%s",chNucIDTemp);
		if(strcmp("directory",chNucIDTemp)  ==0 )
		{
			break;
		}
		fgets(chread_s,200,fptr_xsdir);

		if(feof(fptr_xsdir))
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Keyword 'directory' is not found in xsdir file!\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	while(!feof(fptr_xsdir))
	{
		fscanf(fptr_xsdir,"%s",chNucIDTemp);
		for(int nuc = 1;nuc <= nTotNucNum; ++nuc)
		{
			if(vNucIsInXsdir[nuc])
			{
				continue;
			}
			
			//case insensitive, 20130704
			string sNucID_inp(p_vNuclides[nuc].p_ONucID.p_chIdStr);
			std::transform(sNucID_inp.begin(), sNucID_inp.end(),sNucID_inp.begin(), ::toupper);
			string sNucID_xsdir(chNucIDTemp);
			std::transform(sNucID_xsdir.begin(), sNucID_xsdir.end(),sNucID_xsdir.begin(), ::toupper);

			//if(strcmp(Nuclides[nuc].NucID.IdStr,NucIDTemp) == 0 )
			if(strcmp(sNucID_inp.c_str(),sNucID_xsdir.c_str()) == 0 )
			{
				//printf("%d/%d %s\n",nuc,TotNucNum,NucIDTemp);
				vNucIsInXsdir[nuc] = true;
				vNucSeqInXsdir.push_back(nuc);
				fscanf(fptr_xsdir,"%lf %s %s %d %d",&p_vNuclides[nuc].p_dNucAtomWgt,chread_s,
					chAccessRoute,&vAceFileType[nuc],&vAceAddress[nuc]);
				vAceFileName[nuc] =  chread_s;
				break;
			}
		}
		fgets(chread_s,200,fptr_xsdir);
	}
	fclose(fptr_xsdir);

	////////////////////////// check nuclide existing in xsdir file ////////////////////////
	bool bAllNucsFound = true;
	for(int nuc = 1;nuc <= nTotNucNum; ++nuc)
	{
		if(!vNucIsInXsdir[nuc])
		{		
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Nuclide %s is not found in xsdir file!\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
				Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		}
	}
	if(!bAllNucsFound || int(vNucSeqInXsdir.size())!=nTotNucNum)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Not all nuclides are found in xsdir file!\n");}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}

	////////// read ACE data  according to nuclide sequence appearing in xsdir file /////////
	bool bOpenNewFile = true;
	long lLinePtr = 1;
	for(int seq = 0 ; seq < nTotNucNum ; ++seq)
	{
		int nuc = vNucSeqInXsdir[seq];
		if(bOpenNewFile)
		{
			strcpy(chFullPath,chDataPath);
			strcat(chFullPath,vAceFileName[nuc].c_str());
			fptr_ace = fopen(chFullPath,"rb");
			if (fptr_ace == NULL)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Ace library %s not found.\n",chFullPath);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}
		////////// Read decimal type ACE library //////////////
		if(vAceFileType[nuc] ==1)   // decimal type
		{
			//Locate the ACE data (optimize speed?)
			if(vAceAddress[nuc] < lLinePtr)
			{
				lLinePtr = 1;
				fseek(fptr_ace,0,SEEK_SET);
			}
			for(int i = 1;i <= vAceAddress[nuc] - lLinePtr;i++)    
			{
				fgets(chread_s,100,fptr_ace);
			}  
			fscanf(fptr_ace,"%s",chread_s); 

			if(fscanf(fptr_ace,"%lf %lf",&p_vNuclides[nuc].p_dNucAtomWgt,&p_vNuclides[nuc].p_dNucTmp) < 2) 
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown atom weight and temperature. file location = %s\n",chread_s);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}

			if(nuc <= nTotNonSabNucNum)
			{
				if(p_vNuclides[nuc].p_dNucAtomWgt <= 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"negative atom weight of nuclide %s.\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				if(p_vNuclides[nuc].p_dNucTmp < 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"negative atom temperature of nuclide %s.\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				for(int i = 1; i <= 6;++i)
				{
					fgets(chread_s,100,fptr_ace);
				}
			}
			else  // sab library
			{
				fgets(chread_s,100,fptr_ace);
				fgets(chread_s,100,fptr_ace);
				fscanf(fptr_ace,"%d",&p_vNuclides[nuc].p_nNucZaid); 
				for(int i=1;i<=4;i++){fgets(chread_s,100,fptr_ace);}
			}
			//NXS & JXS
			for(int i=1;i<=16;i++)
			{
				fscanf(fptr_ace,"%d",&p_vNuclides[nuc].NXS[i]);
			}
			for(int i=1;i<=32;i++)
			{
				fscanf(fptr_ace,"%d",&p_vNuclides[nuc].JXS[i]);
			}		
			if(nuc <= nTotNonSabNucNum) 
			{
				p_vNuclides[nuc].p_nNucZaid = GetNuclideZA(nuc);
			}
			//XSS
			p_vNuclides[nuc].XSS.resize(GetXSSLength(nuc)+1); //XSS length
			for(int i = 1;i <= GetXSSLength(nuc);i++)
			{
				fscanf(fptr_ace,"%lf",&p_vNuclides[nuc].XSS[i]);    //
			}
			fgets(chread_s,100,fptr_ace);
		}
		////////// Read binary type ACE library //////////////
		else if(vAceFileType[nuc] == 2)  
		{
			fseek(fptr_ace,(vAceAddress[nuc]-1)*4096,SEEK_SET); //Locate start line
			char chHZ[10],chHD[10],chHK[70],chHM[10];
			int nIZ;
			double dtemp;
			fread(&chHZ,10,1,fptr_ace);
			fread(&p_vNuclides[nuc].p_dNucAtomWgt,8,1,fptr_ace);
			fread(&p_vNuclides[nuc].p_dNucTmp,8,1,fptr_ace);
			fread(&chHD,10,1,fptr_ace);
			fread(&chHK,70,1,fptr_ace);
			fread(&chHM,10,1,fptr_ace);
			if(nuc <= nTotNonSabNucNum)
			{
				if(p_vNuclides[nuc].p_dNucAtomWgt <= 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"negative atom weight of nuclide %s.\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				if(p_vNuclides[nuc].p_dNucTmp < 0)
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"negative atom temperature of nuclide %s.\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
						Output.p_fpOutputFilePtr, CDOutput::_ERROR);
				}
				for (int i=1;i<=16;i++)
				{
					fread(&nIZ,4,1,fptr_ace);
					fread(&dtemp,8,1,fptr_ace);
				}
			}
			else  /////// sab library case
			{
				fread(&p_vNuclides[nuc].p_nNucZaid,4,1,fptr_ace);
				fread(&dtemp,8,1,fptr_ace);
				for (int i=1;i<=15;i++)
				{
					fread(&nIZ,4,1,fptr_ace);
					fread(&dtemp,8,1,fptr_ace);
				}
			}

			//////// Read NXS & JXS  ///////
			fread(&p_vNuclides[nuc].NXS[1],4,16,fptr_ace); 
			fread(&p_vNuclides[nuc].JXS[1],4,32,fptr_ace); 

			if(nuc <= nTotNonSabNucNum)
			{
				p_vNuclides[nuc].p_nNucZaid = GetNuclideZA(nuc);
			}  

			////// read XSS
			fseek(fptr_ace,vAceAddress[nuc]*4096,SEEK_SET);
			p_vNuclides[nuc].XSS.resize(GetXSSLength(nuc) + 1);  //XSS length
			fread(&(p_vNuclides[nuc].XSS[1]),8,GetXSSLength(nuc),fptr_ace);   
		}

		else
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown library format of nuclide %s !\n",p_vNuclides[nuc].p_ONucID.p_chIdStr);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}

		/// check next ACE file
		if(seq < nTotNucNum - 1 )
		{
			int NextNuc =  vNucSeqInXsdir[seq+1];
			if(vAceFileName[nuc].compare(vAceFileName[NextNuc]) == 0)
			{
				bOpenNewFile = false;
				if(vAceFileType[nuc] == 1)
				{
					lLinePtr = vAceAddress[nuc] + 12 + (GetXSSLength(nuc) + 3)/4;
				}
				continue;
			}
			else
			{
				bOpenNewFile = true;
				lLinePtr = 1;
				fclose(fptr_ace);
				continue;
			}
		}

		fclose(fptr_ace);
	}

	////////////////////////////// Treat ACE data //////////////////////////////
	if(p_bIsMultiGroup)
	{
		CheckMgAceBlock(nTotNonSabNucNum);
	}
	else
	{
		CheckCeAceBlock(nTotNucNum, nTotNonSabNucNum);
	}

	//////////////////////////////// Finish Reading ////////////////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Finish reading XSDIR/ACE library.\n");}, Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

    return;
}