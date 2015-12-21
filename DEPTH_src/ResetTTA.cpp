# include "TTA_Class.h"

//-----------------------------------
void TTA_Class::ResetTTA()
{
	ChainNuclide.clear();
	ChainLamda.clear();
	ChainEffBranch.clear();
	StackChains.clear();


	DaughterNucNum = 0;
	StackNum = 0;//堆栈数量
	RepeatStackNum = 0;
	ChainNum = 0;//储存有多少条，从0开始
	MotherNucIndex = 0;//某一条链中，核素位置的编号
}