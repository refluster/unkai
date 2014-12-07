#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#include <list>
#include <linux/videodev2.h>
#include "uvccapture-0.5/v4l2uvc.h"

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
	IplImage *get_image();
	void convert_yuyv_to_rgb(const unsigned char *yuyv, unsigned char *bgr, int width, int height);
	IplImage *capture_uvc_camera();
};

image::image(char *file) {
	infile = file;
	capture = NULL;
	frame = NULL;
}

image::~image() {
	cvReleaseImage(&frame);
}

IplImage *image::getImage() {
	double w = 320, h = 240;

	if (infile) {
		frame = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	} else {
#if 0
		capture = cvCreateCameraCapture(-1);
		// set capture image size
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);

		printf("capture : %p\n", capture);
		frame = cvQueryFrame (capture);
		printf("frame : %p\n", frame);
#else
		frame = capture_uvc();
#endif
	}
	return frame;
}

void image::convert_yuyv_to_rgb(const unsigned char *yuyv, unsigned char *bgr, int width, int height) {
	// convert yuyv to rgb
	int z = 0;
	int x;
	int yline;
 
	for (yline = 0; yline < height; yline++){
		for (x = 0; x < width; x++){
			int r, g, b;
			int y, u, v;
 
			if (!z)
				y = yuyv[0] << 8;
			else
				y = yuyv[2] << 8;
			u = yuyv[1] - 128;
			v = yuyv[3] - 128;
 
			r = (y + (359 * v)) >> 8;
			g = (y - (88 * u) - (183 * v)) >> 8;
			b = (y + (454 * u)) >> 8;
 
			*(bgr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);
			*(bgr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
			*(bgr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
 
			if (z++) {
				z = 0;
				yuyv += 4;
			}
		}
	}
}

IplImage *image::capture_uvc_camera() {
	int brightness = 20;
	int contrast = 60;
	int saturation = 40;
	int gain = 80;
	struct vdIn *videoIn;
	char *videodevice = (char*)"/dev/video0";
	int width = 320;
	int height = 240;
	int format = V4L2_PIX_FMT_YUYV;
	int grabmethod = 1;
	IplImage* frame;

	videoIn = (struct vdIn *) calloc (1, sizeof (struct vdIn));
	if (init_videoIn(videoIn, (char *) videodevice, width, height, format, grabmethod) < 0) {
		puts("init failed");
		exit (1);
	}

	//Reset all camera controls
	v4l2ResetControl (videoIn, V4L2_CID_BRIGHTNESS);
	v4l2ResetControl (videoIn, V4L2_CID_CONTRAST);
	v4l2ResetControl (videoIn, V4L2_CID_SATURATION);
	v4l2ResetControl (videoIn, V4L2_CID_GAIN);

	//Setup Camera Parameters
	v4l2SetControl (videoIn, V4L2_CID_BRIGHTNESS, brightness);
	v4l2SetControl (videoIn, V4L2_CID_CONTRAST, contrast);
	v4l2SetControl (videoIn, V4L2_CID_SATURATION, saturation);
	v4l2SetControl (videoIn, V4L2_CID_GAIN, gain);

	if (uvcGrab (videoIn) < 0) {
		fprintf (stderr, "Error grabbing\n");
		close_v4l2 (videoIn);
		free (videoIn);
		exit (1);
	}

	// IplImage
	frame = cvCreateImage(cvSize(videoIn->width, videoIn->height), IPL_DEPTH_8U, 3);	

	// convert image format
	convert_yuyv_to_rgb(videoIn->framebuffer, (unsigned char*)frame->imageData,
						videoIn->width, videoIn->height);

	cvSaveImage("src.jpg", frame);

	close_v4l2 (videoIn);
	free (videoIn);

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
