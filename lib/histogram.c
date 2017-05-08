/*author: charles cheng 2017-05-05*/
#include "histogram.h"

static uint8_t histogram_table_init(HBMP_i_t *src, uint32_t *luma_gray_table)
{
	uint32_t i, j, t;
	uint8_t max = 0;
	memset(luma_gray_table, 0, sizeof(uint32_t)*GRAY_PROGRESSION);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){			
			luma_gray_table[src->yuv_buffer.y_buffer.buffer[i*src->width+j]]++;
			if(src->yuv_buffer.y_buffer.buffer[i*src->width+j] > max){
				max = src->yuv_buffer.y_buffer.buffer[i*src->width+j];
			}
		}
	}
	return max;
}
int histogram_equalization(HBMP_i_t *src)
{
	uint32_t i, j, t;
	double tmp = 0;
	uint8_t min = 255;
	uint8_t max = 0;
	uint32_t luma_gray_table[GRAY_PROGRESSION];
	max = histogram_table_init(src, luma_gray_table);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			for(t=0;t<src->yuv_buffer.y_buffer.buffer[i*src->width+j];t++){
				tmp += ((double)luma_gray_table[t]/(double)src->yuv_buffer.y_buffer.size);				
			}
			src->yuv_buffer.y_buffer.buffer[i*src->width+j] = max * tmp;
			tmp = 0;
		}
	}
	return 0;
}


int histogram_matching(HBMP_i_t *src, HBMP_i_t *dst)
{
	uint8_t src_max, dst_max;
	uint32_t i, j, t;
	double tmp = 0;
	uint32_t dst_luma_gray_table[GRAY_PROGRESSION], src_tansform_gray_table[GRAY_PROGRESSION];
	dst_max = histogram_table_init(dst, dst_luma_gray_table);
	for(i=0;i<GRAY_PROGRESSION;i++){
			for(t=0;t<i;t++){
				tmp += ((double)dst_luma_gray_table[t]/(double)src->yuv_buffer.y_buffer.size);				
			}
			src_tansform_gray_table[dst_max * tmp] = dst_max * tmp;
			tmp = 0;
		}
	}
	return 0;
}

