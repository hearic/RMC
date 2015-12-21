# include "AceData.h"
# include "Material.h"

void CDAceData::GenUnionErgGrid(CDMaterial &cMaterial)
{
	int n_erg1,n_erg2,n_erg3,i_erg,i1,i2,i3,n3;
	vector<double> vTemp_lar;
	int uni_nuc_num = cMaterial.GetTotNucNumExcSab();
	/////////////////////////////  uniform energy gird////////////////////////////////
	n_erg1 = GetErgGridNum(1);   //核素能量点总数  暂不考虑sab的情况
	i_erg = GetLocOfESZ(1);      //起始位置
	p_vUnionEnergyGrid.resize(n_erg1 + 1);
	for(int i=1;i<=n_erg1;i++)
	{
		p_vUnionEnergyGrid[i]=p_vNuclides[1].XSS[i_erg+i-1];
	}
	//此时UnionEnergyGrid包含了第一个核素的能量点信息
	for(int i=2;i<=uni_nuc_num;i++)  //迭代所有其他核素
	{
		n_erg3 = GetErgGridNum(i);      //加入的新核素能量框架能量点
		n_erg2 = n_erg1;           //原能量框架能量点
		n_erg1 = n_erg2 + n_erg3;   //假设两个能量框架无能量点重合，开辟数组temp_lar(n_erg1)
		vTemp_lar.resize(n_erg1+1);	
		i1 = 1;   //新能量框架指针
		i2 = 1;   //原能量框架指针
		i3 = GetLocOfESZ(i);     //加入的新核素能量框架指针
		n3 = 1;
		for(;;)
		{
			if (p_vUnionEnergyGrid[i2]==p_vNuclides[i].XSS[i3]) //如果两能量点相同，只保留一个，所有指针前移一位
			{
				vTemp_lar[i1] = p_vUnionEnergyGrid[i2];
				i1 = i1 + 1;
				i2 = i2 + 1;
				i3 = i3 + 1;
				n3 = n3 + 1;
			}
			else if(p_vUnionEnergyGrid[i2] < p_vNuclides[i].XSS[i3] )           //剩余两种情况保留能量值小的，且只移动该指针
			{
				vTemp_lar[i1] = p_vUnionEnergyGrid[i2];
				i1 = i1 + 1;
				i2 = i2 + 1;
			}
			else
			{
				vTemp_lar[i1] = p_vNuclides[i].XSS[i3];
				i1 = i1 + 1;
				i3 = i3 + 1;
				n3 = n3 + 1;
			}		
			//如果其中一个能量框架已经全部处理完，将另外一个的剩余部分直接放入新能量框架
			if( n3 > n_erg3 )        
			{
				for(;;)
				{
					if( i2 > n_erg2 ){goto go80;}
					vTemp_lar[i1] = p_vUnionEnergyGrid[i2];
					i1 = i1 + 1;
					i2 = i2 + 1;
				}
			}
			else if( i2 > n_erg2 )
			{
				for(;;)
				{
					if( n3 >n_erg3 ) {goto go80;}
					vTemp_lar[i1] = p_vNuclides[i].XSS[i3];
					i1 = i1 + 1;
					i3 = i3 + 1;
					n3 = n3 + 1;
				}
			}
		}

go80:
		n_erg1 = i1 - 1;
		vTemp_lar.resize(n_erg1+1);
		p_vUnionEnergyGrid.resize(n_erg1+1);
		p_vUnionEnergyGrid= vTemp_lar;                
	}

	p_nUnionEgNum = n_erg1; //能量框架合并完成，此时UnionEnergyGrid为合并的能量框架，n_erg1为能量点总数

	return;
}