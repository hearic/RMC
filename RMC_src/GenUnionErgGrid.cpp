# include "AceData.h"
# include "Material.h"

void CDAceData::GenUnionErgGrid(CDMaterial &cMaterial)
{
	int n_erg1,n_erg2,n_erg3,i_erg,i1,i2,i3,n3;
	vector<double> vTemp_lar;
	int uni_nuc_num = cMaterial.GetTotNucNumExcSab();
	/////////////////////////////  uniform energy gird////////////////////////////////
	n_erg1 = GetErgGridNum(1);   //��������������  �ݲ�����sab�����
	i_erg = GetLocOfESZ(1);      //��ʼλ��
	p_vUnionEnergyGrid.resize(n_erg1 + 1);
	for(int i=1;i<=n_erg1;i++)
	{
		p_vUnionEnergyGrid[i]=p_vNuclides[1].XSS[i_erg+i-1];
	}
	//��ʱUnionEnergyGrid�����˵�һ�����ص���������Ϣ
	for(int i=2;i<=uni_nuc_num;i++)  //����������������
	{
		n_erg3 = GetErgGridNum(i);      //������º����������������
		n_erg2 = n_erg1;           //ԭ�������������
		n_erg1 = n_erg2 + n_erg3;   //������������������������غϣ���������temp_lar(n_erg1)
		vTemp_lar.resize(n_erg1+1);	
		i1 = 1;   //���������ָ��
		i2 = 1;   //ԭ�������ָ��
		i3 = GetLocOfESZ(i);     //������º����������ָ��
		n3 = 1;
		for(;;)
		{
			if (p_vUnionEnergyGrid[i2]==p_vNuclides[i].XSS[i3]) //�������������ͬ��ֻ����һ��������ָ��ǰ��һλ
			{
				vTemp_lar[i1] = p_vUnionEnergyGrid[i2];
				i1 = i1 + 1;
				i2 = i2 + 1;
				i3 = i3 + 1;
				n3 = n3 + 1;
			}
			else if(p_vUnionEnergyGrid[i2] < p_vNuclides[i].XSS[i3] )           //ʣ�����������������ֵС�ģ���ֻ�ƶ���ָ��
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
			//�������һ����������Ѿ�ȫ�������꣬������һ����ʣ�ಿ��ֱ�ӷ������������
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

	p_nUnionEgNum = n_erg1; //������ܺϲ���ɣ���ʱUnionEnergyGridΪ�ϲ���������ܣ�n_erg1Ϊ����������

	return;
}