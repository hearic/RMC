# include "Input.h"


void CDInput::ReadMeshTallyCard(bool &bIsBlockEnd,vector<CDMeshTally> &vMeshTally,bool &bIsWithMeshTally)
{

	//vector<MeshTallyClass> &MeshTally = Tally.MeshTally;
	int nTallySize = vMeshTally.size();
	vMeshTally.resize(nTallySize + 1) ;


	//////////////////////////////// Read cell tally ID ////////////////////////////////
	int nTally_id ;
	Output.CheckInputParas(std::fscanf(p_fpInputFilePtr,"%d",&nTally_id) > 0,
		[&](){sprintf(Output.p_sPrintStr,"unknown mesh tally ID.\n");},2);
	vMeshTally[nTallySize].p_nTallyID = nTally_id;


	////////////////////////////// Read cell tally options /////////////////////////////
	////////////////// avoid redefinition ////////////////////
	vector<string> vOptions(4);
	vector<int> vParaNum(4);
	vector<vector<double> > vParas;
	vOptions[0] = "TYPE" ;  vParaNum[0] = 1 ;
	vOptions[1] = "ENERGY"; vParaNum[1] = -1 ;
	vOptions[2] = "SCOPE"; vParaNum[2] = 3 ;
	vOptions[3] = "BOUND"; vParaNum[3] = 6 ;
	ReadCardOptions(string("MESHTALLY"),vOptions,vParaNum,vParas,bIsBlockEnd);


	///////////////// read tally type //////////////
	int nTally_type = ExtratIntPara(vParas[0][0],"TYPE");
	Output.CheckInputParas(nTally_type >= 1 && nTally_type <= 5,
		[&](){sprintf(Output.p_sPrintStr,"unknown tally type for mesh tally %d.\n",nTally_id);},2);

	vMeshTally[nTallySize].p_nTallyType = nTally_type;

	////////////////// Read energy bins /////////////////
	int nErgSize = vParas[1].size();
	if(nErgSize > 0) // check Energy
	{
		vMeshTally[nTallySize].p_vErgBins = vParas[1];
		for(int i = 0 ; i < nErgSize - 1; ++i)
		{
			Output.CheckInputParas(vMeshTally[nTallySize].p_vErgBins[i] < vMeshTally[nTallySize].p_vErgBins[i+1],
				[&](){sprintf(Output.p_sPrintStr,"incorrect ENERGY parameters for mesh tally %d.\n",nTally_id);},2);
		}
	}


	////////////////// Read mesh Scope/Bound  /////////////////
	Output.CheckInputParas(vParas[2].size() == 3 && vParas[3].size() == 6,
		[&](){sprintf(Output.p_sPrintStr,"incorrect SCOPE/BOUND parameters for mesh tally %d.\n",nTally_id);},2);

	for(int i = 0 ; i < 3 ; ++i)
	{
		vMeshTally[nTallySize].p_OTallyMesh.p_nMeshScope[i] = ExtratIntPara(vParas[2][i],"SCOPE");
		vMeshTally[nTallySize].p_OTallyMesh.p_nMeshNum[i] = std::max(vMeshTally[nTallySize].p_OTallyMesh.p_nMeshScope[i],1);

		vMeshTally[nTallySize].p_OTallyMesh.p_dBoundMin[i] = vParas[3][2*i];
		vMeshTally[nTallySize].p_OTallyMesh.p_dBoundMax[i] = vParas[3][2*i + 1];
	}


	vMeshTally[nTallySize].p_OTallyMesh.CheckMeshPara(string("MeshTally"));


	//Tally.IsWithMeshTally = true;
	bIsWithMeshTally= true;
	return;
}