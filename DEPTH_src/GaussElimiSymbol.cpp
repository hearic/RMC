# include "Depth_Class.h"
# include "ME_Class.h"

void ME_Class::GaussElimiSymbol(vector<int> &IRC, vector<int> &ICFR,  vector<int> &LUP)
{
	// reference:  杨绍祺，稀疏矩阵——算法及其程序实现，$6.3.1，P176
	// reference:  吴建平，稀疏线性方程组的高效求解与并行计算，$2.1，P26
	int NucNum = Depth.NucNum;
	int FillCount=0;
	static int InitialFlag=1;
	vector<vector<int> > ARowPtr(TranMatrixMCS._UpperPtr),AColPtr(TranMatrixMCS._LowerPtr);
	vector<vector <int> > LUPTrack(3,vector<int>(1));

    ////////////////////// 1.  Reset row&col pointers////////////////////////
	//for(i=1;i<=NucNum;++i)
	//{
	//	size1=TranMatrixMCS._UpperPtr[i].size();
	//	ARowPtr[i].resize(size1);
	//	size2=TranMatrixMCS._LowerPtr[i].size();
	//	AColPtr[i].resize(size2);
	//	EleCount=EleCount+size1+size2;
	//}
	//LUPTrack[0].resize(1);             //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
	//LUPTrack[1].resize(1);             //  column of lower triangle  or  row of upper triangle
	//LUPTrack[2].resize(1);             //  the i-th nonzero element of the column or row 


    //////////////////////2. Perform Symbol Gauss Elimination////////////////////////
	for(int k=1;k<NucNum;++k)
	{
		int size1=AColPtr[k].size();    //  lower triangle
		int size2=ARowPtr[k].size();  // upper triangle
		for(int i=0;i<size1;++i)     
		{
			int row=AColPtr[k][i];     /// Find the row need to be eliminated
			if(row<=k)
			{
				Depth.ErrorWarning("wrong lower-triangle pointers of Matrix A.",1);
			}
			/////// Elimination and filling-in of  Matrix A and its pointers////////
			for(int j=0;j<size2;++j)    /// Eliminate the columns of the given row
			{			
				int col=ARowPtr[k][j];
				if(col<=k)
				{
					Depth.ErrorWarning("wrong upper-triangle pointers of Matrix A.",1);
				}
				if(col==row)     //fill in diagonal  elements of Matrix A
				{	
					LUPTrack[0].push_back(0);                  //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
					LUPTrack[1].push_back(row);             //  column of lower triangle  or  row of upper triangle
					LUPTrack[2].push_back(0);             //  the i-th nonzero element of the column or row 
				}
				else if(col<row)   //fill in lower triangle of Matrix A
				{	
					int size=AColPtr[col].size();
					int iLoc = 0;
					for(iLoc=0;iLoc<size;++iLoc)
					{
						if(row==AColPtr[col][iLoc]){break;}
					}
					if(iLoc>=size) // (zero element)
					{
						FillCount++;
						AColPtr[col].push_back(row);              //add a pointer to the end of current column 

						LUPTrack[0].push_back(-1);                 //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
					    LUPTrack[1].push_back(col);               //  column of lower triangle  or  row of upper triangle
					    LUPTrack[2].push_back(size);             //  the i-th nonzero element of the column or row 
					}
					else
					{
						LUPTrack[0].push_back(-1);                 //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
					    LUPTrack[1].push_back(col);               //  column of lower triangle  or  row of upper triangle
					    LUPTrack[2].push_back(iLoc);             //  the i-th nonzero element of the column or row 
					}
				}
				else// if(col>row)   //fill in upper triangle of Matrix A
				{	
					int size=ARowPtr[row].size();
					int iLoc = 0;
					for(iLoc = 0; iLoc < size;++iLoc)
					{
						if(col==ARowPtr[row][iLoc]){break;}
					}
					if(iLoc>=size) // (zero element)
					{
						FillCount++;
						ARowPtr[row].push_back(col);           //add a pointer to the end of current row 		
						LUPTrack[0].push_back(1);                 //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
					    LUPTrack[1].push_back(row);               //  column of lower triangle  or  row of upper triangle
					    LUPTrack[2].push_back(size);             //  the i-th nonzero element of the column or row 
					}
					else
					{
						LUPTrack[0].push_back(1);                 //  Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
					    LUPTrack[1].push_back(row);               //  column of lower triangle  or  row of upper triangle
					    LUPTrack[2].push_back(iLoc);             //  the i-th nonzero element of the column or row 
					}
				}
			} // end for(j=0;j<size2;++j)
		}// end for(i=0;i<size1;++i)
	}// endffor(k=1;k<=NucNum-1;++k)


	//////////////////////3. Generate IRC, ICFR, LUP////////////////////////

	IRC.resize(NucNum+1);
	ICFR.resize(2*NucNum+2);

	if(InitialFlag==1)
	{
		for(int i=1;i<=NucNum;++i)
		{
			IRC[i]=i;
		}	
		ICFR[0]=0;
		ICFR[1]=NucNum+1;
	}

	//  Low triangle pointers - column compressed
	for(int i=1;i<=NucNum;++i)
	{
		int size = AColPtr[i].size();
		for(int j=0;j<size;++j)  // 0 -> size
		{
			IRC.push_back(AColPtr[i][j]);
		}
		ICFR[i+1]=ICFR[i]+size;
	}

	//  Upper triangle pointers - row compressed
	for(int i=1;i<=NucNum;++i)
	{
		int size2 = ARowPtr[i].size();
		for(int j=0;j<size2;++j)  // 0 -> size
		{
			IRC.push_back(ARowPtr[i][j]);
		}
		ICFR[NucNum+i+1]=ICFR[NucNum+i]+size2;
	}

	int size = LUPTrack[0].size();
	LUP.resize(size);
	for(int i=1;i<size;++i)
	{
		//  Check Flag:  0 diagonal   -1 lower triangle  +1 upper triangle
		if(LUPTrack[0][i]==0)            // diagonal
		{
			LUP[i]=LUPTrack[1][i];
		}
		else if(LUPTrack[0][i]==-1)    // lower triangle 
		{
			LUP[i]=ICFR[LUPTrack[1][i]]+LUPTrack[2][i];
		}
		else    // upper triangle 
		{
			LUP[i]=ICFR[NucNum+LUPTrack[1][i]]+LUPTrack[2][i];
		}
	}

	//cout<<"Initial Elements="<<EleCount<<"  Filled Elements="<<FillCount<<endl;

	return;
}