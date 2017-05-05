//By MoreWindows (http://blog.csdn.net/MoreWindows)  
#include <opencv2/opencv.hpp>  
#include <opencv2/legacy/compat.hpp>  
using namespace std;  
void FillWhite(IplImage *pImage)  
{  
    cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
}  
// ´´½¨»ҶÈ¼ÏµÄ±·½ͼ  
CvHistogram* CreateGrayImageHist(IplImage **ppImage)  
{  
    int nHistSize = 256;  
    float fRange[] = {0, 255};  //»Ҷȼ¶µķ¶Χ    
    float *pfRanges[] = {fRange};    
    CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);  
    cvCalcHist(ppImage, pcvHistogram);  
    return pcvHistogram;  
}  
// ¸ù·½ͼ´´½¨ֱ·½ͼͼÏ  
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)  
{  
    IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);  
    FillWhite(pHistImage);  
  
    //ͳ¼Æ±·½ͼÖµÄîֱ·½¿é
    float fMaxHistValue = 0;  
    cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);  
  
    //·ֱð¿¸ö½¿éֵ»æµ½ͼÖ  
    int i;  
    for(i = 0; i < nImageWidth; i++)  
    {  
        float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //ÏËΪiµÄ±·½¿éС  
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //Ҫ»æµĸ߶È 
        cvRectangle(pHistImage,  
            cvPoint(i * nScale, nImageHeight - 1),  
            cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),  
            cvScalar(i, 0, 0, 0),   
            CV_FILLED  
        );   
    }  
    return pHistImage;  
}  
int main( int argc, char** argv )  
{     
    const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsGrayTitle = "»ҶÈ¼(http://blog.csdn.net/MoreWindows)";  
    const char *pstrWindowsHistTitle = "ֱ·½ͼ(http://blog.csdn.net/MoreWindows)";  
  
    // ´ÓļþÖ¼ÓØ­ͼ  
    IplImage *pSrcImage = cvLoadImage("src.bmp", CV_LOAD_IMAGE_UNCHANGED);  
    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  
    // »ҶÈ¼  
    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);  
  
    // »ҶÈ±·½ͼ  
    CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);  
      
    // ´´½¨ֱ·½ͼͼÏ  
    int nHistImageWidth = 255;  
    int nHistImageHeight = 150;  //ֱ·½ͼͼÏ¸߶È 
    int nScale = 2;              
    IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);  
  
    // Ïʾ  
    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);  
    cvNamedWindow(pstrWindowsGrayTitle, CV_WINDOW_AUTOSIZE);  
    cvNamedWindow(pstrWindowsHistTitle, CV_WINDOW_AUTOSIZE);  
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);  
    cvShowImage(pstrWindowsGrayTitle, pGrayImage);  
    cvShowImage(pstrWindowsHistTitle, pHistImage);  
  
  
    cvWaitKey(0);  
  
    cvReleaseHist(&pcvHistogram);  
  
    cvDestroyWindow(pstrWindowsSrcTitle);  
    cvDestroyWindow(pstrWindowsGrayTitle);  
    cvDestroyWindow(pstrWindowsHistTitle);  
    cvReleaseImage(&pSrcImage);  
    cvReleaseImage(&pGrayImage);  
    cvReleaseImage(&pHistImage);  
    return 0;  
}  
