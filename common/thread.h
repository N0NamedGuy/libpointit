#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include "pointit.h"

extern void *go_detect_thread(void* ptr);

/* We need to pass the current point it instance */
extern void init_thread(PointIt* pntIt, bool* quit_flag);
extern void thread_wait();
#endif
