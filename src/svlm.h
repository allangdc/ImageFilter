/*
 * svlm.h
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */

#ifndef SVLM_H_
#define SVLM_H_

#include <cv.h>

typedef struct svlmimage
{
    IplImage    *b32, *g32, *r32;
    IplImage    *h32, *s32, *v32;
    CvSize      size;
    IplImage    *out;
    IplImage    *svlm;
    IplImage    *S;
    float       alpha;
    float       lambda;
} SVLMImage;

SVLMImage *SvlmCreate(IplImage *bgr_image, float alpha, float lambda);
void SvlmDestroy(SVLMImage **svlm);
void SvlmComponent(SVLMImage *svlm_image);
void SvlmFilter(IplImage *src, IplImage *dst);
void SvlmLuminanceEnhancement(SVLMImage *svlm_image);
void SvlmContrastEnhancement(SVLMImage *svlm_image);
void SvlmColorRestoration(SVLMImage *svlm_image);

#endif /* SVLM_H_ */
