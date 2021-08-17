uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/i386/)
ifeq ($(ARCH),i386)
<<<<<<< HEAD
        ARCH := X86
	CFLAGS := -DCONFIG_X86_32 -D__i386__
endif
ifeq ($(ARCH),x86_64)
	ARCH := X86
=======
        ARCH := x86
	CFLAGS := -DCONFIG_X86_32 -D__i386__
endif
ifeq ($(ARCH),x86_64)
	ARCH := x86
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	CFLAGS := -DCONFIG_X86_64 -D__x86_64__
endif

CFLAGS += -I../../../../usr/include/

all:
<<<<<<< HEAD
ifeq ($(ARCH),X86)
	gcc $(CFLAGS) msgque.c -o msgque_test
else
	echo "Not an x86 target, can't build msgque selftest"
endif

run_tests: all
	./msgque_test
=======
	$(CC) $(CFLAGS) msgque.c -o msgque_test

TEST_PROGS := msgque_test

include ../lib.mk
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
	rm -fr ./msgque_test
