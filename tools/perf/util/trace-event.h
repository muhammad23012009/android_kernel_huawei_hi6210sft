#ifndef _PERF_UTIL_TRACE_EVENT_H
#define _PERF_UTIL_TRACE_EVENT_H

<<<<<<< HEAD
#include "parse-events.h"
#include "event-parse.h"
#include "session.h"
=======
#include <traceevent/event-parse.h>
#include "parse-events.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct machine;
struct perf_sample;
union perf_event;
struct perf_tool;
<<<<<<< HEAD

extern int header_page_size_size;
extern int header_page_ts_size;
extern int header_page_data_offset;

extern bool latency_format;
extern struct pevent *perf_pevent;

enum {
	RINGBUF_TYPE_PADDING		= 29,
	RINGBUF_TYPE_TIME_EXTEND	= 30,
	RINGBUF_TYPE_TIME_STAMP		= 31,
};

#ifndef TS_SHIFT
#define TS_SHIFT		27
#endif

int bigendian(void);

struct pevent *read_trace_init(int file_bigendian, int host_bigendian);
=======
struct thread;
struct plugin_list;

struct trace_event {
	struct pevent		*pevent;
	struct plugin_list	*plugin_list;
};

int trace_event__init(struct trace_event *t);
void trace_event__cleanup(struct trace_event *t);
int trace_event__register_resolver(struct machine *machine,
				   pevent_func_resolver_t *func);
struct event_format*
trace_event__tp_format(const char *sys, const char *name);

struct event_format *trace_event__tp_format_id(int id);

int bigendian(void);

void event_format__fprintf(struct event_format *event,
			   int cpu, void *data, int size, FILE *fp);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void event_format__print(struct event_format *event,
			 int cpu, void *data, int size);

int parse_ftrace_file(struct pevent *pevent, char *buf, unsigned long size);
int parse_event_file(struct pevent *pevent,
		     char *buf, unsigned long size, char *sys);

<<<<<<< HEAD
struct pevent_record *trace_peek_data(struct pevent *pevent, int cpu);

unsigned long long
raw_field_value(struct event_format *event, const char *name, void *data);
void *raw_field_ptr(struct event_format *event, const char *name, void *data);
=======
unsigned long long
raw_field_value(struct event_format *event, const char *name, void *data);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void parse_proc_kallsyms(struct pevent *pevent, char *file, unsigned int size);
void parse_ftrace_printk(struct pevent *pevent, char *file, unsigned int size);

<<<<<<< HEAD
ssize_t trace_report(int fd, struct pevent **pevent, bool repipe);

int trace_parse_common_type(struct pevent *pevent, void *data);
int trace_parse_common_pid(struct pevent *pevent, void *data);
=======
ssize_t trace_report(int fd, struct trace_event *tevent, bool repipe);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct event_format *trace_find_next_event(struct pevent *pevent,
					   struct event_format *event);
unsigned long long read_size(struct event_format *event, void *ptr, int size);
unsigned long long eval_flag(const char *flag);

<<<<<<< HEAD
struct pevent_record *trace_read_data(struct pevent *pevent, int cpu);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int read_tracing_data(int fd, struct list_head *pattrs);

struct tracing_data {
	/* size is only valid if temp is 'true' */
	ssize_t size;
	bool temp;
	char temp_file[50];
};

struct tracing_data *tracing_data_get(struct list_head *pattrs,
				      int fd, bool temp);
int tracing_data_put(struct tracing_data *tdata);


struct addr_location;

struct perf_session;
<<<<<<< HEAD
=======
struct perf_stat_config;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct scripting_ops {
	const char *name;
	int (*start_script) (const char *script, int argc, const char **argv);
<<<<<<< HEAD
=======
	int (*flush_script) (void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int (*stop_script) (void);
	void (*process_event) (union perf_event *event,
			       struct perf_sample *sample,
			       struct perf_evsel *evsel,
<<<<<<< HEAD
			       struct machine *machine,
			       struct addr_location *al);
	int (*generate_script) (struct pevent *pevent, const char *outfile);
};

=======
			       struct addr_location *al);
	void (*process_stat)(struct perf_stat_config *config,
			     struct perf_evsel *evsel, u64 tstamp);
	void (*process_stat_interval)(u64 tstamp);
	int (*generate_script) (struct pevent *pevent, const char *outfile);
};

extern unsigned int scripting_max_stack;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int script_spec_register(const char *spec, struct scripting_ops *ops);

void setup_perl_scripting(void);
void setup_python_scripting(void);

struct scripting_context {
	struct pevent *pevent;
	void *event_data;
};

int common_pc(struct scripting_context *context);
int common_flags(struct scripting_context *context);
int common_lock_depth(struct scripting_context *context);

#endif /* _PERF_UTIL_TRACE_EVENT_H */
