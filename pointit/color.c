#include "color.h"

struct hsv_color rgb_to_hsv(struct rgb_color rgb) {
    float r = ((float)(rgb.r) / 255);
    float g = ((float)(rgb.g) / 255);
    float b = ((float)(rgb.b) / 255);

    double h;
    double s;
    double v;
    
    double maxC;
    double minC;
    double delta;

    double dR;
    double dG;
    double dB;

    struct hsv_color hsv;

    maxC = b;
    if (maxC < g) maxC = g;
    if (maxC < r) maxC = r;
    minC = b;
    if (minC > g) minC = g;
    if (minC > r) minC = r;

    v = maxC;
    delta = maxC - minC;

    if (delta == 0) {
        h = 0;
        s = 0;
    } else {
        s = delta / maxC;
        dR = 60*(maxC - r) / delta + 180;
        dG = 60*(maxC - g) / delta + 180;
        dB = 60*(maxC - b) / delta + 180;

        if (r == maxC) {
            h = dB - dG;
        } else if (g == maxC) {
            h = 120 + dR - dB;
        } else {
            h = 240 + dG - dR;
        }

        if (h < 0) {
            h += 360;
        }
        if (h >= 360) {
            h -= 360;
        }

        hsv.h = (int)(h);
        hsv.s = (int)(s * 100);
        hsv.v = (int)(v * 100);

    }

    return hsv;
}
