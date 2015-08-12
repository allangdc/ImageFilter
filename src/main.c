#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "ImageIO.h"
#include "sampling.h"

#define FILE_IN  "/tmp/guest-NQNzeV/Pictures/img.png"

void ShowImage(IplImage *image, const char *title) 
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvSetWindowProperty(title, CV_WND_PROP_AUTOSIZE , CV_WINDOW_FULLSCREEN);
	cvShowImage(title, image);
}

void SVLM(IplImage *src, IplImage **dst)
{
	IplImage *tr1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *tr2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *tr3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *tr4 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage *tr = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

	cvSmooth(src, tr1, CV_GAUSSIAN, 3, 3, 0, 0);
	cvSmooth(src, tr2, CV_GAUSSIAN, 5, 5, 0, 0);
	cvSmooth(src, tr3, CV_GAUSSIAN, 7, 7, 0, 0);
	cvSmooth(src, tr4, CV_GAUSSIAN, 9, 9, 0, 0);

	if(*dst)
	{
		free(*dst);
		*dst=NULL;
	}
	*dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	int w, h;
	for(h=0; h<src->height; h++)
	{
		for(w=0; w<src->width; w++)
		{
			uchar v1 = CV_IMAGE_ELEM(tr1, uchar, h, w);
			uchar v2 = CV_IMAGE_ELEM(tr2, uchar, h, w);
			uchar v3 = CV_IMAGE_ELEM(tr3, uchar, h, w);
			uchar v4 = CV_IMAGE_ELEM(tr4, uchar, h, w);
			uchar I = CV_IMAGE_ELEM(src, uchar, h, w);
			float svlm = v1 + v2 + v3 + v4;
			svlm /= 4.0;

			double alpha = 0.5;
			double gama = pow(alpha, ((128.0-svlm)/128.0) );

			double o = pow(255.0 * (I/255.0), gama);

			uchar out = (uchar) o;
			CV_IMAGE_ELEM(*dst, uchar, h, w) = out;
		}
	}
}

int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in_a = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);
	IplImage *img_in = cvCreateImage(cvSize(img_in_a->width/2, img_in_a->height/2), IPL_DEPTH_8U, 3);
	cvResize(img_in_a, img_in, CV_INTER_LINEAR);

	IplImage *in = cvCreateImage(cvGetSize(img_in), IPL_DEPTH_8U, 3);
	IplImage *H = cvCreateImage(cvGetSize(img_in), IPL_DEPTH_8U, 1);
	IplImage *S = cvCreateImage(cvGetSize(img_in), IPL_DEPTH_8U, 1);
	IplImage *V = cvCreateImage(cvGetSize(img_in), IPL_DEPTH_8U, 1);
	cvCvtColor(img_in, in, CV_BGR2HSV);
	cvSplit(in, H,S,V, NULL);
	IplImage *out = NULL;

	SVLM(V, &out);
	ShowImage(V, "input");
	ShowImage(out, "output");
	cvWaitKey(0);
	

	printf("\n\n ***FIM***\n");
	return 0;
}


