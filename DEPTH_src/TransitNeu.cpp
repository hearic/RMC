# include "Depth_Class.h"
const double Cutoff25 = 1.0E-25;

void Depth_Class::TransitNeu()
{
	if(DecOnlyFlag==1){return;}  // Only Decay library are used to generate the transition matrix

	int ReactantID,ProductID,ReactantIndex,ProductIndex;
	double ReactRate, SNG, SN2N, SN3N_SNA, SNF_SNP, SNGX,SN2NX;

	NeuMatrix.Resize(0);

	//////////// Construct transition matrix from cross-section data ///////////
	for(int NucTemp = 1;NucTemp <= NucNum;++NucTemp)
	{
		ReactantIndex = NucTemp;

		if(ReactantIndex<1 || ReactantIndex>NucNum)
		{
			ErrorWarning("Wrong transition reactant.",1);
			continue;
		}

		SNG = LibData[ReactantIndex][19];
		SN2N = LibData[ReactantIndex][20];
		SN3N_SNA = LibData[ReactantIndex][21];
		SNF_SNP = LibData[ReactantIndex][22];
		SNGX = LibData[ReactantIndex][23];
		SN2NX = LibData[ReactantIndex][24];

		ReactantID=GetNucId(ReactantIndex);

		//1: (n,r) cross-section of nuclide leading to ground state of the daughter nuclide
		ReactRate = SNG;
		if(ReactRate > Cutoff25)
		{
			ProductID = ReactantID + 10;
			ProductID = ProductID/10*10;   // ground state
			ProductIndex=GetNucIdIndex(ProductID);
			NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
		}

		//2: (n,2n) cross-section of nuclide leading to ground state of the daughter nuclide
		ReactRate = SN2N;
		if(ReactRate > Cutoff25)
		{
			ProductID = ReactantID - 10;
			ProductID = ProductID/10*10;   // ground state
			ProductIndex=GetNucIdIndex(ProductID);
			NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
		}

		//3: (n,3n)/(n,a) cross-section of nuclide leading to ground state of the daughter nuclide
		ReactRate = SN3N_SNA;
		if(ReactRate > Cutoff25)
		{
			if(ReactantID >= FirstActinide)         // actinide  (n,3n)
			{
				ProductID = ReactantID-20;
				ProductID = ProductID/10*10;   // ground state
				ProductIndex = GetNucIdIndex(ProductID);
				NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
			}
			else  // activation product and fission product (n,a)
			{
				ProductID = ReactantID - 20030;
				ProductID = ProductID/10*10;   // ground state
				ProductIndex = GetNucIdIndex(ProductID);
				NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
				NeuMatrix.AddElem(GetNucIdIndex(20040),ReactantIndex,ReactRate);//  production of He
			}
		}

		//4: (n,f)/(n,p) cross-section of nuclide leading to ground state of the daughter nuclide
		ReactRate = SNF_SNP;
		if(ReactRate > Cutoff25)
		{
			if(ReactantID < FirstActinide)        // activation product and fission product (n,p)
			{
				ProductID=ReactantID - 10000;
				ProductID=ProductID/10*10;   // ground state
				ProductIndex=GetNucIdIndex(ProductID);
				NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
				NeuMatrix.AddElem(GetNucIdIndex(10010),ReactantIndex,ReactRate); // production of H
			}
		}
		//5: (n,rx) cross-section of nuclide leading to excited state of the daughter nuclide
		ReactRate = SNGX;
		if(ReactRate > Cutoff25)
		{
			ProductID = ReactantID+10;
			ProductID = ProductID/10*10+1;   // excited state
			ProductIndex = GetNucIdIndex(ProductID);
			NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
		}
		//6: (n,2nx) cross-section of nuclide leading to excited state of the daughter nuclide
		ReactRate = SN2NX;
		if(ReactRate > Cutoff25)
		{
			ProductID = ReactantID - 10;
			ProductID = ProductID/10*10 + 1;   // excited state
			ProductIndex = GetNucIdIndex(ProductID);
			NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
		}

		// Absorption/Reduction Rate of the nuclide
		ReactRate = SNG + SN2N + SN3N_SNA + SNF_SNP + SNGX + SN2NX;
		if(ReactRate > Cutoff25)
		{
			NeuMatrix.AddElem(ReactantIndex,ReactantIndex,-ReactRate);
		}
	}


	//////////// Construct transition matrix from yield data ///////////
	for(int i = 0; i < YieldData.size();++i)
	{
		ReactantID = YieldData[i].FisNucID;
		ReactantIndex = GetNucIdIndex(ReactantID);
		if (ReactantIndex == 0)// not exist in decay and xs lib
		{
			continue;
		}		
		double FisRate = GetLibFisXs(ReactantIndex);
		if(NearestNeighbor == ReactantID)
		{
			FisRate = FisRate*YieldFactor;
		}

		for(int j = 0 ; j < YieldData[i].ProductId.size() ; ++j)
		{
			int ProdId = YieldData[i].ProductId[j];
			ProductIndex = GetNucIdIndex(ProdId);
			ReactRate = FisRate * YieldData[i].PrdctYld[j] ;
			NeuMatrix.AddElem(ProductIndex,ReactantIndex,ReactRate);
		}
	}
	return;
}