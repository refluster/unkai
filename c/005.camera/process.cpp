#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#include <list>
#include "image_input.h"

using namespace std;

#define square(x) ((x)*(x))

#define KPOC_MEASURE ((unsigned long long)10000)
#define INVALID 0xffffffff

void laveling(uint **lavel, uint width, uint height, list<uint> *lavel_area) {
	// lavel
	int cur_lavel_no = 0;
	uint green_level = 0;
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (lavel[y][x] == INVALID) {
				stack<point> stack;
				int area = 0;

				point p = {x, y};
				stack.push(p);

				++cur_lavel_no;

				// set lavel to the same area
				while(! stack.empty()) {
					// pop
					int xx = stack.top().x, yy = stack.top().y;
					stack.pop();
					
					if (lavel[yy][xx] == INVALID) {
						++area;
						
						// laveling
						lavel[yy][xx] = cur_lavel_no;
					}
					
					// push arround
					if (xx - 1 >= 0 && lavel[yy][xx - 1] == INVALID) {
						point p = {xx - 1, yy};
						stack.push(p);
					}
					if (xx + 1 < width && lavel[yy][xx + 1] == INVALID) {
						point p = {xx + 1, yy};
						stack.push(p);
					}
					if (yy - 1 >= 0 && lavel[yy - 1][xx] == INVALID) {
						point p = {xx, yy - 1};
						stack.push(p);
					}
					if (yy + 1 < height && lavel[yy + 1][xx] == INVALID) {
						point p = {xx, yy + 1};
						stack.push(p);
					}
				}

				lavel_area->push_back(area);
			}
		}
	}

	printf("lavel_no %d\n", cur_lavel_no);
}

void get_mask_hsv(IplImage* src, IplImage* mask, hsv_filter *hsv_filter) {
	CvPixelPosition8u pos_src;
	IplImage* tmp;

	tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	cvCvtColor(src, tmp, CV_RGB2HSV);
	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
					  tmp->widthStep,cvGetSize(tmp), 0, 0, tmp->origin);

	// filter
	uint *_lavel = new uint [src->height*src->width];
	uint **lavel = new uint* [src->height];

	for (int y = 0; y < src->height; y++) {
		lavel[y] = _lavel + y*src->width;
	}

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
			uchar H = p_src[0], S = p_src[1], V = p_src[2];
			if (hsv_filter->h_min <= H && H <= hsv_filter->h_max &&
				hsv_filter->s_min <= S && S <= hsv_filter->s_max &&
				hsv_filter->v_min <= V && V <= hsv_filter->v_max) {
				lavel[y][x] = INVALID;
			}
		}
	}

	// lavel
	list<uint> lavel_area;
	laveling(lavel, src->width, src->height, &lavel_area);

	int centerH = (hsv_filter->h_min + hsv_filter->h_max) / 2;
	CvPixelPosition8u pos_dst;
	uint max_area = 0;
	int max_area_lavel_no = INVALID;
	uint green_level = 0;

	for (int i = 1; !lavel_area.empty(); ++i) {
		uint area = lavel_area.front();
		if (max_area < area) {
			max_area_lavel_no = i;
			max_area = area;
		}
		lavel_area.pop_front();
	}

//////////////////////////////
	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
					  tmp->widthStep,cvGetSize(tmp), 0, 0, tmp->origin);
	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
					  mask->widthStep, cvGetSize(mask), 0, 0, mask->origin);

	// create mask to pass only the largest lavel
	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			if (lavel[y][x] == max_area_lavel_no) {
				uchar *p_dst = CV_MOVE_TO(pos_dst, x, y, 3);
				uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
				p_dst[0] = p_dst[1] = p_dst[2] = 255;
				green_level += square(p_src[0] - centerH)*3;
			}
//			uchar *p_dst = CV_MOVE_TO(pos_dst, x, y, 3);
//			p_dst[0] = ((uchar)(0x12345678*lavel[y][x]))/2;
//			p_dst[1] = (uchar)(0x34567812*lavel[y][x]);
//			p_dst[2] = (uchar)(0x56781234*lavel[y][x]);
		}
	}

	printf("%llu %u %d %d %d\n",
		   KPOC_MEASURE,
		   max_area, (src->height*src->width), src->height, src->width);
	printf("green area (0-%llu, higher is larger): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*max_area/(src->height*src->width));
	printf("green level (0-%llu, lower is better): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*green_level/max_area/(src->height*src->width));
	
	cvReleaseImage(&tmp);
}

void measure(IplImage* frame, char *outfile, int display, hsv_filter *hsv_filter) {
	IplImage* mask = NULL;
	IplImage* dst = NULL;
	
	mask = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	dst = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	
	get_mask_hsv(frame, mask, hsv_filter);
	
	if (display) {
		cvAnd(frame, mask, dst);

		// show original image
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvShowImage("src", frame);
		
		// show mask image
		cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
		cvShowImage("mask", mask);

		// show masked image
		cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
		cvShowImage("dst", dst);

		cvWaitKey(0);

		cvDestroyWindow("src");
		cvDestroyWindow("mask");
		cvDestroyWindow("dst");
	}

#if 0
	{
		cvAnd(frame, mask, dst);

		// show original image
		cvSaveImage("src.jpg", frame);
		
		// show mask image
		cvSaveImage("mask.jpg", mask);

		// show masked image
		cvSaveImage("dst.jpg", dst);
	}
#endif

	if (outfile) {
		cvAnd(frame, mask, dst);
		cvSaveImage(outfile, dst);
	}

	cvReleaseImage(&dst);
	cvReleaseImage(&mask);
}

int main(int argc, char **argv) {
	int display = 0; // display image
	char *infile = NULL;
	char *outfile = NULL;
	struct stat st;
	image_input *image = NULL;
	camera_property camconf;
	hsv_filter hsv_filter;

	// set default value
	camconf.brightness = 20;
	camconf.contrast = 60;
	camconf.saturation = 40;
	camconf.gain = 80;
	camconf.width = 320;
	camconf.height = 240;

	hsv_filter.h_min = 30;
	hsv_filter.h_max = 100;
	hsv_filter.s_min = 60;
	hsv_filter.s_max = 255;
	hsv_filter.v_min = 20;
	hsv_filter.v_max = 255;

	int go_res;
	while ((go_res = getopt(argc, argv, "di:o:b:c:s:g:r:l:")) != -1) {
		switch (go_res) {
		case 'b': // camera brightness
			camconf.brightness = atoi(optarg);;
			break;
		case 'c': // camera contrast
			camconf.contrast = atoi(optarg);;
			break;
		case 'd': // display image 
			display = 1;
			break;
		case 'g': // camera gain
			camconf.gain = atoi(optarg);;
			break;
		case 'i': // input image file
			infile = optarg;
			break;
		case 'o': // set output file
			outfile = optarg;
			break;
		case 's': // camera saturation
			camconf.saturation = atoi(optarg);
			break;
		case 'r': // camera resolution
			camconf.width = atoi(strtok(optarg, "x"));
			camconf.height = atoi(strtok(NULL, "x"));
			break;
		case 'l': // hsv sub space to filter green area
			hsv_filter.h_min = atoi(strtok(optarg, ":"));
			hsv_filter.h_max = atoi(strtok(NULL, ":"));
			hsv_filter.s_min = atoi(strtok(NULL, ":"));
			hsv_filter.s_max = atoi(strtok(NULL, ":"));
			hsv_filter.v_min = atoi(strtok(NULL, ":"));
			hsv_filter.v_max = atoi(strtok(NULL, ":"));
			break;
		case ':':
			fprintf(stdout, "%c needs value\n", go_res);
			break;
		case '?':
			break;
		}
	}
	
#ifndef ENABLE_CAMERA
	if (infile == NULL) {
		fprintf(stderr, "camera is not enabled\n");
		return -1;
	}
#endif
	if (infile != NULL && stat(infile, &st) != 0) {
		fprintf(stderr, "input file not exist\n");
		return -1;
	}

	printf("HSV filter: %d-%d %d-%d %d-%d\n", hsv_filter.h_min, hsv_filter.h_max,
		   hsv_filter.s_min, hsv_filter.s_max,
		   hsv_filter.v_min, hsv_filter.v_max);

	if (infile) {
		printf("load image from %s\n", infile);
		image = new class image_input(infile);
	} else {
		printf("capture image B:%d S:%d G:%d C:%d [%dx%d]\n",
			   camconf.brightness, camconf.saturation, camconf.gain, camconf.contrast,
			   camconf.width, camconf.height);
		image = new class image_input(&camconf);
	}

	measure(image->get_image(), outfile, display, &hsv_filter);

	delete image;

	return(0);
}
