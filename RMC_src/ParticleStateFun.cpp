# include "ParticleState.h"
# include "GlobeFun.h"

/*
void ParticleStateClass::Normalize3Array(double VectorTemp[3])
{
	double length_temp;
	length_temp = 1.0/sqrt(VectorTemp[0]*VectorTemp[0]+VectorTemp[1]*VectorTemp[1]+VectorTemp[2]*VectorTemp[2]);
	VectorTemp[0] = VectorTemp[0]*length_temp;
	VectorTemp[1] = VectorTemp[1]*length_temp;
	VectorTemp[2] = VectorTemp[2]*length_temp;
}
*/

void CDParticleState::UpdateNeuState()
{
	p_dErg = p_dExitErg;

	for (int k=0;k<3;k++)
	{
		p_dDir[k] = p_dExitDir[k];
	}

	CDGlobeFun::Normalize3Array(p_dDir);
}

void CDParticleState::FlyByLength(double dLength)
{
	//length = length + 1.0E-8;

	p_dPos[0] = p_dPos[0] + p_dDir[0]*dLength; 
	p_dPos[1] = p_dPos[1] + p_dDir[1]*dLength;
	p_dPos[2] = p_dPos[2] + p_dDir[2]*dLength;

	p_dLocPos[0]=p_dLocPos[0] + p_dLocDir[0]*dLength; 
	p_dLocPos[1]=p_dLocPos[1] + p_dLocDir[1]*dLength;
	p_dLocPos[2]=p_dLocPos[2] + p_dLocDir[2]*dLength;
}

void  CDParticleState::ClearLevel()
{
	p_vLocUnivs.resize(0);
	p_vLocCells.resize(0);
	p_vLocCellsU.resize(0);
	return;
}