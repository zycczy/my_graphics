/*author: charles cheng 2016-010-22*/

#include "epdk.h"
#include <getopt.h>
#define SEPARATE_HEIGHT 512
#define SEPARATE_WIDTH  512
static const char *short_options = "f:yc:s:";
static struct option long_options[] = {	
	{"catmap",	   HAS_ARG, 0, 'c'},
	{"rgb32->yuv", NO_ARG,  0, 'y'},
	{"separate",   HAS_ARG, 0, 's'},

};

int main(int argc, char **argv)
{
	HBMP_i_t* hbmp_src = NULL;	
	HBMP_i_t* hbmp_dst = NULL;
	int ch, option_index = 0;
	hbmp_src = bmp_parser("src.bmp", "dst.bin");

	while(1){
		ch = getopt_long(argc, argv, short_options, long_options, &option_index);
		
		if (ch < 0)
			break;
		switch (ch) {
			case 'c':
			{
				//catmapping	
				FILE* file;
				uint32_t map_count = 0;
				map_count = strtoul(optarg, NULL, 0);
				__dbg("map_count = %d\n", map_count);
				hbmp_dst = (HBMP_i_t*)malloc(sizeof(HBMP_i_t));
				memcpy(hbmp_dst, hbmp_src, sizeof(HBMP_i_t));
				hbmp_dst->rgb_buffer = (uint32_t*)malloc(hbmp_dst->rgb_size); 
				catmapping(hbmp_src, hbmp_dst, map_count);
				file = fopen("dst_map.bin","wb+");
				fwrite(hbmp_dst->rgb_buffer, 1, hbmp_dst->rgb_size, file);
				free(hbmp_dst);
				fclose(file);
				break;
			}
			case 's':
			{
				if(argc != 5){
					__wrn("separate parameter error\n");
					break;
				}
				//separate maritx
				HBMP_i_t** dst = NULL;
				FILE*      maritx_file;
				uint32_t   i;
				uint32_t   height = strtoul(argv[3], NULL, 0);
				uint32_t   width  = strtoul(argv[4], NULL, 0);
				uint32_t   maritx_copies = (hbmp_src->height/height)*(hbmp_src->width/width);
				uint32_t   separate_size = height*width;
				dst = (HBMP_i_t**)malloc(sizeof(HBMP_i_t*)*maritx_copies);
				for(i=0;i<maritx_copies;i++){
					dst[i]= (HBMP_i_t*)malloc(sizeof(HBMP_i_t));		
					dst[i]->rgb_size = separate_size*4;
					dst[i]->rgb_buffer = malloc(dst[i]->rgb_size); 
					dst[i]->height = height;
					dst[i]->width  = width;
					
					dst[i]->yuv_buffer.type = YUV444;
					yuv_buffer_init(dst[i]);
				}
				__dbg("start separate\n");
				__dbg("maritx_copies = %d\n", maritx_copies);
				
				if(!strcmp(argv[2], "-Y")){
					__dbg("yuv separate!\n");
					hbmp_src->yuv_buffer.type = YUV444;
					rgb_tranform_to_yuv(hbmp_src);
					separate_maritx(hbmp_src, dst, YUV);
					
					__dbg("end separate\n");
					maritx_file = fopen("maritx0_y.bin","wb+");
					fwrite(dst[0]->yuv_buffer.y_buffer.buffer, 1, dst[0]->yuv_buffer.y_buffer.size, maritx_file);		
					fclose(maritx_file);
					
					maritx_file = fopen("maritx0_u.bin","wb+");
					fwrite(dst[0]->yuv_buffer.u_buffer.buffer, 1, dst[0]->yuv_buffer.u_buffer.size, maritx_file);		
					fclose(maritx_file);

					maritx_file = fopen("maritx0_v.bin","wb+");
					fwrite(dst[0]->yuv_buffer.v_buffer.buffer, 1, dst[0]->yuv_buffer.v_buffer.size, maritx_file);		
					fclose(maritx_file);
				}else if(!strcmp(argv[2], "-R32")){
					separate_maritx(hbmp_src, dst, RGB32BIT);	
					
					maritx_file = fopen("maritx0.bin","wb+");
					fwrite(dst[0]->rgb_buffer, 1, dst[0]->rgb_size, maritx_file);		
					fclose(maritx_file);
				}else if(!strcmp(argv[2], "-R1")){			
					separate_maritx(hbmp_src, dst, RGB1BIT);	
					
					maritx_file = fopen("maritx0.bin","wb+");
					fwrite(dst[0]->rgb_buffer, 1, dst[0]->rgb_size, maritx_file);		
					fclose(maritx_file);
				}
				break;
			}
			case 'y':
			{
				//RGB->YUV
				FILE *y_file, *u_file, *v_file;
				hbmp_src->yuv_buffer.type = YUV444;
				rgb_tranform_to_yuv(hbmp_src);
	
				y_file = fopen("y_file.bin","wb+");
				fwrite(hbmp_src->yuv_buffer.y_buffer.buffer, 1, hbmp_src->yuv_buffer.y_buffer.size, y_file);
				fclose(y_file);
		
				u_file = fopen("u_file.bin","wb+");
				fwrite(hbmp_src->yuv_buffer.u_buffer.buffer, 1, hbmp_src->yuv_buffer.u_buffer.size, u_file);
				fclose(u_file);
	
				v_file = fopen("v_file.bin","wb+");
				fwrite(hbmp_src->yuv_buffer.v_buffer.buffer, 1, hbmp_src->yuv_buffer.v_buffer.size, v_file);
				fclose(v_file);
				break;
			}
			default:
			{
				__wrn("parameter error!\n");
				break;
			}
		}
	}
	free(hbmp_src);
	return EPDK_OK;
}
