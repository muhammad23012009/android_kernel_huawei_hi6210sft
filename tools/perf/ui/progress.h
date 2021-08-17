#ifndef _PERF_UI_PROGRESS_H_
#define _PERF_UI_PROGRESS_H_ 1

<<<<<<< HEAD
#include <../types.h>

struct ui_progress {
	void (*update)(u64, u64, const char *);
	void (*finish)(void);
};

extern struct ui_progress *progress_fns;

void ui_progress__init(void);

void ui_progress__update(u64 curr, u64 total, const char *title);
void ui_progress__finish(void);
=======
#include <linux/types.h>

void ui_progress__finish(void);

struct ui_progress {
	const char *title;
	u64 curr, next, step, total;
};

void ui_progress__init(struct ui_progress *p, u64 total, const char *title);
void ui_progress__update(struct ui_progress *p, u64 adv);

struct ui_progress_ops {
	void (*update)(struct ui_progress *p);
	void (*finish)(void);
};

extern struct ui_progress_ops *ui_progress__ops;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
