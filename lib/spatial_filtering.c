/*author: charles cheng 2017-07-19*/
#include "spatial_filtering.h"
static uint8_t get_median_value(uint8_t *array, uint32_t length)
{
	uint32_t i, j;
	uint8_t tmp;
	for(i=0;i<length;i++){
		for(j=0;j<length-i;j++){
			if(array[j]>array[j+1]){
				tmp = array[j];
				array[j] = array[j+1];
				array[j+1] = tmp;
			}
		}
	}
	return (length & 1)?array[(length+1)/2]:(array[length/2+1]+array[length/2]);
}

static uint8_t *median_filtering(HBMP_i_t *src, uint32_t x, uint32_t y)
{
	uint32_t i, j;
	uint8_t dst_y_value;
	FILTER_TEMPLATE *filter = malloc(sizeof(FILTER_TEMPLATE));
	filter->filter_height = filter->filter_width = FILTER_HW;
	filter->filter_array = malloc(FILTER_HW*FILTER_HW*4);
	filter->filter_kernel_location = FILTER_KERNEL_LOCATION;
	for(i=0;i<filter->filter_height;i++){
		for(j=0;j<filter->filter_width;j++){
			filter->filter_array[i*filter->filter_width+j] = src->get_y_value(src, x-filter->filter_kernel_location+i, y-filter->filter_kernel_location+j);
		}
	}
	dst_y_value = get_median_value(filter->filter_array, FILTER_HW*FILTER_HW-1);
	return (src->get_y_value(src, x, y) == filter->filter_array[0] || 
			src->get_y_value(src, x, y) == filter->filter_array[FILTER_HW*FILTER_HW-1])?
			dst_y_value:src->get_y_value(src, x, y);
}

static int32_t *templete_filter(HBMP_i_t *src, FILTER_TEMPLATE *filter, uint32_t x, uint32_t y)
{
	uint32_t i, j;	
	int32_t dst_y_value = 0;
	for(i=0;i<filter->filter_height;i++){
		for(j=0;j<filter->filter_width;j++){
			dst_y_value += (src->get_y_value(src, x-filter->filter_kernel_location+j, y-filter->filter_kernel_location+i)*filter->filter_array[i*filter->filter_width+j]);
		}
	}
	dst_y_value *= filter->filter_coef;
	dst_y_value  = fabs(dst_y_value);
	return dst_y_value>255?255:dst_y_value;
}

static FILTER_TEMPLATE *init_filter_array(SPATIAL_FILTER_METHOD filter_method)
{
	switch(filter_method)
	{
		case TEMPLATE_SMOOTH_AVG:
		{
			return &smooth_avg;
		}
		case TEMPLATE_SMOOTH_GAUSS:
		{
			return &smooth_gauss;
		}
		case TEMPLATE_HSOBLE:
		{
			return &sharpening_hsobel;
		}
		case TEMPLATE_VSOBLE:
		{
			return &sharpening_vsobel;
		}
		case TEMPLATE_LOG:
		{
			return &sharpening_log;
		}

	}
}

void spatial_filter(HBMP_i_t *src, SPATIAL_FILTER_METHOD filter_method)
{
	uint32_t i, j;
	FILTER_TEMPLATE *filter = init_filter_array(filter_method);
	for(i=filter->filter_kernel_location;i<src->height-filter->filter_kernel_location*2;i++){
		for(j=filter->filter_kernel_location;j<src->width-filter->filter_kernel_location*2;j++){
			src->yuv_buffer.y_buffer.buffer[i*src->width+j] = templete_filter(src, filter, j, i);
		}
	}
	return ;
}
