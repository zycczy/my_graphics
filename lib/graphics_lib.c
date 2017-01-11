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



uint32_t separate_maritx(HBMP_i_t* hbmp, HBMP_i_t **dst, TYPE_OF_MARITX type)
{
	uint32_t width_copies = hbmp->width / dst[0]->width;
	uint32_t height_copies = hbmp->height / dst[0]->height;
	uint32_t copies_count = width_copies * height_copies;
	MARITX_HBMP* maritx;
	uint32_t i,j,t,m;
	if(hbmp->height%dst[0]->height != 0 && hbmp->width%dst[0]->width != 0){
		__wrn("cannot separate the picture average!\n");
		return EPDK_FAIL;
	}
	__dbg("copies_count = %d\n", copies_count);
	//specify the coordinate
	t = 0;
	maritx = (MARITX_HBMP *)malloc(sizeof(MARITX_HBMP)*copies_count);
	while(t<copies_count){
		for(m=0;m<height_copies;m++){
			for(j=0;j<width_copies;j++){
				maritx[t].hbmp = dst[t];
				maritx[t].height_coordinate = m;
				maritx[t].width_coordinate = j;
#if 1
				for(i=0;i<maritx[t].hbmp->height;i++){
					if(type == RGB32BIT){
						memcpy((uint8_t *)(maritx[t].hbmp->rgb_buffer+(maritx[t].hbmp->width*i)),\
						   	   (uint8_t *)(hbmp->rgb_buffer+((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width),\
						        maritx[t].hbmp->width*4);
					}else if(type == YUV){
						memcpy((uint8_t *)(maritx[t].hbmp->yuv_buffer.y_buffer.buffer+(maritx[t].hbmp->width*i)),\
						   	   (uint8_t *)(hbmp->yuv_buffer.y_buffer.buffer+((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width),\
						        maritx[t].hbmp->width);						
						memcpy((uint8_t *)(maritx[t].hbmp->yuv_buffer.u_buffer.buffer+(maritx[t].hbmp->width*i)),\
						   	   (uint8_t *)(hbmp->yuv_buffer.u_buffer.buffer+((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width),\
						        maritx[t].hbmp->width);						
						memcpy((uint8_t *)(maritx[t].hbmp->yuv_buffer.v_buffer.buffer+(maritx[t].hbmp->width*i)),\
						   	   (uint8_t *)(hbmp->yuv_buffer.v_buffer.buffer+((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width),\
						        maritx[t].hbmp->width);
					}else if(type == RGB1BIT){
						//add
					}
					//__dbg("destnate buffer addr: %d\n", (maritx[t].hbmp->width*i));
					//__dbg("source buffer addr: %d\n", ((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width);
					
				}
#endif
				t++;
			}
		}
	}
	
#if 0
	for(t=0;t<copies_count;t++){
		for(i=0;i<maritx[t].hbmp->height;i++){
			memcpy((uint8_t *)(maritx[t].hbmp->rgb_buffer+(maritx[t].hbmp->width*i)),\
			       (uint8_t *)(hbmp->rgb_buffer+((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width),\
			       maritx[t].hbmp->width*4);
			//__dbg("destnate buffer addr: %d\n", (maritx[t].hbmp->width*i));
			//__dbg("source buffer addr: %d\n", ((maritx[t].height_coordinate*maritx[t].hbmp->height+i)*hbmp->width)+maritx[t].width_coordinate*maritx[t].hbmp->width);
			
		}
	}

#endif
	return EPDK_OK;
}
void yuv_buffer_init(HBMP_i_t* hbmp)
{
	hbmp->yuv_buffer.y_buffer.size = hbmp->width*hbmp->height;	
	hbmp->yuv_buffer.y_buffer.buffer = malloc(hbmp->yuv_buffer.y_buffer.size);
	switch(hbmp->yuv_buffer.type)
	{
		case YUV420:
			hbmp->yuv_buffer.u_buffer.size = hbmp->yuv_buffer.y_buffer.size >> 1;			
			hbmp->yuv_buffer.u_buffer.buffer = malloc(hbmp->yuv_buffer.u_buffer.size);
			hbmp->yuv_buffer.v_buffer.size = 0;
			break;
		case YUV422:
			hbmp->yuv_buffer.u_buffer.size = hbmp->yuv_buffer.y_buffer.size >> 1;
			hbmp->yuv_buffer.u_buffer.buffer = malloc(hbmp->yuv_buffer.u_buffer.size);
			hbmp->yuv_buffer.v_buffer.size = hbmp->yuv_buffer.u_buffer.size;			
			hbmp->yuv_buffer.v_buffer.buffer = malloc(hbmp->yuv_buffer.v_buffer.size);
			break;
		default:	
		case YUV444:
			hbmp->yuv_buffer.u_buffer.size = hbmp->yuv_buffer.y_buffer.size;
			hbmp->yuv_buffer.u_buffer.buffer = malloc(hbmp->yuv_buffer.u_buffer.size);
			hbmp->yuv_buffer.v_buffer.size = hbmp->yuv_buffer.y_buffer.size;			
			hbmp->yuv_buffer.v_buffer.buffer = malloc(hbmp->yuv_buffer.v_buffer.size);
			break;
	}
	return ;
}


/*
do not use this function, the function is only used for reference
There is something different between YUV and YCrBr(we call it Y'Crbr as below)
Y = Y' + 16   = 0.257R + 0.504G + 0.098B + 16
U = Cr + 128 = 0.148R - 0.291G + 0.439B + 128
V = Br + 128 = 0.439R - 0.368G - 0.071B + 128

B = 1.164(Y - 16) + 2.018(U - 128)
G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
R = 1.164(Y - 16) + 1.596(V - 128)
*/
static inline void table_init()
{
	uint32_t i;
	uint32_t j;
	for(i = 0; i < SIZE; i++){
		Y_R[i] = (i * 1052) >> 12; //Y table
		Y_G[i] = (i * 2064) >> 12;
		Y_B[i] = (i * 401) >> 12;
		U_R[i] = (i * 606) >> 12; //U table
		U_G[i] = (i * 1192) >> 12;
		U_B[i] = (i * 1798) >> 12;
		V_R[i] = (i * 1798) >> 12; //V table
		V_G[i] = (i * 1507) >> 12;
		V_B[i] = (i * 291) >> 12;
	}
	printf("\n");	
	printf("Y_R\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", Y_R[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("Y_G\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", Y_G[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");	
	printf("Y_B\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", Y_B[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("U_R\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", U_R[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("U_G\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", U_G[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("U_B\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", U_B[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("V_R\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", V_R[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");	
	printf("V_G\n");
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", V_G[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");
	printf("V_B\n");	
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("0x%x, ", V_B[i*16+j]);
		}
		printf("\\\n");
	}
	printf("\n");

}

int32_t rgb_tranform_to_yuv(HBMP_i_t* hbmp)
{
	int i;
	yuv_buffer_init(hbmp);
	//table_init();
	__dbg("yuv buffer init finish!\n");
	
	if(hbmp->yuv_buffer.type != YUV444){
		__wrn("Cannot support this type!\n");
		return EPDK_FAIL;
	}
	
	for(i = 0; i < hbmp->yuv_buffer.y_buffer.size; i++){
		hbmp->yuv_buffer.y_buffer.buffer[i] = Y_R[ARGB_PARSE_R(hbmp->rgb_buffer[i])] + Y_G[ARGB_PARSE_G(hbmp->rgb_buffer[i])] + Y_B[ARGB_PARSE_B(hbmp->rgb_buffer[i])] + 16; //Y
		hbmp->yuv_buffer.u_buffer.buffer[i] = U_B[ARGB_PARSE_B(hbmp->rgb_buffer[i])] - U_R[ARGB_PARSE_R(hbmp->rgb_buffer[i])] - U_G[ARGB_PARSE_G(hbmp->rgb_buffer[i])] + 128; //U
		hbmp->yuv_buffer.v_buffer.buffer[i] = V_R[ARGB_PARSE_R(hbmp->rgb_buffer[i])] - V_G[ARGB_PARSE_G(hbmp->rgb_buffer[i])] - V_B[ARGB_PARSE_B(hbmp->rgb_buffer[i])] + 128; //V
	}
	return EPDK_OK;
}


