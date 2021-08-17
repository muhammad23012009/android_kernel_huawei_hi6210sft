all:

<<<<<<< HEAD
run_tests:
	@./on-off-test.sh || echo "cpu-hotplug selftests: [FAIL]"
=======
TEST_PROGS := cpu-on-off-test.sh

include ../lib.mk

run_full_test:
	@/bin/bash ./cpu-on-off-test.sh -a || echo "cpu-hotplug selftests: [FAIL]"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

clean:
