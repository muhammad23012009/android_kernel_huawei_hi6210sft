<<<<<<< HEAD
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/i386/)
ifeq ($(ARCH),i386)
        ARCH := X86
	CFLAGS := -DCONFIG_X86_32 -D__i386__
endif
ifeq ($(ARCH),x86_64)
	ARCH := X86
	CFLAGS := -DCONFIG_X86_64 -D__x86_64__
endif

CFLAGS += -I../../../../arch/x86/include/generated/
CFLAGS += -I../../../../include/
CFLAGS += -I../../../../usr/include/
CFLAGS += -I../../../../arch/x86/include/

all:
ifeq ($(ARCH),X86)
	gcc $(CFLAGS) kcmp_test.c -o kcmp_test
else
	echo "Not an x86 target, can't build kcmp selftest"
endif

run_tests: all
	@./kcmp_test || echo "kcmp_test: [FAIL]"

clean:
	rm -fr ./run_test
	rm -fr ./test-file
=======
CFLAGS += -I../../../../usr/include/

all: kcmp_test

TEST_PROGS := kcmp_test

include ../lib.mk

clean:
	$(RM) kcmp_test kcmp-test-file
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
