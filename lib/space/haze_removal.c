/*author: charles cheng 2018-07-17*/
#include "haze_removal.h"
#define WIN_SIZE 15
static void dark_channel(HBMP_i_t *src, HBMP_i_t *dst)
{
	int i, j;
	int tmp_rgb;
	for (i=0; i<src->width; i++) {
		for (j=0; j<src->height; j++) {
			tmp_rgb = src->get_rgb_value(src, i, j);
			dst->yuv_buffer.y_buffer.buffer[src->width*j+i] = 0;
			dst->yuv_buffer.y_buffer.buffer[src->width*j+i] = MIN(ARGB_PARSE_G(tmp_rgb), MIN(ARGB_PARSE_R(tmp_rgb),  ARGB_PARSE_B(tmp_rgb)));
		}
	}
}

static void min_filter(HBMP_i_t *src, HBMP_i_t *dst)
{
	int win_size = WIN_SIZE;
	int win_radius = (win_size-1)/2;
	int win_min = 255;
	int i, j, m;
	int value;
	
	for (i=win_radius; i<src->width-win_radius; i++) {
		for (j=win_radius; j<src->height-win_radius; j++) {
			for(m = -win_radius; m <=win_radius; m++ ) {
				if(m != 0) {
					value = src->get_y_value(src, i+m, j+m);
					win_min = MIN(value, win_min);
				}
			}
			dst->yuv_buffer.y_buffer.buffer[src->width*j+i] = win_min;
			win_min = 255;		
		}
		
	}
	
}


/**
	A: atmospheric optics
	J = (I - A)/t +A
	t = 1 - w*min(I/A)
**/


static int AO_value(HBMP_i_t *dark, HBMP_i_t *src)
{
	
	int win_size = WIN_SIZE;
	int win_radius = (win_size-1)/2;	
	int i, j, m;

	uint8_t *ao_array = malloc((src->width-win_radius)*(src->height-win_radius));
	for (i=win_radius; i<src->width-win_radius; i++) {
		for (j=win_radius; j<src->height-win_radius; j++) {
			
		}		
	}
}

int  haze_removal(HBMP_i_t *src, HBMP_i_t *dst)
{
	HBMP_i_t dst2;

	hbmp_copy(src, dst);
	hbmp_copy(src, &dst2);
	dark_channel(src, dst);
	min_filter(dst, &dst2);
	hbmp_copy(&dst2, dst);
	return 0;
}

