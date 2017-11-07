/*author: charles cheng 2017-05-04*/
#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "epdk.h"
#include "epdk_inc.h"


typedef struct _HISTOGRAM_DATA
{
	HBMP_i_t *src;	
	int16_t gray_table[GRAY_PROGRESSION];
	uint32_t CDF_table[GRAY_PROGRESSION];
	uint8_t max;
	
}HISTOGRAM_DATA;

#endif
