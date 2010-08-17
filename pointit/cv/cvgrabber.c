#ifndef CVGRABBER
#define CVGRABBER

#include "cv.h"
#include "highgui.h"

#include "../color.h"

CvCapture* capture;
IplImage* frame;

int pointit_init_cap(int w, int h) {
    capture = cvCaptureFromCAM(CV_CAP_ANY);

    if (!capture) return -1;

    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, h);
    
    frame = cvQueryFrame(capture);

    if (!frame) return -1;

    return 0;
}

int pointit_destroy_cap(void) {
    cvReleaseImage(frame);
    cvReleaseCapture(&capture);    

    return 0;
}

int pointit_capture(void) {
    IplImage* tmp;
    
    cvReleaseImage(frame);
    
    tmp = cvQueryFrame(capture);
    frame= cvCloneImage(tmp);
    cvFlip(tmp, frame, 1);

    cvReleaseImage(tmp);
    
    return 0;
};

struct hsv_color pointit_get_color(int x, int y) {
    struct rgb_color rgb;
    rgb.b = ((uchar *)(frame->imageData + y*frame->widthStep))[x*frame->nChannels + 0];
    rgb.g = ((uchar *)(frame->imageData + y*frame->widthStep))[x*frame->nChannels + 1];
    rgb.r = ((uchar *)(frame->imageData + y*frame->widthStep))[x*frame->nChannels + 2];

    return rgb_to_hsv(rgb);
}

void pointit_show_cam(void) {
    // TODO: Me
}

void pointit_hide_cam(void) {
    // TODO: Me
}

#endif
