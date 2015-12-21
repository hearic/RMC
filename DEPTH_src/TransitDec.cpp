# include "Depth_Class.h"

void Depth_Class::TransitDec()
{
	double Cutoff25 = 1.0E-40;
	int NucTemp,ReactId,ProdId,ReactIndex,ProdIndex;
	double ReactRate;

	DecMatrix.Resize(0);

	for(NucTemp = 1; NucTemp <= NucNum; ++NucTemp)
	{
		ReactIndex = NucTemp;

		double lamd = LibData[ReactIndex][1] ;
		if(lamd <= Cutoff25)
		{
			continue;
		}
	    double fb1 = LibData[ReactIndex][2] ;
		double fp = LibData[ReactIndex][3] ;
		double fp1 = LibData[ReactIndex][4] ;
		double fa = LibData[ReactIndex][5] ;
		double ft = LibData[ReactIndex][6] ;
		double fsf = LibData[ReactIndex][7] ;
		double fbn = LibData[ReactIndex][8] ;
		double fb = LibData[ReactIndex][9] ;
		double fbb = LibData[ReactIndex][10] ;
		double fneut = LibData[ReactIndex][11] ;
		double fba = LibData[ReactIndex][12] ;

		ReactId = GetNucId(ReactIndex);

		//1: Negatron beta decay to ground state of the daughter nuclide
		ReactRate = lamd*fb;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId + 10000;
			ProdId = ProdId/10*10;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		//2: Negatron beta decay to excited state of the daughter nuclide
		ReactRate = lamd*fb1;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId + 10000 + 1;   // excited state (what if ReactId is already in excited state?)
			ProdId = ProdId/10*10 + 1;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		// 3:  positron emission or electron capture  to ground state
		ReactRate = lamd*fp;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId - 10000;
			ProdId = ProdId/10*10;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		// 4:  positron emission or electron capture  to excited state
		ReactRate = lamd*fp1;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId - 10000 + 1;
			ProdId = ProdId/10*10 + 1;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		// 5: alpha decay 
		ReactRate = lamd*fa;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId - 20040;
			ProdId = ProdId/10*10;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
			DecMatrix.AddElem(GetNucIdIndex(20040),ReactIndex,ReactRate);//  production of He
		}

		// 6:  isomeric transition : excited state to ground state  of the same nuclide
		ReactRate = lamd*ft;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId - 1;
			if((ProdId%10)!=0) // ground state
			{
				sprintf(PrintMssg,"Wrong isomeric transition of %d.",ReactId);
				ErrorWarning(PrintMssg,1);
			}     
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		//7: spontaneous fission  --> 162500
		ReactRate = lamd*fsf;
		if(ReactRate > Cutoff25)
		{
			ProdId = 162500;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		//8: (beta + neutron) decay
		ReactRate = lamd*fbn;
		if(ReactRate > Cutoff25)
		{
			ProdId = ReactId + 10000 - 10;
			ProdId = ProdId/10*10;
			ProdIndex = GetNucIdIndex(ProdId);
			DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		}

		//9: double beta decay
		//ReactRate = lamd*fbb;
		//if(ReactRate > Cutoff25)
		//{
		//	ProdId = ReactId + 20000;
		//	ProdIndex = GetNucIdIndex(ProdId);
		//	DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		//}

		//10: neutron decay
		//ReactRate = lamd*fneut;
		//if(ReactRate > Cutoff25)
		//{
		//	ProdId = ReactId - 10;
		//	ProdIndex = GetNucIdIndex(ProdId);
		//	DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		//}

		//11: Beta decay plus an alpha particle
		//ReactRate = lamd*fba;
		//if(ReactRate > Cutoff25)
		//{
		//	ProdId = ReactId + 10000 - 20040;
		//	ProdIndex = GetNucIdIndex(ProdId);
		//	DecMatrix.AddElem(ProdIndex,ReactIndex,ReactRate);
		//}

		// Reduction Rate of the nuclide
		ReactRate = lamd;
		if(ReactRate > Cutoff25)
		{
			DecMatrix.AddElem(ReactIndex,ReactIndex,-ReactRate);
		}
	}

	return;
}