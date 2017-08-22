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
			p = j * bfsize;
			for(i=0;i<bfsize/2;i++){
				p = j * bfsize;
				x2[i+p] = x1[i+p] + x1[i+p+bfsize/2];
				x2[i+p+bfsize] = (x1[i+p] - x1[i+p+bfsize/2]) * w[i*(1<<k)];
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
	free(w);
	free(x1);
	free(x2);
	return 0;
}

int image_FFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
{
	double tmp;
	uint32_t i, j;
	uint32_t freq_width = 1;
	uint32_t freq_height = 1;
	uint32_t width_iteration = 0;
	uint32_t height_iteration = 0;
	COMPLEX_NUMBER *fft_src;
	while(freq_width*2 <= src->width){
		freq_width *= 2;
		width_iteration++;
	}

	while(freq_width*2 <= src->width){
		freq_height *= 2;
		height_iteration++;
	}
	if(fft_dst->expand && (freq_width != src->width) && (freq_height != src->height)){
		freq_width *= 2;
		width_iteration++;
		freq_height *= 2;
		height_iteration;
	}

	fft_src = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*freq_width*freq_height);
	fft_dst = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*freq_width*freq_height);

	for(i=0;i<)
}
