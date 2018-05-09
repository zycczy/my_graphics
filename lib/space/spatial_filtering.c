/*author: charles cheng 2017-07-19*/
#include "spatial_filtering.h"
//#define ENHANCE_FLITER
//#define SPATIAL_FILTERING_RGB
static uint8_t get_median_value(float *array, uint32_t length)
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

static uint8_t median_filtering(HBMP_i_t *src, uint32_t x, uint32_t y)
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


static float templete_filter(HBMP_i_t *src, FILTER_TEMPLATE *filter, uint32_t x, uint32_t y, RGB_CHANNEL m)
{
	uint32_t i, j;	
	float dst_value = 0;

	for(i=0;i<filter->filter_height;i++){
		for(j=0;j<filter->filter_width;j++){
			#ifdef SPATIAL_FILTERING_RGB 
			if(m == R_CHANNEL){
				dst_value += (ARGB_PARSE_R(src->get_rgb_value(src, x-filter->filter_kernel_location+j, y-filter->filter_kernel_location+i)) * filter->filter_array[i*filter->filter_width+j]);
			}else if(m == G_CHANNEL){
				dst_value += (ARGB_PARSE_G(src->get_rgb_value(src, x-filter->filter_kernel_location+j, y-filter->filter_kernel_location+i)) * filter->filter_array[i*filter->filter_width+j]);			
			}else if(m == B_CHANNEL){
				dst_value += (ARGB_PARSE_B(src->get_rgb_value(src, x-filter->filter_kernel_location+j, y-filter->filter_kernel_location+i)) * filter->filter_array[i*filter->filter_width+j]);
			}
			#else
			dst_value += ((src->get_y_value(src, x-filter->filter_kernel_location+j, y-filter->filter_kernel_location+i)) * filter->filter_array[i*filter->filter_width+j]);
			#endif
		}
	}
	dst_value *= filter->filter_coef;
	#ifndef ENHANCE_FLITER
	dst_value = fabs(dst_value);
	#endif
	if(dst_value > 255)
		dst_value = 255;
	if(dst_value < -255)
		dst_value = -255;


	return dst_value;
}

static FILTER_TEMPLATE *init_filter_array(SPATIAL_FILTER_METHOD filter_method, int arg)
{
	switch(filter_method)
	{
		case TEMPLATE_SMOOTH_AVG:
		{
			return &smooth_avg;
		}
		case TEMPLATE_SMOOTH_GAUSS:
		{
			int i, j;
			float sum;
			float smooth_gauss_filter_array[25];
			double sigma = (double)arg;
			for(i=0;i<5;i++){
				for(j=0;j<5;j++){  
					smooth_gauss_filter_array[i*5+j] = (1/(2*PI*sigma*sigma))*exp(-((i-2)*(i-2)+(j-2)*(j-2))/(2*sigma*sigma));  
					sum += smooth_gauss_filter_array[i*5+j];
				}
			}
			for(i=0;i<5;i++){
				for(j=0;j<5;j++){  
					smooth_gauss_filter_array[i*5+j] /= sum;
				}
			}
			smooth_gauss.filter_array = smooth_gauss_filter_array;
			smooth_gauss.filter_width = 5;
			smooth_gauss.filter_height = 5;
			smooth_gauss.filter_kernel_location = 1;
			smooth_gauss.filter_coef =(float)arg;
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
		case TEMPLATE_LAPLACIAN5:
		{
			return &sharpening_laplacian5;
		}
	}
	return EPDK_FAIL;
}
#define WEIGHT_COEF 1.6

#ifdef SPATIAL_FILTERING_RGB 
int min_r = 65535;
int max_r = 0;

int min_g = 65535;
int max_g = 0;

int min_b = 65535;
int max_b = 0;

void spatial_filter(HBMP_i_t *src, SPATIAL_FILTER_METHOD filter_method, int arg)
{
	uint32_t i, j;
	
	FILTER_TEMPLATE *filter = init_filter_array(filter_method, arg);
	int32_t *tmp = malloc(src->rgb_size);
	int32_t *tmp_r = malloc(src->rgb_size);
	int32_t *tmp_g = malloc(src->rgb_size);	
	int32_t *tmp_b = malloc(src->rgb_size);
	memcpy(tmp, src->rgb_buffer, src->rgb_size);
	for(i=filter->filter_kernel_location;i<src->height-filter->filter_kernel_location*2;i++){
		for(j=filter->filter_kernel_location;j<src->width-filter->filter_kernel_location*2;j++){	
			tmp_r[i*src->width+j] = WEIGHT_COEF*ARGB_PARSE_R(src->rgb_buffer[i*src->width+j]) + (templete_filter(src, filter, j, i, R_CHANNEL));
			tmp_g[i*src->width+j] = WEIGHT_COEF*ARGB_PARSE_G(src->rgb_buffer[i*src->width+j]) + (templete_filter(src, filter, j, i, G_CHANNEL));
			tmp_b[i*src->width+j] = WEIGHT_COEF*ARGB_PARSE_B(src->rgb_buffer[i*src->width+j]) + (templete_filter(src, filter, j, i, B_CHANNEL));

			max_r = MAX(max_r, tmp_r[i*src->width+j]);
			min_r = MIN(min_r, tmp_r[i*src->width+j]);

			max_g = MAX(max_g, tmp_g[i*src->width+j]);
			min_g = MIN(min_g, tmp_g[i*src->width+j]);

			max_b = MAX(max_b, tmp_b[i*src->width+j]);
			min_b = MIN(min_b, tmp_b[i*src->width+j]);
		}
	}
	int span_r = max_r - min_r;
	int span_g = max_g - min_g;	
	int span_b = max_b - min_b;
	int r, g, b;
	for(i=filter->filter_kernel_location;i<src->height-filter->filter_kernel_location*2;i++){
		for(j=filter->filter_kernel_location;j<src->width-filter->filter_kernel_location*2;j++){
			if(span_r > 0){
				r = (tmp_r[i*src->width+j] - min_r)*255/span_r;
			}else if(tmp_r[i*src->width+j] <= 255){
				r = tmp_r[i*src->width+j];
			}else{
				r = 255;
			}

			if(span_g > 0){
				g = (tmp_g[i*src->width+j] - min_g)*255/span_g;
			}else if(tmp_g[i*src->width+j] <= 255){		
				g = tmp_g[i*src->width+j];
			}else{
				g = 255;
			}

			if(span_b > 0){
				b = (tmp_b[i*src->width+j] - min_b)*255/span_b;
			}else if(tmp_b[i*src->width+j] <= 255){		
				b = tmp_b[i*src->width+j];
			}else{
				b = 255;
			}

			src->rgb_buffer[i*src->width+j] = ARGB_SET_RGB(r, g, b);
		}
	}

	return ;
}
#else
int min = 65535;
int max = 0;

void spatial_filter(HBMP_i_t *src, SPATIAL_FILTER_METHOD filter_method, int arg)
{
	uint32_t i, j;
	
	FILTER_TEMPLATE *filter = init_filter_array(filter_method, arg);
	int32_t *tmp = malloc(src->yuv_buffer.y_buffer.size*4);
	memcpy(tmp, src->yuv_buffer.y_buffer.buffer, src->yuv_buffer.y_buffer.size);
	for(i=filter->filter_kernel_location;i<src->height-filter->filter_kernel_location*2;i++){
		for(j=filter->filter_kernel_location;j<src->width-filter->filter_kernel_location*2;j++){	
			#ifndef ENHANCE_FLITER
			tmp[i*src->width+j] = templete_filter(src, filter, j, i, 0);
			#else
			tmp[i*src->width+j] = WEIGHT_COEF*(src->yuv_buffer.y_buffer.buffer[i*src->width+j]) + (templete_filter(src, filter, j, i, 0));

			max = MAX(max, tmp[i*src->width+j]);
			min = MIN(min, tmp[i*src->width+j]);

			#endif
		}
	}
	int span = max - min;
	int y;
	for(i=filter->filter_kernel_location;i<src->height - filter->filter_kernel_location*2;i++){
		for(j=filter->filter_kernel_location;j<src->width - filter->filter_kernel_location*2;j++){
			if(span > 0){
				y = (tmp[i*src->width+j] - min)*255/span;
			}else if(tmp[i*src->width+j] <= 255){
				y = tmp[i*src->width+j];
			}else{
				y = 0;
			}

			src->yuv_buffer.y_buffer.buffer[i*src->width+j] = y;
		}
	}

	return ;
}

#endif
