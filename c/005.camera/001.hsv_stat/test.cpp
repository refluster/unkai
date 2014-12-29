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
#if 0
	int count[3][256] = {{0}};
#endif

	img = cvLoadImage(infile1, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);

	cvCvtColor(img, img_hsv, CV_RGB2HSV);

#if 0
	for (int i = 0; i < img->widthStep*img->height; i+=3 ) {
		count[0][(uint)(uchar)img_hsv->imageData[i    ]] ++;
		count[1][(uint)(uchar)img_hsv->imageData[i + 1]] ++;
		count[2][(uint)(uchar)img_hsv->imageData[i + 2]] ++;
	}

	for (int i = 0; i < 256; i++ ) {
		printf("%d %d %d %d\n", i, count[0][i], count[1][i], count[2][i]);
	}
#endif

	int sum[3] = {0};
	for (int y = 80; y < 84; y++) {
		for (int x = 200; x < 204; x++) {
			sum[0] += (uint)(uchar)img->imageData[x*3 + y*320 + 0];
			sum[1] += (uint)(uchar)img->imageData[x*3 + y*320 + 1];
			sum[2] += (uint)(uchar)img->imageData[x*3 + y*320 + 2];
		}
	}
	
	printf("%d %d %d\n", sum[0]/16, sum[1]/16, sum[2]/16);

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	test000(argv[1]);
	
	return 0;
}
