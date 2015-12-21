# include "Tally.h"
# include "Criticality.h"
# include "Convergence.h"

void CDTally::OutputTally(int nPrintFlag, CDCriticality &cCriticality, CDMaterial &cMaterial)
{
	//////////////// parallel case /////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	//////////////// normal case /////////////////
	if(nPrintFlag!=0 && nPrintFlag!=1)
	{
		printf("Wrong flag for printing cell tally\n");
		return;
	}

	enum ePrintFlag{CycPrint = 0, FinalPrint = 1};
	if(nPrintFlag == CycPrint)
	{
		fprintf(Output.p_fpCycTallyFilePtr,"\n ============== Tally information at cycle %d ==============\n",cCriticality.p_nCurrentCYCLE) ;
	}

	//////////////////////// output cell tally ///////////////////////////
	OutputCellTally(nPrintFlag, cCriticality);


	///////////////////// output cross-section tally /////////////////////
	OutputMeshTally(nPrintFlag);


	///////////////////// output cross-section tally /////////////////////
	OutputCsTally(nPrintFlag, cMaterial);

	return;
}

void CDTally::OutputCellTally(int nPrintFlag, CDCriticality &cCriticality)
{

	//////////////////////////////without cell tally/////////////////////////
	if(!p_bIsWithCellTally)
	{
		return;
	}

	/////////////// Judge print flag: cycle tally or final tally /////////////////////
	enum ePrintFlag{CycPrint = 0, FinalPrint = 1};
	FILE *fpFilePtr;
	if(nPrintFlag == CycPrint)
	{
		fpFilePtr = Output.p_fpCycTallyFilePtr;
	}
	else if(nPrintFlag == FinalPrint)
	{
		fpFilePtr = Output.p_fpTallyFilePtr;
	}


	///////////////////// switched off ////////////////////////
	if(!Output.p_bIsCellTallyPrint)
	{
		fprintf(fpFilePtr,"\nCellTally printing is switched off.\n") ;
		return;
	}


	//////////////////////////////output cell tally /////////////////////////
	char chTallyType[][80] = {"None","flux(¦Õ)",  "power(qf*¦²f*¦Õ)","fission rate(¦²f*¦Õ)","absorption rate(¦²a*¦Õ)","fission neutron production rate(¦Ô¦²f*¦Õ)"};

	char chTableTitleStr[][80]={"Cyc            Sum","Ave            RE"};

	double *pScore1, *pScore2;
	pScore1 = nPrintFlag == CycPrint? &p_OCellTallyData.p_vScore[0] : &p_OCellTallyData.p_vAve[0];
	pScore2 = nPrintFlag == CycPrint? &p_OCellTallyData.p_vSum1[0] : &p_OCellTallyData.p_vRe[0];

	fprintf(fpFilePtr,"\n================================ Tally Table Start ================================ \n");

	if(cCriticality.p_bUseBatch)
	{
		fprintf(fpFilePtr,"======================= Batch tally is used. Batch size = %d ===================== \n",cCriticality.p_nBatchNum);
	}

	fprintf(fpFilePtr,"============================= %d cell tally in total ==============================\n",p_vCellTally.size());
	for(int n = 0; n < p_vCellTally.size(); ++n)
	{
		int nCellSize = p_vCellTally[n].p_nCellBinSize;
		// output identifier, type, cell number
		fprintf(fpFilePtr,"\n------------------ ID = %d,  Type = %s, Number of cell bins  = %d ------------------\n",
			p_vCellTally[n].p_nTallyID,chTallyType[p_vCellTally[n].p_nTallyType],nCellSize);

		// output table head
		if(p_vCellTally[n].p_vErgBins.size() > 0) // output sub tallies if energy bin is present
		{
			fprintf(fpFilePtr,"Cell                          Group       Energy Bin         %s\n",chTableTitleStr[nPrintFlag]);
		}
		else
		{
			fprintf(fpFilePtr,"Cell                                 %s\n",chTableTitleStr[nPrintFlag]);
		}

		// output tallies for individual cell
		int nCellI = -1 ;
		int nDataPtr = p_vCellTally[n].p_nDataStartPtr - 1;
		for(int i = 0; i < nCellSize; ++i)
		{
			// output cell
			++ nCellI;
			fprintf(fpFilePtr,"%-30s",Output.PrintCellVec(p_vCellTally[n].p_vCellVecListU[nCellI]));
			if( p_vCellTally[n].p_vIntgrlBins.size() > 0 ) // integral tally case
			{
				for(;;)
				{
					if(nCellI == int(p_vCellTally[n].p_vCellVecListU.size()) - 1)
					{
						break;
					}
					if(p_vCellTally[n].p_vIntgrlIndex[nCellI + 1] != i)
					{
						break;
					}
					++ nCellI;
					fprintf(fpFilePtr,"\n%-30s",Output.PrintCellVec(p_vCellTally[n].p_vCellVecListU[nCellI]));
				}
			}

			// output tally data 
			int nErgBinNum = p_vCellTally[n].p_vErgBins.size();
			if(nErgBinNum > 0) // output sub tallies if energy bin is present
			{
				for(int j = 0;j < nErgBinNum; ++j)
				{
					++ nDataPtr;

					if(j == 0)
					{
						fprintf(fpFilePtr,"  %-4d      %5.4E     %5.4E     %5.4E\n",
							j+1, p_vCellTally[n].p_vErgBins[j], pScore1[nDataPtr], pScore2[nDataPtr]);
					}
					else
					{
						fprintf(fpFilePtr,"                                %-4d      %5.4E     %5.4E     %5.4E\n",
							j+1, p_vCellTally[n].p_vErgBins[j], pScore1[nDataPtr], pScore2[nDataPtr]);
					}
				}
				++ nDataPtr;
				fprintf(Output.p_fpTallyFilePtr,"                               Tot                       %5.4E     %5.4E\n",
					pScore1[nDataPtr], pScore2[nDataPtr]);
			}
			else
			{
				++ nDataPtr;
				fprintf(fpFilePtr,"   %5.4E      %5.4E\n",pScore1[nDataPtr], pScore2[nDataPtr]);
			}
		}
	}
	return;
}



void CDTally::OutputMeshTally(int nPrintFlag)
{

	//////////////////////////////without cell tally/////////////////////////
	if(!p_bIsWithMeshTally)
	{
		return;
	}

	/////////////// Judge print flag: cycle tally or final tally /////////////////////
	enum ePrintFlag{CycPrint = 0, FinalPrint = 1};
	FILE *fpFilePtr;
	if(nPrintFlag == CycPrint)
	{
		fpFilePtr = Output.p_fpCycTallyFilePtr;
	}
	else if(nPrintFlag == FinalPrint)
	{
		fpFilePtr = Output.p_fpTallyFilePtr;
	}


	///////////////////// switched off ////////////////////////
	if(!Output.p_bIsMeshTallyPrint)
	{
		fprintf(fpFilePtr,"\nMeshTally printing is switched off.\n") ;
		return;
	}


	//////////////////////////////output cell tally /////////////////////////
	char chTallyType[][80] = {"None","flux",  "power","fission rate","absorption rate"};

	char chTableTitleStr[][80]={"Cyc            Sum","Ave            RE"};

	double *pScore1, *pScore2;
	pScore1 = nPrintFlag == CycPrint? &p_OMeshTallyData.p_vScore[0] : &p_OMeshTallyData.p_vAve[0];
	pScore2 = nPrintFlag == CycPrint? &p_OMeshTallyData.p_vSum1[0] : &p_OMeshTallyData.p_vRe[0];


	fprintf(fpFilePtr,"============================= %d Mesh tally in total ==============================\n",p_vMeshTally.size());
	for(int n = 0; n < p_vMeshTally.size(); ++n)
	{
		int nMeshNum = p_vMeshTally[n].p_OTallyMesh.GetTotMeshNum();;
		// output identifier, type, cell number
		fprintf(fpFilePtr,"\n------------------ ID = %d,  Type = %s, Number of mesh grids  = %d ------------------\n",
			p_vMeshTally[n].p_nTallyID,chTallyType[p_vMeshTally[n].p_nTallyType],nMeshNum);

		// output table head
		if(p_vMeshTally[n].p_vErgBins.size() > 0) // output sub tallies if energy bin is present
		{
			fprintf(fpFilePtr,"Mesh index(x y z)             Group       Energy Bin         %s\n",chTableTitleStr[nPrintFlag]);
		}
		else
		{
			fprintf(fpFilePtr,"Mesh index(x y z)                    %s\n",chTableTitleStr[nPrintFlag]);
		}

		// output tallies for individual mesh index
		int nDataPtr = p_vMeshTally[n].p_nDataStartPtr - 1;
		int nNumX = std::max(p_vMeshTally[n].p_OTallyMesh.p_nMeshNum[0],1);
		int nNumY = std::max(p_vMeshTally[n].p_OTallyMesh.p_nMeshNum[1],1);
		int nNumZ = std::max(p_vMeshTally[n].p_OTallyMesh.p_nMeshNum[2],1);
		for(int nIndexZ = 1; nIndexZ <= nNumZ; ++nIndexZ)
		{
			for(int nIndexY = 1; nIndexY <= nNumY; ++nIndexY)
			{
				for(int nIndexX = 1; nIndexX <= nNumX; ++nIndexX)
				{
					// output mesh index
					fprintf(fpFilePtr,"%-6d %-6d %-6d          ",nIndexX,nIndexY,nIndexZ);

					// output tally data 
					int nErgBinNum = p_vMeshTally[n].p_vErgBins.size();
					if(nErgBinNum > 0) // output sub tallies if energy bin is present
					{
						for(int j = 0;j < nErgBinNum; ++j)
						{
							++ nDataPtr;
							if(j == 0)
							{
								fprintf(fpFilePtr,"  %-4d      %5.4E     %5.4E     %5.4E\n",
									j+1, p_vMeshTally[n].p_vErgBins[j], pScore1[nDataPtr], pScore2[nDataPtr]);
							}
							else
							{
								fprintf(fpFilePtr,"                                %-4d      %5.4E     %5.4E     %5.4E\n",
									j+1, p_vMeshTally[n].p_vErgBins[j], pScore1[nDataPtr], pScore2[nDataPtr]);
							}
						}
						++ nDataPtr;
						fprintf(Output.p_fpTallyFilePtr,"                               Tot                       %5.4E     %5.4E\n",
							pScore1[nDataPtr], pScore2[nDataPtr]);
					}
					else
					{
						++ nDataPtr;
						fprintf(fpFilePtr,"   %5.4E      %5.4E\n",pScore1[nDataPtr], pScore2[nDataPtr]);
					}
				}
			}
		}
	}
	return;
}


void CDTally::OutputCsTally(int nPrintFlag, CDMaterial &cMaterial)
{
	if(!p_bIsWithCsTally)
	{
		return;
	}

	/////////////// Judge print flag: cycle tally or final tally /////////////////////
	enum ePrintFlag{CycPrint = 0, FinalPrint = 1};
	FILE *fpFilePtr;
	if(nPrintFlag == CycPrint)
	{
		fpFilePtr = Output.p_fpCycTallyFilePtr;
	}
	else if(nPrintFlag == FinalPrint)
	{
		fpFilePtr = Output.p_fpTallyFilePtr;
	}


	if(!Output.p_bIsCsTallyPrint)
	{
		fprintf(fpFilePtr,"\nCstally printing is switched off.\n") ;
		return;
	}


	//////////////////////// output cross-section tally ///////////////////////
	char chTableTitleStr[][80]={"Cyc            Sum","Ave            RE"};
	double *pScore1, *pScore2;
	pScore1 = nPrintFlag == CycPrint? &p_OCsTallyData.p_vScore[0] : &p_OCsTallyData.p_vAve[0];
	pScore2 = nPrintFlag == CycPrint? &p_OCsTallyData.p_vSum1[0] : &p_OCsTallyData.p_vRe[0];

	int nCsTallyNum = p_vOneGroupCsTally.size();;
	fprintf(fpFilePtr,"\n========================== %d cross-section tally in total ========================\n",nCsTallyNum);
	for(int i = 0; i < nCsTallyNum; ++i)
	{
		int nId = p_vOneGroupCsTally[i].p_nTallyID;
		string sCellVec = Output.PrintCellVec(p_vOneGroupCsTally[i].p_vCellVecU);
		int nMat = p_vOneGroupCsTally[i].p_nMat ;
		int nMatU = cMaterial.p_OMatSetIndex.GetIndexU(nMat);
		int nPtr = p_vOneGroupCsTally[i].p_nDataStartPtr;


		fprintf(fpFilePtr,"ID = %d, Cell = %s,Mat = %d \n",nId,sCellVec.c_str(),nMatU);
		fprintf(fpFilePtr,"Total Flux:  %5.4E      RE:  %5.4E\n",pScore1[nPtr],pScore2[nPtr]);
		fprintf(fpFilePtr," Nuclide       MT           %s\n",chTableTitleStr[nPrintFlag]);

		int nNucNum = cMaterial.GetMatTotNucNum(nMat) ;
		for(int j = 0; j < nNucNum; ++j )
		{
			int nNuc = cMaterial.GetMatNucIndex(nMat,j);
			fprintf(fpFilePtr,"%-10s",cMaterial.p_vNucIDList[nNuc].p_chIdStr);

			int nMtNum = p_vOneGroupCsTally[i].p_vNucMTs[j].size();
			for(int k = 0; k < nMtNum; ++k )
			{
				int nMt = p_vOneGroupCsTally[i].p_vNucMTs[j][k];

				nPtr++ ;
				if(k == 0)
				{
					fprintf(fpFilePtr,"     %-3d      %5.4E      %5.4E\n",nMt,pScore1[nPtr],pScore2[nPtr]);
				}
				else
				{
					fprintf(fpFilePtr,"               %-3d      %5.4E      %5.4E\n",nMt,pScore1[nPtr],pScore2[nPtr]);
				}
			}
		}
	}
	return;
}


void CDTally::OutputTally(int print_flag, CDFixedSource &cFixedSource, CDMaterial &CMaterial)
{
	//////////////// parallel case /////////////////
# ifdef _IS_PARALLEL_
	if(!OParallel.p_bIsMaster)
	{
		return;
	}
# endif

	//////////////// normal case /////////////////
	if(print_flag!=0 && print_flag!=1)
	{
		printf("Wrong flag for printing cell tally\n");
		return;
	}

	enum PrintFlag{CycPrint = 0, FinalPrint = 1};
	if(print_flag == CycPrint)
	{
		fprintf(Output.p_fpCycTallyFilePtr,"\n ============== Tally information at cycle %d ==============\n",0) ;
	}

	//////////////////////// output cell tally ///////////////////////////
	OutputCellTally(print_flag, cFixedSource);


	///////////////////// output cross-section tally /////////////////////
	OutputMeshTally(print_flag);


	///////////////////// output cross-section tally /////////////////////
	OutputCsTally(print_flag, CMaterial);

	return;
}


void CDTally::OutputCellTally(int print_flag, CDFixedSource &cFixedSource)
{

	//////////////////////////////without cell tally/////////////////////////
	if(!p_bIsWithCellTally)
	{
		return;
	}

	/////////////// Judge print flag: cycle tally or final tally /////////////////////
	enum PrintFlag{CycPrint = 0, FinalPrint = 1};
	FILE *file_ptr;
	if(print_flag == CycPrint)
	{
		file_ptr = Output.p_fpCycTallyFilePtr;
	}
	else if(print_flag == FinalPrint)
	{
		file_ptr = Output.p_fpTallyFilePtr;
	}


	///////////////////// switched off ////////////////////////
	if(!Output.p_bIsCellTallyPrint)
	{
		fprintf(file_ptr,"\nCellTally printing is switched off.\n") ;
		return;
	}


	//////////////////////////////output cell tally /////////////////////////
	char tally_type[][80] = {"None","flux",  "power","fission rate","absorption rate"};

	char table_title_str[][80]={"Cyc            Sum","Ave            RE"};

	double *score1, *score2;
	score1 = print_flag == CycPrint? &p_OCellTallyData.p_vScore[0] : &p_OCellTallyData.p_vAve[0];
	score2 = print_flag == CycPrint? &p_OCellTallyData.p_vSum1[0] : &p_OCellTallyData.p_vRe[0];

	fprintf(file_ptr,"\n================================ Tally Table Start ================================ \n");


	fprintf(file_ptr,"============================= %d cell tally in total ==============================\n",p_vCellTally.size());
	for(int n = 0; n < p_vCellTally.size(); ++n)
	{
		int CellSize = p_vCellTally[n].p_nCellBinSize;
		// output identifier, type, cell number
		fprintf(file_ptr,"\n------------------ ID = %d,  Type = %s, Number of cell bins  = %d ------------------\n",
			p_vCellTally[n].p_nTallyID,tally_type[p_vCellTally[n].p_nTallyType],CellSize);

		// output table head
		if(p_vCellTally[n].p_vErgBins.size() > 0) // output sub tallies if energy bin is present
		{
			fprintf(file_ptr,"Cell                          Group       Energy Bin         %s\n",table_title_str[print_flag]);
		}
		else
		{
			fprintf(file_ptr,"Cell                                 %s\n",table_title_str[print_flag]);
		}

		// output tallies for individual cell
		int Cell_i = -1 ;
		int DataPtr = p_vCellTally[n].p_nDataStartPtr - 1;
		for(int i = 0; i < CellSize; ++i)
		{
			// output cell
			++ Cell_i;
			fprintf(file_ptr,"%-30s",Output.PrintCellVec(p_vCellTally[n].p_vCellVecListU[Cell_i]));
			if( p_vCellTally[n].p_vIntgrlBins.size() > 0 ) // integral tally case
			{
				for(;;)
				{
					if(Cell_i == int(p_vCellTally[n].p_vCellVecListU.size()) - 1)
					{
						break;
					}
					if(p_vCellTally[n].p_vIntgrlIndex[Cell_i + 1] != i)
					{
						break;
					}
					++ Cell_i;
					fprintf(file_ptr,"\n%-30s",Output.PrintCellVec(p_vCellTally[n].p_vCellVecListU[Cell_i]));
				}
			}

			// output tally data 
			int ErgBinNum = p_vCellTally[n].p_vErgBins.size();
			if(ErgBinNum > 0) // output sub tallies if energy bin is present
			{
				for(int j = 0;j < ErgBinNum; ++j)
				{
					++ DataPtr;

					if(j == 0)
					{
						fprintf(file_ptr,"  %-4d      %5.4E     %5.4E     %5.4E\n",
							j+1, p_vCellTally[n].p_vErgBins[j], score1[DataPtr], score2[DataPtr]);
					}
					else
					{
						fprintf(file_ptr,"                                %-4d      %5.4E     %5.4E     %5.4E\n",
							j+1, p_vCellTally[n].p_vErgBins[j], score1[DataPtr], score2[DataPtr]);
					}
				}
				++ DataPtr;
				fprintf(Output.p_fpTallyFilePtr,"                               Tot                       %5.4E     %5.4E\n",
					score1[DataPtr], score2[DataPtr]);
			}
			else
			{
				++ DataPtr;
				fprintf(file_ptr,"   %5.4E      %5.4E\n",score1[DataPtr], score2[DataPtr]);
			}
		}
	}
	return;
}