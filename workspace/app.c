/*author: charles cheng 2016-010-22*/

#include "epdk.h"
#define SEPARATE_HEIGHT 32
#define SEPARATE_WIDTH  32
int main(void)
{
	HBMP_i_t* hbmp_src = NULL;	
	HBMP_i_t* hbmp_dst = NULL;
	hbmp_src = bmp_parser("src.bmp", "dst.bin");
	
	//catmapping	
	FILE* file;
	hbmp_dst = (HBMP_i_t*)malloc(sizeof(HBMP_i_t));
	memcpy(hbmp_dst, hbmp_src, sizeof(HBMP_i_t));
	hbmp_dst->rgb_buffer = (uint32_t*)malloc(hbmp_dst->rgb_size); 
	catmapping(hbmp_src, hbmp_dst, 189);
	file = fopen("dst_map.bin","wb+");
	fwrite(hbmp_dst->rgb_buffer, 1, hbmp_dst->rgb_size, file);

	//separate maritx
	HBMP_i_t** dst = NULL;
	FILE*      maritx_file;
	uint32_t   i;
	uint32_t   maritx_copies = (hbmp_src->height/SEPARATE_HEIGHT)*(hbmp_src->width/SEPARATE_WIDTH);
	uint32_t   separate_size = SEPARATE_HEIGHT*SEPARATE_WIDTH;
	dst = (HBMP_i_t**)malloc(sizeof(HBMP_i_t*)*maritx_copies);
	for(i=0;i<maritx_copies;i++){
		dst[i]= (HBMP_i_t*)malloc(sizeof(HBMP_i_t));		
		dst[i]->rgb_size = separate_size*32;
		dst[i]->rgb_buffer = malloc(((*dst)+i)->rgb_size); 
		dst[i]->height = SEPARATE_HEIGHT;
		dst[i]->width  = SEPARATE_WIDTH;
	}
	__dbg("start separate\n");
	separate_maritx(hbmp_src, dst);
	maritx_file = fopen("maritx0.bin","wb+");
	fwrite(dst[0]->rgb_buffer, 1, dst[0]->rgb_size, maritx_file);

	free(hbmp_dst);
	free(hbmp_src);
	fclose(file);
	fclose(maritx_file);
	return EPDK_OK;
}
