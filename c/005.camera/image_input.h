#ifndef _IMAGE_INPUT_H_
#define _IMAGE_INPUT_H_

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

#endif

