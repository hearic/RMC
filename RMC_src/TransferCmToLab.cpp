# include "Criticality.h"

void CDAceData::TransformCmToLab(int nNuc,int nMT,double dIncidErg,double dExitErg_Cm,double dMu_Cm,double &dExitErg_Lab,double &dMu_Lab)
{
	if(GetEmissNeuNum(nNuc, nMT) < 0)
	{
		double aw = p_vNuclides[nNuc].p_dNucAtomWgt;
		dExitErg_Lab = dExitErg_Cm + (dIncidErg + 2*dMu_Cm*(aw+1)*sqrt(dIncidErg*dExitErg_Cm))/(((1+aw)*(1+aw)));
		dMu_Lab = dMu_Cm*sqrt(dExitErg_Cm/dExitErg_Lab)+sqrt(dIncidErg/dExitErg_Lab)/(aw+1);
	}
	else
	{
		dExitErg_Lab = dExitErg_Cm;
		dMu_Lab = dMu_Cm;
	}

}