CC=g++
CFLAGS=-c -Wall -std=c++14 -pedantic -Wpedantic -Ihpp/ -IjsonParser/src/
LDFLAGS=

SOURCES=$(wildcard cpp/*.cpp)
OBJECTS=$(SOURCES:cpp/%.cpp=obj/%.o)
EXECUTABLE=readErr
MAINFILE=main.cpp
MAINOBJ=obj/main.o

all: $(SOURCES) $(MAINFILE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAINOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(MAINOBJ) -o $@

pyfe: $(OBJECTS) pyfe.o
	$(CC) $(LDFLAGS) $(OBJECTS) pyfe.o -o $@

pyfe.o: pyfe.cpp
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: cpp/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(MAINOBJ): $(MAINFILE)
	$(CC) $(CFLAGS) $< -o $@

clean:
	find -name "*.o" -delete
