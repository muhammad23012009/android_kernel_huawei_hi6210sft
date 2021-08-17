/*
 * builtin-report.c
 *
 * Builtin report command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

#include "util/util.h"
<<<<<<< HEAD
#include "util/cache.h"
=======
#include "util/config.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "util/annotate.h"
#include "util/color.h"
#include <linux/list.h>
#include <linux/rbtree.h>
#include "util/symbol.h"
#include "util/callchain.h"
#include "util/strlist.h"
#include "util/values.h"

#include "perf.h"
#include "util/debug.h"
#include "util/evlist.h"
#include "util/evsel.h"
#include "util/header.h"
#include "util/session.h"
#include "util/tool.h"

<<<<<<< HEAD
#include "util/parse-options.h"
=======
#include <subcmd/parse-options.h>
#include <subcmd/exec-cmd.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "util/parse-events.h"

#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"
<<<<<<< HEAD
#include "arch/common.h"

#include <linux/bitmap.h>

struct perf_report {
	struct perf_tool	tool;
	struct perf_session	*session;
	bool			force, use_tui, use_gtk, use_stdio;
	bool			hide_unresolved;
	bool			dont_use_callchains;
=======
#include "util/data.h"
#include "arch/common.h"

#include "util/auxtrace.h"

#include <dlfcn.h>
#include <linux/bitmap.h>
#include <linux/stringify.h>

struct report {
	struct perf_tool	tool;
	struct perf_session	*session;
	bool			use_tui, use_gtk, use_stdio;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool			show_full_info;
	bool			show_threads;
	bool			inverted_callchain;
	bool			mem_mode;
<<<<<<< HEAD
	struct perf_read_values	show_threads_values;
	const char		*pretty_printing_style;
	symbol_filter_t		annotate_init;
	const char		*cpu_list;
	const char		*symbol_filter_str;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

static int perf_report_config(const char *var, const char *value, void *cb)
{
=======
	bool			header;
	bool			header_only;
	bool			nonany_branch_mode;
	int			max_stack;
	struct perf_read_values	show_threads_values;
	const char		*pretty_printing_style;
	const char		*cpu_list;
	const char		*symbol_filter_str;
	float			min_percent;
	u64			nr_entries;
	u64			queue_size;
	int			socket_filter;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

static int report__config(const char *var, const char *value, void *cb)
{
	struct report *rep = cb;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!strcmp(var, "report.group")) {
		symbol_conf.event_group = perf_config_bool(var, value);
		return 0;
	}
<<<<<<< HEAD

	return perf_default_config(var, value, cb);
}

static int perf_report__add_mem_hist_entry(struct perf_tool *tool,
					   struct addr_location *al,
					   struct perf_sample *sample,
					   struct perf_evsel *evsel,
					   struct machine *machine,
					   union perf_event *event)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
	struct symbol *parent = NULL;
	u8 cpumode = event->header.misc & PERF_RECORD_MISC_CPUMODE_MASK;
	int err = 0;
	struct hist_entry *he;
	struct mem_info *mi, *mx;
	uint64_t cost;

	if ((sort__has_parent || symbol_conf.use_callchain) &&
	    sample->callchain) {
		err = machine__resolve_callchain(machine, evsel, al->thread,
						 sample, &parent);
		if (err)
			return err;
	}

	mi = machine__resolve_mem(machine, al->thread, sample, cpumode);
	if (!mi)
		return -ENOMEM;

	if (rep->hide_unresolved && !al->sym)
		return 0;

	cost = sample->weight;
	if (!cost)
		cost = 1;

	/*
	 * must pass period=weight in order to get the correct
	 * sorting from hists__collapse_resort() which is solely
	 * based on periods. We want sorting be done on nr_events * weight
	 * and this is indirectly achieved by passing period=weight here
	 * and the he_stat__add_period() function.
	 */
	he = __hists__add_mem_entry(&evsel->hists, al, parent, mi, cost, cost);
	if (!he)
		return -ENOMEM;

	/*
	 * In the TUI browser, we are doing integrated annotation,
	 * so we don't allocate the extra space needed because the stdio
	 * code will not use it.
	 */
	if (sort__has_sym && he->ms.sym && use_browser > 0) {
		struct annotation *notes = symbol__annotation(he->ms.sym);

		assert(evsel != NULL);

		if (notes->src == NULL && symbol__alloc_hist(he->ms.sym) < 0)
			goto out;

		err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
		if (err)
			goto out;
	}

	if (sort__has_sym && he->mem_info->daddr.sym && use_browser > 0) {
		struct annotation *notes;

		mx = he->mem_info;

		notes = symbol__annotation(mx->daddr.sym);
		if (notes->src == NULL && symbol__alloc_hist(mx->daddr.sym) < 0)
			goto out;

		err = symbol__inc_addr_samples(mx->daddr.sym,
					       mx->daddr.map,
					       evsel->idx,
					       mx->daddr.al_addr);
		if (err)
			goto out;
	}

	evsel->hists.stats.total_period += cost;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
	err = 0;

	if (symbol_conf.use_callchain) {
		err = callchain_append(he->callchain,
				       &callchain_cursor,
				       sample->period);
	}
out:
	return err;
}

static int perf_report__add_branch_hist_entry(struct perf_tool *tool,
					struct addr_location *al,
					struct perf_sample *sample,
					struct perf_evsel *evsel,
				      struct machine *machine)
{
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
	struct symbol *parent = NULL;
	int err = 0;
	unsigned i;
	struct hist_entry *he;
	struct branch_info *bi, *bx;

	if ((sort__has_parent || symbol_conf.use_callchain)
	    && sample->callchain) {
		err = machine__resolve_callchain(machine, evsel, al->thread,
						 sample, &parent);
		if (err)
			return err;
	}

	bi = machine__resolve_bstack(machine, al->thread,
				     sample->branch_stack);
	if (!bi)
		return -ENOMEM;

	for (i = 0; i < sample->branch_stack->nr; i++) {
		if (rep->hide_unresolved && !(bi[i].from.sym && bi[i].to.sym))
			continue;
		/*
		 * The report shows the percentage of total branches captured
		 * and not events sampled. Thus we use a pseudo period of 1.
		 */
		he = __hists__add_branch_entry(&evsel->hists, al, parent,
				&bi[i], 1, 1);
		if (he) {
			struct annotation *notes;
			err = -ENOMEM;
			bx = he->branch_info;
			if (bx->from.sym && use_browser == 1 && sort__has_sym) {
				notes = symbol__annotation(bx->from.sym);
				if (!notes->src
				    && symbol__alloc_hist(bx->from.sym) < 0)
					goto out;

				err = symbol__inc_addr_samples(bx->from.sym,
							       bx->from.map,
							       evsel->idx,
							       bx->from.al_addr);
				if (err)
					goto out;
			}

			if (bx->to.sym && use_browser == 1 && sort__has_sym) {
				notes = symbol__annotation(bx->to.sym);
				if (!notes->src
				    && symbol__alloc_hist(bx->to.sym) < 0)
					goto out;

				err = symbol__inc_addr_samples(bx->to.sym,
							       bx->to.map,
							       evsel->idx,
							       bx->to.al_addr);
				if (err)
					goto out;
			}
			evsel->hists.stats.total_period += 1;
			hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
			err = 0;
		} else
			return -ENOMEM;
	}
out:
	return err;
}

static int perf_evsel__add_hist_entry(struct perf_evsel *evsel,
				      struct addr_location *al,
				      struct perf_sample *sample,
				      struct machine *machine)
{
	struct symbol *parent = NULL;
	int err = 0;
	struct hist_entry *he;

	if ((sort__has_parent || symbol_conf.use_callchain) && sample->callchain) {
		err = machine__resolve_callchain(machine, evsel, al->thread,
						 sample, &parent);
		if (err)
			return err;
	}

	he = __hists__add_entry(&evsel->hists, al, parent, sample->period,
					sample->weight);
	if (he == NULL)
		return -ENOMEM;

	if (symbol_conf.use_callchain) {
		err = callchain_append(he->callchain,
				       &callchain_cursor,
				       sample->period);
		if (err)
			return err;
	}
	/*
	 * Only in the TUI browser we are doing integrated annotation,
	 * so we don't allocated the extra space needed because the stdio
	 * code will not use it.
	 */
	if (he->ms.sym != NULL && use_browser == 1 && sort__has_sym) {
		struct annotation *notes = symbol__annotation(he->ms.sym);

		assert(evsel != NULL);

		err = -ENOMEM;
		if (notes->src == NULL && symbol__alloc_hist(he->ms.sym) < 0)
			goto out;

		err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	}

	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
=======
	if (!strcmp(var, "report.percent-limit")) {
		double pcnt = strtof(value, NULL);

		rep->min_percent = pcnt;
		callchain_param.min_percent = pcnt;
		return 0;
	}
	if (!strcmp(var, "report.children")) {
		symbol_conf.cumulate_callchain = perf_config_bool(var, value);
		return 0;
	}
	if (!strcmp(var, "report.queue-size")) {
		rep->queue_size = perf_config_u64(var, value);
		return 0;
	}
	if (!strcmp(var, "report.sort_order")) {
		default_sort_order = strdup(value);
		return 0;
	}

	return 0;
}

static int hist_iter__report_callback(struct hist_entry_iter *iter,
				      struct addr_location *al, bool single,
				      void *arg)
{
	int err = 0;
	struct report *rep = arg;
	struct hist_entry *he = iter->he;
	struct perf_evsel *evsel = iter->evsel;
	struct mem_info *mi;
	struct branch_info *bi;

	if (!ui__has_annotation())
		return 0;

	hist__account_cycles(iter->sample->branch_stack, al, iter->sample,
			     rep->nonany_branch_mode);

	if (sort__mode == SORT_MODE__BRANCH) {
		bi = he->branch_info;
		err = addr_map_symbol__inc_samples(&bi->from, evsel->idx);
		if (err)
			goto out;

		err = addr_map_symbol__inc_samples(&bi->to, evsel->idx);

	} else if (rep->mem_mode) {
		mi = he->mem_info;
		err = addr_map_symbol__inc_samples(&mi->daddr, evsel->idx);
		if (err)
			goto out;

		err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);

	} else if (symbol_conf.cumulate_callchain) {
		if (single)
			err = hist_entry__inc_addr_samples(he, evsel->idx,
							   al->addr);
	} else {
		err = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return err;
}

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
<<<<<<< HEAD
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
	struct addr_location al;

	if (perf_event__preprocess_sample(event, machine, &al, sample,
					  rep->annotate_init) < 0) {
		fprintf(stderr, "problem processing %d event, skipping it.\n",
			event->header.type);
		return -1;
	}

	if (al.filtered || (rep->hide_unresolved && al.sym == NULL))
		return 0;

	if (rep->cpu_list && !test_bit(sample->cpu, rep->cpu_bitmap))
		return 0;

	if (sort__branch_mode == 1) {
		if (perf_report__add_branch_hist_entry(tool, &al, sample,
						       evsel, machine)) {
			pr_debug("problem adding lbr entry, skipping event\n");
			return -1;
		}
	} else if (rep->mem_mode == 1) {
		if (perf_report__add_mem_hist_entry(tool, &al, sample,
						    evsel, machine, event)) {
			pr_debug("problem adding mem entry, skipping event\n");
			return -1;
		}
	} else {
		if (al.map != NULL)
			al.map->dso->hit = 1;

		if (perf_evsel__add_hist_entry(evsel, &al, sample, machine)) {
			pr_debug("problem incrementing symbol period, skipping event\n");
			return -1;
		}
	}
	return 0;
=======
	struct report *rep = container_of(tool, struct report, tool);
	struct addr_location al;
	struct hist_entry_iter iter = {
		.evsel 			= evsel,
		.sample 		= sample,
		.hide_unresolved 	= symbol_conf.hide_unresolved,
		.add_entry_cb 		= hist_iter__report_callback,
	};
	int ret = 0;

	if (machine__resolve(machine, &al, sample) < 0) {
		pr_debug("problem processing %d event, skipping it.\n",
			 event->header.type);
		return -1;
	}

	if (symbol_conf.hide_unresolved && al.sym == NULL)
		goto out_put;

	if (rep->cpu_list && !test_bit(sample->cpu, rep->cpu_bitmap))
		goto out_put;

	if (sort__mode == SORT_MODE__BRANCH) {
		/*
		 * A non-synthesized event might not have a branch stack if
		 * branch stacks have been synthesized (using itrace options).
		 */
		if (!sample->branch_stack)
			goto out_put;
		iter.ops = &hist_iter_branch;
	} else if (rep->mem_mode) {
		iter.ops = &hist_iter_mem;
	} else if (symbol_conf.cumulate_callchain) {
		iter.ops = &hist_iter_cumulative;
	} else {
		iter.ops = &hist_iter_normal;
	}

	if (al.map != NULL)
		al.map->dso->hit = 1;

	ret = hist_entry_iter__add(&iter, &al, rep->max_stack, rep);
	if (ret < 0)
		pr_debug("problem adding hist entry, skipping event\n");
out_put:
	addr_location__put(&al);
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int process_read_event(struct perf_tool *tool,
			      union perf_event *event,
			      struct perf_sample *sample __maybe_unused,
			      struct perf_evsel *evsel,
			      struct machine *machine __maybe_unused)
{
<<<<<<< HEAD
	struct perf_report *rep = container_of(tool, struct perf_report, tool);
=======
	struct report *rep = container_of(tool, struct report, tool);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (rep->show_threads) {
		const char *name = evsel ? perf_evsel__name(evsel) : "unknown";
		perf_read_values_add_value(&rep->show_threads_values,
					   event->read.pid, event->read.tid,
					   event->read.id,
					   name,
					   event->read.value);
	}

	dump_printf(": %d %d %s %" PRIu64 "\n", event->read.pid, event->read.tid,
		    evsel ? perf_evsel__name(evsel) : "FAIL",
		    event->read.value);

	return 0;
}

/* For pipe mode, sample_type is not currently set */
<<<<<<< HEAD
static int perf_report__setup_sample_type(struct perf_report *rep)
{
	struct perf_session *self = rep->session;
	u64 sample_type = perf_evlist__sample_type(self->evlist);

	if (!self->fd_pipe && !(sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (sort__has_parent) {
=======
static int report__setup_sample_type(struct report *rep)
{
	struct perf_session *session = rep->session;
	u64 sample_type = perf_evlist__combined_sample_type(session->evlist);
	bool is_pipe = perf_data_file__is_pipe(session->file);

	if (session->itrace_synth_opts->callchain ||
	    (!is_pipe &&
	     perf_header__has_feat(&session->header, HEADER_AUXTRACE) &&
	     !session->itrace_synth_opts->set))
		sample_type |= PERF_SAMPLE_CALLCHAIN;

	if (session->itrace_synth_opts->last_branch)
		sample_type |= PERF_SAMPLE_BRANCH_STACK;

	if (!is_pipe && !(sample_type & PERF_SAMPLE_CALLCHAIN)) {
		if (perf_hpp_list.parent) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			ui__error("Selected --sort parent, but no "
				    "callchain data. Did you call "
				    "'perf record' without -g?\n");
			return -EINVAL;
		}
		if (symbol_conf.use_callchain) {
<<<<<<< HEAD
			ui__error("Selected -g but no callchain data. Did "
				    "you call 'perf record' without -g?\n");
			return -1;
		}
	} else if (!rep->dont_use_callchains &&
=======
			ui__error("Selected -g or --branch-history but no "
				  "callchain data. Did\n"
				  "you call 'perf record' without -g?\n");
			return -1;
		}
	} else if (!callchain_param.enabled &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		   callchain_param.mode != CHAIN_NONE &&
		   !symbol_conf.use_callchain) {
			symbol_conf.use_callchain = true;
			if (callchain_register_param(&callchain_param) < 0) {
				ui__error("Can't register callchain params.\n");
				return -EINVAL;
			}
	}

<<<<<<< HEAD
	if (sort__branch_mode == 1) {
		if (!self->fd_pipe &&
=======
	if (symbol_conf.cumulate_callchain) {
		/* Silently ignore if callchain is missing */
		if (!(sample_type & PERF_SAMPLE_CALLCHAIN)) {
			symbol_conf.cumulate_callchain = false;
			perf_hpp__cancel_cumulate();
		}
	}

	if (sort__mode == SORT_MODE__BRANCH) {
		if (!is_pipe &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		    !(sample_type & PERF_SAMPLE_BRANCH_STACK)) {
			ui__error("Selected -b but no branch data. "
				  "Did you call perf record without -b?\n");
			return -1;
		}
	}

<<<<<<< HEAD
	return 0;
}

extern volatile int session_done;

=======
	if (symbol_conf.use_callchain || symbol_conf.cumulate_callchain) {
		if ((sample_type & PERF_SAMPLE_REGS_USER) &&
		    (sample_type & PERF_SAMPLE_STACK_USER))
			callchain_param.record_mode = CALLCHAIN_DWARF;
		else if (sample_type & PERF_SAMPLE_BRANCH_STACK)
			callchain_param.record_mode = CALLCHAIN_LBR;
		else
			callchain_param.record_mode = CALLCHAIN_FP;
	}

	/* ??? handle more cases than just ANY? */
	if (!(perf_evlist__combined_branch_type(session->evlist) &
				PERF_SAMPLE_BRANCH_ANY))
		rep->nonany_branch_mode = true;

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void sig_handler(int sig __maybe_unused)
{
	session_done = 1;
}

<<<<<<< HEAD
static size_t hists__fprintf_nr_sample_events(struct perf_report *rep,
					      struct hists *self,
=======
static size_t hists__fprintf_nr_sample_events(struct hists *hists, struct report *rep,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					      const char *evname, FILE *fp)
{
	size_t ret;
	char unit;
<<<<<<< HEAD
	unsigned long nr_samples = self->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = self->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(self);
	char buf[512];
	size_t size = sizeof(buf);
=======
	unsigned long nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = hists->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(hists);
	char buf[512];
	size_t size = sizeof(buf);
	int socked_id = hists->socket_filter;

	if (symbol_conf.filter_relative) {
		nr_samples = hists->stats.nr_non_filtered_samples;
		nr_events = hists->stats.total_non_filtered_period;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (perf_evsel__is_group_event(evsel)) {
		struct perf_evsel *pos;

		perf_evsel__group_desc(evsel, buf, size);
		evname = buf;

		for_each_group_member(pos, evsel) {
<<<<<<< HEAD
			nr_samples += pos->hists.stats.nr_events[PERF_RECORD_SAMPLE];
			nr_events += pos->hists.stats.total_period;
=======
			const struct hists *pos_hists = evsel__hists(pos);

			if (symbol_conf.filter_relative) {
				nr_samples += pos_hists->stats.nr_non_filtered_samples;
				nr_events += pos_hists->stats.total_non_filtered_period;
			} else {
				nr_samples += pos_hists->stats.nr_events[PERF_RECORD_SAMPLE];
				nr_events += pos_hists->stats.total_period;
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	nr_samples = convert_unit(nr_samples, &unit);
	ret = fprintf(fp, "# Samples: %lu%c", nr_samples, unit);
	if (evname != NULL)
		ret += fprintf(fp, " of event '%s'", evname);

<<<<<<< HEAD
	if (rep->mem_mode) {
		ret += fprintf(fp, "\n# Total weight : %" PRIu64, nr_events);
		ret += fprintf(fp, "\n# Sort order   : %s", sort_order);
	} else
		ret += fprintf(fp, "\n# Event count (approx.): %" PRIu64, nr_events);
=======
	if (symbol_conf.show_ref_callgraph && evname && strstr(evname, "call-graph=no")) {
		ret += fprintf(fp, ", show reference callgraph");
	}

	if (rep->mem_mode) {
		ret += fprintf(fp, "\n# Total weight : %" PRIu64, nr_events);
		ret += fprintf(fp, "\n# Sort order   : %s", sort_order ? : default_mem_sort_order);
	} else
		ret += fprintf(fp, "\n# Event count (approx.): %" PRIu64, nr_events);

	if (socked_id > -1)
		ret += fprintf(fp, "\n# Processor Socket: %d", socked_id);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret + fprintf(fp, "\n#\n");
}

static int perf_evlist__tty_browse_hists(struct perf_evlist *evlist,
<<<<<<< HEAD
					 struct perf_report *rep,
=======
					 struct report *rep,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					 const char *help)
{
	struct perf_evsel *pos;

<<<<<<< HEAD
	list_for_each_entry(pos, &evlist->entries, node) {
		struct hists *hists = &pos->hists;
=======
	fprintf(stdout, "#\n# Total Lost Samples: %" PRIu64 "\n#\n", evlist->stats.total_lost_samples);
	evlist__for_each_entry(evlist, pos) {
		struct hists *hists = evsel__hists(pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		const char *evname = perf_evsel__name(pos);

		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos))
			continue;

<<<<<<< HEAD
		hists__fprintf_nr_sample_events(rep, hists, evname, stdout);
		hists__fprintf(hists, true, 0, 0, stdout);
		fprintf(stdout, "\n\n");
	}

	if (sort_order == default_sort_order &&
	    parent_pattern == default_parent_pattern) {
		fprintf(stdout, "#\n# (%s)\n#\n", help);

		if (rep->show_threads) {
			bool style = !strcmp(rep->pretty_printing_style, "raw");
			perf_read_values_display(stdout, &rep->show_threads_values,
						 style);
			perf_read_values_destroy(&rep->show_threads_values);
		}
=======
		hists__fprintf_nr_sample_events(hists, rep, evname, stdout);
		hists__fprintf(hists, true, 0, 0, rep->min_percent, stdout,
			       symbol_conf.use_callchain);
		fprintf(stdout, "\n\n");
	}

	if (sort_order == NULL &&
	    parent_pattern == default_parent_pattern)
		fprintf(stdout, "#\n# (%s)\n#\n", help);

	if (rep->show_threads) {
		bool style = !strcmp(rep->pretty_printing_style, "raw");
		perf_read_values_display(stdout, &rep->show_threads_values,
					 style);
		perf_read_values_destroy(&rep->show_threads_values);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return 0;
}

<<<<<<< HEAD
static int __cmd_report(struct perf_report *rep)
{
	int ret = -EINVAL;
	u64 nr_samples;
	struct perf_session *session = rep->session;
	struct perf_evsel *pos;
	struct map *kernel_map;
	struct kmap *kernel_kmap;
	const char *help = "For a higher level overview, try: perf report --sort comm,dso";

	signal(SIGINT, sig_handler);

	if (rep->cpu_list) {
		ret = perf_session__cpu_bitmap(session, rep->cpu_list,
					       rep->cpu_bitmap);
		if (ret)
			goto out_delete;
	}

	if (use_browser <= 0)
		perf_session__fprintf_info(session, stdout, rep->show_full_info);

	if (rep->show_threads)
		perf_read_values_init(&rep->show_threads_values);

	ret = perf_report__setup_sample_type(rep);
	if (ret)
		goto out_delete;

	ret = perf_session__process_events(session, &rep->tool);
	if (ret)
		goto out_delete;

	kernel_map = session->machines.host.vmlinux_maps[MAP__FUNCTION];
	kernel_kmap = map__kmap(kernel_map);
=======
static void report__warn_kptr_restrict(const struct report *rep)
{
	struct map *kernel_map = machine__kernel_map(&rep->session->machines.host);
	struct kmap *kernel_kmap = kernel_map ? map__kmap(kernel_map) : NULL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (kernel_map == NULL ||
	    (kernel_map->dso->hit &&
	     (kernel_kmap->ref_reloc_sym == NULL ||
	      kernel_kmap->ref_reloc_sym->addr == 0))) {
		const char *desc =
		    "As no suitable kallsyms nor vmlinux was found, kernel samples\n"
		    "can't be resolved.";

		if (kernel_map) {
			const struct dso *kdso = kernel_map->dso;
			if (!RB_EMPTY_ROOT(&kdso->symbols[MAP__FUNCTION])) {
				desc = "If some relocation was applied (e.g. "
				       "kexec) symbols may be misresolved.";
			}
		}

		ui__warning(
"Kernel address maps (/proc/{kallsyms,modules}) were restricted.\n\n"
"Check /proc/sys/kernel/kptr_restrict before running 'perf record'.\n\n%s\n\n"
"Samples in kernel modules can't be resolved as well.\n\n",
		desc);
	}
<<<<<<< HEAD

	if (verbose > 3)
		perf_session__fprintf(session, stdout);

	if (verbose > 2)
		perf_session__fprintf_dsos(session, stdout);

	if (dump_trace) {
		perf_session__fprintf_nr_events(session, stdout);
		goto out_delete;
	}

	nr_samples = 0;
	list_for_each_entry(pos, &session->evlist->entries, node) {
		struct hists *hists = &pos->hists;
=======
}

static int report__gtk_browse_hists(struct report *rep, const char *help)
{
	int (*hist_browser)(struct perf_evlist *evlist, const char *help,
			    struct hist_browser_timer *timer, float min_pcnt);

	hist_browser = dlsym(perf_gtk_handle, "perf_evlist__gtk_browse_hists");

	if (hist_browser == NULL) {
		ui__error("GTK browser not found!\n");
		return -1;
	}

	return hist_browser(rep->session->evlist, help, NULL, rep->min_percent);
}

static int report__browse_hists(struct report *rep)
{
	int ret;
	struct perf_session *session = rep->session;
	struct perf_evlist *evlist = session->evlist;
	const char *help = perf_tip(system_path(TIPDIR));

	if (help == NULL) {
		/* fallback for people who don't install perf ;-) */
		help = perf_tip(DOCDIR);
		if (help == NULL)
			help = "Cannot load tips.txt file, please install perf!";
	}

	switch (use_browser) {
	case 1:
		ret = perf_evlist__tui_browse_hists(evlist, help, NULL,
						    rep->min_percent,
						    &session->header.env);
		/*
		 * Usually "ret" is the last pressed key, and we only
		 * care if the key notifies us to switch data file.
		 */
		if (ret != K_SWITCH_INPUT_DATA)
			ret = 0;
		break;
	case 2:
		ret = report__gtk_browse_hists(rep, help);
		break;
	default:
		ret = perf_evlist__tty_browse_hists(evlist, rep, help);
		break;
	}

	return ret;
}

static int report__collapse_hists(struct report *rep)
{
	struct ui_progress prog;
	struct perf_evsel *pos;
	int ret = 0;

	ui_progress__init(&prog, rep->nr_entries, "Merging related events...");

	evlist__for_each_entry(rep->session->evlist, pos) {
		struct hists *hists = evsel__hists(pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (pos->idx == 0)
			hists->symbol_filter_str = rep->symbol_filter_str;

<<<<<<< HEAD
		hists__collapse_resort(hists);
		nr_samples += hists->stats.nr_events[PERF_RECORD_SAMPLE];
=======
		hists->socket_filter = rep->socket_filter;

		ret = hists__collapse_resort(hists, &prog);
		if (ret < 0)
			break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* Non-group events are considered as leader */
		if (symbol_conf.event_group &&
		    !perf_evsel__is_group_leader(pos)) {
<<<<<<< HEAD
			struct hists *leader_hists = &pos->leader->hists;
=======
			struct hists *leader_hists = evsel__hists(pos->leader);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			hists__match(leader_hists, hists);
			hists__link(leader_hists, hists);
		}
	}

<<<<<<< HEAD
	if (nr_samples == 0) {
		ui__error("The %s file has no samples!\n", session->filename);
		goto out_delete;
	}

	list_for_each_entry(pos, &session->evlist->entries, node)
		hists__output_resort(&pos->hists);

	if (use_browser > 0) {
		if (use_browser == 1) {
			ret = perf_evlist__tui_browse_hists(session->evlist,
							help,
							NULL,
							&session->header.env);
			/*
			 * Usually "ret" is the last pressed key, and we only
			 * care if the key notifies us to switch data file.
			 */
			if (ret != K_SWITCH_INPUT_DATA)
				ret = 0;

		} else if (use_browser == 2) {
			perf_evlist__gtk_browse_hists(session->evlist, help,
						      NULL);
		}
	} else
		perf_evlist__tty_browse_hists(session->evlist, rep, help);

out_delete:
	/*
	 * Speed up the exit process, for large files this can
	 * take quite a while.
	 *
	 * XXX Enable this when using valgrind or if we ever
	 * librarize this command.
	 *
	 * Also experiment with obstacks to see how much speed
	 * up we'll get here.
	 *
 	 * perf_session__delete(session);
 	 */
	return ret;
}

static int
parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct perf_report *rep = (struct perf_report *)opt->value;
	char *tok, *tok2;
	char *endptr;

	/*
	 * --no-call-graph
	 */
	if (unset) {
		rep->dont_use_callchains = true;
		return 0;
	}

	symbol_conf.use_callchain = true;

	if (!arg)
		return 0;

	tok = strtok((char *)arg, ",");
	if (!tok)
		return -1;

	/* get the output mode */
	if (!strncmp(tok, "graph", strlen(arg)))
		callchain_param.mode = CHAIN_GRAPH_ABS;

	else if (!strncmp(tok, "flat", strlen(arg)))
		callchain_param.mode = CHAIN_FLAT;

	else if (!strncmp(tok, "fractal", strlen(arg)))
		callchain_param.mode = CHAIN_GRAPH_REL;

	else if (!strncmp(tok, "none", strlen(arg))) {
		callchain_param.mode = CHAIN_NONE;
		symbol_conf.use_callchain = false;

		return 0;
	}

	else
		return -1;

	/* get the min percentage */
	tok = strtok(NULL, ",");
	if (!tok)
		goto setup;

	callchain_param.min_percent = strtod(tok, &endptr);
	if (tok == endptr)
		return -1;

	/* get the print limit */
	tok2 = strtok(NULL, ",");
	if (!tok2)
		goto setup;

	if (tok2[0] != 'c') {
		callchain_param.print_limit = strtoul(tok2, &endptr, 0);
		tok2 = strtok(NULL, ",");
		if (!tok2)
			goto setup;
	}

	/* get the call chain order */
	if (!strcmp(tok2, "caller"))
		callchain_param.order = ORDER_CALLER;
	else if (!strcmp(tok2, "callee"))
		callchain_param.order = ORDER_CALLEE;
	else
		return -1;
setup:
	if (callchain_register_param(&callchain_param) < 0) {
		fprintf(stderr, "Can't register callchain params\n");
		return -1;
	}
=======
	ui_progress__finish();
	return ret;
}

static void report__output_resort(struct report *rep)
{
	struct ui_progress prog;
	struct perf_evsel *pos;

	ui_progress__init(&prog, rep->nr_entries, "Sorting events for output...");

	evlist__for_each_entry(rep->session->evlist, pos)
		perf_evsel__output_resort(pos, &prog);

	ui_progress__finish();
}

static int __cmd_report(struct report *rep)
{
	int ret;
	struct perf_session *session = rep->session;
	struct perf_evsel *pos;
	struct perf_data_file *file = session->file;

	signal(SIGINT, sig_handler);

	if (rep->cpu_list) {
		ret = perf_session__cpu_bitmap(session, rep->cpu_list,
					       rep->cpu_bitmap);
		if (ret) {
			ui__error("failed to set cpu bitmap\n");
			return ret;
		}
	}

	if (rep->show_threads)
		perf_read_values_init(&rep->show_threads_values);

	ret = report__setup_sample_type(rep);
	if (ret) {
		/* report__setup_sample_type() already showed error message */
		return ret;
	}

	ret = perf_session__process_events(session);
	if (ret) {
		ui__error("failed to process sample\n");
		return ret;
	}

	report__warn_kptr_restrict(rep);

	evlist__for_each_entry(session->evlist, pos)
		rep->nr_entries += evsel__hists(pos)->nr_entries;

	if (use_browser == 0) {
		if (verbose > 3)
			perf_session__fprintf(session, stdout);

		if (verbose > 2)
			perf_session__fprintf_dsos(session, stdout);

		if (dump_trace) {
			perf_session__fprintf_nr_events(session, stdout);
			perf_evlist__fprintf_nr_events(session->evlist, stdout);
			return 0;
		}
	}

	ret = report__collapse_hists(rep);
	if (ret) {
		ui__error("failed to process hist entry\n");
		return ret;
	}

	if (session_done())
		return 0;

	/*
	 * recalculate number of entries after collapsing since it
	 * might be changed during the collapse phase.
	 */
	rep->nr_entries = 0;
	evlist__for_each_entry(session->evlist, pos)
		rep->nr_entries += evsel__hists(pos)->nr_entries;

	if (rep->nr_entries == 0) {
		ui__error("The %s file has no samples!\n", file->path);
		return 0;
	}

	report__output_resort(rep);

	return report__browse_hists(rep);
}

static int
report_parse_callchain_opt(const struct option *opt, const char *arg, int unset)
{
	struct callchain_param *callchain = opt->value;

	callchain->enabled = !unset;
	/*
	 * --no-call-graph
	 */
	if (unset) {
		symbol_conf.use_callchain = false;
		callchain->mode = CHAIN_NONE;
		return 0;
	}

	return parse_callchain_report_opt(arg);
}

int
report_parse_ignore_callees_opt(const struct option *opt __maybe_unused,
				const char *arg, int unset __maybe_unused)
{
	if (arg) {
		int err = regcomp(&ignore_callees_regex, arg, REG_EXTENDED);
		if (err) {
			char buf[BUFSIZ];
			regerror(err, &ignore_callees_regex, buf, sizeof(buf));
			pr_err("Invalid --ignore-callees regex: %s\n%s", arg, buf);
			return -1;
		}
		have_ignore_callees = 1;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

static int
parse_branch_mode(const struct option *opt __maybe_unused,
		  const char *str __maybe_unused, int unset)
{
<<<<<<< HEAD
	sort__branch_mode = !unset;
	return 0;
}

int cmd_report(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct perf_session *session;
	struct stat st;
	bool has_br_stack = false;
	int ret = -1;
	char callchain_default_opt[] = "fractal,0.5,callee";
=======
	int *branch_mode = opt->value;

	*branch_mode = !unset;
	return 0;
}

static int
parse_percent_limit(const struct option *opt, const char *str,
		    int unset __maybe_unused)
{
	struct report *rep = opt->value;
	double pcnt = strtof(str, NULL);

	rep->min_percent = pcnt;
	callchain_param.min_percent = pcnt;
	return 0;
}

#define CALLCHAIN_DEFAULT_OPT  "graph,0.5,caller,function,percent"

const char report_callchain_help[] = "Display call graph (stack chain/backtrace):\n\n"
				     CALLCHAIN_REPORT_HELP
				     "\n\t\t\t\tDefault: " CALLCHAIN_DEFAULT_OPT;

int cmd_report(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct perf_session *session;
	struct itrace_synth_opts itrace_synth_opts = { .set = 0, };
	struct stat st;
	bool has_br_stack = false;
	int branch_mode = -1;
	int last_key = 0;
	bool branch_call_mode = false;
	char callchain_default_opt[] = CALLCHAIN_DEFAULT_OPT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char * const report_usage[] = {
		"perf report [<options>]",
		NULL
	};
<<<<<<< HEAD
	struct perf_report report = {
		.tool = {
			.sample		 = process_sample_event,
			.mmap		 = perf_event__process_mmap,
=======
	struct report report = {
		.tool = {
			.sample		 = process_sample_event,
			.mmap		 = perf_event__process_mmap,
			.mmap2		 = perf_event__process_mmap2,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			.comm		 = perf_event__process_comm,
			.exit		 = perf_event__process_exit,
			.fork		 = perf_event__process_fork,
			.lost		 = perf_event__process_lost,
			.read		 = process_read_event,
			.attr		 = perf_event__process_attr,
<<<<<<< HEAD
			.event_type	 = perf_event__process_event_type,
			.tracing_data	 = perf_event__process_tracing_data,
			.build_id	 = perf_event__process_build_id,
			.ordered_samples = true,
			.ordering_requires_timestamps = true,
		},
		.pretty_printing_style	 = "normal",
=======
			.tracing_data	 = perf_event__process_tracing_data,
			.build_id	 = perf_event__process_build_id,
			.id_index	 = perf_event__process_id_index,
			.auxtrace_info	 = perf_event__process_auxtrace_info,
			.auxtrace	 = perf_event__process_auxtrace,
			.ordered_events	 = true,
			.ordering_requires_timestamps = true,
		},
		.max_stack		 = PERF_MAX_STACK_DEPTH,
		.pretty_printing_style	 = "normal",
		.socket_filter		 = -1,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	};
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_STRING(0, "kallsyms", &symbol_conf.kallsyms_name,
		   "file", "kallsyms pathname"),
<<<<<<< HEAD
	OPT_BOOLEAN('f', "force", &report.force, "don't complain, do it"),
=======
	OPT_BOOLEAN('f', "force", &symbol_conf.force, "don't complain, do it"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_BOOLEAN('n', "show-nr-samples", &symbol_conf.show_nr_samples,
		    "Show a column with the number of samples"),
	OPT_BOOLEAN('T', "threads", &report.show_threads,
		    "Show per-thread event counters"),
	OPT_STRING(0, "pretty", &report.pretty_printing_style, "key",
		   "pretty printing style key: normal raw"),
	OPT_BOOLEAN(0, "tui", &report.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "gtk", &report.use_gtk, "Use the GTK2 interface"),
	OPT_BOOLEAN(0, "stdio", &report.use_stdio,
		    "Use the stdio interface"),
<<<<<<< HEAD
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent, cpu, srcline,"
		   " dso_to, dso_from, symbol_to, symbol_from, mispredict,"
		   " weight, local_weight, mem, symbol_daddr, dso_daddr, tlb, "
		   "snoop, locked"),
	OPT_BOOLEAN(0, "showcpuutilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes"),
=======
	OPT_BOOLEAN(0, "header", &report.header, "Show data header."),
	OPT_BOOLEAN(0, "header-only", &report.header_only,
		    "Show only data header."),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent, cpu, srcline, ..."
		   " Please refer the man page for the complete list."),
	OPT_STRING('F', "fields", &field_order, "key[,keys...]",
		   "output field(s): overhead, period, sample plus all of sort keys"),
	OPT_BOOLEAN(0, "show-cpu-utilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes"),
	OPT_BOOLEAN_FLAG(0, "showcpuutilization", &symbol_conf.show_cpu_utilization,
		    "Show sample percentage for different cpu modes", PARSE_OPT_HIDDEN),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('p', "parent", &parent_pattern, "regex",
		   "regex filter to identify parent, see: '--sort parent'"),
	OPT_BOOLEAN('x', "exclude-other", &symbol_conf.exclude_other,
		    "Only display entries with parent-match"),
<<<<<<< HEAD
	OPT_CALLBACK_DEFAULT('g', "call-graph", &report, "output_type,min_percent[,print_limit],call_order",
		     "Display callchains using output_type (graph, flat, fractal, or none) , min percent threshold, optional print limit and callchain order. "
		     "Default: fractal,0.5,callee", &parse_callchain_opt, callchain_default_opt),
	OPT_BOOLEAN('G', "inverted", &report.inverted_callchain,
		    "alias for inverted call graph"),
=======
	OPT_CALLBACK_DEFAULT('g', "call-graph", &callchain_param,
			     "print_type,threshold[,print_limit],order,sort_key[,branch],value",
			     report_callchain_help, &report_parse_callchain_opt,
			     callchain_default_opt),
	OPT_BOOLEAN(0, "children", &symbol_conf.cumulate_callchain,
		    "Accumulate callchains of children and show total overhead as well"),
	OPT_INTEGER(0, "max-stack", &report.max_stack,
		    "Set the maximum stack depth when parsing the callchain, "
		    "anything beyond the specified depth will be ignored. "
		    "Default: kernel.perf_event_max_stack or " __stringify(PERF_MAX_STACK_DEPTH)),
	OPT_BOOLEAN('G', "inverted", &report.inverted_callchain,
		    "alias for inverted call graph"),
	OPT_CALLBACK(0, "ignore-callees", NULL, "regex",
		   "ignore callees of these functions in call graphs",
		   report_parse_ignore_callees_opt),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('c', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
<<<<<<< HEAD
=======
	OPT_STRING(0, "pid", &symbol_conf.pid_list_str, "pid[,pid...]",
		   "only consider symbols in these pids"),
	OPT_STRING(0, "tid", &symbol_conf.tid_list_str, "tid[,tid...]",
		   "only consider symbols in these tids"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_STRING(0, "symbol-filter", &report.symbol_filter_str, "filter",
		   "only show symbols that (partially) match with this filter"),
	OPT_STRING('w', "column-widths", &symbol_conf.col_width_list_str,
		   "width[,width...]",
		   "don't try to adjust column width, use these fixed values"),
<<<<<<< HEAD
	OPT_STRING('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
	OPT_BOOLEAN('U', "hide-unresolved", &report.hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		    "Look for files with symbols relative to this directory"),
=======
	OPT_STRING_NOEMPTY('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
	OPT_BOOLEAN('U', "hide-unresolved", &symbol_conf.hide_unresolved,
		    "Only display entries resolved to a symbol"),
	OPT_CALLBACK(0, "symfs", NULL, "directory",
		     "Look for files with symbols relative to this directory",
		     symbol__config_symfs),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING('C', "cpu", &report.cpu_list, "cpu",
		   "list of cpus to profile"),
	OPT_BOOLEAN('I', "show-info", &report.show_full_info,
		    "Display extended information about perf.data file"),
	OPT_BOOLEAN(0, "source", &symbol_conf.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &symbol_conf.annotate_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
	OPT_BOOLEAN(0, "show-total-period", &symbol_conf.show_total_period,
		    "Show a column with the sum of periods"),
	OPT_BOOLEAN(0, "group", &symbol_conf.event_group,
		    "Show event group information together"),
<<<<<<< HEAD
	OPT_CALLBACK_NOOPT('b', "branch-stack", &sort__branch_mode, "",
		    "use branch records for histogram filling", parse_branch_mode),
=======
	OPT_CALLBACK_NOOPT('b', "branch-stack", &branch_mode, "",
		    "use branch records for per branch histogram filling",
		    parse_branch_mode),
	OPT_BOOLEAN(0, "branch-history", &branch_call_mode,
		    "add last branch records to call history"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_STRING(0, "objdump", &objdump_path, "path",
		   "objdump binary to use for disassembly and annotations"),
	OPT_BOOLEAN(0, "demangle", &symbol_conf.demangle,
		    "Disable symbol demangling"),
<<<<<<< HEAD
	OPT_BOOLEAN(0, "mem-mode", &report.mem_mode, "mem access profile"),
	OPT_END()
	};

	perf_config(perf_report_config, NULL);

	argc = parse_options(argc, argv, options, report_usage, 0);
=======
	OPT_BOOLEAN(0, "demangle-kernel", &symbol_conf.demangle_kernel,
		    "Enable kernel symbol demangling"),
	OPT_BOOLEAN(0, "mem-mode", &report.mem_mode, "mem access profile"),
	OPT_CALLBACK(0, "percent-limit", &report, "percent",
		     "Don't show entries under that percent", parse_percent_limit),
	OPT_CALLBACK(0, "percentage", NULL, "relative|absolute",
		     "how to display percentage of filtered entries", parse_filter_percentage),
	OPT_CALLBACK_OPTARG(0, "itrace", &itrace_synth_opts, NULL, "opts",
			    "Instruction Tracing options",
			    itrace_parse_synth_opts),
	OPT_BOOLEAN(0, "full-source-path", &srcline_full_filename,
			"Show full source file name path for source lines"),
	OPT_BOOLEAN(0, "show-ref-call-graph", &symbol_conf.show_ref_callgraph,
		    "Show callgraph from reference event"),
	OPT_INTEGER(0, "socket-filter", &report.socket_filter,
		    "only show processor socket that match with this filter"),
	OPT_BOOLEAN(0, "raw-trace", &symbol_conf.raw_trace,
		    "Show raw trace event output (do not use print fmt or plugins)"),
	OPT_BOOLEAN(0, "hierarchy", &symbol_conf.report_hierarchy,
		    "Show entries in a hierarchy"),
	OPT_CALLBACK_DEFAULT(0, "stdio-color", NULL, "mode",
			     "'always' (default), 'never' or 'auto' only applicable to --stdio mode",
			     stdio__config_color, "always"),
	OPT_END()
	};
	struct perf_data_file file = {
		.mode  = PERF_DATA_MODE_READ,
	};
	int ret = hists__init();

	if (ret < 0)
		return ret;

	perf_config(report__config, &report);

	argc = parse_options(argc, argv, options, report_usage, 0);
	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(report_usage, options);

		report.symbol_filter_str = argv[0];
	}

	if (symbol_conf.vmlinux_name &&
	    access(symbol_conf.vmlinux_name, R_OK)) {
		pr_err("Invalid file: %s\n", symbol_conf.vmlinux_name);
		return -EINVAL;
	}
	if (symbol_conf.kallsyms_name &&
	    access(symbol_conf.kallsyms_name, R_OK)) {
		pr_err("Invalid file: %s\n", symbol_conf.kallsyms_name);
		return -EINVAL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (report.use_stdio)
		use_browser = 0;
	else if (report.use_tui)
		use_browser = 1;
	else if (report.use_gtk)
		use_browser = 2;

	if (report.inverted_callchain)
		callchain_param.order = ORDER_CALLER;
<<<<<<< HEAD
=======
	if (symbol_conf.cumulate_callchain && !callchain_param.order_set)
		callchain_param.order = ORDER_CALLER;

	if (itrace_synth_opts.callchain &&
	    (int)itrace_synth_opts.callchain_sz > report.max_stack)
		report.max_stack = itrace_synth_opts.callchain_sz;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!input_name || !strlen(input_name)) {
		if (!fstat(STDIN_FILENO, &st) && S_ISFIFO(st.st_mode))
			input_name = "-";
		else
			input_name = "perf.data";
	}

<<<<<<< HEAD
	if (strcmp(input_name, "-") != 0)
		setup_browser(true);
	else {
		use_browser = 0;
		perf_hpp__column_enable(PERF_HPP__OVERHEAD);
		perf_hpp__init();
	}

repeat:
	session = perf_session__new(input_name, O_RDONLY,
				    report.force, false, &report.tool);
	if (session == NULL)
		return -ENOMEM;
=======
	file.path  = input_name;
	file.force = symbol_conf.force;

repeat:
	session = perf_session__new(&file, false, &report.tool);
	if (session == NULL)
		return -1;

	if (report.queue_size) {
		ordered_events__set_alloc_size(&session->ordered_events,
					       report.queue_size);
	}

	session->itrace_synth_opts = &itrace_synth_opts;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	report.session = session;

	has_br_stack = perf_header__has_feat(&session->header,
					     HEADER_BRANCH_STACK);

<<<<<<< HEAD
	if (sort__branch_mode == -1 && has_br_stack)
		sort__branch_mode = 1;

	/* sort__branch_mode could be 0 if --no-branch-stack */
	if (sort__branch_mode == 1) {
		/*
		 * if no sort_order is provided, then specify
		 * branch-mode specific order
		 */
		if (sort_order == default_sort_order)
			sort_order = "comm,dso_from,symbol_from,"
				     "dso_to,symbol_to";

	}
	if (report.mem_mode) {
		if (sort__branch_mode == 1) {
			fprintf(stderr, "branch and mem mode incompatible\n");
			goto error;
		}
		/*
		 * if no sort_order is provided, then specify
		 * branch-mode specific order
		 */
		if (sort_order == default_sort_order)
			sort_order = "local_weight,mem,sym,dso,symbol_daddr,dso_daddr,snoop,tlb,locked";
	}

	if (setup_sorting() < 0)
		usage_with_options(report_usage, options);
=======
	if (itrace_synth_opts.last_branch)
		has_br_stack = true;

	/*
	 * Branch mode is a tristate:
	 * -1 means default, so decide based on the file having branch data.
	 * 0/1 means the user chose a mode.
	 */
	if (((branch_mode == -1 && has_br_stack) || branch_mode == 1) &&
	    !branch_call_mode) {
		sort__mode = SORT_MODE__BRANCH;
		symbol_conf.cumulate_callchain = false;
	}
	if (branch_call_mode) {
		callchain_param.key = CCKEY_ADDRESS;
		callchain_param.branch_callstack = 1;
		symbol_conf.use_callchain = true;
		callchain_register_param(&callchain_param);
		if (sort_order == NULL)
			sort_order = "srcline,symbol,dso";
	}

	if (report.mem_mode) {
		if (sort__mode == SORT_MODE__BRANCH) {
			pr_err("branch and mem mode incompatible\n");
			goto error;
		}
		sort__mode = SORT_MODE__MEMORY;
		symbol_conf.cumulate_callchain = false;
	}

	if (symbol_conf.report_hierarchy) {
		/* disable incompatible options */
		symbol_conf.cumulate_callchain = false;

		if (field_order) {
			pr_err("Error: --hierarchy and --fields options cannot be used together\n");
			parse_options_usage(report_usage, options, "F", 1);
			parse_options_usage(NULL, options, "hierarchy", 0);
			goto error;
		}

		perf_hpp_list.need_collapse = true;
	}

	/* Force tty output for header output and per-thread stat. */
	if (report.header || report.header_only || report.show_threads)
		use_browser = 0;

	if (strcmp(input_name, "-") != 0)
		setup_browser(true);
	else
		use_browser = 0;

	if ((last_key != K_SWITCH_INPUT_DATA) &&
	    (setup_sorting(session->evlist) < 0)) {
		if (sort_order)
			parse_options_usage(report_usage, options, "s", 1);
		if (field_order)
			parse_options_usage(sort_order ? NULL : report_usage,
					    options, "F", 1);
		goto error;
	}

	if (report.header || report.header_only) {
		perf_session__fprintf_info(session, stdout,
					   report.show_full_info);
		if (report.header_only) {
			ret = 0;
			goto error;
		}
	} else if (use_browser == 0) {
		fputs("# To display the perf.data header info, please use --header/--header-only options.\n#\n",
		      stdout);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Only in the TUI browser we are doing integrated annotation,
	 * so don't allocate extra space that won't be used in the stdio
	 * implementation.
	 */
<<<<<<< HEAD
	if (use_browser == 1 && sort__has_sym) {
		symbol_conf.priv_size = sizeof(struct annotation);
		report.annotate_init  = symbol__annotate_init;
=======
	if (ui__has_annotation()) {
		ret = symbol__annotation_init();
		if (ret < 0)
			goto error;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
 		 * For searching by name on the "Browse map details".
 		 * providing it only in verbose mode not to bloat too
 		 * much struct symbol.
 		 */
		if (verbose) {
			/*
			 * XXX: Need to provide a less kludgy way to ask for
			 * more space per symbol, the u32 is for the index on
			 * the ui browser.
			 * See symbol__browser_index.
			 */
			symbol_conf.priv_size += sizeof(u32);
			symbol_conf.sort_by_name = true;
		}
	}

<<<<<<< HEAD
	if (symbol__init() < 0)
		goto error;

	if (parent_pattern != default_parent_pattern) {
		if (sort_dimension__add("parent") < 0)
			goto error;

		/*
		 * Only show the parent fields if we explicitly
		 * sort that way. If we only use parent machinery
		 * for filtering, we don't want it.
		 */
		if (!strstr(sort_order, "parent"))
			sort_parent.elide = 1;
	} else
		symbol_conf.exclude_other = false;

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(report_usage, options);

		report.symbol_filter_str = argv[0];
	}

	sort_entry__setup_elide(&sort_comm, symbol_conf.comm_list, "comm", stdout);

	if (sort__branch_mode == 1) {
		sort_entry__setup_elide(&sort_dso_from, symbol_conf.dso_from_list, "dso_from", stdout);
		sort_entry__setup_elide(&sort_dso_to, symbol_conf.dso_to_list, "dso_to", stdout);
		sort_entry__setup_elide(&sort_sym_from, symbol_conf.sym_from_list, "sym_from", stdout);
		sort_entry__setup_elide(&sort_sym_to, symbol_conf.sym_to_list, "sym_to", stdout);
	} else {
		if (report.mem_mode) {
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "symbol_daddr", stdout);
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso_daddr", stdout);
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "mem", stdout);
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "local_weight", stdout);
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "tlb", stdout);
			sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "snoop", stdout);
		}
		sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso", stdout);
		sort_entry__setup_elide(&sort_sym, symbol_conf.sym_list, "symbol", stdout);
	}
=======
	if (symbol__init(&session->header.env) < 0)
		goto error;

	sort__setup_elide(stdout);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = __cmd_report(&report);
	if (ret == K_SWITCH_INPUT_DATA) {
		perf_session__delete(session);
<<<<<<< HEAD
=======
		last_key = K_SWITCH_INPUT_DATA;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto repeat;
	} else
		ret = 0;

error:
	perf_session__delete(session);
	return ret;
}
