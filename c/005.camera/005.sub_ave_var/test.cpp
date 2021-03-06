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

#define square(x) ((x)*(x))

typedef struct {
	int ave[3];
	int var[3];
	int sd[3];
} img_stat;

void img_sub(IplImage** psub, char *infile1, float scale1, char *infile2, float scale2) {
	IplImage* img1 = NULL;
	IplImage* img2 = NULL;
	IplImage* sub = NULL;

	img1 = cvLoadImage(infile1, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img2 = cvLoadImage(infile2, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

	sub = cvCreateImage(cvSize(img1->width, img1->height), IPL_DEPTH_8U, 3);
	*psub = sub;

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

//	cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
//	cvShowImage("img", sub);
//	cvWaitKey(0);
//	cvDestroyWindow("img");

//	cvSaveImage("sub.jpg", sub);

	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
//	cvReleaseImage(&sub);
}

void get_img_statistics(img_stat *img_stat, IplImage* img) {
	IplImage* img_hsv = NULL;
	uint sum[3] = {0};
	uint diff_sum[3] = {0};

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

	cvReleaseImage(&img_hsv);
}

void test000(char *infile1, float scale1, char *infile2, float scale2,
			 float sat_tgt_ave, float sat_tgt_deviation,
			 float val_tgt_ave, float val_tgt_deviation) {
	char sat_conv_tab[256];
	char val_conv_tab[256];
	img_stat img_stat;
	IplImage* img = NULL;
	
//	img = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);

//	img_sub(IplImage* sub, char *infile1, float scale1, char *infile2, float scale2);
	img_sub(&img, infile1, scale1, infile2, scale2);
	
	get_img_statistics(&img_stat, img);
	
	printf("ave ( . %f %f ) << ( %d %d %d )\n", sat_tgt_ave, val_tgt_ave,
		   img_stat.ave[0], img_stat.ave[1], img_stat.ave[2]);
	printf("sd  ( . %f %f ) << ( %d %d %d )\n", sat_tgt_deviation, val_tgt_deviation,
		   img_stat.sd[0], img_stat.sd[1], img_stat.sd[2]);

	for (int i = 0; i < sizeof(sat_conv_tab)/sizeof(sat_conv_tab[0]); i++) {
		float t = sat_tgt_ave + (i - img_stat.ave[1])*sat_tgt_deviation/img_stat.sd[1];
		if (t < 0) {
			sat_conv_tab[i] = 0;
		} else if (t > 255) {
			sat_conv_tab[i] = 255;
		} else {
			sat_conv_tab[i] = (int)t;
		}
	}

	for (int i = 0; i < sizeof(val_conv_tab)/sizeof(val_conv_tab[0]); i++) {
		float t = val_tgt_ave + (i - img_stat.ave[2])*val_tgt_deviation/img_stat.sd[2];
		if (t < 0) {
			val_conv_tab[i] = 0;
		} else if (t > 255) {
			val_conv_tab[i] = 255;
		} else {
			val_conv_tab[i] = (int)t;
		}
	}

//	IplImage* img = NULL;
	IplImage* img_hsv = NULL;

//	img = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	img_hsv = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	cvCvtColor(img, img_hsv, CV_RGB2HSV);

	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		img_hsv->imageData[i + 1] = sat_conv_tab[(uint)(uchar)img_hsv->imageData[i + 1]];
	}
	
	for (int i = 0; i < img_hsv->widthStep*img_hsv->height; i += 3 ) {
		img_hsv->imageData[i + 2] = val_conv_tab[(uint)(uchar)img_hsv->imageData[i + 2]];
	}
	
	cvCvtColor(img_hsv, img, CV_HSV2RGB);

	cvSaveImage("out.jpg", img);

	cvReleaseImage(&img);
	cvReleaseImage(&img_hsv);
}

int main(int argc, char **argv) {
	// infile1, scale1, infile2, scale2
	// saturation-target-ave, saturation-target-sandard-deviation
	//   value-target-ave, value-target-standart-deviation
	char *infile1 = argv[1];
	float scale1 = atof(argv[2]);
	char *infile2 = argv[3];
	float scale2 = atof(argv[4]);
	float saturation_tgt_ave = atof(argv[5]);
	float saturation_tgt_sd = atof(argv[6]);
	float value_tgt_ave = atof(argv[7]);
	float value_tgt_sd = atof(argv[8]);

	test000(infile1, scale1, infile2, scale2,
			saturation_tgt_ave, saturation_tgt_sd,
			value_tgt_ave, value_tgt_sd);

	return 0;
}
