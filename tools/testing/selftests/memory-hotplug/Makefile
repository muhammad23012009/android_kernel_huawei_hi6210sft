all:

<<<<<<< HEAD
run_tests:
	@./on-off-test.sh || echo "memory-hotplug selftests: [FAIL]"
=======
include ../lib.mk

TEST_PROGS := mem-on-off-test.sh
override RUN_TESTS := ./mem-on-off-test.sh -r 2 || echo "selftests: memory-hotplug [FAIL]"
override EMIT_TESTS := echo "$(RUN_TESTS)"

run_full_test:
	@/bin/bash ./mem-on-off-test.sh || echo "memory-hotplug selftests: [FAIL]"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
