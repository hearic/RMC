//# include "Criticality.h"
//# include "FixedSource.h"
# include "NeutronTransport.h"

void CDNeutronTransport::RotationFreeGas(CDParticleState &cParticleState,double dDirRel[3],double dNucAtomWgt,double dTmp,
	double dMuLab,double* dExitDir, int nColliNuc, double dExitErgLab, double& dExitErg)
{
	//printf("Ptr\n");
	RotateDirection(dMuLab,dDirRel, dExitDir);   //转换后的出射方向

	double dAtomTmp = dNucAtomWgt/dTmp; //cParticleState.CellTmp; 
	double dtemp1 = sqrt(dExitErgLab * dAtomTmp);
	for(int i = 0 ; i < 3 ; ++i )
	{
		dExitDir[i] = dtemp1*dExitDir[i] + cParticleState.p_dTgtVel[i];
	}	
	double dtemp2 = (dExitDir[0]*dExitDir[0]) + (dExitDir[1]*dExitDir[1]) + (dExitDir[2]*dExitDir[2]);
	for(int i = 0 ; i < 3 ; ++i )
	{
		dExitDir[i] = dExitDir[i]/sqrt(dtemp2);
	}
	dExitErg = dtemp2/dAtomTmp;                     //转换后的出射能量

	return;
}

void CDNeutronTransport::GetExitState(CDCriticality &cCriticality, CDAceData &cAceData,CDParticleState &cParticleState)
{
	//////////////////////// multi-group case /////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		double dMuLab;
		cAceData.GetMgExitErgMu(cParticleState.p_nNUC,cParticleState.p_dErg,cParticleState.p_dExitErg,dMuLab);
		RotateDirection(dMuLab,cParticleState.p_dDir,cParticleState.p_dExitDir);

		double dTemp = cAceData.GetMgNucP0Cs(cParticleState.p_nNUC,cParticleState.ErgGrp())/(cParticleState.p_OColNucCs.p_dTot - cParticleState.p_OColNucCs.p_dAbs - cParticleState.p_OColNucCs.p_dFis);
		cParticleState.p_dWgt = cParticleState.p_dWgt * dTemp;
		return;
	}


	////////////////////// continuous-energy case /////////////////////////		
	//////////////// sab collision case ////////////////
	if(cParticleState.p_bIsSabCol)  
	{
		cAceData.TreatSabColliType(cParticleState.p_nSabNUC,cParticleState.p_OColNucCs.p_dEl,cParticleState.p_OColNucCs.p_dInel,cParticleState.p_dErg,cParticleState.p_dDir,cParticleState.p_dExitErg,cParticleState.p_dExitDir,*this);
		if (cParticleState.p_dErg <= p_dEg0CutOff) //energy cutoff
		{
			cParticleState.p_bIsKilled = true;
		}  
		return;
	}  
	///////////// ordinary collision case //////////////
	//// Get Exit state: weight & energy  &  IncidDirect
	int nEmissNeuNum = abs(cAceData.GetEmissNeuNum(cParticleState.p_nNUC, cParticleState.p_nColType));

	if(nEmissNeuNum == 0)
	{
		cParticleState.p_bIsKilled = true;
		return;
	}
	else if(nEmissNeuNum > 100)
	{
		nEmissNeuNum = 1;
	}

	cCriticality.p_cAbsNeuStrength.p_dScore += cParticleState.p_dWgt*(1-nEmissNeuNum);

	cParticleState.p_dWgt = cParticleState.p_dWgt * nEmissNeuNum;  //  multiply WGT by the emission neutron number

	GetCeExitState(cAceData,cParticleState,cParticleState.p_nNUC,cParticleState.p_nColType,cParticleState.p_dErg_Rel,cParticleState.p_dDir,
		cParticleState.p_bIsFreeGasCol,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
	if (cParticleState.p_dErg <= p_dEg0CutOff) //energy cutoff
	{
		cParticleState.p_bIsKilled = true;
	}  
	return;
}

void CDNeutronTransport::GetCeExitState(CDAceData &cAceData,CDParticleState &cParticleState,int nColliNuc,int nReactType,double dIncidErg,const double *dIncidDir,bool bTreatedFreeGas,double &dExitErg,double *dExitDir)
{
	////////////////////// Get dExitErg and Mu in lab /////////////////////////
	double dMuLab;  // cosine of scattering angle in lab coordinate systm
	double dExitErgLab;
	cAceData.GetCeExitErgMu(nColliNuc,nReactType,dIncidErg,dExitErgLab,dMuLab);   

	////////////////////////////   Rotation ////////////////////////////////
	if(bTreatedFreeGas)   //若使用了自由气体模型,则需要同时转换出射能量与角度
	{
		RotationFreeGas(cParticleState,cParticleState.p_dDir_Rel,cAceData.p_vNuclides[nColliNuc].p_dNucAtomWgt,cParticleState.p_dCellTmp,dMuLab,dExitDir, nColliNuc, dExitErgLab, dExitErg);
	}
	else  //未使用自由气体模型,则只需要转换出射角度
	{
		RotateDirection(dMuLab,dIncidDir,dExitDir);
		dExitErg = dExitErgLab;
	}

	//////////////////////////////  check exit state ///////////////////////////
	if( !(dExitErg>=0 && dExitErg < 100))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit energy out of range.\n");},CDOutput::_WARNING);
		
		dExitErg = p_dEg0CutOff;
	}

	return;
}


void CDNeutronTransport::GetExitState(CDFixedSource &cFixedSource, CDAceData &cAceData,CDParticleState &cParticleState)
{
	//////////////////////// multi-group case /////////////////////////////
	if(cAceData.p_bIsMultiGroup)
	{
		if(cParticleState.p_nColType==19)
		{
			////////////////////////////////// treat fission /////////////////////
			cFixedSource.TreatFission(*this, cAceData,cParticleState);
			cParticleState.p_bIsKilled = true;
			return;
		}
		else  //mt=2
		{
			double MuLab;
			cAceData.GetMgExitErgMu(cParticleState.p_nNUC,cParticleState.p_dErg,cParticleState.p_dExitErg,MuLab);
			RotateDirection(MuLab,cParticleState.p_dDir,cParticleState.p_dExitDir);

			double temp = cAceData.GetMgNucP0Cs(cParticleState.p_nNUC,cParticleState.ErgGrp())/(cParticleState.p_OColNucCs.p_dTot - cParticleState.p_OColNucCs.p_dAbs - cParticleState.p_OColNucCs.p_dFis);
			cParticleState.p_dWgt = cParticleState.p_dWgt * temp;
			return;
		}
		
	}


	////////////////////// continuous-energy case /////////////////////////		
	//////////////// sab collision case ////////////////
	if(cParticleState.p_bIsSabCol)  
	{
		cAceData.TreatSabColliType(cParticleState.p_nSabNUC,cParticleState.p_OColNucCs.p_dEl,cParticleState.p_OColNucCs.p_dInel,cParticleState.p_dErg,cParticleState.p_dDir,cParticleState.p_dExitErg,cParticleState.p_dExitDir,*this);
		if (cParticleState.p_dErg <= p_dEg0CutOff) //energy cutoff
		{
			cParticleState.p_bIsKilled = true;
		}  
		return;
	}  
	///////////// ordinary collision case //////////////
	//// Get Exit state: weight & energy  &  IncidDirect
	int EmissNeuNum = abs(cAceData.GetEmissNeuNum(cParticleState.p_nNUC, cParticleState.p_nColType));

	if(EmissNeuNum == 0)
	{
		cParticleState.p_bIsKilled = true;
		return;
	}
	else if(EmissNeuNum > 100)
	{
		EmissNeuNum = 1;
	}

	/////////////// fission reaction ////////////////
	if(EmissNeuNum==19)
	{
		////////////////////////////////// treat fission /////////////////////
		cFixedSource.TreatFission(*this, cAceData,cParticleState);
		cParticleState.p_bIsKilled = true;
		return;
	}

	///////////// ordinary scattering ////////////////////////
	cParticleState.p_dWgt = cParticleState.p_dWgt * EmissNeuNum;  //  multiply WGT by the emission neutron number

	GetCeExitState(cAceData,cParticleState,cParticleState.p_nNUC,cParticleState.p_nColType,cParticleState.p_dErg_Rel,cParticleState.p_dDir,
		cParticleState.p_bIsFreeGasCol,cParticleState.p_dExitErg,cParticleState.p_dExitDir);
	if (cParticleState.p_dErg <= p_dEg0CutOff) //energy cutoff
	{
		cParticleState.p_bIsKilled = true;
	}  
	return;
}
