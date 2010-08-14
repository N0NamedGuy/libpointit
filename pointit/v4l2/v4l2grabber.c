#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "v4l2grabber.h"
#include "../color.h"

int cam_width, cam_height;

int pointit_init_cap(int w, int h) {
    cam_width = w;
    cam_height = h;
   
    return 0;
}

int pointit_destroy_cap(void) {
    return 0;
}


int pointit_capture(void) {
    return 0;
}


struct hsv_color pointit_get_color(int x, int y) {
    struct rgb_color rgb;
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 0;
    return rgb_to_hsv(rgb);
}

void pointit_show_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Show cam!");
}

void pointit_hide_cam(void) {
    /* FIXME: Do me */
    printf("Do me! Hide cam!");
}
