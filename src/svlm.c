/*
 * svlm.c
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */


#include "svlm.h"

#include <highgui.h>

IplImage *SvlmComponent(IplImage *value_component_32f)
{
	IplImage **L, **svlm_vector;
	int i;
	CvSize size;

	size = cvGetSize(value_component_32f);
	L = (IplImage **) malloc(sizeof(IplImage *) * 4);
	int dv=1;
#pragma omp parallel for
	for(i=0; i<4; i++)
	{
		IplImage *aux;
		CvSize size_reduce = cvSize(size.width/dv, size.height/dv);

		*(L+i) = cvCreateImage(size, IPL_DEPTH_32F, 1);
		aux = cvCreateImage(size_reduce, IPL_DEPTH_32F, 1);

		cvResize(value_component_32f, aux, CV_INTER_CUBIC);
		cvSmooth(aux, aux, CV_GAUSSIAN, 3, 3, 0, 0);
		cvResize(aux, *(L+i), CV_INTER_CUBIC);
		cvReleaseImage(&aux);
		dv *= 2;
	}

	svlm_vector = (IplImage **) malloc(sizeof(IplImage *) * 2);
	int j;
#pragma omp parallel for
	for(j=0; j<2; j++)
	{
		*(svlm_vector + j) = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvAddWeighted(*(L+j), 0.25, *(L+j+2), 0.25, 0, *(svlm_vector + j));
		cvReleaseImage(&(*(L+j)));
		cvReleaseImage(&(*(L+j+2)));
	}
	free(L);

	IplImage *svlm = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvAddWeighted(*(svlm_vector + 0), 0.25, *(svlm_vector + 1), 0.25, 0, svlm);
	cvReleaseImage(&(*(svlm_vector+0)));
	cvReleaseImage(&(*(svlm_vector+1)));
	free(svlm_vector);
	return svlm;
}

SVLMImage *SvlmCreate(IplImage *bgr_image)
{
	IplImage *hsv, *hsv32;

	SVLMImage *svlm = (SVLMImage *) malloc(sizeof(SVLMImage));
	svlm->size = cvGetSize(bgr_image);

	hsv = cvCreateImage(svlm->size, IPL_DEPTH_8U, 3);
	hsv32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 3);
	svlm->h32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->s32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->v32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);

	cvCvtColor(bgr_image, hsv, CV_BGR2HSV);
	cvConvertScale(hsv, hsv32, 1, 0);
	cvSplit(hsv32, svlm->h32, svlm->s32, svlm->v32, NULL);
	cvReleaseImage(&hsv32);
	cvReleaseImage(&hsv);
	return svlm;
}

void SvlmEdit(SVLMImage *svlm, IplImage *bgr_image)
{
	IplImage *hsv, *hsv32;

	CvSize tmp = cvGetSize(bgr_image);
	if(svlm->size.height != tmp.height || svlm->size.width != tmp.width)
	{
		printf("Cannot to edit image - Different size\n");
		return;
	}

	hsv = cvCreateImage(svlm->size, IPL_DEPTH_8U, 3);
	hsv32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 3);

	cvCvtColor(bgr_image, hsv, CV_BGR2HSV);
	cvConvertScale(hsv, hsv32, 1, 0);
	cvSplit(hsv32, svlm->h32, svlm->s32, svlm->v32, NULL);

	cvReleaseImage(&hsv32);
	cvReleaseImage(&hsv);
}

void SvlmDestroy(SVLMImage **svlm)
{
	SVLMImage *s = *svlm;
	cvReleaseImage(&(s->h32));
	cvReleaseImage(&(s->s32));
	cvReleaseImage(&(s->v32));
	free(*svlm);
	*svlm = NULL;
}

void SvlmFilter(IplImage *src, IplImage *dst)
{
	SVLMImage *svlm_image = SvlmCreate(src);

	IplImage *svlm = SvlmComponent(svlm_image->v32);

	cvConvertScale(svlm, dst, 1, 0);

	cvReleaseImage(&svlm);
	SvlmDestroy(&svlm_image);
}

