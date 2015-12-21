# if ! defined __RNG__H__
# define  __RNG__H__

# include "CommonHead.h"

using namespace std;

class CDRNG
{
public:
	////////////////////// linear congruential RNGs ///////////////////
	//// r(k) = S(k)/p                
	//// S(k+1) = [ g * S(k) + c ] mod p                        
	//// r(k) = psuedo-random number,  0 <= r(k) <= 1  
	//// S(k) = seed                    
	//// g = multiplier                                
	//// c = increment, 0 or 1 (Multiplicative or Mixed).   
	//// p = modulus, p = 2^m            
	///////////////////////////////////////////////////////////////////

	///////////////////////////// stride RNGs /////////////////////////
	//// S(k) = g^k*S(0) + c*(g^k-1)/(g-1) mod p
	////      = G(k) + c(k) mod p
	//// G(k) = g^k mod p;
	//// c(k) = c*(g^k-1)/(g-1) mod p;
	///////////////////////////////////////////////////////////////////

	CDRNG()
	{
		RN_MULT = 3512401965023503517 ;
		RN_ADD = 0;
		RN_MOD  = ((unsigned long long)(1)<<63) ;
		RN_MASK = RN_MOD - 1;       //2**63 - 1
		RN_NORM = double(1.0)/double(RN_MOD); 
		RN_SEED0 = 1;
		RN_SEED = RN_SEED0;
		RN_STRIDE = 100000; //152917;
		RN_POSITION = 0;                     // New stride position
		RN_POSITION_PRE = -1000;
	}
	///////////// Public RNG function /////////////
	double Rand();
	void SkipRand(int nSkip);
	void SetRngParas(int nRandNumType);
	void GetRandSeed();
	void RestRandSeed();
	void GetSkipPara(unsigned long long ullSkip,unsigned long long &ullGK,unsigned long long &ullCK);

	
	unsigned long long RN_MULT;  
	unsigned long long RN_ADD;  
	unsigned long long RN_MOD;   
	unsigned long long RN_MASK;   
	double    RN_NORM;
	unsigned long long RN_SEED; 
	unsigned long long RN_SEED0; 
	unsigned long long RN_STRIDE;
	int RN_POSITION;
	int RN_POSITION_PRE;
};

extern CDRNG ORNG;

# endif
