#include "../cache.h"
#include "progress.h"

<<<<<<< HEAD
static void nop_progress_update(u64 curr __maybe_unused,
				u64 total __maybe_unused,
				const char *title __maybe_unused)
{
}

static struct ui_progress default_progress_fns =
{
	.update		= nop_progress_update,
};

struct ui_progress *progress_fns = &default_progress_fns;

void ui_progress__update(u64 curr, u64 total, const char *title)
{
	return progress_fns->update(curr, total, title);
=======
static void null_progress__update(struct ui_progress *p __maybe_unused)
{
}

static struct ui_progress_ops null_progress__ops =
{
	.update = null_progress__update,
};

struct ui_progress_ops *ui_progress__ops = &null_progress__ops;

void ui_progress__update(struct ui_progress *p, u64 adv)
{
	p->curr += adv;

	if (p->curr >= p->next) {
		p->next += p->step;
		ui_progress__ops->update(p);
	}
}

void ui_progress__init(struct ui_progress *p, u64 total, const char *title)
{
	p->curr = 0;
	p->next = p->step = total / 16;
	p->total = total;
	p->title = title;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void ui_progress__finish(void)
{
<<<<<<< HEAD
	if (progress_fns->finish)
		progress_fns->finish();
=======
	if (ui_progress__ops->finish)
		ui_progress__ops->finish();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
