#include "fourier_transform.h"

int FFT(COMPLEX_NUMBER *fft_time, COMPLEX_NUMBER *fft_freq, int iteration_times)
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

	memcpy(x1, fft_time, sizeof(COMPLEX_NUMBER)*count);
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
			fft_freq[j].real = x1[p].real;			
			fft_freq[j].imaginary = x1[p].imaginary;
		}
	}
	free(w);
	free(x1);
	free(x2);
	return 0;
}

void IFFT(COMPLEX_NUMBER *fft_time, COMPLEX_NUMBER *fft_freq, int iteration_times)
{
	long count;
	uint32_t i;
	COMPLEX_NUMBER *x;

	count = 1 << iteration_times;
	x = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*count);	

	memcpy(x, fft_freq, sizeof(COMPLEX_NUMBER)*count);
	for(i=0;i<count;i++){
		x[i].imaginary *= -1;
	}

	FFT(x, fft_time, iteration_times);

	for(i=0;i<count;i++){
		fft_time[i].real /= count;		
		fft_time[i].imaginary /= count;
	}
	free(x);
	return ;
}


#define DEFAULT_FILL_LUMA 255
int image_FFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
{
	double tmp;
	COMPLEX_NUMBER *tmp_fft_output;
	uint32_t i, j;
	uint32_t fft_width = 1;
	uint32_t fft_height = 1;
	uint32_t width_iteration = 0;
	uint32_t height_iteration = 0;
	COMPLEX_NUMBER *time_image;
	double max = 0;
	double min = 0xffffffff;
	fft_dst->fill_luma = DEFAULT_FILL_LUMA;
	fft_dst->spectrum = malloc(sizeof(HBMP_i_t));
	memcpy(fft_dst->spectrum, src, sizeof(HBMP_i_t));
	while(fft_width*2 <= src->width){
		fft_width *= 2;
		width_iteration++;
	}

	while(fft_height*2 <= src->height){
		fft_height *= 2;
		height_iteration++;
	}
	if(fft_dst->expand && (fft_width != src->width) && (fft_height != src->height)){
		fft_width *= 2;
		width_iteration++;
		fft_height *= 2;
		height_iteration;
	}

	time_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*fft_width*fft_height);
	fft_dst->freq_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*fft_width*fft_height);

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
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
	fft_dst->spectrum->height = fft_height;
	fft_dst->spectrum->width = fft_width;
	//yuv_buffer_init(fft_dst->spectrum);
	fft_dst->spectrum->rgb_size = fft_dst->spectrum->height * fft_dst->spectrum->width * 4;
	yuv_buffer_init(fft_dst->spectrum);
	fft_dst->spectrum->rgb_buffer = (uint32_t *)malloc(fft_dst->spectrum->rgb_size);
	tmp_fft_output = malloc(sizeof(COMPLEX_NUMBER)*fft_width*fft_height);

	for(i=0;i<fft_height;i++){
		FFT(time_image+fft_width*i, fft_dst->freq_image+fft_width*i, width_iteration);
	}

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			time_image[j*fft_height+i].real = fft_dst->freq_image[i*fft_width+j].real;			
			time_image[j*fft_height+i].imaginary = fft_dst->freq_image[i*fft_width+j].imaginary;
		}
	}
	
	for(i=0;i<fft_width;i++){
		FFT(time_image+fft_height*i, fft_dst->freq_image+fft_height*i, height_iteration);
			
	} 
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp_fft_output[i*fft_width+j].real = fft_dst->freq_image[j*fft_height+i].real;			
			tmp_fft_output[i*fft_width+j].imaginary = fft_dst->freq_image[j*fft_height+i].imaginary;
		}
	}
	
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*fft_height+i].real * fft_dst->freq_image[j*fft_height+i].real+
					   fft_dst->freq_image[j*fft_height+i].imaginary * fft_dst->freq_image[j*fft_height+i].imaginary)/100;
			tmp = log(1+tmp);
			max = MAX(max, tmp);
			min = MIN(min, tmp);
		}
	}
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*fft_height+i].real * fft_dst->freq_image[j*fft_height+i].real+
					   fft_dst->freq_image[j*fft_height+i].imaginary * fft_dst->freq_image[j*fft_height+i].imaginary)/100;

			tmp = log(1+tmp);
			tmp = (tmp - min)/(max - min) * 255;
			fft_dst->spectrum->yuv_buffer.y_buffer.buffer[(j<fft_width/2?j+fft_width/2:j-fft_width/2) + fft_dst->spectrum->width * (i<fft_height/2?i+fft_height/2:i-fft_height/2)] = (uint8_t)tmp;
			
		}
	}
	show_para(fft_dst->freq_image[512].real);
	return EPDK_OK;
}

int image_IFFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
{
	double tmp;
	uint32_t i, j;
	uint32_t ifft_width = 1, ifft_height = 1;
	uint32_t width_iteration = 0, height_iteration = 0;
	COMPLEX_NUMBER *time_image;
	COMPLEX_NUMBER *freq_image;
	while(ifft_width*2 <= src->width){
		ifft_width *= 2;
		width_iteration++;
	}

	while(ifft_height*2 <= src->width){
		ifft_height *= 2;
		height_iteration++;
	}
	show_para(width_iteration);	

	time_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*ifft_width*ifft_height);
	freq_image = (COMPLEX_NUMBER *)malloc(sizeof(COMPLEX_NUMBER)*ifft_width*ifft_height);
	
	memcpy(freq_image, fft_dst->freq_image, sizeof(COMPLEX_NUMBER)*ifft_width*ifft_width);
	
	show_para(freq_image[512].real);
	for(i=0;i<ifft_height;i++){
		IFFT(time_image+ifft_width*i, fft_dst->freq_image+ifft_width*i, width_iteration);
	}

	for(i=0;i<ifft_height;i++){
		for(j=0;j<ifft_width;j++){
			freq_image[i+ifft_height*j].real = time_image[j+i*ifft_width].real;			
			freq_image[i+ifft_height*j].imaginary= time_image[j+i*ifft_width].imaginary;
		}
	}

	for(i=0;i<ifft_width;i++){
		IFFT(time_image+ifft_height*i, fft_dst->freq_image+ifft_height*i, height_iteration);
	}

	double max = 0, min = 0xffffffff;

	
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			tmp = (double)sqrt(time_image[j*ifft_height+i].real * time_image[j*ifft_height+i].real+
					   time_image[j*ifft_height+i].imaginary * time_image[j*ifft_height+i].imaginary);
			max = MAX(max, tmp);
			min = MIN(min, tmp);
		}
	}
	printf("max = %lf", max);	
	printf("min = %lf", min);
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			tmp = (double)sqrt(time_image[j*ifft_height+i].real * time_image[j*ifft_height+i].real+
					   time_image[j*ifft_height+i].imaginary * time_image[j*ifft_height+i].imaginary)*100;

			tmp = (tmp - min)/(max - min) * 255;
			printf("min = %lf\n", tmp);
			src->yuv_buffer.y_buffer.buffer[j+src->width*i] = (uint8_t)tmp;
			
		}
	}
	return EPDK_OK;
}

