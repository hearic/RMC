# include "Depth_Class.h"
const int DataWith = 12, DataPrecision = 4;

void Depth_Class::ConvertLib()
{ 
	if(!ConvertLibFlag)
	{
		return;
	}

	
	//////////////////////// set size //////////////////////////////
	LibData.resize(NucNum+1);
	for(int i = 0 ; i <= NucNum ; ++i )
	{
		LibData[i].resize(DataNum+1);
		for(int j = 0 ; j <= DataNum ; ++j)
		{
			LibData[i][j] = 0 ;
		}
	}


	////////////// Read Origen Library ///////////////
	ReadOrigenDec();
	ReadOrigenXs();
	ReadOrigenYld();


	/////////////////////// Write DEPTH Library ///////////////////////////
    ofstream MainLib;
	MainLib.open(MainLibFile,ios::out);
	////////////// Write LibData (Decay + X.S.) /////////////////
	 MainLib<<"******************** Decay and cross-section data ********************\n";
	for(int i=1;i<=NucNum;++i)
	{
		int NucId = GetNucId(i);
		MainLib.setf(MainLib.left);
		MainLib.width(8);
		MainLib<<NucId;
		MainLib.width(8);
		MainLib<<GetNucName(NucId).c_str();

		MainLib.width(DataWith);
		MainLib.precision(DataPrecision);
		MainLib.setf(MainLib.uppercase|MainLib.scientific);
		for(int j = 1;j <= 24; ++j ) //// write decay and cross-section data
		{
			MainLib.width(DataWith);
			MainLib<<LibData[i][j];
			if((j%6)==0)
			{
				MainLib<<'\n';
				MainLib.width(16);
				MainLib<<' ';
			}
		}
		MainLib<<"-1"<<"\n";
	}
	MainLib<<"-1"<<"\n";

	////////////// Write YieldData (Fission product yield) /////////////////
	MainLib<<"******************** Fission product yield data ********************\n";
	MainLib.width(8);
	int fis_nuc_num = YieldData.size();
	MainLib<<fis_nuc_num<<"\n";
	for(int i = 0 ; i < fis_nuc_num ; ++i )
	{
		MainLib.width(8);
		MainLib<<YieldData[i].FisNucID;
		if((i+1)%8 == 0)
		{
			MainLib<<'\n';
		}
	}
	MainLib<<'\n';

	for(int i = 0 ; i < fis_nuc_num ; ++i )
	{
		int product_num = YieldData[i].ProductId.size();
		int erg_num = YieldData[i].Energy.size();
		MainLib.width(8);
		MainLib<<YieldData[i].FisNucID;
		MainLib.width(8);
		MainLib<<product_num;

		MainLib.width(8);
		MainLib<<erg_num;

		for(int k = 0 ; k < erg_num; ++k)
		{
			MainLib<<YieldData[i].Energy[k]<<"  ";
		}
		MainLib.width(8);
		MainLib<<'\n'<<' ';


		for(int j = 0 ; j < product_num ; ++j)
		{
			MainLib.width(8);
			MainLib<<YieldData[i].ProductId[j];

			for(int k = 0 ; k < erg_num; ++k)
			{
				MainLib<<YieldData[i].ProductYld[k][j]<<"  ";
			}
			if((j+1)%4 == 0)
			{
				MainLib<<'\n';
				MainLib.width(8);
				MainLib<<' ';
			}
		}
		MainLib<<"-1"<<"\n";
	}


	MainLib<<"-1";
	MainLib.close();


	YieldData.resize(0);

	return;
}