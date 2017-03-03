/*author: charles cheng 2016-07-21*/
#ifndef GRAPHICS_LIB_I_H
#define GRAPHICS_LIB_I_H
//int picture_init(int usr_height, int usr_width);

#include "epdk_inc.h"


//int set_pixel(int x, int y);
typedef enum _YUV_STORE_TYPE
{
	YUV420 = 420,
	YUV422 = 422,
	YUV444 = 444,
}YUV_STORE_TYPE;

typedef enum _TYPE_OF_MARITX
{
	YUV = 0,
	RGB32BIT,
	RGB1BIT,
}TYPE_OF_MARITX;

typedef struct _YUV_BUFFER
{
	EPDK_BUFFER     y_buffer;
	EPDK_BUFFER     u_buffer;
	EPDK_BUFFER     v_buffer;
	YUV_STORE_TYPE  type;
}YUV_BUFFER;
typedef struct _HBMP_i_t 
{
	uint32_t   byte_count;
	uint32_t   bitcount;
	uint32_t   width;
	uint32_t   height;
	uint32_t   matrix_off;
	uint32_t   *rgb_buffer;
	uint32_t   rgb_size;
	uint32_t   row_size;
	YUV_BUFFER yuv_buffer;
}HBMP_i_t;

typedef struct _MARITX_HBMP
{
	HBMP_i_t* hbmp;
	uint32_t  height_coordinate;
	uint32_t  width_coordinate;
}MARITX_HBMP;

HBMP_i_t* bmp_parser(char *scr_file, char *dst_file);
int catmapping(HBMP_i_t* src, HBMP_i_t *dst, uint32_t map_times);
uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst, TYPE_OF_MARITX type);
int32_t rgb_tranform_to_yuv(HBMP_i_t* hbmp);
void yuv_buffer_init(HBMP_i_t* hbmp);


#endif
