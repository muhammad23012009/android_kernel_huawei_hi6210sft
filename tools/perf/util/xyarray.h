#ifndef _PERF_XYARRAY_H_
#define _PERF_XYARRAY_H_ 1

#include <sys/types.h>

struct xyarray {
	size_t row_size;
	size_t entry_size;
<<<<<<< HEAD
=======
	size_t entries;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char contents[];
};

struct xyarray *xyarray__new(int xlen, int ylen, size_t entry_size);
void xyarray__delete(struct xyarray *xy);
<<<<<<< HEAD
=======
void xyarray__reset(struct xyarray *xy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void *xyarray__entry(struct xyarray *xy, int x, int y)
{
	return &xy->contents[x * xy->row_size + y * xy->entry_size];
}

#endif /* _PERF_XYARRAY_H_ */
