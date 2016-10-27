/*author: charles cheng 2016-07-21*/
#ifndef GRAPHICS_LIB_H
#define GRAPHICS_LIB_H
#include "epdk.h"
#include "bmp.h"
#define DST_FILE_PWD 	"/Users/czy/Documents/Graphics/workspace/example.bin"

#define BLACK_1BIT      0x0
#define WHITE_1BIT		0x1

#define BLACK_32BIT     0xFF000000
#define WHITE_32BIT		0xFFFFFFFF

#define PIXEL_COLOR		BLACK_32BIT

HBMP_i_t* bmp_parser(char *scr_file, char *dst_file);
uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst);


#endif
