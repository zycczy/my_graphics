/*author: charles cheng 2017-10-10*/
#include "freq_filter_tools.h"

/*
	The function of ideal low pass filter:
	
			   1,    [(u-M/2)^2 + (v-N/2)^2]^(1/2) <= D
	H(u, v) = {
			   0,    [(u-M/2)^2 + (v-N/2)^2]^(1/2) > D

	D is the cut off frequency
*/
static void freq_ideal_LPF(FFT_STRUCT *fft_src, int cut_off_freq)
{
	int fft_width = fft_src->spectrum->width;	
	int fft_height = fft_src->spectrum->height;
	int i, j;
	if(fft_src->freq_filter == NULL){
		fft_src->freq_filter = (double *)malloc(fft_width*fft_height*sizeof(double));
	}
	
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			if(sqrt(pow((double)(i-fft_height/2), 2) + pow((double)(j-fft_width/2), 2)) > cut_off_freq){
				fft_src->freq_filter[FFT_SHIFT(j, i, fft_width, fft_height)] = (double)0;
			}else{
				fft_src->freq_filter[FFT_SHIFT(j, i, fft_width, fft_height)] = (double)1;
			}
		}
	}
}

/*
	The function of Guass low pass filter:
	
			  
	H(u, v) = e ^ -[(u-M/2)^2 + (v-N/2)^2]/2*(sig^2)
			  

	sig is the standard deviation of Guass function
*/

static void freq_Gauss_LPF(FFT_STRUCT *fft_src, int sigma)
{
	int fft_width = fft_src->spectrum->width;	
	int fft_height = fft_src->spectrum->height;
	int i, j;
	if(fft_src->freq_filter == NULL){
		fft_src->freq_filter = (double *)malloc(fft_width*fft_height*sizeof(double));
	}
	
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			fft_src->freq_filter[FFT_SHIFT(j, i, fft_width, fft_height)] = exp(-(pow((double)(i-fft_height/2), 2) + pow((double)(j-fft_width/2), 2))/2/pow(sigma, 2));
		}
	}
}

void freq_filter(FFT_STRUCT *fft_src, FREQ_FILTER_METHOD method, int arg)
{
	uint32_t i, j;
	image_FFT(fft_src);
	switch(method)
	{
		case FREQ_IDEAL_LFP:
		{
			freq_ideal_LPF(fft_src, arg);
			break;
		}
		
		case FREQ_GUASS_LFP:
		{
			freq_Gauss_LPF(fft_src, arg);
			break;
		}
	}
	
	if(method != NONE_FILTER){
		for(i=0;i<fft_src->spectrum->height*fft_src->spectrum->width;i++){
			fft_src->freq_image[i].real *= fft_src->freq_filter[i];	
			fft_src->freq_image[i].imagin *= fft_src->freq_filter[i];
			
		}
	}
	image_FFT_save(fft_src);
	
	image_IFFT(fft_src);
}

