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

void test000(char *infile1, int upper, int bottom, int left, int right) {
	IplImage* img = NULL;
	IplImage* img_hsv = NULL;

	img = cvLoadImage(infile1, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);

	cvCvtColor(img, img_hsv, CV_RGB2HSV);

	int sum[3] = {0};
	int count = (bottom - upper)*(right - left);
	for (int y = upper; y < bottom; y++) {
		for (int x = left; x < right; x++) {
			sum[0] += (uint)(uchar)img->imageData[x*3 + y*320 + 0];
			sum[1] += (uint)(uchar)img->imageData[x*3 + y*320 + 1];
			sum[2] += (uint)(uchar)img->imageData[x*3 + y*320 + 2];
		}
	}
	
	printf("%d %d %d\n", sum[0]/count, sum[1]/count, sum[2]/count);

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	test000(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
	
	return 0;
}
