#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>

#include "svlm.h"
#include "ToneMap.h"
#include "ImageTest.h"

#define FILE_IN  "../src/images/img001.jpg"

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

    IplImage *out1 = cvCreateImage(cvGetSize(img_in_a), IPL_DEPTH_8U, 3);
    IplImage *out2 = cvCreateImage(cvGetSize(img_in_a), IPL_DEPTH_8U, 3);
	int x;

	double diff = 0;
    for(x=0; x<100; x++)
	{
		clock_t start = clock();
        SvlmFilter(img_in_a, out1);
        //ToneMapFilter(img_in_a, out2);
		clock_t stop = clock();
		diff += (stop - start) / (double) (CLOCKS_PER_SEC);
	}
	printf("\nTIME = %lf\n", diff);
    ShowImage(img_in_a, "Original");
    ShowImage(out1, "SVLM");
    //ShowImage(out2, "ToneMapping");
	cvWaitKey(0);


	//VideoImage();
	
    //in 100 convertions
    //TIME = 3.944358   -> ToneMapping
    //TIME = 27.485970  -> SVLM


	printf("\n\n ***FIM***\n");
	return 0;
}


