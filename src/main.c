#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#include "ImageIO.h"
#include "sampling.h"

#define FILE_IN  "/home/allan/Imagens/floresta.jpg"
#define FILE_OUT FILE_IN  //"/home/allan/Imagens/Filter/o_002.png"

void ShowImage(IplImage *image, const char *title) 
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvSetWindowProperty(title, CV_WND_PROP_AUTOSIZE , CV_WINDOW_FULLSCREEN);
	cvShowImage(title, image);
}

int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);

	ImageIO *imageio = ImageIOCreate();
	ImageIOSetImage(imageio, img_in);
	cvReleaseImage(&img_in);
	ImageIOGenerate(imageio);
	SamplingRegion *region = SamplingRegionCreate(NULL);
	SampleRegionSetSquareSize(region, 39);
	SampleRegionSetPoint(region, 150,150);
	IplImage *img_in_9x9 = SampleRegionGetImageRegion(region, imageio->in);
	

	ShowImage(img_in_9x9, title_in);
	cvWaitKey(0);
	cvReleaseImage(&img_in_9x9);

	cvDestroyWindow(title_in);
	ImageIODestroy(&imageio);

	printf("\n\n ***FIM***\n");
	return 0;
}


