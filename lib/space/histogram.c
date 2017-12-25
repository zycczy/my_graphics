/*author: charles cheng 2017-05-05*/
#include "histogram.h"

static uint8_t histogram_table_init(HISTOGRAM_DATA *histogram_data)
{
	uint32_t i, j;
	uint8_t max = 0;
	
	HBMP_i_t *src = histogram_data->src;
	uint32_t *CDF_table = histogram_data->CDF_table;
	__dbg("histogram_table_init start\n");
	memset(CDF_table, 0, sizeof(uint32_t)*GRAY_PROGRESSION);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){			
			CDF_table[src->yuv_buffer.y_buffer.buffer[i*src->width+j]]++;
			if(src->yuv_buffer.y_buffer.buffer[i*src->width+j] > max){
				max = src->yuv_buffer.y_buffer.buffer[i*src->width+j];
			}
		}
	}
	return max;
}

static int histogram_equalization(HISTOGRAM_DATA *histogram_data)
{
	uint32_t i, j, t;
	double tmp = 0;
	uint8_t max = histogram_data->max;
	uint32_t *CDF_table = histogram_data->CDF_table;
	HBMP_i_t *src = histogram_data->src;
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			for(t=0;t<src->yuv_buffer.y_buffer.buffer[i*src->width+j];t++){
				tmp += ((double)CDF_table[t]/(double)src->yuv_buffer.y_buffer.size);				
			}
			src->yuv_buffer.y_buffer.buffer[i*src->width+j] = histogram_data->gray_table[(int)(max * tmp)];
			tmp = 0;
		}
	}
	return 0;
}

static inline void normal_gray_table(HISTOGRAM_DATA *histogram_data)
{
	uint32_t i;
	for(i=0;i<GRAY_PROGRESSION;i++){
		histogram_data->gray_table[i] = i;
	}
}

static int histogram_matching(HISTOGRAM_DATA *src_histogram_data, HISTOGRAM_DATA *dst_histogram_data)
{
	uint32_t i, t;
	double tmp = 0;
	uint32_t *dst_CDF_table = dst_histogram_data->CDF_table;
	HBMP_i_t *dst = dst_histogram_data->src;
	
	__dbg("histogram_matching start\n");
	memset(src_histogram_data->gray_table, -1, GRAY_PROGRESSION*sizeof(int16_t));
	for(i=0;i<GRAY_PROGRESSION;i++){
		for(t=0;t<i;t++){
			tmp += ((double)dst_CDF_table[t]/(double)dst->yuv_buffer.y_buffer.size);				
		}
		src_histogram_data->gray_table[(uint32_t)(0.5 + 255 * tmp)] = i;
		tmp = 0;
	}
	__dbg("src_histogram_data GRAY_PROGRESSION transform\n");
	for(i=1;i<GRAY_PROGRESSION;i++){
		if(src_histogram_data->gray_table[i] == -1){
			src_histogram_data->gray_table[i] = src_histogram_data->gray_table[i-1];
		}
	}
	return 0;
}

int histogram_operation(HBMP_i_t *src, HISTOGRAM_OP operation, void* arg)
{
	
	switch(operation)
	{
		case HISTOGRAM_EQUALIZATION:
		{
			HISTOGRAM_DATA histogram_data;
			histogram_data.src = src;
			histogram_data.max = histogram_table_init(&histogram_data);
			normal_gray_table(&histogram_data);			
			histogram_equalization(&histogram_data);			
			break;
		}

		case HISTOGRAM_MATCHING:
		{
			HISTOGRAM_DATA src_histogram_data, dst_histogram_data;
			src_histogram_data.src = src;
			src_histogram_data.max = histogram_table_init(&src_histogram_data);
			dst_histogram_data.src = (HBMP_i_t *)arg;
			dst_histogram_data.max = histogram_table_init(&dst_histogram_data);
			histogram_matching(&src_histogram_data, &dst_histogram_data);
			histogram_equalization(&src_histogram_data);
			break;
		}
	}
	return 0;
}

