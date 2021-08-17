#ifndef __PERF_TOOL_H
#define __PERF_TOOL_H

#include <stdbool.h>

<<<<<<< HEAD
=======
#include <linux/types.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct perf_session;
union perf_event;
struct perf_evlist;
struct perf_evsel;
struct perf_sample;
struct perf_tool;
struct machine;
<<<<<<< HEAD
=======
struct ordered_events;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef int (*event_sample)(struct perf_tool *tool, union perf_event *event,
			    struct perf_sample *sample,
			    struct perf_evsel *evsel, struct machine *machine);

typedef int (*event_op)(struct perf_tool *tool, union perf_event *event,
			struct perf_sample *sample, struct machine *machine);

<<<<<<< HEAD
typedef int (*event_attr_op)(union perf_event *event,
			     struct perf_evlist **pevlist);
typedef int (*event_simple_op)(struct perf_tool *tool, union perf_event *event);

typedef int (*event_synth_op)(union perf_event *event,
			      struct perf_session *session);
=======
typedef int (*event_attr_op)(struct perf_tool *tool,
			     union perf_event *event,
			     struct perf_evlist **pevlist);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

typedef int (*event_op2)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);

<<<<<<< HEAD
=======
typedef int (*event_oe)(struct perf_tool *tool, union perf_event *event,
			struct ordered_events *oe);

typedef s64 (*event_op3)(struct perf_tool *tool, union perf_event *event,
			 struct perf_session *session);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct perf_tool {
	event_sample	sample,
			read;
	event_op	mmap,
<<<<<<< HEAD
=======
			mmap2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			comm,
			fork,
			exit,
			lost,
<<<<<<< HEAD
			throttle,
			unthrottle;
	event_attr_op	attr;
	event_synth_op	tracing_data;
	event_simple_op	event_type;
	event_op2	finished_round,
			build_id;
	bool		ordered_samples;
=======
			lost_samples,
			aux,
			itrace_start,
			context_switch,
			throttle,
			unthrottle;
	event_attr_op	attr;
	event_attr_op	event_update;
	event_op2	tracing_data;
	event_oe	finished_round;
	event_op2	build_id,
			id_index,
			auxtrace_info,
			auxtrace_error,
			time_conv,
			thread_map,
			cpu_map,
			stat_config,
			stat,
			stat_round;
	event_op3	auxtrace;
	bool		ordered_events;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool		ordering_requires_timestamps;
};

#endif /* __PERF_TOOL_H */
