/*author: charles cheng 2016-07-21*/
#ifndef GRAPHICS_LIB_I_H
#define GRAPHICS_LIB_I_H
//int picture_init(int usr_height, int usr_width);

#include "epdk_inc.h"
#define GRAY_PROGRESSION 256
#define BLACK_1BIT      0x0
#define WHITE_1BIT		0x1

#define BLACK_32BIT     0xFF000000
#define WHITE_32BIT		0xFFFFFFFF

#define PIXEL_COLOR		BLACK_32BIT

#define ARGB_PARSE_R(rgb)	((rgb&0x00ff0000)>>16)		
#define ARGB_PARSE_G(rgb)	((rgb&0x0000ff00)>>8)
#define ARGB_PARSE_B(rgb)	(rgb&0x000000ff)
#define ARGB_SET_RGB(r, g, b)    (((r<<16) |(g<<8) |(b)) |0xff000000)



typedef enum _SPATIAL_FILTER_METHOD
{
	TEMPLATE_SMOOTH_AVG = 0,
	TEMPLATE_SMOOTH_GAUSS,
	TEMPLATE_HSOBLE,
	TEMPLATE_VSOBLE,
	TEMPLATE_LOG,
	TEMPLATE_LAPLACIAN1,
	TEMPLATE_LAPLACIAN2,	
	TEMPLATE_LAPLACIAN5,
}SPATIAL_FILTER_METHOD;

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

typedef enum _TYPE_OF_MIRROR
{
	HORIZONTAL = 0,
	VERTICAL,
}TYPE_OF_MIRROR;

typedef enum _WAY_OF_TRANSPOSE
{
	NEAREST_NEIGHOR_INTERPOLATION = 0,
	BILINEAR_INTERPOLATION,
	BICUBIC_INTERPOLATION,
}WAY_OF_TRANSPOSE;

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
	
	uint32_t   (*set_rgb_value)(void*, uint32_t, uint32_t, uint32_t);	
	uint32_t   (*get_rgb_value)(void*, uint32_t, uint32_t);	
	uint8_t   (*get_y_value)(void*, uint32_t, uint32_t);  
}HBMP_i_t;

typedef struct _MARITX_HBMP
{
	HBMP_i_t* hbmp;
	uint32_t  height_coordinate;
	uint32_t  width_coordinate;
}MARITX_HBMP;

typedef enum _HISTOGRAM_OP
{
	HISTOGRAM_EQUALIZATION = 0,
	HISTOGRAM_MATCHING,
}HISTOGRAM_OP;

typedef struct _COMPLEX_NUMBER
{
	double real;
	double imaginary;
}COMPLEX_NUMBER;

typedef struct _FFT_STRUCT
{
	HBMP_i_t *spectrum;
	int expand;
	COMPLEX_NUMBER *freq_image;
	uint8_t fill_luma;
}FFT_STRUCT;


HBMP_i_t* bmp_parser(char *scr_file, char *dst_file);
int catmapping(HBMP_i_t* src, HBMP_i_t *dst, uint32_t map_times);
uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst, TYPE_OF_MARITX type);

/*--------------------------------------------------------------------------------*/
int32_t rgb_tranform_to_yuv(HBMP_i_t* hbmp);
void yuv_buffer_init(HBMP_i_t* hbmp);

/*--------------------------------------------------------------------------------*/

int image_FFT(HBMP_i_t *src, FFT_STRUCT *fft_dst);
int image_IFFT(HBMP_i_t *src, FFT_STRUCT *fft_dst);

/*--------------------------------------------------------------------------------*/
uint32_t gamma_correct(HBMP_i_t *hbmp_buf, float gamma);
int histogram_operation(HBMP_i_t *src, HISTOGRAM_OP operation, void* arg);

/*--------------------------------------------------------------------------------*/
int image_transformation(HBMP_i_t *src, int32_t x, int32_t y);
int image_mirror(HBMP_i_t *src, TYPE_OF_MIRROR type);
int image_transpose(HBMP_i_t *src, WAY_OF_TRANSPOSE transpose_way, uint32_t arg);

/*--------------------------------------------------------------------------------*/
uint32_t bilinear_interpolation(HBMP_i_t *src, double x, double y);
uint32_t bicubic_interpolation(HBMP_i_t *src, double x, double y);

void spatial_filter(HBMP_i_t *src, SPATIAL_FILTER_METHOD filter_method);

#endif
