/*author: charles cheng 2017-10-10*/
#include "freq_filter_tools.h"
void freq_filter(FFT_STRUCT *fft_src)
{
	uint32_t i, j;
	image_FFT(&fft_src);
	for(i=0;i<fft_src->spectrum->height;i++){
		for(j=0;j<fft_src->spectrum->width;j++){
			fft_src->freq_image[i*fft_src->spectrum->width+j].real *= fft_src->freq_filter[i*fft_src->spectrum->width+j];	
			fft_src->freq_image[i*fft_src->spectrum->width+j].imagin *= fft_src->freq_filter[i*fft_src->spectrum->width+j];
		}
	}
	image_IFFT(&fft_src);
}


void freq_ideal_LPF(FFT_STRUCT *fft_src)
{
	
}

