# include "NeutronTransport.h"


void CDNeutronTransport::TreatFreeGasModel(CDParticleState &cParticleState,double dCellTmp, double dNucWgt, double dErg0, double &dErgRel)    //热化处理
{

  double dAtomTmp;
  double r1,z2,s,z,c,x2;
  double dYcn;
  int nMaxIter = 100000;
  int nIterCount = 0 ;

  /////// sample the velocity of the target nucleus.///////////
  dAtomTmp = dNucWgt/dCellTmp;  //?
  dYcn = sqrt(dErg0*dAtomTmp) ;   //Temperature-normalized neutron velocity.
  do{
	  nIterCount++ ;
	  if(nIterCount >= nMaxIter)
	  {
		  Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"Waring: too many samples of Free gas model.\n");},CDOutput::_WARNING);
	  }
    if( ORNG.Rand()*(dYcn+1.12837917) > dYcn)
	{
      r1 = ORNG.Rand();
      z2 = -log(r1*ORNG.Rand());
	}
    else
	{
	    do{
			double ksi1=ORNG.Rand();
			double ksi2=ORNG.Rand();
            r1 = ksi1*ksi1;
            s = r1 + ksi2*ksi2;
		}while(s > 1);	
        z2 = -r1*log(s)/s-log(ORNG.Rand());
    }
    z = sqrt(z2);
    c = 2*ORNG.Rand()-1.;
    x2 = dYcn*dYcn+z2-2*dYcn*z*c;
  }while( pow(ORNG.Rand()*(dYcn + z),2) > x2 );

  RotateDirection(c,cParticleState.p_dDir,cParticleState.p_dTgtVel);
  //cParticleState.TgtVel[0] = z*cParticleState.TgtVel[0];
  //cParticleState.TgtVel[1] = z*cParticleState.TgtVel[1];
  //cParticleState.TgtVel[2] = z*cParticleState.TgtVel[2]; 

  /////////// calculate functions of the target velocity.////////
  //cParticleState.Dir_Rel[0] = ycn*cParticleState.Dir[0]-cParticleState.TgtVel[0];
  //cParticleState.Dir_Rel[1] = ycn*cParticleState.Dir[1]-cParticleState.TgtVel[1];
  //cParticleState.Dir_Rel[2] = ycn*cParticleState.Dir[2]-cParticleState.TgtVel[2];
  for(int i = 0 ; i < 3 ; ++i )
  {
	  cParticleState.p_dTgtVel[i] = z*cParticleState.p_dTgtVel[i];
	  cParticleState.p_dDir_Rel[i] = dYcn*cParticleState.p_dDir[i] - cParticleState.p_dTgtVel[i];
  }	

  CDGlobeFun::Normalize3Array(cParticleState.p_dDir_Rel);

  dErgRel = x2/dAtomTmp;

  if( !(dErgRel > 0 && dErgRel < 100))
  {
	  Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit energy in free gas treatment is out of range.\n");},CDOutput::_WARNING);
	  dErgRel = p_dEg0CutOff;
  }
  return;
}