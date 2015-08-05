#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#include "ImageIO.h"

#define FILE_IN  "/tmp/guest-3XRYGC/Pictures/file01.jpg"   //"/home/allan/Imagens/Filter/i_002.png"
#define FILE_OUT FILE_IN  //"/home/allan/Imagens/Filter/o_002.png"

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

	IplImage *img = cvCreateImage(cvGetSize(img_in), IPL_DEPTH_8U, 1);
	IplImage *img2 = NULL;
	cvCvtColor(img_in, img, CV_BGR2GRAY);
	//PrintImage(img, 0);

	printf("Aqui\n");
	ImageIOSaltEffect(img_in, NULL, 0.5);
	ShowImage(img_in, "Arquivo");
	cvWaitKey(0);

	printf("\n\n ***FIM***\n");
	return 0;
}


