/*author: charles cheng 2017-05-12*/
#include "geometric_transformation.h"

int image_translation(HBMP_i_t *src, int32_t x, int32_t y)
{
	uint32_t* tmp = malloc(src->rgb_size);
	memset(tmp, BLACK_32BIT, src->rgb_size);
	if(abs(x)>src->width || abs(y)>src->height){
		__wrn("The value of x or y is too large!\n");
		return EPDK_FAIL;
	}
	for(i=y;i<src->height;i++){
		memcpy(tmp+i*src->width+x, src->rgb_buffer+(i-y)*src->width, src->width-x);
	}
	return 0
	
}
