#ifndef CVGRABBER
#define CVGRABBER

#include "../color.h"

extern int pointit_init_cap(int w, int h);
extern int pointit_destroy_cap(void);
extern int pointit_capture(void);
extern struct hsv_color pointit_get_color(int x, int y);

extern void pointit_show_cam(void);
extern void pointit_hide_cam(void);
#endif
