/*
 * ImageComponents.c
 *
 *  Created on: 11 de ago de 2015
 *      Author: allan
 */

#include "ImageComponents.h"

#include <stdlib.h>

ImageComponents *ImageComponentsCreate()
{
	ImageComponents *ic = (ImageComponents *) malloc(sizeof(ImageComponents));
	ic->image = NULL;
	ic->img[0] = NULL;
	ic->img[1] = NULL;
	ic->img[2] = NULL;
	return ic;
}

void ImageComponentsSetImage(ImageComponents *image_componentes, IplImage *image)
{
	if(image_componentes->image)
	{
		cvReleaseImage(&image_componentes->image);
		image_componentes->image = NULL;
	}
	image_componentes->image = cvClone(image);
}

void ImageComponentsSetComponent(ImageComponents *image_componentes, int component, IplImage *image)
{
	if(component<3)
	{
		if(image_componentes->img[component])
		{
			cvReleaseImage(&image_componentes->img[component]);
			image_componentes->img[component] = NULL;
		}
		image_componentes->img[component] = cvClone(image);
	}
}

void ImageComponentsDestroy(ImageComponents **image_componentes)
{
	int i;
	ImageComponents *ic;
	if(*image_componentes)
	{
		ic = *image_componentes;
		if(ic->image)
			cvReleaseImage(&ic->image);
		for(int i=0; i<3; i++)
		{
			if(ic->img[i])
				cvReleaseImage(ic->img[i]);
		}
		free(*image_componentes);
		*image_componentes = NULL;
	}
}

void ImageComponentsSplitComponents(ImageComponents *image_componentes)
{
	int i;
	for(int i=0; i<3; i++)
	{
		if(image_componentes->img[i])
			cvReleaseImage(&image_componentes->img[i]);
		image_componentes->img[i] = NULL;
		image_componentes->img[i] = cvCreateImage(cvGetSize(image_componentes->image, IPL_DEPTH_8U, 1));
	}
	cvSplit(image_componentes->image,
			image_componentes->img[0],
			image_componentes->img[1],
			image_componentes->img[2],
			NULL);
}

void ImageComponentsImage2RGB(ImageComponents *image_componentes)
{
	ImageComponentsSplitComponents(image_componentes);
}

void ImageComponentsImage2YUV(ImageComponents *image_componentes)
{
	int i;
	IplImage *image = cvCreateImage(cvGetSize(image_componentes->image, IPL_DEPTH_8U, 3));
	cvCvtColor(image_componentes->image, image, CV_BGR2YUV);
	ImageComponentsSplitComponents(image_componentes);
	cvReleaseImage(&image);
}

void ImageComponentsRGB2Image(ImageComponents *image_componentes)
{
	cvMerge(image_componentes->img[0],
			image_componentes->img[0],
			image_componentes->img[0],
			NULL,
			image_componentes->image);
}






