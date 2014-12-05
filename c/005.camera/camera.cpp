#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#include <list>

using namespace std;

#define square(x) ((x)*(x))

#define KPOC_MEASURE ((unsigned long long)10000)
#define INVALID 0xffffffff

typedef struct {
	int x, y;
} point;

typedef struct {
	uchar H_min, H_max;
	uchar S_min, S_max;
	uchar V_min, V_max;
} HSV_filter;

class image {
	char *infile;
	CvCapture *capture;
	IplImage *frame;

public:
	image(char *infile);
	~image();
	IplImage *getImage();
};

image::image(char *infile) {
	double w = 320, h = 240;

	this->infile = infile;
	capture = NULL;

	// set capture image size
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);
}

image::~image() {
	if (infile) {
		cvReleaseImage(&frame);
	} else {
		cvReleaseCapture(&capture);
	}
}

IplImage *image::getImage() {
	IplImage *frame = 0;

	if (infile) {
		frame = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	} else {
		capture = cvCreateCameraCapture(0);
		frame = cvQueryFrame (capture);
	}
	return frame;
}

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

void GetMaskHSV(IplImage* src, IplImage* mask, HSV_filter *hsv_filter) {
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
			if (hsv_filter->H_min <= H && H <= hsv_filter->H_max &&
				hsv_filter->S_min <= S && S <= hsv_filter->S_max &&
				hsv_filter->V_min <= V && V <= hsv_filter->V_max) {
				lavel[y][x] = INVALID;
			}
		}
	}

	// lavel
	list<uint> lavel_area;
	laveling(lavel, src->width, src->height, &lavel_area);

	int centerH = (hsv_filter->H_min + hsv_filter->H_max) / 2;
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

void measure(char *infile, char *outfile, int display, HSV_filter *hsv_filter) {
	IplImage* frame = NULL;
	IplImage* mask = NULL;
	IplImage* dst = NULL;
	image *image = new class image(infile);

	frame = image->getImage();

	mask = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	dst = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);

	GetMaskHSV(frame, mask, hsv_filter);
	
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

	cvReleaseImage(&dst);
	cvReleaseImage(&mask);

	delete image;
}

int main(int argc, char **argv) {
	int display = 0; // display image
	char *infile = NULL;
	char *outfile = NULL;
	struct stat st;

	HSV_filter hsv_filter;

	int go_res;

	while ((go_res = getopt(argc, argv, "di:o:")) != -1) {
		switch (go_res) {
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
			fprintf(stdout, "%c needs value\n", go_res);
			break;
		case '?':
			break;
		}
	}
	
	if (argc - optind != 6) {
		fprintf(stdout, "HSV param is not set\n");
		return -1;
	}

	if (infile != NULL && stat(infile, &st) != 0) {
		fprintf(stderr, "input file not exist\n");
		return -1;
	}

	hsv_filter.H_min = atoi(argv[optind + 0]);
	hsv_filter.H_max = atoi(argv[optind + 1]);
	hsv_filter.S_min = atoi(argv[optind + 2]);
	hsv_filter.S_max = atoi(argv[optind + 3]);
	hsv_filter.V_min = atoi(argv[optind + 4]);
	hsv_filter.V_max = atoi(argv[optind + 5]);

	printf("infile: %s\n", infile);
	printf("HSV param: %d-%d %d-%d %d-%d\n", hsv_filter.H_min, hsv_filter.H_max,
		   hsv_filter.S_min, hsv_filter.S_max,
		   hsv_filter.V_min, hsv_filter.V_max);

	measure(infile, outfile, display, &hsv_filter);

	return(0);
}
