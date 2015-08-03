#include "ImageIO.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <highgui.h>

ImageIO *ImageIOCreate(void)
{
	ImageIO *imageio = (ImageIO *) malloc(sizeof(ImageIO));
	imageio->in = NULL;
	imageio->out = NULL;
	return imageio;
}

void ImageIODestroy(ImageIO **imageio)
{
	if(*imageio)
	{
		if((*imageio)->in)
		{
			free((*imageio)->in);
			(*imageio)->in = NULL;
		}
		if((*imageio)->out)
		{
			free((*imageio)->out);
			(*imageio)->out = NULL;
		}
		free(*imageio);
		*imageio = NULL;
	}
}

void ImageIOSetImage(ImageIO *imageio, IplImage *img)
{
	imageio->out = img;
}

void ImageIOSaltEffect(IplImage *src, IplImage *dst, double percent)
{
	int total = src->width * src->height;
	int range_noise = (double) total * percent;
	int i;
	srand(time(NULL));
	for(i=0; i<range_noise; i++)
	{
		int ch;
		CvPoint pt = cvPoint(rand()%src->height, rand()%src->width);
		for(ch=0; ch<src->nChannels; ch++)
		{
			CV_MAT_ELEM(dst, uchar, pt.y, pt.x*src->nChannels + ch) = (uchar) (rand()%256);
		}
	}
}

//void ImageIOGenerate(ImageIO *imageio);
