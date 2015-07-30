#ifndef __SAMPLING__
#define __SAMPLING__

#include <cv.h>

typedef struct samplingregion
{
	CvPoint center;
	int square_size;
	CvRect region;
	int updated;
} SamplingRegion;

SamplingRegion *SamplingRegionCreate(SamplingRegion *sample_allocated);
void SampleRegionDestroy(SamplingRegion **sample);
void SampleRegionSetPoint(SamplingRegion *sampling, int x, int y);
void SampleRegionSetSquareSize(SamplingRegion *sampling, int side_size);
CvRect SampleRegionGetRegion(SamplingRegion *sampling);
IplImage *SampleRegionGetImageRegion(IplImage *src, SamplingRegion *sampling);
uchar *SampleRegionGenerateVector(IplImage *image);

void PrintImage(const IplImage *image, int channel);
IplImage *GetComponentColor(const IplImage *rgb_image, int id_component);

#endif //__SAMPLING__
