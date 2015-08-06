#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#include "ImageIO.h"

#define FILE_IN  "/home/allan/Documentos/repositorios/lsbd_release_v4.4.0/src/views/lsc_lite/Resources/Images/Icons/AAAicnLSCLite_256.png"   //"/home/allan/Imagens/Filter/i_002.png"
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

	ImageIO *imageio=NULL;
	imageio = ImageIOCreate();
	ImageIOSetImage(imageio, img_in);
	cvReleaseImage(&img_in);
	ImageIOGenerate(imageio);

	ShowImage(imageio->out, "Arquivo");
	cvWaitKey(0);

	ImageIODestroy(&imageio);

	printf("\n\n ***FIM***\n");
	return 0;
}


