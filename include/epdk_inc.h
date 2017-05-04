/*author: charles cheng 2016-07-21*/
#ifndef EPDK_INC_H
#define EPDK_INC_H

#define alignment_down(a, size) (a & (~(size-1)))
#define alignment_up(a, size)	((a+size-1) & (~ (size-1)))

#define NO_ARG 0
#define HAS_ARG 1

#define PI 3.1415926
typedef struct _EPDK_BUFFER
{
	union{
		uint8_t *buffer;
		uint32_t *buffer_32bit;
	};
	uint32_t size;
}EPDK_BUFFER;
#endif
