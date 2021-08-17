#include <stdio.h>
<<<<<<< HEAD
#include "../libslang.h"
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <stdlib.h>
#include <string.h>
#include <linux/rbtree.h>

#include "../../util/evsel.h"
#include "../../util/evlist.h"
#include "../../util/hist.h"
#include "../../util/pstack.h"
#include "../../util/sort.h"
#include "../../util/util.h"
<<<<<<< HEAD
#include "../../arch/common.h"

#include "../browser.h"
=======
#include "../../util/top.h"
#include "../../arch/common.h"

#include "../browsers/hists.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "../helpline.h"
#include "../util.h"
#include "../ui.h"
#include "map.h"
<<<<<<< HEAD

struct hist_browser {
	struct ui_browser   b;
	struct hists	    *hists;
	struct hist_entry   *he_selection;
	struct map_symbol   *selection;
	int		     print_seq;
	bool		     show_dso;
	bool		     has_symbols;
};

extern void hist_browser__init_hpp(void);

static int hists__browser_title(struct hists *hists, char *bf, size_t size,
				const char *ev_name);

static void hist_browser__refresh_dimensions(struct hist_browser *browser)
{
	/* 3 == +/- toggle symbol before actual hist_entry rendering */
	browser->b.width = 3 + (hists__sort_list_width(browser->hists) +
			     sizeof("[k]"));
=======
#include "annotate.h"

extern void hist_browser__init_hpp(void);

static int perf_evsel_browser_title(struct hist_browser *browser,
				    char *bf, size_t size);
static void hist_browser__update_nr_entries(struct hist_browser *hb);

static struct rb_node *hists__filter_entries(struct rb_node *nd,
					     float min_pcnt);

static bool hist_browser__has_filter(struct hist_browser *hb)
{
	return hists__has_filter(hb->hists) || hb->min_pcnt || symbol_conf.has_filter;
}

static int hist_browser__get_folding(struct hist_browser *browser)
{
	struct rb_node *nd;
	struct hists *hists = browser->hists;
	int unfolded_rows = 0;

	for (nd = rb_first(&hists->entries);
	     (nd = hists__filter_entries(nd, browser->min_pcnt)) != NULL;
	     nd = rb_hierarchy_next(nd)) {
		struct hist_entry *he =
			rb_entry(nd, struct hist_entry, rb_node);

		if (he->leaf && he->unfolded)
			unfolded_rows += he->nr_rows;
	}
	return unfolded_rows;
}

static u32 hist_browser__nr_entries(struct hist_browser *hb)
{
	u32 nr_entries;

	if (symbol_conf.report_hierarchy)
		nr_entries = hb->nr_hierarchy_entries;
	else if (hist_browser__has_filter(hb))
		nr_entries = hb->nr_non_filtered_entries;
	else
		nr_entries = hb->hists->nr_entries;

	hb->nr_callchain_rows = hist_browser__get_folding(hb);
	return nr_entries + hb->nr_callchain_rows;
}

static void hist_browser__update_rows(struct hist_browser *hb)
{
	struct ui_browser *browser = &hb->b;
	struct hists *hists = hb->hists;
	struct perf_hpp_list *hpp_list = hists->hpp_list;
	u16 header_offset, index_row;

	header_offset = hb->show_headers ? hpp_list->nr_header_lines : 0;
	browser->rows = browser->height - header_offset;
	/*
	 * Verify if we were at the last line and that line isn't
	 * visibe because we now show the header line(s).
	 */
	index_row = browser->index - browser->top_idx;
	if (index_row >= browser->rows)
		browser->index -= index_row - browser->rows + 1;
}

static void hist_browser__refresh_dimensions(struct ui_browser *browser)
{
	struct hist_browser *hb = container_of(browser, struct hist_browser, b);

	/* 3 == +/- toggle symbol before actual hist_entry rendering */
	browser->width = 3 + (hists__sort_list_width(hb->hists) + sizeof("[k]"));
	/*
 	 * FIXME: Just keeping existing behaviour, but this really should be
 	 *	  before updating browser->width, as it will invalidate the
 	 *	  calculation above. Fix this and the fallout in another
 	 *	  changeset.
 	 */
	ui_browser__refresh_dimensions(browser);
	hist_browser__update_rows(hb);
}

static void hist_browser__gotorc(struct hist_browser *browser, int row, int column)
{
	struct hists *hists = browser->hists;
	struct perf_hpp_list *hpp_list = hists->hpp_list;
	u16 header_offset;

	header_offset = browser->show_headers ? hpp_list->nr_header_lines : 0;
	ui_browser__gotorc(&browser->b, row + header_offset, column);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void hist_browser__reset(struct hist_browser *browser)
{
<<<<<<< HEAD
	browser->b.nr_entries = browser->hists->nr_entries;
	hist_browser__refresh_dimensions(browser);
=======
	/*
	 * The hists__remove_entry_filter() already folds non-filtered
	 * entries so we can assume it has 0 callchain rows.
	 */
	browser->nr_callchain_rows = 0;

	hist_browser__update_nr_entries(browser);
	browser->b.nr_entries = hist_browser__nr_entries(browser);
	hist_browser__refresh_dimensions(&browser->b);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ui_browser__reset_index(&browser->b);
}

static char tree__folded_sign(bool unfolded)
{
	return unfolded ? '-' : '+';
}

<<<<<<< HEAD
static char map_symbol__folded(const struct map_symbol *ms)
{
	return ms->has_children ? tree__folded_sign(ms->unfolded) : ' ';
}

static char hist_entry__folded(const struct hist_entry *he)
{
	return map_symbol__folded(&he->ms);
=======
static char hist_entry__folded(const struct hist_entry *he)
{
	return he->has_children ? tree__folded_sign(he->unfolded) : ' ';
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static char callchain_list__folded(const struct callchain_list *cl)
{
<<<<<<< HEAD
	return map_symbol__folded(&cl->ms);
}

static void map_symbol__set_folding(struct map_symbol *ms, bool unfold)
{
	ms->unfolded = unfold ? ms->has_children : false;
=======
	return cl->has_children ? tree__folded_sign(cl->unfolded) : ' ';
}

static void callchain_list__set_folding(struct callchain_list *cl, bool unfold)
{
	cl->unfolded = unfold ? cl->has_children : false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int callchain_node__count_rows_rb_tree(struct callchain_node *node)
{
	int n = 0;
	struct rb_node *nd;

	for (nd = rb_first(&node->rb_root); nd; nd = rb_next(nd)) {
		struct callchain_node *child = rb_entry(nd, struct callchain_node, rb_node);
		struct callchain_list *chain;
		char folded_sign = ' '; /* No children */

		list_for_each_entry(chain, &child->val, list) {
			++n;
			/* We need this because we may not have children */
			folded_sign = callchain_list__folded(chain);
			if (folded_sign == '+')
				break;
		}

		if (folded_sign == '-') /* Have children and they're unfolded */
			n += callchain_node__count_rows_rb_tree(child);
	}

	return n;
}

<<<<<<< HEAD
=======
static int callchain_node__count_flat_rows(struct callchain_node *node)
{
	struct callchain_list *chain;
	char folded_sign = 0;
	int n = 0;

	list_for_each_entry(chain, &node->parent_val, list) {
		if (!folded_sign) {
			/* only check first chain list entry */
			folded_sign = callchain_list__folded(chain);
			if (folded_sign == '+')
				return 1;
		}
		n++;
	}

	list_for_each_entry(chain, &node->val, list) {
		if (!folded_sign) {
			/* node->parent_val list might be empty */
			folded_sign = callchain_list__folded(chain);
			if (folded_sign == '+')
				return 1;
		}
		n++;
	}

	return n;
}

static int callchain_node__count_folded_rows(struct callchain_node *node __maybe_unused)
{
	return 1;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int callchain_node__count_rows(struct callchain_node *node)
{
	struct callchain_list *chain;
	bool unfolded = false;
	int n = 0;

<<<<<<< HEAD
	list_for_each_entry(chain, &node->val, list) {
		++n;
		unfolded = chain->ms.unfolded;
=======
	if (callchain_param.mode == CHAIN_FLAT)
		return callchain_node__count_flat_rows(node);
	else if (callchain_param.mode == CHAIN_FOLDED)
		return callchain_node__count_folded_rows(node);

	list_for_each_entry(chain, &node->val, list) {
		++n;
		unfolded = chain->unfolded;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (unfolded)
		n += callchain_node__count_rows_rb_tree(node);

	return n;
}

static int callchain__count_rows(struct rb_root *chain)
{
	struct rb_node *nd;
	int n = 0;

	for (nd = rb_first(chain); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);
		n += callchain_node__count_rows(node);
	}

	return n;
}

<<<<<<< HEAD
static bool map_symbol__toggle_fold(struct map_symbol *ms)
{
	if (!ms)
		return false;

	if (!ms->has_children)
		return false;

	ms->unfolded = !ms->unfolded;
=======
static int hierarchy_count_rows(struct hist_browser *hb, struct hist_entry *he,
				bool include_children)
{
	int count = 0;
	struct rb_node *node;
	struct hist_entry *child;

	if (he->leaf)
		return callchain__count_rows(&he->sorted_chain);

	if (he->has_no_entry)
		return 1;

	node = rb_first(&he->hroot_out);
	while (node) {
		float percent;

		child = rb_entry(node, struct hist_entry, rb_node);
		percent = hist_entry__get_percent_limit(child);

		if (!child->filtered && percent >= hb->min_pcnt) {
			count++;

			if (include_children && child->unfolded)
				count += hierarchy_count_rows(hb, child, true);
		}

		node = rb_next(node);
	}
	return count;
}

static bool hist_entry__toggle_fold(struct hist_entry *he)
{
	if (!he)
		return false;

	if (!he->has_children)
		return false;

	he->unfolded = !he->unfolded;
	return true;
}

static bool callchain_list__toggle_fold(struct callchain_list *cl)
{
	if (!cl)
		return false;

	if (!cl->has_children)
		return false;

	cl->unfolded = !cl->unfolded;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return true;
}

static void callchain_node__init_have_children_rb_tree(struct callchain_node *node)
{
	struct rb_node *nd = rb_first(&node->rb_root);

	for (nd = rb_first(&node->rb_root); nd; nd = rb_next(nd)) {
		struct callchain_node *child = rb_entry(nd, struct callchain_node, rb_node);
		struct callchain_list *chain;
		bool first = true;

		list_for_each_entry(chain, &child->val, list) {
			if (first) {
				first = false;
<<<<<<< HEAD
				chain->ms.has_children = chain->list.next != &child->val ||
							 !RB_EMPTY_ROOT(&child->rb_root);
			} else
				chain->ms.has_children = chain->list.next == &child->val &&
=======
				chain->has_children = chain->list.next != &child->val ||
							 !RB_EMPTY_ROOT(&child->rb_root);
			} else
				chain->has_children = chain->list.next == &child->val &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
							 !RB_EMPTY_ROOT(&child->rb_root);
		}

		callchain_node__init_have_children_rb_tree(child);
	}
}

<<<<<<< HEAD
static void callchain_node__init_have_children(struct callchain_node *node)
{
	struct callchain_list *chain;

	list_for_each_entry(chain, &node->val, list)
		chain->ms.has_children = !RB_EMPTY_ROOT(&node->rb_root);
=======
static void callchain_node__init_have_children(struct callchain_node *node,
					       bool has_sibling)
{
	struct callchain_list *chain;

	chain = list_entry(node->val.next, struct callchain_list, list);
	chain->has_children = has_sibling;

	if (!list_empty(&node->val)) {
		chain = list_entry(node->val.prev, struct callchain_list, list);
		chain->has_children = !RB_EMPTY_ROOT(&node->rb_root);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	callchain_node__init_have_children_rb_tree(node);
}

static void callchain__init_have_children(struct rb_root *root)
{
<<<<<<< HEAD
	struct rb_node *nd;

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);
		callchain_node__init_have_children(node);
=======
	struct rb_node *nd = rb_first(root);
	bool has_sibling = nd && rb_next(nd);

	for (nd = rb_first(root); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);
		callchain_node__init_have_children(node, has_sibling);
		if (callchain_param.mode == CHAIN_FLAT ||
		    callchain_param.mode == CHAIN_FOLDED)
			callchain_node__make_parent_list(node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

static void hist_entry__init_have_children(struct hist_entry *he)
{
<<<<<<< HEAD
	if (!he->init_have_children) {
		he->ms.has_children = !RB_EMPTY_ROOT(&he->sorted_chain);
		callchain__init_have_children(&he->sorted_chain);
		he->init_have_children = true;
	}
=======
	if (he->init_have_children)
		return;

	if (he->leaf) {
		he->has_children = !RB_EMPTY_ROOT(&he->sorted_chain);
		callchain__init_have_children(&he->sorted_chain);
	} else {
		he->has_children = !RB_EMPTY_ROOT(&he->hroot_out);
	}

	he->init_have_children = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool hist_browser__toggle_fold(struct hist_browser *browser)
{
<<<<<<< HEAD
	if (map_symbol__toggle_fold(browser->selection)) {
		struct hist_entry *he = browser->he_selection;

		hist_entry__init_have_children(he);
		browser->hists->nr_entries -= he->nr_rows;

		if (he->ms.unfolded)
			he->nr_rows = callchain__count_rows(&he->sorted_chain);
		else
			he->nr_rows = 0;
		browser->hists->nr_entries += he->nr_rows;
		browser->b.nr_entries = browser->hists->nr_entries;
=======
	struct hist_entry *he = browser->he_selection;
	struct map_symbol *ms = browser->selection;
	struct callchain_list *cl = container_of(ms, struct callchain_list, ms);
	bool has_children;

	if (!he || !ms)
		return false;

	if (ms == &he->ms)
		has_children = hist_entry__toggle_fold(he);
	else
		has_children = callchain_list__toggle_fold(cl);

	if (has_children) {
		int child_rows = 0;

		hist_entry__init_have_children(he);
		browser->b.nr_entries -= he->nr_rows;

		if (he->leaf)
			browser->nr_callchain_rows -= he->nr_rows;
		else
			browser->nr_hierarchy_entries -= he->nr_rows;

		if (symbol_conf.report_hierarchy)
			child_rows = hierarchy_count_rows(browser, he, true);

		if (he->unfolded) {
			if (he->leaf)
				he->nr_rows = callchain__count_rows(&he->sorted_chain);
			else
				he->nr_rows = hierarchy_count_rows(browser, he, false);

			/* account grand children */
			if (symbol_conf.report_hierarchy)
				browser->b.nr_entries += child_rows - he->nr_rows;

			if (!he->leaf && he->nr_rows == 0) {
				he->has_no_entry = true;
				he->nr_rows = 1;
			}
		} else {
			if (symbol_conf.report_hierarchy)
				browser->b.nr_entries -= child_rows - he->nr_rows;

			if (he->has_no_entry)
				he->has_no_entry = false;

			he->nr_rows = 0;
		}

		browser->b.nr_entries += he->nr_rows;

		if (he->leaf)
			browser->nr_callchain_rows += he->nr_rows;
		else
			browser->nr_hierarchy_entries += he->nr_rows;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		return true;
	}

	/* If it doesn't have children, no toggling performed */
	return false;
}

static int callchain_node__set_folding_rb_tree(struct callchain_node *node, bool unfold)
{
	int n = 0;
	struct rb_node *nd;

	for (nd = rb_first(&node->rb_root); nd; nd = rb_next(nd)) {
		struct callchain_node *child = rb_entry(nd, struct callchain_node, rb_node);
		struct callchain_list *chain;
		bool has_children = false;

		list_for_each_entry(chain, &child->val, list) {
			++n;
<<<<<<< HEAD
			map_symbol__set_folding(&chain->ms, unfold);
			has_children = chain->ms.has_children;
=======
			callchain_list__set_folding(chain, unfold);
			has_children = chain->has_children;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		if (has_children)
			n += callchain_node__set_folding_rb_tree(child, unfold);
	}

	return n;
}

static int callchain_node__set_folding(struct callchain_node *node, bool unfold)
{
	struct callchain_list *chain;
	bool has_children = false;
	int n = 0;

	list_for_each_entry(chain, &node->val, list) {
		++n;
<<<<<<< HEAD
		map_symbol__set_folding(&chain->ms, unfold);
		has_children = chain->ms.has_children;
=======
		callchain_list__set_folding(chain, unfold);
		has_children = chain->has_children;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (has_children)
		n += callchain_node__set_folding_rb_tree(node, unfold);

	return n;
}

static int callchain__set_folding(struct rb_root *chain, bool unfold)
{
	struct rb_node *nd;
	int n = 0;

	for (nd = rb_first(chain); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);
		n += callchain_node__set_folding(node, unfold);
	}

	return n;
}

<<<<<<< HEAD
static void hist_entry__set_folding(struct hist_entry *he, bool unfold)
{
	hist_entry__init_have_children(he);
	map_symbol__set_folding(&he->ms, unfold);

	if (he->ms.has_children) {
		int n = callchain__set_folding(&he->sorted_chain, unfold);
=======
static int hierarchy_set_folding(struct hist_browser *hb, struct hist_entry *he,
				 bool unfold __maybe_unused)
{
	float percent;
	struct rb_node *nd;
	struct hist_entry *child;
	int n = 0;

	for (nd = rb_first(&he->hroot_out); nd; nd = rb_next(nd)) {
		child = rb_entry(nd, struct hist_entry, rb_node);
		percent = hist_entry__get_percent_limit(child);
		if (!child->filtered && percent >= hb->min_pcnt)
			n++;
	}

	return n;
}

static void hist_entry__set_folding(struct hist_entry *he,
				    struct hist_browser *hb, bool unfold)
{
	hist_entry__init_have_children(he);
	he->unfolded = unfold ? he->has_children : false;

	if (he->has_children) {
		int n;

		if (he->leaf)
			n = callchain__set_folding(&he->sorted_chain, unfold);
		else
			n = hierarchy_set_folding(hb, he, unfold);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		he->nr_rows = unfold ? n : 0;
	} else
		he->nr_rows = 0;
}

<<<<<<< HEAD
static void hists__set_folding(struct hists *hists, bool unfold)
{
	struct rb_node *nd;

	hists->nr_entries = 0;

	for (nd = rb_first(&hists->entries); nd; nd = rb_next(nd)) {
		struct hist_entry *he = rb_entry(nd, struct hist_entry, rb_node);
		hist_entry__set_folding(he, unfold);
		hists->nr_entries += 1 + he->nr_rows;
=======
static void
__hist_browser__set_folding(struct hist_browser *browser, bool unfold)
{
	struct rb_node *nd;
	struct hist_entry *he;
	double percent;

	nd = rb_first(&browser->hists->entries);
	while (nd) {
		he = rb_entry(nd, struct hist_entry, rb_node);

		/* set folding state even if it's currently folded */
		nd = __rb_hierarchy_next(nd, HMD_FORCE_CHILD);

		hist_entry__set_folding(he, browser, unfold);

		percent = hist_entry__get_percent_limit(he);
		if (he->filtered || percent < browser->min_pcnt)
			continue;

		if (!he->depth || unfold)
			browser->nr_hierarchy_entries++;
		if (he->leaf)
			browser->nr_callchain_rows += he->nr_rows;
		else if (unfold && !hist_entry__has_hierarchy_children(he, browser->min_pcnt)) {
			browser->nr_hierarchy_entries++;
			he->has_no_entry = true;
			he->nr_rows = 1;
		} else
			he->has_no_entry = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

static void hist_browser__set_folding(struct hist_browser *browser, bool unfold)
{
<<<<<<< HEAD
	hists__set_folding(browser->hists, unfold);
	browser->b.nr_entries = browser->hists->nr_entries;
=======
	browser->nr_hierarchy_entries = 0;
	browser->nr_callchain_rows = 0;
	__hist_browser__set_folding(browser, unfold);

	browser->b.nr_entries = hist_browser__nr_entries(browser);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* Go to the start, we may be way after valid entries after a collapse */
	ui_browser__reset_index(&browser->b);
}

static void ui_browser__warn_lost_events(struct ui_browser *browser)
{
	ui_browser__warning(browser, 4,
		"Events are being lost, check IO/CPU overload!\n\n"
		"You may want to run 'perf' using a RT scheduler policy:\n\n"
		" perf top -r 80\n\n"
		"Or reduce the sampling frequency.");
}

<<<<<<< HEAD
static int hist_browser__run(struct hist_browser *browser, const char *ev_name,
			     struct hist_browser_timer *hbt)
{
	int key;
	char title[160];
	int delay_secs = hbt ? hbt->refresh : 0;

	browser->b.entries = &browser->hists->entries;
	browser->b.nr_entries = browser->hists->nr_entries;

	hist_browser__refresh_dimensions(browser);
	hists__browser_title(browser->hists, title, sizeof(title), ev_name);

	if (ui_browser__show(&browser->b, title,
			     "Press '?' for help on key bindings") < 0)
=======
static int hist_browser__title(struct hist_browser *browser, char *bf, size_t size)
{
	return browser->title ? browser->title(browser, bf, size) : 0;
}

int hist_browser__run(struct hist_browser *browser, const char *help)
{
	int key;
	char title[160];
	struct hist_browser_timer *hbt = browser->hbt;
	int delay_secs = hbt ? hbt->refresh : 0;

	browser->b.entries = &browser->hists->entries;
	browser->b.nr_entries = hist_browser__nr_entries(browser);

	hist_browser__title(browser, title, sizeof(title));

	if (ui_browser__show(&browser->b, title, "%s", help) < 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -1;

	while (1) {
		key = ui_browser__run(&browser->b, delay_secs);

		switch (key) {
<<<<<<< HEAD
		case K_TIMER:
			hbt->timer(hbt->arg);
			ui_browser__update_nr_entries(&browser->b, browser->hists->nr_entries);
=======
		case K_TIMER: {
			u64 nr_entries;
			hbt->timer(hbt->arg);

			if (hist_browser__has_filter(browser) ||
			    symbol_conf.report_hierarchy)
				hist_browser__update_nr_entries(browser);

			nr_entries = hist_browser__nr_entries(browser);
			ui_browser__update_nr_entries(&browser->b, nr_entries);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

			if (browser->hists->stats.nr_lost_warned !=
			    browser->hists->stats.nr_events[PERF_RECORD_LOST]) {
				browser->hists->stats.nr_lost_warned =
					browser->hists->stats.nr_events[PERF_RECORD_LOST];
				ui_browser__warn_lost_events(&browser->b);
			}

<<<<<<< HEAD
			hists__browser_title(browser->hists, title, sizeof(title), ev_name);
			ui_browser__show_title(&browser->b, title);
			continue;
=======
			hist_browser__title(browser, title, sizeof(title));
			ui_browser__show_title(&browser->b, title);
			continue;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case 'D': { /* Debug */
			static int seq;
			struct hist_entry *h = rb_entry(browser->b.top,
							struct hist_entry, rb_node);
			ui_helpline__pop();
<<<<<<< HEAD
			ui_helpline__fpush("%d: nr_ent=(%d,%d), height=%d, idx=%d, fve: idx=%d, row_off=%d, nrows=%d",
					   seq++, browser->b.nr_entries,
					   browser->hists->nr_entries,
					   browser->b.height,
=======
			ui_helpline__fpush("%d: nr_ent=(%d,%d), rows=%d, idx=%d, fve: idx=%d, row_off=%d, nrows=%d",
					   seq++, browser->b.nr_entries,
					   browser->hists->nr_entries,
					   browser->b.rows,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					   browser->b.index,
					   browser->b.top_idx,
					   h->row_offset, h->nr_rows);
		}
			break;
		case 'C':
			/* Collapse the whole world. */
			hist_browser__set_folding(browser, false);
			break;
		case 'E':
			/* Expand the whole world. */
			hist_browser__set_folding(browser, true);
			break;
<<<<<<< HEAD
=======
		case 'H':
			browser->show_headers = !browser->show_headers;
			hist_browser__update_rows(browser);
			break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case K_ENTER:
			if (hist_browser__toggle_fold(browser))
				break;
			/* fall thru */
		default:
			goto out;
		}
	}
out:
	ui_browser__hide(&browser->b);
	return key;
}

<<<<<<< HEAD
static char *callchain_list__sym_name(struct callchain_list *cl,
				      char *bf, size_t bfsize, bool show_dso)
{
	int printed;

	if (cl->ms.sym)
		printed = scnprintf(bf, bfsize, "%s", cl->ms.sym->name);
	else
		printed = scnprintf(bf, bfsize, "%#" PRIx64, cl->ip);

	if (show_dso)
		scnprintf(bf + printed, bfsize - printed, " %s",
			  cl->ms.map ? cl->ms.map->dso->short_name : "unknown");

	return bf;
=======
struct callchain_print_arg {
	/* for hists browser */
	off_t	row_offset;
	bool	is_current_entry;

	/* for file dump */
	FILE	*fp;
	int	printed;
};

typedef void (*print_callchain_entry_fn)(struct hist_browser *browser,
					 struct callchain_list *chain,
					 const char *str, int offset,
					 unsigned short row,
					 struct callchain_print_arg *arg);

static void hist_browser__show_callchain_entry(struct hist_browser *browser,
					       struct callchain_list *chain,
					       const char *str, int offset,
					       unsigned short row,
					       struct callchain_print_arg *arg)
{
	int color, width;
	char folded_sign = callchain_list__folded(chain);
	bool show_annotated = browser->show_dso && chain->ms.sym && symbol__annotation(chain->ms.sym)->src;

	color = HE_COLORSET_NORMAL;
	width = browser->b.width - (offset + 2);
	if (ui_browser__is_current_entry(&browser->b, row)) {
		browser->selection = &chain->ms;
		color = HE_COLORSET_SELECTED;
		arg->is_current_entry = true;
	}

	ui_browser__set_color(&browser->b, color);
	hist_browser__gotorc(browser, row, 0);
	ui_browser__write_nstring(&browser->b, " ", offset);
	ui_browser__printf(&browser->b, "%c", folded_sign);
	ui_browser__write_graph(&browser->b, show_annotated ? SLSMG_RARROW_CHAR : ' ');
	ui_browser__write_nstring(&browser->b, str, width);
}

static void hist_browser__fprintf_callchain_entry(struct hist_browser *b __maybe_unused,
						  struct callchain_list *chain,
						  const char *str, int offset,
						  unsigned short row __maybe_unused,
						  struct callchain_print_arg *arg)
{
	char folded_sign = callchain_list__folded(chain);

	arg->printed += fprintf(arg->fp, "%*s%c %s\n", offset, " ",
				folded_sign, str);
}

typedef bool (*check_output_full_fn)(struct hist_browser *browser,
				     unsigned short row);

static bool hist_browser__check_output_full(struct hist_browser *browser,
					    unsigned short row)
{
	return browser->b.rows == row;
}

static bool hist_browser__check_dump_full(struct hist_browser *browser __maybe_unused,
					  unsigned short row __maybe_unused)
{
	return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#define LEVEL_OFFSET_STEP 3

<<<<<<< HEAD
static int hist_browser__show_callchain_node_rb_tree(struct hist_browser *browser,
						     struct callchain_node *chain_node,
						     u64 total, int level,
						     unsigned short row,
						     off_t *row_offset,
						     bool *is_current_entry)
{
	struct rb_node *node;
	int first_row = row, width, offset = level * LEVEL_OFFSET_STEP;
	u64 new_total, remaining;

	if (callchain_param.mode == CHAIN_GRAPH_REL)
		new_total = chain_node->children_hit;
	else
		new_total = total;

	remaining = new_total;
	node = rb_first(&chain_node->rb_root);
	while (node) {
		struct callchain_node *child = rb_entry(node, struct callchain_node, rb_node);
		struct rb_node *next = rb_next(node);
		u64 cumul = callchain_cumul_hits(child);
=======
static int hist_browser__show_callchain_list(struct hist_browser *browser,
					     struct callchain_node *node,
					     struct callchain_list *chain,
					     unsigned short row, u64 total,
					     bool need_percent, int offset,
					     print_callchain_entry_fn print,
					     struct callchain_print_arg *arg)
{
	char bf[1024], *alloc_str;
	const char *str;

	if (arg->row_offset != 0) {
		arg->row_offset--;
		return 0;
	}

	alloc_str = NULL;
	str = callchain_list__sym_name(chain, bf, sizeof(bf),
				       browser->show_dso);

	if (need_percent) {
		char buf[64];

		callchain_node__scnprintf_value(node, buf, sizeof(buf),
						total);

		if (asprintf(&alloc_str, "%s %s", buf, str) < 0)
			str = "Not enough memory!";
		else
			str = alloc_str;
	}

	print(browser, chain, str, offset, row, arg);

	free(alloc_str);
	return 1;
}

static bool check_percent_display(struct rb_node *node, u64 parent_total)
{
	struct callchain_node *child;

	if (node == NULL)
		return false;

	if (rb_next(node))
		return true;

	child = rb_entry(node, struct callchain_node, rb_node);
	return callchain_cumul_hits(child) != parent_total;
}

static int hist_browser__show_callchain_flat(struct hist_browser *browser,
					     struct rb_root *root,
					     unsigned short row, u64 total,
					     u64 parent_total,
					     print_callchain_entry_fn print,
					     struct callchain_print_arg *arg,
					     check_output_full_fn is_output_full)
{
	struct rb_node *node;
	int first_row = row, offset = LEVEL_OFFSET_STEP;
	bool need_percent;

	node = rb_first(root);
	need_percent = check_percent_display(node, parent_total);

	while (node) {
		struct callchain_node *child = rb_entry(node, struct callchain_node, rb_node);
		struct rb_node *next = rb_next(node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct callchain_list *chain;
		char folded_sign = ' ';
		int first = true;
		int extra_offset = 0;

<<<<<<< HEAD
		remaining -= cumul;

		list_for_each_entry(chain, &child->val, list) {
			char bf[1024], *alloc_str;
			const char *str;
			int color;
=======
		list_for_each_entry(chain, &child->parent_val, list) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			bool was_first = first;

			if (first)
				first = false;
<<<<<<< HEAD
			else
				extra_offset = LEVEL_OFFSET_STEP;

			folded_sign = callchain_list__folded(chain);
			if (*row_offset != 0) {
				--*row_offset;
				goto do_next;
			}

			alloc_str = NULL;
			str = callchain_list__sym_name(chain, bf, sizeof(bf),
						       browser->show_dso);
			if (was_first) {
				double percent = cumul * 100.0 / new_total;

				if (asprintf(&alloc_str, "%2.2f%% %s", percent, str) < 0)
					str = "Not enough memory!";
				else
					str = alloc_str;
			}

			color = HE_COLORSET_NORMAL;
			width = browser->b.width - (offset + extra_offset + 2);
			if (ui_browser__is_current_entry(&browser->b, row)) {
				browser->selection = &chain->ms;
				color = HE_COLORSET_SELECTED;
				*is_current_entry = true;
			}

			ui_browser__set_color(&browser->b, color);
			ui_browser__gotorc(&browser->b, row, 0);
			slsmg_write_nstring(" ", offset + extra_offset);
			slsmg_printf("%c ", folded_sign);
			slsmg_write_nstring(str, width);
			free(alloc_str);

			if (++row == browser->b.height)
				goto out;
do_next:
=======
			else if (need_percent)
				extra_offset = LEVEL_OFFSET_STEP;

			folded_sign = callchain_list__folded(chain);

			row += hist_browser__show_callchain_list(browser, child,
							chain, row, total,
							was_first && need_percent,
							offset + extra_offset,
							print, arg);

			if (is_output_full(browser, row))
				goto out;

			if (folded_sign == '+')
				goto next;
		}

		list_for_each_entry(chain, &child->val, list) {
			bool was_first = first;

			if (first)
				first = false;
			else if (need_percent)
				extra_offset = LEVEL_OFFSET_STEP;

			folded_sign = callchain_list__folded(chain);

			row += hist_browser__show_callchain_list(browser, child,
							chain, row, total,
							was_first && need_percent,
							offset + extra_offset,
							print, arg);

			if (is_output_full(browser, row))
				goto out;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (folded_sign == '+')
				break;
		}

<<<<<<< HEAD
		if (folded_sign == '-') {
			const int new_level = level + (extra_offset ? 2 : 1);
			row += hist_browser__show_callchain_node_rb_tree(browser, child, new_total,
									 new_level, row, row_offset,
									 is_current_entry);
		}
		if (row == browser->b.height)
			goto out;
=======
next:
		if (is_output_full(browser, row))
			break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		node = next;
	}
out:
	return row - first_row;
}

<<<<<<< HEAD
static int hist_browser__show_callchain_node(struct hist_browser *browser,
					     struct callchain_node *node,
					     int level, unsigned short row,
					     off_t *row_offset,
					     bool *is_current_entry)
{
	struct callchain_list *chain;
	int first_row = row,
	     offset = level * LEVEL_OFFSET_STEP,
	     width = browser->b.width - offset;
	char folded_sign = ' ';

	list_for_each_entry(chain, &node->val, list) {
		char bf[1024], *s;
		int color;

		folded_sign = callchain_list__folded(chain);

		if (*row_offset != 0) {
			--*row_offset;
			continue;
		}

		color = HE_COLORSET_NORMAL;
		if (ui_browser__is_current_entry(&browser->b, row)) {
			browser->selection = &chain->ms;
			color = HE_COLORSET_SELECTED;
			*is_current_entry = true;
		}

		s = callchain_list__sym_name(chain, bf, sizeof(bf),
					     browser->show_dso);
		ui_browser__gotorc(&browser->b, row, 0);
		ui_browser__set_color(&browser->b, color);
		slsmg_write_nstring(" ", offset);
		slsmg_printf("%c ", folded_sign);
		slsmg_write_nstring(s, width - 2);

		if (++row == browser->b.height)
			goto out;
	}

	if (folded_sign == '-')
		row += hist_browser__show_callchain_node_rb_tree(browser, node,
								 browser->hists->stats.total_period,
								 level + 1, row,
								 row_offset,
								 is_current_entry);
out:
	return row - first_row;
}

static int hist_browser__show_callchain(struct hist_browser *browser,
					struct rb_root *chain,
					int level, unsigned short row,
					off_t *row_offset,
					bool *is_current_entry)
{
	struct rb_node *nd;
	int first_row = row;

	for (nd = rb_first(chain); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);

		row += hist_browser__show_callchain_node(browser, node, level,
							 row, row_offset,
							 is_current_entry);
		if (row == browser->b.height)
			break;
=======
static char *hist_browser__folded_callchain_str(struct hist_browser *browser,
						struct callchain_list *chain,
						char *value_str, char *old_str)
{
	char bf[1024];
	const char *str;
	char *new;

	str = callchain_list__sym_name(chain, bf, sizeof(bf),
				       browser->show_dso);
	if (old_str) {
		if (asprintf(&new, "%s%s%s", old_str,
			     symbol_conf.field_sep ?: ";", str) < 0)
			new = NULL;
	} else {
		if (value_str) {
			if (asprintf(&new, "%s %s", value_str, str) < 0)
				new = NULL;
		} else {
			if (asprintf(&new, "%s", str) < 0)
				new = NULL;
		}
	}
	return new;
}

static int hist_browser__show_callchain_folded(struct hist_browser *browser,
					       struct rb_root *root,
					       unsigned short row, u64 total,
					       u64 parent_total,
					       print_callchain_entry_fn print,
					       struct callchain_print_arg *arg,
					       check_output_full_fn is_output_full)
{
	struct rb_node *node;
	int first_row = row, offset = LEVEL_OFFSET_STEP;
	bool need_percent;

	node = rb_first(root);
	need_percent = check_percent_display(node, parent_total);

	while (node) {
		struct callchain_node *child = rb_entry(node, struct callchain_node, rb_node);
		struct rb_node *next = rb_next(node);
		struct callchain_list *chain, *first_chain = NULL;
		int first = true;
		char *value_str = NULL, *value_str_alloc = NULL;
		char *chain_str = NULL, *chain_str_alloc = NULL;

		if (arg->row_offset != 0) {
			arg->row_offset--;
			goto next;
		}

		if (need_percent) {
			char buf[64];

			callchain_node__scnprintf_value(child, buf, sizeof(buf), total);
			if (asprintf(&value_str, "%s", buf) < 0) {
				value_str = (char *)"<...>";
				goto do_print;
			}
			value_str_alloc = value_str;
		}

		list_for_each_entry(chain, &child->parent_val, list) {
			chain_str = hist_browser__folded_callchain_str(browser,
						chain, value_str, chain_str);
			if (first) {
				first = false;
				first_chain = chain;
			}

			if (chain_str == NULL) {
				chain_str = (char *)"Not enough memory!";
				goto do_print;
			}

			chain_str_alloc = chain_str;
		}

		list_for_each_entry(chain, &child->val, list) {
			chain_str = hist_browser__folded_callchain_str(browser,
						chain, value_str, chain_str);
			if (first) {
				first = false;
				first_chain = chain;
			}

			if (chain_str == NULL) {
				chain_str = (char *)"Not enough memory!";
				goto do_print;
			}

			chain_str_alloc = chain_str;
		}

do_print:
		print(browser, first_chain, chain_str, offset, row++, arg);
		free(value_str_alloc);
		free(chain_str_alloc);

next:
		if (is_output_full(browser, row))
			break;
		node = next;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return row - first_row;
}

<<<<<<< HEAD
struct hpp_arg {
	struct ui_browser *b;
	char folded_sign;
	bool current_entry;
};

static int __hpp__color_callchain(struct hpp_arg *arg)
{
	if (!symbol_conf.use_callchain)
		return 0;

	slsmg_printf("%c ", arg->folded_sign);
	return 2;
}

static int __hpp__color_fmt(struct perf_hpp *hpp, struct hist_entry *he,
			    u64 (*get_field)(struct hist_entry *),
			    int (*callchain_cb)(struct hpp_arg *))
{
	int ret = 0;
	double percent = 0.0;
	struct hists *hists = he->hists;
	struct hpp_arg *arg = hpp->ptr;

	if (hists->stats.total_period)
		percent = 100.0 * get_field(he) / hists->stats.total_period;

	ui_browser__set_percent_color(arg->b, percent, arg->current_entry);

	if (callchain_cb)
		ret += callchain_cb(arg);

	ret += scnprintf(hpp->buf, hpp->size, "%6.2f%%", percent);
	slsmg_printf("%s", hpp->buf);

	if (symbol_conf.event_group) {
		int prev_idx, idx_delta;
		struct perf_evsel *evsel = hists_to_evsel(hists);
		struct hist_entry *pair;
		int nr_members = evsel->nr_members;

		if (nr_members <= 1)
			goto out;

		prev_idx = perf_evsel__group_idx(evsel);

		list_for_each_entry(pair, &he->pairs.head, pairs.node) {
			u64 period = get_field(pair);
			u64 total = pair->hists->stats.total_period;

			if (!total)
				continue;

			evsel = hists_to_evsel(pair->hists);
			idx_delta = perf_evsel__group_idx(evsel) - prev_idx - 1;

			while (idx_delta--) {
				/*
				 * zero-fill group members in the middle which
				 * have no sample
				 */
				ui_browser__set_percent_color(arg->b, 0.0,
							arg->current_entry);
				ret += scnprintf(hpp->buf, hpp->size,
						 " %6.2f%%", 0.0);
				slsmg_printf("%s", hpp->buf);
			}

			percent = 100.0 * period / total;
			ui_browser__set_percent_color(arg->b, percent,
						      arg->current_entry);
			ret += scnprintf(hpp->buf, hpp->size,
					 " %6.2f%%", percent);
			slsmg_printf("%s", hpp->buf);

			prev_idx = perf_evsel__group_idx(evsel);
		}

		idx_delta = nr_members - prev_idx - 1;

		while (idx_delta--) {
			/*
			 * zero-fill group members at last which have no sample
			 */
			ui_browser__set_percent_color(arg->b, 0.0,
						      arg->current_entry);
			ret += scnprintf(hpp->buf, hpp->size,
					 " %6.2f%%", 0.0);
			slsmg_printf("%s", hpp->buf);
		}
	}
out:
	if (!arg->current_entry || !arg->b->navkeypressed)
		ui_browser__set_color(arg->b, HE_COLORSET_NORMAL);
=======
static int hist_browser__show_callchain_graph(struct hist_browser *browser,
					struct rb_root *root, int level,
					unsigned short row, u64 total,
					u64 parent_total,
					print_callchain_entry_fn print,
					struct callchain_print_arg *arg,
					check_output_full_fn is_output_full)
{
	struct rb_node *node;
	int first_row = row, offset = level * LEVEL_OFFSET_STEP;
	bool need_percent;
	u64 percent_total = total;

	if (callchain_param.mode == CHAIN_GRAPH_REL)
		percent_total = parent_total;

	node = rb_first(root);
	need_percent = check_percent_display(node, parent_total);

	while (node) {
		struct callchain_node *child = rb_entry(node, struct callchain_node, rb_node);
		struct rb_node *next = rb_next(node);
		struct callchain_list *chain;
		char folded_sign = ' ';
		int first = true;
		int extra_offset = 0;

		list_for_each_entry(chain, &child->val, list) {
			bool was_first = first;

			if (first)
				first = false;
			else if (need_percent)
				extra_offset = LEVEL_OFFSET_STEP;

			folded_sign = callchain_list__folded(chain);

			row += hist_browser__show_callchain_list(browser, child,
							chain, row, percent_total,
							was_first && need_percent,
							offset + extra_offset,
							print, arg);

			if (is_output_full(browser, row))
				goto out;

			if (folded_sign == '+')
				break;
		}

		if (folded_sign == '-') {
			const int new_level = level + (extra_offset ? 2 : 1);

			row += hist_browser__show_callchain_graph(browser, &child->rb_root,
							    new_level, row, total,
							    child->children_hit,
							    print, arg, is_output_full);
		}
		if (is_output_full(browser, row))
			break;
		node = next;
	}
out:
	return row - first_row;
}

static int hist_browser__show_callchain(struct hist_browser *browser,
					struct hist_entry *entry, int level,
					unsigned short row,
					print_callchain_entry_fn print,
					struct callchain_print_arg *arg,
					check_output_full_fn is_output_full)
{
	u64 total = hists__total_period(entry->hists);
	u64 parent_total;
	int printed;

	if (symbol_conf.cumulate_callchain)
		parent_total = entry->stat_acc->period;
	else
		parent_total = entry->stat.period;

	if (callchain_param.mode == CHAIN_FLAT) {
		printed = hist_browser__show_callchain_flat(browser,
						&entry->sorted_chain, row,
						total, parent_total, print, arg,
						is_output_full);
	} else if (callchain_param.mode == CHAIN_FOLDED) {
		printed = hist_browser__show_callchain_folded(browser,
						&entry->sorted_chain, row,
						total, parent_total, print, arg,
						is_output_full);
	} else {
		printed = hist_browser__show_callchain_graph(browser,
						&entry->sorted_chain, level, row,
						total, parent_total, print, arg,
						is_output_full);
	}

	if (arg->is_current_entry)
		browser->he_selection = entry;

	return printed;
}

struct hpp_arg {
	struct ui_browser *b;
	char folded_sign;
	bool current_entry;
};

int __hpp__slsmg_color_printf(struct perf_hpp *hpp, const char *fmt, ...)
{
	struct hpp_arg *arg = hpp->ptr;
	int ret, len;
	va_list args;
	double percent;

	va_start(args, fmt);
	len = va_arg(args, int);
	percent = va_arg(args, double);
	va_end(args);

	ui_browser__set_percent_color(arg->b, percent, arg->current_entry);

	ret = scnprintf(hpp->buf, hpp->size, fmt, len, percent);
	ui_browser__printf(arg->b, "%s", hpp->buf);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return ret;
}

<<<<<<< HEAD
#define __HPP_COLOR_PERCENT_FN(_type, _field, _cb)			\
=======
#define __HPP_COLOR_PERCENT_FN(_type, _field)				\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static u64 __hpp_get_##_field(struct hist_entry *he)			\
{									\
	return he->stat._field;						\
}									\
									\
<<<<<<< HEAD
static int hist_browser__hpp_color_##_type(struct perf_hpp *hpp,	\
					   struct hist_entry *he)	\
{									\
	return __hpp__color_fmt(hpp, he, __hpp_get_##_field, _cb);	\
}

__HPP_COLOR_PERCENT_FN(overhead, period, __hpp__color_callchain)
__HPP_COLOR_PERCENT_FN(overhead_sys, period_sys, NULL)
__HPP_COLOR_PERCENT_FN(overhead_us, period_us, NULL)
__HPP_COLOR_PERCENT_FN(overhead_guest_sys, period_guest_sys, NULL)
__HPP_COLOR_PERCENT_FN(overhead_guest_us, period_guest_us, NULL)

#undef __HPP_COLOR_PERCENT_FN

void hist_browser__init_hpp(void)
{
	perf_hpp__column_enable(PERF_HPP__OVERHEAD);

	perf_hpp__init();

=======
static int								\
hist_browser__hpp_color_##_type(struct perf_hpp_fmt *fmt,		\
				struct perf_hpp *hpp,			\
				struct hist_entry *he)			\
{									\
	return hpp__fmt(fmt, hpp, he, __hpp_get_##_field, " %*.2f%%",	\
			__hpp__slsmg_color_printf, true);		\
}

#define __HPP_COLOR_ACC_PERCENT_FN(_type, _field)			\
static u64 __hpp_get_acc_##_field(struct hist_entry *he)		\
{									\
	return he->stat_acc->_field;					\
}									\
									\
static int								\
hist_browser__hpp_color_##_type(struct perf_hpp_fmt *fmt,		\
				struct perf_hpp *hpp,			\
				struct hist_entry *he)			\
{									\
	if (!symbol_conf.cumulate_callchain) {				\
		struct hpp_arg *arg = hpp->ptr;				\
		int len = fmt->user_len ?: fmt->len;			\
		int ret = scnprintf(hpp->buf, hpp->size,		\
				    "%*s", len, "N/A");			\
		ui_browser__printf(arg->b, "%s", hpp->buf);		\
									\
		return ret;						\
	}								\
	return hpp__fmt(fmt, hpp, he, __hpp_get_acc_##_field,		\
			" %*.2f%%", __hpp__slsmg_color_printf, true);	\
}

__HPP_COLOR_PERCENT_FN(overhead, period)
__HPP_COLOR_PERCENT_FN(overhead_sys, period_sys)
__HPP_COLOR_PERCENT_FN(overhead_us, period_us)
__HPP_COLOR_PERCENT_FN(overhead_guest_sys, period_guest_sys)
__HPP_COLOR_PERCENT_FN(overhead_guest_us, period_guest_us)
__HPP_COLOR_ACC_PERCENT_FN(overhead_acc, period)

#undef __HPP_COLOR_PERCENT_FN
#undef __HPP_COLOR_ACC_PERCENT_FN

void hist_browser__init_hpp(void)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	perf_hpp__format[PERF_HPP__OVERHEAD].color =
				hist_browser__hpp_color_overhead;
	perf_hpp__format[PERF_HPP__OVERHEAD_SYS].color =
				hist_browser__hpp_color_overhead_sys;
	perf_hpp__format[PERF_HPP__OVERHEAD_US].color =
				hist_browser__hpp_color_overhead_us;
	perf_hpp__format[PERF_HPP__OVERHEAD_GUEST_SYS].color =
				hist_browser__hpp_color_overhead_guest_sys;
	perf_hpp__format[PERF_HPP__OVERHEAD_GUEST_US].color =
				hist_browser__hpp_color_overhead_guest_us;
<<<<<<< HEAD
=======
	perf_hpp__format[PERF_HPP__OVERHEAD_ACC].color =
				hist_browser__hpp_color_overhead_acc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int hist_browser__show_entry(struct hist_browser *browser,
				    struct hist_entry *entry,
				    unsigned short row)
{
<<<<<<< HEAD
	char s[256];
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int printed = 0;
	int width = browser->b.width;
	char folded_sign = ' ';
	bool current_entry = ui_browser__is_current_entry(&browser->b, row);
	off_t row_offset = entry->row_offset;
	bool first = true;
	struct perf_hpp_fmt *fmt;

	if (current_entry) {
		browser->he_selection = entry;
		browser->selection = &entry->ms;
	}

	if (symbol_conf.use_callchain) {
		hist_entry__init_have_children(entry);
		folded_sign = hist_entry__folded(entry);
	}

	if (row_offset == 0) {
		struct hpp_arg arg = {
<<<<<<< HEAD
			.b 		= &browser->b,
			.folded_sign	= folded_sign,
			.current_entry	= current_entry,
		};
		struct perf_hpp hpp = {
			.buf		= s,
			.size		= sizeof(s),
			.ptr		= &arg,
		};

		ui_browser__gotorc(&browser->b, row, 0);

		perf_hpp__for_each_format(fmt) {
			if (!first) {
				slsmg_printf("  ");
				width -= 2;
			}
			first = false;

			if (fmt->color) {
				width -= fmt->color(&hpp, entry);
			} else {
				width -= fmt->entry(&hpp, entry);
				slsmg_printf("%s", s);
			}
=======
			.b		= &browser->b,
			.folded_sign	= folded_sign,
			.current_entry	= current_entry,
		};
		int column = 0;

		hist_browser__gotorc(browser, row, 0);

		hists__for_each_format(browser->hists, fmt) {
			char s[2048];
			struct perf_hpp hpp = {
				.buf	= s,
				.size	= sizeof(s),
				.ptr	= &arg,
			};

			if (perf_hpp__should_skip(fmt, entry->hists) ||
			    column++ < browser->b.horiz_scroll)
				continue;

			if (current_entry && browser->b.navkeypressed) {
				ui_browser__set_color(&browser->b,
						      HE_COLORSET_SELECTED);
			} else {
				ui_browser__set_color(&browser->b,
						      HE_COLORSET_NORMAL);
			}

			if (first) {
				if (symbol_conf.use_callchain) {
					ui_browser__printf(&browser->b, "%c ", folded_sign);
					width -= 2;
				}
				first = false;
			} else {
				ui_browser__printf(&browser->b, "  ");
				width -= 2;
			}

			if (fmt->color) {
				int ret = fmt->color(fmt, &hpp, entry);
				hist_entry__snprintf_alignment(entry, &hpp, fmt, ret);
				/*
				 * fmt->color() already used ui_browser to
				 * print the non alignment bits, skip it (+ret):
				 */
				ui_browser__printf(&browser->b, "%s", s + ret);
			} else {
				hist_entry__snprintf_alignment(entry, &hpp, fmt, fmt->entry(fmt, &hpp, entry));
				ui_browser__printf(&browser->b, "%s", s);
			}
			width -= hpp.buf - s;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* The scroll bar isn't being used */
		if (!browser->b.navkeypressed)
			width += 1;

<<<<<<< HEAD
		hist_entry__sort_snprintf(entry, s, sizeof(s), browser->hists);
		slsmg_write_nstring(s, width);
=======
		ui_browser__write_nstring(&browser->b, "", width);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		++row;
		++printed;
	} else
		--row_offset;

<<<<<<< HEAD
	if (folded_sign == '-' && row != browser->b.height) {
		printed += hist_browser__show_callchain(browser, &entry->sorted_chain,
							1, row, &row_offset,
							&current_entry);
		if (current_entry)
			browser->he_selection = entry;
=======
	if (folded_sign == '-' && row != browser->b.rows) {
		struct callchain_print_arg arg = {
			.row_offset = row_offset,
			.is_current_entry = current_entry,
		};

		printed += hist_browser__show_callchain(browser, entry, 1, row,
					hist_browser__show_callchain_entry, &arg,
					hist_browser__check_output_full);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return printed;
}

<<<<<<< HEAD
=======
static int hist_browser__show_hierarchy_entry(struct hist_browser *browser,
					      struct hist_entry *entry,
					      unsigned short row,
					      int level)
{
	int printed = 0;
	int width = browser->b.width;
	char folded_sign = ' ';
	bool current_entry = ui_browser__is_current_entry(&browser->b, row);
	off_t row_offset = entry->row_offset;
	bool first = true;
	struct perf_hpp_fmt *fmt;
	struct perf_hpp_list_node *fmt_node;
	struct hpp_arg arg = {
		.b		= &browser->b,
		.current_entry	= current_entry,
	};
	int column = 0;
	int hierarchy_indent = (entry->hists->nr_hpp_node - 2) * HIERARCHY_INDENT;

	if (current_entry) {
		browser->he_selection = entry;
		browser->selection = &entry->ms;
	}

	hist_entry__init_have_children(entry);
	folded_sign = hist_entry__folded(entry);
	arg.folded_sign = folded_sign;

	if (entry->leaf && row_offset) {
		row_offset--;
		goto show_callchain;
	}

	hist_browser__gotorc(browser, row, 0);

	if (current_entry && browser->b.navkeypressed)
		ui_browser__set_color(&browser->b, HE_COLORSET_SELECTED);
	else
		ui_browser__set_color(&browser->b, HE_COLORSET_NORMAL);

	ui_browser__write_nstring(&browser->b, "", level * HIERARCHY_INDENT);
	width -= level * HIERARCHY_INDENT;

	/* the first hpp_list_node is for overhead columns */
	fmt_node = list_first_entry(&entry->hists->hpp_formats,
				    struct perf_hpp_list_node, list);
	perf_hpp_list__for_each_format(&fmt_node->hpp, fmt) {
		char s[2048];
		struct perf_hpp hpp = {
			.buf		= s,
			.size		= sizeof(s),
			.ptr		= &arg,
		};

		if (perf_hpp__should_skip(fmt, entry->hists) ||
		    column++ < browser->b.horiz_scroll)
			continue;

		if (current_entry && browser->b.navkeypressed) {
			ui_browser__set_color(&browser->b,
					      HE_COLORSET_SELECTED);
		} else {
			ui_browser__set_color(&browser->b,
					      HE_COLORSET_NORMAL);
		}

		if (first) {
			ui_browser__printf(&browser->b, "%c ", folded_sign);
			width -= 2;
			first = false;
		} else {
			ui_browser__printf(&browser->b, "  ");
			width -= 2;
		}

		if (fmt->color) {
			int ret = fmt->color(fmt, &hpp, entry);
			hist_entry__snprintf_alignment(entry, &hpp, fmt, ret);
			/*
			 * fmt->color() already used ui_browser to
			 * print the non alignment bits, skip it (+ret):
			 */
			ui_browser__printf(&browser->b, "%s", s + ret);
		} else {
			int ret = fmt->entry(fmt, &hpp, entry);
			hist_entry__snprintf_alignment(entry, &hpp, fmt, ret);
			ui_browser__printf(&browser->b, "%s", s);
		}
		width -= hpp.buf - s;
	}

	if (!first) {
		ui_browser__write_nstring(&browser->b, "", hierarchy_indent);
		width -= hierarchy_indent;
	}

	if (column >= browser->b.horiz_scroll) {
		char s[2048];
		struct perf_hpp hpp = {
			.buf		= s,
			.size		= sizeof(s),
			.ptr		= &arg,
		};

		if (current_entry && browser->b.navkeypressed) {
			ui_browser__set_color(&browser->b,
					      HE_COLORSET_SELECTED);
		} else {
			ui_browser__set_color(&browser->b,
					      HE_COLORSET_NORMAL);
		}

		perf_hpp_list__for_each_format(entry->hpp_list, fmt) {
			if (first) {
				ui_browser__printf(&browser->b, "%c ", folded_sign);
				first = false;
			} else {
				ui_browser__write_nstring(&browser->b, "", 2);
			}

			width -= 2;

			/*
			 * No need to call hist_entry__snprintf_alignment()
			 * since this fmt is always the last column in the
			 * hierarchy mode.
			 */
			if (fmt->color) {
				width -= fmt->color(fmt, &hpp, entry);
			} else {
				int i = 0;

				width -= fmt->entry(fmt, &hpp, entry);
				ui_browser__printf(&browser->b, "%s", ltrim(s));

				while (isspace(s[i++]))
					width++;
			}
		}
	}

	/* The scroll bar isn't being used */
	if (!browser->b.navkeypressed)
		width += 1;

	ui_browser__write_nstring(&browser->b, "", width);

	++row;
	++printed;

show_callchain:
	if (entry->leaf && folded_sign == '-' && row != browser->b.rows) {
		struct callchain_print_arg carg = {
			.row_offset = row_offset,
		};

		printed += hist_browser__show_callchain(browser, entry,
					level + 1, row,
					hist_browser__show_callchain_entry, &carg,
					hist_browser__check_output_full);
	}

	return printed;
}

static int hist_browser__show_no_entry(struct hist_browser *browser,
				       unsigned short row, int level)
{
	int width = browser->b.width;
	bool current_entry = ui_browser__is_current_entry(&browser->b, row);
	bool first = true;
	int column = 0;
	int ret;
	struct perf_hpp_fmt *fmt;
	struct perf_hpp_list_node *fmt_node;
	int indent = browser->hists->nr_hpp_node - 2;

	if (current_entry) {
		browser->he_selection = NULL;
		browser->selection = NULL;
	}

	hist_browser__gotorc(browser, row, 0);

	if (current_entry && browser->b.navkeypressed)
		ui_browser__set_color(&browser->b, HE_COLORSET_SELECTED);
	else
		ui_browser__set_color(&browser->b, HE_COLORSET_NORMAL);

	ui_browser__write_nstring(&browser->b, "", level * HIERARCHY_INDENT);
	width -= level * HIERARCHY_INDENT;

	/* the first hpp_list_node is for overhead columns */
	fmt_node = list_first_entry(&browser->hists->hpp_formats,
				    struct perf_hpp_list_node, list);
	perf_hpp_list__for_each_format(&fmt_node->hpp, fmt) {
		if (perf_hpp__should_skip(fmt, browser->hists) ||
		    column++ < browser->b.horiz_scroll)
			continue;

		ret = fmt->width(fmt, NULL, browser->hists);

		if (first) {
			/* for folded sign */
			first = false;
			ret++;
		} else {
			/* space between columns */
			ret += 2;
		}

		ui_browser__write_nstring(&browser->b, "", ret);
		width -= ret;
	}

	ui_browser__write_nstring(&browser->b, "", indent * HIERARCHY_INDENT);
	width -= indent * HIERARCHY_INDENT;

	if (column >= browser->b.horiz_scroll) {
		char buf[32];

		ret = snprintf(buf, sizeof(buf), "no entry >= %.2f%%", browser->min_pcnt);
		ui_browser__printf(&browser->b, "  %s", buf);
		width -= ret + 2;
	}

	/* The scroll bar isn't being used */
	if (!browser->b.navkeypressed)
		width += 1;

	ui_browser__write_nstring(&browser->b, "", width);
	return 1;
}

static int advance_hpp_check(struct perf_hpp *hpp, int inc)
{
	advance_hpp(hpp, inc);
	return hpp->size <= 0;
}

static int
hists_browser__scnprintf_headers(struct hist_browser *browser, char *buf,
				 size_t size, int line)
{
	struct hists *hists = browser->hists;
	struct perf_hpp dummy_hpp = {
		.buf    = buf,
		.size   = size,
	};
	struct perf_hpp_fmt *fmt;
	size_t ret = 0;
	int column = 0;
	int span = 0;

	if (symbol_conf.use_callchain) {
		ret = scnprintf(buf, size, "  ");
		if (advance_hpp_check(&dummy_hpp, ret))
			return ret;
	}

	hists__for_each_format(browser->hists, fmt) {
		if (perf_hpp__should_skip(fmt, hists)  || column++ < browser->b.horiz_scroll)
			continue;

		ret = fmt->header(fmt, &dummy_hpp, hists, line, &span);
		if (advance_hpp_check(&dummy_hpp, ret))
			break;

		if (span)
			continue;

		ret = scnprintf(dummy_hpp.buf, dummy_hpp.size, "  ");
		if (advance_hpp_check(&dummy_hpp, ret))
			break;
	}

	return ret;
}

static int hists_browser__scnprintf_hierarchy_headers(struct hist_browser *browser, char *buf, size_t size)
{
	struct hists *hists = browser->hists;
	struct perf_hpp dummy_hpp = {
		.buf    = buf,
		.size   = size,
	};
	struct perf_hpp_fmt *fmt;
	struct perf_hpp_list_node *fmt_node;
	size_t ret = 0;
	int column = 0;
	int indent = hists->nr_hpp_node - 2;
	bool first_node, first_col;

	ret = scnprintf(buf, size, "  ");
	if (advance_hpp_check(&dummy_hpp, ret))
		return ret;

	first_node = true;
	/* the first hpp_list_node is for overhead columns */
	fmt_node = list_first_entry(&hists->hpp_formats,
				    struct perf_hpp_list_node, list);
	perf_hpp_list__for_each_format(&fmt_node->hpp, fmt) {
		if (column++ < browser->b.horiz_scroll)
			continue;

		ret = fmt->header(fmt, &dummy_hpp, hists, 0, NULL);
		if (advance_hpp_check(&dummy_hpp, ret))
			break;

		ret = scnprintf(dummy_hpp.buf, dummy_hpp.size, "  ");
		if (advance_hpp_check(&dummy_hpp, ret))
			break;

		first_node = false;
	}

	if (!first_node) {
		ret = scnprintf(dummy_hpp.buf, dummy_hpp.size, "%*s",
				indent * HIERARCHY_INDENT, "");
		if (advance_hpp_check(&dummy_hpp, ret))
			return ret;
	}

	first_node = true;
	list_for_each_entry_continue(fmt_node, &hists->hpp_formats, list) {
		if (!first_node) {
			ret = scnprintf(dummy_hpp.buf, dummy_hpp.size, " / ");
			if (advance_hpp_check(&dummy_hpp, ret))
				break;
		}
		first_node = false;

		first_col = true;
		perf_hpp_list__for_each_format(&fmt_node->hpp, fmt) {
			char *start;

			if (perf_hpp__should_skip(fmt, hists))
				continue;

			if (!first_col) {
				ret = scnprintf(dummy_hpp.buf, dummy_hpp.size, "+");
				if (advance_hpp_check(&dummy_hpp, ret))
					break;
			}
			first_col = false;

			ret = fmt->header(fmt, &dummy_hpp, hists, 0, NULL);
			dummy_hpp.buf[ret] = '\0';

			start = trim(dummy_hpp.buf);
			ret = strlen(start);

			if (start != dummy_hpp.buf)
				memmove(dummy_hpp.buf, start, ret + 1);

			if (advance_hpp_check(&dummy_hpp, ret))
				break;
		}
	}

	return ret;
}

static void hists_browser__hierarchy_headers(struct hist_browser *browser)
{
	char headers[1024];

	hists_browser__scnprintf_hierarchy_headers(browser, headers,
						   sizeof(headers));

	ui_browser__gotorc(&browser->b, 0, 0);
	ui_browser__set_color(&browser->b, HE_COLORSET_ROOT);
	ui_browser__write_nstring(&browser->b, headers, browser->b.width + 1);
}

static void hists_browser__headers(struct hist_browser *browser)
{
	struct hists *hists = browser->hists;
	struct perf_hpp_list *hpp_list = hists->hpp_list;

	int line;

	for (line = 0; line < hpp_list->nr_header_lines; line++) {
		char headers[1024];

		hists_browser__scnprintf_headers(browser, headers,
						 sizeof(headers), line);

		ui_browser__gotorc(&browser->b, line, 0);
		ui_browser__set_color(&browser->b, HE_COLORSET_ROOT);
		ui_browser__write_nstring(&browser->b, headers, browser->b.width + 1);
	}
}

static void hist_browser__show_headers(struct hist_browser *browser)
{
	if (symbol_conf.report_hierarchy)
		hists_browser__hierarchy_headers(browser);
	else
		hists_browser__headers(browser);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void ui_browser__hists_init_top(struct ui_browser *browser)
{
	if (browser->top == NULL) {
		struct hist_browser *hb;

		hb = container_of(browser, struct hist_browser, b);
		browser->top = rb_first(&hb->hists->entries);
	}
}

static unsigned int hist_browser__refresh(struct ui_browser *browser)
{
	unsigned row = 0;
<<<<<<< HEAD
	struct rb_node *nd;
	struct hist_browser *hb = container_of(browser, struct hist_browser, b);

	ui_browser__hists_init_top(browser);

	for (nd = browser->top; nd; nd = rb_next(nd)) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);

		if (h->filtered)
			continue;

		row += hist_browser__show_entry(hb, h, row);
		if (row == browser->height)
			break;
	}

	return row;
}

static struct rb_node *hists__filter_entries(struct rb_node *nd)
{
	while (nd != NULL) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);
		if (!h->filtered)
			return nd;

		nd = rb_next(nd);
=======
	u16 header_offset = 0;
	struct rb_node *nd;
	struct hist_browser *hb = container_of(browser, struct hist_browser, b);
	struct hists *hists = hb->hists;

	if (hb->show_headers) {
		struct perf_hpp_list *hpp_list = hists->hpp_list;

		hist_browser__show_headers(hb);
		header_offset = hpp_list->nr_header_lines;
	}

	ui_browser__hists_init_top(browser);
	hb->he_selection = NULL;
	hb->selection = NULL;

	for (nd = browser->top; nd; nd = rb_hierarchy_next(nd)) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);
		float percent;

		if (h->filtered) {
			/* let it move to sibling */
			h->unfolded = false;
			continue;
		}

		percent = hist_entry__get_percent_limit(h);
		if (percent < hb->min_pcnt)
			continue;

		if (symbol_conf.report_hierarchy) {
			row += hist_browser__show_hierarchy_entry(hb, h, row,
								  h->depth);
			if (row == browser->rows)
				break;

			if (h->has_no_entry) {
				hist_browser__show_no_entry(hb, row, h->depth + 1);
				row++;
			}
		} else {
			row += hist_browser__show_entry(hb, h, row);
		}

		if (row == browser->rows)
			break;
	}

	return row + header_offset;
}

static struct rb_node *hists__filter_entries(struct rb_node *nd,
					     float min_pcnt)
{
	while (nd != NULL) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);
		float percent = hist_entry__get_percent_limit(h);

		if (!h->filtered && percent >= min_pcnt)
			return nd;

		/*
		 * If it's filtered, its all children also were filtered.
		 * So move to sibling node.
		 */
		if (rb_next(nd))
			nd = rb_next(nd);
		else
			nd = rb_hierarchy_next(nd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return NULL;
}

<<<<<<< HEAD
static struct rb_node *hists__filter_prev_entries(struct rb_node *nd)
{
	while (nd != NULL) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);
		if (!h->filtered)
			return nd;

		nd = rb_prev(nd);
=======
static struct rb_node *hists__filter_prev_entries(struct rb_node *nd,
						  float min_pcnt)
{
	while (nd != NULL) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);
		float percent = hist_entry__get_percent_limit(h);

		if (!h->filtered && percent >= min_pcnt)
			return nd;

		nd = rb_hierarchy_prev(nd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return NULL;
}

static void ui_browser__hists_seek(struct ui_browser *browser,
				   off_t offset, int whence)
{
	struct hist_entry *h;
	struct rb_node *nd;
	bool first = true;
<<<<<<< HEAD
=======
	struct hist_browser *hb;

	hb = container_of(browser, struct hist_browser, b);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (browser->nr_entries == 0)
		return;

	ui_browser__hists_init_top(browser);

	switch (whence) {
	case SEEK_SET:
<<<<<<< HEAD
		nd = hists__filter_entries(rb_first(browser->entries));
=======
		nd = hists__filter_entries(rb_first(browser->entries),
					   hb->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case SEEK_CUR:
		nd = browser->top;
		goto do_offset;
	case SEEK_END:
<<<<<<< HEAD
		nd = hists__filter_prev_entries(rb_last(browser->entries));
=======
		nd = rb_hierarchy_last(rb_last(browser->entries));
		nd = hists__filter_prev_entries(nd, hb->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		first = false;
		break;
	default:
		return;
	}

	/*
	 * Moves not relative to the first visible entry invalidates its
	 * row_offset:
	 */
	h = rb_entry(browser->top, struct hist_entry, rb_node);
	h->row_offset = 0;

	/*
	 * Here we have to check if nd is expanded (+), if it is we can't go
	 * the next top level hist_entry, instead we must compute an offset of
	 * what _not_ to show and not change the first visible entry.
	 *
	 * This offset increments when we are going from top to bottom and
	 * decreases when we're going from bottom to top.
	 *
	 * As we don't have backpointers to the top level in the callchains
	 * structure, we need to always print the whole hist_entry callchain,
	 * skipping the first ones that are before the first visible entry
	 * and stop when we printed enough lines to fill the screen.
	 */
do_offset:
<<<<<<< HEAD
	if (offset > 0) {
		do {
			h = rb_entry(nd, struct hist_entry, rb_node);
			if (h->ms.unfolded) {
=======
	if (!nd)
		return;

	if (offset > 0) {
		do {
			h = rb_entry(nd, struct hist_entry, rb_node);
			if (h->unfolded && h->leaf) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				u16 remaining = h->nr_rows - h->row_offset;
				if (offset > remaining) {
					offset -= remaining;
					h->row_offset = 0;
				} else {
					h->row_offset += offset;
					offset = 0;
					browser->top = nd;
					break;
				}
			}
<<<<<<< HEAD
			nd = hists__filter_entries(rb_next(nd));
=======
			nd = hists__filter_entries(rb_hierarchy_next(nd),
						   hb->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (nd == NULL)
				break;
			--offset;
			browser->top = nd;
		} while (offset != 0);
	} else if (offset < 0) {
		while (1) {
			h = rb_entry(nd, struct hist_entry, rb_node);
<<<<<<< HEAD
			if (h->ms.unfolded) {
=======
			if (h->unfolded && h->leaf) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (first) {
					if (-offset > h->row_offset) {
						offset += h->row_offset;
						h->row_offset = 0;
					} else {
						h->row_offset += offset;
						offset = 0;
						browser->top = nd;
						break;
					}
				} else {
					if (-offset > h->nr_rows) {
						offset += h->nr_rows;
						h->row_offset = 0;
					} else {
						h->row_offset = h->nr_rows + offset;
						offset = 0;
						browser->top = nd;
						break;
					}
				}
			}

<<<<<<< HEAD
			nd = hists__filter_prev_entries(rb_prev(nd));
=======
			nd = hists__filter_prev_entries(rb_hierarchy_prev(nd),
							hb->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (nd == NULL)
				break;
			++offset;
			browser->top = nd;
			if (offset == 0) {
				/*
				 * Last unfiltered hist_entry, check if it is
				 * unfolded, if it is then we should have
				 * row_offset at its last entry.
				 */
				h = rb_entry(nd, struct hist_entry, rb_node);
<<<<<<< HEAD
				if (h->ms.unfolded)
=======
				if (h->unfolded && h->leaf)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					h->row_offset = h->nr_rows;
				break;
			}
			first = false;
		}
	} else {
		browser->top = nd;
		h = rb_entry(nd, struct hist_entry, rb_node);
		h->row_offset = 0;
	}
}

<<<<<<< HEAD
static int hist_browser__fprintf_callchain_node_rb_tree(struct hist_browser *browser,
							struct callchain_node *chain_node,
							u64 total, int level,
							FILE *fp)
{
	struct rb_node *node;
	int offset = level * LEVEL_OFFSET_STEP;
	u64 new_total, remaining;
	int printed = 0;

	if (callchain_param.mode == CHAIN_GRAPH_REL)
		new_total = chain_node->children_hit;
	else
		new_total = total;

	remaining = new_total;
	node = rb_first(&chain_node->rb_root);
	while (node) {
		struct callchain_node *child = rb_entry(node, struct callchain_node, rb_node);
		struct rb_node *next = rb_next(node);
		u64 cumul = callchain_cumul_hits(child);
		struct callchain_list *chain;
		char folded_sign = ' ';
		int first = true;
		int extra_offset = 0;

		remaining -= cumul;

		list_for_each_entry(chain, &child->val, list) {
			char bf[1024], *alloc_str;
			const char *str;
			bool was_first = first;

			if (first)
				first = false;
			else
				extra_offset = LEVEL_OFFSET_STEP;

			folded_sign = callchain_list__folded(chain);

			alloc_str = NULL;
			str = callchain_list__sym_name(chain, bf, sizeof(bf),
						       browser->show_dso);
			if (was_first) {
				double percent = cumul * 100.0 / new_total;

				if (asprintf(&alloc_str, "%2.2f%% %s", percent, str) < 0)
					str = "Not enough memory!";
				else
					str = alloc_str;
			}

			printed += fprintf(fp, "%*s%c %s\n", offset + extra_offset, " ", folded_sign, str);
			free(alloc_str);
			if (folded_sign == '+')
				break;
		}

		if (folded_sign == '-') {
			const int new_level = level + (extra_offset ? 2 : 1);
			printed += hist_browser__fprintf_callchain_node_rb_tree(browser, child, new_total,
										new_level, fp);
		}

		node = next;
	}

	return printed;
}

static int hist_browser__fprintf_callchain_node(struct hist_browser *browser,
						struct callchain_node *node,
						int level, FILE *fp)
{
	struct callchain_list *chain;
	int offset = level * LEVEL_OFFSET_STEP;
	char folded_sign = ' ';
	int printed = 0;

	list_for_each_entry(chain, &node->val, list) {
		char bf[1024], *s;

		folded_sign = callchain_list__folded(chain);
		s = callchain_list__sym_name(chain, bf, sizeof(bf), browser->show_dso);
		printed += fprintf(fp, "%*s%c %s\n", offset, " ", folded_sign, s);
	}

	if (folded_sign == '-')
		printed += hist_browser__fprintf_callchain_node_rb_tree(browser, node,
									browser->hists->stats.total_period,
									level + 1,  fp);
	return printed;
}

static int hist_browser__fprintf_callchain(struct hist_browser *browser,
					   struct rb_root *chain, int level, FILE *fp)
{
	struct rb_node *nd;
	int printed = 0;

	for (nd = rb_first(chain); nd; nd = rb_next(nd)) {
		struct callchain_node *node = rb_entry(nd, struct callchain_node, rb_node);

		printed += hist_browser__fprintf_callchain_node(browser, node, level, fp);
	}
=======
static int hist_browser__fprintf_callchain(struct hist_browser *browser,
					   struct hist_entry *he, FILE *fp,
					   int level)
{
	struct callchain_print_arg arg  = {
		.fp = fp,
	};

	hist_browser__show_callchain(browser, he, level, 0,
				     hist_browser__fprintf_callchain_entry, &arg,
				     hist_browser__check_dump_full);
	return arg.printed;
}

static int hist_browser__fprintf_entry(struct hist_browser *browser,
				       struct hist_entry *he, FILE *fp)
{
	char s[8192];
	int printed = 0;
	char folded_sign = ' ';
	struct perf_hpp hpp = {
		.buf = s,
		.size = sizeof(s),
	};
	struct perf_hpp_fmt *fmt;
	bool first = true;
	int ret;

	if (symbol_conf.use_callchain) {
		folded_sign = hist_entry__folded(he);
		printed += fprintf(fp, "%c ", folded_sign);
	}

	hists__for_each_format(browser->hists, fmt) {
		if (perf_hpp__should_skip(fmt, he->hists))
			continue;

		if (!first) {
			ret = scnprintf(hpp.buf, hpp.size, "  ");
			advance_hpp(&hpp, ret);
		} else
			first = false;

		ret = fmt->entry(fmt, &hpp, he);
		ret = hist_entry__snprintf_alignment(he, &hpp, fmt, ret);
		advance_hpp(&hpp, ret);
	}
	printed += fprintf(fp, "%s\n", s);

	if (folded_sign == '-')
		printed += hist_browser__fprintf_callchain(browser, he, fp, 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return printed;
}

<<<<<<< HEAD
static int hist_browser__fprintf_entry(struct hist_browser *browser,
				       struct hist_entry *he, FILE *fp)
{
	char s[8192];
	double percent;
	int printed = 0;
	char folded_sign = ' ';

	if (symbol_conf.use_callchain)
		folded_sign = hist_entry__folded(he);

	hist_entry__sort_snprintf(he, s, sizeof(s), browser->hists);
	percent = (he->stat.period * 100.0) / browser->hists->stats.total_period;

	if (symbol_conf.use_callchain)
		printed += fprintf(fp, "%c ", folded_sign);

	printed += fprintf(fp, " %5.2f%%", percent);

	if (symbol_conf.show_nr_samples)
		printed += fprintf(fp, " %11u", he->stat.nr_events);

	if (symbol_conf.show_total_period)
		printed += fprintf(fp, " %12" PRIu64, he->stat.period);

	printed += fprintf(fp, "%s\n", rtrim(s));

	if (folded_sign == '-')
		printed += hist_browser__fprintf_callchain(browser, &he->sorted_chain, 1, fp);
=======

static int hist_browser__fprintf_hierarchy_entry(struct hist_browser *browser,
						 struct hist_entry *he,
						 FILE *fp, int level)
{
	char s[8192];
	int printed = 0;
	char folded_sign = ' ';
	struct perf_hpp hpp = {
		.buf = s,
		.size = sizeof(s),
	};
	struct perf_hpp_fmt *fmt;
	struct perf_hpp_list_node *fmt_node;
	bool first = true;
	int ret;
	int hierarchy_indent = (he->hists->nr_hpp_node - 2) * HIERARCHY_INDENT;

	printed = fprintf(fp, "%*s", level * HIERARCHY_INDENT, "");

	folded_sign = hist_entry__folded(he);
	printed += fprintf(fp, "%c", folded_sign);

	/* the first hpp_list_node is for overhead columns */
	fmt_node = list_first_entry(&he->hists->hpp_formats,
				    struct perf_hpp_list_node, list);
	perf_hpp_list__for_each_format(&fmt_node->hpp, fmt) {
		if (!first) {
			ret = scnprintf(hpp.buf, hpp.size, "  ");
			advance_hpp(&hpp, ret);
		} else
			first = false;

		ret = fmt->entry(fmt, &hpp, he);
		advance_hpp(&hpp, ret);
	}

	ret = scnprintf(hpp.buf, hpp.size, "%*s", hierarchy_indent, "");
	advance_hpp(&hpp, ret);

	perf_hpp_list__for_each_format(he->hpp_list, fmt) {
		ret = scnprintf(hpp.buf, hpp.size, "  ");
		advance_hpp(&hpp, ret);

		ret = fmt->entry(fmt, &hpp, he);
		advance_hpp(&hpp, ret);
	}

	printed += fprintf(fp, "%s\n", rtrim(s));

	if (he->leaf && folded_sign == '-') {
		printed += hist_browser__fprintf_callchain(browser, he, fp,
							   he->depth + 1);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return printed;
}

static int hist_browser__fprintf(struct hist_browser *browser, FILE *fp)
{
<<<<<<< HEAD
	struct rb_node *nd = hists__filter_entries(rb_first(browser->b.entries));
=======
	struct rb_node *nd = hists__filter_entries(rb_first(browser->b.entries),
						   browser->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int printed = 0;

	while (nd) {
		struct hist_entry *h = rb_entry(nd, struct hist_entry, rb_node);

<<<<<<< HEAD
		printed += hist_browser__fprintf_entry(browser, h, fp);
		nd = hists__filter_entries(rb_next(nd));
=======
		if (symbol_conf.report_hierarchy) {
			printed += hist_browser__fprintf_hierarchy_entry(browser,
									 h, fp,
									 h->depth);
		} else {
			printed += hist_browser__fprintf_entry(browser, h, fp);
		}

		nd = hists__filter_entries(rb_hierarchy_next(nd),
					   browser->min_pcnt);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return printed;
}

static int hist_browser__dump(struct hist_browser *browser)
{
	char filename[64];
	FILE *fp;

	while (1) {
		scnprintf(filename, sizeof(filename), "perf.hist.%d", browser->print_seq);
		if (access(filename, F_OK))
			break;
		/*
 		 * XXX: Just an arbitrary lazy upper limit
 		 */
		if (++browser->print_seq == 8192) {
			ui_helpline__fpush("Too many perf.hist.N files, nothing written!");
			return -1;
		}
	}

	fp = fopen(filename, "w");
	if (fp == NULL) {
		char bf[64];
<<<<<<< HEAD
		const char *err = strerror_r(errno, bf, sizeof(bf));
=======
		const char *err = str_error_r(errno, bf, sizeof(bf));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ui_helpline__fpush("Couldn't write to %s: %s", filename, err);
		return -1;
	}

	++browser->print_seq;
	hist_browser__fprintf(browser, fp);
	fclose(fp);
	ui_helpline__fpush("%s written!", filename);

	return 0;
}

<<<<<<< HEAD
static struct hist_browser *hist_browser__new(struct hists *hists)
{
	struct hist_browser *browser = zalloc(sizeof(*browser));

	if (browser) {
		browser->hists = hists;
		browser->b.refresh = hist_browser__refresh;
		browser->b.seek = ui_browser__hists_seek;
		browser->b.use_navkeypressed = true;
		if (sort__branch_mode == 1)
			browser->has_symbols = sort_sym_from.list.next != NULL;
		else
			browser->has_symbols = sort_sym.list.next != NULL;
	}

	return browser;
}

static void hist_browser__delete(struct hist_browser *browser)
=======
void hist_browser__init(struct hist_browser *browser,
			struct hists *hists)
{
	struct perf_hpp_fmt *fmt;

	browser->hists			= hists;
	browser->b.refresh		= hist_browser__refresh;
	browser->b.refresh_dimensions	= hist_browser__refresh_dimensions;
	browser->b.seek			= ui_browser__hists_seek;
	browser->b.use_navkeypressed	= true;
	browser->show_headers		= symbol_conf.show_hist_headers;

	if (symbol_conf.report_hierarchy) {
		struct perf_hpp_list_node *fmt_node;

		/* count overhead columns (in the first node) */
		fmt_node = list_first_entry(&hists->hpp_formats,
					    struct perf_hpp_list_node, list);
		perf_hpp_list__for_each_format(&fmt_node->hpp, fmt)
			++browser->b.columns;

		/* add a single column for whole hierarchy sort keys*/
		++browser->b.columns;
	} else {
		hists__for_each_format(hists, fmt)
			++browser->b.columns;
	}

	hists__reset_column_width(hists);
}

struct hist_browser *hist_browser__new(struct hists *hists)
{
	struct hist_browser *browser = zalloc(sizeof(*browser));

	if (browser)
		hist_browser__init(browser, hists);

	return browser;
}

static struct hist_browser *
perf_evsel_browser__new(struct perf_evsel *evsel,
			struct hist_browser_timer *hbt,
			struct perf_env *env)
{
	struct hist_browser *browser = hist_browser__new(evsel__hists(evsel));

	if (browser) {
		browser->hbt   = hbt;
		browser->env   = env;
		browser->title = perf_evsel_browser_title;
	}
	return browser;
}

void hist_browser__delete(struct hist_browser *browser)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	free(browser);
}

static struct hist_entry *hist_browser__selected_entry(struct hist_browser *browser)
{
	return browser->he_selection;
}

static struct thread *hist_browser__selected_thread(struct hist_browser *browser)
{
	return browser->he_selection->thread;
}

<<<<<<< HEAD
static int hists__browser_title(struct hists *hists, char *bf, size_t size,
				const char *ev_name)
{
=======
/* Check whether the browser is for 'top' or 'report' */
static inline bool is_report_browser(void *timer)
{
	return timer == NULL;
}

static int perf_evsel_browser_title(struct hist_browser *browser,
				char *bf, size_t size)
{
	struct hist_browser_timer *hbt = browser->hbt;
	struct hists *hists = browser->hists;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char unit;
	int printed;
	const struct dso *dso = hists->dso_filter;
	const struct thread *thread = hists->thread_filter;
<<<<<<< HEAD
	unsigned long nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = hists->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(hists);
	char buf[512];
	size_t buflen = sizeof(buf);
=======
	int socket_id = hists->socket_filter;
	unsigned long nr_samples = hists->stats.nr_events[PERF_RECORD_SAMPLE];
	u64 nr_events = hists->stats.total_period;
	struct perf_evsel *evsel = hists_to_evsel(hists);
	const char *ev_name = perf_evsel__name(evsel);
	char buf[512];
	size_t buflen = sizeof(buf);
	char ref[30] = " show reference callgraph, ";
	bool enable_ref = false;

	if (symbol_conf.filter_relative) {
		nr_samples = hists->stats.nr_non_filtered_samples;
		nr_events = hists->stats.total_non_filtered_period;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (perf_evsel__is_group_event(evsel)) {
		struct perf_evsel *pos;

		perf_evsel__group_desc(evsel, buf, buflen);
		ev_name = buf;

		for_each_group_member(pos, evsel) {
<<<<<<< HEAD
			nr_samples += pos->hists.stats.nr_events[PERF_RECORD_SAMPLE];
			nr_events += pos->hists.stats.total_period;
		}
	}

	nr_samples = convert_unit(nr_samples, &unit);
	printed = scnprintf(bf, size,
			   "Samples: %lu%c of event '%s', Event count (approx.): %lu",
			   nr_samples, unit, ev_name, nr_events);
=======
			struct hists *pos_hists = evsel__hists(pos);

			if (symbol_conf.filter_relative) {
				nr_samples += pos_hists->stats.nr_non_filtered_samples;
				nr_events += pos_hists->stats.total_non_filtered_period;
			} else {
				nr_samples += pos_hists->stats.nr_events[PERF_RECORD_SAMPLE];
				nr_events += pos_hists->stats.total_period;
			}
		}
	}

	if (symbol_conf.show_ref_callgraph &&
	    strstr(ev_name, "call-graph=no"))
		enable_ref = true;
	nr_samples = convert_unit(nr_samples, &unit);
	printed = scnprintf(bf, size,
			   "Samples: %lu%c of event '%s',%sEvent count (approx.): %" PRIu64,
			   nr_samples, unit, ev_name, enable_ref ? ref : " ", nr_events);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414


	if (hists->uid_filter_str)
		printed += snprintf(bf + printed, size - printed,
				    ", UID: %s", hists->uid_filter_str);
<<<<<<< HEAD
	if (thread)
		printed += scnprintf(bf + printed, size - printed,
				    ", Thread: %s(%d)",
				    (thread->comm_set ? thread->comm : ""),
				    thread->pid);
	if (dso)
		printed += scnprintf(bf + printed, size - printed,
				    ", DSO: %s", dso->short_name);
=======
	if (thread) {
		if (hists__has(hists, thread)) {
			printed += scnprintf(bf + printed, size - printed,
				    ", Thread: %s(%d)",
				     (thread->comm_set ? thread__comm_str(thread) : ""),
				    thread->tid);
		} else {
			printed += scnprintf(bf + printed, size - printed,
				    ", Thread: %s",
				     (thread->comm_set ? thread__comm_str(thread) : ""));
		}
	}
	if (dso)
		printed += scnprintf(bf + printed, size - printed,
				    ", DSO: %s", dso->short_name);
	if (socket_id > -1)
		printed += scnprintf(bf + printed, size - printed,
				    ", Processor Socket: %d", socket_id);
	if (!is_report_browser(hbt)) {
		struct perf_top *top = hbt->arg;

		if (top->zero)
			printed += scnprintf(bf + printed, size - printed, " [z]");
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return printed;
}

static inline void free_popup_options(char **options, int n)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < n; ++i) {
		free(options[i]);
		options[i] = NULL;
	}
}

/* Check whether the browser is for 'top' or 'report' */
static inline bool is_report_browser(void *timer)
{
	return timer == NULL;
=======
	for (i = 0; i < n; ++i)
		zfree(&options[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Only runtime switching of perf data file will make "input_name" point
 * to a malloced buffer. So add "is_input_name_malloced" flag to decide
 * whether we need to call free() for current "input_name" during the switch.
 */
static bool is_input_name_malloced = false;

static int switch_data_file(void)
{
	char *pwd, *options[32], *abs_path[32], *tmp;
	DIR *pwd_dir;
	int nr_options = 0, choice = -1, ret = -1;
	struct dirent *dent;

	pwd = getenv("PWD");
	if (!pwd)
		return ret;

	pwd_dir = opendir(pwd);
	if (!pwd_dir)
		return ret;

	memset(options, 0, sizeof(options));
	memset(options, 0, sizeof(abs_path));

	while ((dent = readdir(pwd_dir))) {
		char path[PATH_MAX];
		u64 magic;
		char *name = dent->d_name;
		FILE *file;

		if (!(dent->d_type == DT_REG))
			continue;

		snprintf(path, sizeof(path), "%s/%s", pwd, name);

		file = fopen(path, "r");
		if (!file)
			continue;

		if (fread(&magic, 1, 8, file) < 8)
			goto close_file_and_continue;

		if (is_perf_magic(magic)) {
			options[nr_options] = strdup(name);
			if (!options[nr_options])
				goto close_file_and_continue;

			abs_path[nr_options] = strdup(path);
			if (!abs_path[nr_options]) {
<<<<<<< HEAD
				free(options[nr_options]);
=======
				zfree(&options[nr_options]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ui__warning("Can't search all data files due to memory shortage.\n");
				fclose(file);
				break;
			}

			nr_options++;
		}

close_file_and_continue:
		fclose(file);
		if (nr_options >= 32) {
			ui__warning("Too many perf data files in PWD!\n"
				    "Only the first 32 files will be listed.\n");
			break;
		}
	}
	closedir(pwd_dir);

	if (nr_options) {
		choice = ui__popup_menu(nr_options, options);
		if (choice < nr_options && choice >= 0) {
			tmp = strdup(abs_path[choice]);
			if (tmp) {
				if (is_input_name_malloced)
					free((void *)input_name);
				input_name = tmp;
				is_input_name_malloced = true;
				ret = 0;
			} else
				ui__warning("Data switch failed due to memory shortage!\n");
		}
	}

	free_popup_options(options, nr_options);
	free_popup_options(abs_path, nr_options);
	return ret;
}

<<<<<<< HEAD

static int perf_evsel__hists_browse(struct perf_evsel *evsel, int nr_events,
				    const char *helpline, const char *ev_name,
				    bool left_exits,
				    struct hist_browser_timer *hbt,
				    struct perf_session_env *env)
{
	struct hists *hists = &evsel->hists;
	struct hist_browser *browser = hist_browser__new(hists);
	struct branch_info *bi;
	struct pstack *fstack;
	char *options[16];
	int nr_options = 0;
	int key = -1;
	char buf[64];
	char script_opt[64];
	int delay_secs = hbt ? hbt->refresh : 0;

	if (browser == NULL)
		return -1;

	fstack = pstack__new(2);
	if (fstack == NULL)
=======
struct popup_action {
	struct thread 		*thread;
	struct map_symbol 	ms;
	int			socket;

	int (*fn)(struct hist_browser *browser, struct popup_action *act);
};

static int
do_annotate(struct hist_browser *browser, struct popup_action *act)
{
	struct perf_evsel *evsel;
	struct annotation *notes;
	struct hist_entry *he;
	int err;

	if (!objdump_path && perf_env__lookup_objdump(browser->env))
		return 0;

	notes = symbol__annotation(act->ms.sym);
	if (!notes->src)
		return 0;

	evsel = hists_to_evsel(browser->hists);
	err = map_symbol__tui_annotate(&act->ms, evsel, browser->hbt);
	he = hist_browser__selected_entry(browser);
	/*
	 * offer option to annotate the other branch source or target
	 * (if they exists) when returning from annotate
	 */
	if ((err == 'q' || err == CTRL('c')) && he->branch_info)
		return 1;

	ui_browser__update_nr_entries(&browser->b, browser->hists->nr_entries);
	if (err)
		ui_browser__handle_resize(&browser->b);
	return 0;
}

static int
add_annotate_opt(struct hist_browser *browser __maybe_unused,
		 struct popup_action *act, char **optstr,
		 struct map *map, struct symbol *sym)
{
	if (sym == NULL || map->dso->annotate_warned)
		return 0;

	if (asprintf(optstr, "Annotate %s", sym->name) < 0)
		return 0;

	act->ms.map = map;
	act->ms.sym = sym;
	act->fn = do_annotate;
	return 1;
}

static int
do_zoom_thread(struct hist_browser *browser, struct popup_action *act)
{
	struct thread *thread = act->thread;

	if ((!hists__has(browser->hists, thread) &&
	     !hists__has(browser->hists, comm)) || thread == NULL)
		return 0;

	if (browser->hists->thread_filter) {
		pstack__remove(browser->pstack, &browser->hists->thread_filter);
		perf_hpp__set_elide(HISTC_THREAD, false);
		thread__zput(browser->hists->thread_filter);
		ui_helpline__pop();
	} else {
		if (hists__has(browser->hists, thread)) {
			ui_helpline__fpush("To zoom out press ESC or ENTER + \"Zoom out of %s(%d) thread\"",
					   thread->comm_set ? thread__comm_str(thread) : "",
					   thread->tid);
		} else {
			ui_helpline__fpush("To zoom out press ESC or ENTER + \"Zoom out of %s thread\"",
					   thread->comm_set ? thread__comm_str(thread) : "");
		}

		browser->hists->thread_filter = thread__get(thread);
		perf_hpp__set_elide(HISTC_THREAD, false);
		pstack__push(browser->pstack, &browser->hists->thread_filter);
	}

	hists__filter_by_thread(browser->hists);
	hist_browser__reset(browser);
	return 0;
}

static int
add_thread_opt(struct hist_browser *browser, struct popup_action *act,
	       char **optstr, struct thread *thread)
{
	int ret;

	if ((!hists__has(browser->hists, thread) &&
	     !hists__has(browser->hists, comm)) || thread == NULL)
		return 0;

	if (hists__has(browser->hists, thread)) {
		ret = asprintf(optstr, "Zoom %s %s(%d) thread",
			       browser->hists->thread_filter ? "out of" : "into",
			       thread->comm_set ? thread__comm_str(thread) : "",
			       thread->tid);
	} else {
		ret = asprintf(optstr, "Zoom %s %s thread",
			       browser->hists->thread_filter ? "out of" : "into",
			       thread->comm_set ? thread__comm_str(thread) : "");
	}
	if (ret < 0)
		return 0;

	act->thread = thread;
	act->fn = do_zoom_thread;
	return 1;
}

static int
do_zoom_dso(struct hist_browser *browser, struct popup_action *act)
{
	struct map *map = act->ms.map;

	if (!hists__has(browser->hists, dso) || map == NULL)
		return 0;

	if (browser->hists->dso_filter) {
		pstack__remove(browser->pstack, &browser->hists->dso_filter);
		perf_hpp__set_elide(HISTC_DSO, false);
		browser->hists->dso_filter = NULL;
		ui_helpline__pop();
	} else {
		ui_helpline__fpush("To zoom out press ESC or ENTER + \"Zoom out of %s DSO\"",
				   __map__is_kernel(map) ? "the Kernel" : map->dso->short_name);
		browser->hists->dso_filter = map->dso;
		perf_hpp__set_elide(HISTC_DSO, true);
		pstack__push(browser->pstack, &browser->hists->dso_filter);
	}

	hists__filter_by_dso(browser->hists);
	hist_browser__reset(browser);
	return 0;
}

static int
add_dso_opt(struct hist_browser *browser, struct popup_action *act,
	    char **optstr, struct map *map)
{
	if (!hists__has(browser->hists, dso) || map == NULL)
		return 0;

	if (asprintf(optstr, "Zoom %s %s DSO",
		     browser->hists->dso_filter ? "out of" : "into",
		     __map__is_kernel(map) ? "the Kernel" : map->dso->short_name) < 0)
		return 0;

	act->ms.map = map;
	act->fn = do_zoom_dso;
	return 1;
}

static int
do_browse_map(struct hist_browser *browser __maybe_unused,
	      struct popup_action *act)
{
	map__browse(act->ms.map);
	return 0;
}

static int
add_map_opt(struct hist_browser *browser,
	    struct popup_action *act, char **optstr, struct map *map)
{
	if (!hists__has(browser->hists, dso) || map == NULL)
		return 0;

	if (asprintf(optstr, "Browse map details") < 0)
		return 0;

	act->ms.map = map;
	act->fn = do_browse_map;
	return 1;
}

static int
do_run_script(struct hist_browser *browser __maybe_unused,
	      struct popup_action *act)
{
	char script_opt[64];
	memset(script_opt, 0, sizeof(script_opt));

	if (act->thread) {
		scnprintf(script_opt, sizeof(script_opt), " -c %s ",
			  thread__comm_str(act->thread));
	} else if (act->ms.sym) {
		scnprintf(script_opt, sizeof(script_opt), " -S %s ",
			  act->ms.sym->name);
	}

	script_browse(script_opt);
	return 0;
}

static int
add_script_opt(struct hist_browser *browser __maybe_unused,
	       struct popup_action *act, char **optstr,
	       struct thread *thread, struct symbol *sym)
{
	if (thread) {
		if (asprintf(optstr, "Run scripts for samples of thread [%s]",
			     thread__comm_str(thread)) < 0)
			return 0;
	} else if (sym) {
		if (asprintf(optstr, "Run scripts for samples of symbol [%s]",
			     sym->name) < 0)
			return 0;
	} else {
		if (asprintf(optstr, "Run scripts for all samples") < 0)
			return 0;
	}

	act->thread = thread;
	act->ms.sym = sym;
	act->fn = do_run_script;
	return 1;
}

static int
do_switch_data(struct hist_browser *browser __maybe_unused,
	       struct popup_action *act __maybe_unused)
{
	if (switch_data_file()) {
		ui__warning("Won't switch the data files due to\n"
			    "no valid data file get selected!\n");
		return 0;
	}

	return K_SWITCH_INPUT_DATA;
}

static int
add_switch_opt(struct hist_browser *browser,
	       struct popup_action *act, char **optstr)
{
	if (!is_report_browser(browser->hbt))
		return 0;

	if (asprintf(optstr, "Switch to another data file in PWD") < 0)
		return 0;

	act->fn = do_switch_data;
	return 1;
}

static int
do_exit_browser(struct hist_browser *browser __maybe_unused,
		struct popup_action *act __maybe_unused)
{
	return 0;
}

static int
add_exit_opt(struct hist_browser *browser __maybe_unused,
	     struct popup_action *act, char **optstr)
{
	if (asprintf(optstr, "Exit") < 0)
		return 0;

	act->fn = do_exit_browser;
	return 1;
}

static int
do_zoom_socket(struct hist_browser *browser, struct popup_action *act)
{
	if (!hists__has(browser->hists, socket) || act->socket < 0)
		return 0;

	if (browser->hists->socket_filter > -1) {
		pstack__remove(browser->pstack, &browser->hists->socket_filter);
		browser->hists->socket_filter = -1;
		perf_hpp__set_elide(HISTC_SOCKET, false);
	} else {
		browser->hists->socket_filter = act->socket;
		perf_hpp__set_elide(HISTC_SOCKET, true);
		pstack__push(browser->pstack, &browser->hists->socket_filter);
	}

	hists__filter_by_socket(browser->hists);
	hist_browser__reset(browser);
	return 0;
}

static int
add_socket_opt(struct hist_browser *browser, struct popup_action *act,
	       char **optstr, int socket_id)
{
	if (!hists__has(browser->hists, socket) || socket_id < 0)
		return 0;

	if (asprintf(optstr, "Zoom %s Processor Socket %d",
		     (browser->hists->socket_filter > -1) ? "out of" : "into",
		     socket_id) < 0)
		return 0;

	act->socket = socket_id;
	act->fn = do_zoom_socket;
	return 1;
}

static void hist_browser__update_nr_entries(struct hist_browser *hb)
{
	u64 nr_entries = 0;
	struct rb_node *nd = rb_first(&hb->hists->entries);

	if (hb->min_pcnt == 0 && !symbol_conf.report_hierarchy) {
		hb->nr_non_filtered_entries = hb->hists->nr_non_filtered_entries;
		return;
	}

	while ((nd = hists__filter_entries(nd, hb->min_pcnt)) != NULL) {
		nr_entries++;
		nd = rb_hierarchy_next(nd);
	}

	hb->nr_non_filtered_entries = nr_entries;
	hb->nr_hierarchy_entries = nr_entries;
}

static void hist_browser__update_percent_limit(struct hist_browser *hb,
					       double percent)
{
	struct hist_entry *he;
	struct rb_node *nd = rb_first(&hb->hists->entries);
	u64 total = hists__total_period(hb->hists);
	u64 min_callchain_hits = total * (percent / 100);

	hb->min_pcnt = callchain_param.min_percent = percent;

	while ((nd = hists__filter_entries(nd, hb->min_pcnt)) != NULL) {
		he = rb_entry(nd, struct hist_entry, rb_node);

		if (he->has_no_entry) {
			he->has_no_entry = false;
			he->nr_rows = 0;
		}

		if (!he->leaf || !symbol_conf.use_callchain)
			goto next;

		if (callchain_param.mode == CHAIN_GRAPH_REL) {
			total = he->stat.period;

			if (symbol_conf.cumulate_callchain)
				total = he->stat_acc->period;

			min_callchain_hits = total * (percent / 100);
		}

		callchain_param.sort(&he->sorted_chain, he->callchain,
				     min_callchain_hits, &callchain_param);

next:
		nd = __rb_hierarchy_next(nd, HMD_FORCE_CHILD);

		/* force to re-evaluate folding state of callchains */
		he->init_have_children = false;
		hist_entry__set_folding(he, hb, false);
	}
}

static int perf_evsel__hists_browse(struct perf_evsel *evsel, int nr_events,
				    const char *helpline,
				    bool left_exits,
				    struct hist_browser_timer *hbt,
				    float min_pcnt,
				    struct perf_env *env)
{
	struct hists *hists = evsel__hists(evsel);
	struct hist_browser *browser = perf_evsel_browser__new(evsel, hbt, env);
	struct branch_info *bi;
#define MAX_OPTIONS  16
	char *options[MAX_OPTIONS];
	struct popup_action actions[MAX_OPTIONS];
	int nr_options = 0;
	int key = -1;
	char buf[64];
	int delay_secs = hbt ? hbt->refresh : 0;

#define HIST_BROWSER_HELP_COMMON					\
	"h/?/F1        Show this window\n"				\
	"UP/DOWN/PGUP\n"						\
	"PGDN/SPACE    Navigate\n"					\
	"q/ESC/CTRL+C  Exit browser\n\n"				\
	"For multiple event sessions:\n\n"				\
	"TAB/UNTAB     Switch events\n\n"				\
	"For symbolic views (--sort has sym):\n\n"			\
	"ENTER         Zoom into DSO/Threads & Annotate current symbol\n" \
	"ESC           Zoom out\n"					\
	"a             Annotate current symbol\n"			\
	"C             Collapse all callchains\n"			\
	"d             Zoom into current DSO\n"				\
	"E             Expand all callchains\n"				\
	"F             Toggle percentage of filtered entries\n"		\
	"H             Display column headers\n"			\
	"L             Change percent limit\n"				\
	"m             Display context menu\n"				\
	"S             Zoom into current Processor Socket\n"		\

	/* help messages are sorted by lexical order of the hotkey */
	const char report_help[] = HIST_BROWSER_HELP_COMMON
	"i             Show header information\n"
	"P             Print histograms to perf.hist.N\n"
	"r             Run available scripts\n"
	"s             Switch to another data file in PWD\n"
	"t             Zoom into current Thread\n"
	"V             Verbose (DSO names in callchains, etc)\n"
	"/             Filter symbol by name";
	const char top_help[] = HIST_BROWSER_HELP_COMMON
	"P             Print histograms to perf.hist.N\n"
	"t             Zoom into current Thread\n"
	"V             Verbose (DSO names in callchains, etc)\n"
	"z             Toggle zeroing of samples\n"
	"f             Enable/Disable events\n"
	"/             Filter symbol by name";

	if (browser == NULL)
		return -1;

	/* reset abort key so that it can get Ctrl-C as a key */
	SLang_reset_tty();
	SLang_init_tty(0, 0, 0);

	if (min_pcnt)
		browser->min_pcnt = min_pcnt;
	hist_browser__update_nr_entries(browser);

	browser->pstack = pstack__new(3);
	if (browser->pstack == NULL)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto out;

	ui_helpline__push(helpline);

	memset(options, 0, sizeof(options));
<<<<<<< HEAD

	while (1) {
		const struct thread *thread = NULL;
		const struct dso *dso = NULL;
		int choice = 0,
		    annotate = -2, zoom_dso = -2, zoom_thread = -2,
		    annotate_f = -2, annotate_t = -2, browse_map = -2;
		int scripts_comm = -2, scripts_symbol = -2,
		    scripts_all = -2, switch_data = -2;

		nr_options = 0;

		key = hist_browser__run(browser, ev_name, hbt);

		if (browser->he_selection != NULL) {
			thread = hist_browser__selected_thread(browser);
			dso = browser->selection->map ? browser->selection->map->dso : NULL;
=======
	memset(actions, 0, sizeof(actions));

	if (symbol_conf.col_width_list_str)
		perf_hpp__set_user_width(symbol_conf.col_width_list_str);

	while (1) {
		struct thread *thread = NULL;
		struct map *map = NULL;
		int choice = 0;
		int socked_id = -1;

		nr_options = 0;

		key = hist_browser__run(browser, helpline);

		if (browser->he_selection != NULL) {
			thread = hist_browser__selected_thread(browser);
			map = browser->selection->map;
			socked_id = browser->he_selection->socket;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		switch (key) {
		case K_TAB:
		case K_UNTAB:
			if (nr_events == 1)
				continue;
			/*
			 * Exit the browser, let hists__browser_tree
			 * go to the next or previous
			 */
			goto out_free_stack;
		case 'a':
<<<<<<< HEAD
			if (!browser->has_symbols) {
=======
			if (!hists__has(hists, sym)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				ui_browser__warning(&browser->b, delay_secs * 2,
			"Annotation is only available for symbolic views, "
			"include \"sym*\" in --sort to use it.");
				continue;
			}

			if (browser->selection == NULL ||
			    browser->selection->sym == NULL ||
			    browser->selection->map->dso->annotate_warned)
				continue;
<<<<<<< HEAD
			goto do_annotate;
=======

			actions->ms.map = browser->selection->map;
			actions->ms.sym = browser->selection->sym;
			do_annotate(browser, actions);
			continue;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case 'P':
			hist_browser__dump(browser);
			continue;
		case 'd':
<<<<<<< HEAD
			goto zoom_dso;
=======
			actions->ms.map = map;
			do_zoom_dso(browser, actions);
			continue;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		case 'V':
			browser->show_dso = !browser->show_dso;
			continue;
		case 't':
<<<<<<< HEAD
			goto zoom_thread;
		case '/':
			if (ui_browser__input_window("Symbol to show",
					"Please enter the name of symbol you want to see",
=======
			actions->thread = thread;
			do_zoom_thread(browser, actions);
			continue;
		case 'S':
			actions->socket = socked_id;
			do_zoom_socket(browser, actions);
			continue;
		case '/':
			if (ui_browser__input_window("Symbol to show",
					"Please enter the name of symbol you want to see.\n"
					"To remove the filter later, press / + ENTER.",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					buf, "ENTER: OK, ESC: Cancel",
					delay_secs * 2) == K_ENTER) {
				hists->symbol_filter_str = *buf ? buf : NULL;
				hists__filter_by_symbol(hists);
				hist_browser__reset(browser);
			}
			continue;
		case 'r':
<<<<<<< HEAD
			if (is_report_browser(hbt))
				goto do_scripts;
			continue;
		case 's':
			if (is_report_browser(hbt))
				goto do_data_switch;
=======
			if (is_report_browser(hbt)) {
				actions->thread = NULL;
				actions->ms.sym = NULL;
				do_run_script(browser, actions);
			}
			continue;
		case 's':
			if (is_report_browser(hbt)) {
				key = do_switch_data(browser, actions);
				if (key == K_SWITCH_INPUT_DATA)
					goto out_free_stack;
			}
			continue;
		case 'i':
			/* env->arch is NULL for live-mode (i.e. perf top) */
			if (env->arch)
				tui__header_window(env);
			continue;
		case 'F':
			symbol_conf.filter_relative ^= 1;
			continue;
		case 'z':
			if (!is_report_browser(hbt)) {
				struct perf_top *top = hbt->arg;

				top->zero = !top->zero;
			}
			continue;
		case 'L':
			if (ui_browser__input_window("Percent Limit",
					"Please enter the value you want to hide entries under that percent.",
					buf, "ENTER: OK, ESC: Cancel",
					delay_secs * 2) == K_ENTER) {
				char *end;
				double new_percent = strtod(buf, &end);

				if (new_percent < 0 || new_percent > 100) {
					ui_browser__warning(&browser->b, delay_secs * 2,
						"Invalid percent: %.2f", new_percent);
					continue;
				}

				hist_browser__update_percent_limit(browser, new_percent);
				hist_browser__reset(browser);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		case K_F1:
		case 'h':
		case '?':
			ui_browser__help_window(&browser->b,
<<<<<<< HEAD
					"h/?/F1        Show this window\n"
					"UP/DOWN/PGUP\n"
					"PGDN/SPACE    Navigate\n"
					"q/ESC/CTRL+C  Exit browser\n\n"
					"For multiple event sessions:\n\n"
					"TAB/UNTAB Switch events\n\n"
					"For symbolic views (--sort has sym):\n\n"
					"->            Zoom into DSO/Threads & Annotate current symbol\n"
					"<-            Zoom out\n"
					"a             Annotate current symbol\n"
					"C             Collapse all callchains\n"
					"E             Expand all callchains\n"
					"d             Zoom into current DSO\n"
					"t             Zoom into current Thread\n"
					"r             Run available scripts('perf report' only)\n"
					"s             Switch to another data file in PWD ('perf report' only)\n"
					"P             Print histograms to perf.hist.N\n"
					"V             Verbose (DSO names in callchains, etc)\n"
					"/             Filter symbol by name");
			continue;
		case K_ENTER:
		case K_RIGHT:
			/* menu */
			break;
		case K_LEFT: {
			const void *top;

			if (pstack__empty(fstack)) {
=======
				is_report_browser(hbt) ? report_help : top_help);
			continue;
		case K_ENTER:
		case K_RIGHT:
		case 'm':
			/* menu */
			break;
		case K_ESC:
		case K_LEFT: {
			const void *top;

			if (pstack__empty(browser->pstack)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				/*
				 * Go back to the perf_evsel_menu__run or other user
				 */
				if (left_exits)
					goto out_free_stack;
<<<<<<< HEAD
				continue;
			}
			top = pstack__pop(fstack);
			if (top == &browser->hists->dso_filter)
				goto zoom_out_dso;
			if (top == &browser->hists->thread_filter)
				goto zoom_out_thread;
			continue;
		}
		case K_ESC:
			if (!left_exits &&
			    !ui_browser__dialog_yesno(&browser->b,
					       "Do you really want to exit?"))
				continue;
			/* Fall thru */
		case 'q':
		case CTRL('c'):
			goto out_free_stack;
		default:
			continue;
		}

		if (!browser->has_symbols)
			goto add_exit_option;

		if (sort__branch_mode == 1) {
			bi = browser->he_selection->branch_info;
			if (browser->selection != NULL &&
			    bi &&
			    bi->from.sym != NULL &&
			    !bi->from.map->dso->annotate_warned &&
				asprintf(&options[nr_options], "Annotate %s",
					 bi->from.sym->name) > 0)
				annotate_f = nr_options++;

			if (browser->selection != NULL &&
			    bi &&
			    bi->to.sym != NULL &&
			    !bi->to.map->dso->annotate_warned &&
			    (bi->to.sym != bi->from.sym ||
			     bi->to.map->dso != bi->from.map->dso) &&
				asprintf(&options[nr_options], "Annotate %s",
					 bi->to.sym->name) > 0)
				annotate_t = nr_options++;
		} else {

			if (browser->selection != NULL &&
			    browser->selection->sym != NULL &&
			    !browser->selection->map->dso->annotate_warned &&
				asprintf(&options[nr_options], "Annotate %s",
					 browser->selection->sym->name) > 0)
				annotate = nr_options++;
		}

		if (thread != NULL &&
		    asprintf(&options[nr_options], "Zoom %s %s(%d) thread",
			     (browser->hists->thread_filter ? "out of" : "into"),
			     (thread->comm_set ? thread->comm : ""),
			     thread->pid) > 0)
			zoom_thread = nr_options++;

		if (dso != NULL &&
		    asprintf(&options[nr_options], "Zoom %s %s DSO",
			     (browser->hists->dso_filter ? "out of" : "into"),
			     (dso->kernel ? "the Kernel" : dso->short_name)) > 0)
			zoom_dso = nr_options++;

		if (browser->selection != NULL &&
		    browser->selection->map != NULL &&
		    asprintf(&options[nr_options], "Browse map details") > 0)
			browse_map = nr_options++;

		/* perf script support */
		if (browser->he_selection) {
			struct symbol *sym;

			if (asprintf(&options[nr_options], "Run scripts for samples of thread [%s]",
				browser->he_selection->thread->comm) > 0)
				scripts_comm = nr_options++;

			sym = browser->he_selection->ms.sym;
			if (sym && sym->namelen &&
				asprintf(&options[nr_options], "Run scripts for samples of symbol [%s]",
						sym->name) > 0)
				scripts_symbol = nr_options++;
		}

		if (asprintf(&options[nr_options], "Run scripts for all samples") > 0)
			scripts_all = nr_options++;

		if (is_report_browser(hbt) && asprintf(&options[nr_options],
				"Switch to another data file in PWD") > 0)
			switch_data = nr_options++;
add_exit_option:
		options[nr_options++] = (char *)"Exit";
retry_popup_menu:
		choice = ui__popup_menu(nr_options, options);

		if (choice == nr_options - 1)
			break;

		if (choice == -1) {
			free_popup_options(options, nr_options - 1);
			continue;
		}

		if (choice == annotate || choice == annotate_t || choice == annotate_f) {
			struct hist_entry *he;
			int err;
do_annotate:
			if (!objdump_path && perf_session_env__lookup_objdump(env))
				continue;

			he = hist_browser__selected_entry(browser);
			if (he == NULL)
				continue;

			/*
			 * we stash the branch_info symbol + map into the
			 * the ms so we don't have to rewrite all the annotation
			 * code to use branch_info.
			 * in branch mode, the ms struct is not used
			 */
			if (choice == annotate_f) {
				he->ms.sym = he->branch_info->from.sym;
				he->ms.map = he->branch_info->from.map;
			}  else if (choice == annotate_t) {
				he->ms.sym = he->branch_info->to.sym;
				he->ms.map = he->branch_info->to.map;
			}

			/*
			 * Don't let this be freed, say, by hists__decay_entry.
			 */
			he->used = true;
			err = hist_entry__tui_annotate(he, evsel, hbt);
			he->used = false;
			/*
			 * offer option to annotate the other branch source or target
			 * (if they exists) when returning from annotate
			 */
			if ((err == 'q' || err == CTRL('c'))
			    && annotate_t != -2 && annotate_f != -2)
				goto retry_popup_menu;

			ui_browser__update_nr_entries(&browser->b, browser->hists->nr_entries);
			if (err)
				ui_browser__handle_resize(&browser->b);

		} else if (choice == browse_map)
			map__browse(browser->selection->map);
		else if (choice == zoom_dso) {
zoom_dso:
			if (browser->hists->dso_filter) {
				pstack__remove(fstack, &browser->hists->dso_filter);
zoom_out_dso:
				ui_helpline__pop();
				browser->hists->dso_filter = NULL;
				sort_dso.elide = false;
			} else {
				if (dso == NULL)
					continue;
				ui_helpline__fpush("To zoom out press <- or -> + \"Zoom out of %s DSO\"",
						   dso->kernel ? "the Kernel" : dso->short_name);
				browser->hists->dso_filter = dso;
				sort_dso.elide = true;
				pstack__push(fstack, &browser->hists->dso_filter);
			}
			hists__filter_by_dso(hists);
			hist_browser__reset(browser);
		} else if (choice == zoom_thread) {
zoom_thread:
			if (browser->hists->thread_filter) {
				pstack__remove(fstack, &browser->hists->thread_filter);
zoom_out_thread:
				ui_helpline__pop();
				browser->hists->thread_filter = NULL;
				sort_thread.elide = false;
			} else {
				ui_helpline__fpush("To zoom out press <- or -> + \"Zoom out of %s(%d) thread\"",
						   thread->comm_set ? thread->comm : "",
						   thread->pid);
				browser->hists->thread_filter = thread;
				sort_thread.elide = true;
				pstack__push(fstack, &browser->hists->thread_filter);
			}
			hists__filter_by_thread(hists);
			hist_browser__reset(browser);
		}
		/* perf scripts support */
		else if (choice == scripts_all || choice == scripts_comm ||
				choice == scripts_symbol) {
do_scripts:
			memset(script_opt, 0, 64);

			if (choice == scripts_comm)
				sprintf(script_opt, " -c %s ", browser->he_selection->thread->comm);

			if (choice == scripts_symbol)
				sprintf(script_opt, " -S %s ", browser->he_selection->ms.sym->name);

			script_browse(script_opt);
		}
		/* Switch to another data file */
		else if (choice == switch_data) {
do_data_switch:
			if (!switch_data_file()) {
				key = K_SWITCH_INPUT_DATA;
				break;
			} else
				ui__warning("Won't switch the data files due to\n"
					"no valid data file get selected!\n");
		}
	}
out_free_stack:
	pstack__delete(fstack);
out:
	hist_browser__delete(browser);
	free_popup_options(options, nr_options - 1);
=======

				if (key == K_ESC &&
				    ui_browser__dialog_yesno(&browser->b,
							     "Do you really want to exit?"))
					goto out_free_stack;

				continue;
			}
			actions->ms.map = map;
			top = pstack__peek(browser->pstack);
			if (top == &browser->hists->dso_filter) {
				/*
				 * No need to set actions->dso here since
				 * it's just to remove the current filter.
				 * Ditto for thread below.
				 */
				do_zoom_dso(browser, actions);
			} else if (top == &browser->hists->thread_filter) {
				do_zoom_thread(browser, actions);
			} else if (top == &browser->hists->socket_filter) {
				do_zoom_socket(browser, actions);
			}
			continue;
		}
		case 'q':
		case CTRL('c'):
			goto out_free_stack;
		case 'f':
			if (!is_report_browser(hbt)) {
				struct perf_top *top = hbt->arg;

				perf_evlist__toggle_enable(top->evlist);
				/*
				 * No need to refresh, resort/decay histogram
				 * entries if we are not collecting samples:
				 */
				if (top->evlist->enabled) {
					helpline = "Press 'f' to disable the events or 'h' to see other hotkeys";
					hbt->refresh = delay_secs;
				} else {
					helpline = "Press 'f' again to re-enable the events";
					hbt->refresh = 0;
				}
				continue;
			}
			/* Fall thru */
		default:
			helpline = "Press '?' for help on key bindings";
			continue;
		}

		if (!hists__has(hists, sym) || browser->selection == NULL)
			goto skip_annotation;

		if (sort__mode == SORT_MODE__BRANCH) {
			bi = browser->he_selection->branch_info;

			if (bi == NULL)
				goto skip_annotation;

			nr_options += add_annotate_opt(browser,
						       &actions[nr_options],
						       &options[nr_options],
						       bi->from.map,
						       bi->from.sym);
			if (bi->to.sym != bi->from.sym)
				nr_options += add_annotate_opt(browser,
							&actions[nr_options],
							&options[nr_options],
							bi->to.map,
							bi->to.sym);
		} else {
			nr_options += add_annotate_opt(browser,
						       &actions[nr_options],
						       &options[nr_options],
						       browser->selection->map,
						       browser->selection->sym);
		}
skip_annotation:
		nr_options += add_thread_opt(browser, &actions[nr_options],
					     &options[nr_options], thread);
		nr_options += add_dso_opt(browser, &actions[nr_options],
					  &options[nr_options], map);
		nr_options += add_map_opt(browser, &actions[nr_options],
					  &options[nr_options],
					  browser->selection ?
						browser->selection->map : NULL);
		nr_options += add_socket_opt(browser, &actions[nr_options],
					     &options[nr_options],
					     socked_id);
		/* perf script support */
		if (!is_report_browser(hbt))
			goto skip_scripting;

		if (browser->he_selection) {
			if (hists__has(hists, thread) && thread) {
				nr_options += add_script_opt(browser,
							     &actions[nr_options],
							     &options[nr_options],
							     thread, NULL);
			}
			/*
			 * Note that browser->selection != NULL
			 * when browser->he_selection is not NULL,
			 * so we don't need to check browser->selection
			 * before fetching browser->selection->sym like what
			 * we do before fetching browser->selection->map.
			 *
			 * See hist_browser__show_entry.
			 */
			if (hists__has(hists, sym) && browser->selection->sym) {
				nr_options += add_script_opt(browser,
							     &actions[nr_options],
							     &options[nr_options],
							     NULL, browser->selection->sym);
			}
		}
		nr_options += add_script_opt(browser, &actions[nr_options],
					     &options[nr_options], NULL, NULL);
		nr_options += add_switch_opt(browser, &actions[nr_options],
					     &options[nr_options]);
skip_scripting:
		nr_options += add_exit_opt(browser, &actions[nr_options],
					   &options[nr_options]);

		do {
			struct popup_action *act;

			choice = ui__popup_menu(nr_options, options);
			if (choice == -1 || choice >= nr_options)
				break;

			act = &actions[choice];
			key = act->fn(browser, act);
		} while (key == 1);

		if (key == K_SWITCH_INPUT_DATA)
			break;
	}
out_free_stack:
	pstack__delete(browser->pstack);
out:
	hist_browser__delete(browser);
	free_popup_options(options, MAX_OPTIONS);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return key;
}

struct perf_evsel_menu {
	struct ui_browser b;
	struct perf_evsel *selection;
	bool lost_events, lost_events_warned;
<<<<<<< HEAD
	struct perf_session_env *env;
=======
	float min_pcnt;
	struct perf_env *env;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static void perf_evsel_menu__write(struct ui_browser *browser,
				   void *entry, int row)
{
	struct perf_evsel_menu *menu = container_of(browser,
						    struct perf_evsel_menu, b);
	struct perf_evsel *evsel = list_entry(entry, struct perf_evsel, node);
<<<<<<< HEAD
	bool current_entry = ui_browser__is_current_entry(browser, row);
	unsigned long nr_events = evsel->hists.stats.nr_events[PERF_RECORD_SAMPLE];
=======
	struct hists *hists = evsel__hists(evsel);
	bool current_entry = ui_browser__is_current_entry(browser, row);
	unsigned long nr_events = hists->stats.nr_events[PERF_RECORD_SAMPLE];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char *ev_name = perf_evsel__name(evsel);
	char bf[256], unit;
	const char *warn = " ";
	size_t printed;

	ui_browser__set_color(browser, current_entry ? HE_COLORSET_SELECTED :
						       HE_COLORSET_NORMAL);

	if (perf_evsel__is_group_event(evsel)) {
		struct perf_evsel *pos;

		ev_name = perf_evsel__group_name(evsel);

		for_each_group_member(pos, evsel) {
<<<<<<< HEAD
			nr_events += pos->hists.stats.nr_events[PERF_RECORD_SAMPLE];
=======
			struct hists *pos_hists = evsel__hists(pos);
			nr_events += pos_hists->stats.nr_events[PERF_RECORD_SAMPLE];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	nr_events = convert_unit(nr_events, &unit);
	printed = scnprintf(bf, sizeof(bf), "%lu%c%s%s", nr_events,
			   unit, unit == ' ' ? "" : " ", ev_name);
<<<<<<< HEAD
	slsmg_printf("%s", bf);

	nr_events = evsel->hists.stats.nr_events[PERF_RECORD_LOST];
=======
	ui_browser__printf(browser, "%s", bf);

	nr_events = hists->stats.nr_events[PERF_RECORD_LOST];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (nr_events != 0) {
		menu->lost_events = true;
		if (!current_entry)
			ui_browser__set_color(browser, HE_COLORSET_TOP);
		nr_events = convert_unit(nr_events, &unit);
		printed += scnprintf(bf, sizeof(bf), ": %ld%c%schunks LOST!",
				     nr_events, unit, unit == ' ' ? "" : " ");
		warn = bf;
	}

<<<<<<< HEAD
	slsmg_write_nstring(warn, browser->width - printed);
=======
	ui_browser__write_nstring(browser, warn, browser->width - printed);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (current_entry)
		menu->selection = evsel;
}

static int perf_evsel_menu__run(struct perf_evsel_menu *menu,
				int nr_events, const char *help,
				struct hist_browser_timer *hbt)
{
	struct perf_evlist *evlist = menu->b.priv;
	struct perf_evsel *pos;
<<<<<<< HEAD
	const char *ev_name, *title = "Available samples";
=======
	const char *title = "Available samples";
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int delay_secs = hbt ? hbt->refresh : 0;
	int key;

	if (ui_browser__show(&menu->b, title,
			     "ESC: exit, ENTER|->: Browse histograms") < 0)
		return -1;

	while (1) {
		key = ui_browser__run(&menu->b, delay_secs);

		switch (key) {
		case K_TIMER:
			hbt->timer(hbt->arg);

			if (!menu->lost_events_warned && menu->lost_events) {
				ui_browser__warn_lost_events(&menu->b);
				menu->lost_events_warned = true;
			}
			continue;
		case K_RIGHT:
		case K_ENTER:
			if (!menu->selection)
				continue;
			pos = menu->selection;
browse_hists:
			perf_evlist__set_selected(evlist, pos);
			/*
			 * Give the calling tool a chance to populate the non
			 * default evsel resorted hists tree.
			 */
			if (hbt)
				hbt->timer(hbt->arg);
<<<<<<< HEAD
			ev_name = perf_evsel__name(pos);
			key = perf_evsel__hists_browse(pos, nr_events, help,
						       ev_name, true, hbt,
=======
			key = perf_evsel__hists_browse(pos, nr_events, help,
						       true, hbt,
						       menu->min_pcnt,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						       menu->env);
			ui_browser__show_title(&menu->b, title);
			switch (key) {
			case K_TAB:
				if (pos->node.next == &evlist->entries)
<<<<<<< HEAD
					pos = list_entry(evlist->entries.next, struct perf_evsel, node);
				else
					pos = list_entry(pos->node.next, struct perf_evsel, node);
				goto browse_hists;
			case K_UNTAB:
				if (pos->node.prev == &evlist->entries)
					pos = list_entry(evlist->entries.prev, struct perf_evsel, node);
				else
					pos = list_entry(pos->node.prev, struct perf_evsel, node);
				goto browse_hists;
			case K_ESC:
				if (!ui_browser__dialog_yesno(&menu->b,
						"Do you really want to exit?"))
					continue;
				/* Fall thru */
=======
					pos = perf_evlist__first(evlist);
				else
					pos = perf_evsel__next(pos);
				goto browse_hists;
			case K_UNTAB:
				if (pos->node.prev == &evlist->entries)
					pos = perf_evlist__last(evlist);
				else
					pos = perf_evsel__prev(pos);
				goto browse_hists;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			case K_SWITCH_INPUT_DATA:
			case 'q':
			case CTRL('c'):
				goto out;
<<<<<<< HEAD
=======
			case K_ESC:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			default:
				continue;
			}
		case K_LEFT:
			continue;
		case K_ESC:
			if (!ui_browser__dialog_yesno(&menu->b,
					       "Do you really want to exit?"))
				continue;
			/* Fall thru */
		case 'q':
		case CTRL('c'):
			goto out;
		default:
			continue;
		}
	}

out:
	ui_browser__hide(&menu->b);
	return key;
}

<<<<<<< HEAD
static bool filter_group_entries(struct ui_browser *self __maybe_unused,
=======
static bool filter_group_entries(struct ui_browser *browser __maybe_unused,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 void *entry)
{
	struct perf_evsel *evsel = list_entry(entry, struct perf_evsel, node);

	if (symbol_conf.event_group && !perf_evsel__is_group_leader(evsel))
		return true;

	return false;
}

static int __perf_evlist__tui_browse_hists(struct perf_evlist *evlist,
					   int nr_entries, const char *help,
					   struct hist_browser_timer *hbt,
<<<<<<< HEAD
					   struct perf_session_env *env)
=======
					   float min_pcnt,
					   struct perf_env *env)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct perf_evsel *pos;
	struct perf_evsel_menu menu = {
		.b = {
			.entries    = &evlist->entries,
			.refresh    = ui_browser__list_head_refresh,
			.seek	    = ui_browser__list_head_seek,
			.write	    = perf_evsel_menu__write,
			.filter	    = filter_group_entries,
			.nr_entries = nr_entries,
			.priv	    = evlist,
		},
<<<<<<< HEAD
=======
		.min_pcnt = min_pcnt,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.env = env,
	};

	ui_helpline__push("Press ESC to exit");

<<<<<<< HEAD
	list_for_each_entry(pos, &evlist->entries, node) {
=======
	evlist__for_each_entry(evlist, pos) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		const char *ev_name = perf_evsel__name(pos);
		size_t line_len = strlen(ev_name) + 7;

		if (menu.b.width < line_len)
			menu.b.width = line_len;
	}

	return perf_evsel_menu__run(&menu, nr_entries, help, hbt);
}

int perf_evlist__tui_browse_hists(struct perf_evlist *evlist, const char *help,
				  struct hist_browser_timer *hbt,
<<<<<<< HEAD
				  struct perf_session_env *env)
=======
				  float min_pcnt,
				  struct perf_env *env)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int nr_entries = evlist->nr_entries;

single_entry:
	if (nr_entries == 1) {
<<<<<<< HEAD
		struct perf_evsel *first = list_entry(evlist->entries.next,
						      struct perf_evsel, node);
		const char *ev_name = perf_evsel__name(first);

		return perf_evsel__hists_browse(first, nr_entries, help,
						ev_name, false, hbt, env);
=======
		struct perf_evsel *first = perf_evlist__first(evlist);

		return perf_evsel__hists_browse(first, nr_entries, help,
						false, hbt, min_pcnt,
						env);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (symbol_conf.event_group) {
		struct perf_evsel *pos;

		nr_entries = 0;
<<<<<<< HEAD
		list_for_each_entry(pos, &evlist->entries, node)
			if (perf_evsel__is_group_leader(pos))
				nr_entries++;
=======
		evlist__for_each_entry(evlist, pos) {
			if (perf_evsel__is_group_leader(pos))
				nr_entries++;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (nr_entries == 1)
			goto single_entry;
	}

	return __perf_evlist__tui_browse_hists(evlist, nr_entries, help,
<<<<<<< HEAD
					       hbt, env);
=======
					       hbt, min_pcnt, env);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
