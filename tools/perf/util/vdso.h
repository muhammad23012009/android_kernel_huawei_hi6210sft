#ifndef __PERF_VDSO__
#define __PERF_VDSO__

#include <linux/types.h>
#include <string.h>
#include <stdbool.h>

#define VDSO__MAP_NAME "[vdso]"

<<<<<<< HEAD
=======
#define DSO__NAME_VDSO    "[vdso]"
#define DSO__NAME_VDSO32  "[vdso32]"
#define DSO__NAME_VDSOX32 "[vdsox32]"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline bool is_vdso_map(const char *filename)
{
	return !strcmp(filename, VDSO__MAP_NAME);
}

<<<<<<< HEAD
struct dso *vdso__dso_findnew(struct list_head *head);
void vdso__exit(void);
=======
struct dso;

bool dso__is_vdso(struct dso *dso);

struct machine;
struct thread;

struct dso *machine__findnew_vdso(struct machine *machine, struct thread *thread);
void machine__exit_vdso(struct machine *machine);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __PERF_VDSO__ */
