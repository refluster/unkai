#include <cv.h>
#include <highgui.h>
#include <stdio.h>

uchar minH, minS, minV, maxH, maxS, maxV;

void GetMaskHSV(IplImage* src, IplImage* mask, int erosions, int dilations) {
	int x = 0, y = 0;
	uchar H, S, V;

	CvPixelPosition8u pos_src, pos_dst;
	IplImage* tmp;

	tmp = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);

	//HSVに変換
	cvCvtColor(src, tmp, CV_RGB2HSV);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) tmp->imageData,
					   tmp->widthStep,cvGetSize(tmp), x, y, tmp->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) mask->imageData,
					   mask->widthStep, cvGetSize(mask), x, y, mask->origin);

#if 0
	minH = 70/2;	maxH = 160/2;
	minS = 80;	maxS = 255;
	minV = 120;	maxV = 255;
#endif

	for(y = 0; y < tmp->height; y++) {
		for(x = 0; x < tmp->width; x++) {
			uchar *p_src = CV_MOVE_TO(pos_src, x, y, 3);
			uchar *p_dst = CV_MOVE_TO(pos_dst, x, y, 3);

			H = p_src[0];	//0から180
			S = p_src[1];
			V = p_src[2];

			if (minH <= H && H <= maxH &&
				minS <= S && S <= maxS &&
				minV <= V && V <= maxV) {
				p_dst[0] = p_dst[1] = p_dst[2] = 255;
			} else {
				p_dst[0] = p_dst[1] = p_dst[2] = 0;
			}
		}
	}
	
	if(erosions > 0)  cvErode(mask, mask, 0, erosions);
	if(dilations > 0) cvDilate(mask, mask, 0, dilations);
	
	cvReleaseImage(&tmp);
}

int main(int argc, char **argv)
{
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
