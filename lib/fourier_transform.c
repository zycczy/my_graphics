#include "fourier_transform.h"

int isBase2(int size_n){
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        return -1;
    else
        return k;
}

void Add_Complex(Complex * src1,Complex *src2,Complex *dst){
    dst->imagin=src1->imagin+src2->imagin;
    dst->real=src1->real+src2->real;
}
void Sub_Complex(Complex * src1,Complex *src2,Complex *dst){
    dst->imagin=src1->imagin-src2->imagin;
    dst->real=src1->real-src2->real;
}
void Multy_Complex(Complex * src1,Complex *src2,Complex *dst){
    double r1=0.0,r2=0.0;
    double i1=0.0,i2=0.0;
    r1=src1->real;
    r2=src2->real;
    i1=src1->imagin;
    i2=src2->imagin;
    dst->imagin=r1*i2+r2*i1;
    dst->real=r1*r2-i1*i2;
}
void Copy_Complex(Complex * src,Complex *dst){
    dst->real=src->real;
    dst->imagin=src->imagin;
}
void Show_Complex(Complex * src,int size_n){
    if(size_n==1){
        if(src->imagin>=0.0)
        printf("%lf+%lfj  ",src->real,src->imagin);
        else
        printf("%lf%lfj  ",src->real,src->imagin);
    
    }
    else if(size_n>1){
        for(int i=0;i<size_n;i++)
            if(src[i].imagin>=0.0){
                printf("%lf+%lfj  ",src[i].real,src[i].imagin);
            }
            else
                printf("%lf%lfj  ",src[i].real,src[i].imagin);
    
    
    
    }


}

void getWN(double n,double size_n,Complex * dst){
    double x=2.0*M_PI*n/size_n;
    dst->imagin=-sin(x);
    dst->real=cos(x);
}

void setInput(double * data,int  n){
    printf("Setinput signal:\n");
    srand((int)time(0));
    for(int i=0;i<n;i++){
        data[i]=rand()%VALUE_MAX;
        
    }
    
}

void DFT(double * src,Complex * dst,int size){

    for(int m=0;m<size;m++){
        double real=0.0;
        double imagin=0.0;
        for(int n=0;n<size;n++){
            double x=M_PI*2*m*n;
            real+=src[n]*cos(x/size);
            imagin+=src[n]*(-sin(x/size));
            
        }
        dst[m].imagin=imagin;
        dst[m].real=real;
        
    }
}

void IDFT(Complex *src,Complex *dst,int size){
    for(int m=0;m<size;m++){
        double real=0.0;
        double imagin=0.0;
        for(int n=0;n<size;n++){
            double x=M_PI*2*m*n/size;
            real+=src[n].real*cos(x)-src[n].imagin*sin(x);
            imagin+=src[n].real*sin(x)+src[n].imagin*cos(x);
            
        }
        real/=SIZE;
        imagin/=SIZE;
        if(dst!=NULL){
            dst[m].real=real;
            dst[m].imagin=imagin;
        }
    }
    
    
}

int FFTReal_remap(double * src,int size_n){
    
    if(size_n==1)
        return 0;
    double * temp=(double *)malloc(sizeof(double)*size_n);
    for(int i=0;i<size_n;i++)
        if(i%2==0)
            temp[i/2]=src[i];
        else
            temp[(size_n+i)/2]=src[i];
    for(int i=0;i<size_n;i++)
        src[i]=temp[i];
    free(temp);
    FFTReal_remap(src, size_n/2);
    FFTReal_remap(src+size_n/2, size_n/2);
    return 1;
    
    
}
int FFTComplex_remap(Complex * src,int size_n){
    
    if(size_n==1)
        return 0;
    Complex * temp=(Complex *)malloc(sizeof(Complex)*size_n);
    for(int i=0;i<size_n;i++)
        if(i%2==0)
            Copy_Complex(&src[i],&(temp[i/2]));
        else
            Copy_Complex(&(src[i]),&(temp[(size_n+i)/2]));
    for(int i=0;i<size_n;i++)
        Copy_Complex(&(temp[i]),&(src[i]));
    free(temp);
    FFTComplex_remap(src, size_n/2);
    FFTComplex_remap(src+size_n/2, size_n/2);
    return 1;
    
    
}

void FFT(Complex * src,Complex * dst,int size_n){
    
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
    if(src_com==NULL)
        exit(0);
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src[i], &src_com[i]);
    }
    FFTComplex_remap(src_com, size_n);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src_com[j], &wn,&src_com[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imagin=src_com[neighbour].imagin;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
        
    }
    
    
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src_com[i], &dst[i]);
    }
    free(src_com);
    

}
void RealFFT(double * src,Complex * dst,int size_n){
    

    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
    if(src_com==NULL)
        exit(0);
    for(int i=0;i<size_n;i++){
        src_com[i].real=src[i];
        src_com[i].imagin=0;
    }
    FFTComplex_remap(src_com, size_n);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src_com[j], &wn,&src_com[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imagin=src_com[neighbour].imagin;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
        
    }
    
    
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src_com[i], &dst[i]);
    }
    free(src_com);
    
}

void IFFT(Complex * src,Complex * dst,int size_n){
    for(int i=0;i<size_n;i++)
        src[i].imagin=-src[i].imagin;
    FFTComplex_remap(src, size_n);
    int z,k;
    if((z=isBase2(size_n))!=-1)
        k=isBase2(size_n);
    else
        exit(0);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src[j], &wn,&src[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                Copy_Complex(&src[neighbour], &temp);
                Add_Complex(&temp, &src[j], &src[neighbour]);
                Sub_Complex(&temp, &src[j], &src[j]);
            }
            else
                z=0;
        }
        
    }
    for(int i=0;i<size_n;i++){
            dst[i].imagin=(1./size_n)*src[i].imagin;
            dst[i].real=(1./size_n)*src[i].real;
        }
    
    
    
    
}

int DFT2D(double *src,Complex *dst,int size_w,int size_h){
    for(int u=0;u<size_w;u++){
        for(int v=0;v<size_h;v++){
            double real=0.0;
            double imagin=0.0;
            for(int i=0;i<size_w;i++){
                for(int j=0;j<size_h;j++){
                    double I=src[i*size_w+j];
                    double x=M_PI*2*((double)i*u/(double)size_w+(double)j*v/(double)size_h);
                    real+=cos(x)*I;
                    imagin+=-sin(x)*I;
                    
                }
            }
            dst[u*size_w+v].real=real;
            dst[u*size_w+v].imagin=imagin;

        }

    }
    return 0;
}

int IDFT2D(Complex *src,Complex *dst,int size_w,int size_h){
    for(int i=0;i<size_w;i++){
        for(int j=0;j<size_h;j++){
            double real=0.0;
            double imagin=0.0;
            for(int u=0;u<size_w;u++){
                for(int v=0;v<size_h;v++){
                    double R=src[u*size_w+v].real;
                    double I=src[u*size_w+v].imagin;
                    double x=M_PI*2*((double)i*u/(double)size_w+(double)j*v/(double)size_h);
                    real+=R*cos(x)-I*sin(x);
                    imagin+=I*cos(x)+R*sin(x);
                    
                }
            }
            dst[i*size_w+j].real=(1./(size_w*size_h))*real;
            dst[i*size_w+j].imagin=(1./(size_w*size_h))*imagin;
            
        }
    }
    return 0;
}

void ColumnVector(Complex * src,Complex * dst,int size_w,int size_h){
    for(int i=0;i<size_h;i++)
        Copy_Complex(&src[size_w*i], &dst[i]);
    
}

void IColumnVector(Complex * src,Complex * dst,int size_w,int size_h){
    for(int i=0;i<size_h;i++)
        Copy_Complex(&src[i],&dst[size_w*i]);
    
}

int FFT2D(double *src,Complex *dst,int size_w,int size_h){
    if(isBase2(size_w)==-1||isBase2(size_h)==-1)
        exit(0);
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL)
        return -1;
    for(int i=0;i<size_h;i++){
        RealFFT(&src[size_w*i], &temp[size_w*i], size_w);
    }

    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL)
        return -1;
    for(int i=0;i<size_w;i++){
        ColumnVector(&temp[i], Column, size_w, size_h);
        FFT(Column, Column, size_h);
        IColumnVector(Column, &temp[i], size_w, size_h);
        
    }
    
    
    
    for(int i=0;i<size_h*size_w;i++)
        Copy_Complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
    return 0;
}

int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h){
    
    if(isBase2(size_w)==-1||isBase2(size_h)==-1)
        exit(0);
    
    Complex *temp=(Complex *)malloc(sizeof(Complex)*size_h*size_w);
    if(temp==NULL)
        return -1;
    Complex *Column=(Complex *)malloc(sizeof(Complex)*size_h);
    if(Column==NULL)
        return -1;
    
    for(int i=0;i<size_w;i++){
        ColumnVector(&src[i], Column, size_w, size_h);
        IFFT(Column, Column, size_h);
        IColumnVector(Column, &src[i], size_w, size_h);
        
    }
    for(int i=0;i<size_w*size_h;i++)
        src[i].imagin=-src[i].imagin;
    for(int i=0;i<size_h;i++){
        IFFT(&src[size_w*i], &temp[size_w*i], size_w);
        
    }
    
    
    for(int i=0;i<size_h*size_w;i++)
        Copy_Complex(&temp[i], &dst[i]);
    free(temp);
    free(Column);
    return 0;
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
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
		angle = (-1 * (double)i * PI * (double)2 / (double)count);
		
		w[i].real = (double)cos(angle);
		w[i].imagin = (double)sin(angle);
	}
	memcpy(x1, fft_time, sizeof(COMPLEX_NUMBER)*count);
	for(k=0;k<iteration_times;k++){
		for(j=0;j<1<<k;j++){
			bfsize = 1 << (iteration_times-k);
			for(i=0;i<bfsize/2;i++){
				p = j * bfsize;
				x2[i+p].real = x1[i+p].real + x1[i+p+bfsize/2].real;
				x2[i+p].imagin = x1[i+p].imagin + x1[i+p+bfsize/2].imagin;
				
				x2[i+p+bfsize/2].real = (x1[i+p].real - x1[i+p+bfsize/2].real) * w[i*(1<<k)].real;
				x2[i+p+bfsize/2].imagin = (x1[i+p].imagin - x1[i+p+bfsize/2].imagin) * w[i*(1<<k)].imagin;
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
		}
		
		fft_freq[j].real = x1[p].real;			
		fft_freq[j].imagin = x1[p].imagin;
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
		x[i].imagin *= (double)-1;
	}

	FFT(x, fft_time, iteration_times);

	for(i=0;i<count;i++){
		fft_time[i].real = fft_time[i].real/(double)count;		
		fft_time[i].imagin = fft_time[i].imagin/(double)count;
	}
	free(x);
	return ;
}

#endif

#define DEFAULT_FILL_LUMA 255
int image_FFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
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
	tmp_fft_output = malloc(sizeof(Complex)*fft_width*fft_height);
#if 0
	for(i=0;i<fft_height;i++){
		FFT(time_image+fft_width*i, fft_dst->freq_image+fft_width*i, width_iteration);
	}
	__dbg("time_image[0].real = %lf\n", time_image[0].real);	

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			time_image[j*fft_height+i].real = fft_dst->freq_image[i*fft_width+j].real;			
			time_image[j*fft_height+i].imagin = fft_dst->freq_image[i*fft_width+j].imagin;
		}
	}
	
	for(i=0;i<fft_width;i++){
		FFT(time_image+fft_height*i, fft_dst->freq_image+fft_height*i, height_iteration);
			
	} 
	__dbg("fft_dst->freq_image[0].real = %lf\n", fft_dst->freq_image[0].real);	

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			tmp_fft_output[i*fft_width+j].real = fft_dst->freq_image[j*fft_height+i].real;			
			tmp_fft_output[i*fft_width+j].imagin = fft_dst->freq_image[j*fft_height+i].imagin;
		}
	}
#endif
	fft_src = (double *)malloc(sizeof(double)*fft_height*fft_width);

	for(i=0;i<fft_height;i++){
		for(j=0;j<fft_width;j++){
			fft_src[i*fft_width+j] =  time_image[i*fft_width+j].real;
		}
	}


	FFT2D(fft_src, fft_dst->freq_image,fft_width, fft_height);
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
			fft_dst->spectrum->yuv_buffer.y_buffer.buffer[(j<fft_width/2?j+fft_width/2:j-fft_width/2) + fft_dst->spectrum->width * (i<fft_height/2?i+fft_height/2:i-fft_height/2)] = (uint8_t)tmp;
		}
	}
	
	return EPDK_OK;
}
int image_IFFT(HBMP_i_t *src, FFT_STRUCT *fft_dst)
{
#if 1

	double tmp;
	uint32_t i, j;
	uint32_t ifft_width = 1, ifft_height = 1;
	uint32_t width_iteration = 0, height_iteration = 0;
	Complex *time_image;
	Complex *freq_image;
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
	double max = 0, min = 0xffffffff;
	IFFT2D(freq_image,time_image, ifft_width, ifft_height);
	
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
	
#endif
}
