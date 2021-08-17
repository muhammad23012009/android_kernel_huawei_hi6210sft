/*
 * Builtin evlist command: Show the list of event selectors present
 * in a perf.data file.
 */
#include "builtin.h"

#include "util/util.h"

#include <linux/list.h>

#include "perf.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/parse-events.h"
<<<<<<< HEAD
#include "util/parse-options.h"
#include "util/session.h"
=======
#include <subcmd/parse-options.h>
#include "util/session.h"
#include "util/data.h"
#include "util/debug.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int __cmd_evlist(const char *file_name, struct perf_attr_details *details)
{
	struct perf_session *session;
	struct perf_evsel *pos;
<<<<<<< HEAD

	session = perf_session__new(file_name, O_RDONLY, 0, false, NULL);
	if (session == NULL)
		return -ENOMEM;

	list_for_each_entry(pos, &session->evlist->entries, node)
		perf_evsel__fprintf(pos, details, stdout);

=======
	struct perf_data_file file = {
		.path = file_name,
		.mode = PERF_DATA_MODE_READ,
		.force = details->force,
	};
	bool has_tracepoint = false;

	session = perf_session__new(&file, 0, NULL);
	if (session == NULL)
		return -1;

	evlist__for_each_entry(session->evlist, pos) {
		perf_evsel__fprintf(pos, details, stdout);

		if (pos->attr.type == PERF_TYPE_TRACEPOINT)
			has_tracepoint = true;
	}

	if (has_tracepoint && !details->trace_fields)
		printf("# Tip: use 'perf evlist --trace-fields' to show fields for tracepoint events\n");

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_session__delete(session);
	return 0;
}

int cmd_evlist(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct perf_attr_details details = { .verbose = false, };
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file", "Input file name"),
	OPT_BOOLEAN('F', "freq", &details.freq, "Show the sample frequency"),
	OPT_BOOLEAN('v', "verbose", &details.verbose,
		    "Show all event attr details"),
	OPT_BOOLEAN('g', "group", &details.event_group,
		    "Show event group information"),
<<<<<<< HEAD
=======
	OPT_BOOLEAN('f', "force", &details.force, "don't complain, do it"),
	OPT_BOOLEAN(0, "trace-fields", &details.trace_fields, "Show tracepoint fields"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_END()
	};
	const char * const evlist_usage[] = {
		"perf evlist [<options>]",
		NULL
	};

	argc = parse_options(argc, argv, options, evlist_usage, 0);
	if (argc)
		usage_with_options(evlist_usage, options);

	if (details.event_group && (details.verbose || details.freq)) {
<<<<<<< HEAD
		pr_err("--group option is not compatible with other options\n");
		usage_with_options(evlist_usage, options);
=======
		usage_with_options_msg(evlist_usage, options,
			"--group option is not compatible with other options\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return __cmd_evlist(input_name, &details);
}
