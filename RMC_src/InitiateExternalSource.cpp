# include "FixedSource.h"
# include "Convergence.h"



void CDFixedSource::InitiateExternalSource(CDNeutronTransport &cNeutronTransport, CDGeometry &cGeometry,CDAceData &cAceData,CDParticleState &cParticleState)
{
	double  dKsi1,dKsi2,dKsi3;
	enum eKsrcType{Point = 1, Slab = 2, Sph = 3, Cylx = 4, Cyly = 5, Cylz = 6};


	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Initiating fission source...");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	int nSourceCount = p_nNeuNum;



	/////////////////////// initiate fission source for parallel case /////////////////
# ifdef _IS_PARALLEL_
	{
		OParallel.BalanceFisBank(nSourceCount,p_nNeuNum, p_nNeuNum);
	}
#endif


	///////////////////////////// position ////////////////////////////////
	switch(p_nFixedSrcType)
	{
	case Point:
		{
			int nPointNum = p_vFixedSrcPara.size()/3;
			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif
				int j = int(ORNG.Rand()*nPointNum);
				p_vFixedSrc[i].p_dPos[0] = p_vFixedSrcPara[3*j];
				p_vFixedSrc[i].p_dPos[1] = p_vFixedSrcPara[3*j + 1];
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[3*j + 2];
			}
			break;
		}

	case Slab:
		{
			double dLenX = p_vFixedSrcPara[1]-p_vFixedSrcPara[0];
			double dLenY = p_vFixedSrcPara[3]-p_vFixedSrcPara[2];
			double dLenZ = p_vFixedSrcPara[5]-p_vFixedSrcPara[4];
			Output.CheckInputParas(dLenX > 0 && dLenY > 0 && dLenZ > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial SLAB source.\n");},2);

			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif
				p_vFixedSrc[i].p_dPos[0] = p_vFixedSrcPara[0] + ORNG.Rand()*dLenX;
				p_vFixedSrc[i].p_dPos[1] = p_vFixedSrcPara[2] + ORNG.Rand()*dLenY;
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[4] + ORNG.Rand()*dLenZ;
			}
			break;
		}

	case Sph:
		{
			Output.CheckInputParas(p_vFixedSrcPara[3] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial SPH source.\n");},2);

			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif
				do{
					dKsi1 = 2*ORNG.Rand()-1;
					dKsi2 = 2*ORNG.Rand()-1;	
					dKsi3 = 2*ORNG.Rand()-1;	
				}while(dKsi1*dKsi1+dKsi2*dKsi2+dKsi3*dKsi3>1);   
				p_vFixedSrc[i].p_dPos[0] = p_vFixedSrcPara[0] + p_vFixedSrcPara[3]*dKsi1;
				p_vFixedSrc[i].p_dPos[1] = p_vFixedSrcPara[1] + p_vFixedSrcPara[3]*dKsi2;
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[2] + p_vFixedSrcPara[3]*dKsi3;
			}
			break;
		}

	case Cylx:
		{
			Output.CheckInputParas(p_vFixedSrcPara[2] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLX source.\n");},2);

			double Height = p_vFixedSrcPara[4]-p_vFixedSrcPara[3];
			Output.CheckInputParas(Height > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLX source.\n");},2);

			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif

				do{
					dKsi1 = 2*ORNG.Rand()-1;
					dKsi2 = 2*ORNG.Rand()-1;	
				}while(dKsi1*dKsi1+dKsi2*dKsi2>1);  
				p_vFixedSrc[i].p_dPos[1] = p_vFixedSrcPara[0] + p_vFixedSrcPara[2]*dKsi1;
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[1] + p_vFixedSrcPara[2]*dKsi2;
				p_vFixedSrc[i].p_dPos[0]  = p_vFixedSrcPara[3] + ORNG.Rand() * Height;
			}
			break;
		}

	case Cyly:
		{
			Output.CheckInputParas(p_vFixedSrcPara[2] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLY source.\n");},2);

			double Height = p_vFixedSrcPara[4]-p_vFixedSrcPara[3];
			Output.CheckInputParas(Height > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLY source.\n");},2);

			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif
				do{
					dKsi1 = 2*ORNG.Rand()-1;
					dKsi2 = 2*ORNG.Rand()-1;	
				}while(dKsi1*dKsi1+dKsi2*dKsi2>1);  
				p_vFixedSrc[i].p_dPos[0] = p_vFixedSrcPara[0] + p_vFixedSrcPara[2]*dKsi1;
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[1] + p_vFixedSrcPara[2]*dKsi2;
				p_vFixedSrc[i].p_dPos[1]  = p_vFixedSrcPara[3] + ORNG.Rand() *Height;
			}
			break;
		}

	case Cylz:
		{
			Output.CheckInputParas(p_vFixedSrcPara[2] > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLZ source.\n");},2);

			double Height = p_vFixedSrcPara[4]-p_vFixedSrcPara[3];
			Output.CheckInputParas(Height > 0,
				[&](){sprintf(Output.p_sPrintStr,"incorrect parameters for initial CYLZ source.\n");},2);

			for(int i = 0;i < nSourceCount; ++i)
			{
				//# ifdef _IS_PARALLEL_
				ORNG.GetRandSeed();
				//# endif
				do{
					dKsi1 = 2*ORNG.Rand()-1;
					dKsi2 = 2*ORNG.Rand()-1;	
				}while(dKsi1*dKsi1+dKsi2*dKsi2>1);  
				p_vFixedSrc[i].p_dPos[0] = p_vFixedSrcPara[0] + p_vFixedSrcPara[2]*dKsi1;
				p_vFixedSrc[i].p_dPos[1] = p_vFixedSrcPara[1] + p_vFixedSrcPara[2]*dKsi2;
				p_vFixedSrc[i].p_dPos[2] = p_vFixedSrcPara[3] + ORNG.Rand() * Height;
			}
			break;
		}
	default:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unkonw initial source type %d.\n",p_nFixedSrcType);}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	ORNG.RN_POSITION_PRE = -1000;
	ORNG.RN_POSITION = 0;
# ifdef _IS_PARALLEL_
	MPI_Scatter(&OParallel.p_vRandNumPos[0], 1, MPI_INTEGER, &ORNG.RN_POSITION, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
# endif
	///////////////////////////// Direction & Energy ////////////////////////////////
	for(int i = 0;i < nSourceCount; ++i)
	{
		// Direction 
		//# ifdef _IS_PARALLEL_
		ORNG.GetRandSeed();
		//# endif
		dKsi1 = ORNG.Rand();
		dKsi2 = ORNG.Rand();  
		p_vFixedSrc[i].p_dDir[0] =  2*dKsi2-1;   
		p_vFixedSrc[i].p_dDir[1] =  sqrt(1-(p_vFixedSrc[i].p_dDir[0]*p_vFixedSrc[i].p_dDir[0]))*cos(2*3.14159*dKsi1);
		p_vFixedSrc[i].p_dDir[2] =  sqrt(1-(p_vFixedSrc[i].p_dDir[0]*p_vFixedSrc[i].p_dDir[0]))*sin(2*3.14159*dKsi1);

		//if(Parallel.MyId == 0)
		//{
		//	printf("Src %f, %f\n",FissionSrc[i].Pos[0],FissionSrc[i].Dir[0]);
		//}

		// Energy
		if(cAceData.p_bIsMultiGroup)
		{
			if(p_dFixedSrcErg<=0)
			{
				double T = 4.0/3;
				p_dFixedSrcErg = cAceData.SampleMaxwell(T);
			}
			//get the group number
			int nMin = 0, nMax = cAceData.p_nMltGrpNum - 1;
			int j;
			CDGlobeFun::GetIntpltPos(&cAceData.p_vMltErgBins[0], p_dFixedSrcErg, nMin, nMax, j);
			p_vFixedSrc[i].p_dErg = cAceData.p_nMltGrpNum-j;
		}
		else
		{
			if(p_dFixedSrcErg>0)
			{
				p_vFixedSrc[i].p_dErg = p_dFixedSrcErg;
			}
			else
			{
				double T = 4.0/3;
				p_vFixedSrc[i].p_dErg = cAceData.SampleMaxwell(T);
			}
		}

		/////////////////// check initial source //////////////////
		cParticleState.ClearLevel();
		int cell = cGeometry.LocateParticle(cParticleState, 0,p_vFixedSrc[i].p_dPos,p_vFixedSrc[i].p_dDir,true);
		if(cell == -1) // wrong initial source
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial source is on surface or undefined area.\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		} 
		if(cGeometry.p_vCell[cell].p_nImp == 0) // 0 importance cell
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"initial source in 0 importance cell %s!\n",
				Output.PrintCellVec(cParticleState.p_vLocCellsU));}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		} 

	}


	////////////////////////// weight /////////////////////////////
	cNeutronTransport.p_dStartWgt = 1.0;

	///////////////////////// finished ////////////////////////////
	Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Finished.\n");}, 
		Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

	ORNG.RN_POSITION_PRE = -1000;
	//"ORNG.RN_POSITION = 1" make the random number strides for source neutron generation and neutron transport staggered,
	//if not, there maybe asymmetric in cell tally.
	ORNG.RN_POSITION = 1;   
# ifdef _IS_PARALLEL_
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(&OParallel.p_vRandNumPos[0], 1, MPI_INTEGER, &ORNG.RN_POSITION, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	//"ORNG.RN_POSITION++" make the random number strides for source neutron generation and neutron transport staggered,
	//if not, there maybe asymmetric in cell tally.
	ORNG.RN_POSITION++;
# endif

	return;

}