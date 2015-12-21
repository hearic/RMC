# include "CalMode.h"

void CDCalMode::RunCalculation(CDNeutronTransport &cNeutronTransport, CDAceData &cAceData, CDGeometry &cGeometry, Depth_Class &cDepth, CDCriticality &cCriticality, CDMaterial &cMaterial, 
CDParticleState &cParticleState, CDConvergence &cConvergence, CDTally &cTally, CDBurnup &cBurnup, CDFixedSource& cFixedSource)
{
	if (p_bSkipCalculation)
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Calculation skipped.\n");}, Output.p_fpOutputFilePtr, CDOutput::_WARNING);
		return;
	}
	////////////// Execute according to modes ////////////////
	switch(p_nRmcCalcMode)
	{
	case CriticalMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: criticality ********\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
			CalcCriticality(cNeutronTransport, cAceData, cCriticality, cMaterial, cGeometry, cParticleState, cConvergence, cTally, cBurnup);
			break;
		}
	case BurnupMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: burnup ********\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);

			CalcBurnup(cNeutronTransport, cAceData, cCriticality, cMaterial, cGeometry, cParticleState, cConvergence, cTally, cBurnup);
			break;
		}
	case PointBurnMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: point burnup ********\n");},
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
			CalcPointBurnup(cDepth);
			break;
		}
	case FixedSourceMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: fixed-source ********\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
			CalcFixedSource(cNeutronTransport, cAceData, cFixedSource, cMaterial, cGeometry, cParticleState, cTally);
			break;
		}
	case QuasiStaticSMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: criticality ********\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
			CalcCriticality(cNeutronTransport, cAceData, cCriticality, cMaterial, cGeometry, cParticleState, cConvergence, cTally, cBurnup);
			break;
		}
	case QuasiStaticDMode:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n******** Calculation mode: QuasiStatic ********\n");}, 
				Output.p_fpOutputFilePtr, CDOutput::_NORMAL);
			CalcCriticality(cNeutronTransport,cAceData, cCriticality, cMaterial, cGeometry, cParticleState, cConvergence, cTally, cBurnup);
			break;
		}
	default:
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"\n********Unknown calculation mode.********\n");},
				Output.p_fpOutputFilePtr, CDOutput::_ERROR);
		}
	}

	return;
}
