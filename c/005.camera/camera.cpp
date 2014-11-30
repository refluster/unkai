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

void GetMaskHSV(IplImage* src, IplImage* mask, int erosions, int dilations) {
	int x = 0, y = 0;
	uchar H, S, V;

	CvPixelPosition8u pos_src, pos_dst;
	IplImage* tmp;

	tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);

	cvCvtColor(src, tmp, CV_RGB2HSV);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
					   tmp->widthStep,cvGetSize(tmp), x, y, tmp->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
					   mask->widthStep, cvGetSize(mask), x, y, mask->origin);

	area = 0;
	green_level = 0;
	int centerH = (minH + maxH) / 2;
	int centerS = (minS + maxS) / 2;
	int centerV = (minV + maxV) / 2;

	for(y = 0; y < tmp->height; y++) {
		for(x = 0; x < tmp->width; x++) {
			uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
			uchar *p_dst = CV_MOVE_TO(pos_dst, x, y, 3);

			H = p_src[0];
			S = p_src[1];
			V = p_src[2];

			if (minH <= H && H <= maxH &&
				minS <= S && S <= maxS &&
				minV <= V && V <= maxV) {
				p_dst[0] = p_dst[1] = p_dst[2] = 255;

				++ area;
				green_level += square(H - centerH) +
					square(S - centerS) +
					square(V - centerS);
			} else {
				p_dst[0] = p_dst[1] = p_dst[2] = 0;
			}
		}
	}

	if(erosions > 0)  cvErode(mask, mask, 0, erosions);
	if(dilations > 0) cvDilate(mask, mask, 0, dilations);

	printf("green area (0-%llu, higher is larger): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*area/(tmp->height*tmp->width));
	printf("green level (0-%llu, lower is better): %llu\n",
		   KPOC_MEASURE,
		   KPOC_MEASURE*green_level/area/(255*255*3));
	
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

	GetMaskHSV(frame, mask, 1, 1);
	
	if (display) {
		cvAnd(frame, mask, dst);

		// original
		cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
		cvShowImage("src", frame);
		
		// mask
		cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
		cvShowImage("mask", mask);

		// masked
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
