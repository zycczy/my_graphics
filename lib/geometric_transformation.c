/*author: charles cheng 2017-05-12*/
#include "geometric_transformation.h"

int image_transformation(HBMP_i_t *src, int32_t x, int32_t y)
{
	uint32_t* tmp = malloc(src->rgb_size);
	uint32_t i;
	memset(tmp, BLACK_32BIT, src->rgb_size);
	if(abs(x)>src->width || abs(y)>src->height){
		__wrn("The value of x or y is too large!\n");
		return EPDK_FAIL;
	}
	show_para((src->width-abs(x)));
	for(i=abs(y);i<src->height;i++){
		if(x>0 && y>0){
			memcpy((tmp+((i*src->width)+abs(x))), (src->rgb_buffer+((i-abs(y))*src->width)), (src->width-abs(x))*4);
		}else if(x<0 && y<0){	
			memcpy((tmp+((i-abs(y))*src->width)), (src->rgb_buffer+((i*src->width)+abs(x))), (src->width-abs(x))*4);
		}else if(x>0 && y<0){
			memcpy((tmp+(((i-abs(y))*src->width)+abs(x))), (src->rgb_buffer+(i*src->width)), (src->width-abs(x))*4);
		}else if(x<0 && y>0){
			memcpy((tmp+(i*src->width)), (src->rgb_buffer+(((i-abs(y))*src->width)+abs(x))), (src->width-abs(x))*4);
		}
	}
	memcpy(src->rgb_buffer, tmp, src->rgb_size);
	free(tmp);
	return 0;
}

int image_mirror(HBMP_i_t *src, TYPE_OF_MIRROR type)
{
	uint32_t* tmp = malloc(src->rgb_size);
	uint32_t i, j;	
	for(i=0;i<src->height;i++){
		if(type == HORIZONTAL){
			for(j=0;j<src->width;j++){
				memcpy(tmp+i*src->width+j, src->rgb_buffer+i*src->width+(src->width-1-j), 4);
			}
		}else if(type == VERTICAL){
			memcpy(tmp+i*src->width, (src->rgb_buffer+((src->height-1-i)*src->width)), src->width*4);
		}
	}
	memcpy(src->rgb_buffer, tmp, src->rgb_size);
	free(tmp);
}

static int nearest_neightbor_interpolation()
{

}

static int bilinear_interpolation()
{

}

static int bicubic_interpolation()
{

}
int image_transpose(HBMP_i_t *src, WAY_OF_TRANSPOSE transpose_way)
{
	
}

