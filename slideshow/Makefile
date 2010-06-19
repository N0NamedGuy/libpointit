CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=-I/usr/include/opencv -lcv -lcvaux -lhighgui -lSDL -lSDL_image -lSDL_gfx -lpthread
SOURCES=slideshow.cpp thread.cpp pointit.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=slideshow

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o: .h.o
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)