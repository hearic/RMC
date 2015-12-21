# include "Depth_Class.h"
# include "TTA_Class.h"
# include "ME_Class.h"

int old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);

Depth_Class Depth;
TTA_Class TTA;
ME_Class ME;

extern void CallDepth();
int DepthMain(int argc_temp,char *argv_temp[])// change to main when running Depth only
{

	Depth.RunDepth(argc_temp,argv_temp);
	
	return 0;
}

