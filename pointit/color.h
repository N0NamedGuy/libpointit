#ifndef COLOR_H
#define COLOR_H

struct hsv_color {
    int h;
    int s;
    int v;
};

struct rgb_color {
    int r;
    int g;
    int b;
};

extern struct hsv_color rgb_to_hsv(struct rgb_color rgb);
#endif
