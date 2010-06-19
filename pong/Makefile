CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=-I/usr/include/opencv -lcv -lcvaux -lhighgui -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -pthread
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
