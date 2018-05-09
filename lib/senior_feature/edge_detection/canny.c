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
		for(j=0; j<sobel_ampXY->height; j++){
			if(i==0 || j==0 || i==sobel_ampXY->width-1 || j==sobel_ampXY->height-1){
				sobel_ampXY->yuv_buffer.y_buffer.buffer[j*sobel_ampXY->width + i] = 0;
			}else{
				sobel_ampXY->yuv_buffer.y_buffer.buffer[j*sobel_ampXY->width + i] = CAL_MOUDLE(sobel_x->get_y_value(sobel_x, i, j), sobel_y->get_y_value(sobel_y, i, j));
			}
		}
	}
}

static void grad_xy(HBMP_i_t *sobel_x, HBMP_i_t *sobel_y, double **point_drection)
{	
	int i, j;
	int k = 0;
	*point_drection = malloc(sizeof(double)*(sobel_x->width-1)*(sobel_x->height-1));
	for(i=1; i<sobel_x->width-1; i++){
		for(j=1; j<sobel_x->height-1; j++){
			(*point_drection)[k] = atan(sobel_y->get_y_value(sobel_y, i, j)/
			(sobel_x->get_y_value(sobel_x, i, j)==0?0.00000000000000001:sobel_x->get_y_value(sobel_x, i, j)))*57.3 + 90;
		}
	}
}

static void local_max_value(HBMP_i_t *hbmp_src, double *point_drection)
{
	int i, j;
	int k = 0;
	for(i=1; i<hbmp_src->width-1; i++){
		for(j=1; j<hbmp_src->height-1; j++){
			int value00 = hbmp_src->get_y_value(hbmp_src, i-1, j-1);
			int value01 = hbmp_src->get_y_value(hbmp_src, i, j-1);
			int value02 = hbmp_src->get_y_value(hbmp_src, i+1, j-1);
			
			int value10 = hbmp_src->get_y_value(hbmp_src, i-1, j);
			int value11 = hbmp_src->get_y_value(hbmp_src, i, j);
			int value12 = hbmp_src->get_y_value(hbmp_src, i+1, j);

			int value20 = hbmp_src->get_y_value(hbmp_src, i-1, j+1);
			int value21 = hbmp_src->get_y_value(hbmp_src, i, j+1);
			int value22 = hbmp_src->get_y_value(hbmp_src, i+1, j+1);

			if (point_drection[k] > 0 && point_drection[k] < 45) {
				if (value11<=(value12+(value02-value12)*tan(point_drection[j*hbmp_src->width + i]))||
					value11<=(value10+(value20-value10)*tan(point_drection[j*hbmp_src->width + i]))) {
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;
					
				}
			} else if (point_drection[k] > 45 && point_drection[k] < 90) {
				if(value11<=(value01+(value02-value01)*tan(point_drection[j*hbmp_src->width + i]))||
					value11<=(value21+(value20-value21)*tan(point_drection[j*hbmp_src->width + i]))) {
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;
				}
			} else if (point_drection[k] > 90 && point_drection[k] < 135) {
				if(value11<=(value01+(value00-value01)*tan(180-point_drection[j*hbmp_src->width + i]))||
					value11<=(value21+(value22-value21)*tan(180-point_drection[j*hbmp_src->width + i]))) {
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;
				}
			} else if (point_drection[k] > 135 && point_drection[k] < 180) {
				if(value11<=(value10+(value00-value10)*tan(180-point_drection[j*hbmp_src->width + i]))||
					value11<=(value12+(value22-value11)*tan(180-point_drection[j*hbmp_src->width + i]))) {
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;
				}
			}
		}
	}
}

static void double_threshold(HBMP_i_t *hbmp_src, double low, double high)
{
	int i, j;
	for (i=1; i<hbmp_src->width-1; i++) {
		for (j=1; j<hbmp_src->height-1; j++) {
			if (hbmp_src->get_y_value(hbmp_src, i, j) > high)
				hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 255;
			if (hbmp_src->get_y_value(hbmp_src, i, j) < low)	
				hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;
		}
	}
}

static void double_threshold_link(HBMP_i_t *hbmp_src, double low, double high)
{
	int i, j;
	for (i=1; i<hbmp_src->width-1; i++) {
		for (j=1; j<hbmp_src->height-1; j++) {
			if (hbmp_src->get_y_value(hbmp_src, i,j)>low && hbmp_src->get_y_value(hbmp_src, i,j)<255) {  
				if (hbmp_src->get_y_value(hbmp_src, i-1,j-1)==255||hbmp_src->get_y_value(hbmp_src, i-1,j)==255||hbmp_src->get_y_value(hbmp_src, i-1,j+1)==255||  
					hbmp_src->get_y_value(hbmp_src, i,j-1)==255||hbmp_src->get_y_value(hbmp_src, i,j)==255||hbmp_src->get_y_value(hbmp_src, i,j+1)==255||  
					hbmp_src->get_y_value(hbmp_src, i+1,j-1)==255||hbmp_src->get_y_value(hbmp_src, i+1,j)==255||hbmp_src->get_y_value(hbmp_src, i+1,j+1)==255)
				{
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 255;	
					double_threshold_link(hbmp_src, low, high); 	
				} else {
					hbmp_src->yuv_buffer.y_buffer.buffer[j*hbmp_src->width + i] = 0;  
				}				  
			}		 
		}
	}
}


void canny(HBMP_i_t *hbmp_src, HBMP_i_t *sobel_ampXY)
{
	HBMP_i_t sobel_x, sobel_y;
	FILE *sobel_amplitude_file;
	double *point_drection;
	spatial_filter(hbmp_src, TEMPLATE_SMOOTH_GAUSS, 1);
	gamma_correct(hbmp_src, 0.7);

	hbmp_copy(hbmp_src, &sobel_x);
	spatial_filter(&sobel_x, TEMPLATE_VSOBLE, 0);
	hbmp_copy(hbmp_src, &sobel_y);
	spatial_filter(&sobel_y, TEMPLATE_HSOBLE, 0);
	hbmp_copy(hbmp_src, sobel_ampXY);
	grad_xy(&sobel_x, &sobel_y, &point_drection);
	sobel_amplitude(&sobel_x, &sobel_y, sobel_ampXY);
	local_max_value(sobel_ampXY, point_drection);
	double_threshold(sobel_ampXY, 20, 25);
	double_threshold_link(sobel_ampXY, 20, 25);
	sobel_amplitude_file = fopen("canny.bin","wb+");
	fwrite(sobel_ampXY->yuv_buffer.y_buffer.buffer, 1, sobel_ampXY->yuv_buffer.y_buffer.size, sobel_amplitude_file);
	fclose(sobel_amplitude_file);
	return ;
}

