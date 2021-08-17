#ifndef __UNWIND_H
#define __UNWIND_H

<<<<<<< HEAD
#include "types.h"
#include "event.h"
#include "symbol.h"
=======
#include <linux/types.h>
#include "event.h"
#include "symbol.h"
#include "thread.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct unwind_entry {
	struct map	*map;
	struct symbol	*sym;
	u64		ip;
};

typedef int (*unwind_entry_cb_t)(struct unwind_entry *entry, void *arg);

<<<<<<< HEAD
#ifdef LIBUNWIND_SUPPORT
int unwind__get_entries(unwind_entry_cb_t cb, void *arg,
			struct machine *machine,
			struct thread *thread,
			u64 sample_uregs,
			struct perf_sample *data);
int unwind__arch_reg_id(int regnum);
=======
struct unwind_libunwind_ops {
	int (*prepare_access)(struct thread *thread);
	void (*flush_access)(struct thread *thread);
	void (*finish_access)(struct thread *thread);
	int (*get_entries)(unwind_entry_cb_t cb, void *arg,
			   struct thread *thread,
			   struct perf_sample *data, int max_stack);
};

#ifdef HAVE_DWARF_UNWIND_SUPPORT
int unwind__get_entries(unwind_entry_cb_t cb, void *arg,
			struct thread *thread,
			struct perf_sample *data, int max_stack);
/* libunwind specific */
#ifdef HAVE_LIBUNWIND_SUPPORT
#ifndef LIBUNWIND__ARCH_REG_ID
#define LIBUNWIND__ARCH_REG_ID(regnum) libunwind__arch_reg_id(regnum)
#endif

#ifndef LIBUNWIND__ARCH_REG_SP
#define LIBUNWIND__ARCH_REG_SP PERF_REG_SP
#endif

#ifndef LIBUNWIND__ARCH_REG_IP
#define LIBUNWIND__ARCH_REG_IP PERF_REG_IP
#endif

int LIBUNWIND__ARCH_REG_ID(int regnum);
int unwind__prepare_access(struct thread *thread, struct map *map,
			   bool *initialized);
void unwind__flush_access(struct thread *thread);
void unwind__finish_access(struct thread *thread);
#else
static inline int unwind__prepare_access(struct thread *thread __maybe_unused,
					 struct map *map __maybe_unused,
					 bool *initialized __maybe_unused)
{
	return 0;
}

static inline void unwind__flush_access(struct thread *thread __maybe_unused) {}
static inline void unwind__finish_access(struct thread *thread __maybe_unused) {}
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
static inline int
unwind__get_entries(unwind_entry_cb_t cb __maybe_unused,
		    void *arg __maybe_unused,
<<<<<<< HEAD
		    struct machine *machine __maybe_unused,
		    struct thread *thread __maybe_unused,
		    u64 sample_uregs __maybe_unused,
		    struct perf_sample *data __maybe_unused)
{
	return 0;
}
#endif /* LIBUNWIND_SUPPORT */
=======
		    struct thread *thread __maybe_unused,
		    struct perf_sample *data __maybe_unused,
		    int max_stack __maybe_unused)
{
	return 0;
}

static inline int unwind__prepare_access(struct thread *thread __maybe_unused,
					 struct map *map __maybe_unused,
					 bool *initialized __maybe_unused)
{
	return 0;
}

static inline void unwind__flush_access(struct thread *thread __maybe_unused) {}
static inline void unwind__finish_access(struct thread *thread __maybe_unused) {}
#endif /* HAVE_DWARF_UNWIND_SUPPORT */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __UNWIND_H */
