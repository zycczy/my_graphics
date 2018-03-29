/*author: charles cheng 2017-08-16*/
#include "fourier_transform.h"

int is_base2(int size_n){
    int k = size_n;
    int z = 0;
    while (k/=2) {
        z++;
    }
    k = z;
    if(size_n != (1<<k))
        return -1;
    else
        return k;
}



void getWN(double n,double size_n,Complex * dst){
    double x = 2.0*PI*n/size_n;
    dst->imagin = -sin(x);
    dst->real = cos(x);
}

void FFT(Complex *src,Complex *dst,int size_n)
{
	int i;
	Complex *tmp_even = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex *tmp_odd = (Complex *)malloc(size_n/2*sizeof(Complex));
	
	Complex *tmp_even_fft = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex *tmp_odd_fft = (Complex *)malloc(size_n/2*sizeof(Complex));
	
	Complex *tmp_Wn = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex tmp_value;
	if(size_n > 2){
		for(i=0;i<size_n;i++){	
			if(i%2==0){
				copy_complex(&src[i], &tmp_even[i/2]);
			}else{
				copy_complex(&src[i], &tmp_odd[(i-1)/2]);
			}
		}
		
		FFT(tmp_even, tmp_even_fft, size_n/2);			
		FFT(tmp_odd, tmp_odd_fft, size_n/2);
		for(i=0;i<size_n/2;i++){
			getWN(i, size_n, &tmp_Wn[i]);
			
			//dst[i] = tmp_even[i]+tmp_Wn[i]*tmp_odd[i];			
			multy_complex(&tmp_Wn[i], &tmp_odd_fft[i], &tmp_value);
			add_complex(&tmp_even_fft[i], &tmp_value, &dst[i]);
			
			//dst[i+size_n/2] = tmp_even[i]-tmp_Wn[i]*tmp_odd[i];
			multy_complex(&tmp_Wn[i], &tmp_odd_fft[i], &tmp_value);
			sub_complex(&tmp_even_fft[i], &tmp_value, &dst[i+size_n/2]);
		}

	}else{		
		add_complex(&src[0], &src[1], &dst[0]);
		
		sub_complex(&src[0], &src[1], &dst[1]);
	}
}

void IFFT(Complex *src,Complex *dst,int size_n)
{
	int i;
	
	Complex *tmp_even = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex *tmp_odd = (Complex *)malloc(size_n/2*sizeof(Complex));
	
	Complex *tmp_even_fft = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex *tmp_odd_fft = (Complex *)malloc(size_n/2*sizeof(Complex));
	
	Complex *tmp_Wn = (Complex *)malloc(size_n/2*sizeof(Complex));
	Complex tmp_value;
	
    for(i=0;i<size_n;i++){
    	src[i].imagin=-src[i].imagin;
    }

	if(size_n > 2){
		for(i=0;i<size_n;i++){	
			
			if(i%2==0){
				copy_complex(&src[i], &tmp_even[i/2]);
			}else{
				copy_complex(&src[i], &tmp_odd[(i-1)/2]);
			}
		}
		
		FFT(tmp_even, tmp_even_fft, size_n/2);			
		FFT(tmp_odd, tmp_odd_fft, size_n/2);
		for(i=0;i<size_n/2;i++){
			getWN(i, size_n, &tmp_Wn[i]);
			
			//dst[i] = tmp_even[i]+tmp_Wn[i]*tmp_odd[i];			
			multy_complex(&tmp_Wn[i], &tmp_odd_fft[i], &tmp_value);
			add_complex(&tmp_even_fft[i], &tmp_value, &dst[i]);
			dst[i].imagin *= (1./size_n);			
			dst[i].real*= (1./size_n);
			//dst[i+size_n/2] = tmp_even[i]-tmp_Wn[i]*tmp_odd[i];
			multy_complex(&tmp_Wn[i], &tmp_odd_fft[i], &tmp_value);
			sub_complex(&tmp_even_fft[i], &tmp_value, &dst[i+size_n/2]);
			dst[i+size_n/2].imagin *= (1./size_n);			
			dst[i+size_n/2].real *= (1./size_n);
		}

	}else{		
		add_complex(&src[0], &src[1], &dst[0]);
		dst[0].imagin *= 0.5;
		dst[0].real *= 0.5;
		sub_complex(&src[0], &src[1], &dst[1]);		
		dst[1].imagin *= 0.5;
		dst[0].real *= 0.5;
	}
}




void column_vector(Complex * src,Complex * dst,int size_w,int size_h){
	int i;
	for(i=0;i<size_h;i++){
        copy_complex(&src[size_w*i], &dst[i]);
    }
    
}

void Icolumn_vector(Complex * src,Complex * dst,int size_w,int size_h){
	int i;
	for(i=0;i<size_h;i++)
        copy_complex(&src[i], &dst[size_w*i]);
    
}

int FFT2D(Complex *src,Complex *dst,int size_w,int size_h)
{
    int i;
    if(is_base2(size_w)==-1||is_base2(size_h)==-1){
        exit(0);
    }
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL){
        return -1;
    }
    for(i=0;i<size_h;i++){
        FFT(&src[size_w*i], &temp[size_w*i], size_w);
    }

    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
	
    Complex *Column_tmp=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL){
        return -1;
    }
    for(i=0;i<size_w;i++){
        column_vector(&temp[i], Column, size_w, size_h);
        FFT(Column, Column_tmp, size_h);
        Icolumn_vector(Column_tmp, &temp[i], size_w, size_h);
        
    }
    
    
    
    for(i=0;i<size_h*size_w;i++)
        copy_complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
	
    free(Column_tmp);
    return 0;
}

int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h)
{
 	int i;   
    if(is_base2(size_w) == -1 || is_base2(size_h) == -1){
        exit(0);
    }
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL){
        return -1;
    }
    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL){
        return -1;
    }
    Complex *Column_tmp=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL){
        return -1;
    }
    for(i=0;i<size_w;i++){
        column_vector(&src[i], Column, size_w, size_h);
        IFFT(Column, Column_tmp, size_h);
        Icolumn_vector(Column_tmp, &src[i], size_w, size_h);
        
    }
	
    for(i=0;i<size_w*size_h;i++)
        src[i].imagin=-src[i].imagin;
    for(i=0;i<size_h;i++){
        IFFT(&src[size_w*i], &temp[size_w*i], size_w);
    }
    
    
    for(i=0;i<size_h*size_w;i++)
        copy_complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void image_FFT_save(FFT_STRUCT *fft_dst)
{	
	int i, j;
	double tmp;
	
	double max = 0;
	double min = 0xffffffff;	
	uint32_t fft_height = fft_dst->spectrum->height;
	uint32_t fft_width = fft_dst->spectrum->width;
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*fft_height+i].real * fft_dst->freq_image[j*fft_height+i].real+
					   fft_dst->freq_image[j*fft_height+i].imagin * fft_dst->freq_image[j*fft_height+i].imagin)/100;
			tmp = (double)log(1+tmp);
			max = MAX(max, tmp);
			min = MIN(min, tmp);
		}
	}
	__dbg("max = %lf\n", max);	
	__dbg("min = %lf\n", min);
	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp = (double)sqrt(fft_dst->freq_image[j*fft_height+i].real * fft_dst->freq_image[j*fft_height+i].real+
					   fft_dst->freq_image[j*fft_height+i].imagin * fft_dst->freq_image[j*fft_height+i].imagin)/100;

			tmp = (double)log(1+tmp);
			tmp = (tmp - min)/(max - min) * 255;
			fft_dst->spectrum->yuv_buffer.y_buffer.buffer[FFT_SHIFT(j, i, fft_width, fft_height)] = (uint8_t)tmp;
		}
	}
	
}
#define DEFAULT_FILL_LUMA 255
int image_FFT(FFT_STRUCT *fft_dst)
{
	double tmp;
	double *fft_src;
	Complex *tmp_fft_output;
	uint32_t i, j;
	uint32_t fft_width = 1;
	uint32_t fft_height = 1;
	uint32_t width_iteration = 0;
	uint32_t height_iteration = 0;
	Complex *time_image;
	HBMP_i_t *src = fft_dst->src;
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

	time_image = (Complex *)malloc(sizeof(Complex)*fft_width*fft_height);
	fft_dst->freq_image = (Complex *)malloc(sizeof(Complex)*fft_width*fft_height);

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			if(fft_dst->expand){
				if((j<src->height) && (i<src->width)){
					time_image[i*fft_width+j].real = (double)src->get_y_value(src, j, i);
					time_image[i*fft_width+j].imagin = 0;
				}else{
					time_image[i*fft_width+j].real = (double)fft_dst->fill_luma;
					time_image[i*fft_width+j].imagin = 0;
				}
			}else{
				time_image[i*fft_width+j].real = (double)src->get_y_value(src, j, i);
				time_image[i*fft_width+j].imagin = 0;
			}
		}
	}
	fft_dst->spectrum->height = fft_height;
	fft_dst->spectrum->width = fft_width;
	fft_dst->spectrum->rgb_size = fft_dst->spectrum->height * fft_dst->spectrum->width * 4;
	yuv_buffer_init(fft_dst->spectrum);

	FFT2D(time_image, fft_dst->freq_image, fft_width, fft_height);

	return EPDK_OK;
}
int image_IFFT(FFT_STRUCT *fft_dst)
{
	double tmp;
	uint32_t i, j;
	uint32_t ifft_width = 1, ifft_height = 1;
	uint32_t width_iteration = 0, height_iteration = 0;
	Complex *time_image;
	Complex *freq_image;	
	HBMP_i_t *src = fft_dst->src;
	double max = 0, min = 0xffffffff;
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
	
	memcpy(freq_image, fft_dst->freq_image, sizeof(COMPLEX_NUMBER)*ifft_width*ifft_height);
	IFFT2D(freq_image, time_image, ifft_width, ifft_height);
	
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			tmp = (double)sqrt(time_image[i*ifft_width+j].real * time_image[i*ifft_width+j].real+
					   		   time_image[i*ifft_width+j].imagin * time_image[i*ifft_width+j].imagin);
			max = MAX(max, tmp);
			min = MIN(min, tmp);
		}
	}
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			tmp = (double)sqrt(time_image[i*ifft_width+j].real * time_image[i*ifft_width+j].real+
					   time_image[i*ifft_width+j].imagin * time_image[i*ifft_width+j].imagin);

			tmp = (tmp - min)/(max - min) * 255;
			src->yuv_buffer.y_buffer.buffer[j+src->width*i] = (uint8_t)tmp;
			
		}
	}
	return EPDK_OK;	
}
