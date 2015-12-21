# include "Depth_Class.h"
const int FS[8] = {902320,922330,922350,922380,942390,942410,962450,982520};  // fissile nuclides with explicit yields

void Depth_Class::ReadOrigenYld()
{ 
	enum LibType{Origens = 1 , Origen2 = 2 };
	YieldData.resize(0);

	////////////////////////////  Read & Process yield for Origen-s ///////////////////////////////
	if(OrigenLibType == Origens) //// read yields of the first energy
	{
		ifstream OrigenYld;
		int StrLen = 200;
		char sTemp[200];

		OrigenYld.open(OrigenYldFile,ios::in);
		if( !OrigenYld ) 
		{
			ErrorWarning("Origen yield library not found.",1);
		}
		OrigenYld.getline(sTemp,StrLen); //// title

		int i = 0;
		while(!OrigenYld.eof())
		{
			double f1, f2, f3, f4, f5, f6;
			OrigenYld>>f1>>f2>>f3>>f4>>f5>>f6;

			int za = int(f1);
			if (za == 0) 
			{
				break;
			}
			int nr = int(f3);
			if (nr < 1 || nr > 3) 
			{
				sprintf(PrintMssg,"%d has %d sets of yields.",za,nr);
				ErrorWarning(PrintMssg,1);
			}

			YieldData.resize(i + 1);
			YieldData[i].FisNucID = za ;
			YieldData[i].Energy.resize(nr);
			YieldData[i].ProductYld.resize(nr);


			///////////// read yields of the thermal/fast/high energy ////////
			for(int k = 0 ; k < nr; ++k)
			{

				OrigenYld>>f1>>f2>>f3>>f4>>f5>>f6;

				YieldData[i].Energy[k] = f1;

				int nfp = int(f6);	
				YieldData[i].ProductId.resize(nfp);
				YieldData[i].ProductYld[k].resize(nfp);

				for(int j = 0 ; j < nfp ; ++j)
				{
					if((OrigenYld>>f1>>f2) == 0 )
					{
						sprintf(PrintMssg,"Wrong yield ID-Fraction pairs for %d.",za);
						ErrorWarning(PrintMssg,1);
					}

					int product_id = int(f1);
					double product_yld = f2;

					if(product_id < 10010 || product_id > 1100000 || product_yld < 0)
					{
						sprintf(PrintMssg,"Wrong fission product yield (%d,%6.3E) for %d.",product_id, product_yld, za);
						ErrorWarning(PrintMssg,1);
					}

					if(k == 0)
					{
						YieldData[i].ProductId[j] = product_id ;
					}
					else
					{
						if(product_id != YieldData[i].ProductId[j])
						{
							sprintf(PrintMssg,"Product ID %d does not match for %d.",product_id,za);
							ErrorWarning(PrintMssg,1);
						}
					}

					YieldData[i].ProductYld[k][j] = f2;

				}

				OrigenYld.getline(sTemp,StrLen);
				OrigenYld>>f1>>f2>>f3>>f4>>f5>>f6;
			}
			OrigenYld>>f1>>f2>>f3>>f4>>f5>>f6;
			++i;   //// yield data for next fissionable actinide
		}

		OrigenYld.close();

		return;
	}

	////////////////////////////  Process yield for Origen-2 ///////////////////////////////
	if(OrigenLibType == Origen2)
	{
		YieldData.resize(8);
		for(int i = 0 ; i < YieldData.size() ; ++i )
		{
			YieldData[i].FisNucID = FS[i];
			YieldData[i].Energy = vector<double>(1, 0.0253);
			YieldData[i].ProductYld.resize(1);
		}
		for(int i = 1 ; i <= NucNum ; ++i )
		{
			int NucID = GetNucId(i);
			if(LibData[i][25] > -1.0 )
			{
				for(int j = 25 ; j <= 32 ; ++j )
				{
					if(LibData[i][j] > 0)
					{
						YieldData[j-25].ProductId.push_back(NucID);
						YieldData[j-25].ProductYld[0].push_back(0.01*LibData[i][j]);   //// note 0.01
					}
				}

			}
		}

		return;
	}


	return;

}