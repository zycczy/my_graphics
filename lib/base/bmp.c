/*author: charles cheng 2017-05-04*/

#include "bmp.h"

static int get_matrix_32bit(HBMP_i_t *hbmp, void *buf, uint8_t *bmp_buf)
{	
	uint32_t   row_size;
	uint32_t   real_row_size;
	uint32_t   height;
	uint32_t   i;
	uint8_t    *q;
	int32_t    pad_count;
	uint8_t    *pSrc;
	if( hbmp == NULL || buf == NULL || bmp_buf == NULL)
		return EPDK_FAIL;
		
	row_size = hbmp->row_size;
	real_row_size = hbmp->width * hbmp->byte_count;
	height = hbmp->height;
	pad_count = row_size - real_row_size;

	pSrc = bmp_buf+hbmp->matrix_off;
	__dbg("real_row_size = %d\n", real_row_size);
	q = (uint8_t *)buf + real_row_size * ( height - 1 );
	for( i = 0;  i < height;  i++ ){
    	if(bmp_buf){
        	memcpy(q, pSrc, real_row_size);
            pSrc += real_row_size;
		}
		pSrc += pad_count;
		q -= real_row_size;
	}
	return EPDK_OK;
}

static int get_matrix_1bit(HBMP_i_t *hbmp, void *buf, uint8_t *bmp_buf)
{
	uint32_t i;
	
	uint8_t  *q;
	uint8_t  *pSrc;	
	pSrc = bmp_buf + hbmp->matrix_off;	
	q = (uint8_t *)buf + hbmp->rgb_size - 1;
	for(i=0;i<hbmp->rgb_size;i++){
       	memcpy(q, pSrc, 1);
        pSrc += 1;
		q -= 1;
	}	
	return EPDK_OK;
}


/**************************************************************************************************/
/*                                         function list                                          */
/**************************************************************************************************/

void bmp_open(const char *bmp_file, HBMP_i_t *hbmp_hdl_buf)
{
	void *fp = NULL;
	bmp_file_head_t  file_head;
	bmp_info_head_t  info_head;
	uint8_t* pbuf;
	uint32_t file_len;
	uint32_t read_len;


	if(bmp_file == NULL || hbmp_hdl_buf == NULL){
		
		__wrn("Error in hbmp_hdl_buf or bmp_file\n");
		return;
	}

		
	fp = fopen(bmp_file, "rw+");
	if( fp == NULL ){
		__wrn("Error in open bmp file %s.\n", bmp_file);
        hbmp_hdl_buf = NULL;
		return;
	}

	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	__dbg("file_len=%d\n", file_len);
	pbuf = malloc(alignment_up(file_len,8));
 
	fseek(fp, 0, SEEK_SET);
	read_len = fread(pbuf, 1, file_len, fp);
	if(read_len != file_len){
        	__wrn("fread fail..., read_len = %d\n", read_len);
        	hbmp_hdl_buf = NULL;
        	return;
    	}


	/* get bmp file head */	
	memcpy(&file_head, pbuf, sizeof(file_head));
	if( file_head.bfType[0] != 'B' || file_head.bfType[1] != 'M' ){
        	hbmp_hdl_buf = NULL;
		return;
	}
	
	/* get bmp information head */	
    memcpy(&info_head, (char*)pbuf+sizeof(file_head), sizeof(info_head));
	__msg("width: %d\n",info_head.biWidth);
	__msg("height: %d\n",info_head.biHeight);

	hbmp_hdl_buf->width  = info_head.biWidth ;
	hbmp_hdl_buf->height = info_head.biHeight;
	hbmp_hdl_buf->bitcount = info_head.biBitCount;	
	hbmp_hdl_buf->matrix_off = file_head.bfOffBits;
	if(info_head.biBitCount == 32){//32bit map
		hbmp_hdl_buf->byte_count = hbmp_hdl_buf->bitcount >> 3;
		hbmp_hdl_buf->row_size = alignment_up(hbmp_hdl_buf->width*hbmp_hdl_buf->byte_count, 4); //( hbmp_hdl_buf->width * byte_count + 4 - 1 ) & (~( 4 - 1 ) );//
		hbmp_hdl_buf->rgb_size = hbmp_hdl_buf->row_size * hbmp_hdl_buf->height*sizeof(int);
		
		hbmp_hdl_buf->rgb_buffer = malloc(hbmp_hdl_buf->rgb_size);		
		__dbg("biBitCount = 32, rgb_size = %d\n", hbmp_hdl_buf->rgb_size);
		get_matrix_32bit(hbmp_hdl_buf, hbmp_hdl_buf->rgb_buffer, pbuf);
	}else if(info_head.biBitCount == 1){//1bit map,only black and white
		hbmp_hdl_buf->rgb_size = alignment_up(hbmp_hdl_buf->width * hbmp_hdl_buf->height, 8)/8;	
		__dbg("biBitCount = 1, rgb_size = %d\n", hbmp_hdl_buf->rgb_size);
		hbmp_hdl_buf->rgb_buffer = malloc(hbmp_hdl_buf->rgb_size);		
		get_matrix_1bit(hbmp_hdl_buf, hbmp_hdl_buf->rgb_buffer, pbuf);
	}

	free(pbuf);
	fclose(fp);
	__msg("bmp_open end!\n");


}


