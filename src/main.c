#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#include "sampling.h"

#define FILE_IN  "/home/allan/Imagens/floresta.jpg"   //"/home/allan/Imagens/Filter/i_002.png"
#define FILE_OUT "/home/allan/Imagens/floresta.jpg"  //"/home/allan/Imagens/Filter/o_002.png"

void ShowImage(IplImage *image, const char *title) {
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvSetWindowProperty(title, CV_WND_PROP_AUTOSIZE , CV_WINDOW_FULLSCREEN);
	cvShowImage(title, image);
}

int main(int argc, char **argv) {
	char *title_in = "Entrada";
	char *title_out = "Saida";

	IplImage *img_in = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);
	IplImage *img_out = cvLoadImage(FILE_OUT, CV_LOAD_IMAGE_COLOR);

	SamplingRegion s;
	SamplingRegionCreate(&s);
	SampleRegionSetPoint(&s, 150, 150);
	SampleRegionSetSquareSize(&s, 3);

	IplImage *ii = SampleRegionGetImageRegion(img_in, &s);
	IplImage *i = cvCreateImage(cvGetSize(ii), IPL_DEPTH_8U, 1);
	cvCvtColor(ii, i, CV_BGR2GRAY);

	uchar *v = SampleRegionGenerateVector(i);
	int a;
	for(a=0; a<ii->width*ii->height*i->nChannels; a++)
 		printf( " %d ", *v++ );
	printf( "\n" );

	PrintImage(i, 0);
	ShowImage(ii, "Opa");
	cvWaitKey(0);

	printf("\n\n ***FIM***\n");
	return 0;
}


