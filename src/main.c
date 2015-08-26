#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

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
	IplImage *hsv, *hsv_32, *h, *s, *v;
	CvSize size = cvGetSize(src);
	hsv = cvCreateImage(size, IPL_DEPTH_8U, 3);
	hsv_32 = cvCreateImage(size, IPL_DEPTH_32F, 3);
	h = cvCreateImage(size, IPL_DEPTH_32F, 1);
	s = cvCreateImage(size, IPL_DEPTH_32F, 1);
	v = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	cvConvertScale(hsv, hsv_32, 1, 0);
	cvSplit(hsv_32, h, s, v, NULL);

	IplImage **L = (IplImage **) malloc(sizeof(IplImage *) * 4);
	int dv;
	int i;
	for(i=0; i<4; i++)
	{
		dv = pow(2, i);
		CvSize size_reduce = cvSize(size.width/dv, size.height/dv);

		*(L+i) = cvCreateImage(size, IPL_DEPTH_32F, 1);
		IplImage *aux = cvCreateImage(size_reduce, IPL_DEPTH_32F, 1);

		cvResize(v, aux, CV_INTER_CUBIC);
		cvSmooth(aux, aux, CV_GAUSSIAN, 3, 3, 0, 0);
		cvResize(aux, *(L+i), CV_INTER_CUBIC);
		cvReleaseImage(&aux);
	}

	IplImage *s1, *s2, *s3, *f;
	s1 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	s2 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	s3 = cvCreateImage(size, IPL_DEPTH_32F, 1);
	f = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvAddWeighted(*(L+0), 0.25, *(L+1), 0.25, 0, s1);
	cvAddWeighted(*(L+2), 0.25, *(L+3), 0.25, 0, s2);
	cvAddWeighted(s1, 0.25, s2, 0.25, 0, s3);
	cvConvertScale(s3, f, 1, 0);
	ShowImage(f, "S3");
}

void aSVLM(IplImage *src, IplImage *dst)
{
	int i;
	IplImage *h = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *s = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *v = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cvCvtColor(src, hsv, CV_BGR2HSV);
	cvSplit(hsv, h, s, v, NULL);
	IplImage **L = (IplImage **) malloc(sizeof(IplImage *) * 4);

	IplImage *aux;
	int dv;
	CvSize size = cvGetSize(v);
	for(i=0; i<4; i++)
	{
		dv = pow(2, i);
		*(L+i) = cvCreateImage(cvGetSize(v), IPL_DEPTH_8U, 1);
		aux = cvCreateImage(cvSize(size.width/dv, size.height/dv), IPL_DEPTH_8U, 1);
		cvResize(v, aux, CV_INTER_CUBIC);
		cvSmooth(aux, aux, CV_GAUSSIAN, 3, 3, 0, 0);
		cvResize(aux, *(L+i), CV_INTER_CUBIC);
		cvReleaseImage(&aux);
	}

	int wx, hy;
	for(hy=0; hy<v->height; hy++)
	{
		for(wx=0; wx<v->width; wx++)
		{
			uchar v1 = CV_IMAGE_ELEM(*(L+0), uchar, hy, wx);
			uchar v2 = CV_IMAGE_ELEM(*(L+1), uchar, hy, wx);
			uchar v3 = CV_IMAGE_ELEM(*(L+2), uchar, hy, wx);
			uchar v4 = CV_IMAGE_ELEM(*(L+3), uchar, hy, wx);
			uchar I = CV_IMAGE_ELEM(v, uchar, hy, wx);
			float svlm = (float)v1 + (float)v2 + (float)v3 + (float)v4;
			svlm /= 4.0;

			double alpha = 0.5;
			double gama = pow(alpha, ((128.0-svlm)/128.0) );
			double o = pow(255.0 * (I/255.0), gama);
			uchar out = (uchar) o;
			CV_IMAGE_ELEM(v, uchar, hy, wx) = out;
			//is missing to keep coding
		}
	}
	cvMerge(h, s, v, NULL, hsv);
	cvCvtColor(hsv, dst, CV_HSV2BGR);

	/* destruindo */
	cvReleaseImage(&h);
	cvReleaseImage(&s);
	cvReleaseImage(&v);
	cvReleaseImage(&hsv);
	for(i=0; i<4; i++)
	{
		cvReleaseImage(&(*(L+i)));
		*(L+i) = NULL;
	}
	free(L);
	L=NULL;
}

int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in_a = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);

	IplImage *out = cvCreateImage(cvGetSize(img_in_a), IPL_DEPTH_8U, 3);
	int x;
	//for(x=0; x<1000000; x++);
		svlm(img_in_a, out);
	//ShowImage(out, "output");
	cvWaitKey(0);
	

	printf("\n\n ***FIM***\n");
	return 0;
}


