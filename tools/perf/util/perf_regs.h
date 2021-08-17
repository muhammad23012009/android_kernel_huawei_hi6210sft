#ifndef __PERF_REGS_H
#define __PERF_REGS_H

<<<<<<< HEAD
#ifdef HAVE_PERF_REGS
#include <perf_regs.h>
#else
#define PERF_REGS_MASK	0

static inline const char *perf_reg_name(int id __maybe_unused)
{
	return NULL;
}
#endif /* HAVE_PERF_REGS */
=======
#include <linux/types.h>
#include <linux/compiler.h>

struct regs_dump;

struct sample_reg {
	const char *name;
	uint64_t mask;
};
#define SMPL_REG(n, b) { .name = #n, .mask = 1ULL << (b) }
#define SMPL_REG_END { .name = NULL }

extern const struct sample_reg sample_reg_masks[];

#ifdef HAVE_PERF_REGS_SUPPORT
#include <perf_regs.h>

int perf_reg_value(u64 *valp, struct regs_dump *regs, int id);

#else
#define PERF_REGS_MASK	0
#define PERF_REGS_MAX	0

static inline const char *perf_reg_name(int id __maybe_unused)
{
	return "unknown";
}

static inline int perf_reg_value(u64 *valp __maybe_unused,
				 struct regs_dump *regs __maybe_unused,
				 int id __maybe_unused)
{
	return 0;
}
#endif /* HAVE_PERF_REGS_SUPPORT */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __PERF_REGS_H */
