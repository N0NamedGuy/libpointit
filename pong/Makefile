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
CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=`pkg-config --cflags --libs opencv` -lcv -lcvaux -lhighgui `pkg-config --libs sdl` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -pthread
SOURCES=globals.cpp init.cpp ../common/thread.cpp audio.cpp pong.cpp pad.cpp ball.cpp ../pointit/pointit.cpp utils.cpp objs.cpp ../common/particle.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=pong

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: .h.o
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)
