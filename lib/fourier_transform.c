#include "fourier_transform.h"

int FFT(COMPLEX_NUMBER *fft_src, COMPLEX_NUMBER *fft_dst, int iteration_times)
{
	long count;
	uint32_t i, j, k;
	uint32_t bfsize, p;
	double angle;

	COMPLEX_NUMBER *w, *x1, *x2, *x;
	count = 1 << iteration_times;
	w = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*count/2);
	x1 = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*count);	
	x2 = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*count);
	for(i=0;i<count/2;i++){
		angle = -i * PI * 2 / count;
		w[i].real = cos(angle);
		w[i].imaginary = sin(angle);
	}

	memcpy(x1, fft_src, sizeof(COMPLEX_NUMBER)*count);
	for(k=0;k<iteration_times;k++){
		for(j=0;j<(1<<k);j++){
			bfsize = 1 << (iteration_times-k);
			for(i=0;i<bfsize/2;i++){
				p = j * bfsize;
				x2[i+p].real = x1[i+p].real + x1[i+p+bfsize/2].real;
				x2[i+p].imaginary = x1[i+p].imaginary + x1[i+p+bfsize/2].imaginary;
				
				x2[i+p+bfsize/2].real = (x1[i+p].real - x1[i+p+bfsize/2].real) * w[i*(1<<k)].real;
				x2[i+p+bfsize/2].imaginary = (x1[i+p].imaginary - x1[i+p+bfsize/2].imaginary) * w[i*(1<<k)].imaginary;
			}
		}
		x = x1;
		x1 = x2;
		x2 = x;
	}
	for(j=0;j<count;j++){
		p = 0;
		for(i=0;i<iteration_times;i++){
			if(j&(1<<i)){
				p+=1<<(iteration_times-i-1);
			}
			fft_dst[j].real = x1[p].real;			
			fft_dst[j].imaginary = x1[p].imaginary;
		}
	}
	show_para(fft_dst[0].real);
	free(w);
	free(x1);
	free(x2);
	return 0;
}

#define DEFAULT_FILL_LUMA 255
int image_FFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
{
	double tmp;
	COMPLEX_NUMBER *tmp_fft_output;
	uint32_t i, j;
	uint32_t freq_width = 1;
	uint32_t freq_height = 1;
	uint32_t width_iteration = 0;
	uint32_t height_iteration = 0;
	COMPLEX_NUMBER *time_image;
	double max = 0;
	double min = 0xffffffff;
	fft_dst->fill_luma = DEFAULT_FILL_LUMA;
	fft_dst->spectrum = malloc(sizeof(HBMP_i_t));
	memcpy(fft_dst->spectrum, src, sizeof(HBMP_i_t));
	while(freq_width*2 <= src->width){
		freq_width *= 2;
		width_iteration++;
	}

	while(freq_height*2 <= src->height){
		freq_height *= 2;
		height_iteration++;
	}
	if(fft_dst->expand && (freq_width != src->width) && (freq_height != src->height)){
		freq_width *= 2;
		width_iteration++;
		freq_height *= 2;
		height_iteration;
	}

	time_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*freq_width*freq_height);
	fft_dst->freq_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*freq_width*freq_height);

	for(i=0;i<freq_height;i++){
		for(j=0;j<freq_width;j++){
			if(fft_dst->expand){
				if((j<src->height) && (i<src->width)){
					time_image[i*src->width+j].real = src->get_y_value(src, j, i);
					time_image[i*src->width+j].imaginary = 0;
				}else{
					time_image[i*src->width+j].real = fft_dst->fill_luma;
					time_image[i*src->width+j].imaginary = 0;
				}
			}else{
				time_image[i*src->width+j].real = src->get_y_value(src, j, i);
				time_image[i*src->width+j].imaginary = 0;
			}
		}
	}
	fft_dst->spectrum->height = freq_height;
	fft_dst->spectrum->width = freq_width;
	//yuv_buffer_init(fft_dst->spectrum);
	fft_dst->spectrum->rgb_size = fft_dst->spectrum->height * fft_dst->spectrum->width * 4;
	show_para(fft_dst->spectrum->height);
	
	show_para(fft_dst->spectrum->width);
	yuv_buffer_init(fft_dst->spectrum);
	fft_dst->spectrum->rgb_buffer = (uint32_t *)malloc(fft_dst->spectrum->rgb_size);
	tmp_fft_output = malloc(sizeof(COMPLEX_NUMBER)*freq_width*freq_height);

	for(i=0;i<freq_height;i++){
		FFT(time_image+freq_width*i, fft_dst->freq_image+freq_width*i, width_iteration);
	}

	for(i=0;i<freq_height;i++){
		for(j=0;j<freq_width;j++){
			time_image[j*freq_height+i].real = fft_dst->freq_image[i*freq_width+j].real;			
			time_image[j*freq_height+i].imaginary = fft_dst->freq_image[i*freq_width+j].imaginary;
		}
	}
	
	for(i=0;i<freq_width;i++){
		FFT(time_image+freq_height*i, fft_dst->freq_image+freq_height*i, height_iteration);
		show_para(fft_dst->freq_image[freq_width*i].real);
		show_para(freq_width*i);
			
	} 
	for(i=0;i<freq_height;i++){
		for(j=0;j<freq_width;j++){
			tmp_fft_output[i*freq_width+j].real = fft_dst->freq_image[j*freq_height+i].real;			
			tmp_fft_output[i*freq_width+j].imaginary = fft_dst->freq_image[j*freq_height+i].imaginary;
		}
	}
	
	for(i=0;i<freq_height;i++){
		for(j=0;j<freq_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*freq_height+i].real * fft_dst->freq_image[j*freq_height+i].real+
					   fft_dst->freq_image[j*freq_height+i].imaginary * fft_dst->freq_image[j*freq_height+i].imaginary)/100;
			tmp = log(1+tmp);
			max = MAX(max, tmp);
			min = MIN(min, tmp);
		}
	}
	for(i=0;i<freq_height;i++){
		for(j=0;j<freq_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*freq_height+i].real * fft_dst->freq_image[j*freq_height+i].real+
					   fft_dst->freq_image[j*freq_height+i].imaginary * fft_dst->freq_image[j*freq_height+i].imaginary)/100;

			tmp = log(1+tmp);
			tmp = (tmp - min)/(max - min) * 255;
			fft_dst->spectrum->yuv_buffer.y_buffer.buffer[(j<freq_width/2?j+freq_width/2:j-freq_width/2) + fft_dst->spectrum->width * (i<freq_height/2?i+freq_height/2:i-freq_height/2)] = (uint8_t)tmp;
			
		}
	}

	
}
