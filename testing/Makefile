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
CC=gcc
CFLAGS=-c -Wall -ggdb
LDFLAGS=-I../pointit -L../pointit -lpointit `pkg-config --libs sdl` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -lpthread
SOURCES=testing.c 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=testing

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: .h.o
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)
