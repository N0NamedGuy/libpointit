CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=-I/usr/include/opencv -lcv -lcvaux -lhighgui -lSDL -lSDL_image -lSDL_gfx -lpthread -lm
SOURCES=draw.cpp ../common/thread.cpp pointit.cpp ../common/particle.cpp shapeengine.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=draw

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: .h.o
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)
