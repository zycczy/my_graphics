/*author: charles cheng 2017-05-12*/
#include "geometric_transformation.h"

static int image_translation(HBMP_i_t *src, int32_t x, int32_t y)
{
	YUV_BUFFER tmp;
	memcpy(&tmp, src, sizeof(YUV_BUFFER));
	if(abs(x)>src->width || abs(y)>src->height){
		__wrn("The value of x or y is too large!\n");
		return -1;
	}
	
	
}
