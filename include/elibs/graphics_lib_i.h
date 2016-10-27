/*author: charles cheng 2016-07-21*/
#ifndef GRAPHICS_LIB_I_H
#define GRAPHICS_LIB_I_H

//int picture_init(int usr_height, int usr_width);


//int set_pixel(int x, int y);


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
}HBMP_i_t;

HBMP_i_t* bmp_parser(char *scr_file, char *dst_file);
int catmapping(HBMP_i_t* src, HBMP_i_t *dst, uint32_t map_times);
uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst);


#endif
