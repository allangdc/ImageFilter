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
	IplImage *h32, *s32, *v32;
	CvSize size;
	IplImage *vout;
} SVLMImage;

SVLMImage *SvlmCreate(IplImage *bgr_image);
void SvlmDestroy(SVLMImage **svlm);
void SvlmEdit(SVLMImage *svlm, IplImage *bgr_image);

IplImage *SvlmComponent(IplImage *value_component_32f);
void SvlmFilter(IplImage *src, IplImage *dst);

#endif /* SVLM_H_ */
