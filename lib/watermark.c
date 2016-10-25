/*author: charles cheng 2016-10-23*/
#include "watermark.h"
struct Point
{
	int x;
	int y;
};

/*-------------------------------------------------------
|Matrix order| 8 | 16 | 32 | 64 | 100 | 128 | 256 | 512 |
---------------------------------------------------------
|map count   | 6 | 12 | 24 | 48 | 150 | 96  | 192 | 384 |
---------------------------------------------------------*/

#define M 32
int catmapping(HBMP_i_t* src, HBMP_i_t *dst, uint32_t map_times)
{

	int i, j;
	if(dst->rgb_buffer == NULL || src->rgb_buffer == NULL){
		__wrn("rgb buffer is NULL! dst->rgb_buffer = 0x%x, src->rgb_buffer = 0x%x\n", (int)dst->rgb_buffer, (int)src->rgb_buffer);
		return EPDK_FAIL;
	}
	struct Point MatrixPoint[M*M];
	struct Point IntLeavPoint[M*M];
	for(i=0;i<M;i++){
		for(j=0;j<M;j++){
			MatrixPoint[i * M + j].x = i;
			MatrixPoint[i * M + j].y = j;
		}
	}
	for(i=0;i<map_times;i++){
		for(j=0;j<M*M;j++){
			IntLeavPoint[j].x = (MatrixPoint[j].x + MatrixPoint[j].y) % M;
			IntLeavPoint[j].y = (MatrixPoint[j].x + 2 * MatrixPoint[j].y) % M;
			MatrixPoint[j] = IntLeavPoint[j];
		}
	}
	for(i=0;i<M;i++){
		for(j=0;j<M;j++){
			*(dst->rgb_buffer + i * M + j) = *(src->rgb_buffer + MatrixPoint[ i * M + j].x * M + MatrixPoint[ i * M + j].y);
		}
	}
	return EPDK_OK;
}



