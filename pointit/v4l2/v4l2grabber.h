#ifndef V4L2GRABBER
#define V4L2GRABBER

#include "../color.h"

extern int pointit_init_cap(int w, int h);
extern int pointit_destroy_cap(void);
extern int pointit_capture(void);
extern struct hsv_color pointit_get_color(int x, int y);

extern void pointit_show_cam(void);
extern void pointit_hide_cam(void);

#ifdef POINTIT_SDL
extern SDL_Surface* pointit_sdlcam_surf(void);
#endif
#endif
