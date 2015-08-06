#ifndef __IMAGEIO__
#define __IMAGEIO__

#include <cv.h>

typedef struct imageio
{
	IplImage *in;
	IplImage *out;
} ImageIO;

ImageIO *ImageIOCreate(void);
void ImageIODestroy(ImageIO **imageio);
void ImageIOSetImage(ImageIO *imageio, IplImage *img);
void ImageIOSaltEffect(IplImage *src, IplImage **dst, double percent);
void ImageIOGenerate(ImageIO *imageio);

#endif //__IMAGEIO__
