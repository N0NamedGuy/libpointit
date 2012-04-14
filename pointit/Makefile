# Copyright (C) 2010 David Serrano <david.nonamedguy@gmail.com> 
# 
# This program is free software: you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation, either version 3 of the License, or 
# (at your option) any later version. 
# 
# This program is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
# GNU General Public License for more details. 
LIBNAME=pointit
VERSION=1.0
NUMBER=1
CC=gcc
CFLAGS=-c -Wall -fPIC -DUSING_V4L2 -DPOINTIT_SDL
LDFLAGS=`pkg-config --libs sdl` -DUSING_V4L2
LIBFLAGS=-shared -Wl,-soname,lib$(LIBNAME).$(NUMBER)
SOURCES=pointit.c color.c v4l2/v4l2grabber.c
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) lib$(LIBNAME).$(VERSION).$(NUMBER)

lib$(LIBNAME).$(VERSION).$(NUMBER): $(OBJECTS)
	$(CC) $(LIBFLAGS) -o $@ $(OBJECTS)

.c.o: .h.o
	echo Compiling $@
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@
	echo =======

clean:
	rm *.o v4l2/*.o
