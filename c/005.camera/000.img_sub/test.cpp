#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#include <list>

using namespace std;

void test000(char *infile1, float scale1, char *infile2, float scale2) {
	IplImage* img1 = NULL;
	IplImage* img2 = NULL;
	IplImage* sub = NULL;

	img1 = cvLoadImage(infile1, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img2 = cvLoadImage(infile2, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	sub = cvCreateImage(cvSize(img1->width, img1->height), IPL_DEPTH_8U, 3);

	printf("s1:%f s2:%f\n", scale1, scale2);
	printf("widthStep:%d height:%d\n", img1->widthStep, img1->height);

	for (int i = 0; i < img1->widthStep*img1->height; i++ ) {
		int c = ((float)(uchar)img2->imageData[i])*scale2 -
			((float)(uchar)img1->imageData[i])*scale1;
		if (c > 255) {
			sub->imageData[i] = 255;
		} else if (c < 0) {
			sub->imageData[i] = 0;
		} else {
			sub->imageData[i] = c;
		}
	}

#if 0
	IplImage* sub_hsv = NULL;
	sub_hsv = cvCreateImage(cvSize(img1->width, img1->height), IPL_DEPTH_8U, 3);
	cvCvtColor(sub, sub_hsv, CV_RGB2HSV);

	for (int i = 2; i < img1->widthStep*img1->height; i++ ) {
		int c = (int)((uint)(uchar)sub->imageData[i]+30);
		if (c > 255) {
			c = 255;
		}
		sub_hsv->imageData[i] = c;
	}

	for (int i = 1; i < img1->widthStep*img1->height; i++ ) {
		int c = (int)((uint)(uchar)sub->imageData[i]-30);
		if (c < 0) {
			c = 0;
		}
		sub_hsv->imageData[i] = c;
	}
#endif

	cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
#if 0
	cvShowImage("img", sub_hsv);
#endif
	cvWaitKey(0);
	cvDestroyWindow("img");

	cvSaveImage("sub.jpg", sub);

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&sub);
#if 0
	cvReleaseImage(&sub_hsv);
#endif
}

int main(int argc, char **argv) {
	test000(argv[1], atof(argv[2]), argv[3], atof(argv[4]));
	
	return 0;
}
