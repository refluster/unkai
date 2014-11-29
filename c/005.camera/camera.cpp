#include <cv.h>
#include <highgui.h>
#include <iostream>

#define square(x) ((x)*(x))

uchar minH, minS, minV, maxH, maxS, maxV;
unsigned int area;
unsigned long long green_point;

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
	green_point = 0;
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
				green_point += square(H - centerH) +
					square(S - centerS) +
					square(V - centerS);
			} else {
				p_dst[0] = p_dst[1] = p_dst[2] = 0;
			}
		}
	}

	if(erosions > 0)  cvErode(mask, mask, 0, erosions);
	if(dilations > 0) cvDilate(mask, mask, 0, dilations);

	cout << "green area (1-1000, higher is larger): "
		 << (unsigned long long)1000*area/(tmp->height*tmp->width) << endl;
	cout << "green level (1-1000, lower is better): "
		 << 1000*green_level/area/(255*255*3) << endl;
	
	cvReleaseImage(&tmp);
}

int main(int argc, char **argv) {
	IplImage* frame = 0;
	IplImage* mask = 0;
	IplImage* dst = 0;

	frame = cvLoadImage(argv[1], CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvNamedWindow("src", CV_WINDOW_AUTOSIZE);
	cvShowImage("src", frame);
	cvWaitKey(0);
	cvDestroyWindow("src");

	minH = atoi(argv[2]);
	maxH = atoi(argv[3]);
	minS = atoi(argv[4]);
	maxS = atoi(argv[5]);
	minV = atoi(argv[6]);
	maxV = atoi(argv[7]);

	mask = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	dst = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);

	GetMaskHSV(frame, mask, 1, 1);
	cvAnd(frame, mask, dst);
	
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	cvShowImage("dst", dst);
	cvWaitKey(0);
	cvDestroyWindow("dst");

    cvSaveImage("output.jpg", dst);
	
	cvReleaseImage(&frame);
	cvReleaseImage(&dst);
	cvReleaseImage(&mask);

	return(0);
}

