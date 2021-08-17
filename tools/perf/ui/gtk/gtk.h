#ifndef _PERF_GTK_H_
#define _PERF_GTK_H_ 1

#include <stdbool.h>

#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#include <gtk/gtk.h>
#pragma GCC diagnostic error "-Wstrict-prototypes"


struct perf_gtk_context {
	GtkWidget *main_window;
	GtkWidget *notebook;

<<<<<<< HEAD
#ifdef HAVE_GTK_INFO_BAR
=======
#ifdef HAVE_GTK_INFO_BAR_SUPPORT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	GtkWidget *info_bar;
	GtkWidget *message_label;
#endif
	GtkWidget *statbar;
	guint statbar_ctx_id;
};

<<<<<<< HEAD
=======
int perf_gtk__init(void);
void perf_gtk__exit(bool wait_for_ok);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern struct perf_gtk_context *pgctx;

static inline bool perf_gtk__is_active_context(struct perf_gtk_context *ctx)
{
	return ctx && ctx->main_window;
}

struct perf_gtk_context *perf_gtk__activate_context(GtkWidget *window);
int perf_gtk__deactivate_context(struct perf_gtk_context **ctx);

void perf_gtk__init_helpline(void);
<<<<<<< HEAD
void perf_gtk__init_progress(void);
=======
void gtk_ui_progress__init(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void perf_gtk__init_hpp(void);

void perf_gtk__signal(int sig);
void perf_gtk__resize_window(GtkWidget *window);
const char *perf_gtk__get_percent_color(double percent);
GtkWidget *perf_gtk__setup_statusbar(void);

<<<<<<< HEAD
#ifdef HAVE_GTK_INFO_BAR
=======
#ifdef HAVE_GTK_INFO_BAR_SUPPORT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
GtkWidget *perf_gtk__setup_info_bar(void);
#else
static inline GtkWidget *perf_gtk__setup_info_bar(void)
{
	return NULL;
}
#endif

<<<<<<< HEAD
=======
struct perf_evsel;
struct perf_evlist;
struct hist_entry;
struct hist_browser_timer;

int perf_evlist__gtk_browse_hists(struct perf_evlist *evlist, const char *help,
				  struct hist_browser_timer *hbt,
				  float min_pcnt);
int hist_entry__gtk_annotate(struct hist_entry *he,
			     struct perf_evsel *evsel,
			     struct hist_browser_timer *hbt);
void perf_gtk__show_annotations(void);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _PERF_GTK_H_ */
