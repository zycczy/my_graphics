/*author: charles cheng 2018-05-01*/
#include "hough.h"

void hough(HBMP_i_t *hbmp_src)
{
	int i, j;
	int q, p;
	//double p;
	int q_index, p_index;
	int *hough_table;

	hough_table = malloc(361*(hbmp_src->width+hbmp_src->height)*sizeof(int));
	for(i=2; i<hbmp_src->width-2; i++){
		for(j=2; j<hbmp_src->height-2; j++){
			if(hbmp_src->get_y_value(hbmp_src, i, j) == 255){
				for(q = 0; q < 360; q++){
					p = i*cos(q)+j*sin(q);
					p += (hbmp_src->width+hbmp_src->height);
					hough_table[q*(hbmp_src->width+hbmp_src->height)+p] += 1;
				}
			}
		}
	}

	int max, max_index;
	max = hough_table[0];
	for(i=0; i<(361*(hbmp_src->width+hbmp_src->height)); i++){
		if(max < hough_table[i]){
			max = hough_table[i];
			max_index = i;
		}
	}
	show_para(max);
	show_para(max_index);
}
