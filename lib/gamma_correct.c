/*author: charles cheng 2016-10-22*/
#include "gamma_correct.h"

uint8_t gamma_table[256];
static void init_gamma_table(float fPrecompensation)
{
	int i;
	float f;
	for(i=0;i<256;i++){
		f = (i + 0.5f)/256;
		f = (float)pow(f, fPrecompensation);
		gamma_table[i] = (uint8_t)(f*256-0.5f);
	}
	return ;
}

uint32_t gamma_correct(HBMP_i_t *hbmp_buf, float gamma)
{
	printf("gamma_correct start\n");
	int i, j;
	init_gamma_table(gamma);
	for(i=0;i<hbmp_buf->height;i++){
		for(j=0;j<hbmp_buf->width;j++){
			hbmp_buf->yuv_buffer.y_buffer.buffer[i*hbmp_buf->width+j] = gamma_table[hbmp_buf->yuv_buffer.y_buffer.buffer[i*hbmp_buf->width+j]];
		}
	}
	return 0;
}

