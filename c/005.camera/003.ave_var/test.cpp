#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#include <list>
#include <math.h>

using namespace std;

#define square(x) ((x)*(x))

void test000(char *infile) {
	IplImage* img = NULL;
	IplImage* img_hsv = NULL;
	uint sum[3] = {0};
	uint ave[3];
	uint diff_sum[3] = {0};
	uint var[3];

	img = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	cvCvtColor(img, img_hsv, CV_RGB2HSV);

	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		sum[0] += (uint)(uchar)img_hsv->imageData[i + 0];
		sum[1] += (uint)(uchar)img_hsv->imageData[i + 1];
		sum[2] += (uint)(uchar)img_hsv->imageData[i + 2];
	}
	
	for (int i = 0; i < 3; i++) {
		ave[i] = sum[i]/(img_hsv->width*img_hsv->height);
	}

	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
#if 0
		printf("%d %d %d %d\n", (int)(uint)(uchar)img_hsv->imageData[i + 0],
			   (int)ave[0], (int)(uint)(uchar)img_hsv->imageData[i + 0] - (int)ave[0],
			   square((int)(uint)(uchar)img_hsv->imageData[i + 0] - (int)ave[0]));
		printf("%d %d %d %d\n", (int)(uint)(uchar)img_hsv->imageData[i + 1],
			   (int)ave[1], (int)(uint)(uchar)img_hsv->imageData[i + 1] - (int)ave[1],
			   square((int)(uint)(uchar)img_hsv->imageData[i + 1] - (int)ave[1]));
		printf("%d %d %d %d\n", (int)(uint)(uchar)img_hsv->imageData[i + 2],
			   (int)ave[2], (int)(uint)(uchar)img_hsv->imageData[i + 2] - (int)ave[2],
			   square((int)(uint)(uchar)img_hsv->imageData[i + 2] - (int)ave[2]));
#endif
		diff_sum[0] += square((int)(uint)(uchar)img_hsv->imageData[i + 0] - (int)ave[0]);
		diff_sum[1] += square((int)(uint)(uchar)img_hsv->imageData[i + 1] - (int)ave[1]);
		diff_sum[2] += square((int)(uint)(uchar)img_hsv->imageData[i + 2] - (int)ave[2]);
	}

	for (int i = 0; i < 3; i++) {
		var[i] = diff_sum[i]/(img_hsv->width*img_hsv->height);
	}

	printf("ave ( %d %d %d )\n", ave[0], ave[1], ave[2]);
	printf("var ( %d %d %d )\n", var[0], var[1], var[2]);
	printf("standard-deviation ( %d %d %d )\n", (int)sqrt(var[0]), (int)sqrt(var[1]), (int)sqrt(var[2]));
	
	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	test000(argv[1]);
	
	return 0;
}
