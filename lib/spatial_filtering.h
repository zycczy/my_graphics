/*author: charles cheng 2017-07-19*/
#ifndef SPATIAL_FILTERING_H
#define SPATIAL_FILTERING_H
#include "epdk.h"
#include "epdk_inc.h"

typedef struct _FILTER_TEMPLATE
{
	uint8_t *filter_array;
	uint32_t filter_width;
	uint32_t filter_height;
	uint32_t filter_kernel_local;
}FILTER_TEMPLATE;

#define FILTER_HW 3
#endif
