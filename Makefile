CC=g++
CFLAGS=-c -Wall -std=c++14 -pedantic -Wpedantic -Ihpp/ -Idependencies
LDFLAGS=

SOURCES=$(wildcard cpp/*.cpp)
OBJECTS=$(SOURCES:cpp/%.cpp=obj/%.o)
EXECUTABLE=pyfe
MAINFILE=main.cpp
MAINOBJ=obj/main.o

all: $(OBJECTS) $(MAINFILE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAINOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(MAINOBJ) -o $@

obj/%.o: cpp/%.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

$(MAINOBJ): $(MAINFILE)
	$(CC) $(CFLAGS) $< -o $@

clean:
	find -name "*.o" -delete