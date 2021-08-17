#include "builtin.h"
#include "perf.h"

<<<<<<< HEAD
#include "util/parse-options.h"
#include "util/trace-event.h"
#include "util/tool.h"
#include "util/session.h"

#define MEM_OPERATION_LOAD	"load"
#define MEM_OPERATION_STORE	"store"

static const char	*mem_operation		= MEM_OPERATION_LOAD;
=======
#include <subcmd/parse-options.h>
#include "util/trace-event.h"
#include "util/tool.h"
#include "util/session.h"
#include "util/data.h"
#include "util/mem-events.h"
#include "util/debug.h"

#define MEM_OPERATION_LOAD	0x1
#define MEM_OPERATION_STORE	0x2
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct perf_mem {
	struct perf_tool	tool;
	char const		*input_name;
<<<<<<< HEAD
	symbol_filter_t		annotate_init;
	bool			hide_unresolved;
	bool			dump_raw;
=======
	bool			hide_unresolved;
	bool			dump_raw;
	bool			force;
	int			operation;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char		*cpu_list;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

<<<<<<< HEAD
static const char * const mem_usage[] = {
	"perf mem [<options>] {record <command> |report}",
	NULL
};

static int __cmd_record(int argc, const char **argv)
{
	int rec_argc, i = 0, j;
	const char **rec_argv;
	char event[64];
	int ret;

	rec_argc = argc + 4;
=======
static int parse_record_events(const struct option *opt,
			       const char *str, int unset __maybe_unused)
{
	struct perf_mem *mem = *(struct perf_mem **)opt->value;
	int j;

	if (strcmp(str, "list")) {
		if (!perf_mem_events__parse(str)) {
			mem->operation = 0;
			return 0;
		}
		exit(-1);
	}

	for (j = 0; j < PERF_MEM_EVENTS__MAX; j++) {
		struct perf_mem_event *e = &perf_mem_events[j];

		fprintf(stderr, "%-13s%-*s%s\n",
			e->tag,
			verbose ? 25 : 0,
			verbose ? perf_mem_events__name(j) : "",
			e->supported ? ": available" : "");
	}
	exit(0);
}

static const char * const __usage[] = {
	"perf mem record [<options>] [<command>]",
	"perf mem record [<options>] -- <command> [<options>]",
	NULL
};

static const char * const *record_mem_usage = __usage;

static int __cmd_record(int argc, const char **argv, struct perf_mem *mem)
{
	int rec_argc, i = 0, j;
	const char **rec_argv;
	int ret;
	bool all_user = false, all_kernel = false;
	struct option options[] = {
	OPT_CALLBACK('e', "event", &mem, "event",
		     "event selector. use 'perf mem record -e list' to list available events",
		     parse_record_events),
	OPT_UINTEGER(0, "ldlat", &perf_mem_events__loads_ldlat, "mem-loads latency"),
	OPT_INCR('v', "verbose", &verbose,
		 "be more verbose (show counter open errors, etc)"),
	OPT_BOOLEAN('U', "all-user", &all_user, "collect only user level data"),
	OPT_BOOLEAN('K', "all-kernel", &all_kernel, "collect only kernel level data"),
	OPT_END()
	};

	argc = parse_options(argc, argv, options, record_mem_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	rec_argc = argc + 9; /* max number of arguments */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rec_argv = calloc(rec_argc + 1, sizeof(char *));
	if (!rec_argv)
		return -1;

<<<<<<< HEAD
	rec_argv[i++] = strdup("record");
	if (!strcmp(mem_operation, MEM_OPERATION_LOAD))
		rec_argv[i++] = strdup("-W");
	rec_argv[i++] = strdup("-d");
	rec_argv[i++] = strdup("-e");

	if (strcmp(mem_operation, MEM_OPERATION_LOAD))
		sprintf(event, "cpu/mem-stores/pp");
	else
		sprintf(event, "cpu/mem-loads/pp");

	rec_argv[i++] = strdup(event);
	for (j = 1; j < argc; j++, i++)
		rec_argv[i] = argv[j];

=======
	rec_argv[i++] = "record";

	if (mem->operation & MEM_OPERATION_LOAD)
		perf_mem_events[PERF_MEM_EVENTS__LOAD].record = true;

	if (mem->operation & MEM_OPERATION_STORE)
		perf_mem_events[PERF_MEM_EVENTS__STORE].record = true;

	if (perf_mem_events[PERF_MEM_EVENTS__LOAD].record)
		rec_argv[i++] = "-W";

	rec_argv[i++] = "-d";

	for (j = 0; j < PERF_MEM_EVENTS__MAX; j++) {
		if (!perf_mem_events[j].record)
			continue;

		if (!perf_mem_events[j].supported) {
			pr_err("failed: event '%s' not supported\n",
			       perf_mem_events__name(j));
			return -1;
		}

		rec_argv[i++] = "-e";
		rec_argv[i++] = perf_mem_events__name(j);
	};

	if (all_user)
		rec_argv[i++] = "--all-user";

	if (all_kernel)
		rec_argv[i++] = "--all-kernel";

	for (j = 0; j < argc; j++, i++)
		rec_argv[i] = argv[j];

	if (verbose > 0) {
		pr_debug("calling: record ");

		while (rec_argv[j]) {
			pr_debug("%s ", rec_argv[j]);
			j++;
		}
		pr_debug("\n");
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = cmd_record(i, rec_argv, NULL);
	free(rec_argv);
	return ret;
}

static int
dump_raw_samples(struct perf_tool *tool,
		 union perf_event *event,
		 struct perf_sample *sample,
<<<<<<< HEAD
		 struct perf_evsel *evsel __maybe_unused,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		 struct machine *machine)
{
	struct perf_mem *mem = container_of(tool, struct perf_mem, tool);
	struct addr_location al;
	const char *fmt;

<<<<<<< HEAD
	if (perf_event__preprocess_sample(event, machine, &al, sample,
				mem->annotate_init) < 0) {
=======
	if (machine__resolve(machine, &al, sample) < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		fprintf(stderr, "problem processing %d event, skipping it.\n",
				event->header.type);
		return -1;
	}

	if (al.filtered || (mem->hide_unresolved && al.sym == NULL))
<<<<<<< HEAD
		return 0;
=======
		goto out_put;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (al.map != NULL)
		al.map->dso->hit = 1;

	if (symbol_conf.field_sep) {
		fmt = "%d%s%d%s0x%"PRIx64"%s0x%"PRIx64"%s%"PRIu64
		      "%s0x%"PRIx64"%s%s:%s\n";
	} else {
		fmt = "%5d%s%5d%s0x%016"PRIx64"%s0x016%"PRIx64
		      "%s%5"PRIu64"%s0x%06"PRIx64"%s%s:%s\n";
		symbol_conf.field_sep = " ";
	}

	printf(fmt,
		sample->pid,
		symbol_conf.field_sep,
		sample->tid,
		symbol_conf.field_sep,
<<<<<<< HEAD
		event->ip.ip,
=======
		sample->ip,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		symbol_conf.field_sep,
		sample->addr,
		symbol_conf.field_sep,
		sample->weight,
		symbol_conf.field_sep,
		sample->data_src,
		symbol_conf.field_sep,
		al.map ? (al.map->dso ? al.map->dso->long_name : "???") : "???",
		al.sym ? al.sym->name : "???");
<<<<<<< HEAD

=======
out_put:
	addr_location__put(&al);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
<<<<<<< HEAD
				struct perf_evsel *evsel,
				struct machine *machine)
{
	return dump_raw_samples(tool, event, sample, evsel, machine);
=======
				struct perf_evsel *evsel __maybe_unused,
				struct machine *machine)
{
	return dump_raw_samples(tool, event, sample, machine);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int report_raw_events(struct perf_mem *mem)
{
<<<<<<< HEAD
	int err = -EINVAL;
	int ret;
	struct perf_session *session = perf_session__new(input_name, O_RDONLY,
							 0, false, &mem->tool);

	if (session == NULL)
		return -ENOMEM;
=======
	struct perf_data_file file = {
		.path = input_name,
		.mode = PERF_DATA_MODE_READ,
		.force = mem->force,
	};
	int ret;
	struct perf_session *session = perf_session__new(&file, false,
							 &mem->tool);

	if (session == NULL)
		return -1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (mem->cpu_list) {
		ret = perf_session__cpu_bitmap(session, mem->cpu_list,
					       mem->cpu_bitmap);
<<<<<<< HEAD
		if (ret)
			goto out_delete;
	}

	if (symbol__init() < 0)
		return -1;

	printf("# PID, TID, IP, ADDR, LOCAL WEIGHT, DSRC, SYMBOL\n");

	err = perf_session__process_events(session, &mem->tool);
	if (err)
		return err;

	return 0;

out_delete:
	perf_session__delete(session);
	return err;
=======
		if (ret < 0)
			goto out_delete;
	}

	ret = symbol__init(&session->header.env);
	if (ret < 0)
		goto out_delete;

	printf("# PID, TID, IP, ADDR, LOCAL WEIGHT, DSRC, SYMBOL\n");

	ret = perf_session__process_events(session);

out_delete:
	perf_session__delete(session);
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int report_events(int argc, const char **argv, struct perf_mem *mem)
{
	const char **rep_argv;
	int ret, i = 0, j, rep_argc;

	if (mem->dump_raw)
		return report_raw_events(mem);

	rep_argc = argc + 3;
	rep_argv = calloc(rep_argc + 1, sizeof(char *));
	if (!rep_argv)
		return -1;

<<<<<<< HEAD
	rep_argv[i++] = strdup("report");
	rep_argv[i++] = strdup("--mem-mode");
	rep_argv[i++] = strdup("-n"); /* display number of samples */
=======
	rep_argv[i++] = "report";
	rep_argv[i++] = "--mem-mode";
	rep_argv[i++] = "-n"; /* display number of samples */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * there is no weight (cost) associated with stores, so don't print
	 * the column
	 */
<<<<<<< HEAD
	if (strcmp(mem_operation, MEM_OPERATION_LOAD))
		rep_argv[i++] = strdup("--sort=mem,sym,dso,symbol_daddr,"
				       "dso_daddr,tlb,locked");
=======
	if (!(mem->operation & MEM_OPERATION_LOAD))
		rep_argv[i++] = "--sort=mem,sym,dso,symbol_daddr,"
				"dso_daddr,tlb,locked";
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (j = 1; j < argc; j++, i++)
		rep_argv[i] = argv[j];

	ret = cmd_report(i, rep_argv, NULL);
	free(rep_argv);
	return ret;
}

<<<<<<< HEAD
=======
struct mem_mode {
	const char *name;
	int mode;
};

#define MEM_OPT(n, m) \
	{ .name = n, .mode = (m) }

#define MEM_END { .name = NULL }

static const struct mem_mode mem_modes[]={
	MEM_OPT("load", MEM_OPERATION_LOAD),
	MEM_OPT("store", MEM_OPERATION_STORE),
	MEM_END
};

static int
parse_mem_ops(const struct option *opt, const char *str, int unset)
{
	int *mode = (int *)opt->value;
	const struct mem_mode *m;
	char *s, *os = NULL, *p;
	int ret = -1;

	if (unset)
		return 0;

	/* str may be NULL in case no arg is passed to -t */
	if (str) {
		/* because str is read-only */
		s = os = strdup(str);
		if (!s)
			return -1;

		/* reset mode */
		*mode = 0;

		for (;;) {
			p = strchr(s, ',');
			if (p)
				*p = '\0';

			for (m = mem_modes; m->name; m++) {
				if (!strcasecmp(s, m->name))
					break;
			}
			if (!m->name) {
				fprintf(stderr, "unknown sampling op %s,"
					    " check man page\n", s);
				goto error;
			}

			*mode |= m->mode;

			if (!p)
				break;

			s = p + 1;
		}
	}
	ret = 0;

	if (*mode == 0)
		*mode = MEM_OPERATION_LOAD;
error:
	free(os);
	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int cmd_mem(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct stat st;
	struct perf_mem mem = {
		.tool = {
			.sample		= process_sample_event,
			.mmap		= perf_event__process_mmap,
<<<<<<< HEAD
=======
			.mmap2		= perf_event__process_mmap2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.comm		= perf_event__process_comm,
			.lost		= perf_event__process_lost,
			.fork		= perf_event__process_fork,
			.build_id	= perf_event__process_build_id,
<<<<<<< HEAD
			.ordered_samples = true,
		},
		.input_name		 = "perf.data",
	};
	const struct option mem_options[] = {
	OPT_STRING('t', "type", &mem_operation,
		   "type", "memory operations(load/store)"),
=======
			.ordered_events	= true,
		},
		.input_name		 = "perf.data",
		/*
		 * default to both load an store sampling
		 */
		.operation		 = MEM_OPERATION_LOAD | MEM_OPERATION_STORE,
	};
	const struct option mem_options[] = {
	OPT_CALLBACK('t', "type", &mem.operation,
		   "type", "memory operations(load,store) Default load,store",
		    parse_mem_ops),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_BOOLEAN('D', "dump-raw-samples", &mem.dump_raw,
		    "dump raw samples in ASCII"),
	OPT_BOOLEAN('U', "hide-unresolved", &mem.hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_STRING('i', "input", &input_name, "file",
		   "input file name"),
	OPT_STRING('C', "cpu", &mem.cpu_list, "cpu",
		   "list of cpus to profile"),
<<<<<<< HEAD
	OPT_STRING('x', "field-separator", &symbol_conf.field_sep,
		   "separator",
		   "separator for columns, no spaces will be added"
		   " between columns '.' is reserved."),
	OPT_END()
	};

	argc = parse_options(argc, argv, mem_options, mem_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	if (!argc || !(strncmp(argv[0], "rec", 3) || mem_operation))
=======
	OPT_STRING_NOEMPTY('x', "field-separator", &symbol_conf.field_sep,
		   "separator",
		   "separator for columns, no spaces will be added"
		   " between columns '.' is reserved."),
	OPT_BOOLEAN('f', "force", &mem.force, "don't complain, do it"),
	OPT_END()
	};
	const char *const mem_subcommands[] = { "record", "report", NULL };
	const char *mem_usage[] = {
		NULL,
		NULL
	};

	if (perf_mem_events__init()) {
		pr_err("failed: memory events not supported\n");
		return -1;
	}

	argc = parse_options_subcommand(argc, argv, mem_options, mem_subcommands,
					mem_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	if (!argc || !(strncmp(argv[0], "rec", 3) || mem.operation))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		usage_with_options(mem_usage, mem_options);

	if (!mem.input_name || !strlen(mem.input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			mem.input_name = "-";
		else
			mem.input_name = "perf.data";
	}

	if (!strncmp(argv[0], "rec", 3))
<<<<<<< HEAD
		return __cmd_record(argc, argv);
=======
		return __cmd_record(argc, argv, &mem);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	else if (!strncmp(argv[0], "rep", 3))
		return report_events(argc, argv, &mem);
	else
		usage_with_options(mem_usage, mem_options);

	return 0;
}
