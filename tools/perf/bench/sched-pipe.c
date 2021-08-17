/*
 *
 * sched-pipe.c
 *
 * pipe: Benchmark for pipe()
 *
 * Based on pipe-test-1m.c by Ingo Molnar <mingo@redhat.com>
 *  http://people.redhat.com/mingo/cfs-scheduler/tools/pipe-test-1m.c
 * Ported to perf by Hitoshi Mitake <mitake@dcl.info.waseda.ac.jp>
<<<<<<< HEAD
 *
 */

#include "../perf.h"
#include "../util/util.h"
#include "../util/parse-options.h"
=======
 */
#include "../perf.h"
#include "../util/util.h"
#include <subcmd/parse-options.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "../builtin.h"
#include "bench.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
<<<<<<< HEAD
#include <linux/unistd.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
<<<<<<< HEAD

#define LOOPS_DEFAULT 1000000
static int loops = LOOPS_DEFAULT;

static const struct option options[] = {
	OPT_INTEGER('l', "loop", &loops,
		    "Specify number of loops"),
=======
#include <sys/syscall.h>
#include <linux/time64.h>

#include <pthread.h>

struct thread_data {
	int			nr;
	int			pipe_read;
	int			pipe_write;
	pthread_t		pthread;
};

#define LOOPS_DEFAULT 1000000
static	int			loops = LOOPS_DEFAULT;

/* Use processes by default: */
static bool			threaded;

static const struct option options[] = {
	OPT_INTEGER('l', "loop",	&loops,		"Specify number of loops"),
	OPT_BOOLEAN('T', "threaded",	&threaded,	"Specify threads/process based task setup"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_END()
};

static const char * const bench_sched_pipe_usage[] = {
	"perf bench sched pipe <options>",
	NULL
};

<<<<<<< HEAD
int bench_sched_pipe(int argc, const char **argv,
		     const char *prefix __maybe_unused)
{
	int pipe_1[2], pipe_2[2];
	int m = 0, i;
	struct timeval start, stop, diff;
	unsigned long long result_usec = 0;
=======
static void *worker_thread(void *__tdata)
{
	struct thread_data *td = __tdata;
	int m = 0, i;
	int ret;

	for (i = 0; i < loops; i++) {
		if (!td->nr) {
			ret = read(td->pipe_read, &m, sizeof(int));
			BUG_ON(ret != sizeof(int));
			ret = write(td->pipe_write, &m, sizeof(int));
			BUG_ON(ret != sizeof(int));
		} else {
			ret = write(td->pipe_write, &m, sizeof(int));
			BUG_ON(ret != sizeof(int));
			ret = read(td->pipe_read, &m, sizeof(int));
			BUG_ON(ret != sizeof(int));
		}
	}

	return NULL;
}

int bench_sched_pipe(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct thread_data threads[2], *td;
	int pipe_1[2], pipe_2[2];
	struct timeval start, stop, diff;
	unsigned long long result_usec = 0;
	int nr_threads = 2;
	int t;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * why does "ret" exist?
	 * discarding returned value of read(), write()
	 * causes error in building environment for perf
	 */
	int __maybe_unused ret, wait_stat;
	pid_t pid, retpid __maybe_unused;

<<<<<<< HEAD
	argc = parse_options(argc, argv, options,
			     bench_sched_pipe_usage, 0);
=======
	argc = parse_options(argc, argv, options, bench_sched_pipe_usage, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	BUG_ON(pipe(pipe_1));
	BUG_ON(pipe(pipe_2));

<<<<<<< HEAD
	pid = fork();
	assert(pid >= 0);

	gettimeofday(&start, NULL);

	if (!pid) {
		for (i = 0; i < loops; i++) {
			ret = read(pipe_1[0], &m, sizeof(int));
			ret = write(pipe_2[1], &m, sizeof(int));
		}
	} else {
		for (i = 0; i < loops; i++) {
			ret = write(pipe_1[1], &m, sizeof(int));
			ret = read(pipe_2[0], &m, sizeof(int));
		}
	}

	gettimeofday(&stop, NULL);
	timersub(&stop, &start, &diff);

	if (pid) {
		retpid = waitpid(pid, &wait_stat, 0);
		assert((retpid == pid) && WIFEXITED(wait_stat));
	} else {
		exit(0);
	}

	switch (bench_format) {
	case BENCH_FORMAT_DEFAULT:
		printf("# Executed %d pipe operations between two tasks\n\n",
			loops);

		result_usec = diff.tv_sec * 1000000;
=======
	gettimeofday(&start, NULL);

	for (t = 0; t < nr_threads; t++) {
		td = threads + t;

		td->nr = t;

		if (t == 0) {
			td->pipe_read = pipe_1[0];
			td->pipe_write = pipe_2[1];
		} else {
			td->pipe_write = pipe_1[1];
			td->pipe_read = pipe_2[0];
		}
	}


	if (threaded) {

		for (t = 0; t < nr_threads; t++) {
			td = threads + t;

			ret = pthread_create(&td->pthread, NULL, worker_thread, td);
			BUG_ON(ret);
		}

		for (t = 0; t < nr_threads; t++) {
			td = threads + t;

			ret = pthread_join(td->pthread, NULL);
			BUG_ON(ret);
		}

	} else {
		pid = fork();
		assert(pid >= 0);

		if (!pid) {
			worker_thread(threads + 0);
			exit(0);
		} else {
			worker_thread(threads + 1);
		}

		retpid = waitpid(pid, &wait_stat, 0);
		assert((retpid == pid) && WIFEXITED(wait_stat));
	}

	gettimeofday(&stop, NULL);
	timersub(&stop, &start, &diff);

	switch (bench_format) {
	case BENCH_FORMAT_DEFAULT:
		printf("# Executed %d pipe operations between two %s\n\n",
			loops, threaded ? "threads" : "processes");

		result_usec = diff.tv_sec * USEC_PER_SEC;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		result_usec += diff.tv_usec;

		printf(" %14s: %lu.%03lu [sec]\n\n", "Total time",
		       diff.tv_sec,
<<<<<<< HEAD
		       (unsigned long) (diff.tv_usec/1000));
=======
		       (unsigned long) (diff.tv_usec / USEC_PER_MSEC));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		printf(" %14lf usecs/op\n",
		       (double)result_usec / (double)loops);
		printf(" %14d ops/sec\n",
		       (int)((double)loops /
<<<<<<< HEAD
			     ((double)result_usec / (double)1000000)));
=======
			     ((double)result_usec / (double)USEC_PER_SEC)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	case BENCH_FORMAT_SIMPLE:
		printf("%lu.%03lu\n",
		       diff.tv_sec,
<<<<<<< HEAD
		       (unsigned long) (diff.tv_usec / 1000));
=======
		       (unsigned long) (diff.tv_usec / USEC_PER_MSEC));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	default:
		/* reaching here is something disaster */
		fprintf(stderr, "Unknown format:%d\n", bench_format);
		exit(1);
		break;
	}

	return 0;
}
