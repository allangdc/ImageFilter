/*
 * svlm.c
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */


#include "svlm.h"

#include <highgui.h>
#include <math.h>
#include <stdio.h>

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
	cvAddWeighted(*(svlm_vector + 0), 1, *(svlm_vector + 1), 1, 0, svlm);
	cvReleaseImage(&(*(svlm_vector+0)));
	cvReleaseImage(&(*(svlm_vector+1)));
	free(svlm_vector);
	return svlm;
}

SVLMImage *SvlmCreate(IplImage *bgr_image)
{
	IplImage *hsv, *hsv32;
	IplImage *bgr32;

	SVLMImage *svlm = (SVLMImage *) malloc(sizeof(SVLMImage));
	svlm->size = cvGetSize(bgr_image);

	hsv = cvCreateImage(svlm->size, IPL_DEPTH_8U, 3);
	hsv32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 3);
	svlm->h32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->s32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->v32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);

	bgr32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 3);
	svlm->b32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->g32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);
	svlm->r32 = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);

	svlm->vout = cvCreateImage(svlm->size, IPL_DEPTH_32F, 1);

	cvCvtColor(bgr_image, hsv, CV_BGR2HSV);
	cvConvertScale(hsv, hsv32, 1, 0);
	cvSplit(hsv32, svlm->h32, svlm->s32, svlm->v32, NULL);

	cvConvertScale(bgr_image, bgr32, 1, 0);
	cvSplit(bgr32, svlm->b32, svlm->g32, svlm->r32, NULL);

	cvReleaseImage(&bgr32);
	cvReleaseImage(&hsv32);
	cvReleaseImage(&hsv);
	return svlm;
}

/**
 * @TODO Adds the edit implementation for bgr32 images.
 */
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

	cvReleaseImage(&(s->b32));
	cvReleaseImage(&(s->g32));
	cvReleaseImage(&(s->r32));

	cvReleaseImage(&(s->vout));
	free(*svlm);
	*svlm = NULL;
}

void SvlmFilter(IplImage *src, IplImage *dst)
{
	SVLMImage *svlm_image = SvlmCreate(src);

	IplImage *svlm = SvlmComponent(svlm_image->v32);

    SvlmLuminanceEnhancement(svlm_image, svlm);
    SvlmContrastEnhancement(svlm_image, svlm);

    IplImage *aux = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 3);
    cvMerge(svlm_image->b32, svlm_image->g32, svlm_image->r32, NULL, aux);
    cvConvertScale(aux, dst, 1, 0);

    cvReleaseImage(&aux);
	cvReleaseImage(&svlm);
	SvlmDestroy(&svlm_image);
}

void SvlmLuminanceEnhancement(SVLMImage *svlm_image, IplImage *isvlm)
{
    float alpha = 0.5;

    IplImage *mul = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvSet(mul, cvScalar(1,1,1,1), NULL);

    IplImage *lambda = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    IplImage *scale_svlm = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvMul(isvlm, mul, scale_svlm, 1.0/128.0);
    cvSub(mul, scale_svlm, scale_svlm, NULL);   //scale_svlm = 1 - svlm/128;
    cvMul(scale_svlm, mul, lambda, log(alpha)); // = (I*ln a)
    cvReleaseImage(&scale_svlm);
    cvExp(lambda, lambda);                      // = e^(I*ln a) = a^I

    IplImage *O = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvMul(svlm_image->v32, mul, O, 1.0/255.0);     //(I/255)
    cvLog(O, O);                                    //log(I/255)
    cvMul(O, lambda, O, 1);                         //lambda * log(I/255)
    cvExp(O, O);                                    //e^(lambda * log(I/255)) = (I/255)^lambda
    cvMul(O, mul, svlm_image->vout, 255.0);         //255*((I/255)^lambda)

    cvReleaseImage(&O);
    cvReleaseImage(&mul);
    cvReleaseImage(&lambda);
}

void SvlmContrastEnhancement(SVLMImage *svlm_image, IplImage *isvlm)
{
    CvScalar std_scalar, mean_scalar;
    cvAvgSdv(svlm_image->v32, &mean_scalar, &std_scalar, NULL);
    int sigma = std_scalar.val[0];
    float p;
    if(sigma <= 40)
        p = 2;
    else if(sigma <= 80)
        p = -0.025*sigma + 3;
    else
        p = 1;

    IplImage *mul = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvSet(mul, cvScalar(1,1,1,1), NULL);

    IplImage *E = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvDiv(isvlm, svlm_image->vout, E, 1);
    cvPow(E,E, p);

    IplImage *S = cvCreateImage(svlm_image->size, IPL_DEPTH_32F, 1);
    cvMul(svlm_image->vout, mul, S, 1.0/255.0);     //(O/255)
    cvLog(S, S);                                    //log(O/255)
    cvMul(S, E, S, 1);                              //E * log(O/255)
    cvExp(S, S);                                    //e^(E * log(O/255)) = (O/255)^E
    cvMul(S, mul, S, 255.0);                        //255*((O/255)^E)

    SvlmColorRestoration(svlm_image, S);

    cvReleaseImage(&mul);
    cvReleaseImage(&S);
    cvReleaseImage(&E);
}

void SvlmColorRestoration(SVLMImage *svlm_image, IplImage *S)
{
    float lambda = 1;
    cvDiv(svlm_image->r32, svlm_image->vout, svlm_image->r32, 1);
    cvMul(svlm_image->r32, S, svlm_image->r32, lambda);

    cvDiv(svlm_image->g32, svlm_image->vout, svlm_image->g32, 1);
    cvMul(svlm_image->g32, S, svlm_image->g32, lambda);

    cvDiv(svlm_image->b32, svlm_image->vout, svlm_image->b32, 1);
    cvMul(svlm_image->b32, S, svlm_image->b32, lambda);
}
