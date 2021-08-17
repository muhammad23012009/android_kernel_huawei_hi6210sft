# Makefile for USB tools

CC = $(CROSS_COMPILE)gcc
PTHREAD_LIBS = -lpthread
WARNINGS = -Wall -Wextra
<<<<<<< HEAD
CFLAGS = $(WARNINGS) -g $(PTHREAD_LIBS) -I../include

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^
=======
CFLAGS = $(WARNINGS) -g -I../include
LDFLAGS = $(PTHREAD_LIBS)

all: testusb ffs-test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
	$(RM) testusb ffs-test
