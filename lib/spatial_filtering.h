/*author: charles cheng 2017-07-19*/
#ifndef SPATIAL_FILTERING_H
#define SPATIAL_FILTERING_H
#include "epdk.h"
#include "epdk_inc.h"

typedef struct _FILTER_TEMPLATE
{
	int32_t *filter_array;
	uint32_t filter_width;
	uint32_t filter_height;
	uint32_t filter_kernel_location;
	float    filter_coef;
}FILTER_TEMPLATE;

#define FILTER_HW 3
#define FILTER_KERNEL_LOCATION (FILTER_HW-1)/2

int32_t smooth_avg_filter_array[9] = {1, 1, 1, 
									  1, 1 ,1,
									  1, 1 ,1};
FILTER_TEMPLATE smooth_avg = 
{
	.filter_array = smooth_avg_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = 1/9,
};


int32_t smooth_gauss_filter_array[9] = {1, 2, 1, 
									  	2, 4, 2,
									  	1, 2, 1};
FILTER_TEMPLATE smooth_gauss = 
{
	.filter_array = smooth_gauss_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = 1/16,
};

int32_t hsobel_filter_array[9] = {-1, 0, 1, 
								 -2, 0, 2,
								 -1, 0, 1};
FILTER_TEMPLATE sharpening_hsobel = 
{
	.filter_array = hsobel_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = 1,
};

int32_t vsobel_filter_array[9] = {-1, -2, -1, 
								  0,  0,  0,
								  1,  2,  1};
FILTER_TEMPLATE sharpening_vsobel = 
{
	.filter_array = vsobel_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = 1,
};

int32_t log_filter_array[25] = {0,  0, -1,  0,  0, 
							    0, -1, -2, -1,  0,
							   -1, -2, 16, -2, -1,
							    0, -1, -2, -1,  0,
							    0,  0, -1,  0,  0};
FILTER_TEMPLATE sharpening_log = 
{
	.filter_array = log_filter_array,
	.filter_width = 5,
	.filter_height = 5,
	.filter_kernel_location = 2,
	.filter_coef = 1,
};

#endif
