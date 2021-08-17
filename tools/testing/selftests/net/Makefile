# Makefile for net selftests

<<<<<<< HEAD
CC = $(CROSS_COMPILE)gcc
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
CFLAGS = -Wall -O2 -g

CFLAGS += -I../../../../usr/include/

<<<<<<< HEAD
NET_PROGS = socket psock_fanout psock_tpacket
=======
NET_PROGS = socket psock_fanout psock_tpacket reuseport_bpf reuseport_bpf_cpu reuseport_dualstack
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

all: $(NET_PROGS)
%: %.c
	$(CC) $(CFLAGS) -o $@ $^

<<<<<<< HEAD
run_tests: all
	@/bin/sh ./run_netsocktests || echo "sockettests: [FAIL]"
	@/bin/sh ./run_afpackettests || echo "afpackettests: [FAIL]"
=======
TEST_PROGS := run_netsocktests run_afpackettests test_bpf.sh
TEST_FILES := $(NET_PROGS)

include ../lib.mk
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
	$(RM) $(NET_PROGS)
