# if ! defined __Boolean__H__
# define  __Boolean__H__

# include "CommonHead.h"
# include "Output.h"


using namespace std;

class CDParenthPos {
public:
	int p_nStartPos;
	int p_nEndPos;

	CDParenthPos(int nStart_pos, int nEnd_pos)
	{
		p_nStartPos = nStart_pos;
		p_nEndPos = nEnd_pos;
	}
};



class CDBoolean {
public:

	enum p_eBoolOperation
	{
		_LEFT_PARENTHESIS = 1000001, _RIGHT_PARENTHESIS,
		_BOOLEAN_INTERSECTION, _BOOLEAN_UNION, _BOOLEAN_COMPLEMENT
	};

	static bool IsBooleanOperator(int nSurf_bool)
	{
		if(nSurf_bool == _LEFT_PARENTHESIS || nSurf_bool == _RIGHT_PARENTHESIS 
			|| nSurf_bool == _BOOLEAN_INTERSECTION || nSurf_bool == _BOOLEAN_UNION || nSurf_bool == _BOOLEAN_COMPLEMENT)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static void GenParenthPos(vector<int> vBoolean_expression, vector<CDParenthPos> &vParenth_pos)
	{
		vParenth_pos.clear();

		int nStart_pos = -1;
		int i = -1;
		for(;;)
		{
			nStart_pos = -1;
			i = -1;
			for(;;)
			{
				++i;
				if( i == int(vBoolean_expression.size()))
				{
					break;
				}

				if(vBoolean_expression[i] == _LEFT_PARENTHESIS)
				{
					nStart_pos = i;
				}
				else if(vBoolean_expression[i] == _RIGHT_PARENTHESIS)
				{

					vBoolean_expression[nStart_pos] = 0;
					vBoolean_expression[i] = 0;

					vParenth_pos.push_back(CDParenthPos(nStart_pos,i));

					break;
				}
			}
			if(nStart_pos == -1)
			{
				return;
			}
		}
	}

	static int CalcBoolExpression(vector<int> &vBoolExpression) // Calc bool expression with parentheses
	{

		int nStart_pos = -1;
		int i = -1;
		for(;;)
		{
			nStart_pos = -1;
			i = -1;
			for(;;)
			{
				++i;
				if( i== int(vBoolExpression.size()))
				{
					break;
				}

				if(vBoolExpression[i] == _LEFT_PARENTHESIS)
				{
					nStart_pos = i;
				}
				else if(vBoolExpression[i] == _RIGHT_PARENTHESIS)
				{

					int nResult = CalcNonParenBool(vBoolExpression,nStart_pos + 1,i - 1);

					vBoolExpression[nStart_pos] = -(i - nStart_pos);
					vBoolExpression[i] = nResult;

					break;
				}
			}


			if(nStart_pos == -1)
			{
				return CalcNonParenBool(vBoolExpression,0,vBoolExpression.size());
			}

		}
	}


    static int CalcBoolExpression(vector<int> &vBoolExpression,const vector<CDParenthPos> &vParenth_pos) // Calc bool expression with parentheses
	{
		////////////////// Remove parentheses /////////////
		for(int i =0 ; i < vParenth_pos.size(); ++i)
		{
			int nStart_pos = vParenth_pos[i].p_nStartPos;
			int nEnd_pos = vParenth_pos[i].p_nEndPos;

			int nResult = CalcNonParenBool(vBoolExpression,nStart_pos + 1,nEnd_pos - 1);

			vBoolExpression[nStart_pos] = -(nEnd_pos - nStart_pos);
			vBoolExpression[nEnd_pos] = nResult;
		}

		////////////////// calculate final results /////////////
		return CalcNonParenBool(vBoolExpression,0,vBoolExpression.size());
	}

	// calculate the bool expressions without parentheses
	static int CalcNonParenBool(vector<int> &vBoolExpression, int nStart_pos, int nEnd_pos)
	{

		if( nStart_pos > nEnd_pos)
		{
			Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown logical expression!\n");},CDOutput::_WARNING);
			return 0;
		}

		int nPos1 = nStart_pos;
		int nPos2 = 0;

		for(;;)
		{
			nPos2 = nPos1 + 2;
			if( nPos2 > nEnd_pos)
			{
				return vBoolExpression[nPos1];
			}

			int nBool1 = vBoolExpression[nPos1];
			int nBool2 = vBoolExpression[nPos2];

			if(nBool1 < 0)
			{
				nPos1 = nPos1 - nBool1;
				nBool1 = vBoolExpression[nPos1];

				nPos2 = nPos1 + 2;

				if( nPos2 > nEnd_pos)
				{
					return vBoolExpression[nPos1];
				}
				nBool2 = vBoolExpression[nPos2];
			}

			if(nBool2 < 0)
			{
				nPos2 = nPos2 - nBool2;
				//if( pos2 > end_pos)
				//{
				//	return BoolExpression[pos1];
				//}

				nBool2 = vBoolExpression[nPos2];
			}

			int LogicOper = vBoolExpression[nPos1 + 1];

			vBoolExpression[nPos2] = CalcSimpleBool(nBool1,nBool2,LogicOper);
			nPos1 = nPos2;

		}
	}

	static int CalcSimpleBool(int nBOOL1,int nBOOL2,int nLogicOper)  
	{ 

		if(nLogicOper == _BOOLEAN_INTERSECTION)
		{
			return (nBOOL1&&nBOOL2);
		}

		if(nLogicOper == _BOOLEAN_UNION)
		{
			return (nBOOL1 || nBOOL2);
		}

		
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"unknown logical operator.\n");},CDOutput::_WARNING);

		return 0 ;

	}

};

#endif////define  __Boolean__H__