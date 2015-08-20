/*
 * ImageComponents.h
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */

#ifndef IMAGECOMPONENTS_H_
#define IMAGECOMPONENTS_H_

#include <cv.h>

typedef struct imagecomponents
{
	IplImage *image;
	IplImage *img[3];
} ImageComponents;

ImageComponents *ImageComponentsCreate();
void ImageComponentsSetImage(ImageComponents *image_componentes, IplImage *image);
void ImageComponentsSetComponent(ImageComponents *image_componentes, int component, IplImage *image);
void ImageComponentsDestroy(ImageComponents **image_componentes);
void ImageComponentsImage2RGB(ImageComponents *image_componentes);
void ImageComponentsImage2YUV(ImageComponents *image_componentes);
void ImageComponentsRGB2Image(ImageComponents *image_componentes);

#endif /* IMAGECOMPONENTS_H_ */
