# include "Universe.h"

void CDSurface::GenSurfType(int nSurf_index_u, string sSurf_key_word)
{

	p_sKeyWord = sSurf_key_word;

	if(p_sKeyWord.compare("P")==0){p_nType = 1;}
	else if(p_sKeyWord.compare("PX")==0){p_nType = 2;}
	else if(p_sKeyWord.compare("PY")==0){p_nType = 3;}
	else if(p_sKeyWord.compare("PZ")==0){p_nType = 4;}
	else if(p_sKeyWord.compare("SO")==0){p_nType = 5;}
	else if(p_sKeyWord.compare("S")==0){p_nType = 6;}
	else if(p_sKeyWord.compare("SX")==0){p_nType = 7;}
	else if(p_sKeyWord.compare("SY")==0){p_nType = 8;}
	else if(p_sKeyWord.compare("SZ")==0){p_nType = 9;}
	else if(p_sKeyWord.compare("C/X")==0){p_nType = 10;}
	else if(p_sKeyWord.compare("C/Y")==0){p_nType = 11;}
	else if(p_sKeyWord.compare("C/Z")==0){p_nType = 12;}
	else if(p_sKeyWord.compare("CX")==0){p_nType = 13;}
	else if(p_sKeyWord.compare("CY")==0){p_nType = 14;}
	else if(p_sKeyWord.compare("CZ")==0){p_nType = 15;}
	else if(p_sKeyWord.compare("K/X")==0){p_nType = 16;}
	else if(p_sKeyWord.compare("K/Y")==0){p_nType = 17;}
	else if(p_sKeyWord.compare("K/Z")==0){p_nType = 18;}
	else if(p_sKeyWord.compare("KX")==0){p_nType = 19;}
	else if(p_sKeyWord.compare("KY")==0){p_nType = 20;}
	else if(p_sKeyWord.compare("KZ")==0){p_nType = 21;}
	//else if(KeyWord.compare("SQ")==0){Type = 22;}
	//else if(KeyWord.compare("GQ")==0){Type = 23;}
	//else if(KeyWord.compare("TX")==0){Type = 24;}
	//else if(KeyWord.compare("TY")==0){Type = 25;}
	//else if(KeyWord.compare("TZ")==0){Type = 26;}

	char chSurf_str[100];
	sprintf(chSurf_str,"%d %s",nSurf_index_u,sSurf_key_word.c_str());
	p_sSurfStr = chSurf_str;
	return;
}