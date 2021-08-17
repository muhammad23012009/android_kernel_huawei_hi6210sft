#ifndef _LINUX_SLAB_DEF_H
#define	_LINUX_SLAB_DEF_H

<<<<<<< HEAD
/*
 * Definitions unique to the original Linux SLAB allocator.
 *
 * What we provide here is a way to optimize the frequent kmalloc
 * calls in the kernel by selecting the appropriate general cache
 * if kmalloc was called with a size that can be established at
 * compile time.
 */

#include <linux/init.h>
#include <linux/compiler.h>

/*
 * struct kmem_cache
 *
 * manages a cache.
 */

struct kmem_cache {
/* 1) Cache tunables. Protected by cache_chain_mutex */
=======
#include <linux/reciprocal_div.h>

/*
 * Definitions unique to the original Linux SLAB allocator.
 */

struct kmem_cache {
	struct array_cache __percpu *cpu_cache;

/* 1) Cache tunables. Protected by slab_mutex */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int batchcount;
	unsigned int limit;
	unsigned int shared;

	unsigned int size;
<<<<<<< HEAD
	u32 reciprocal_buffer_size;
=======
	struct reciprocal_value reciprocal_buffer_size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* 2) touched by every alloc & free from the backend */

	unsigned int flags;		/* constant flags */
	unsigned int num;		/* # of objs per slab */

/* 3) cache_grow/shrink */
	/* order of pgs per slab (2^n) */
	unsigned int gfporder;

	/* force GFP flags, e.g. GFP_DMA */
	gfp_t allocflags;

	size_t colour;			/* cache colouring range */
	unsigned int colour_off;	/* colour offset */
<<<<<<< HEAD
	struct kmem_cache *slabp_cache;
	unsigned int slab_size;
=======
	struct kmem_cache *freelist_cache;
	unsigned int freelist_size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* constructor func */
	void (*ctor)(void *obj);

/* 4) cache creation/removal */
	const char *name;
	struct list_head list;
	int refcount;
	int object_size;
	int align;

/* 5) statistics */
#ifdef CONFIG_DEBUG_SLAB
	unsigned long num_active;
	unsigned long num_allocations;
	unsigned long high_mark;
	unsigned long grown;
	unsigned long reaped;
	unsigned long errors;
	unsigned long max_freeable;
	unsigned long node_allocs;
	unsigned long node_frees;
	unsigned long node_overflow;
	atomic_t allochit;
	atomic_t allocmiss;
	atomic_t freehit;
	atomic_t freemiss;
<<<<<<< HEAD
=======
#ifdef CONFIG_DEBUG_SLAB_LEAK
	atomic_t store_user_clean;
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * If debugging is enabled, then the allocator can add additional
	 * fields and/or padding to every object. size contains the total
	 * object size including these internal fields, the following two
	 * variables contain the offset to the user object and its size.
	 */
	int obj_offset;
#endif /* CONFIG_DEBUG_SLAB */
<<<<<<< HEAD
#ifdef CONFIG_MEMCG_KMEM
	struct memcg_cache_params *memcg_params;
#endif

/* 6) per-cpu/per-node data, touched during every alloc/free */
	/*
	 * We put array[] at the end of kmem_cache, because we want to size
	 * this array to nr_cpu_ids slots instead of NR_CPUS
	 * (see kmem_cache_init())
	 * We still use [NR_CPUS] and not [1] or [0] because cache_cache
	 * is statically defined, so we reserve the max number of cpus.
	 *
	 * We also need to guarantee that the list is able to accomodate a
	 * pointer for each node since "nodelists" uses the remainder of
	 * available pointers.
	 */
	struct kmem_cache_node **node;
	struct array_cache *array[NR_CPUS + MAX_NUMNODES];
	/*
	 * Do not add fields after array[]
	 */
};

void *kmem_cache_alloc(struct kmem_cache *, gfp_t);
void *__kmalloc(size_t size, gfp_t flags);

#ifdef CONFIG_TRACING
extern void *kmem_cache_alloc_trace(struct kmem_cache *, gfp_t, size_t);
#else
static __always_inline void *
kmem_cache_alloc_trace(struct kmem_cache *cachep, gfp_t flags, size_t size)
{
	return kmem_cache_alloc(cachep, flags);
}
#endif

static __always_inline void *kmalloc(size_t size, gfp_t flags)
{
	struct kmem_cache *cachep;
	void *ret;

	if (__builtin_constant_p(size)) {
		int i;

		if (!size)
			return ZERO_SIZE_PTR;

		if (WARN_ON_ONCE(size > KMALLOC_MAX_SIZE))
			return NULL;

		i = kmalloc_index(size);

#ifdef CONFIG_ZONE_DMA
		if (flags & GFP_DMA)
			cachep = kmalloc_dma_caches[i];
		else
#endif
			cachep = kmalloc_caches[i];

		ret = kmem_cache_alloc_trace(cachep, flags, size);

		return ret;
	}
	return __kmalloc(size, flags);
}

#ifdef CONFIG_NUMA
extern void *__kmalloc_node(size_t size, gfp_t flags, int node);
extern void *kmem_cache_alloc_node(struct kmem_cache *, gfp_t flags, int node);

#ifdef CONFIG_TRACING
extern void *kmem_cache_alloc_node_trace(struct kmem_cache *cachep,
					 gfp_t flags,
					 int nodeid,
					 size_t size);
#else
static __always_inline void *
kmem_cache_alloc_node_trace(struct kmem_cache *cachep,
			    gfp_t flags,
			    int nodeid,
			    size_t size)
{
	return kmem_cache_alloc_node(cachep, flags, nodeid);
}
#endif

static __always_inline void *kmalloc_node(size_t size, gfp_t flags, int node)
{
	struct kmem_cache *cachep;

	if (__builtin_constant_p(size)) {
		int i;

		if (!size)
			return ZERO_SIZE_PTR;

		if (WARN_ON_ONCE(size > KMALLOC_MAX_SIZE))
			return NULL;

		i = kmalloc_index(size);

#ifdef CONFIG_ZONE_DMA
		if (flags & GFP_DMA)
			cachep = kmalloc_dma_caches[i];
		else
#endif
			cachep = kmalloc_caches[i];

		return kmem_cache_alloc_node_trace(cachep, flags, node, size);
	}
	return __kmalloc_node(size, flags, node);
}

#endif	/* CONFIG_NUMA */

=======

#ifdef CONFIG_MEMCG
	struct memcg_cache_params memcg_params;
#endif
#ifdef CONFIG_KASAN
	struct kasan_cache kasan_info;
#endif

#ifdef CONFIG_SLAB_FREELIST_RANDOM
	unsigned int *random_seq;
#endif

	struct kmem_cache_node *node[MAX_NUMNODES];
};

static inline void *nearest_obj(struct kmem_cache *cache, struct page *page,
				void *x)
{
	void *object = x - (x - page->s_mem) % cache->size;
	void *last_object = page->s_mem + (cache->num - 1) * cache->size;

	if (unlikely(object > last_object))
		return last_object;
	else
		return object;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif	/* _LINUX_SLAB_DEF_H */
