#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>

#include <omp.h>

#include "ImageIO.h"
#include "sampling.h"

#define FILE_IN  "../src/images/img001.jpg"

void ShowImage(IplImage *image, const char *title) 
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvSetWindowProperty(title, CV_WND_PROP_AUTOSIZE , CV_WINDOW_FULLSCREEN);
	cvShowImage(title, image);
}

void svlm(IplImage *src, IplImage *dst)
{
	IplImage *hsv, *hsv32, *h32, *s32, *v32;
	CvSize size = cvGetSize(src);

	hsv = cvCreateImage(size, IPL_DEPTH_8U, 3);
	hsv32 = cvCreateImage(size, IPL_DEPTH_32F, 3);
	h32 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	s32 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	v32 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	cvConvertScale(hsv, hsv32, 1, 0);
	cvSplit(hsv32, h32, s32, v32, NULL);
	cvReleaseImage(&hsv32);
	cvReleaseImage(&hsv);

	IplImage **L = (IplImage **) malloc(sizeof(IplImage *) * 4);
	int dv=1;
	int i;
#pragma omp parallel for
	for(i=0; i<4; i++)
	{
		CvSize size_reduce = cvSize(size.width/dv, size.height/dv);

		*(L+i) = cvCreateImage(size, IPL_DEPTH_32F, 1);
		IplImage *aux = cvCreateImage(size_reduce, IPL_DEPTH_32F, 1);

		cvResize(v32, aux, CV_INTER_CUBIC);
		cvSmooth(aux, aux, CV_GAUSSIAN, 3, 3, 0, 0);
		cvResize(aux, *(L+i), CV_INTER_CUBIC);
		cvReleaseImage(&aux);
		dv *= 2;
	}

	IplImage **svlm = (IplImage **) malloc(sizeof(IplImage *) * 2);
	int j;
#pragma omp parallel for
	for(j=0; j<2; j++)
	{
		*(svlm + j) = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvAddWeighted(*(L+j), 0.25, *(L+j+2), 0.25, 0, *(svlm + j));
		cvReleaseImage(&(*(L+j)));
		cvReleaseImage(&(*(L+j+2)));
	}

	IplImage *o = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvAddWeighted(*(svlm + 0), 0.25, *(svlm + 1), 0.25, 0, o);
	cvReleaseImage(&(*(svlm+0)));
	cvReleaseImage(&(*(svlm+1)));
	free(svlm);

	//IplImage *f = cvCreateImage(size, IPL_DEPTH_8U, 1);
	//cvConvertScale(o, f, 1, 0);
	//ShowImage(f, "S3");

	cvReleaseImage(&o);
	cvReleaseImage(&h32);
	cvReleaseImage(&s32);
	cvReleaseImage(&v32);
}

int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in_a = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);

	IplImage *out = cvCreateImage(cvGetSize(img_in_a), IPL_DEPTH_8U, 3);
	int x;
	//for(x=0; x<1000000; x++);

	double diff = 0;
	for(x=0; x<1000; x++)
	{
		clock_t start = clock();
		svlm(img_in_a, out);
		clock_t stop = clock();
		diff += (stop - start) / (double) (CLOCKS_PER_SEC);
	}
	printf("\nTIME = %lf\n", diff);
	//ShowImage(out, "output");
	//cvWaitKey(0);
	
	//TIME = 189.554687
	//TIME = 0.194058


	printf("\n\n ***FIM***\n");
	return 0;
}


