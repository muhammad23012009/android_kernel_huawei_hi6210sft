#include "evlist.h"
#include "evsel.h"
#include "thread_map.h"
#include "cpumap.h"
#include "tests.h"

#include <signal.h>

static int exited;
static int nr_exit;

<<<<<<< HEAD
static void sig_handler(int sig)
{
	exited = 1;

	if (sig == SIGUSR1)
		nr_exit = -1;
=======
static void sig_handler(int sig __maybe_unused)
{
	exited = 1;
}

/*
 * perf_evlist__prepare_workload will send a SIGUSR1 if the fork fails, since
 * we asked by setting its exec_error to this handler.
 */
static void workload_exec_failed_signal(int signo __maybe_unused,
					siginfo_t *info __maybe_unused,
					void *ucontext __maybe_unused)
{
	exited	= 1;
	nr_exit = -1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * This test will start a workload that does nothing then it checks
 * if the number of exit event reported by the kernel is 1 or not
 * in order to check the kernel returns correct number of event.
 */
<<<<<<< HEAD
int test__task_exit(void)
=======
int test__task_exit(int subtest __maybe_unused)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int err = -1;
	union perf_event *event;
	struct perf_evsel *evsel;
	struct perf_evlist *evlist;
<<<<<<< HEAD
	struct perf_target target = {
=======
	struct target target = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.uid		= UINT_MAX,
		.uses_mmap	= true,
	};
	const char *argv[] = { "true", NULL };
<<<<<<< HEAD

	signal(SIGCHLD, sig_handler);
	signal(SIGUSR1, sig_handler);

	evlist = perf_evlist__new();
	if (evlist == NULL) {
		pr_debug("perf_evlist__new\n");
		return -1;
	}
	/*
	 * We need at least one evsel in the evlist, use the default
	 * one: "cycles".
	 */
	err = perf_evlist__add_default(evlist);
	if (err < 0) {
		pr_debug("Not enough memory to create evsel\n");
		goto out_free_evlist;
	}
=======
	char sbuf[STRERR_BUFSIZE];
	struct cpu_map *cpus;
	struct thread_map *threads;

	signal(SIGCHLD, sig_handler);

	evlist = perf_evlist__new_default();
	if (evlist == NULL) {
		pr_debug("perf_evlist__new_default\n");
		return -1;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Create maps of threads and cpus to monitor. In this case
	 * we start with all threads and cpus (-1, -1) but then in
	 * perf_evlist__prepare_workload we'll fill in the only thread
	 * we're monitoring, the one forked there.
	 */
<<<<<<< HEAD
	evlist->cpus = cpu_map__dummy_new();
	evlist->threads = thread_map__new_by_tid(-1);
	if (!evlist->cpus || !evlist->threads) {
		err = -ENOMEM;
		pr_debug("Not enough memory to create thread/cpu maps\n");
		goto out_delete_maps;
	}

	err = perf_evlist__prepare_workload(evlist, &target, argv, false, true);
	if (err < 0) {
		pr_debug("Couldn't run the workload!\n");
		goto out_delete_maps;
=======
	cpus = cpu_map__dummy_new();
	threads = thread_map__new_by_tid(-1);
	if (!cpus || !threads) {
		err = -ENOMEM;
		pr_debug("Not enough memory to create thread/cpu maps\n");
		goto out_free_maps;
	}

	perf_evlist__set_maps(evlist, cpus, threads);

	cpus	= NULL;
	threads = NULL;

	err = perf_evlist__prepare_workload(evlist, &target, argv, false,
					    workload_exec_failed_signal);
	if (err < 0) {
		pr_debug("Couldn't run the workload!\n");
		goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	evsel = perf_evlist__first(evlist);
	evsel->attr.task = 1;
<<<<<<< HEAD
	evsel->attr.sample_freq = 0;
=======
	evsel->attr.sample_freq = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	evsel->attr.inherit = 0;
	evsel->attr.watermark = 0;
	evsel->attr.wakeup_events = 1;
	evsel->attr.exclude_kernel = 1;

	err = perf_evlist__open(evlist);
	if (err < 0) {
<<<<<<< HEAD
		pr_debug("Couldn't open the evlist: %s\n", strerror(-err));
		goto out_delete_maps;
=======
		pr_debug("Couldn't open the evlist: %s\n",
			 str_error_r(-err, sbuf, sizeof(sbuf)));
		goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (perf_evlist__mmap(evlist, 128, true) < 0) {
		pr_debug("failed to mmap events: %d (%s)\n", errno,
<<<<<<< HEAD
			 strerror(errno));
		goto out_close_evlist;
=======
			 str_error_r(errno, sbuf, sizeof(sbuf)));
		err = -1;
		goto out_delete_evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	perf_evlist__start_workload(evlist);

retry:
	while ((event = perf_evlist__mmap_read(evlist, 0)) != NULL) {
<<<<<<< HEAD
		if (event->header.type != PERF_RECORD_EXIT)
			continue;

		nr_exit++;
	}

	if (!exited || !nr_exit) {
		poll(evlist->pollfd, evlist->nr_fds, -1);
=======
		if (event->header.type == PERF_RECORD_EXIT)
			nr_exit++;

		perf_evlist__mmap_consume(evlist, 0);
	}

	if (!exited || !nr_exit) {
		perf_evlist__poll(evlist, -1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto retry;
	}

	if (nr_exit != 1) {
		pr_debug("received %d EXIT records\n", nr_exit);
		err = -1;
	}

<<<<<<< HEAD
	perf_evlist__munmap(evlist);
out_close_evlist:
	perf_evlist__close(evlist);
out_delete_maps:
	perf_evlist__delete_maps(evlist);
out_free_evlist:
=======
out_free_maps:
	cpu_map__put(cpus);
	thread_map__put(threads);
out_delete_evlist:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_evlist__delete(evlist);
	return err;
}
