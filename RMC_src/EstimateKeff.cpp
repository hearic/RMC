# include "Criticality.h"
# include "Convergence.h"

void CDCriticality::EstimateKeffCol(double dWgt, double dMacroMuFisXs, double dMacroTotXs)
{
	p_dKeffWgtSum[0] = p_dKeffWgtSum[0] + dWgt * dMacroMuFisXs/dMacroTotXs;    // Collision: Kcol = ��_all_col ( Wgt * mu*��f / ��t) / W
}

void CDCriticality::EstimateKeffAbs(double dWgt,double dNu,double dMicroFisXs,double dMicroTotXs)
{
	p_dKeffWgtSum[1] = p_dKeffWgtSum[1] + dWgt * dNu * dMicroFisXs/dMicroTotXs;// Absorption: Kabs = ��_all_abs ( Wgt_i * mu*��f / ��a) / W
}

void CDCriticality::EstimateKeffTrk(double dWgt, double dMacroMuFisXs, double dTrackLen)
{
	p_dKeffWgtSum[2] = p_dKeffWgtSum[2] + dWgt * dTrackLen * dMacroMuFisXs;    // TrackLength: Ktl = ��_all_track ( Wgt_i * TrackLength * mu*��f) / W
}

