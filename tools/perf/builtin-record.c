/*
 * builtin-record.c
 *
 * Builtin record command: Record the profile of a workload
 * (or a CPU, or a PID) into the perf.data output file - for
 * later analysis via perf report.
 */
#include "builtin.h"

#include "perf.h"

#include "util/build-id.h"
#include "util/util.h"
<<<<<<< HEAD
#include "util/parse-options.h"
#include "util/parse-events.h"

=======
#include <subcmd/parse-options.h>
#include "util/parse-events.h"
#include "util/config.h"

#include "util/callchain.h"
#include "util/cgroup.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "util/header.h"
#include "util/event.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/debug.h"
<<<<<<< HEAD
=======
#include "util/drv_configs.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "util/session.h"
#include "util/tool.h"
#include "util/symbol.h"
#include "util/cpumap.h"
#include "util/thread_map.h"
<<<<<<< HEAD
=======
#include "util/data.h"
#include "util/perf_regs.h"
#include "util/auxtrace.h"
#include "util/tsc.h"
#include "util/parse-branch-options.h"
#include "util/parse-regs-options.h"
#include "util/llvm-utils.h"
#include "util/bpf-loader.h"
#include "util/trigger.h"
#include "asm/bug.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>
<<<<<<< HEAD

#ifndef HAVE_ON_EXIT
#ifndef ATEXIT_MAX
#define ATEXIT_MAX 32
#endif
static int __on_exit_count = 0;
typedef void (*on_exit_func_t) (int, void *);
static on_exit_func_t __on_exit_funcs[ATEXIT_MAX];
static void *__on_exit_args[ATEXIT_MAX];
static int __exitcode = 0;
static void __handle_on_exit_funcs(void);
static int on_exit(on_exit_func_t function, void *arg);
#define exit(x) (exit)(__exitcode = (x))

static int on_exit(on_exit_func_t function, void *arg)
{
	if (__on_exit_count == ATEXIT_MAX)
		return -ENOMEM;
	else if (__on_exit_count == 0)
		atexit(__handle_on_exit_funcs);
	__on_exit_funcs[__on_exit_count] = function;
	__on_exit_args[__on_exit_count++] = arg;
	return 0;
}

static void __handle_on_exit_funcs(void)
{
	int i;
	for (i = 0; i < __on_exit_count; i++)
		__on_exit_funcs[i] (__exitcode, __on_exit_args[i]);
}
#endif

enum write_mode_t {
	WRITE_FORCE,
	WRITE_APPEND
};

struct perf_record {
	struct perf_tool	tool;
	struct perf_record_opts	opts;
	u64			bytes_written;
	const char		*output_name;
	struct perf_evlist	*evlist;
	struct perf_session	*session;
	const char		*progname;
	int			output;
	unsigned int		page_size;
	int			realtime_prio;
	enum write_mode_t	write_mode;
	bool			no_buildid;
	bool			no_buildid_cache;
	bool			force;
	bool			file_new;
	bool			append_file;
	long			samples;
	off_t			post_processing_offset;
};

static void advance_output(struct perf_record *rec, size_t size)
{
	rec->bytes_written += size;
}

static int write_output(struct perf_record *rec, void *buf, size_t size)
{
	while (size) {
		int ret = write(rec->output, buf, size);

		if (ret < 0) {
			pr_err("failed to write\n");
			return -1;
		}

		size -= ret;
		buf += ret;

		rec->bytes_written += ret;
	}

	return 0;
}

static int process_synthesized_event(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample __maybe_unused,
				     struct machine *machine __maybe_unused)
{
	struct perf_record *rec = container_of(tool, struct perf_record, tool);
	if (write_output(rec, event, event->header.size) < 0)
		return -1;

	return 0;
}

static int perf_record__mmap_read(struct perf_record *rec,
				   struct perf_mmap *md)
{
	unsigned int head = perf_mmap__read_head(md);
	unsigned int old = md->prev;
	unsigned char *data = md->base + rec->page_size;
=======
#include <asm/bug.h>
#include <linux/time64.h>

struct record {
	struct perf_tool	tool;
	struct record_opts	opts;
	u64			bytes_written;
	struct perf_data_file	file;
	struct auxtrace_record	*itr;
	struct perf_evlist	*evlist;
	struct perf_session	*session;
	const char		*progname;
	int			realtime_prio;
	bool			no_buildid;
	bool			no_buildid_set;
	bool			no_buildid_cache;
	bool			no_buildid_cache_set;
	bool			buildid_all;
	bool			timestamp_filename;
	bool			switch_output;
	unsigned long long	samples;
};

static int record__write(struct record *rec, void *bf, size_t size)
{
	if (perf_data_file__write(rec->session->file, bf, size) < 0) {
		pr_err("failed to write perf data, error: %m\n");
		return -1;
	}

	rec->bytes_written += size;
	return 0;
}

static int process_synthesized_event(struct perf_tool *tool,
				     union perf_event *event,
				     struct perf_sample *sample __maybe_unused,
				     struct machine *machine __maybe_unused)
{
	struct record *rec = container_of(tool, struct record, tool);
	return record__write(rec, event, event->header.size);
}

static int
backward_rb_find_range(void *buf, int mask, u64 head, u64 *start, u64 *end)
{
	struct perf_event_header *pheader;
	u64 evt_head = head;
	int size = mask + 1;

	pr_debug2("backward_rb_find_range: buf=%p, head=%"PRIx64"\n", buf, head);
	pheader = (struct perf_event_header *)(buf + (head & mask));
	*start = head;
	while (true) {
		if (evt_head - head >= (unsigned int)size) {
			pr_debug("Finished reading backward ring buffer: rewind\n");
			if (evt_head - head > (unsigned int)size)
				evt_head -= pheader->size;
			*end = evt_head;
			return 0;
		}

		pheader = (struct perf_event_header *)(buf + (evt_head & mask));

		if (pheader->size == 0) {
			pr_debug("Finished reading backward ring buffer: get start\n");
			*end = evt_head;
			return 0;
		}

		evt_head += pheader->size;
		pr_debug3("move evt_head: %"PRIx64"\n", evt_head);
	}
	WARN_ONCE(1, "Shouldn't get here\n");
	return -1;
}

static int
rb_find_range(void *data, int mask, u64 head, u64 old,
	      u64 *start, u64 *end, bool backward)
{
	if (!backward) {
		*start = old;
		*end = head;
		return 0;
	}

	return backward_rb_find_range(data, mask, head, start, end);
}

static int
record__mmap_read(struct record *rec, struct perf_mmap *md,
		  bool overwrite, bool backward)
{
	u64 head = perf_mmap__read_head(md);
	u64 old = md->prev;
	u64 end = head, start = old;
	unsigned char *data = md->base + page_size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long size;
	void *buf;
	int rc = 0;

<<<<<<< HEAD
	if (old == head)
=======
	if (rb_find_range(data, md->mask, head,
			  old, &start, &end, backward))
		return -1;

	if (start == end)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;

	rec->samples++;

<<<<<<< HEAD
	size = head - old;

	if ((old & md->mask) + size != (head & md->mask)) {
		buf = &data[old & md->mask];
		size = md->mask + 1 - (old & md->mask);
		old += size;

		if (write_output(rec, buf, size) < 0) {
=======
	size = end - start;
	if (size > (unsigned long)(md->mask) + 1) {
		WARN_ONCE(1, "failed to keep up with mmap data. (warn only once)\n");

		md->prev = head;
		perf_mmap__consume(md, overwrite || backward);
		return 0;
	}

	if ((start & md->mask) + size != (end & md->mask)) {
		buf = &data[start & md->mask];
		size = md->mask + 1 - (start & md->mask);
		start += size;

		if (record__write(rec, buf, size) < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			rc = -1;
			goto out;
		}
	}

<<<<<<< HEAD
	buf = &data[old & md->mask];
	size = head - old;
	old += size;

	if (write_output(rec, buf, size) < 0) {
=======
	buf = &data[start & md->mask];
	size = end - start;
	start += size;

	if (record__write(rec, buf, size) < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rc = -1;
		goto out;
	}

<<<<<<< HEAD
	md->prev = old;
	perf_mmap__write_tail(md, old);

=======
	md->prev = head;
	perf_mmap__consume(md, overwrite || backward);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return rc;
}

<<<<<<< HEAD
static volatile int done = 0;
static volatile int signr = -1;
static volatile int child_finished = 0;
=======
static volatile int done;
static volatile int signr = -1;
static volatile int child_finished;

static volatile int auxtrace_record__snapshot_started;
static DEFINE_TRIGGER(auxtrace_snapshot_trigger);
static DEFINE_TRIGGER(switch_output_trigger);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void sig_handler(int sig)
{
	if (sig == SIGCHLD)
		child_finished = 1;
<<<<<<< HEAD

	done = 1;
	signr = sig;
}

static void perf_record__sig_exit(int exit_status __maybe_unused, void *arg)
{
	struct perf_record *rec = arg;
	int status;

	if (rec->evlist->workload.pid > 0) {
		if (!child_finished)
			kill(rec->evlist->workload.pid, SIGTERM);

		wait(&status);
		if (WIFSIGNALED(status))
			psignal(WTERMSIG(status), rec->progname);
	}

	if (signr == -1 || signr == SIGUSR1)
		return;

	signal(signr, SIG_DFL);
	kill(getpid(), signr);
}

static bool perf_evlist__equal(struct perf_evlist *evlist,
			       struct perf_evlist *other)
{
	struct perf_evsel *pos, *pair;

	if (evlist->nr_entries != other->nr_entries)
		return false;

	pair = perf_evlist__first(other);

	list_for_each_entry(pos, &evlist->entries, node) {
		if (memcmp(&pos->attr, &pair->attr, sizeof(pos->attr) != 0))
			return false;
		pair = perf_evsel__next(pair);
	}

	return true;
}

static int perf_record__open(struct perf_record *rec)
=======
	else
		signr = sig;

	done = 1;
}

static void record__sig_exit(void)
{
	if (signr == -1)
		return;

	signal(signr, SIG_DFL);
	raise(signr);
}

#ifdef HAVE_AUXTRACE_SUPPORT

static int record__process_auxtrace(struct perf_tool *tool,
				    union perf_event *event, void *data1,
				    size_t len1, void *data2, size_t len2)
{
	struct record *rec = container_of(tool, struct record, tool);
	struct perf_data_file *file = &rec->file;
	size_t padding;
	u8 pad[8] = {0};

	if (!perf_data_file__is_pipe(file)) {
		off_t file_offset;
		int fd = perf_data_file__fd(file);
		int err;

		file_offset = lseek(fd, 0, SEEK_CUR);
		if (file_offset == -1)
			return -1;
		err = auxtrace_index__auxtrace_event(&rec->session->auxtrace_index,
						     event, file_offset);
		if (err)
			return err;
	}

	/* event.auxtrace.size includes padding, see __auxtrace_mmap__read() */
	padding = (len1 + len2) & 7;
	if (padding)
		padding = 8 - padding;

	record__write(rec, event, event->header.size);
	record__write(rec, data1, len1);
	if (len2)
		record__write(rec, data2, len2);
	record__write(rec, &pad, padding);

	return 0;
}

static int record__auxtrace_mmap_read(struct record *rec,
				      struct auxtrace_mmap *mm)
{
	int ret;

	ret = auxtrace_mmap__read(mm, rec->itr, &rec->tool,
				  record__process_auxtrace);
	if (ret < 0)
		return ret;

	if (ret)
		rec->samples++;

	return 0;
}

static int record__auxtrace_mmap_read_snapshot(struct record *rec,
					       struct auxtrace_mmap *mm)
{
	int ret;

	ret = auxtrace_mmap__read_snapshot(mm, rec->itr, &rec->tool,
					   record__process_auxtrace,
					   rec->opts.auxtrace_snapshot_size);
	if (ret < 0)
		return ret;

	if (ret)
		rec->samples++;

	return 0;
}

static int record__auxtrace_read_snapshot_all(struct record *rec)
{
	int i;
	int rc = 0;

	for (i = 0; i < rec->evlist->nr_mmaps; i++) {
		struct auxtrace_mmap *mm =
				&rec->evlist->mmap[i].auxtrace_mmap;

		if (!mm->base)
			continue;

		if (record__auxtrace_mmap_read_snapshot(rec, mm) != 0) {
			rc = -1;
			goto out;
		}
	}
out:
	return rc;
}

static void record__read_auxtrace_snapshot(struct record *rec)
{
	pr_debug("Recording AUX area tracing snapshot\n");
	if (record__auxtrace_read_snapshot_all(rec) < 0) {
		trigger_error(&auxtrace_snapshot_trigger);
	} else {
		if (auxtrace_record__snapshot_finish(rec->itr))
			trigger_error(&auxtrace_snapshot_trigger);
		else
			trigger_ready(&auxtrace_snapshot_trigger);
	}
}

#else

static inline
int record__auxtrace_mmap_read(struct record *rec __maybe_unused,
			       struct auxtrace_mmap *mm __maybe_unused)
{
	return 0;
}

static inline
void record__read_auxtrace_snapshot(struct record *rec __maybe_unused)
{
}

static inline
int auxtrace_record__snapshot_start(struct auxtrace_record *itr __maybe_unused)
{
	return 0;
}

#endif

static int record__mmap_evlist(struct record *rec,
			       struct perf_evlist *evlist)
{
	struct record_opts *opts = &rec->opts;
	char msg[512];

	if (perf_evlist__mmap_ex(evlist, opts->mmap_pages, false,
				 opts->auxtrace_mmap_pages,
				 opts->auxtrace_snapshot_mode) < 0) {
		if (errno == EPERM) {
			pr_err("Permission error mapping pages.\n"
			       "Consider increasing "
			       "/proc/sys/kernel/perf_event_mlock_kb,\n"
			       "or try again with a smaller value of -m/--mmap_pages.\n"
			       "(current value: %u,%u)\n",
			       opts->mmap_pages, opts->auxtrace_mmap_pages);
			return -errno;
		} else {
			pr_err("failed to mmap with %d (%s)\n", errno,
				str_error_r(errno, msg, sizeof(msg)));
			if (errno)
				return -errno;
			else
				return -EINVAL;
		}
	}
	return 0;
}

static int record__mmap(struct record *rec)
{
	return record__mmap_evlist(rec, rec->evlist);
}

static int record__open(struct record *rec)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	char msg[512];
	struct perf_evsel *pos;
	struct perf_evlist *evlist = rec->evlist;
	struct perf_session *session = rec->session;
<<<<<<< HEAD
	struct perf_record_opts *opts = &rec->opts;
	int rc = 0;

	perf_evlist__config(evlist, opts);

	list_for_each_entry(pos, &evlist->entries, node) {
try_again:
		if (perf_evsel__open(pos, evlist->cpus, evlist->threads) < 0) {
=======
	struct record_opts *opts = &rec->opts;
	struct perf_evsel_config_term *err_term;
	int rc = 0;

	perf_evlist__config(evlist, opts, &callchain_param);

	evlist__for_each_entry(evlist, pos) {
try_again:
		if (perf_evsel__open(pos, pos->cpus, pos->threads) < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (perf_evsel__fallback(pos, errno, msg, sizeof(msg))) {
				if (verbose)
					ui__warning("%s\n", msg);
				goto try_again;
			}

			rc = -errno;
			perf_evsel__open_strerror(pos, &opts->target,
						  errno, msg, sizeof(msg));
			ui__error("%s\n", msg);
			goto out;
		}
	}

<<<<<<< HEAD
	if (perf_evlist__apply_filters(evlist)) {
		error("failed to set filter with %d (%s)\n", errno,
			strerror(errno));
=======
	if (perf_evlist__apply_filters(evlist, &pos)) {
		error("failed to set filter \"%s\" on event %s with %d (%s)\n",
			pos->filter, perf_evsel__name(pos), errno,
			str_error_r(errno, msg, sizeof(msg)));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		rc = -1;
		goto out;
	}

<<<<<<< HEAD
	if (perf_evlist__mmap(evlist, opts->mmap_pages, false) < 0) {
		if (errno == EPERM) {
			pr_err("Permission error mapping pages.\n"
			       "Consider increasing "
			       "/proc/sys/kernel/perf_event_mlock_kb,\n"
			       "or try again with a smaller value of -m/--mmap_pages.\n"
			       "(current value: %d)\n", opts->mmap_pages);
			rc = -errno;
		} else if (!is_power_of_2(opts->mmap_pages) &&
			   (opts->mmap_pages != UINT_MAX)) {
			pr_err("--mmap_pages/-m value must be a power of two.");
			rc = -EINVAL;
		} else {
			pr_err("failed to mmap with %d (%s)\n", errno, strerror(errno));
			rc = -errno;
		}
		goto out;
	}

	if (rec->file_new)
		session->evlist = evlist;
	else {
		if (!perf_evlist__equal(session->evlist, evlist)) {
			fprintf(stderr, "incompatible append\n");
			rc = -1;
			goto out;
		}
 	}

=======
	if (perf_evlist__apply_drv_configs(evlist, &pos, &err_term)) {
		error("failed to set config \"%s\" on event %s with %d (%s)\n",
		      err_term->val.drv_cfg, perf_evsel__name(pos), errno,
		      str_error_r(errno, msg, sizeof(msg)));
		rc = -1;
		goto out;
	}

	rc = record__mmap(rec);
	if (rc)
		goto out;

	session->evlist = evlist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_session__set_id_hdr_size(session);
out:
	return rc;
}

<<<<<<< HEAD
static int process_buildids(struct perf_record *rec)
{
	u64 size = lseek(rec->output, 0, SEEK_CUR);

	if (size == 0)
		return 0;

	rec->session->fd = rec->output;
	return __perf_session__process_events(rec->session, rec->post_processing_offset,
					      size - rec->post_processing_offset,
					      size, &build_id__mark_dso_hit_ops);
}

static void perf_record__exit(int status, void *arg)
{
	struct perf_record *rec = arg;

	if (status != 0)
		return;

	if (!rec->opts.pipe_output) {
		rec->session->header.data_size += rec->bytes_written;

		if (!rec->no_buildid)
			process_buildids(rec);
		perf_session__write_header(rec->session, rec->evlist,
					   rec->output, true);
		perf_session__delete(rec->session);
		perf_evlist__delete(rec->evlist);
		symbol__exit();
	}
=======
static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
	struct record *rec = container_of(tool, struct record, tool);

	rec->samples++;

	return build_id__mark_dso_hit(tool, event, sample, evsel, machine);
}

static int process_buildids(struct record *rec)
{
	struct perf_data_file *file  = &rec->file;
	struct perf_session *session = rec->session;

	if (file->size == 0)
		return 0;

	/*
	 * During this process, it'll load kernel map and replace the
	 * dso->long_name to a real pathname it found.  In this case
	 * we prefer the vmlinux path like
	 *   /lib/modules/3.16.4/build/vmlinux
	 *
	 * rather than build-id path (in debug directory).
	 *   $HOME/.debug/.build-id/f0/6e17aa50adf4d00b88925e03775de107611551
	 */
	symbol_conf.ignore_vmlinux_buildid = true;

	/*
	 * If --buildid-all is given, it marks all DSO regardless of hits,
	 * so no need to process samples.
	 */
	if (rec->buildid_all)
		rec->tool.sample = NULL;

	return perf_session__process_events(session);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void perf_event__synthesize_guest_os(struct machine *machine, void *data)
{
	int err;
	struct perf_tool *tool = data;
	/*
	 *As for guest kernel when processing subcommand record&report,
	 *we arrange module mmap prior to guest kernel mmap and trigger
	 *a preload dso because default guest module symbols are loaded
	 *from guest kallsyms instead of /lib/modules/XXX/XXX. This
	 *method is used to avoid symbol missing when the first addr is
	 *in module instead of in guest kernel.
	 */
	err = perf_event__synthesize_modules(tool, process_synthesized_event,
					     machine);
	if (err < 0)
		pr_err("Couldn't record guest kernel [%d]'s reference"
		       " relocation symbol.\n", machine->pid);

	/*
	 * We use _stext for guest kernel because guest kernel's /proc/kallsyms
	 * have no _text sometimes.
	 */
	err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
<<<<<<< HEAD
						 machine, "_text");
	if (err < 0)
		err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
							 machine, "_stext");
=======
						 machine);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err < 0)
		pr_err("Couldn't record guest kernel [%d]'s reference"
		       " relocation symbol.\n", machine->pid);
}

static struct perf_event_header finished_round_event = {
	.size = sizeof(struct perf_event_header),
	.type = PERF_RECORD_FINISHED_ROUND,
};

<<<<<<< HEAD
static int perf_record__mmap_read_all(struct perf_record *rec)
{
	int i;
	int rc = 0;

	for (i = 0; i < rec->evlist->nr_mmaps; i++) {
		if (rec->evlist->mmap[i].base) {
			if (perf_record__mmap_read(rec, &rec->evlist->mmap[i]) != 0) {
=======
static int record__mmap_read_evlist(struct record *rec, struct perf_evlist *evlist,
				    bool backward)
{
	u64 bytes_written = rec->bytes_written;
	int i;
	int rc = 0;
	struct perf_mmap *maps;

	if (!evlist)
		return 0;

	maps = backward ? evlist->backward_mmap : evlist->mmap;
	if (!maps)
		return 0;

	if (backward && evlist->bkw_mmap_state != BKW_MMAP_DATA_PENDING)
		return 0;

	for (i = 0; i < evlist->nr_mmaps; i++) {
		struct auxtrace_mmap *mm = &maps[i].auxtrace_mmap;

		if (maps[i].base) {
			if (record__mmap_read(rec, &maps[i],
					      evlist->overwrite, backward) != 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				rc = -1;
				goto out;
			}
		}
<<<<<<< HEAD
	}

	if (perf_header__has_feat(&rec->session->header, HEADER_TRACING_DATA))
		rc = write_output(rec, &finished_round_event,
				  sizeof(finished_round_event));

=======

		if (mm->base && !rec->opts.auxtrace_snapshot_mode &&
		    record__auxtrace_mmap_read(rec, mm) != 0) {
			rc = -1;
			goto out;
		}
	}

	/*
	 * Mark the round finished in case we wrote
	 * at least one event.
	 */
	if (bytes_written != rec->bytes_written)
		rc = record__write(rec, &finished_round_event, sizeof(finished_round_event));

	if (backward)
		perf_evlist__toggle_bkw_mmap(evlist, BKW_MMAP_EMPTY);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return rc;
}

<<<<<<< HEAD
static int __cmd_record(struct perf_record *rec, int argc, const char **argv)
{
	struct stat st;
	int flags;
	int err, output, feat;
=======
static int record__mmap_read_all(struct record *rec)
{
	int err;

	err = record__mmap_read_evlist(rec, rec->evlist, false);
	if (err)
		return err;

	return record__mmap_read_evlist(rec, rec->evlist, true);
}

static void record__init_features(struct record *rec)
{
	struct perf_session *session = rec->session;
	int feat;

	for (feat = HEADER_FIRST_FEATURE; feat < HEADER_LAST_FEATURE; feat++)
		perf_header__set_feat(&session->header, feat);

	if (rec->no_buildid)
		perf_header__clear_feat(&session->header, HEADER_BUILD_ID);

	if (!have_tracepoints(&rec->evlist->entries))
		perf_header__clear_feat(&session->header, HEADER_TRACING_DATA);

	if (!rec->opts.branch_stack)
		perf_header__clear_feat(&session->header, HEADER_BRANCH_STACK);

	if (!rec->opts.full_auxtrace)
		perf_header__clear_feat(&session->header, HEADER_AUXTRACE);

	perf_header__clear_feat(&session->header, HEADER_STAT);
}

static void
record__finish_output(struct record *rec)
{
	struct perf_data_file *file = &rec->file;
	int fd = perf_data_file__fd(file);

	if (file->is_pipe)
		return;

	rec->session->header.data_size += rec->bytes_written;
	file->size = lseek(perf_data_file__fd(file), 0, SEEK_CUR);

	if (!rec->no_buildid) {
		process_buildids(rec);

		if (rec->buildid_all)
			dsos__hit_all(rec->session);
	}
	perf_session__write_header(rec->session, rec->evlist, fd, true);

	return;
}

static int record__synthesize_workload(struct record *rec, bool tail)
{
	struct {
		struct thread_map map;
		struct thread_map_data map_data;
	} thread_map;

	if (rec->opts.tail_synthesize != tail)
		return 0;

	thread_map.map.nr = 1;
	thread_map.map.map[0].pid = rec->evlist->workload.pid;
	thread_map.map.map[0].comm = NULL;
	return perf_event__synthesize_thread_map(&rec->tool, &thread_map.map,
						 process_synthesized_event,
						 &rec->session->machines.host,
						 rec->opts.sample_address,
						 rec->opts.proc_map_timeout);
}

static int record__synthesize(struct record *rec, bool tail);

static int
record__switch_output(struct record *rec, bool at_exit)
{
	struct perf_data_file *file = &rec->file;
	int fd, err;

	/* Same Size:      "2015122520103046"*/
	char timestamp[] = "InvalidTimestamp";

	record__synthesize(rec, true);
	if (target__none(&rec->opts.target))
		record__synthesize_workload(rec, true);

	rec->samples = 0;
	record__finish_output(rec);
	err = fetch_current_timestamp(timestamp, sizeof(timestamp));
	if (err) {
		pr_err("Failed to get current timestamp\n");
		return -EINVAL;
	}

	fd = perf_data_file__switch(file, timestamp,
				    rec->session->header.data_offset,
				    at_exit);
	if (fd >= 0 && !at_exit) {
		rec->bytes_written = 0;
		rec->session->header.data_size = 0;
	}

	if (!quiet)
		fprintf(stderr, "[ perf record: Dump %s.%s ]\n",
			file->path, timestamp);

	/* Output tracking events */
	if (!at_exit) {
		record__synthesize(rec, false);

		/*
		 * In 'perf record --switch-output' without -a,
		 * record__synthesize() in record__switch_output() won't
		 * generate tracking events because there's no thread_map
		 * in evlist. Which causes newly created perf.data doesn't
		 * contain map and comm information.
		 * Create a fake thread_map and directly call
		 * perf_event__synthesize_thread_map() for those events.
		 */
		if (target__none(&rec->opts.target))
			record__synthesize_workload(rec, false);
	}
	return fd;
}

static volatile int workload_exec_errno;

/*
 * perf_evlist__prepare_workload will send a SIGUSR1
 * if the fork fails, since we asked by setting its
 * want_signal to true.
 */
static void workload_exec_failed_signal(int signo __maybe_unused,
					siginfo_t *info,
					void *ucontext __maybe_unused)
{
	workload_exec_errno = info->si_value.sival_int;
	done = 1;
	child_finished = 1;
}

static void snapshot_sig_handler(int sig);

int __weak
perf_event__synth_time_conv(const struct perf_event_mmap_page *pc __maybe_unused,
			    struct perf_tool *tool __maybe_unused,
			    perf_event__handler_t process __maybe_unused,
			    struct machine *machine __maybe_unused)
{
	return 0;
}

static const struct perf_event_mmap_page *
perf_evlist__pick_pc(struct perf_evlist *evlist)
{
	if (evlist) {
		if (evlist->mmap && evlist->mmap[0].base)
			return evlist->mmap[0].base;
		if (evlist->backward_mmap && evlist->backward_mmap[0].base)
			return evlist->backward_mmap[0].base;
	}
	return NULL;
}

static const struct perf_event_mmap_page *record__pick_pc(struct record *rec)
{
	const struct perf_event_mmap_page *pc;

	pc = perf_evlist__pick_pc(rec->evlist);
	if (pc)
		return pc;
	return NULL;
}

static int record__synthesize(struct record *rec, bool tail)
{
	struct perf_session *session = rec->session;
	struct machine *machine = &session->machines.host;
	struct perf_data_file *file = &rec->file;
	struct record_opts *opts = &rec->opts;
	struct perf_tool *tool = &rec->tool;
	int fd = perf_data_file__fd(file);
	int err = 0;

	if (rec->opts.tail_synthesize != tail)
		return 0;

	if (file->is_pipe) {
		err = perf_event__synthesize_attrs(tool, session,
						   process_synthesized_event);
		if (err < 0) {
			pr_err("Couldn't synthesize attrs.\n");
			goto out;
		}

		if (have_tracepoints(&rec->evlist->entries)) {
			/*
			 * FIXME err <= 0 here actually means that
			 * there were no tracepoints so its not really
			 * an error, just that we don't need to
			 * synthesize anything.  We really have to
			 * return this more properly and also
			 * propagate errors that now are calling die()
			 */
			err = perf_event__synthesize_tracing_data(tool,	fd, rec->evlist,
								  process_synthesized_event);
			if (err <= 0) {
				pr_err("Couldn't record tracing data.\n");
				goto out;
			}
			rec->bytes_written += err;
		}
	}

	err = perf_event__synth_time_conv(record__pick_pc(rec), tool,
					  process_synthesized_event, machine);
	if (err)
		goto out;

	if (rec->opts.full_auxtrace) {
		err = perf_event__synthesize_auxtrace_info(rec->itr, tool,
					session, process_synthesized_event);
		if (err)
			goto out;
	}

	err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
						 machine);
	WARN_ONCE(err < 0, "Couldn't record kernel reference relocation symbol\n"
			   "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
			   "Check /proc/kallsyms permission or run as root.\n");

	err = perf_event__synthesize_modules(tool, process_synthesized_event,
					     machine);
	WARN_ONCE(err < 0, "Couldn't record kernel module information.\n"
			   "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
			   "Check /proc/modules permission or run as root.\n");

	if (perf_guest) {
		machines__process_guests(&session->machines,
					 perf_event__synthesize_guest_os, tool);
	}

	err = __machine__synthesize_threads(machine, tool, &opts->target, rec->evlist->threads,
					    process_synthesized_event, opts->sample_address,
					    opts->proc_map_timeout);
out:
	return err;
}

static int __cmd_record(struct record *rec, int argc, const char **argv)
{
	int err;
	int status = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long waking = 0;
	const bool forks = argc > 0;
	struct machine *machine;
	struct perf_tool *tool = &rec->tool;
<<<<<<< HEAD
	struct perf_record_opts *opts = &rec->opts;
	struct perf_evlist *evsel_list = rec->evlist;
	const char *output_name = rec->output_name;
	struct perf_session *session;
	bool disabled = false;

	rec->progname = argv[0];

	rec->page_size = sysconf(_SC_PAGE_SIZE);

	on_exit(perf_record__sig_exit, rec);
	signal(SIGCHLD, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);

	if (!output_name) {
		if (!fstat(STDOUT_FILENO, &st) && S_ISFIFO(st.st_mode))
			opts->pipe_output = true;
		else
			rec->output_name = output_name = "perf.data";
	}
	if (output_name) {
		if (!strcmp(output_name, "-"))
			opts->pipe_output = true;
		else if (!stat(output_name, &st) && st.st_size) {
			if (rec->write_mode == WRITE_FORCE) {
				char oldname[PATH_MAX];
				snprintf(oldname, sizeof(oldname), "%s.old",
					 output_name);
				unlink(oldname);
				rename(output_name, oldname);
			}
		} else if (rec->write_mode == WRITE_APPEND) {
			rec->write_mode = WRITE_FORCE;
		}
	}

	flags = O_CREAT|O_RDWR;
	if (rec->write_mode == WRITE_APPEND)
		rec->file_new = 0;
	else
		flags |= O_TRUNC;

	if (opts->pipe_output)
		output = STDOUT_FILENO;
	else
		output = open(output_name, flags, S_IRUSR | S_IWUSR);
	if (output < 0) {
		perror("failed to create output file");
		return -1;
	}

	rec->output = output;

	session = perf_session__new(output_name, O_WRONLY,
				    rec->write_mode == WRITE_FORCE, false, NULL);
	if (session == NULL) {
		pr_err("Not enough memory for reading perf file header\n");
		return -1;
	}

	rec->session = session;

	for (feat = HEADER_FIRST_FEATURE; feat < HEADER_LAST_FEATURE; feat++)
		perf_header__set_feat(&session->header, feat);

	if (rec->no_buildid)
		perf_header__clear_feat(&session->header, HEADER_BUILD_ID);

	if (!have_tracepoints(&evsel_list->entries))
		perf_header__clear_feat(&session->header, HEADER_TRACING_DATA);

	if (!rec->opts.branch_stack)
		perf_header__clear_feat(&session->header, HEADER_BRANCH_STACK);

	if (!rec->file_new) {
		err = perf_session__read_header(session, output);
		if (err < 0)
			goto out_delete_session;
	}

	if (forks) {
		err = perf_evlist__prepare_workload(evsel_list, &opts->target,
						    argv, opts->pipe_output,
						    true);
		if (err < 0) {
			pr_err("Couldn't run the workload!\n");
=======
	struct record_opts *opts = &rec->opts;
	struct perf_data_file *file = &rec->file;
	struct perf_session *session;
	bool disabled = false, draining = false;
	int fd;

	rec->progname = argv[0];

	atexit(record__sig_exit);
	signal(SIGCHLD, sig_handler);
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	if (rec->opts.auxtrace_snapshot_mode || rec->switch_output) {
		signal(SIGUSR2, snapshot_sig_handler);
		if (rec->opts.auxtrace_snapshot_mode)
			trigger_on(&auxtrace_snapshot_trigger);
		if (rec->switch_output)
			trigger_on(&switch_output_trigger);
	} else {
		signal(SIGUSR2, SIG_IGN);
	}

	session = perf_session__new(file, false, tool);
	if (session == NULL) {
		pr_err("Perf session creation failed.\n");
		return -1;
	}

	fd = perf_data_file__fd(file);
	rec->session = session;

	record__init_features(rec);

	if (forks) {
		err = perf_evlist__prepare_workload(rec->evlist, &opts->target,
						    argv, file->is_pipe,
						    workload_exec_failed_signal);
		if (err < 0) {
			pr_err("Couldn't run the workload!\n");
			status = err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto out_delete_session;
		}
	}

<<<<<<< HEAD
	if (perf_record__open(rec) != 0) {
		err = -1;
		goto out_delete_session;
	}

	if (!evsel_list->nr_groups)
		perf_header__clear_feat(&session->header, HEADER_GROUP_DESC);

	/*
	 * perf_session__delete(session) will be called at perf_record__exit()
	 */
	on_exit(perf_record__exit, rec);

	if (opts->pipe_output) {
		err = perf_header__write_pipe(output);
		if (err < 0)
			goto out_delete_session;
	} else if (rec->file_new) {
		err = perf_session__write_header(session, evsel_list,
						 output, false);
		if (err < 0)
			goto out_delete_session;
=======
	if (record__open(rec) != 0) {
		err = -1;
		goto out_child;
	}

	err = bpf__apply_obj_config();
	if (err) {
		char errbuf[BUFSIZ];

		bpf__strerror_apply_obj_config(err, errbuf, sizeof(errbuf));
		pr_err("ERROR: Apply config to BPF failed: %s\n",
			 errbuf);
		goto out_child;
	}

	/*
	 * Normally perf_session__new would do this, but it doesn't have the
	 * evlist.
	 */
	if (rec->tool.ordered_events && !perf_evlist__sample_id_all(rec->evlist)) {
		pr_warning("WARNING: No sample_id_all support, falling back to unordered processing\n");
		rec->tool.ordered_events = false;
	}

	if (!rec->evlist->nr_groups)
		perf_header__clear_feat(&session->header, HEADER_GROUP_DESC);

	if (file->is_pipe) {
		err = perf_header__write_pipe(fd);
		if (err < 0)
			goto out_child;
	} else {
		err = perf_session__write_header(session, rec->evlist, fd, false);
		if (err < 0)
			goto out_child;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (!rec->no_buildid
	    && !perf_header__has_feat(&session->header, HEADER_BUILD_ID)) {
		pr_err("Couldn't generate buildids. "
		       "Use --no-buildid to profile anyway.\n");
		err = -1;
<<<<<<< HEAD
		goto out_delete_session;
	}

	rec->post_processing_offset = lseek(output, 0, SEEK_CUR);

	machine = &session->machines.host;

	if (opts->pipe_output) {
		err = perf_event__synthesize_attrs(tool, session,
						   process_synthesized_event);
		if (err < 0) {
			pr_err("Couldn't synthesize attrs.\n");
			goto out_delete_session;
		}

		err = perf_event__synthesize_event_types(tool, process_synthesized_event,
							 machine);
		if (err < 0) {
			pr_err("Couldn't synthesize event_types.\n");
			goto out_delete_session;
		}

		if (have_tracepoints(&evsel_list->entries)) {
			/*
			 * FIXME err <= 0 here actually means that
			 * there were no tracepoints so its not really
			 * an error, just that we don't need to
			 * synthesize anything.  We really have to
			 * return this more properly and also
			 * propagate errors that now are calling die()
			 */
			err = perf_event__synthesize_tracing_data(tool, output, evsel_list,
								  process_synthesized_event);
			if (err <= 0) {
				pr_err("Couldn't record tracing data.\n");
				goto out_delete_session;
			}
			advance_output(rec, err);
		}
	}

	err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
						 machine, "_text");
	if (err < 0)
		err = perf_event__synthesize_kernel_mmap(tool, process_synthesized_event,
							 machine, "_stext");
	if (err < 0)
		pr_err("Couldn't record kernel reference relocation symbol\n"
		       "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
		       "Check /proc/kallsyms permission or run as root.\n");

	err = perf_event__synthesize_modules(tool, process_synthesized_event,
					     machine);
	if (err < 0)
		pr_err("Couldn't record kernel module information.\n"
		       "Symbol resolution may be skewed if relocation was used (e.g. kexec).\n"
		       "Check /proc/modules permission or run as root.\n");

	if (perf_guest) {
		machines__process_guests(&session->machines,
					 perf_event__synthesize_guest_os, tool);
	}

	if (perf_target__has_task(&opts->target))
		err = perf_event__synthesize_thread_map(tool, evsel_list->threads,
						  process_synthesized_event,
						  machine);
	else if (perf_target__has_cpu(&opts->target))
		err = perf_event__synthesize_threads(tool, process_synthesized_event,
					       machine);
	else /* command specified */
		err = 0;

	if (err != 0)
		goto out_delete_session;
=======
		goto out_child;
	}

	machine = &session->machines.host;

	err = record__synthesize(rec, false);
	if (err < 0)
		goto out_child;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (rec->realtime_prio) {
		struct sched_param param;

		param.sched_priority = rec->realtime_prio;
		if (sched_setscheduler(0, SCHED_FIFO, &param)) {
			pr_err("Could not set realtime priority.\n");
			err = -1;
<<<<<<< HEAD
			goto out_delete_session;
=======
			goto out_child;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	/*
	 * When perf is starting the traced process, all the events
	 * (apart from group members) have enable_on_exec=1 set,
	 * so don't spoil it by prematurely enabling them.
	 */
<<<<<<< HEAD
	if (!perf_target__none(&opts->target))
		perf_evlist__enable(evsel_list);
=======
	if (!target__none(&opts->target) && !opts->initial_delay)
		perf_evlist__enable(rec->evlist);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Let the child rip
	 */
<<<<<<< HEAD
	if (forks)
		perf_evlist__start_workload(evsel_list);

	for (;;) {
		int hits = rec->samples;

		if (perf_record__mmap_read_all(rec) < 0) {
			err = -1;
			goto out_delete_session;
		}

		if (hits == rec->samples) {
			if (done)
				break;
			err = poll(evsel_list->pollfd, evsel_list->nr_fds, -1);
			waking++;
=======
	if (forks) {
		union perf_event *event;

		event = malloc(sizeof(event->comm) + machine->id_hdr_size);
		if (event == NULL) {
			err = -ENOMEM;
			goto out_child;
		}

		/*
		 * Some H/W events are generated before COMM event
		 * which is emitted during exec(), so perf script
		 * cannot see a correct process name for those events.
		 * Synthesize COMM event to prevent it.
		 */
		perf_event__synthesize_comm(tool, event,
					    rec->evlist->workload.pid,
					    process_synthesized_event,
					    machine);
		free(event);

		perf_evlist__start_workload(rec->evlist);
	}

	if (opts->initial_delay) {
		usleep(opts->initial_delay * USEC_PER_MSEC);
		perf_evlist__enable(rec->evlist);
	}

	trigger_ready(&auxtrace_snapshot_trigger);
	trigger_ready(&switch_output_trigger);
	for (;;) {
		unsigned long long hits = rec->samples;

		/*
		 * rec->evlist->bkw_mmap_state is possible to be
		 * BKW_MMAP_EMPTY here: when done == true and
		 * hits != rec->samples in previous round.
		 *
		 * perf_evlist__toggle_bkw_mmap ensure we never
		 * convert BKW_MMAP_EMPTY to BKW_MMAP_DATA_PENDING.
		 */
		if (trigger_is_hit(&switch_output_trigger) || done || draining)
			perf_evlist__toggle_bkw_mmap(rec->evlist, BKW_MMAP_DATA_PENDING);

		if (record__mmap_read_all(rec) < 0) {
			trigger_error(&auxtrace_snapshot_trigger);
			trigger_error(&switch_output_trigger);
			err = -1;
			goto out_child;
		}

		if (auxtrace_record__snapshot_started) {
			auxtrace_record__snapshot_started = 0;
			if (!trigger_is_error(&auxtrace_snapshot_trigger))
				record__read_auxtrace_snapshot(rec);
			if (trigger_is_error(&auxtrace_snapshot_trigger)) {
				pr_err("AUX area tracing snapshot failed\n");
				err = -1;
				goto out_child;
			}
		}

		if (trigger_is_hit(&switch_output_trigger)) {
			/*
			 * If switch_output_trigger is hit, the data in
			 * overwritable ring buffer should have been collected,
			 * so bkw_mmap_state should be set to BKW_MMAP_EMPTY.
			 *
			 * If SIGUSR2 raise after or during record__mmap_read_all(),
			 * record__mmap_read_all() didn't collect data from
			 * overwritable ring buffer. Read again.
			 */
			if (rec->evlist->bkw_mmap_state == BKW_MMAP_RUNNING)
				continue;
			trigger_ready(&switch_output_trigger);

			/*
			 * Reenable events in overwrite ring buffer after
			 * record__mmap_read_all(): we should have collected
			 * data from it.
			 */
			perf_evlist__toggle_bkw_mmap(rec->evlist, BKW_MMAP_RUNNING);

			if (!quiet)
				fprintf(stderr, "[ perf record: dump data: Woken up %ld times ]\n",
					waking);
			waking = 0;
			fd = record__switch_output(rec, false);
			if (fd < 0) {
				pr_err("Failed to switch to new file\n");
				trigger_error(&switch_output_trigger);
				err = fd;
				goto out_child;
			}
		}

		if (hits == rec->samples) {
			if (done || draining)
				break;
			err = perf_evlist__poll(rec->evlist, -1);
			/*
			 * Propagate error, only if there's any. Ignore positive
			 * number of returned events and interrupt error.
			 */
			if (err > 0 || (err < 0 && errno == EINTR))
				err = 0;
			waking++;

			if (perf_evlist__filter_pollfd(rec->evlist, POLLERR | POLLHUP) == 0)
				draining = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/*
		 * When perf is starting the traced process, at the end events
		 * die with the process and we wait for that. Thus no need to
		 * disable events in this case.
		 */
<<<<<<< HEAD
		if (done && !disabled && !perf_target__none(&opts->target)) {
			perf_evlist__disable(evsel_list);
			disabled = true;
		}
	}

	if (quiet || signr == SIGUSR1)
		return 0;

	fprintf(stderr, "[ perf record: Woken up %ld times to write data ]\n", waking);

	/*
	 * Approximate RIP event size: 24 bytes.
	 */
	fprintf(stderr,
		"[ perf record: Captured and wrote %.3f MB %s (~%" PRIu64 " samples) ]\n",
		(double)rec->bytes_written / 1024.0 / 1024.0,
		output_name,
		rec->bytes_written / 24);

	return 0;

out_delete_session:
	perf_session__delete(session);
	return err;
}

#define BRANCH_OPT(n, m) \
	{ .name = n, .mode = (m) }

#define BRANCH_END { .name = NULL }

struct branch_mode {
	const char *name;
	int mode;
};

static const struct branch_mode branch_modes[] = {
	BRANCH_OPT("u", PERF_SAMPLE_BRANCH_USER),
	BRANCH_OPT("k", PERF_SAMPLE_BRANCH_KERNEL),
	BRANCH_OPT("hv", PERF_SAMPLE_BRANCH_HV),
	BRANCH_OPT("any", PERF_SAMPLE_BRANCH_ANY),
	BRANCH_OPT("any_call", PERF_SAMPLE_BRANCH_ANY_CALL),
	BRANCH_OPT("any_ret", PERF_SAMPLE_BRANCH_ANY_RETURN),
	BRANCH_OPT("ind_call", PERF_SAMPLE_BRANCH_IND_CALL),
	BRANCH_END
};

static int
parse_branch_stack(const struct option *opt, const char *str, int unset)
{
#define ONLY_PLM \
	(PERF_SAMPLE_BRANCH_USER	|\
	 PERF_SAMPLE_BRANCH_KERNEL	|\
	 PERF_SAMPLE_BRANCH_HV)

	uint64_t *mode = (uint64_t *)opt->value;
	const struct branch_mode *br;
	char *s, *os = NULL, *p;
	int ret = -1;

	if (unset)
		return 0;

	/*
	 * cannot set it twice, -b + --branch-filter for instance
	 */
	if (*mode)
		return -1;

	/* str may be NULL in case no arg is passed to -b */
	if (str) {
		/* because str is read-only */
		s = os = strdup(str);
		if (!s)
			return -1;

		for (;;) {
			p = strchr(s, ',');
			if (p)
				*p = '\0';

			for (br = branch_modes; br->name; br++) {
				if (!strcasecmp(s, br->name))
					break;
			}
			if (!br->name) {
				ui__warning("unknown branch filter %s,"
					    " check man page\n", s);
				goto error;
			}

			*mode |= br->mode;

			if (!p)
				break;

			s = p + 1;
		}
	}
	ret = 0;

	/* default to any branch */
	if ((*mode & ~ONLY_PLM) == 0) {
		*mode = PERF_SAMPLE_BRANCH_ANY;
	}
error:
	free(os);
	return ret;
}

#ifdef LIBUNWIND_SUPPORT
static int get_stack_size(char *str, unsigned long *_size)
{
	char *endptr;
	unsigned long size;
	unsigned long max_size = round_down(USHRT_MAX, sizeof(u64));

	size = strtoul(str, &endptr, 0);

	do {
		if (*endptr)
			break;

		size = round_up(size, sizeof(u64));
		if (!size || size > max_size)
			break;

		*_size = size;
		return 0;

	} while (0);

	pr_err("callchain: Incorrect stack dump size (max %ld): %s\n",
	       max_size, str);
	return -1;
}
#endif /* LIBUNWIND_SUPPORT */

int record_parse_callchain_opt(const struct option *opt,
			       const char *arg, int unset)
{
	struct perf_record_opts *opts = opt->value;
	char *tok, *name, *saveptr = NULL;
	char *buf;
	int ret = -1;

	/* --no-call-graph */
	if (unset)
		return 0;

	/* We specified default option if none is provided. */
	BUG_ON(!arg);

	/* We need buffer that we know we can write to. */
	buf = malloc(strlen(arg) + 1);
	if (!buf)
		return -ENOMEM;

	strcpy(buf, arg);

	tok = strtok_r((char *)buf, ",", &saveptr);
	name = tok ? : (char *)buf;

	do {
		/* Framepointer style */
		if (!strncmp(name, "fp", sizeof("fp"))) {
			if (!strtok_r(NULL, ",", &saveptr)) {
				opts->call_graph = CALLCHAIN_FP;
				ret = 0;
			} else
				pr_err("callchain: No more arguments "
				       "needed for -g fp\n");
			break;

#ifdef LIBUNWIND_SUPPORT
		/* Dwarf style */
		} else if (!strncmp(name, "dwarf", sizeof("dwarf"))) {
			const unsigned long default_stack_dump_size = 8192;

			ret = 0;
			opts->call_graph = CALLCHAIN_DWARF;
			opts->stack_dump_size = default_stack_dump_size;

			tok = strtok_r(NULL, ",", &saveptr);
			if (tok) {
				unsigned long size = 0;

				ret = get_stack_size(tok, &size);
				opts->stack_dump_size = size;
			}

			if (!ret)
				pr_debug("callchain: stack dump size %d\n",
					 opts->stack_dump_size);
#endif /* LIBUNWIND_SUPPORT */
		} else {
			pr_err("callchain: Unknown -g option "
			       "value: %s\n", arg);
			break;
		}

	} while (0);

	free(buf);

	if (!ret)
		pr_debug("callchain: type %d\n", opts->call_graph);

	return ret;
}

static const char * const record_usage[] = {
=======
		if (done && !disabled && !target__none(&opts->target)) {
			trigger_off(&auxtrace_snapshot_trigger);
			perf_evlist__disable(rec->evlist);
			disabled = true;
		}
	}
	trigger_off(&auxtrace_snapshot_trigger);
	trigger_off(&switch_output_trigger);

	if (forks && workload_exec_errno) {
		char msg[STRERR_BUFSIZE];
		const char *emsg = str_error_r(workload_exec_errno, msg, sizeof(msg));
		pr_err("Workload failed: %s\n", emsg);
		err = -1;
		goto out_child;
	}

	if (!quiet)
		fprintf(stderr, "[ perf record: Woken up %ld times to write data ]\n", waking);

	if (target__none(&rec->opts.target))
		record__synthesize_workload(rec, true);

out_child:
	if (forks) {
		int exit_status;

		if (!child_finished)
			kill(rec->evlist->workload.pid, SIGTERM);

		wait(&exit_status);

		if (err < 0)
			status = err;
		else if (WIFEXITED(exit_status))
			status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			signr = WTERMSIG(exit_status);
	} else
		status = err;

	record__synthesize(rec, true);
	/* this will be recalculated during process_buildids() */
	rec->samples = 0;

	if (!err) {
		if (!rec->timestamp_filename) {
			record__finish_output(rec);
		} else {
			fd = record__switch_output(rec, true);
			if (fd < 0) {
				status = fd;
				goto out_delete_session;
			}
		}
	}

	if (!err && !quiet) {
		char samples[128];
		const char *postfix = rec->timestamp_filename ?
					".<timestamp>" : "";

		if (rec->samples && !rec->opts.full_auxtrace)
			scnprintf(samples, sizeof(samples),
				  " (%" PRIu64 " samples)", rec->samples);
		else
			samples[0] = '\0';

		fprintf(stderr,	"[ perf record: Captured and wrote %.3f MB %s%s%s ]\n",
			perf_data_file__size(file) / 1024.0 / 1024.0,
			file->path, postfix, samples);
	}

out_delete_session:
	perf_session__delete(session);
	return status;
}

static void callchain_debug(struct callchain_param *callchain)
{
	static const char *str[CALLCHAIN_MAX] = { "NONE", "FP", "DWARF", "LBR" };

	pr_debug("callchain: type %s\n", str[callchain->record_mode]);

	if (callchain->record_mode == CALLCHAIN_DWARF)
		pr_debug("callchain: stack dump size %d\n",
			 callchain->dump_size);
}

int record_opts__parse_callchain(struct record_opts *record,
				 struct callchain_param *callchain,
				 const char *arg, bool unset)
{
	int ret;
	callchain->enabled = !unset;

	/* --no-call-graph */
	if (unset) {
		callchain->record_mode = CALLCHAIN_NONE;
		pr_debug("callchain: disabled\n");
		return 0;
	}

	ret = parse_callchain_record_opt(arg, callchain);
	if (!ret) {
		/* Enable data address sampling for DWARF unwind. */
		if (callchain->record_mode == CALLCHAIN_DWARF)
			record->sample_address = true;
		callchain_debug(callchain);
	}

	return ret;
}

int record_parse_callchain_opt(const struct option *opt,
			       const char *arg,
			       int unset)
{
	return record_opts__parse_callchain(opt->value, &callchain_param, arg, unset);
}

int record_callchain_opt(const struct option *opt,
			 const char *arg __maybe_unused,
			 int unset __maybe_unused)
{
	struct callchain_param *callchain = opt->value;

	callchain->enabled = true;

	if (callchain->record_mode == CALLCHAIN_NONE)
		callchain->record_mode = CALLCHAIN_FP;

	callchain_debug(callchain);
	return 0;
}

static int perf_record_config(const char *var, const char *value, void *cb)
{
	struct record *rec = cb;

	if (!strcmp(var, "record.build-id")) {
		if (!strcmp(value, "cache"))
			rec->no_buildid_cache = false;
		else if (!strcmp(value, "no-cache"))
			rec->no_buildid_cache = true;
		else if (!strcmp(value, "skip"))
			rec->no_buildid = true;
		else
			return -1;
		return 0;
	}
	if (!strcmp(var, "record.call-graph"))
		var = "call-graph.record-mode"; /* fall-through */

	return perf_default_config(var, value, cb);
}

struct clockid_map {
	const char *name;
	int clockid;
};

#define CLOCKID_MAP(n, c)	\
	{ .name = n, .clockid = (c), }

#define CLOCKID_END	{ .name = NULL, }


/*
 * Add the missing ones, we need to build on many distros...
 */
#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW 4
#endif
#ifndef CLOCK_BOOTTIME
#define CLOCK_BOOTTIME 7
#endif
#ifndef CLOCK_TAI
#define CLOCK_TAI 11
#endif

static const struct clockid_map clockids[] = {
	/* available for all events, NMI safe */
	CLOCKID_MAP("monotonic", CLOCK_MONOTONIC),
	CLOCKID_MAP("monotonic_raw", CLOCK_MONOTONIC_RAW),

	/* available for some events */
	CLOCKID_MAP("realtime", CLOCK_REALTIME),
	CLOCKID_MAP("boottime", CLOCK_BOOTTIME),
	CLOCKID_MAP("tai", CLOCK_TAI),

	/* available for the lazy */
	CLOCKID_MAP("mono", CLOCK_MONOTONIC),
	CLOCKID_MAP("raw", CLOCK_MONOTONIC_RAW),
	CLOCKID_MAP("real", CLOCK_REALTIME),
	CLOCKID_MAP("boot", CLOCK_BOOTTIME),

	CLOCKID_END,
};

static int parse_clockid(const struct option *opt, const char *str, int unset)
{
	struct record_opts *opts = (struct record_opts *)opt->value;
	const struct clockid_map *cm;
	const char *ostr = str;

	if (unset) {
		opts->use_clockid = 0;
		return 0;
	}

	/* no arg passed */
	if (!str)
		return 0;

	/* no setting it twice */
	if (opts->use_clockid)
		return -1;

	opts->use_clockid = true;

	/* if its a number, we're done */
	if (sscanf(str, "%d", &opts->clockid) == 1)
		return 0;

	/* allow a "CLOCK_" prefix to the name */
	if (!strncasecmp(str, "CLOCK_", 6))
		str += 6;

	for (cm = clockids; cm->name; cm++) {
		if (!strcasecmp(str, cm->name)) {
			opts->clockid = cm->clockid;
			return 0;
		}
	}

	opts->use_clockid = false;
	ui__warning("unknown clockid %s, check man page\n", ostr);
	return -1;
}

static int record__parse_mmap_pages(const struct option *opt,
				    const char *str,
				    int unset __maybe_unused)
{
	struct record_opts *opts = opt->value;
	char *s, *p;
	unsigned int mmap_pages;
	int ret;

	if (!str)
		return -EINVAL;

	s = strdup(str);
	if (!s)
		return -ENOMEM;

	p = strchr(s, ',');
	if (p)
		*p = '\0';

	if (*s) {
		ret = __perf_evlist__parse_mmap_pages(&mmap_pages, s);
		if (ret)
			goto out_free;
		opts->mmap_pages = mmap_pages;
	}

	if (!p) {
		ret = 0;
		goto out_free;
	}

	ret = __perf_evlist__parse_mmap_pages(&mmap_pages, p + 1);
	if (ret)
		goto out_free;

	opts->auxtrace_mmap_pages = mmap_pages;

out_free:
	free(s);
	return ret;
}

static const char * const __record_usage[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	"perf record [<options>] [<command>]",
	"perf record [<options>] -- <command> [<options>]",
	NULL
};
<<<<<<< HEAD

/*
 * XXX Ideally would be local to cmd_record() and passed to a perf_record__new
 * because we need to have access to it in perf_record__exit, that is called
=======
const char * const *record_usage = __record_usage;

/*
 * XXX Ideally would be local to cmd_record() and passed to a record__new
 * because we need to have access to it in record__exit, that is called
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * after cmd_record() exits, but since record_options need to be accessible to
 * builtin-script, leave it here.
 *
 * At least we don't ouch it in all the other functions here directly.
 *
 * Just say no to tons of global variables, sigh.
 */
<<<<<<< HEAD
static struct perf_record record = {
	.opts = {
=======
static struct record record = {
	.opts = {
		.sample_time	     = true,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.mmap_pages	     = UINT_MAX,
		.user_freq	     = UINT_MAX,
		.user_interval	     = ULLONG_MAX,
		.freq		     = 4000,
		.target		     = {
			.uses_mmap   = true,
<<<<<<< HEAD
		},
	},
	.write_mode = WRITE_FORCE,
	.file_new   = true,
};

#define CALLCHAIN_HELP "do call-graph (stack chain/backtrace) recording: "

#ifdef LIBUNWIND_SUPPORT
const char record_callchain_help[] = CALLCHAIN_HELP "[fp] dwarf";
#else
const char record_callchain_help[] = CALLCHAIN_HELP "[fp]";
#endif
=======
			.default_per_cpu = true,
		},
		.proc_map_timeout     = 500,
	},
	.tool = {
		.sample		= process_sample_event,
		.fork		= perf_event__process_fork,
		.exit		= perf_event__process_exit,
		.comm		= perf_event__process_comm,
		.mmap		= perf_event__process_mmap,
		.mmap2		= perf_event__process_mmap2,
		.ordered_events	= true,
	},
};

const char record_callchain_help[] = CALLCHAIN_RECORD_HELP
	"\n\t\t\t\tDefault: fp";

static bool dry_run;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * XXX Will stay a global variable till we fix builtin-script.c to stop messing
 * with it and switch to use the library functions in perf_evlist that came
<<<<<<< HEAD
 * from builtin-record.c, i.e. use perf_record_opts,
 * perf_evlist__prepare_workload, etc instead of fork+exec'in 'perf record',
 * using pipes, etc.
 */
const struct option record_options[] = {
=======
 * from builtin-record.c, i.e. use record_opts,
 * perf_evlist__prepare_workload, etc instead of fork+exec'in 'perf record',
 * using pipes, etc.
 */
struct option __record_options[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_CALLBACK('e', "event", &record.evlist, "event",
		     "event selector. use 'perf list' to list available events",
		     parse_events_option),
	OPT_CALLBACK(0, "filter", &record.evlist, "filter",
		     "event filter", parse_filter),
<<<<<<< HEAD
=======
	OPT_CALLBACK_NOOPT(0, "exclude-perf", &record.evlist,
			   NULL, "don't record events from perf itself",
			   exclude_perf),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('p', "pid", &record.opts.target.pid, "pid",
		    "record events on existing process id"),
	OPT_STRING('t', "tid", &record.opts.target.tid, "tid",
		    "record events on existing thread id"),
	OPT_INTEGER('r', "realtime", &record.realtime_prio,
		    "collect data with this RT SCHED_FIFO priority"),
<<<<<<< HEAD
	OPT_BOOLEAN('D', "no-delay", &record.opts.no_delay,
=======
	OPT_BOOLEAN(0, "no-buffering", &record.opts.no_buffering,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    "collect data without buffering"),
	OPT_BOOLEAN('R', "raw-samples", &record.opts.raw_samples,
		    "collect raw sample records from all opened counters"),
	OPT_BOOLEAN('a', "all-cpus", &record.opts.target.system_wide,
			    "system-wide collection from all CPUs"),
<<<<<<< HEAD
	OPT_BOOLEAN('A', "append", &record.append_file,
			    "append to the output file to do incremental profiling"),
	OPT_STRING('C', "cpu", &record.opts.target.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_BOOLEAN('f', "force", &record.force,
			"overwrite existing data file (deprecated)"),
	OPT_U64('c', "count", &record.opts.user_interval, "event period to sample"),
	OPT_STRING('o', "output", &record.output_name, "file",
		    "output file name"),
	OPT_BOOLEAN('i', "no-inherit", &record.opts.no_inherit,
		    "child tasks do not inherit counters"),
	OPT_UINTEGER('F', "freq", &record.opts.user_freq, "profile at this frequency"),
	OPT_UINTEGER('m', "mmap-pages", &record.opts.mmap_pages,
		     "number of mmap data pages"),
	OPT_BOOLEAN(0, "group", &record.opts.group,
		    "put the counters into a counter group"),
	OPT_CALLBACK_DEFAULT('g', "call-graph", &record.opts,
			     "mode[,dump_size]", record_callchain_help,
			     &record_parse_callchain_opt, "fp"),
=======
	OPT_STRING('C', "cpu", &record.opts.target.cpu_list, "cpu",
		    "list of cpus to monitor"),
	OPT_U64('c', "count", &record.opts.user_interval, "event period to sample"),
	OPT_STRING('o', "output", &record.file.path, "file",
		    "output file name"),
	OPT_BOOLEAN_SET('i', "no-inherit", &record.opts.no_inherit,
			&record.opts.no_inherit_set,
			"child tasks do not inherit counters"),
	OPT_BOOLEAN(0, "tail-synthesize", &record.opts.tail_synthesize,
		    "synthesize non-sample events at the end of output"),
	OPT_BOOLEAN(0, "overwrite", &record.opts.overwrite, "use overwrite mode"),
	OPT_UINTEGER('F', "freq", &record.opts.user_freq, "profile at this frequency"),
	OPT_CALLBACK('m', "mmap-pages", &record.opts, "pages[,pages]",
		     "number of mmap data pages and AUX area tracing mmap pages",
		     record__parse_mmap_pages),
	OPT_BOOLEAN(0, "group", &record.opts.group,
		    "put the counters into a counter group"),
	OPT_CALLBACK_NOOPT('g', NULL, &callchain_param,
			   NULL, "enables call-graph recording" ,
			   &record_callchain_opt),
	OPT_CALLBACK(0, "call-graph", &record.opts,
		     "record_mode[,record_size]", record_callchain_help,
		     &record_parse_callchain_opt),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show counter open errors, etc)"),
	OPT_BOOLEAN('q', "quiet", &quiet, "don't print any message"),
	OPT_BOOLEAN('s', "stat", &record.opts.inherit_stat,
		    "per thread counts"),
<<<<<<< HEAD
	OPT_BOOLEAN('d', "data", &record.opts.sample_address,
		    "Sample addresses"),
	OPT_BOOLEAN('T', "timestamp", &record.opts.sample_time, "Sample timestamps"),
	OPT_BOOLEAN('P', "period", &record.opts.period, "Sample period"),
	OPT_BOOLEAN('n', "no-samples", &record.opts.no_samples,
		    "don't sample"),
	OPT_BOOLEAN('N', "no-buildid-cache", &record.no_buildid_cache,
		    "do not update the buildid cache"),
	OPT_BOOLEAN('B', "no-buildid", &record.no_buildid,
		    "do not collect buildids in perf.data"),
	OPT_CALLBACK('G', "cgroup", &record.evlist, "name",
		     "monitor event in cgroup name only",
		     parse_cgroups),
=======
	OPT_BOOLEAN('d', "data", &record.opts.sample_address, "Record the sample addresses"),
	OPT_BOOLEAN(0, "sample-cpu", &record.opts.sample_cpu, "Record the sample cpu"),
	OPT_BOOLEAN_SET('T', "timestamp", &record.opts.sample_time,
			&record.opts.sample_time_set,
			"Record the sample timestamps"),
	OPT_BOOLEAN('P', "period", &record.opts.period, "Record the sample period"),
	OPT_BOOLEAN('n', "no-samples", &record.opts.no_samples,
		    "don't sample"),
	OPT_BOOLEAN_SET('N', "no-buildid-cache", &record.no_buildid_cache,
			&record.no_buildid_cache_set,
			"do not update the buildid cache"),
	OPT_BOOLEAN_SET('B', "no-buildid", &record.no_buildid,
			&record.no_buildid_set,
			"do not collect buildids in perf.data"),
	OPT_CALLBACK('G', "cgroup", &record.evlist, "name",
		     "monitor event in cgroup name only",
		     parse_cgroups),
	OPT_UINTEGER('D', "delay", &record.opts.initial_delay,
		  "ms to wait before starting measurement after program start"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('u', "uid", &record.opts.target.uid_str, "user",
		   "user to profile"),

	OPT_CALLBACK_NOOPT('b', "branch-any", &record.opts.branch_stack,
		     "branch any", "sample any taken branches",
		     parse_branch_stack),

	OPT_CALLBACK('j', "branch-filter", &record.opts.branch_stack,
		     "branch filter mask", "branch stack filter modes",
		     parse_branch_stack),
	OPT_BOOLEAN('W', "weight", &record.opts.sample_weight,
		    "sample by weight (on special events only)"),
<<<<<<< HEAD
	OPT_END()
};

int cmd_record(int argc, const char **argv, const char *prefix __maybe_unused)
{
	int err = -ENOMEM;
	struct perf_evsel *pos;
	struct perf_evlist *evsel_list;
	struct perf_record *rec = &record;
	char errbuf[BUFSIZ];

	evsel_list = perf_evlist__new();
	if (evsel_list == NULL)
		return -ENOMEM;

	rec->evlist = evsel_list;

	argc = parse_options(argc, argv, record_options, record_usage,
			    PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc && perf_target__none(&rec->opts.target))
		usage_with_options(record_usage, record_options);

	if (rec->force && rec->append_file) {
		ui__error("Can't overwrite and append at the same time."
			  " You need to choose between -f and -A");
		usage_with_options(record_usage, record_options);
	} else if (rec->append_file) {
		rec->write_mode = WRITE_APPEND;
	} else {
		rec->write_mode = WRITE_FORCE;
	}

	if (nr_cgroups && !rec->opts.target.system_wide) {
		ui__error("cgroup monitoring only available in"
			  " system-wide mode\n");
		usage_with_options(record_usage, record_options);
	}

	symbol__init();
=======
	OPT_BOOLEAN(0, "transaction", &record.opts.sample_transaction,
		    "sample transaction flags (special events only)"),
	OPT_BOOLEAN(0, "per-thread", &record.opts.target.per_thread,
		    "use per-thread mmaps"),
	OPT_CALLBACK_OPTARG('I', "intr-regs", &record.opts.sample_intr_regs, NULL, "any register",
		    "sample selected machine registers on interrupt,"
		    " use -I ? to list register names", parse_regs),
	OPT_BOOLEAN(0, "running-time", &record.opts.running_time,
		    "Record running/enabled time of read (:S) events"),
	OPT_CALLBACK('k', "clockid", &record.opts,
	"clockid", "clockid to use for events, see clock_gettime()",
	parse_clockid),
	OPT_STRING_OPTARG('S', "snapshot", &record.opts.auxtrace_snapshot_opts,
			  "opts", "AUX area tracing Snapshot Mode", ""),
	OPT_UINTEGER(0, "proc-map-timeout", &record.opts.proc_map_timeout,
			"per thread proc mmap processing timeout in ms"),
	OPT_BOOLEAN(0, "switch-events", &record.opts.record_switch_events,
		    "Record context switch events"),
	OPT_BOOLEAN_FLAG(0, "all-kernel", &record.opts.all_kernel,
			 "Configure all used events to run in kernel space.",
			 PARSE_OPT_EXCLUSIVE),
	OPT_BOOLEAN_FLAG(0, "all-user", &record.opts.all_user,
			 "Configure all used events to run in user space.",
			 PARSE_OPT_EXCLUSIVE),
	OPT_STRING(0, "clang-path", &llvm_param.clang_path, "clang path",
		   "clang binary to use for compiling BPF scriptlets"),
	OPT_STRING(0, "clang-opt", &llvm_param.clang_opt, "clang options",
		   "options passed to clang when compiling BPF scriptlets"),
	OPT_STRING(0, "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN(0, "buildid-all", &record.buildid_all,
		    "Record build-id of all DSOs regardless of hits"),
	OPT_BOOLEAN(0, "timestamp-filename", &record.timestamp_filename,
		    "append timestamp to output filename"),
	OPT_BOOLEAN(0, "switch-output", &record.switch_output,
		    "Switch output when receive SIGUSR2"),
	OPT_BOOLEAN(0, "dry-run", &dry_run,
		    "Parse options then exit"),
	OPT_END()
};

struct option *record_options = __record_options;

int cmd_record(int argc, const char **argv, const char *prefix __maybe_unused)
{
	int err;
	struct record *rec = &record;
	char errbuf[BUFSIZ];

#ifndef HAVE_LIBBPF_SUPPORT
# define set_nobuild(s, l, c) set_option_nobuild(record_options, s, l, "NO_LIBBPF=1", c)
	set_nobuild('\0', "clang-path", true);
	set_nobuild('\0', "clang-opt", true);
# undef set_nobuild
#endif

#ifndef HAVE_BPF_PROLOGUE
# if !defined (HAVE_DWARF_SUPPORT)
#  define REASON  "NO_DWARF=1"
# elif !defined (HAVE_LIBBPF_SUPPORT)
#  define REASON  "NO_LIBBPF=1"
# else
#  define REASON  "this architecture doesn't support BPF prologue"
# endif
# define set_nobuild(s, l, c) set_option_nobuild(record_options, s, l, REASON, c)
	set_nobuild('\0', "vmlinux", true);
# undef set_nobuild
# undef REASON
#endif

	rec->evlist = perf_evlist__new();
	if (rec->evlist == NULL)
		return -ENOMEM;

	perf_config(perf_record_config, rec);

	argc = parse_options(argc, argv, record_options, record_usage,
			    PARSE_OPT_STOP_AT_NON_OPTION);
	if (!argc && target__none(&rec->opts.target))
		usage_with_options(record_usage, record_options);

	if (nr_cgroups && !rec->opts.target.system_wide) {
		usage_with_options_msg(record_usage, record_options,
			"cgroup monitoring only available in system-wide mode");

	}
	if (rec->opts.record_switch_events &&
	    !perf_can_record_switch_events()) {
		ui__error("kernel does not support recording context switch events\n");
		parse_options_usage(record_usage, record_options, "switch-events", 0);
		return -EINVAL;
	}

	if (rec->switch_output)
		rec->timestamp_filename = true;

	if (!rec->itr) {
		rec->itr = auxtrace_record__init(rec->evlist, &err);
		if (err)
			goto out;
	}

	err = auxtrace_parse_snapshot_options(rec->itr, &rec->opts,
					      rec->opts.auxtrace_snapshot_opts);
	if (err)
		goto out;

	/*
	 * Allow aliases to facilitate the lookup of symbols for address
	 * filters. Refer to auxtrace_parse_filters().
	 */
	symbol_conf.allow_aliases = true;

	symbol__init(NULL);

	err = auxtrace_parse_filters(rec->evlist);
	if (err)
		goto out;

	if (dry_run)
		goto out;

	err = bpf__setup_stdout(rec->evlist);
	if (err) {
		bpf__strerror_setup_stdout(rec->evlist, err, errbuf, sizeof(errbuf));
		pr_err("ERROR: Setup BPF stdout failed: %s\n",
			 errbuf);
		goto out;
	}

	err = -ENOMEM;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (symbol_conf.kptr_restrict)
		pr_warning(
"WARNING: Kernel address maps (/proc/{kallsyms,modules}) are restricted,\n"
"check /proc/sys/kernel/kptr_restrict.\n\n"
"Samples in kernel functions may not be resolved if a suitable vmlinux\n"
"file is not found in the buildid cache or in the vmlinux path.\n\n"
"Samples in kernel modules won't be resolved at all.\n\n"
"If some relocation was applied (e.g. kexec) symbols may be misresolved\n"
"even with a suitable vmlinux or kallsyms file.\n\n");

<<<<<<< HEAD
	if (rec->no_buildid_cache || rec->no_buildid)
		disable_buildid_cache();

	if (evsel_list->nr_entries == 0 &&
	    perf_evlist__add_default(evsel_list) < 0) {
		pr_err("Not enough memory for event selector list\n");
		goto out_symbol_exit;
	}

	err = perf_target__validate(&rec->opts.target);
	if (err) {
		perf_target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__warning("%s", errbuf);
	}

	err = perf_target__parse_uid(&rec->opts.target);
	if (err) {
		int saved_errno = errno;

		perf_target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__error("%s", errbuf);

		err = -saved_errno;
		goto out_symbol_exit;
	}

	err = -ENOMEM;
	if (perf_evlist__create_maps(evsel_list, &rec->opts.target) < 0)
		usage_with_options(record_usage, record_options);

	list_for_each_entry(pos, &evsel_list->entries, node) {
		if (perf_header__push_event(pos->attr.config, perf_evsel__name(pos)))
			goto out_free_fd;
	}

	if (rec->opts.user_interval != ULLONG_MAX)
		rec->opts.default_interval = rec->opts.user_interval;
	if (rec->opts.user_freq != UINT_MAX)
		rec->opts.freq = rec->opts.user_freq;

	/*
	 * User specified count overrides default frequency.
	 */
	if (rec->opts.default_interval)
		rec->opts.freq = 0;
	else if (rec->opts.freq) {
		rec->opts.default_interval = rec->opts.freq;
	} else {
		ui__error("frequency and count are zero, aborting\n");
		err = -EINVAL;
		goto out_free_fd;
	}

	err = __cmd_record(&record, argc, argv);

	perf_evlist__munmap(evsel_list);
	perf_evlist__close(evsel_list);
out_free_fd:
	perf_evlist__delete_maps(evsel_list);
out_symbol_exit:
	symbol__exit();
	return err;
}
=======
	if (rec->no_buildid_cache || rec->no_buildid) {
		disable_buildid_cache();
	} else if (rec->switch_output) {
		/*
		 * In 'perf record --switch-output', disable buildid
		 * generation by default to reduce data file switching
		 * overhead. Still generate buildid if they are required
		 * explicitly using
		 *
		 *  perf record --signal-trigger --no-no-buildid \
		 *              --no-no-buildid-cache
		 *
		 * Following code equals to:
		 *
		 * if ((rec->no_buildid || !rec->no_buildid_set) &&
		 *     (rec->no_buildid_cache || !rec->no_buildid_cache_set))
		 *         disable_buildid_cache();
		 */
		bool disable = true;

		if (rec->no_buildid_set && !rec->no_buildid)
			disable = false;
		if (rec->no_buildid_cache_set && !rec->no_buildid_cache)
			disable = false;
		if (disable) {
			rec->no_buildid = true;
			rec->no_buildid_cache = true;
			disable_buildid_cache();
		}
	}

	if (record.opts.overwrite)
		record.opts.tail_synthesize = true;

	if (rec->evlist->nr_entries == 0 &&
	    perf_evlist__add_default(rec->evlist) < 0) {
		pr_err("Not enough memory for event selector list\n");
		goto out;
	}

	if (rec->opts.target.tid && !rec->opts.no_inherit_set)
		rec->opts.no_inherit = true;

	err = target__validate(&rec->opts.target);
	if (err) {
		target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__warning("%s", errbuf);
	}

	err = target__parse_uid(&rec->opts.target);
	if (err) {
		int saved_errno = errno;

		target__strerror(&rec->opts.target, err, errbuf, BUFSIZ);
		ui__error("%s", errbuf);

		err = -saved_errno;
		goto out;
	}

	err = -ENOMEM;
	if (perf_evlist__create_maps(rec->evlist, &rec->opts.target) < 0)
		usage_with_options(record_usage, record_options);

	err = auxtrace_record__options(rec->itr, rec->evlist, &rec->opts);
	if (err)
		goto out;

	/*
	 * We take all buildids when the file contains
	 * AUX area tracing data because we do not decode the
	 * trace because it would take too long.
	 */
	if (rec->opts.full_auxtrace)
		rec->buildid_all = true;

	if (record_opts__config(&rec->opts)) {
		err = -EINVAL;
		goto out;
	}

	err = __cmd_record(&record, argc, argv);
out:
	perf_evlist__delete(rec->evlist);
	symbol__exit();
	auxtrace_record__free(rec->itr);
	return err;
}

static void snapshot_sig_handler(int sig __maybe_unused)
{
	if (trigger_is_ready(&auxtrace_snapshot_trigger)) {
		trigger_hit(&auxtrace_snapshot_trigger);
		auxtrace_record__snapshot_started = 1;
		if (auxtrace_record__snapshot_start(record.itr))
			trigger_error(&auxtrace_snapshot_trigger);
	}

	if (trigger_is_ready(&switch_output_trigger))
		trigger_hit(&switch_output_trigger);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
