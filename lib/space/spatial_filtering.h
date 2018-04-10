/*author: charles cheng 2017-07-19*/
#ifndef SPATIAL_FILTERING_H
#define SPATIAL_FILTERING_H
#include "epdk.h"
#include "epdk_inc.h"

typedef struct _FILTER_TEMPLATE
{
	float *filter_array;
	uint32_t filter_width;
	uint32_t filter_height;
	uint32_t filter_kernel_location;
	float    filter_coef;
}FILTER_TEMPLATE;

#define FILTER_HW 3
#define FILTER_KERNEL_LOCATION (FILTER_HW-1)/2

float smooth_avg_filter_array[9] = {1, 1, 1, 
									  1, 1 ,1,
									  1, 1 ,1};
FILTER_TEMPLATE smooth_avg = 
{
	.filter_array = smooth_avg_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = (float)1/9,
};
/*
  *for(int i=0;i<size;i++){
  *	for(int j=0;j<size;j++){  
  *        gaus[i][j]=(1/(2*PI*sigma*sigma))*exp(-((i-center)*(i-center)+(j-center)*(j-center))/(2*sigma*sigma));  
  *        sum+=gaus[i][j];
  *	}
  *}
*/
#ifdef smooth_gauss_3
float smooth_gauss_filter_array[9] = {0.0751136, 0.123841, 0.0751136,
									  	0.123841, 0.20418, 0.123841,
									  	0.0751136, 0.123841, 0.0751136};
#else
float smooth_gauss_filter_array[25] = {0.00296902, 0.0133062, 0.0219382, 0.0133062, 0.00296902,
									  	0.0133062, 0.0596343, 0.0983203, 0.0596343, 0.0133062,
									  	0.0219382, 0.0983203, 0.162103, 0.0983203, 0.0219382,
									  	0.0133062, 0.0596343, 0.0983203, 0.0596343, 0.0133062,
									  	0.00296902, 0.0133062, 0.0219382, 0.0133062, 0.00296902};
#endif
FILTER_TEMPLATE smooth_gauss = 
{
	.filter_array = smooth_gauss_filter_array,
	.filter_width = 5,
	.filter_height = 5,
	.filter_kernel_location = 1,
	.filter_coef =(float)1,
};

float hsobel_filter_array[9] = {-1, 0, 1, 
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

float vsobel_filter_array[9] = {-1, -2, -1, 
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

float log_filter_array[25] = {0,  0, -1,  0,  0, 
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

float laplacian5_filter_array[9] = {1,   4, 1, 
								  	  4, -20, 4,
								  	  1,   4, 1};
FILTER_TEMPLATE sharpening_laplacian5 = 
{
	.filter_array = laplacian5_filter_array,
	.filter_width = 3,
	.filter_height = 3,
	.filter_kernel_location = 1,
	.filter_coef = 1,
};

#endif
