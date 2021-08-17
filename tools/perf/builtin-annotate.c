/*
 * builtin-annotate.c
 *
 * Builtin annotate command: Analyze the perf.data input file,
 * look up and read DSOs and symbol information and display
 * a histogram of results, along various sorting keys.
 */
#include "builtin.h"

#include "util/util.h"
#include "util/color.h"
#include <linux/list.h>
#include "util/cache.h"
#include <linux/rbtree.h>
#include "util/symbol.h"

#include "perf.h"
#include "util/debug.h"

#include "util/evlist.h"
#include "util/evsel.h"
#include "util/annotate.h"
#include "util/event.h"
<<<<<<< HEAD
#include "util/parse-options.h"
=======
#include <subcmd/parse-options.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "util/parse-events.h"
#include "util/thread.h"
#include "util/sort.h"
#include "util/hist.h"
#include "util/session.h"
#include "util/tool.h"
<<<<<<< HEAD
#include "arch/common.h"

=======
#include "util/data.h"
#include "arch/common.h"
#include "util/block-range.h"

#include <dlfcn.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/bitmap.h>

struct perf_annotate {
	struct perf_tool tool;
<<<<<<< HEAD
	bool	   force, use_tui, use_stdio, use_gtk;
=======
	struct perf_session *session;
	bool	   use_tui, use_stdio, use_gtk;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bool	   full_paths;
	bool	   print_line;
	bool	   skip_missing;
	const char *sym_hist_filter;
	const char *cpu_list;
	DECLARE_BITMAP(cpu_bitmap, MAX_NR_CPUS);
};

<<<<<<< HEAD
=======
/*
 * Given one basic block:
 *
 *	from	to		branch_i
 *	* ----> *
 *		|
 *		| block
 *		v
 *		* ----> *
 *		from	to	branch_i+1
 *
 * where the horizontal are the branches and the vertical is the executed
 * block of instructions.
 *
 * We count, for each 'instruction', the number of blocks that covered it as
 * well as count the ratio each branch is taken.
 *
 * We can do this without knowing the actual instruction stream by keeping
 * track of the address ranges. We break down ranges such that there is no
 * overlap and iterate from the start until the end.
 *
 * @acme: once we parse the objdump output _before_ processing the samples,
 * we can easily fold the branch.cycles IPC bits in.
 */
static void process_basic_block(struct addr_map_symbol *start,
				struct addr_map_symbol *end,
				struct branch_flags *flags)
{
	struct symbol *sym = start->sym;
	struct annotation *notes = sym ? symbol__annotation(sym) : NULL;
	struct block_range_iter iter;
	struct block_range *entry;

	/*
	 * Sanity; NULL isn't executable and the CPU cannot execute backwards
	 */
	if (!start->addr || start->addr > end->addr)
		return;

	iter = block_range__create(start->addr, end->addr);
	if (!block_range_iter__valid(&iter))
		return;

	/*
	 * First block in range is a branch target.
	 */
	entry = block_range_iter(&iter);
	assert(entry->is_target);
	entry->entry++;

	do {
		entry = block_range_iter(&iter);

		entry->coverage++;
		entry->sym = sym;

		if (notes)
			notes->max_coverage = max(notes->max_coverage, entry->coverage);

	} while (block_range_iter__next(&iter));

	/*
	 * Last block in rage is a branch.
	 */
	entry = block_range_iter(&iter);
	assert(entry->is_branch);
	entry->taken++;
	if (flags->predicted)
		entry->pred++;
}

static void process_branch_stack(struct branch_stack *bs, struct addr_location *al,
				 struct perf_sample *sample)
{
	struct addr_map_symbol *prev = NULL;
	struct branch_info *bi;
	int i;

	if (!bs || !bs->nr)
		return;

	bi = sample__resolve_bstack(sample, al);
	if (!bi)
		return;

	for (i = bs->nr - 1; i >= 0; i--) {
		/*
		 * XXX filter against symbol
		 */
		if (prev)
			process_basic_block(prev, &bi[i].from, &bi[i].flags);
		prev = &bi[i].to;
	}

	free(bi);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int perf_evsel__add_sample(struct perf_evsel *evsel,
				  struct perf_sample *sample,
				  struct addr_location *al,
				  struct perf_annotate *ann)
{
<<<<<<< HEAD
=======
	struct hists *hists = evsel__hists(evsel);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct hist_entry *he;
	int ret;

	if (ann->sym_hist_filter != NULL &&
	    (al->sym == NULL ||
	     strcmp(ann->sym_hist_filter, al->sym->name) != 0)) {
		/* We're only interested in a symbol named sym_hist_filter */
<<<<<<< HEAD
=======
		/*
		 * FIXME: why isn't this done in the symbol_filter when loading
		 * the DSO?
		 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (al->sym != NULL) {
			rb_erase(&al->sym->rb_node,
				 &al->map->dso->symbols[al->map->type]);
			symbol__delete(al->sym);
<<<<<<< HEAD
=======
			dso__reset_find_symbol_cache(al->map->dso);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		return 0;
	}

<<<<<<< HEAD
	he = __hists__add_entry(&evsel->hists, al, NULL, 1, 1);
	if (he == NULL)
		return -ENOMEM;

	ret = 0;
	if (he->ms.sym != NULL) {
		struct annotation *notes = symbol__annotation(he->ms.sym);
		if (notes->src == NULL && symbol__alloc_hist(he->ms.sym) < 0)
			return -ENOMEM;

		ret = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	}

	evsel->hists.stats.total_period += sample->period;
	hists__inc_nr_events(&evsel->hists, PERF_RECORD_SAMPLE);
=======
	/*
	 * XXX filtered samples can still have branch entires pointing into our
	 * symbol and are missed.
	 */
	process_branch_stack(sample->branch_stack, al, sample);

	sample->period = 1;
	sample->weight = 1;

	he = hists__add_entry(hists, al, NULL, NULL, NULL, sample, true);
	if (he == NULL)
		return -ENOMEM;

	ret = hist_entry__inc_addr_samples(he, evsel->idx, al->addr);
	hists__inc_nr_samples(hists, true);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static int process_sample_event(struct perf_tool *tool,
				union perf_event *event,
				struct perf_sample *sample,
				struct perf_evsel *evsel,
				struct machine *machine)
{
	struct perf_annotate *ann = container_of(tool, struct perf_annotate, tool);
	struct addr_location al;
<<<<<<< HEAD

	if (perf_event__preprocess_sample(event, machine, &al, sample,
					  symbol__annotate_init) < 0) {
=======
	int ret = 0;

	if (machine__resolve(machine, &al, sample) < 0) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pr_warning("problem processing %d event, skipping it.\n",
			   event->header.type);
		return -1;
	}

	if (ann->cpu_list && !test_bit(sample->cpu, ann->cpu_bitmap))
<<<<<<< HEAD
		return 0;
=======
		goto out_put;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!al.filtered && perf_evsel__add_sample(evsel, sample, &al, ann)) {
		pr_warning("problem incrementing symbol count, "
			   "skipping event\n");
<<<<<<< HEAD
		return -1;
	}

	return 0;
=======
		ret = -1;
	}
out_put:
	addr_location__put(&al);
	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int hist_entry__tty_annotate(struct hist_entry *he,
				    struct perf_evsel *evsel,
				    struct perf_annotate *ann)
{
	return symbol__tty_annotate(he->ms.sym, he->ms.map, evsel,
				    ann->print_line, ann->full_paths, 0, 0);
}

<<<<<<< HEAD
static void hists__find_annotations(struct hists *self,
				    struct perf_evsel *evsel,
				    struct perf_annotate *ann)
{
	struct rb_node *nd = rb_first(&self->entries), *next;
=======
static void hists__find_annotations(struct hists *hists,
				    struct perf_evsel *evsel,
				    struct perf_annotate *ann)
{
	struct rb_node *nd = rb_first(&hists->entries), *next;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int key = K_RIGHT;

	while (nd) {
		struct hist_entry *he = rb_entry(nd, struct hist_entry, rb_node);
		struct annotation *notes;

		if (he->ms.sym == NULL || he->ms.map->dso->annotate_warned)
			goto find_next;

		notes = symbol__annotation(he->ms.sym);
		if (notes->src == NULL) {
find_next:
			if (key == K_LEFT)
				nd = rb_prev(nd);
			else
				nd = rb_next(nd);
			continue;
		}

		if (use_browser == 2) {
			int ret;
<<<<<<< HEAD

			ret = hist_entry__gtk_annotate(he, evsel, NULL);
=======
			int (*annotate)(struct hist_entry *he,
					struct perf_evsel *evsel,
					struct hist_browser_timer *hbt);

			annotate = dlsym(perf_gtk_handle,
					 "hist_entry__gtk_annotate");
			if (annotate == NULL) {
				ui__error("GTK browser not found!\n");
				return;
			}

			ret = annotate(he, evsel, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (!ret || !ann->skip_missing)
				return;

			/* skip missing symbols */
			nd = rb_next(nd);
		} else if (use_browser == 1) {
			key = hist_entry__tui_annotate(he, evsel, NULL);
			switch (key) {
			case -1:
				if (!ann->skip_missing)
					return;
				/* fall through */
			case K_RIGHT:
				next = rb_next(nd);
				break;
			case K_LEFT:
				next = rb_prev(nd);
				break;
			default:
				return;
			}

			if (next != NULL)
				nd = next;
		} else {
			hist_entry__tty_annotate(he, evsel, ann);
			nd = rb_next(nd);
			/*
			 * Since we have a hist_entry per IP for the same
			 * symbol, free he->ms.sym->src to signal we already
			 * processed this symbol.
			 */
<<<<<<< HEAD
			free(notes->src);
			notes->src = NULL;
=======
			zfree(&notes->src->cycles_hist);
			zfree(&notes->src);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
}

static int __cmd_annotate(struct perf_annotate *ann)
{
	int ret;
<<<<<<< HEAD
	struct perf_session *session;
	struct perf_evsel *pos;
	u64 total_nr_samples;

	session = perf_session__new(input_name, O_RDONLY,
				    ann->force, false, &ann->tool);
	if (session == NULL)
		return -ENOMEM;

=======
	struct perf_session *session = ann->session;
	struct perf_evsel *pos;
	u64 total_nr_samples;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ann->cpu_list) {
		ret = perf_session__cpu_bitmap(session, ann->cpu_list,
					       ann->cpu_bitmap);
		if (ret)
<<<<<<< HEAD
			goto out_delete;
	}

	if (!objdump_path) {
		ret = perf_session_env__lookup_objdump(&session->header.env);
		if (ret)
			goto out_delete;
	}

	ret = perf_session__process_events(session, &ann->tool);
	if (ret)
		goto out_delete;

	if (dump_trace) {
		perf_session__fprintf_nr_events(session, stdout);
		goto out_delete;
=======
			goto out;
	}

	if (!objdump_path) {
		ret = perf_env__lookup_objdump(&session->header.env);
		if (ret)
			goto out;
	}

	ret = perf_session__process_events(session);
	if (ret)
		goto out;

	if (dump_trace) {
		perf_session__fprintf_nr_events(session, stdout);
		perf_evlist__fprintf_nr_events(session->evlist, stdout);
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (verbose > 3)
		perf_session__fprintf(session, stdout);

	if (verbose > 2)
		perf_session__fprintf_dsos(session, stdout);

	total_nr_samples = 0;
<<<<<<< HEAD
	list_for_each_entry(pos, &session->evlist->entries, node) {
		struct hists *hists = &pos->hists;
=======
	evlist__for_each_entry(session->evlist, pos) {
		struct hists *hists = evsel__hists(pos);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		u32 nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];

		if (nr_samples > 0) {
			total_nr_samples += nr_samples;
<<<<<<< HEAD
			hists__collapse_resort(hists);
			hists__output_resort(hists);
=======
			hists__collapse_resort(hists, NULL);
			/* Don't sort callchain */
			perf_evsel__reset_sample_bit(pos, CALLCHAIN);
			perf_evsel__output_resort(pos, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			if (symbol_conf.event_group &&
			    !perf_evsel__is_group_leader(pos))
				continue;

			hists__find_annotations(hists, pos, ann);
		}
	}

	if (total_nr_samples == 0) {
<<<<<<< HEAD
		ui__error("The %s file has no samples!\n", session->filename);
		goto out_delete;
	}

	if (use_browser == 2)
		perf_gtk__show_annotations();

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
=======
		ui__error("The %s file has no samples!\n", session->file->path);
		goto out;
	}

	if (use_browser == 2) {
		void (*show_annotations)(void);

		show_annotations = dlsym(perf_gtk_handle,
					 "perf_gtk__show_annotations");
		if (show_annotations == NULL) {
			ui__error("GTK browser not found!\n");
			goto out;
		}
		show_annotations();
	}

out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static const char * const annotate_usage[] = {
	"perf annotate [<options>]",
	NULL
};

int cmd_annotate(int argc, const char **argv, const char *prefix __maybe_unused)
{
	struct perf_annotate annotate = {
		.tool = {
			.sample	= process_sample_event,
			.mmap	= perf_event__process_mmap,
<<<<<<< HEAD
			.comm	= perf_event__process_comm,
			.exit	= perf_event__process_exit,
			.fork	= perf_event__process_fork,
			.ordered_samples = true,
			.ordering_requires_timestamps = true,
		},
	};
=======
			.mmap2	= perf_event__process_mmap2,
			.comm	= perf_event__process_comm,
			.exit	= perf_event__process_exit,
			.fork	= perf_event__process_fork,
			.ordered_events = true,
			.ordering_requires_timestamps = true,
		},
	};
	struct perf_data_file file = {
		.mode  = PERF_DATA_MODE_READ,
	};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const struct option options[] = {
	OPT_STRING('i', "input", &input_name, "file",
		    "input file name"),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('s', "symbol", &annotate.sym_hist_filter, "symbol",
		    "symbol to annotate"),
<<<<<<< HEAD
	OPT_BOOLEAN('f', "force", &annotate.force, "don't complain, do it"),
=======
	OPT_BOOLEAN('f', "force", &file.force, "don't complain, do it"),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_BOOLEAN(0, "gtk", &annotate.use_gtk, "Use the GTK interface"),
	OPT_BOOLEAN(0, "tui", &annotate.use_tui, "Use the TUI interface"),
	OPT_BOOLEAN(0, "stdio", &annotate.use_stdio, "Use the stdio interface"),
	OPT_STRING('k', "vmlinux", &symbol_conf.vmlinux_name,
		   "file", "vmlinux pathname"),
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_BOOLEAN('l', "print-line", &annotate.print_line,
		    "print matching source lines (may be slow)"),
	OPT_BOOLEAN('P', "full-paths", &annotate.full_paths,
		    "Don't shorten the displayed pathnames"),
	OPT_BOOLEAN(0, "skip-missing", &annotate.skip_missing,
		    "Skip symbols that cannot be annotated"),
	OPT_STRING('C', "cpu", &annotate.cpu_list, "cpu", "list of cpus to profile"),
<<<<<<< HEAD
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		   "Look for files with symbols relative to this directory"),
=======
	OPT_CALLBACK(0, "symfs", NULL, "directory",
		     "Look for files with symbols relative to this directory",
		     symbol__config_symfs),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	OPT_BOOLEAN(0, "source", &symbol_conf.annotate_src,
		    "Interleave source code with assembly code (default)"),
	OPT_BOOLEAN(0, "asm-raw", &symbol_conf.annotate_asm_raw,
		    "Display raw encoding of assembly instructions (default)"),
	OPT_STRING('M', "disassembler-style", &disassembler_style, "disassembler style",
		   "Specify disassembler style (e.g. -M intel for intel syntax)"),
	OPT_STRING(0, "objdump", &objdump_path, "path",
		   "objdump binary to use for disassembly and annotations"),
	OPT_BOOLEAN(0, "group", &symbol_conf.event_group,
		    "Show event group information together"),
<<<<<<< HEAD
	OPT_END()
	};

	argc = parse_options(argc, argv, options, annotate_usage, 0);
=======
	OPT_BOOLEAN(0, "show-total-period", &symbol_conf.show_total_period,
		    "Show a column with the sum of periods"),
	OPT_CALLBACK_DEFAULT(0, "stdio-color", NULL, "mode",
			     "'always' (default), 'never' or 'auto' only applicable to --stdio mode",
			     stdio__config_color, "always"),
	OPT_END()
	};
	int ret = hists__init();

	if (ret < 0)
		return ret;

	argc = parse_options(argc, argv, options, annotate_usage, 0);
	if (argc) {
		/*
		 * Special case: if there's an argument left then assume that
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(annotate_usage, options);

		annotate.sym_hist_filter = argv[0];
	}

	file.path  = input_name;

	annotate.session = perf_session__new(&file, false, &annotate.tool);
	if (annotate.session == NULL)
		return -1;

	ret = symbol__annotation_init();
	if (ret < 0)
		goto out_delete;

	symbol_conf.try_vmlinux_path = true;

	ret = symbol__init(&annotate.session->header.env);
	if (ret < 0)
		goto out_delete;

	if (setup_sorting(NULL) < 0)
		usage_with_options(annotate_usage, options);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (annotate.use_stdio)
		use_browser = 0;
	else if (annotate.use_tui)
		use_browser = 1;
	else if (annotate.use_gtk)
		use_browser = 2;

	setup_browser(true);

<<<<<<< HEAD
	symbol_conf.priv_size = sizeof(struct annotation);
	symbol_conf.try_vmlinux_path = true;

	if (symbol__init() < 0)
		return -1;

	if (setup_sorting() < 0)
		usage_with_options(annotate_usage, options);

	if (argc) {
		/*
		 * Special case: if there's an argument left then assume tha
		 * it's a symbol filter:
		 */
		if (argc > 1)
			usage_with_options(annotate_usage, options);

		annotate.sym_hist_filter = argv[0];
	}

	return __cmd_annotate(&annotate);
=======
	ret = __cmd_annotate(&annotate);

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
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
