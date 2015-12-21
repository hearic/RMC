# include "Input.h"

void CDInput::SI_ReadMeshPara(CDMesh &cMesh, string sKeyWord, bool &bIsBlockEnd)
{

	vector<string> vOptions(2);
	vector<int> vParaNum(2);
	vector<vector<double> > vParas;
	vOptions[0] = "SCOPE" ;  vParaNum[0] = 3 ;
	vOptions[1] = "BOUND";   vParaNum[1] = 6 ;
	SI_ReadCardOptions(sKeyWord,vOptions,vParaNum,vParas,bIsBlockEnd);

	///////////////////// check and assign parameters ///////////////////
	Output.CheckInputParas(vParas[0].size() == 3 && vParas[1].size() == 6,
		[&](){sprintf(Output.p_sPrintStr,"undefined SCOPE/BOUND parameters in %s card.\n",sKeyWord.c_str());},2);
	//Output.CheckInputParas(Paras[0][0] > 0 && Paras[0][1] > 0  && Paras[0][2] > 0,
	//	[&](){sprintf(Output.PrintStr,"incorrect SCOPE parameters in %s card.\n",KeyWord.c_str());},2);
	for(int i = 0 ; i < 3 ; ++i)
	{
		cMesh.p_nMeshScope[i] = ExtratIntPara(vParas[0][i],sKeyWord);
		cMesh.p_nMeshNum[i] = std::max(cMesh.p_nMeshScope[i],1);  /// note: scope = -1

		cMesh.p_dBoundMin[i] = vParas[1][2*i];
		cMesh.p_dBoundMax[i] = vParas[1][2*i + 1];
	}

	/////////////////////////// check mesh parameters ////////////////////
	cMesh.CheckMeshPara(sKeyWord);

	return;
}