#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>

#include "svlm.h"

#define FILE_IN  "../src/images/img001.jpg"

void ShowImage(IplImage *image, const char *title) 
{
	cvNamedWindow(title, CV_WINDOW_AUTOSIZE);
	cvSetWindowProperty(title, CV_WND_PROP_AUTOSIZE , CV_WINDOW_FULLSCREEN);
	cvShowImage(title, image);
}

void VideoImage(void)
{
	CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);

	char ch=0;
	IplImage *svlm = NULL;
	IplImage *v = NULL;
	IplImage *hsv = NULL;
	while(ch!='q')
	{
		IplImage *frame = cvQueryFrame(capture);
		if(!svlm)
		{
			svlm = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			hsv = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
			v = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		}
		SvlmFilter(frame, svlm);

		cvCvtColor(frame, hsv, CV_BGR2HSV);
		cvSplit(hsv, NULL, NULL, v, NULL);
		ShowImage(svlm, "svlm");
		ShowImage(v, "frame");
		ch = (char) cvWaitKey(50);
	}

	cvReleaseImage(&svlm);
	cvReleaseCapture(&capture);
}



int main(int argc, char **argv) 
{
	char *title_in = "Entrada";

	IplImage *img_in_a = cvLoadImage(FILE_IN, CV_LOAD_IMAGE_COLOR);

    IplImage *out = cvCreateImage(cvGetSize(img_in_a), IPL_DEPTH_8U, 3);
	int x;

	double diff = 0;
    for(x=0; x<1; x++)
	{
		clock_t start = clock();
		SvlmFilter(img_in_a, out);
		clock_t stop = clock();
		diff += (stop - start) / (double) (CLOCKS_PER_SEC);
	}
	printf("\nTIME = %lf\n", diff);
	ShowImage(img_in_a, "input");
	ShowImage(out, "output");
	cvWaitKey(0);


	//VideoImage();
	
	//TIME = 189.554687
	//TIME = 0.194058


	printf("\n\n ***FIM***\n");
	return 0;
}


