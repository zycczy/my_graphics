/*author: charles cheng 2017-05-05*/
#include "histogram_equalization.h"
uint32_t luma_gray_table[256];

int histogram_equalization(HBMP_i_t *src)
{
	uint32_t i, j, t;
	double tmp = 0;	
	double amount = 0;
	memset(luma_gray_table, sizeof(uint32_t)*256, 0);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){			
			luma_gray_table[src->yuv_buffer.y_buffer.buffer[i*src->width+j]]++;
		}
	}
	
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			for(t=0;t<src->yuv_buffer.y_buffer.buffer[i*src->width+j];t++){
				tmp += ((double)luma_gray_table[t]/(double)src->yuv_buffer.y_buffer.size);				
			}
			src->yuv_buffer.y_buffer.buffer[i*src->width+j] = 255 * tmp;
			tmp = 0;
		}
	}
	
	printf("amount: %f\n", amount);
	return 0;
}

