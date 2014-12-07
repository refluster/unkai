#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <linux/videodev2.h>
#include "uvccapture-0.5/v4l2uvc.h"
#include "image_input.h"

image::image(char *file) {
	infile = file;
	capture = NULL;
	frame = NULL;
}

image::~image() {
	cvReleaseImage(&frame);
}

IplImage *image::get_image() {
	if (infile) {
		frame = cvLoadImage(infile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	} else {
		frame = capture_uvc_camera();
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
