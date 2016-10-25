/*author: charles cheng 2016-010-22*/

#include "epdk.h"

int main(void)
{
	HBMP_i_t* hbmp_src = NULL;	
	HBMP_i_t* hbmp_dst = NULL;	
	FILE* file;
	hbmp_src = bmp_parser("src.bmp", "dst.bin");
	
	hbmp_dst = (HBMP_i_t*)malloc(sizeof(HBMP_i_t));
	memcpy(hbmp_dst, hbmp_src, sizeof(HBMP_i_t));
	
	hbmp_dst->rgb_buffer = (uint32_t*)malloc(hbmp_dst->rgb_size); 
	printf("hello\n");

	catmapping(hbmp_src, hbmp_dst, 16);
	
	file = fopen("dst_map.bin","wb+");
	fwrite(hbmp_dst->rgb_buffer, 1, hbmp_dst->rgb_size, file);
	fclose(file);
	return EPDK_OK;
}
