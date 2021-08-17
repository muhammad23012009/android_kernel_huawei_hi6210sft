<<<<<<< HEAD
=======
/* For the CLR_() macros */
#include <pthread.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "evlist.h"
#include "evsel.h"
#include "thread_map.h"
#include "cpumap.h"
#include "tests.h"
<<<<<<< HEAD
=======
#include <linux/err.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * This test will generate random numbers of calls to some getpid syscalls,
 * then establish an mmap for a group of events that are created to monitor
 * the syscalls.
 *
 * It will receive the events, using mmap, use its PERF_SAMPLE_ID generated
 * sample.id field to map back to its respective perf_evsel instance.
 *
 * Then it checks if the number of syscalls reported as perf events by
 * the kernel corresponds to the number of syscalls made.
 */
<<<<<<< HEAD
int test__basic_mmap(void)
=======
int test__basic_mmap(int subtest __maybe_unused)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int err = -1;
	union perf_event *event;
	struct thread_map *threads;
	struct cpu_map *cpus;
	struct perf_evlist *evlist;
	cpu_set_t cpu_set;
<<<<<<< HEAD
	const char *syscall_names[] = { "getsid", "getppid", "getpgrp",
					"getpgid", };
	pid_t (*syscalls[])(void) = { (void *)getsid, getppid, getpgrp,
				      (void*)getpgid };
=======
	const char *syscall_names[] = { "getsid", "getppid", "getpgid", };
	pid_t (*syscalls[])(void) = { (void *)getsid, getppid, (void*)getpgid };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define nsyscalls ARRAY_SIZE(syscall_names)
	unsigned int nr_events[nsyscalls],
		     expected_nr_events[nsyscalls], i, j;
	struct perf_evsel *evsels[nsyscalls], *evsel;
<<<<<<< HEAD
=======
	char sbuf[STRERR_BUFSIZE];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	threads = thread_map__new(-1, getpid(), UINT_MAX);
	if (threads == NULL) {
		pr_debug("thread_map__new\n");
		return -1;
	}

	cpus = cpu_map__new(NULL);
	if (cpus == NULL) {
		pr_debug("cpu_map__new\n");
		goto out_free_threads;
	}

	CPU_ZERO(&cpu_set);
	CPU_SET(cpus->map[0], &cpu_set);
	sched_setaffinity(0, sizeof(cpu_set), &cpu_set);
	if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) < 0) {
		pr_debug("sched_setaffinity() failed on CPU %d: %s ",
<<<<<<< HEAD
			 cpus->map[0], strerror(errno));
=======
			 cpus->map[0], str_error_r(errno, sbuf, sizeof(sbuf)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out_free_cpus;
	}

	evlist = perf_evlist__new();
	if (evlist == NULL) {
		pr_debug("perf_evlist__new\n");
		goto out_free_cpus;
	}

	perf_evlist__set_maps(evlist, cpus, threads);

	for (i = 0; i < nsyscalls; ++i) {
		char name[64];

		snprintf(name, sizeof(name), "sys_enter_%s", syscall_names[i]);
<<<<<<< HEAD
		evsels[i] = perf_evsel__newtp("syscalls", name, i);
		if (evsels[i] == NULL) {
			pr_debug("perf_evsel__new\n");
			goto out_free_evlist;
		}

		evsels[i]->attr.wakeup_events = 1;
		perf_evsel__set_sample_id(evsels[i]);
=======
		evsels[i] = perf_evsel__newtp("syscalls", name);
		if (IS_ERR(evsels[i])) {
			pr_debug("perf_evsel__new\n");
			goto out_delete_evlist;
		}

		evsels[i]->attr.wakeup_events = 1;
		perf_evsel__set_sample_id(evsels[i], false);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		perf_evlist__add(evlist, evsels[i]);

		if (perf_evsel__open(evsels[i], cpus, threads) < 0) {
			pr_debug("failed to open counter: %s, "
				 "tweak /proc/sys/kernel/perf_event_paranoid?\n",
<<<<<<< HEAD
				 strerror(errno));
			goto out_close_fd;
=======
				 str_error_r(errno, sbuf, sizeof(sbuf)));
			goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		nr_events[i] = 0;
		expected_nr_events[i] = 1 + rand() % 127;
	}

	if (perf_evlist__mmap(evlist, 128, true) < 0) {
		pr_debug("failed to mmap events: %d (%s)\n", errno,
<<<<<<< HEAD
			 strerror(errno));
		goto out_close_fd;
=======
			 str_error_r(errno, sbuf, sizeof(sbuf)));
		goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	for (i = 0; i < nsyscalls; ++i)
		for (j = 0; j < expected_nr_events[i]; ++j) {
			int foo = syscalls[i]();
			++foo;
		}

	while ((event = perf_evlist__mmap_read(evlist, 0)) != NULL) {
		struct perf_sample sample;

		if (event->header.type != PERF_RECORD_SAMPLE) {
			pr_debug("unexpected %s event\n",
				 perf_event__name(event->header.type));
<<<<<<< HEAD
			goto out_munmap;
=======
			goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		err = perf_evlist__parse_sample(evlist, event, &sample);
		if (err) {
			pr_err("Can't parse sample, err = %d\n", err);
<<<<<<< HEAD
			goto out_munmap;
=======
			goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		err = -1;
		evsel = perf_evlist__id2evsel(evlist, sample.id);
		if (evsel == NULL) {
			pr_debug("event with id %" PRIu64
				 " doesn't map to an evsel\n", sample.id);
<<<<<<< HEAD
			goto out_munmap;
		}
		nr_events[evsel->idx]++;
	}

	err = 0;
	list_for_each_entry(evsel, &evlist->entries, node) {
=======
			goto out_delete_evlist;
		}
		nr_events[evsel->idx]++;
		perf_evlist__mmap_consume(evlist, 0);
	}

	err = 0;
	evlist__for_each_entry(evlist, evsel) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (nr_events[evsel->idx] != expected_nr_events[evsel->idx]) {
			pr_debug("expected %d %s events, got %d\n",
				 expected_nr_events[evsel->idx],
				 perf_evsel__name(evsel), nr_events[evsel->idx]);
			err = -1;
<<<<<<< HEAD
			goto out_munmap;
		}
	}

out_munmap:
	perf_evlist__munmap(evlist);
out_close_fd:
	for (i = 0; i < nsyscalls; ++i)
		perf_evsel__close_fd(evsels[i], 1, threads->nr);
out_free_evlist:
	perf_evlist__delete(evlist);
out_free_cpus:
	cpu_map__delete(cpus);
out_free_threads:
	thread_map__delete(threads);
=======
			goto out_delete_evlist;
		}
	}

out_delete_evlist:
	perf_evlist__delete(evlist);
	cpus	= NULL;
	threads = NULL;
out_free_cpus:
	cpu_map__put(cpus);
out_free_threads:
	thread_map__put(threads);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return err;
}
