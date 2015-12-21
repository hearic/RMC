# include "AceData.h"
const double ZeroErg = 1.0E-25;

void CDAceData::GetCeExitErgMu(int nNuc,int nMT,double dIncidErg,double &dExitErg_Lab,double &dMu_Lab)
{	
	//// Nuc: 碰撞核
	//// MT:  反应类型
	//// IncidErg: 入射能量
	//// Mu_Cm: 质心系下的散射角余弦

	double dMu_Cm = 0;
	double dExitErg_Cm;         

	if (p_vNuclides[nNuc].LAND[nMT] >= 0)
	{
		dMu_Cm = GetScattCosine(nNuc,nMT,dIncidErg);   
	}

	if(nMT == 2)  // 弹性散射
	{	
		double aw = p_vNuclides[nNuc].p_dNucAtomWgt ; // atomic weight
		if(aw >= 1 )
		{
			dExitErg_Lab = dIncidErg*(1+(aw*aw)+2*aw*dMu_Cm)/((1+aw)*(1+aw));
			dMu_Lab = (1+aw*dMu_Cm)/sqrt(1+2*aw*dMu_Cm+(aw*aw));
		}
		else
		{
			dExitErg_Lab = 0.5*dIncidErg*(1+dMu_Cm);
			dMu_Lab = sqrt(0.5 + 0.5*dMu_Cm);	  
		}
	}
	else    //非弹性散射
	{
		int LawType, LDAT;

		GetLawType(nNuc,nMT,dIncidErg,LawType,LDAT);
		
		ReactByLaws(nNuc,nMT,LawType, LDAT,dIncidErg,dExitErg_Cm,dMu_Cm);

		TransformCmToLab(nNuc,nMT,dIncidErg,dExitErg_Cm,dMu_Cm,dExitErg_Lab,dMu_Lab);
	}
	
	/////////////////  check exit Erg and Mu in lab /////////////
	if( dExitErg_Lab == 0) // occasionally with MT = 2
	{
		dExitErg_Lab = ZeroErg ;
	}
	else if( !(dExitErg_Lab > 0 && dExitErg_Lab < 100))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit erg_lab out of range. nuc=%d, MT=%d, Erg=%9.6E\n",nNuc,nMT,dExitErg_Lab);},CDOutput::_WARNING);
		dExitErg_Lab = ZeroErg ;
	}

	if( !(dMu_Lab >= -1.000001 && dMu_Lab <= 1.000001))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit mu_lab out of range. nuc=%d, MT=%d, Mu=%f\n",nNuc,nMT,dMu_Lab);},CDOutput::_WARNING);

		dMu_Lab = 2*ORNG.Rand()-1.;
	}

	return;
}