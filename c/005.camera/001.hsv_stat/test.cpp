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

void test000(char *infile1) {
	IplImage* img = NULL;
	IplImage* img_hsv = NULL;
	int count[3][256] = {{0}};

	img = cvLoadImage(infile1, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);

	cvCvtColor(img, img_hsv, CV_RGB2HSV);

	for (int i = 0; i < img->widthStep*img->height; i+=3 ) {
		count[0][(uint)(uchar)img_hsv->imageData[i    ]] ++;
		count[1][(uint)(uchar)img_hsv->imageData[i + 1]] ++;
		count[2][(uint)(uchar)img_hsv->imageData[i + 2]] ++;
	}

	for (int i = 0; i < 256; i++ ) {
		printf("%d %d %d %d\n", i, count[0][i], count[1][i], count[2][i]);
	}

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	test000(argv[1]);
	
	return 0;
}
