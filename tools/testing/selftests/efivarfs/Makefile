<<<<<<< HEAD
CC = $(CROSS_COMPILE)gcc
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
CFLAGS = -Wall

test_objs = open-unlink create-read

all: $(test_objs)

<<<<<<< HEAD
run_tests: all
	@/bin/bash ./efivarfs.sh || echo "efivarfs selftests: [FAIL]"
=======
TEST_PROGS := efivarfs.sh
TEST_FILES := $(test_objs)

include ../lib.mk
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
	rm -f $(test_objs)
