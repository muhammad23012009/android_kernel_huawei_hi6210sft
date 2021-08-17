#include "evlist.h"
#include "evsel.h"
#include "parse-events.h"
#include "tests.h"
<<<<<<< HEAD
=======
#include "debug.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int perf_evsel__roundtrip_cache_name_test(void)
{
	char name[128];
	int type, op, err = 0, ret = 0, i, idx;
	struct perf_evsel *evsel;
	struct perf_evlist *evlist = perf_evlist__new();

        if (evlist == NULL)
                return -ENOMEM;

	for (type = 0; type < PERF_COUNT_HW_CACHE_MAX; type++) {
		for (op = 0; op < PERF_COUNT_HW_CACHE_OP_MAX; op++) {
			/* skip invalid cache type */
			if (!perf_evsel__is_cache_op_valid(type, op))
				continue;

			for (i = 0; i < PERF_COUNT_HW_CACHE_RESULT_MAX; i++) {
				__perf_evsel__hw_cache_type_op_res_name(type, op, i,
									name, sizeof(name));
<<<<<<< HEAD
				err = parse_events(evlist, name);
=======
				err = parse_events(evlist, name, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (err)
					ret = err;
			}
		}
	}

	idx = 0;
	evsel = perf_evlist__first(evlist);

	for (type = 0; type < PERF_COUNT_HW_CACHE_MAX; type++) {
		for (op = 0; op < PERF_COUNT_HW_CACHE_OP_MAX; op++) {
			/* skip invalid cache type */
			if (!perf_evsel__is_cache_op_valid(type, op))
				continue;

			for (i = 0; i < PERF_COUNT_HW_CACHE_RESULT_MAX; i++) {
				__perf_evsel__hw_cache_type_op_res_name(type, op, i,
									name, sizeof(name));
				if (evsel->idx != idx)
					continue;

				++idx;

				if (strcmp(perf_evsel__name(evsel), name)) {
					pr_debug("%s != %s\n", perf_evsel__name(evsel), name);
					ret = -1;
				}

				evsel = perf_evsel__next(evsel);
			}
		}
	}

	perf_evlist__delete(evlist);
	return ret;
}

static int __perf_evsel__name_array_test(const char *names[], int nr_names)
{
	int i, err;
	struct perf_evsel *evsel;
	struct perf_evlist *evlist = perf_evlist__new();

        if (evlist == NULL)
                return -ENOMEM;

	for (i = 0; i < nr_names; ++i) {
<<<<<<< HEAD
		err = parse_events(evlist, names[i]);
=======
		err = parse_events(evlist, names[i], NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (err) {
			pr_debug("failed to parse event '%s', err %d\n",
				 names[i], err);
			goto out_delete_evlist;
		}
	}

	err = 0;
<<<<<<< HEAD
	list_for_each_entry(evsel, &evlist->entries, node) {
=======
	evlist__for_each_entry(evlist, evsel) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (strcmp(perf_evsel__name(evsel), names[evsel->idx])) {
			--err;
			pr_debug("%s != %s\n", perf_evsel__name(evsel), names[evsel->idx]);
		}
	}

out_delete_evlist:
	perf_evlist__delete(evlist);
	return err;
}

#define perf_evsel__name_array_test(names) \
	__perf_evsel__name_array_test(names, ARRAY_SIZE(names))

<<<<<<< HEAD
int test__perf_evsel__roundtrip_name_test(void)
=======
int test__perf_evsel__roundtrip_name_test(int subtest __maybe_unused)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int err = 0, ret = 0;

	err = perf_evsel__name_array_test(perf_evsel__hw_names);
	if (err)
		ret = err;

<<<<<<< HEAD
	err = perf_evsel__name_array_test(perf_evsel__sw_names);
=======
	err = __perf_evsel__name_array_test(perf_evsel__sw_names,
					    PERF_COUNT_SW_DUMMY + 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err)
		ret = err;

	err = perf_evsel__roundtrip_cache_name_test();
	if (err)
		ret = err;

	return ret;
}
