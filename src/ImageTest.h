#ifndef __IMAGETEST__
#define __IMAGETEST__

#include <cv.h>

void PrintMatrix1ch(IplImage *image, int square, int cx, int cy);
void PrintFloatMatrix1ch(IplImage *image, int square, int cx, int cy);
void ShowImage(IplImage *image, const char *title);

#endif  // __IMAGETEST__
