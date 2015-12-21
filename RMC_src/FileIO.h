# if ! defined __FILEIO__H__
# define  __FILEIO__H__

# include "CommonHead.h"

using namespace std;
const int LINE_MAX_LEN = 20000;


class CDFileIO {
public:
	static int GetNextChar(FILE *fptr,char &NextChar)
	{
		/// Skip blanks and comments, and get next char
		/// 0:  Not change line
		/// 1:  Change one line, the new line starts with space.      -- current card is not end
		/// 2:  Change one line, the new line starts with non-space.  -- current card is end, but current block is not end
		/// 3:  Change lines, blank lines are skipped.                -- current block is end
		/// 4:  File end 
		char s[LINE_MAX_LEN];
		long lPos;
		int nRead_p;
		int nLineCount = 0;
		int nSpaceCount = 0;
		int nCommentCount = 0;
		bool bFisrtSkipLineIsComment = false;

		for(;;)
		{
			lPos = ftell(fptr);
			nRead_p = fscanf(fptr,"%c",&NextChar);
			if(nRead_p <= 0){return 4;}   // 4:  file end   
		//	if(NextChar=='\r' || NextChar=='\n' || NextChar=='/')  
			if(NextChar=='\r' || NextChar=='\n' || NextChar=='/'|| NextChar=='#')  
			{
				nLineCount++;
				nSpaceCount = 0;
				fseek(fptr,lPos,SEEK_SET);
				fgets(s,LINE_MAX_LEN,fptr);
				//if(NextChar=='/'){ nCommentCount++; }
				if(NextChar=='/' || NextChar=='#'){ nCommentCount++; }
				if(nLineCount == 1)
				{
					//if(NextChar=='/')
					if(NextChar=='/' || NextChar=='#')
					{ 
						bFisrtSkipLineIsComment = true;
					}
				}
			}
			else if(NextChar==' ' || NextChar=='\t')
			{
				nSpaceCount++; // new line is a blank line beginning with space
			}
			else if(NextChar < 0)
			{
				char str_line[LINE_MAX_LEN];
				fgets(str_line,LINE_MAX_LEN,fptr);
				printf("Error: unknown character %c in input file.\n%s\nPress any key to exit...",NextChar,str_line);
				exit(1);
			}
			else
			{
				break;
			} 
		}
		fseek(fptr,lPos,SEEK_SET);

		if(nLineCount == 0)///0:  Line not changed.
		{
			return 0;
		}                 
		else if(nLineCount == 1)
		{
			return ((nSpaceCount > 0)? 1 : 2);
		}
		else if(nLineCount > 1) 
		{
			if(nCommentCount == nLineCount || (!bFisrtSkipLineIsComment && nLineCount - nCommentCount == 1))
			{
				return ((nSpaceCount > 0)? 1 : 2);
			}
			return 3;
		}

		printf("Error: failed reading next char.\nPress any key to exit...");
		exit(1);
	}

	static void SkipChars(FILE *fptr,int nSkipCharNum) // skip # number of chars
	{
		char NextChar;
		char chSkip;
		for(int i = 1 ; i <= nSkipCharNum; ++i)
		{
			GetNextChar(fptr,NextChar);
			fscanf(fptr,"%c",&chSkip);
		}
		return;
	}

	static void SkipSpecChar(FILE *fptr,char SpecChar) // skip one specific Char
	{
		char NextChar;
		char chSkip;
		GetNextChar(fptr,NextChar);
		if(NextChar == SpecChar)
		{
			fscanf(fptr,"%c",&chSkip);
		}
		else
		{      
			char chStr_line[LINE_MAX_LEN];
			fgets(chStr_line,LINE_MAX_LEN,fptr);
			printf("Error: failed reading expected char %c.\n%s\nPress any key to exit...",SpecChar,chStr_line);
			exit(1);
		}
		return;
	}

	static void GetKeyWord(FILE *fptr,string &sKeyWord)
	{
		sKeyWord.clear();

		char NextChar;
		if( GetNextChar(fptr,NextChar) > 0)
		{
			char chStr_line[LINE_MAX_LEN];
			fgets(chStr_line,LINE_MAX_LEN,fptr);
			printf("Error: failed getting keyword.\n%s\nPress any key to exit...",chStr_line);
			exit(1);
		}

		for(;;)
		{
			long nPos = ftell(fptr);
			if(fscanf(fptr,"%c",&NextChar) < 1) // file end
			{
				std::transform(sKeyWord.begin(), sKeyWord.end(),sKeyWord.begin(), ::toupper);
				return; 
			}   

			if(NextChar == ' ' || NextChar == '=' || NextChar == '\t' || NextChar=='\r' || NextChar=='\n')
			{
				fseek(fptr,nPos,SEEK_SET);
				std::transform(sKeyWord.begin(), sKeyWord.end(),sKeyWord.begin(), ::toupper);
				return;
			}
			else if(NextChar < 0)
			{
				char chStr_line[LINE_MAX_LEN];
				fgets(chStr_line,LINE_MAX_LEN,fptr);
				printf("Error: unknown character %c in input file.\n%s\nPress any key to exit...",NextChar,chStr_line);
				exit(1);
			}
			else
			{
				sKeyWord.push_back(NextChar);
			}
		}
		return;
	}

	static void SI_GetKeyWord(FILE *fptr,string &sKeyWord)
	{
		sKeyWord.clear();

		char NextChar;
		if( GetNextChar(fptr,NextChar) == 4)
		{
			char chStr_line[LINE_MAX_LEN];
			fgets(chStr_line,LINE_MAX_LEN,fptr);
			printf("Error: failed getting keyword.\n%s\nPress any key to exit...",chStr_line);
			exit(1);
		}

		for(;;)
		{
			long nPos = ftell(fptr);
			if(fscanf(fptr,"%c",&NextChar) < 1) // file end
			{
				std::transform(sKeyWord.begin(), sKeyWord.end(),sKeyWord.begin(), ::toupper);
				return; 
			}   

			if(NextChar == ' ' || NextChar == '=' || NextChar == '\t' || NextChar=='\r' || NextChar=='\n'||NextChar == ':'||NextChar == '"'||NextChar == ',')
			{
				fseek(fptr,nPos,SEEK_SET);
				std::transform(sKeyWord.begin(), sKeyWord.end(),sKeyWord.begin(), ::toupper);
				return;
			}
			else if(NextChar < 0)
			{
				char chStr_line[LINE_MAX_LEN];
				fgets(chStr_line,LINE_MAX_LEN,fptr);
				printf("Error: unknown character %c in input file.\n%s\nPress any key to exit...",NextChar,chStr_line);
				exit(1);
			}
			else
			{
				sKeyWord.push_back(NextChar);
			}
		}
		return;
	}



	template<typename type>	static int ReadVaryVec(FILE *fptr,vector<type> &vT,char *pFmt)
	{
		//// read parameters of varying length
		type Read_temp;
		int nNext_p = 0;
		char nextchar;

		nNext_p = GetNextChar(fptr,nextchar);
		if(nNext_p >= 2)       
		{
			return nNext_p;   ///// current block or current card is end
		} 
		for(;;)
		{
			long lPos = ftell(fptr);//bug: avoid pointer being changed falsely
			// read one parameter
			if(fscanf(fptr,pFmt, &Read_temp) <= 0)
			{
				fseek(fptr,lPos,SEEK_SET);
				break;   ///// current block and current card is not end
			}

			//Get next Char for * and : expressions
			nNext_p = GetNextChar(fptr,nextchar);

			// read repeat parameters, as "8.3*5"=8.3 8.3 8.3 8.3 8.3
			if (nextchar == '*')
			{
				ReadRepeatPara(fptr,Read_temp,vT);
				continue;
			}
			// read arithmetic sequences parameters, as "11:15"=11 12 13 14 15, "10:2:16"=10 12 14 16
			else if (nextchar == ':')
			{
				ReadColonPara(fptr,Read_temp,vT,pFmt, nNext_p, nextchar);
				if (nNext_p >= 2)
				{
					break;// break for next option/card/block
				}
				continue;
			}
			// other character, then save one parameter
			else
			{
				vT.push_back(Read_temp);
			}

			if(nNext_p >= 2)       
			{
				break;  // break for next option/card/block
			} 
		}
		return nNext_p;
	}

	template<typename type>	static int SI_ReadVaryVec(FILE *fptr,vector<type> &vT,char *pFmt)
	{
		//// Cannot read colon : expression 不支持冒号表达式功能！
		//// read parameters of varying length
		type Read_temp;
		int nNext_p = 0;
		char nextchar;
		for(;;)
		{
			nNext_p = GetNextChar(fptr,nextchar);
			//if(nNext_p >= 2)       
			//{
			//	break;   ///// current block or current card is end
			//} 
			if (nextchar ==',')	SkipSpecChar(fptr, ',');
			/*	if (nextchar!=',')*/
			else
			{
				if(nextchar == '$'||nextchar == '%'||nNext_p == 4)       
				{
					break;   ///// current block or current card is end
				} 
			}

			if(nextchar == '*')   ///// read repeated parameters
			{	
				if (vT.size() == 0)
				{
					char str_line[LINE_MAX_LEN];
					fgets(str_line,LINE_MAX_LEN,fptr);
					printf("Error: incorrect repeated parameter following *.\n%s\nPress any key to exit...",str_line);
					exit(1);
				}
				int nNum = vT.size() - 1;
				type RepeatVal = vT[nNum];
				vT.resize(nNum);// delete last value , because next function will add it again
				ReadRepeatPara(fptr,RepeatVal,vT);
				continue;
			}

			//if (nextchar ==',')	SkipSpecChar(fptr, ',');
			//else 
			//{
			//	if(fscanf(fptr,pFmt, &Read_temp) <= 0)
			//	{
			//		break;   ///// current block and current card is not end
			//	}
			//}
			nNext_p = GetNextChar(fptr,nextchar);
			if (nextchar ==',')	SkipSpecChar(fptr, ',');
			/*if (nextchar!=',')*/
			else
			{
				//if(nextchar == '$'||nextchar == '%'||nNext_p == 4)       
				//{
				//	break;   ///// current block or current card is end
				//} 
				if(fscanf(fptr,pFmt, &Read_temp) <= 0)
				{
					break;   ///// current block and current card is not end
				}
			}


			vT.push_back(Read_temp);
		}
		return nNext_p;
	}

	template<typename type>	static int ReadRepeatPara(FILE *fptr,type RepeatVal,vector<type> &vT)
	{
		int nRepeatNum;

		SkipSpecChar(fptr, '*');	
		if(fscanf(fptr,"%d",&nRepeatNum) < 1 || nRepeatNum < 0)
		{
			char str_line[LINE_MAX_LEN];
			fgets(str_line,LINE_MAX_LEN,fptr);
			printf("Error: incorrect repeated parameter following *.\n%s\nPress any key to exit...",str_line);
			exit(1);
		}
		for(int i = 1 ; i <= nRepeatNum; ++i)
		{
			vT.push_back(RepeatVal);
		}
		return nRepeatNum;
	}

	template<typename type>	static int ReadColonPara(FILE *fptr,type RepeatVal,vector<type> &vT,char *pFmt,int &nNext_p, char &nextchar)
	{
		type StartVal,MidVal,EndVal,Read_temp;
		StartVal = RepeatVal;

		SkipSpecChar(fptr, ':');
		if(fscanf(fptr,pFmt, &Read_temp) <= 0)//first value after :
		{
			char str_line[LINE_MAX_LEN];
			fgets(str_line,LINE_MAX_LEN,fptr);
			printf("Error: incorrect arithmetic sequences parameters with ':'.\n%s\nPress any key to exit...\n",str_line);
			exit(1);
		}
		nNext_p = GetNextChar(fptr,nextchar);
		if(nextchar == ':')//two colon, as 3:2:7
		{
			SkipSpecChar(fptr, ':');
			MidVal = Read_temp;
			if(MidVal == 0 || fscanf(fptr,pFmt, &Read_temp) <= 0)
			{
				char str_line[LINE_MAX_LEN];
				fgets(str_line,LINE_MAX_LEN,fptr);
				printf("Error: incorrect arithmetic sequences parameters with ':'.\n");
				if (MidVal == 0)
				{
					printf("common difference cannot be 0 !\n");
				}
				else
				{
					printf("%s\n",str_line);
				}
				printf("\nPress any key to exit...\n");
				exit(1);
			}
			EndVal = Read_temp;
		}
		else//one colon, as 1:10
		{
			MidVal = 1;
			EndVal = Read_temp;
		}
		int nArrayNum = (int)((EndVal - StartVal)/MidVal);
		type Redundancy = (EndVal - StartVal)/MidVal- nArrayNum ;
		if(Redundancy > 0.00001 || Redundancy < -0.00001 || nArrayNum < 0 || nArrayNum > 100000000)
		{
			cerr<<"\n Error: incorrect arithmetic sequences parameters with ':'.\n";
			cerr<<"   start- "<<StartVal<<" difference- "<<MidVal<<" end- "<<EndVal<<" length- "<< nArrayNum + 1;
			if (nArrayNum > 100000000)
			{
				cerr<<endl<<"length too large!";
			}
			cerr<<"\nPress any key to exit...\n";
			exit(1);
		}
		for(int i = 0 ; i <= nArrayNum; ++i)
		{
			vT.push_back(StartVal+i*MidVal);
		}
		return nArrayNum + 1;
	}

};

#endif