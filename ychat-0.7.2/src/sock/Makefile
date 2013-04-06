#CC=gcc33
CC=g++
INCLUDE=
DEBUG=-D_GNU_SOURCE
#CFLAGS=-Wall -O3 -s $(DEBUG) $(INCLUDE)
#CFLAGS=-Wall -g -ansi -pedantic $(DEBUG) $(INCLUDE)
CFLAGS=-Wall -g $(DEBUG) $(INCLUDE)
LIBS=-lstdc++

OBJECTS=main-test.o tcp-client.o exception.o
PROGRAM=main-test

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(PROGRAM) $(OBJECTS)
						
