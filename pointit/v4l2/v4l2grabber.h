#ifndef
#define v4lgrabber.h

#include "../color.h"

extern static int pointit_init_cap(int w, int h);
extern static int pointit_destroy_cap(void);
extern static int pointit_capture(void);
extern static hsv_color pointit_color(int x, int y);
#endif
