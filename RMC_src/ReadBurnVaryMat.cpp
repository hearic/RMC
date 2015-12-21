# include "Input.h"

bool CDInput::ReadBurnVaryMat(vector<CDBurnVaryMat> &vBurnVaryMat, bool &bIsBlockEnd)
{
	// VaryMat   step = 1  mat = 1 3   newmat = 5 7
    //           step = 3  mat = 2     newmat = 6

	bIsBlockEnd = false;        // flag used to judge block end

	string InputCardStr ;

	for(;;)
	{
		if (bIsBlockEnd)
		{
			break;
		}

		// read step, mat, newmat
		vector<string> Options(3);
		vector<int> ParaNum(3);
		vector<vector<double> > Paras;
		Options[0] = "STEP" ;          ParaNum[0] =  1 ;
		Options[1] = "MAT";            ParaNum[1] = -1 ;
		Options[2] = "NEWMAT" ;        ParaNum[2] = -1 ;
		ReadFixSeqCardOptions(InputCardStr,Options,ParaNum,Paras,bIsBlockEnd);
		// no STEP
		if (Paras[0].size() == 0)
		{
			if (vBurnVaryMat.size() == 0)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"no STEP option in VARYMAT card in BURNUP block.\n");},Output.p_fpOutputFilePtr,2);
			}
			break;
		}

		///////////////////// check and assign parameters ///////////////////		
		for (int i = 0; i < Paras.size(); i++)
		{
			Output.CheckInputParas(Paras[i].size() > 0,
				[&](){sprintf(Output.p_sPrintStr,"no material varying option %s in VARYMAT card in BURNUP block.\n",Options[i]);},2);
			for (int j = 0;j < Paras[i].size(); j++)
			{
				if (Paras[i][j] <= 0 )
				{
					Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"incorrect value in option %s in VARYMAT card in BURNUP block.\n",Options[i]);},Output.p_fpOutputFilePtr,2);
				}
			}
		}

		// treat step
		int nStepNum = vBurnVaryMat.size();
		vBurnVaryMat.resize(nStepNum + 1);
		vBurnVaryMat[nStepNum].p_nBurnStep = ExtratIntPara(Paras[0][0],"Step");
		//check Step ID redefinition
		for (int i = 0;i < nStepNum;i++)
		{
			if (vBurnVaryMat[nStepNum].p_nBurnStep == vBurnVaryMat[i].p_nBurnStep)
			{
				Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"duplicated STEP %d in VARYMAT card in BURNUP block.\n", vBurnVaryMat[nStepNum].p_nBurnStep);}, 
					Output.p_fpOutputFilePtr, CDOutput::_ERROR);
			}
		}

		// treat Mat, NewMat
		Output.CheckInputParas(Paras[1].size() == Paras[2].size(),
			[&](){sprintf(Output.p_sPrintStr,"inconsistent MAT and NEWMAT in VARYMAT card in BURNUP block.\n");},CDOutput::_ERROR);
	
		vBurnVaryMat[nStepNum].p_vVaryMats.resize(Paras[1].size());
		for (int j = 0;j < Paras[1].size(); j++)
		{
			int nMat = ExtratIntPara(Paras[1][j],"MAT");
			int nNewMat = ExtratIntPara(Paras[2][j],"NEWMAT");
			vBurnVaryMat[nStepNum].p_vVaryMats[j].p_nMatIndex = nMat;
			vBurnVaryMat[nStepNum].p_vVaryMats[j].p_nNewMatIndex = nNewMat;
			Output.CheckInputParas(nMat != nNewMat,
				[&](){sprintf(Output.p_sPrintStr," MAT %d and NEWMAT %d are the same in VARYMAT card in BURNUP block.\n");},CDOutput::_ERROR);
		}
	}

	if (vBurnVaryMat.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}