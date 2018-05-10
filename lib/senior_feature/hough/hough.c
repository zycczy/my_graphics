/*author: charles cheng 2018-05-01*/
#include "hough.h"



void hough(HBMP_i_t *hbmp_src)
{
	int i, j, t;
	int q, p;
	//double p;
	FILE *hough_file;
	int q_index, p_index;
	HOUGH *hough_table;

	hough_table = malloc(361*(hbmp_src->width+hbmp_src->height)*sizeof(HOUGH));
	for(i=0;i<361*(hbmp_src->width+hbmp_src->height);i++){
		hough_table[i].x = malloc((hbmp_src->width+hbmp_src->height)*sizeof(int));	
		hough_table[i].y = malloc((hbmp_src->width+hbmp_src->height)*sizeof(int));
	}
	
	for(i=2; i<hbmp_src->width-2; i++){
		for(j=2; j<hbmp_src->height-2; j++){
			if(hbmp_src->get_y_value(hbmp_src, i, j) == 255){
				for(q = 0; q < 360; q++){
					p = i*cos(q)+j*sin(q);
					p += (hbmp_src->width+hbmp_src->height);
					hough_table[q*(hbmp_src->width+hbmp_src->height)+p].count += 1;
					hough_table[q*(hbmp_src->width+hbmp_src->height)+p].x[hough_table[q*(hbmp_src->width+hbmp_src->height)+p].count] = i;
					hough_table[q*(hbmp_src->width+hbmp_src->height)+p].y[hough_table[q*(hbmp_src->width+hbmp_src->height)+p].count] = j;
				}
			}
		}
	}

	int max, max_index;
	max = hough_table[0].count;
	for(i=0; i<(361*(hbmp_src->width+hbmp_src->height)); i++){
		if(max < hough_table[i].count){
			max = hough_table[i].count;
			max_index = i;
		}
	}
	show_para(max);
	show_para(max_index);

	for(i=0; i<hbmp_src->width; i++){
		for(j=0; j<hbmp_src->height; j++){
			hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width+i] = 0;
		}
	}
	
	for(t=0;t<max;t++){
		hbmp_src->yuv_buffer.y_buffer.buffer[hough_table[max_index].y[t]*hbmp_src->width+hough_table[max_index].x[t]] = 255;
	}
	hough_file = fopen("hough.bin","wb+");
	fwrite(hbmp_src->yuv_buffer.y_buffer.buffer, 1, hbmp_src->yuv_buffer.y_buffer.size, hough_file);
	fclose(hough_file);
}
