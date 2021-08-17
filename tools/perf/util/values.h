#ifndef __PERF_VALUES_H
#define __PERF_VALUES_H

<<<<<<< HEAD
#include "types.h"
=======
#include <linux/types.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct perf_read_values {
	int threads;
	int threads_max;
	u32 *pid, *tid;
	int counters;
	int counters_max;
	u64 *counterrawid;
	char **countername;
	u64 **value;
};

void perf_read_values_init(struct perf_read_values *values);
void perf_read_values_destroy(struct perf_read_values *values);

void perf_read_values_add_value(struct perf_read_values *values,
				u32 pid, u32 tid,
				u64 rawid, const char *name, u64 value);

void perf_read_values_display(FILE *fp, struct perf_read_values *values,
			      int raw);

#endif /* __PERF_VALUES_H */
