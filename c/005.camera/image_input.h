#ifndef _IMAGE_INPUT_H_
#define _IMAGE_INPUT_H_

typedef struct {
	int x, y;
} point;

typedef struct {
	uchar h_min;
	uchar h_max;
	uchar s_min;
	uchar s_max;
	uchar v_min;
	uchar v_max;
} hsv_filter;

typedef struct {
	int brightness;
	int contrast;
	int saturation;
	int gain;
	int width;
	int height;
} camera_property;	

class image_input {
	char *infile;
	IplImage *frame;
	camera_property	camera;

public:
	image_input(char *infile);
	image_input(camera_property *cp);
	~image_input();
	IplImage *get_image();
	void convert_yuyv_to_rgb(const unsigned char *yuyv, unsigned char *bgr, int width, int height);
	IplImage *capture_uvc_camera();
};

#endif

