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

typedef struct {
	int ave[3];
	int var[3];
	int sd[3];
} img_stat;

void get_img_statistics(img_stat *img_stat, char *infile) {
	IplImage* img = NULL;
	IplImage* img_hsv = NULL;
	uint sum[3] = {0};
	uint diff_sum[3] = {0};

	img = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	cvCvtColor(img, img_hsv, CV_RGB2HSV);

	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		sum[0] += (uint)(uchar)img_hsv->imageData[i + 0];
		sum[1] += (uint)(uchar)img_hsv->imageData[i + 1];
		sum[2] += (uint)(uchar)img_hsv->imageData[i + 2];
	}

	for (int i = 0; i < 3; i++) {
		img_stat->ave[i] = (int)(sum[i]/(img_hsv->width*img_hsv->height));
	}

	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		diff_sum[0] += square((int)(uint)(uchar)img_hsv->imageData[i + 0] - img_stat->ave[0]);
		diff_sum[1] += square((int)(uint)(uchar)img_hsv->imageData[i + 1] - img_stat->ave[1]);
		diff_sum[2] += square((int)(uint)(uchar)img_hsv->imageData[i + 2] - img_stat->ave[2]);
	}

	for (int i = 0; i < 3; i++) {
		img_stat->var[i] = diff_sum[i]/(img_hsv->width*img_hsv->height);
		img_stat->sd[i] = sqrt(img_stat->var[i]);
	}

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

void test000(char *infile, float sat_tgt_ave, float sat_tgt_deviation,
			 float val_tgt_ave, float val_tgt_deviation) {
	char sat_conv_tab[256];
	char val_conv_tab[256];
	img_stat img_stat;

	printf("ave ( . %f %f )\n", sat_tgt_ave, val_tgt_ave);
	printf("sd  ( . %f %f )\n", sat_tgt_deviation, val_tgt_deviation);

	get_img_statistics(&img_stat, infile);

	for (int i = 0; i < sizeof(sat_conv_tab)/sizeof(sat_conv_tab[0]); i++) {
#if 1
		float t = i + (sat_tgt_ave - img_stat.ave[1]) +
			(i - img_stat.ave[1])*sat_tgt_deviation/img_stat.sd[1];
#else
		float t = i + (sat_tgt_ave - img_stat.ave[1]);
#endif
		if (t < 0) {
			sat_conv_tab[i] = 0;
		} else if (t > 255) {
			sat_conv_tab[i] = 255;
		} else {
			sat_conv_tab[i] = (int)t;
		}
	}

	for (int i = 0; i < sizeof(val_conv_tab)/sizeof(val_conv_tab[0]); i++) {
#if 1
		float t = i + (val_tgt_ave - img_stat.ave[2]) +
			(i - img_stat.ave[2])*val_tgt_deviation/img_stat.sd[2];
#else
		float t = i + (val_tgt_ave - img_stat.ave[2]);
#endif
		if (t < 0) {
			val_conv_tab[i] = 0;
		} else if (t > 255) {
			val_conv_tab[i] = 255;
		} else {
			val_conv_tab[i] = (int)t;
		}
	}

	IplImage* img = NULL;
	IplImage* img_hsv = NULL;

	img = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	cvCvtColor(img, img_hsv, CV_RGB2HSV);

#if 0
	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		img_hsv->imageData[i + 1] = sat_conv_tab[(uint)(uchar)img_hsv->imageData[i + 1]];
	}
#endif
	
	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		img_hsv->imageData[i + 2] = val_conv_tab[(uint)(uchar)img_hsv->imageData[i + 2]];
	}
	
	cvCvtColor(img_hsv, img, CV_HSV2RGB);

	cvSaveImage("out.jpg", img);

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	// infile, saturation-target-ave, saturation-target-sandard-deviation
	//   value-target-ave, value-target-standart-deviation
	test000(argv[1], atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
	return 0;
}
