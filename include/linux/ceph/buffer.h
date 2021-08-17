#ifndef __FS_CEPH_BUFFER_H
#define __FS_CEPH_BUFFER_H

#include <linux/kref.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/uio.h>

/*
 * a simple reference counted buffer.
 *
<<<<<<< HEAD
 * use kmalloc for small sizes (<= one page), vmalloc for larger
 * sizes.
=======
 * use kmalloc for smaller sizes, vmalloc for larger sizes.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct ceph_buffer {
	struct kref kref;
	struct kvec vec;
	size_t alloc_len;
<<<<<<< HEAD
	bool is_vmalloc;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

extern struct ceph_buffer *ceph_buffer_new(size_t len, gfp_t gfp);
extern void ceph_buffer_release(struct kref *kref);

static inline struct ceph_buffer *ceph_buffer_get(struct ceph_buffer *b)
{
	kref_get(&b->kref);
	return b;
}

static inline void ceph_buffer_put(struct ceph_buffer *b)
{
<<<<<<< HEAD
	kref_put(&b->kref, ceph_buffer_release);
=======
	if (b)
		kref_put(&b->kref, ceph_buffer_release);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

extern int ceph_decode_buffer(struct ceph_buffer **b, void **p, void *end);

#endif
