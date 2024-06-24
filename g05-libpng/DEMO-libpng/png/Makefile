GCC = gcc
# GCC = g++
LIBS = -lpng -lm
SOURCES = $(wildcard  *.c)
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE = example


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(GCC) $(OBJECTS) -o $@ $(LIBS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

