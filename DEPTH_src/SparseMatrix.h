# if ! defined __SparseMatrix__H__
# define  __SparseMatrix__H__
# include <vector>
# include <math.h>
using namespace std;

class SparseMatrixIS                  // Indices storage
{
public:
	vector<int> _row;
    vector<int> _col;
    vector<double> _val;

public:
	void AddElem(int row, int col, double val)
	{
		if(row==0 || col==0){return;}
		_row.push_back(row);
		_col.push_back(col);
		_val.push_back(val);
	}

	void Resize(int size)
	{
		_row.resize(size);
		_col.resize(size);
		_val.resize(size);
	}

	int Size()const
	{
		return _row.size();
	}
};

class SparseMatrixMCS                  // Mixed compressed Storage
{
public:
		vector<double>	   _DiagVal;                      // diagonal value
		vector<vector<int> >           _UpperPtr;     // Upper triangle pointers
		vector<vector<double> >    _UpperVal;     // Upper triangle value
		vector<vector<int> >	       _LowerPtr;        // Lower triangle pointers
		vector<vector<double> >    _LowerVal;       // Lower triangle value
		int DimSize;
		int iLoc;
		int size;
		int ElemNum;

public:
	void Reset(int nucum)
	{
		DimSize = nucum;
		_DiagVal.resize(DimSize);                      //  compressed sparse row
		_UpperPtr.resize(DimSize);                       //  compressed sparse column 
		_UpperVal.resize(DimSize);                     
		_LowerPtr.resize(DimSize);     
		_LowerVal.resize(DimSize);  

		for(int i=1;i<=DimSize-1;++i)  /// Reset sparse Transition Matrix	
		{
			_DiagVal[i]=0;
			_UpperPtr[i].resize(0);
			_UpperVal[i].resize(0);
			_LowerPtr[i].resize(0);
			_LowerVal[i].resize(0);
		}
		ElemNum=0;

	}
	
	int UpperSize(int row)      // number of none zero elements of a given column in upper triangle matrix
	{
		return _UpperPtr[row].size();
	}
	int LowerSize(int col)     // number of none zero elements of a given row  in lower triangle matrix
	{
		return _LowerPtr[col].size();
	}

	void AddElem(int row, int col, double val)
	{
		if(row==col)  // diagonal elements
		{
			_DiagVal[row]=_DiagVal[row]+val;
		}
		else if(row>col)  // Lower triangle elements
		{
			size=_LowerPtr[col].size();
			for(iLoc=0;iLoc<size;++iLoc)
			{
				if(row==_LowerPtr[col][iLoc]){break;}
			}
			if(iLoc>=size)   // add an element to the sparse matrix if the current pointer does NOT exist
			{
				++ElemNum;
				_LowerPtr[col].push_back(row);
				_LowerVal[col].push_back(val);    // Lower triangle elements 
			}
			else           // modify the element if the current pointer  exists
			{
				_LowerVal[col][iLoc]=_LowerVal[col][iLoc]+val;   
			}
		}
		else if(row<col)
		{
			size=_UpperPtr[row].size();
			for(iLoc=0;iLoc<size;++iLoc)
			{
				if(col==_UpperPtr[row][iLoc]){break;}
			}
			if(iLoc>=size)   // add an element to the sparse matrix if the current pointer does NOT exist
			{
				++ElemNum;
				_UpperPtr[row].push_back(col);
				_UpperVal[row].push_back(val);    // Upper triangle elements 
			}
			else   // modify the element if the current pointer  exists
			{
				_UpperVal[row][iLoc]=_UpperVal[row][iLoc]+val;   
			}
		}
	}
		
};


class SparseMatrixCCS                  // column compressed storage excluding diagonal elements.   Used by TTA
{
public:
		vector<double>	   _DiagVal;                      // diagonal value
		vector<vector<int> >	       _ColRowPtr;        // Col - Row pointers
		vector<vector<double> >    _ColRowVal;            // Col - Row value
		int DimSize;
		int iLoc;
		int size;
		int ElemNum;

public:
	void Reset(int nucum)
	{
		ElemNum=0;
		DimSize=nucum;
		_DiagVal.resize(DimSize);                      
		_ColRowPtr.resize(DimSize);                   
		_ColRowVal.resize(DimSize);   

		for(int i=0;i<=DimSize-1;++i)  /// Reset sparse Transition Matrix	
		{
			_DiagVal[i]=0;
		    _ColRowPtr[i].resize(0);
		    _ColRowVal[i].resize(0);
		}
		ElemNum=0;
	}
	
	int RowNum(int col)      // number of none zero elements of a given column
	{
		return _ColRowPtr[col].size();
	}


	void AddElem(int row, int col, double val)
	{
		if(row==col)  // diagonal elements
		{
			_DiagVal[row]=_DiagVal[row]+val;
		}
		else
		{
			size=_ColRowPtr[col].size();
			for(iLoc=0;iLoc<size;++iLoc)
			{
				if(row==_ColRowPtr[col][iLoc]){break;}
			}
			if(iLoc>=size)   // add an element to the sparse matrix if the current pointer does NOT exist
			{
				++ElemNum;
				_ColRowPtr[col].push_back(row);
				_ColRowVal[col].push_back(val);    // Lower triangle elements 
			}
			else           // modify the element if the current pointer  exists
			{
				_ColRowVal[col][iLoc]=_ColRowVal[col][iLoc]+val;   
			}
		}
	}
		
};

class SparseMatrixRCS                  // Row compressed storage  Used by Laguerre polynomial method
{
public:
		vector<vector<int> >	   _RowColPtr;        // Col - Row pointers
		vector<vector<double> >    _RowColVal;            // Col - Row value
		int DimSize;
		int iLoc;
		int size;
		int ElemNum;

public:
	void Reset(int nucum)
	{
		ElemNum=0;
		DimSize=nucum;                    
		_RowColPtr.resize(DimSize);                   
		_RowColVal.resize(DimSize);   

		for(int i=0;i<=DimSize-1;++i)  /// Reset sparse Transition Matrix	
		{
			_RowColPtr[i].resize(0);
			_RowColVal[i].resize(0);
		}
		ElemNum=0;
	}

	
	int ColNum(int row)      // number of none zero elements of a given column
	{
		return _RowColPtr[row].size();
	}


	void AddElem(int row, int col, double val)
	{
		//if(fabs(val) < 1.0E-50 )
		//{
		//	return;
		//}
		size=_RowColPtr[row].size();
		for(iLoc=0;iLoc<size;++iLoc)
		{
			if(col==_RowColPtr[row][iLoc]){break;}
		}
		if(iLoc>=size)   // add an element to the sparse matrix if the current pointer does NOT exist
		{
			++ElemNum;
			_RowColPtr[row].push_back(col);
			_RowColVal[row].push_back(val);    // Lower triangle elements 
		}
		else           // modify the element if the current pointer  exists
		{
			_RowColVal[row][iLoc]=_RowColVal[row][iLoc]+val;   
		}
	}
		
};

#endif