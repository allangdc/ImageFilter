#include "sampling.h"

#include <stdio.h>
#include <time.h>
#include <highgui.h>

SamplingRegion *SamplingRegionCreate(SamplingRegion *sample_allocated)
{
	SamplingRegion *sample;
	if(sample_allocated != NULL) {
		sample = sample_allocated;
	}
	else
	{
		sample = (SamplingRegion *) malloc(sizeof(SamplingRegion));
	}
	sample->center = cvPoint(0, 0);
	sample->square_size = 0;
	sample->region = cvRect(0,0,0,0);
	sample->updated=0;
	return sample;
}

void SampleRegionDestroy(SamplingRegion **sample)
{
	free(*sample);
	*sample = NULL;
}

void SampleRegionSetPoint(SamplingRegion *sampling, int x, int y)
{
	sampling->center = cvPoint(x, y);
	sampling->updated = 0;
}

void SampleRegionSetSquareSize(SamplingRegion *sampling, int side_size)
{
	sampling->square_size = side_size;
	sampling->updated = 0;
}

CvRect SampleRegionGetRegion(SamplingRegion *sampling)
{
	if(!sampling->updated)
	{
		int mn = sampling->square_size;
		int d = (mn-1)/2;
		CvPoint *pt = &(sampling->center);
		sampling->region = cvRect(pt->x-d, pt->y-d, mn, mn);
		sampling->updated = 1;
	}
	return sampling->region;
}

IplImage *SampleRegionGetImageRegion(SamplingRegion *sampling, IplImage *src)
{
	IplImage *image = NULL;
	CvRect region = SampleRegionGetRegion(sampling);
	cvSetImageROI(src, region);
	image = cvCreateImage(cvSize(sampling->square_size, sampling->square_size), IPL_DEPTH_8U, src->nChannels);
	cvCopy(src, image, NULL);
	cvResetImageROI(src);
	return image;
}

void PrintImage(const IplImage *image, int channel)
{
	int x, y;
	for(y=0 ; y<image->height; y++)
	{
		printf("|");
		for(x=0; x<image->width; x++)
		{
			uchar value = CV_IMAGE_ELEM(image, uchar, y, x*image->nChannels + channel);
			printf(" %3d ", value);
		}
		printf("|\n");
	}
}

IplImage *GetComponentColor(const IplImage *rgb_image, int id_component)
{
	IplImage *y;
	y = cvCreateImage(cvGetSize(rgb_image), IPL_DEPTH_8U, 1);
	switch(id_component)
	{
		case 0:
			cvSplit(rgb_image, y, NULL, NULL, NULL);
			break;
		case 1:
			cvSplit(rgb_image, NULL, y, NULL, NULL);
			break;
		case 2:
			cvSplit(rgb_image, NULL, NULL, y, NULL);
			break;
		case 3:
			cvSplit(rgb_image, NULL, NULL, NULL, y);
			break;
	}
	return y;
}

uchar *SampleRegionGenerateVector(IplImage *image)
{
	uchar *vector = (uchar *) malloc(sizeof(uchar)*(image->width*image->height*image->nChannels));
	uchar *c = (uchar *) image->imageData;
	int w, h, ch, index=0;
	for(h=0; h<image->height; h++)
	{
		for(w=0; w<image->width; w++)
			for(ch=0; ch<image->nChannels; ch++)
				*(vector + index++) = CV_IMAGE_ELEM(image, uchar, h, w*image->nChannels + ch); //*c++;
	}
	return vector;
}

