# include "TTA_Class.h"

//-----------------------------------
void TTA_Class::ResetTTA()
{
	ChainNuclide.clear();
	ChainLamda.clear();
	ChainEffBranch.clear();
	StackChains.clear();


	DaughterNucNum = 0;
	StackNum = 0;//��ջ����
	RepeatStackNum = 0;
	ChainNum = 0;//�����ж���������0��ʼ
	MotherNucIndex = 0;//ĳһ�����У�����λ�õı��
}