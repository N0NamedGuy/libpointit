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
#include "thread.h"
#include "../pointit/pointit.h"
pthread_t detect_thread;

bool *proceed_ptr;

void *go_detect_thread(void* ptr) {
  //  int cnt = 0;
  //while (!quit) printf("Iter:%d\n",cnt++);

  while(*proceed_ptr)
    pointit_detect((struct pointit_context*)ptr);

  return 0;
}

void init_thread(pointit_context* pntIt, bool* proceed_flag) {
  proceed_ptr = proceed_flag;
  pthread_create( &detect_thread, NULL, go_detect_thread, pntIt);
}

void thread_wait() {
  *proceed_ptr = false;
  pthread_join( detect_thread, NULL);
}
