CC=gcc
CFLAGS= -g -Wall -O2
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
SUFFIXES=.c.o
TARGET=shell

all: $(OBJS)
	$(CC) -o $(TARGET) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)

dep:
	makedepend $(SRCS) -Y
# DO NOT DELETE
