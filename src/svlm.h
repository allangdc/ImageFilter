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
	IplImage *b32, *g32, *r32;
	IplImage *h32, *s32, *v32;
	CvSize size;
	IplImage *vout;
} SVLMImage;

SVLMImage *SvlmCreate(IplImage *bgr_image);
void SvlmDestroy(SVLMImage **svlm);
void SvlmEdit(SVLMImage *svlm, IplImage *bgr_image);
IplImage *SvlmComponent(IplImage *value_component_32f);
void SvlmFilter(IplImage *src, IplImage *dst);
void SvlmLuminanceEnhancement(SVLMImage *svlm_image, IplImage *svlm);
void SvlmContrastEnhancement(SVLMImage *svlm_image, IplImage *svlm);
void SvlmColorRestoration(SVLMImage *svlm_image, IplImage *S);

#endif /* SVLM_H_ */
