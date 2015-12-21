# include "RNG.h"
# include "Output.h"

double CDRNG::Rand()  ///// Get one Random number
{
	////////////////////// linear congruential RNGs ///////////////////
	//// r(k) = S(k)/p                
	//// S(k+1) = [ g * S(k) + c ] mod p                        
	//// r(k) = psuedo-random number,  0 <= r(k) <= 1  
	//// S(k) = seed                    
	//// g = multiplier                                
	//// c = increment, 0 or 1      
	//// p = modulus, p = 2^m            
	///////////////////////////////////////////////////////////////////

	double dRand = double(RN_SEED)*RN_NORM;
	RN_SEED = (((RN_MULT * RN_SEED) & RN_MASK) + RN_ADD) & RN_MASK;
	return dRand;
}

void CDRNG::SkipRand(int nSkip)
{
	for(int i = 0 ; i < nSkip ; ++i)
	{
		Rand();
	}
	return;
}


void CDRNG::GetRandSeed()  
{
	///////////////////////////// stride RNGs /////////////////////////
	//// S(k) = g^k*S(0) + c*(g^k-1)/(g-1) mod p
	////      = G(k) + c(k) mod p
	//// k = k-th random number
	//// G(k) = g^k mod p;
	//// c(k) = c*(g^k-1)/(g-1) mod p;
	///////////////////////////////////////////////////////////////////
	static unsigned long long ullRN_GK;
	static unsigned long long ullRN_CK;
	static unsigned long long ullRN_ISEED;	
	if(RN_POSITION_PRE != (RN_POSITION - 1)) //// only execute at the very beginning
	{
		GetSkipPara(RN_STRIDE,ullRN_GK,ullRN_CK);
		unsigned long long ullGK_temp, ullCK_temp;
		GetSkipPara(RN_STRIDE * RN_POSITION, ullGK_temp, ullCK_temp);
		ullRN_ISEED = (((ullGK_temp * RN_SEED0) & RN_MASK ) + ullCK_temp) & RN_MASK;
	}			
	else
	{
		ullRN_ISEED = (((ullRN_GK * ullRN_ISEED) & RN_MASK) + ullRN_CK) & RN_MASK ; ////
	}	
	RN_SEED = ullRN_ISEED ;
	RN_POSITION_PRE = RN_POSITION ;
	RN_POSITION = RN_POSITION + 1 ;
	return;
}

void CDRNG::GetSkipPara(unsigned long long ullSkip,unsigned long long &ullGK,unsigned long long &ullCK)
{
	long long lli = ullSkip & RN_MASK ;
	long long llg = RN_MULT;
	long long llc = RN_ADD;
	ullGK = 1;
	ullCK = 0;
	while(lli > 0)
	{
		if((lli & 1)==1) //// Odd
		{
			ullGK = (ullGK * llg) & RN_MASK;
			ullCK = (((ullCK * llg) & RN_MASK) + llc) & RN_MASK;
		}
		llc = (((llg + 1) & RN_MASK) * llc) & RN_MASK ;
		llg = (llg * llg) & RN_MASK;
		lli = lli/2;
	}	
}


void CDRNG::SetRngParas(int nRandNumType)
{
	if(nRandNumType == 1)       //// Multiplicative 48-bit LCG
	{
		RN_MULT = 19073486328125;                //5**19
		RN_ADD  = 0;
		RN_MOD  = ((unsigned long long)(1)<<48);                       // 2**48 = 281474976710656
		RN_MASK = RN_MOD - 1;                    //2**48 - 1
		RN_NORM = double(1.0)/double(RN_MOD);
	}
	else if (nRandNumType == 2) //// Multiplicative 63-bit LCG
	{
		RN_MULT = 3512401965023503517 ;
		RN_ADD  = 0;
		RN_MOD  = ((unsigned long long)(1)<<63) ;
		RN_MASK = RN_MOD - 1;       //2**63 - 1
		RN_NORM = double(1.0)/double(RN_MOD);
	}
	else if(nRandNumType == 3)  //// Mixed 63-bit  LCG, c = 1
	{
		RN_MULT = 9219741426499971445; 
		RN_ADD  = 1;
		RN_MOD  = ((unsigned long long)(1)<<63) ;
		RN_MASK = RN_MOD - 1;   
		RN_NORM = double(1.0)/double(RN_MOD);
	}
	else
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown RNG type %d.\n",nRandNumType);}, 
			Output.p_fpOutputFilePtr, CDOutput::_ERROR);
	}
}

