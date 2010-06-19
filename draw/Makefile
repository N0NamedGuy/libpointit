CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=`pkg-config --cflags --libs opencv` `pkg-config --libs sdl` -lSDL_image -lSDL_gfx -lpthread -lm
SOURCES=draw.cpp ../common/thread.cpp ../pointit/pointit.cpp ../common/particle.cpp shapeengine.cpp
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
