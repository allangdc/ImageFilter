#include "ToneMap.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ImageTest.h"

ToneMapImage *ToneMapCreate(IplImage *bgr_image, int type, float alpha)
{
    ToneMapImage *tmap;
    IplImage *bgr32;

    tmap = (ToneMapImage *) malloc(sizeof(ToneMapImage));
    tmap->filter_type = type;
    tmap->alfa = alpha;
    tmap->beta = 10;
    tmap->size = cvGetSize(bgr_image);

    bgr32 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 3);
    tmap->b = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->g = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->r = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);

    tmap->b1 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->g1 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->r1 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);

    cvConvertScale(bgr_image, bgr32, 1, 0);
    cvSplit(bgr32, tmap->b, tmap->g, tmap->r, NULL);

    tmap->l1 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->invl = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    tmap->out = cvCreateImage(tmap->size, IPL_DEPTH_32F, 3);

    cvReleaseImage(&bgr32);
    return tmap;
}

void ToneMapDestroy(ToneMapImage **tmap)
{
    ToneMapImage *t;
    t = *tmap;

    cvReleaseImage(&t->b);
    cvReleaseImage(&t->g);
    cvReleaseImage(&t->r);
    cvReleaseImage(&t->b1);
    cvReleaseImage(&t->g1);
    cvReleaseImage(&t->r1);
    cvReleaseImage(&t->l1);
    cvReleaseImage(&t->invl);
    cvReleaseImage(&t->out);

    free(*tmap);
    tmap = NULL;
}

void ToneMapLuminanceEnhancement(ToneMapImage *tmap)
{
    float lmax;
    IplImage *laux, *l;
    double aux;
    IplImage *ones;

    ones = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    cvSet(ones, cvScalar(1,1,1,1), NULL);                   // set matrix with ones

    lmax = 255;
    laux = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    l = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    cvAddWeighted(tmap->r, 0.27, tmap->g, 0.67, 0, laux);
    cvAddWeighted(laux, 1, tmap->b, 0.06, 0, l);            // l=0.27R + 0.67G + 0.06B

    cvDiv(ones, l, tmap->invl, 1);                          // inv(l) = 1 / l

    switch (tmap->filter_type) {
    case GENERAL:
        //@TODO: GENERAL has problem, not use
        cvAddWeighted(l, tmap->beta*(tmap->alfa-1), ones, 1, 0, laux);  // laux = L*B*(A-1)+1
        cvLog(laux, tmap->l1);                                          // l1 = ln(laux)
        aux  = 1 / log(tmap->alfa);
        cvMul(tmap->l1, ones, tmap->l1, aux);                           // l1 = l1 / ln(A);
        break;
    case LOW:
        cvAddWeighted(l, 1/lmax * (tmap->alfa-1), ones, 1, 0, laux);    // laux = (L/Lmax)*(A-1)+1
        cvLog(laux, tmap->l1);                                          // l1 = ln(laux)
        aux  = 255 / log(tmap->alfa);
        cvMul(tmap->l1, ones, tmap->l1, aux);                           // l1 = 255 * l1 / ln(A);
        break;
    case HIGH:
        //%L1 = (a.^(L/Lmax) - 1) / (a-1) * Lmax;
        cvMul(l, ones, laux, 1/lmax);                   // laux = (L/LMax);
        cvMul(laux, ones, laux, log(tmap->alfa));       // laux = laux * ln(a);
        cvExp(laux, laux);                              // laux = e^(laux*ln(a)) = a^laux
        cvSub(laux, ones, laux, NULL);                  // laux = a^laux-1
        aux = lmax/(tmap->alfa-1);
        cvMul(laux, ones, tmap->l1, aux);               //(a^(L/Lmax))*Lmax/(a-1)
        break;
    default:
        break;
    }

    cvReleaseImage(&ones);
    cvReleaseImage(&laux);
    cvReleaseImage(&l);
}

void ToneMapColorRestoration(ToneMapImage *tmap)
{
    IplImage *r2, *g2, *b2;
    r2 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    g2 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);
    b2 = cvCreateImage(tmap->size, IPL_DEPTH_32F, 1);

    cvMul(tmap->r, tmap->invl, tmap->r1, 1);
    cvMul(tmap->g, tmap->invl, tmap->g1, 1);
    cvMul(tmap->b, tmap->invl, tmap->b1, 1);

    cvMul(tmap->r1, tmap->l1, r2, 1);
    cvMul(tmap->g1, tmap->l1, g2, 1);
    cvMul(tmap->b1, tmap->l1, b2, 1);

    cvMerge(b2, g2, r2, NULL, tmap->out);

    cvReleaseImage(&r2);
    cvReleaseImage(&g2);
    cvReleaseImage(&b2);
}

void ToneMapFilter(IplImage *src, IplImage *dst)
{
    ToneMapImage *tmap = ToneMapCreate(src, LOW, 10);
    ToneMapLuminanceEnhancement(tmap);
    ToneMapColorRestoration(tmap);
    cvConvertScale(tmap->out, dst, 1, 0);
    ToneMapDestroy(&tmap);
}
