#ifndef TONEMAP_H_
#define TONEMAP_H_

#include <cv.h>

typedef enum illumination_type{
    GENERAL,
    LOW,
    HIGH
} ILLUMINATION_TYPE;

typedef struct tonemapimage
{
    CvSize      size;
    IplImage    *b, *g, *r;
    IplImage    *b1, *g1, *r1;
    float       alfa, beta;
    IplImage    *l1, *invl;
    IplImage    *out;
} ToneMapImage;

ToneMapImage *ToneMapCreate(IplImage *bgr_image);
void ToneMapDestroy(ToneMapImage **tmap);
void ToneMapFilter(IplImage *src, IplImage *dst);
void ToneMapLuminanceEnhancement(ToneMapImage *tmap);
void ToneMapColorRestoration(ToneMapImage *tmap);

#endif  //TONEMAP_H_
