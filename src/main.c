#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "ImageIO.h"
#include "sampling.h"

#define FILE_IN  "/tmp/guest-BPnygU/Pictures/lena_noise.bmp"
#define FILE_OUT FILE_IN  //"/home/allan/Imagens/Filter/o_002.png"

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

	ShowImage(tr1, "3x3");
	ShowImage(tr2, "5x5");
	ShowImage(tr3, "7x7");
	ShowImage(tr4, "9x9");

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
			float sum = v1 + v2 + v3 + v4;
			sum /= 4.0;
			//CV_IMAGE_ELEM(tr, uchar, h, w) = (uchar) sum;
			//Image_ratio(i,j) = 0.5^((128-Image_gray_trans(i,j))/128);
			double ratio = pow(0.5, ((128.0-sum)/128.0) );

			//O(i,j) = 255 * (Image_gray(i,j)/255)^Image_ratio(i,j);
			double o = pow(255.0 * (sum/255.0), ratio);

			CV_IMAGE_ELEM(*dst, uchar, h, w) = (uchar) o;
		}
	}
}

int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);
	IplImage *img_in_res = cvCreateImage(cvSize(img_in->width/2, img_in->height/2), IPL_DEPTH_8U, 3);
	cvResize(img_in, img_in_res, CV_INTER_LINEAR);

	IplImage *in = cvCreateImage(cvGetSize(img_in_res), IPL_DEPTH_8U, 1);
	IplImage *Y = cvCreateImage(cvGetSize(img_in_res), IPL_DEPTH_8U, 1);
	IplImage *U = cvCreateImage(cvGetSize(img_in_res), IPL_DEPTH_8U, 1);
	IplImage *V = cvCreateImage(cvGetSize(img_in_res), IPL_DEPTH_8U, 1);
	cvCvtColor(img_in_res, in, CV_BGR2GRAY);
	IplImage *out = NULL;

	SVLM(in, &out);
	ShowImage(in, "input");
	ShowImage(out, "output");
	cvWaitKey(0);
	

	printf("\n\n ***FIM***\n");
	return 0;
}


