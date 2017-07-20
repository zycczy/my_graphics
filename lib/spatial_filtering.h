/*author: charles cheng 2017-07-19*/
#ifndef SPATIAL_FILTERING_H
#define SPATIAL_FILTERING_H
#include "epdk.h"
#include "epdk_inc.h"

typedef struct _FILTER_TEMPLATE
{
	int8_t *filter_array;
	uint32_t filter_width;
	uint32_t filter_height;
	uint32_t filter_kernel_location;
	float    filter_coef;
}FILTER_TEMPLATE;

#define FILTER_HW 3
#define FILTER_KERNEL_LOCATION (FILTER_HW-1)/2

uint8_t smooth_avg_filter_array[9] = {1, 1, 1, 
									  1, 1 ,1,
									  1, 1 ,1};
FILTER_TEMPLATE smooth_avg = 
{
	.filter_array = smooth_avg_filter_array;
	.filter_width = FILTER_HW;
	.filter_height = FILTER_HW;
	.filter_kernel_location = FILTER_KERNEL_LOCATION;
	.filter_coef = 1/9;
};


uint8_t smooth_gauss_filter_array[9] = {1, 2, 1, 
									  	2, 4, 2,
									  	1, 2, 1};
FILTER_TEMPLATE smooth_avg = 
{
	.filter_array = smooth_gauss_filter_array;
	.filter_width = FILTER_HW;
	.filter_height = FILTER_HW;
	.filter_kernel_location = FILTER_KERNEL_LOCATION;
	.filter_coef = 1/16;
};

int8_t hsobel_filter_array[9] = {-1, 0, 1, 
								  -2, 0, 2,
								  -1, 0, 1};
FILTER_TEMPLATE smooth_avg = 
{
	.filter_array = smooth_avg_filter_array;
	.filter_width = FILTER_HW;
	.filter_height = FILTER_HW;
	.filter_kernel_location = FILTER_KERNEL_LOCATION;
	.filter_coef = 1;
};

#endif
