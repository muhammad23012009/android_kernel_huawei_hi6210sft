# Makefile for vm selftests

<<<<<<< HEAD
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall

all: hugepage-mmap hugepage-shm  map_hugetlb thuge-gen
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

run_tests: all
	@/bin/sh ./run_vmtests || echo "vmtests: [FAIL]"

clean:
	$(RM) hugepage-mmap hugepage-shm  map_hugetlb
=======
ifndef OUTPUT
  OUTPUT := $(shell pwd)
endif

CFLAGS = -Wall -I ../../../../usr/include $(EXTRA_CFLAGS)
BINARIES = compaction_test
BINARIES += hugepage-mmap
BINARIES += hugepage-shm
BINARIES += map_hugetlb
BINARIES += mlock2-tests
BINARIES += on-fault-limit
BINARIES += thuge-gen
BINARIES += transhuge-stress
BINARIES += userfaultfd
BINARIES += mlock-random-test

all: $(BINARIES)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lrt
userfaultfd: userfaultfd.c ../../../../usr/include/linux/kernel.h
	$(CC) $(CFLAGS) -O2 -o $@ $< -lpthread

mlock-random-test: mlock-random-test.c
	$(CC) $(CFLAGS) -o $@ $< -lcap

../../../../usr/include/linux/kernel.h:
	make -C ../../../.. headers_install

TEST_PROGS := run_vmtests
TEST_FILES := $(BINARIES)

include ../lib.mk

clean:
	$(RM) $(BINARIES)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
