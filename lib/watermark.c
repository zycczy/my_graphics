/*author: charles cheng 2016-10-23*/
#include "watermark.h"
#define COORDINATE(array, height, x, y)  *(array + x*height + y)

/*-------------------------------------------------------
|Matrix order| 8 | 16 | 32 | 64 | 100 | 128 | 256 | 512 |
---------------------------------------------------------
|map count   | 6 | 12 | 24 | 48 | 150  | 96 | 192 | 384 |
---------------------------------------------------------*/

int catmapping(HBMP_i_t* src, HBMP_i_t *dst, uint32_t map_times)
{

	int i, j, t;
	uint32_t *tmp_buffer = malloc(src->rgb_size);
	if(dst->rgb_buffer == NULL || src->rgb_buffer == NULL){
		__wrn("rgb buffer is NULL! dst->rgb_buffer = 0x%x, src->rgb_buffer = 0x%x\n", (int)dst->rgb_buffer, (int)src->rgb_buffer);
		return EPDK_FAIL;
	}

	memcpy(tmp_buffer, src->rgb_buffer, src->rgb_size);
	for(t=0;t<map_times;t++){
		for(i=0;i<src->height;i++){
			for(j=0;j<src->width;j++){
				COORDINATE(dst->rgb_buffer, src->height, i, j) = COORDINATE(tmp_buffer, src->height, ((i+j)%src->height), ((i+2*j)%src->height));
			}
		}
		memcpy(tmp_buffer, dst->rgb_buffer, src->rgb_size);
	}
	free(tmp_buffer);
	return EPDK_OK;
}
