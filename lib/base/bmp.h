/*author: charles cheng 2017-05-04*/

#ifndef BMP_H
#define BMP_H
#include "epdk.h"
#include "epdk_inc.h"


#pragma pack(1)
typedef struct _bmp_file_head_t
{
	uint8_t    bfType[2];        // "BM"
	uint32_t   bfSize;           // total size of bmp file
	uint32_t   bfReserved;       // reserved field
	uint32_t   bfOffBits;        // pixel matrix offset from file head
}bmp_file_head_t;


typedef struct _bmp_info_head_t
{
	uint32_t   biSize;           // the size of bmp information head
	uint32_t   biWidth;          // the width of bmp, unit: pixel
	uint32_t   biHeight;         // the height of bmp, unit:pixel
	uint16_t   biPlanes;         // always 1
	uint16_t   biBitCount;       // bits per pixel. 1-mono, 4-16color, 8-256color, 24-truecolor
	uint32_t   biCompression;    // copression format. 0: no compression.
	uint32_t   biSizeImage;      // the size of pixel matirx.
	uint32_t   biXPelsPerMeter;  // horizontal dis, unit : pixel/meter
	uint32_t   biYPelsPerMeter;  // vertical dis, unit : pixel/meter
	uint32_t   biClrUsed;        // number of used colors, 0 means 2^biBitCount
	uint32_t   biClrImportant;   // number of important colors, 0 means that all colors are important.
}bmp_info_head_t;
#pragma pack()



void bmp_open( const char *bmp_file, HBMP_i_t *hbmp_hdl_buf);
#endif
