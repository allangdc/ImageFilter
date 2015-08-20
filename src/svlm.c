/*
 * svlm.c
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */


#include "svlm.h"

#include <highgui.h>

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
