CC=g++
CFLAGS=-c -Wall -std=c++14 -pedantic -Wpedantic -Ihpp/
LDFLAGS=
# FILES=$(wildcard cpp/*.cpp)
# FILES=main.cpp interpreteErrors.cpp parseErrorOutput.cpp pyCode.cpp pyFile.cpp
# SOURCES=$(addprefix cpp/, $(FILES))
SOURCES=$(wildcard cpp/*.cpp)
OBJECTS=$(SOURCES:cpp/%.cpp=obj/%.o)
EXECUTABLE=readErr
MAINFILE=main.cpp
MAINOBJ=obj/main.o

all: $(SOURCES) $(MAINFILE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAINOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(MAINOBJ) -o $@

obj/%.o: cpp/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(MAINOBJ): $(MAINFILE)
	$(CC) $(CFLAGS) $< -o $@

clean:
	find -name "*.o" -delete
