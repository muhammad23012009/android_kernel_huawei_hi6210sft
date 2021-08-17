#ifndef _PERF_UI_BROWSER_H_
#define _PERF_UI_BROWSER_H_ 1

<<<<<<< HEAD
#include <stdbool.h>
#include <sys/types.h>
#include "../types.h"
=======
#include <linux/types.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define HE_COLORSET_TOP		50
#define HE_COLORSET_MEDIUM	51
#define HE_COLORSET_NORMAL	52
#define HE_COLORSET_SELECTED	53
<<<<<<< HEAD
#define HE_COLORSET_CODE	54
=======
#define HE_COLORSET_JUMP_ARROWS	54
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define HE_COLORSET_ADDR	55
#define HE_COLORSET_ROOT	56

struct ui_browser {
	u64	      index, top_idx;
	void	      *top, *entries;
<<<<<<< HEAD
	u16	      y, x, width, height;
=======
	u16	      y, x, width, height, rows, columns, horiz_scroll;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int	      current_color;
	void	      *priv;
	const char    *title;
	char	      *helpline;
<<<<<<< HEAD
	unsigned int  (*refresh)(struct ui_browser *self);
	void	      (*write)(struct ui_browser *self, void *entry, int row);
	void	      (*seek)(struct ui_browser *self, off_t offset, int whence);
	bool	      (*filter)(struct ui_browser *self, void *entry);
=======
	void 	      (*refresh_dimensions)(struct ui_browser *browser);
	unsigned int  (*refresh)(struct ui_browser *browser);
	void	      (*write)(struct ui_browser *browser, void *entry, int row);
	void	      (*seek)(struct ui_browser *browser, off_t offset, int whence);
	bool	      (*filter)(struct ui_browser *browser, void *entry);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32	      nr_entries;
	bool	      navkeypressed;
	bool	      use_navkeypressed;
};

int  ui_browser__set_color(struct ui_browser *browser, int color);
<<<<<<< HEAD
void ui_browser__set_percent_color(struct ui_browser *self,
				   double percent, bool current);
bool ui_browser__is_current_entry(struct ui_browser *self, unsigned row);
void ui_browser__refresh_dimensions(struct ui_browser *self);
void ui_browser__reset_index(struct ui_browser *self);

void ui_browser__gotorc(struct ui_browser *self, int y, int x);
=======
void ui_browser__set_percent_color(struct ui_browser *browser,
				   double percent, bool current);
bool ui_browser__is_current_entry(struct ui_browser *browser, unsigned row);
void ui_browser__refresh_dimensions(struct ui_browser *browser);
void ui_browser__reset_index(struct ui_browser *browser);

void ui_browser__gotorc(struct ui_browser *browser, int y, int x);
void ui_browser__write_nstring(struct ui_browser *browser, const char *msg,
			       unsigned int width);
void ui_browser__printf(struct ui_browser *browser, const char *fmt, ...);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void ui_browser__write_graph(struct ui_browser *browser, int graph);
void __ui_browser__line_arrow(struct ui_browser *browser, unsigned int column,
			      u64 start, u64 end);
void __ui_browser__show_title(struct ui_browser *browser, const char *title);
void ui_browser__show_title(struct ui_browser *browser, const char *title);
<<<<<<< HEAD
int ui_browser__show(struct ui_browser *self, const char *title,
		     const char *helpline, ...);
void ui_browser__hide(struct ui_browser *self);
int ui_browser__refresh(struct ui_browser *self);
=======
int ui_browser__show(struct ui_browser *browser, const char *title,
		     const char *helpline, ...);
void ui_browser__hide(struct ui_browser *browser);
int ui_browser__refresh(struct ui_browser *browser);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int ui_browser__run(struct ui_browser *browser, int delay_secs);
void ui_browser__update_nr_entries(struct ui_browser *browser, u32 nr_entries);
void ui_browser__handle_resize(struct ui_browser *browser);
void __ui_browser__vline(struct ui_browser *browser, unsigned int column,
			 u16 start, u16 end);

int ui_browser__warning(struct ui_browser *browser, int timeout,
			const char *format, ...);
int ui_browser__help_window(struct ui_browser *browser, const char *text);
bool ui_browser__dialog_yesno(struct ui_browser *browser, const char *text);
int ui_browser__input_window(const char *title, const char *text, char *input,
			     const char *exit_msg, int delay_sec);
<<<<<<< HEAD
=======
struct perf_env;
int tui__header_window(struct perf_env *env);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void ui_browser__argv_seek(struct ui_browser *browser, off_t offset, int whence);
unsigned int ui_browser__argv_refresh(struct ui_browser *browser);

<<<<<<< HEAD
void ui_browser__rb_tree_seek(struct ui_browser *self, off_t offset, int whence);
unsigned int ui_browser__rb_tree_refresh(struct ui_browser *self);

void ui_browser__list_head_seek(struct ui_browser *self, off_t offset, int whence);
unsigned int ui_browser__list_head_refresh(struct ui_browser *self);
=======
void ui_browser__rb_tree_seek(struct ui_browser *browser, off_t offset, int whence);
unsigned int ui_browser__rb_tree_refresh(struct ui_browser *browser);

void ui_browser__list_head_seek(struct ui_browser *browser, off_t offset, int whence);
unsigned int ui_browser__list_head_refresh(struct ui_browser *browser);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void ui_browser__init(void);
void annotate_browser__init(void);
#endif /* _PERF_UI_BROWSER_H_ */
