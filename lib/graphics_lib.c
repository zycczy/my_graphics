/*author: charles cheng 2016-10-22*/

#include "graphics_lib.h"
uint32_t pixel_white_32bit = WHITE_32BIT;
uint32_t pixel_black_32bit = BLACK_32BIT;


static int transform_1bit_to_32bit(HBMP_i_t *hbmp_buf, FILE* file)
{
	uint32_t i = 0;
	uint32_t j = 0;
	if(hbmp_buf->rgb_size != 0 && hbmp_buf->rgb_buffer != NULL){
		__msg("rgb_size:%d, rgb_buffer = 0x%x\n", hbmp_buf->rgb_size, (int)hbmp_buf->rgb_buffer);

		for(i=0;i<hbmp_buf->rgb_size;i++){
			for(j=0;j<8;j++){
				if(((*((uint8_t *)hbmp_buf->rgb_buffer+i)>>j)&0x1) == WHITE_1BIT){
					fwrite(&pixel_white_32bit, 1, 4, file);
				}else{
					fwrite(&pixel_black_32bit, 1, 4, file);
				}
			}
		}
	}
	return 0;
}

HBMP_i_t* bmp_parser(char *scr_file, char *dst_file)
{
	HBMP_i_t *hbmp_buf = malloc(sizeof(HBMP_i_t));
	FILE* file;
	memset(hbmp_buf, 0, sizeof(HBMP_i_t));
	bmp_open(scr_file, hbmp_buf);
	
	file = fopen(dst_file,"wb+");
	if(hbmp_buf->bitcount == 32){
		if(hbmp_buf->rgb_size != 0 && hbmp_buf->rgb_buffer != NULL){	
			__msg("rgb_size:%d, rgb_buffer = 0x%x\n", hbmp_buf->rgb_size, (int)hbmp_buf->rgb_buffer);
			fwrite(hbmp_buf->rgb_buffer, 1, hbmp_buf->rgb_size, file);
		}
	}else if(hbmp_buf->bitcount == 1){
		transform_1bit_to_32bit(hbmp_buf, file);
		fclose(file);
		file = fopen(dst_file,"rb+");
		free(hbmp_buf->rgb_buffer);
		hbmp_buf->rgb_size *= 32;
		hbmp_buf->rgb_buffer = (uint32_t*)malloc(hbmp_buf->rgb_size);
		fread(hbmp_buf->rgb_buffer, 1, hbmp_buf->rgb_size, file);
	}else{
		__wrn("Cannot support this bit map!\n");
	}
	fclose(file);
	return hbmp_buf;
}
typedef struct _MARITX_HBMP
{
	HBMP_i_t* hbmp;
	uint32_t  height_coordinate;
	uint32_t  width_coordinate;
}MARITX_HBMP;


uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst)
{
	uint32_t width_copies = hbmp->width / dst[0]->width;
	uint32_t height_copies = hbmp->height / dst[0]->height;
	uint32_t copies_count = width_copies * height_copies;
	MARITX_HBMP* maritx;
	uint32_t i,j,t;
	if(hbmp->height%dst[0]->height != 0 && hbmp->width%dst[0]->width != 0){
		__wrn("cannot separate the picture average!\n");
		return EPDK_FAIL;
	}
	//specify the coordinate
	t = 0;
	maritx = (MARITX_HBMP *)malloc(sizeof(MARITX_HBMP)*copies_count);
	while(t<copies_count){
		for(i=0;i<height_copies;i++){
			for(j=0;j<width_copies;j++){
				maritx[copies_count].hbmp = dst[copies_count];
				maritx[copies_count].height_coordinate = i;
				maritx[copies_count].width_coordinate = j;
				
				t++;
			}
		}
	}
	
	for(t=0;t<copies_count;t++){
		for(i=0;i<maritx[copies_count].hbmp->height;i++){
			memcpy(maritx[copies_count].hbmp->rgb_buffer+(maritx[copies_count].hbmp->width*i),\
				   hbmp->rgb_buffer+(maritx[copies_count].height_coordinate*t)+maritx[copies_count].width_coordinate*maritx[copies_count].hbmp->width,\
				   maritx[copies_count].hbmp->width);
		}
	}
	#if 0
	for(t=0;t<copies_count;t++){
		for(i=0;i<dst->height;i++){
			for(j=0;j<width_copies;j++){
				memcpy(((*(dst+t))+(dst->width*i)), hbmp->rgb_buffer+)
			}
		}
	}
	#endif
	return EPDK_OK;
}



