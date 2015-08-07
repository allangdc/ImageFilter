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
			cvReleaseImage(&(*imageio)->in);
			(*imageio)->in = NULL;
		}
		if((*imageio)->out)
		{
			cvReleaseImage(&(*imageio)->out);
			(*imageio)->out = NULL;
		}
		free(*imageio);
		*imageio = NULL;
	}
}

void ImageIOSetImage(ImageIO *imageio, IplImage *img)
{
	if(imageio->out)
	{
		cvReleaseImage(&imageio->in);
		imageio->in = NULL;
	}
	imageio->in = cvCloneImage(img);
}

void ImageIOSaltEffect(IplImage *src, IplImage **dst, double percent)
{
	int total = src->width * src->height;
	int range_noise = (double) total * percent;
	printf("Total=%d, range_noise=%d", total, range_noise);
	int i;
	srand(time(NULL));
	if(*dst)
	{
		cvReleaseImage(dst);
		*dst = NULL;
	}
	else
	{
		//*dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);
		*dst = cvCloneImage(src);
	}
	//dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);
	for(i=0; i<range_noise; i++)
	{
		int ch;
		CvPoint pt = cvPoint(rand()%(*dst)->height, rand()%(*dst)->width);
		for(ch=0; ch<(*dst)->nChannels; ch++)
		{
			CV_IMAGE_ELEM(*dst, uchar, pt.x, pt.y*(*dst)->nChannels + ch) = rand()%256;
		}
	}
}

void ImageIOGenerate(ImageIO *imageio)
{
	ImageIOSaltEffect(imageio->in, &(imageio->out), 0.01);
	cvSmooth(imageio->out, imageio->out, CV_BLUR, 5, 5, 0, 0);
}


