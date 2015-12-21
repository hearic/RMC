# include "AceData.h"

double CDAceData::GetScattCosine(int nNuc,int nMT,double dIncidErg)
{	
	//// Nuc: 碰撞核
	//// MT:  反应类型
	//// IncidErg: 入射能量

	double dMu_Cm = 0; // 质心系下的散射角余弦

	//// Isotropic case ////////////
	if (p_vNuclides[nNuc].LAND[nMT]==0)
	{
		dMu_Cm = 2*ORNG.Rand()-1;
		return dMu_Cm;
	}   



	double dKsi, dEii,dEi,f1,f2;
	int LOCC,nNE,nLC,min,max,mid;
	int nKsi_int;

	LOCC = GetLocOfAND(nNuc) + p_vNuclides[nNuc].LAND[nMT];
	nNE = int(p_vNuclides[nNuc].XSS[LOCC-1]); 
	min = LOCC;
	max = LOCC+nNE-1;
	if(dIncidErg<=p_vNuclides[nNuc].XSS[LOCC])
	{
		nLC = int(p_vNuclides[nNuc].XSS[LOCC+nNE]);
	}
	else if(dIncidErg>=p_vNuclides[nNuc].XSS[LOCC+nNE-1])
	{
		nLC = int(p_vNuclides[nNuc].XSS[LOCC+nNE+nNE-1]);
	}
	else
	{
		while(max - min > 1)
		{
			mid = (min + max)/2 ;
			if( dIncidErg>=p_vNuclides[nNuc].XSS[mid]) {min = mid;}
			else{max = mid;}
		}
		dEi = p_vNuclides[nNuc].XSS[min];
		dEii = p_vNuclides[nNuc].XSS[max];
		if(ORNG.Rand()<(dIncidErg-dEi)/(dEii-dEi))
		{
			min = max;
		}
		nLC = int(p_vNuclides[nNuc].XSS[min+nNE]);
	}

	if(nLC == 0)// Isotropic case
	{
		dMu_Cm = 2*ORNG.Rand()-1;
		return dMu_Cm;
	}   

	int LOCC2 ;
	LOCC2 = GetLocOfAND(nNuc) + abs(nLC)-1;
	if(nLC > 0)   //32等概率余弦表
	{		
		dKsi=32*ORNG.Rand();
		nKsi_int = int(dKsi);
		f1 = p_vNuclides[nNuc].XSS[LOCC2+nKsi_int] ;
		f2 = p_vNuclides[nNuc].XSS[LOCC2+nKsi_int+1] ;
		dMu_Cm = p_vNuclides[nNuc].XSS[LOCC2+nKsi_int]+(dKsi-nKsi_int)*(p_vNuclides[nNuc].XSS[LOCC2+nKsi_int+1]-p_vNuclides[nNuc].XSS[LOCC2+nKsi_int]);
		goto end;
	}

	int AND_JJ,AND_NP;
	double fa,ca,bb;
	if(nLC < 0)  //角分布表					
	{
		nLC = -nLC;
		AND_JJ=int(p_vNuclides[nNuc].XSS[LOCC2]);  //1=histogram  2=lin-lin
		AND_NP=int(p_vNuclides[nNuc].XSS[LOCC2+1]);

		min = LOCC2+2*AND_NP+2;
		max = LOCC2+3*AND_NP+1;
		dKsi=ORNG.Rand();
		while(max-min>1)
		{
			mid = (min+max)/2 ;
			if( dKsi>=p_vNuclides[nNuc].XSS[mid])
			{
				min = mid;
			}
			else
			{
				max = mid;
			}
		}

		fa = p_vNuclides[nNuc].XSS[min-AND_NP];
		ca = p_vNuclides[nNuc].XSS[min-2*AND_NP];
		dMu_Cm = ca + (dKsi-p_vNuclides[nNuc].XSS[min])/fa;
		if( AND_JJ == 1 ) 
		{
			goto end;;
		}
		bb = (p_vNuclides[nNuc].XSS[min-AND_NP+1]-fa)/(p_vNuclides[nNuc].XSS[min-2*AND_NP+1]-ca);
		if( bb != 0)
		{
			dMu_Cm = ca + (sqrt(std::max(0.0,fa*fa+2*bb*(dKsi-p_vNuclides[nNuc].XSS[min])))-fa)/bb;
			goto end;
		}
	}

	end:
	if(!(dMu_Cm >=-1.000001 && dMu_Cm <=1.000001))
	{
		Output.OutputMessage([&](){sprintf(Output.p_sPrintStr,"exit mu out of range. nuc=%d, MT=%d, Mu_cm=%20.16f\n",nNuc,nMT,dMu_Cm);},CDOutput::_WARNING);

		dMu_Cm = 2*ORNG.Rand()-1;
	}

	return dMu_Cm;

}
