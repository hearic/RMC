//# include "Geometry.h"
//
//
//int GeometryClass::CalcBoolExpression(vector<int> &BoolExpression) // Calc bool expression with parentheses
//{
//	int StartPos = -1;
//	int i = -1;
//	int Result;
//	for(;;)
//	{
//		++i;
//		if( i== int(BoolExpression.size()))
//		{
//			break;
//		}
//
//		if(BoolExpression[i] == _LEFT_PARENTHESIS)
//		{
//			StartPos = i;
//		}
//		else if(BoolExpression[i] == _RIGHT_PARENTHESIS)
//		{
//			BoolExpression.erase(BoolExpression.begin() + StartPos);
//			BoolExpression.erase(BoolExpression.begin() + i - 1);
//			Result = CalcNonParenBool(BoolExpression,StartPos,i - 2);
//
//			for(int j = StartPos + 1;j < i-1 ; ++j)
//			{
//				BoolExpression.erase(BoolExpression.begin() + StartPos + 1);
//			}
//			BoolExpression[StartPos] = Result;
//			return CalcBoolExpression(BoolExpression);
//		}
//	}
//
//	if(StartPos == -1)
//	{
//		return CalcNonParenBool(BoolExpression,0,BoolExpression.size());
//	}
//	else
//	{
//		Output.OutputMessage([&](){sprintf(Output.PrintStr,"unknown logical expression with parentheses.\n");},CD_Output::_WARNING);
//		return 0;
//	}
//}
//// calculate the bool expressions without parentheses
//int GeometryClass::CalcNonParenBool(vector<int> &BoolExpression, int StartPos, int EndPos)
//{
//
//	int BOOL1,BOOL2,LogicOper;
//
//	if( StartPos > EndPos)
//	{
//		Output.OutputMessage([&](){sprintf(Output.PrintStr,"unknown logical expression!\n");},CD_Output::_WARNING);
//		return 0;
//
//	}
//
//start:
//	if( StartPos + 2 > EndPos)
//	{
//		return BoolExpression[StartPos];
//	}
//
//	BOOL1 = BoolExpression[StartPos];
//	BOOL2 = BoolExpression[StartPos + 2];
//	LogicOper = BoolExpression[StartPos + 1];
//
//	StartPos = StartPos + 2 ;
//
//	BoolExpression[StartPos] = CalcSimpleBool(BOOL1,BOOL2,LogicOper);
//
//	goto start;
//}
//
//int GeometryClass::CalcSimpleBool(int BOOL1,int BOOL2,int LogicOper)  
//{ 
//
//	if(LogicOper == _BOOLEAN_INTERSECTION)
//	{
//		return (BOOL1&&BOOL2);
//	}
//
//	if(LogicOper == _BOOLEAN_UNION)
//	{
//		return (BOOL1 || BOOL2);
//	}
//
//	Output.OutputMessage([&](){sprintf(Output.PrintStr,"unknown logical operator.\n");},CD_Output::_WARNING);
//
//	return 0 ;
//
//}
//
