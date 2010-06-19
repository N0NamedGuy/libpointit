// Copyright (C) 2010 David Serrano <david.nonamedguy@gmail.com> 
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include "../pointit/pointit.h"

extern void *go_detect_thread(void* ptr);

/* We need to pass the current point it instance */
extern void init_thread(PointIt* pntIt, bool* quit_flag);
extern void thread_wait();
#endif
