#include "ImageTest.h"

#include <stdio.h>

void PrintMatrix1ch(IplImage *image, int square, int cx, int cy)
{
    int ch = 1;
    int x,y;
    int mid = (square-1)/2;
    for(y=cy-mid; y<=cy+mid; y++) {
        printf("|");
        for(x=cx-mid; x<=cx+mid; x++) {
            int pixel = CV_IMAGE_ELEM(image, uchar, y, x*ch + 1);
            printf(" %3d ", pixel);
        }
        printf("|\n");
    }
    printf("\n");
}

void PrintFloatMatrix1ch(IplImage *image, int square, int cx, int cy)
{
    int ch = 1;
    int x,y;
    int mid = (square-1)/2;
    for(y=cy-mid; y<=cy+mid; y++) {
        printf("|");
        for(x=cx-mid; x<=cx+mid; x++) {
            float pixel = CV_IMAGE_ELEM(image, float, y, x*ch + 1);
            printf(" %8.4f ", pixel);
        }
        printf("|\n");
    }
    printf("\n");
}
