/*author: charles cheng 2018-04-10*/
#include "canny.h"
/* The canny egde detection:
  *	1. Guass filtrer
  *	2. Sobel calculate the grad of x, y direction and the amplitude
  *	3. non-maxima suppression
  *	4. double threshold and connect the edge
  */

static void sobel_amplitude(HBMP_i_t *sobel_x, HBMP_i_t *sobel_y, HBMP_i_t *sobel_ampXY)
{
	int i, j;
	
	for(i=0; i<sobel_ampXY->width; i++){
		for(j=0;j<sobel_ampXY->height;j++){
			sobel_ampXY->yuv_buffer.y_buffer.buffer[j*sobel_ampXY->width + i] = CAL_MOUDLE(sobel_x->get_y_value(sobel_x, i, j), sobel_y->get_y_value(sobel_y, i, j));
		}
	}
}

static void grad_xy(HBMP_i_t *sobel_x, HBMP_i_t *sobel_y, double **point_drection)
{	
	int i, j;
	int k = 0;
	point_drection = malloc(sizeof(double)*(sobel_x->width-1)*(sobel_x->height-1));
	for(i=1; i<sobel_x->width-1; i++){
		for(j=1;j<sobel_x->height-1;j++){
			(*point_drection)[k] = atan(sobel_y->get_y_value(sobel_y, i, j)/
			(sobel_x->get_y_value(sobel_x, i, j)==0?0.00000000000000001:sobel_x->get_y_value(sobel_x, i, j)))*57.3 + 90;
		}
	}
}

static void local_max_value(HBMP_i_t *hbmp_src, double *point_drection)
{
	int i, j;
	int k = 0;
	int value[3][3] = {{0, 0}};
	for(i=1; i<hbmp_src->width-1; i++){
		for(j=1;j<hbmp_src->height-1;j++){
			value[0][0] = hbmp_src->get_y_value(hbmp_src, i-1, j-1);
			value[0][1] = hbmp_src->get_y_value(hbmp_src, i, j-1);
			value[0][2] = hbmp_src->get_y_value(hbmp_src, i+1, j-1);
			
			value[1][0] = hbmp_src->get_y_value(hbmp_src, i-1, j);
			value[1][1] = hbmp_src->get_y_value(hbmp_src, i, j);
			value[1][2] = hbmp_src->get_y_value(hbmp_src, i+1, j);

			value[2][0] = hbmp_src->get_y_value(hbmp_src, i-1, j+1);
			value[2][1] = hbmp_src->get_y_value(hbmp_src, i, j+1);
			value[2][2] = hbmp_src->get_y_value(hbmp_src, i+1, j+1);

			if (point_drection[k])
		}
	}
}

int canny(HBMP_i_t *hbmp_src)
{
	HBMP_i_t sobel_x, sobel_y, sobel_ampXY;
	FILE *sobel_amplitude_file;
	double *point_drection;
	spatial_filter(hbmp_src, TEMPLATE_SMOOTH_GAUSS);

	hbmp_copy(hbmp_src, &sobel_x);
	spatial_filter(&sobel_x, TEMPLATE_VSOBLE);
	hbmp_copy(hbmp_src, &sobel_y);
	spatial_filter(&sobel_y, TEMPLATE_HSOBLE);
	hbmp_copy(hbmp_src, &sobel_ampXY);
	grad_xy(&sobel_x, &sobel_y, &point_drection);
	sobel_amplitude(&sobel_x, &sobel_y, &sobel_ampXY);
	
	sobel_amplitude_file = fopen("sobel_amplitude_file.bin","wb+");
	fwrite(sobel_ampXY.yuv_buffer.y_buffer.buffer, 1, sobel_ampXY.yuv_buffer.y_buffer.size, sobel_amplitude_file);
	fclose(sobel_amplitude_file);
	return EPDK_OK;
}

