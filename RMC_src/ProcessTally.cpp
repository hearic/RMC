# include "Criticality.h"
# include "Tally.h"

void CDTally::ProcessTally(CDCriticality &cCriticality, CDMaterial &cMaterial, CDAceData &cAceData)
{
	///////////////////////////////// Judge if it is Active/Last cycle //////////////////////////////////////
	bool bIsActiveCyc = (cCriticality.p_nCurrentCYCLE > cCriticality.p_nInactCycNum);
	if(!bIsActiveCyc)
	{
		return;
	}


	if(cCriticality.p_bUseBatch)
	{
		bool bIsBatchEnd = ((cCriticality.p_nCurrentCYCLE - cCriticality.p_nInactCycNum)%cCriticality.p_nBatchLen == 0);
		if(!bIsBatchEnd)
		{
			return;
		}
	}


	bool bIsLastCyc = (cCriticality.p_nCurrentCYCLE == cCriticality.p_nTotCycNum);


	//////////////////////////////// Gather tally data for parallel case /////////////////////////////////////
# ifdef _IS_PARALLEL_
	if(p_bIsPerCyc)   // Gather score at each active cycle for criticality case
	{
		OParallel.GatherCycTally(*this);
	}
	else if(p_bIsPerHstry && bIsLastCyc) // Gather sum1 and sum1 at the last cycle for fixed-source case
	{
		OParallel.GatherHstryTally(*this);
	}
# endif



	///////////////////////////////// Sum up tally data for kcode mode ///////////////////////////////////////
	if(p_bIsPerCyc)
	{
		if(Output.p_bIsTallyPrintPerCyc)
		{
			OutputTally(0, cCriticality, cMaterial);
		}

		SumUpTally(1, cCriticality);
	}

	//////////////////////////////////  NOTE: All for the LAST cycle in the below //////////////////////////////////

	if(!bIsLastCyc)
	{
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////// Calculate mean and s.t.d. at the last cycle ////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	double dM = 1.0;
	double dDiv = 1.0;
	if(p_bIsPerCyc)   // Gather score at each active cycle for criticality case
	{
		if(cCriticality.p_bUseBatch)
		{
			dM = cCriticality.p_nBatchNum;
			dDiv = cCriticality.p_dTotStartWgt * cCriticality.p_nBatchLen;
		}
		else
		{
			dM = cCriticality.p_nActCycNum;
			dDiv = cCriticality.p_dTotStartWgt;
		}
	}
	else if(p_bIsPerHstry)
	{
		dM = cCriticality.p_nActCycNum * cCriticality.p_dTotStartWgt;
		dDiv = 1;
	}

	/////////////////////////  Process cell tally  ///////////////////////////
# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsMaster)//// Only process cell tally in master processor
	{
		if(p_bIsWithCellTally)  
		{
			p_OCellTallyData.CalcAveRe(dM,dDiv);
		}
		if(p_bIsWithMeshTally)
		{
			p_OMeshTallyData.CalcAveRe(dM,dDiv);
		}
	}

# else
	if(p_bIsWithCellTally)
	{
		p_OCellTallyData.CalcAveRe(dM,dDiv);
	}
	if(p_bIsWithMeshTally)
	{
		p_OMeshTallyData.CalcAveRe(dM,dDiv);
	}
# endif

	///////////////////////  Process reaction rate tally  /////////////////////
	//////////////// NOTE: CsTally are reduced and calculated in all processors(for using in burnup)
	if(p_bIsWithCsTally)
	{
		if(p_bUseUnionCsTally)
		{
			ProcessCsByUEG(cMaterial, cCriticality, cAceData); // calculate reaction rates by Union-Energy Flux
		}
		else
		{
			p_OCsTallyData.CalcAveRe(dM,dDiv);

			////// Calculate micro cross-section /////
			int nCsTallyNum = p_vOneGroupCsTally.size();
			for(int i = 0;i < nCsTallyNum; ++i)
			{
				p_vOneGroupCsTally[i].DivideByFlux(p_OCsTallyData);
			}			
		}  

		//////////////////////////// Calculate Average Energy at the last cycle ////////////////////////////////
		int nCsTallyNum = p_vOneGroupCsTally.size();

		//Gather tally times and NeuAveEgy for parallel case, reduce to EVERY process
# ifdef _IS_PARALLEL_
		for(int i = 0;i < nCsTallyNum; ++i)
		{
			if (p_vOneGroupCsTally[i].p_bIsTallyNeuAveErg)
			{
				MPI_Allreduce(MPI_IN_PLACE,&p_vOneGroupCsTally[i].p_dTallyNeuNum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
				MPI_Allreduce(MPI_IN_PLACE,&p_vOneGroupCsTally[i].p_dNeuAveErg,   1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
			}			
		}
#endif
		//Calculate the average(for EVERY process)
		for(int i = 0;i < nCsTallyNum; ++i)
		{
			if (p_vOneGroupCsTally[i].p_bIsTallyNeuAveErg)
			{
				double dTallyNum = p_vOneGroupCsTally[i].p_dTallyNeuNum;
				double dSum = p_vOneGroupCsTally[i].p_dNeuAveErg;
				if (dTallyNum <= 0 || dSum <= 0) 
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"error average energy in tally of Cell %s.\n",
						Output.PrintCellVec(p_vOneGroupCsTally[i].p_vCellVecU));}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
				}
				else
				{
					p_vOneGroupCsTally[i].p_dNeuAveErg = dSum/dTallyNum;
				}
			}			
		}		
	}
	return;
}


void CDTally::ProcessTally(CDFixedSource &cFixedSource, CDMaterial &CMaterial, CDAceData &CAceData)
{
	//////////////////////////////// Gather tally data for parallel case /////////////////////////////////////
# ifdef _IS_PARALLEL_
	//if(p_bIsPerCyc)   // Gather score at each active cycle for criticality case
	//{
	//	OParallel.GatherCycTally(*this);
	//}
	//else if(p_bIsPerHstry && IsLastCyc) // Gather sum1 and sum1 at the last cycle for fixed-source case
	//{
		OParallel.GatherHstryTally(*this);
	//}
# endif




	//////////////////////////// Calculate mean and s.t.d. at the last cycle ///////////////////////////////////
	
	double M = 1.0;
	double Div = 1.0;
	M = cFixedSource.p_dTotStartWgt;
	Div = 1;
	

	/////////////////////////  Process cell tally  ///////////////////////////
# ifdef _IS_PARALLEL_
	if(OParallel.p_bIsMaster)//// Only process cell tally in master processor
	{
		if(p_bIsWithCellTally)  
		{
			p_OCellTallyData.CalcAveRe(M,Div);
		}
		if(p_bIsWithMeshTally)
		{
			p_OMeshTallyData.CalcAveRe(M,Div);
		}
	}

# else
	if(p_bIsWithCellTally)
	{
		p_OCellTallyData.CalcAveRe(M,Div);
	}
	if(p_bIsWithMeshTally)
	{
		p_OMeshTallyData.CalcAveRe(M,Div);
	}
# endif

	///////////////////////  Process reaction rate tally  /////////////////////
	if(p_bIsWithCsTally)
	{
		if(p_bUseUnionCsTally)
		{
			ProcessCsByUEG(CMaterial, cFixedSource, CAceData); // calculate reaction rates by Union-Energy Flux
		}
		else
		{
			p_OCsTallyData.CalcAveRe(M,Div);

			////// Calculate micro cross-section /////
			int CsTallyNum = p_vOneGroupCsTally.size();
			for(int i = 0;i < CsTallyNum; ++i)
			{
				p_vOneGroupCsTally[i].DivideByFlux(p_OCsTallyData);
			}			
		}  
	}
	return;
}