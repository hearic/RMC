# include "Depth_Class.h"
# include "ME_Class.h"

void ME_Class::DecmpMatrixByLU(SparseMatrixMCS &MatrixA, SparseMatrixRCS &LowMatrix, SparseMatrixRCS &UpperMatrix)
{
	
	int row,col,size,size1,size2, iLoc;
	double ElimiFactor;
	int NucNum = Depth.NucNum;
	int FillCount = 0;
	static int InitialFlag=1;

 
    ////////////////////// LU Decomposition ////////////////////////
	for(int k=1;k<NucNum;++k)
	{
		size1=MatrixA._LowerPtr[k].size();    //  lower triangle
		size2=MatrixA._UpperPtr[k].size();  // upper triangle
		for(int i=0;i<size1;++i)     
		{
			row=MatrixA._LowerPtr[k][i];     /// Find the row need to be eliminated
			if(row<=k)
			{
				Depth.ErrorWarning("wrong lower-triangle pointers of Matrix A.",1);
			}
			ElimiFactor = MatrixA._LowerVal[k][i]/MatrixA._DiagVal[k];
			if(ElimiFactor==0)
			{
				Depth.ErrorWarning("wrong lower-triangle pointers of Matrix A.",1);
			}

			//// Generate LowMatrix L////
			LowMatrix._RowColPtr[row].push_back(k); // Caution: NOT = -ElimiFactor
		    LowMatrix._RowColVal[row].push_back(ElimiFactor); // Caution: NOT = -ElimiFactor

			/////// Elimination and filling-in of  Matrix A and its pointers////////
			for(int j=0;j<size2;++j)    /// Eliminate the columns of the given row
			{			
				col=MatrixA._UpperPtr[k][j];
				if(col<=k)
				{
					Depth.ErrorWarning("wrong upper-triangle pointers of Matrix A.",1);
				}
				if(col==row)     //fill in diagonal  elements of Matrix A
				{	
					MatrixA._DiagVal[row]=MatrixA._DiagVal[row]-ElimiFactor*MatrixA._UpperVal[k][j];
				}
				else if(col<row)   //fill in lower triangle of Matrix A
				{	
					size=MatrixA._LowerPtr[col].size();
					
					for(iLoc=0;iLoc<size;++iLoc)
					{
						if(row==MatrixA._LowerPtr[col][iLoc]){break;}
					}
					if(iLoc>=size) // (zero element)
					{
						FillCount++;
						MatrixA._LowerPtr[col].push_back(row);     //add a pointer to the end of current column 
						MatrixA._LowerVal[col].push_back(-ElimiFactor*MatrixA._UpperVal[k][j]);     //add a pointer to the end of current column 
					}
					else
					{
						MatrixA._LowerVal[col][iLoc]=MatrixA._LowerVal[col][iLoc]-ElimiFactor*MatrixA._UpperVal[k][j];
					}
				}
				else// if(col>row)   //fill in upper triangle of Matrix A
				{	
					size=MatrixA._UpperPtr[row].size();
					for(iLoc=0;iLoc<size;++iLoc)
					{
						if(col==MatrixA._UpperPtr[row][iLoc]){break;}
					}
					if(iLoc>=size) // (zero element)
					{
						FillCount++;
						MatrixA._UpperPtr[row].push_back(col);           
						MatrixA._UpperVal[row].push_back(-ElimiFactor*MatrixA._UpperVal[k][j]); //add a pointer to the end of current row 		
					}
					else
					{
						MatrixA._UpperVal[row][iLoc]=MatrixA._UpperVal[row][iLoc]-ElimiFactor*MatrixA._UpperVal[k][j];				
					}
				}
			} // end for(j=0;j<size2;++j)
		}// end for(i=0;i<size1;++i)
	}// endffor(k=1;k<=NucNum-1;++k)


	//////////////////////Generate UpperMatrix////////////////////////
	for(int i=1;i<=NucNum;++i)
	{
		UpperMatrix._RowColPtr[i].push_back(i); // push_back the diagonal pointers
		UpperMatrix._RowColVal[i].push_back(MatrixA._DiagVal[i]); // push_back the diagonal elements

		size2=MatrixA._UpperPtr[i].size();
		for(int j=0;j<size2;++j)  // 0 -> size
		{
			UpperMatrix._RowColPtr[i].push_back(MatrixA._UpperPtr[i][j]);
			UpperMatrix._RowColVal[i].push_back(MatrixA._UpperVal[i][j]);
		}
	}
	
	return;

}