#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define square(x) ((x)*(x))

#define KPOC_MEASURE ((unsigned long long)10000)

uchar minH, minS, minV, maxH, maxS, maxV;
unsigned int area;
unsigned long long green_level;

using namespace std;

void GetMaskHSV(IplImage* src, IplImage* mask) {
	CvPixelPosition8u pos_src, pos_dst;
	IplImage* tmp;

	tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);

	cvCvtColor(src, tmp, CV_RGB2HSV);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
					  tmp->widthStep,cvGetSize(tmp), 0, 0, tmp->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
					  mask->widthStep, cvGetSize(mask), 0, 0, mask->origin);

	area = 0;
	green_level = 0;
	int centerH = (minH + maxH) / 2;
	int centerS = (minS + maxS) / 2;
	int centerV = (minV + maxV) / 2;
		
	// filter
	uint *_lavel = new uint [tmp->height*tmp->width];
	uint **lavel = new uint* [tmp->height];

	for (int y = 0; y < tmp->height; y++) {
		lavel[y] = _lavel + y*tmp->width;
	}

	for (int y = 0; y < tmp->height; y++) {
		for (int x = 0; x < tmp->width; x++) {
			uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
			uchar H = p_src[0], S = p_src[1], V = p_src[2];
			if (minH <= H && H <= maxH &&
				minS <= S && S <= maxS &&
				minV <= V && V <= maxV) {
				lavel[y][x] = 0xffffffff;
			}
		}
	}

	// lavel
	int lavel_no = 0;

	int max_area_lavel_no = 0xffffffff;
	int max_area = 0;
	
	for (int y = 0; y < tmp->height; y++) {
		for (int x = 0; x < tmp->width; x++) {
			if (lavel[y][x] == 0xffffffff) {
				typedef struct {
					int x, y;
				} point;

				int area = 0;
				int stp = 0;
				point stack[tmp->width*tmp->height];

				// push
				stack[stp].x = x;
				stack[stp].y = y;
				++stp;

				++lavel_no;
				// stack operation
				while(stp != 0) {
					// pop
					int xx, yy;
					--stp;
					xx = stack[stp].x;
					yy = stack[stp].y;

					// push arround
					if (xx - 1 >= 0 && lavel[yy][xx - 1] == 0xffffffff) {
						stack[stp].x = xx - 1;
						stack[stp].y = yy;
						++stp;
					}
					if (xx + 1 < tmp->width && lavel[yy][xx + 1] == 0xffffffff) {
						stack[stp].x = xx + 1;
						stack[stp].y = yy;
						++stp;
					}
					if (yy - 1 >= 0 && lavel[yy - 1][xx] == 0xffffffff) {
						stack[stp].x = xx;
						stack[stp].y = yy - 1;
						++stp;
					}
					if (yy + 1 < tmp->height && lavel[yy + 1][xx] == 0xffffffff) {
						stack[stp].x = xx;
						stack[stp].y = yy + 1;
						++stp;
					}
					
					// laveling
					lavel[yy][xx] = lavel_no;
					
					uchar *p_dst = CV_MOVE_TO(pos_dst, xx, yy, 3);
//					p_dst[0] = ((uchar)(0x12345678*lavel_no))/2;
//					p_dst[1] = (uchar)(0x34567812*lavel_no);
//					p_dst[2] = (uchar)(0x56781234*lavel_no);

					++area;
				}

				if (max_area < area) {
					max_area_lavel_no = lavel_no;
					max_area = area;
				}
			}
		}
	}

	area = max_area;

	for (int y = 0; y < tmp->height; y++) {
		for (int x = 0; x < tmp->width; x++) {
			if (lavel[y][x] == max_area_lavel_no) {
				uchar *p_dst = CV_MOVE_TO(pos_dst, x, y, 3);
				uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
				p_dst[0] = p_dst[1] = p_dst[2] = 255;
				green_level += square(p_src[0] - centerH);
			}
		}
	}

	printf("lavel_no %d\n", lavel_no);

#if 1
	printf("green area (0-%llu, higher is larger): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*area/(tmp->height*tmp->width));
	printf("green level (0-%llu, lower is better): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*green_level/area/1000);
#endif
	
	cvReleaseImage(&tmp);
}

int main(int argc, char **argv) {
	IplImage* frame = NULL;
	IplImage* mask = NULL;
	IplImage* dst = NULL;

	int display = 0; // display image
	char *infile = NULL;
	char *outfile = NULL;
	struct stat st;

	int result;

	while ((result = getopt(argc, argv, "di:o:")) != -1) {
		switch (result) {
		case 'd':
			display = 1;
			break;
		case 'i':
			infile = optarg;
			break;
		case 'o':
			outfile = optarg;
			break;
		case ':':
			fprintf(stdout, "%c needs value\n", result);
			break;
		case '?':
			break;
		}
	}
	
	if (argc - optind != 6) {
		fprintf(stdout, "HSV param is not set\n");
		return -1;
	}

	minH = atoi(argv[optind + 0]);
	maxH = atoi(argv[optind + 1]);
	minS = atoi(argv[optind + 2]);
	maxS = atoi(argv[optind + 3]);
	minV = atoi(argv[optind + 4]);
	maxV = atoi(argv[optind + 5]);

	printf("infile: %s\n", infile);
	printf("HSV param: %d-%d %d-%d %d-%d\n", minH, maxH, minS, maxS, minV, maxV);

	// argument check
	if (infile == NULL) {
		fprintf(stderr, "input file is not specified\n");
		return -1;
	}
	if (stat(infile, &st) != 0) {
		fprintf(stderr, "input file not exist\n");
		return -1;
	}

	frame = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	mask = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	dst = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);

	GetMaskHSV(frame, mask);
	
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

	if (outfile) {
		cvAnd(frame, mask, dst);
		cvSaveImage(outfile, dst);
	}

	cvReleaseImage(&frame);
	cvReleaseImage(&dst);
	cvReleaseImage(&mask);

	return(0);
}
