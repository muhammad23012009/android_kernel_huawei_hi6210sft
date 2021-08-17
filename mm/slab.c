/*
 * linux/mm/slab.c
 * Written by Mark Hemment, 1996/97.
 * (markhe@nextd.demon.co.uk)
 *
 * kmem_cache_destroy() + some cleanup - 1999 Andrea Arcangeli
 *
 * Major cleanup, different bufctl logic, per-cpu arrays
 *	(c) 2000 Manfred Spraul
 *
 * Cleanup, make the head arrays unconditional, preparation for NUMA
 * 	(c) 2002 Manfred Spraul
 *
 * An implementation of the Slab Allocator as described in outline in;
 *	UNIX Internals: The New Frontiers by Uresh Vahalia
 *	Pub: Prentice Hall	ISBN 0-13-101908-2
 * or with a little more detail in;
 *	The Slab Allocator: An Object-Caching Kernel Memory Allocator
 *	Jeff Bonwick (Sun Microsystems).
 *	Presented at: USENIX Summer 1994 Technical Conference
 *
 * The memory is organized in caches, one cache for each object type.
 * (e.g. inode_cache, dentry_cache, buffer_head, vm_area_struct)
 * Each cache consists out of many slabs (they are small (usually one
 * page long) and always contiguous), and each slab contains multiple
 * initialized objects.
 *
 * This means, that your constructor is used only for newly allocated
 * slabs and you must pass objects with the same initializations to
 * kmem_cache_free.
 *
 * Each cache can only support one memory type (GFP_DMA, GFP_HIGHMEM,
 * normal). If you need a special memory type, then must create a new
 * cache for that memory type.
 *
 * In order to reduce fragmentation, the slabs are sorted in 3 groups:
 *   full slabs with 0 free objects
 *   partial slabs
 *   empty slabs with no allocated objects
 *
 * If partial slabs exist, then new allocations come from these slabs,
 * otherwise from empty slabs or new slabs are allocated.
 *
 * kmem_cache_destroy() CAN CRASH if you try to allocate from the cache
 * during kmem_cache_destroy(). The caller must prevent concurrent allocs.
 *
 * Each cache has a short per-cpu head array, most allocs
 * and frees go into that array, and if that array overflows, then 1/2
 * of the entries in the array are given back into the global cache.
 * The head array is strictly LIFO and should improve the cache hit rates.
 * On SMP, it additionally reduces the spinlock operations.
 *
 * The c_cpuarray may not be read with enabled local interrupts -
 * it's changed with a smp_call_function().
 *
 * SMP synchronization:
 *  constructors and destructors are called without any locking.
 *  Several members in struct kmem_cache and struct slab never change, they
 *	are accessed without any locking.
 *  The per-cpu arrays are never accessed from the wrong cpu, no locking,
 *  	and local interrupts are disabled so slab code is preempt-safe.
 *  The non-constant members are protected with a per-cache irq spinlock.
 *
 * Many thanks to Mark Hemment, who wrote another per-cpu slab patch
 * in 2000 - many ideas in the current implementation are derived from
 * his patch.
 *
 * Further notes from the original documentation:
 *
 * 11 April '97.  Started multi-threading - markhe
 *	The global cache-chain is protected by the mutex 'slab_mutex'.
 *	The sem is only needed when accessing/extending the cache-chain, which
 *	can never happen inside an interrupt (kmem_cache_create(),
 *	kmem_cache_shrink() and kmem_cache_reap()).
 *
 *	At present, each engine can be growing a cache.  This should be blocked.
 *
 * 15 March 2005. NUMA slab allocator.
 *	Shai Fultheim <shai@scalex86.org>.
 *	Shobhit Dayal <shobhit@calsoftinc.com>
 *	Alok N Kataria <alokk@calsoftinc.com>
 *	Christoph Lameter <christoph@lameter.com>
 *
 *	Modified the slab allocator to be node aware on NUMA systems.
 *	Each node has its own list of partial, free and full slabs.
 *	All object allocations for a node occur from node specific slab lists.
 */

#include	<linux/slab.h>
#include	<linux/mm.h>
#include	<linux/poison.h>
#include	<linux/swap.h>
#include	<linux/cache.h>
#include	<linux/interrupt.h>
#include	<linux/init.h>
#include	<linux/compiler.h>
#include	<linux/cpuset.h>
#include	<linux/proc_fs.h>
#include	<linux/seq_file.h>
#include	<linux/notifier.h>
#include	<linux/kallsyms.h>
#include	<linux/cpu.h>
#include	<linux/sysctl.h>
#include	<linux/module.h>
#include	<linux/rcupdate.h>
#include	<linux/string.h>
#include	<linux/uaccess.h>
#include	<linux/nodemask.h>
#include	<linux/kmemleak.h>
#include	<linux/mempolicy.h>
#include	<linux/mutex.h>
#include	<linux/fault-inject.h>
#include	<linux/rtmutex.h>
#include	<linux/reciprocal_div.h>
#include	<linux/debugobjects.h>
#include	<linux/kmemcheck.h>
#include	<linux/memory.h>
#include	<linux/prefetch.h>

#include	<net/sock.h>

#include	<asm/cacheflush.h>
#include	<asm/tlbflush.h>
#include	<asm/page.h>

#include <trace/events/kmem.h>

#include	"internal.h"

#include	"slab.h"

/*
 * DEBUG	- 1 for kmem_cache_create() to honour; SLAB_RED_ZONE & SLAB_POISON.
 *		  0 for faster, smaller code (especially in the critical paths).
 *
 * STATS	- 1 to collect stats for /proc/slabinfo.
 *		  0 for faster, smaller code (especially in the critical paths).
 *
 * FORCED_DEBUG	- 1 enables SLAB_RED_ZONE and SLAB_POISON (if possible)
 */

#ifdef CONFIG_DEBUG_SLAB
#define	DEBUG		1
#define	STATS		1
#define	FORCED_DEBUG	1
#else
#define	DEBUG		0
#define	STATS		0
#define	FORCED_DEBUG	0
#endif

/* Shouldn't this be in a header file somewhere? */
#define	BYTES_PER_WORD		sizeof(void *)
#define	REDZONE_ALIGN		max(BYTES_PER_WORD, __alignof__(unsigned long long))

#ifndef ARCH_KMALLOC_FLAGS
#define ARCH_KMALLOC_FLAGS SLAB_HWCACHE_ALIGN
#endif

<<<<<<< HEAD
/*
 * true if a page was allocated from pfmemalloc reserves for network-based
 * swap
 */
static bool pfmemalloc_active __read_mostly;

/*
 * kmem_bufctl_t:
 *
 * Bufctl's are used for linking objs within a slab
 * linked offsets.
 *
 * This implementation relies on "struct page" for locating the cache &
 * slab an object belongs to.
 * This allows the bufctl structure to be small (one int), but limits
 * the number of objects a slab (not a cache) can contain when off-slab
 * bufctls are used. The limit is the size of the largest general cache
 * that does not use off-slab slabs.
 * For 32bit archs with 4 kB pages, is this 56.
 * This is not serious, as it is only for large objects, when it is unwise
 * to have too many per slab.
 * Note: This limit can be raised by introducing a general cache whose size
 * is less than 512 (PAGE_SIZE<<3), but greater than 256.
 */

typedef unsigned int kmem_bufctl_t;
#define BUFCTL_END	(((kmem_bufctl_t)(~0U))-0)
#define BUFCTL_FREE	(((kmem_bufctl_t)(~0U))-1)
#define	BUFCTL_ACTIVE	(((kmem_bufctl_t)(~0U))-2)
#define	SLAB_LIMIT	(((kmem_bufctl_t)(~0U))-3)

/*
 * struct slab_rcu
 *
 * slab_destroy on a SLAB_DESTROY_BY_RCU cache uses this structure to
 * arrange for kmem_freepages to be called via RCU.  This is useful if
 * we need to approach a kernel structure obliquely, from its address
 * obtained without the usual locking.  We can lock the structure to
 * stabilize it and check it's still at the given address, only if we
 * can be sure that the memory has not been meanwhile reused for some
 * other kind of object (which our subsystem's lock might corrupt).
 *
 * rcu_read_lock before reading the address, then rcu_read_unlock after
 * taking the spinlock within the structure expected at that address.
 */
struct slab_rcu {
	struct rcu_head head;
	struct kmem_cache *cachep;
	void *addr;
};

/*
 * struct slab
 *
 * Manages the objs in a slab. Placed either at the beginning of mem allocated
 * for a slab, or allocated from an general cache.
 * Slabs are chained into three list: fully used, partial, fully free slabs.
 */
struct slab {
	union {
		struct {
			struct list_head list;
			unsigned long colouroff;
			void *s_mem;		/* including colour offset */
			unsigned int inuse;	/* num of objs active in slab */
			kmem_bufctl_t free;
			unsigned short nodeid;
		};
		struct slab_rcu __slab_cover_slab_rcu;
	};
};
=======
#define FREELIST_BYTE_INDEX (((PAGE_SIZE >> BITS_PER_BYTE) \
				<= SLAB_OBJ_MIN_SIZE) ? 1 : 0)

#if FREELIST_BYTE_INDEX
typedef unsigned char freelist_idx_t;
#else
typedef unsigned short freelist_idx_t;
#endif

#define SLAB_OBJ_MAX_NUM ((1 << sizeof(freelist_idx_t) * BITS_PER_BYTE) - 1)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * struct array_cache
 *
 * Purpose:
 * - LIFO ordering, to hand out cache-warm objects from _alloc
 * - reduce the number of linked list operations
 * - reduce spinlock operations
 *
 * The limit is stored in the per-cpu structure to reduce the data cache
 * footprint.
 *
 */
struct array_cache {
	unsigned int avail;
	unsigned int limit;
	unsigned int batchcount;
	unsigned int touched;
<<<<<<< HEAD
	spinlock_t lock;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void *entry[];	/*
			 * Must have this definition in here for the proper
			 * alignment of array_cache. Also simplifies accessing
			 * the entries.
<<<<<<< HEAD
			 *
			 * Entries should not be directly dereferenced as
			 * entries belonging to slabs marked pfmemalloc will
			 * have the lower bits set SLAB_OBJ_PFMEMALLOC
			 */
};

#define SLAB_OBJ_PFMEMALLOC	1
static inline bool is_obj_pfmemalloc(void *objp)
{
	return (unsigned long)objp & SLAB_OBJ_PFMEMALLOC;
}

static inline void set_obj_pfmemalloc(void **objp)
{
	*objp = (void *)((unsigned long)*objp | SLAB_OBJ_PFMEMALLOC);
	return;
}

static inline void clear_obj_pfmemalloc(void **objp)
{
	*objp = (void *)((unsigned long)*objp & ~SLAB_OBJ_PFMEMALLOC);
}

/*
 * bootstrap: The caches do not work without cpuarrays anymore, but the
 * cpuarrays are allocated from the generic caches...
 */
#define BOOT_CPUCACHE_ENTRIES	1
struct arraycache_init {
	struct array_cache cache;
	void *entries[BOOT_CPUCACHE_ENTRIES];
=======
			 */
};

struct alien_cache {
	spinlock_t lock;
	struct array_cache ac;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * Need this for bootstrapping a per node allocator.
 */
<<<<<<< HEAD
#define NUM_INIT_LISTS (3 * MAX_NUMNODES)
static struct kmem_cache_node __initdata init_kmem_cache_node[NUM_INIT_LISTS];
#define	CACHE_CACHE 0
#define	SIZE_AC MAX_NUMNODES
#define	SIZE_NODE (2 * MAX_NUMNODES)
=======
#define NUM_INIT_LISTS (2 * MAX_NUMNODES)
static struct kmem_cache_node __initdata init_kmem_cache_node[NUM_INIT_LISTS];
#define	CACHE_CACHE 0
#define	SIZE_NODE (MAX_NUMNODES)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int drain_freelist(struct kmem_cache *cache,
			struct kmem_cache_node *n, int tofree);
static void free_block(struct kmem_cache *cachep, void **objpp, int len,
<<<<<<< HEAD
			int node);
static int enable_cpucache(struct kmem_cache *cachep, gfp_t gfp);
static void cache_reap(struct work_struct *unused);

static int slab_early_init = 1;

#define INDEX_AC kmalloc_index(sizeof(struct arraycache_init))
=======
			int node, struct list_head *list);
static void slabs_destroy(struct kmem_cache *cachep, struct list_head *list);
static int enable_cpucache(struct kmem_cache *cachep, gfp_t gfp);
static void cache_reap(struct work_struct *unused);

static inline void fixup_objfreelist_debug(struct kmem_cache *cachep,
						void **list);
static inline void fixup_slab_list(struct kmem_cache *cachep,
				struct kmem_cache_node *n, struct page *page,
				void **list);
static int slab_early_init = 1;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define INDEX_NODE kmalloc_index(sizeof(struct kmem_cache_node))

static void kmem_cache_node_init(struct kmem_cache_node *parent)
{
	INIT_LIST_HEAD(&parent->slabs_full);
	INIT_LIST_HEAD(&parent->slabs_partial);
	INIT_LIST_HEAD(&parent->slabs_free);
	parent->shared = NULL;
	parent->alien = NULL;
	parent->colour_next = 0;
	spin_lock_init(&parent->list_lock);
	parent->free_objects = 0;
	parent->free_touched = 0;
<<<<<<< HEAD
=======
	parent->num_slabs = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#define MAKE_LIST(cachep, listp, slab, nodeid)				\
	do {								\
		INIT_LIST_HEAD(listp);					\
<<<<<<< HEAD
		list_splice(&(cachep->node[nodeid]->slab), listp);	\
=======
		list_splice(&get_node(cachep, nodeid)->slab, listp);	\
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} while (0)

#define	MAKE_ALL_LISTS(cachep, ptr, nodeid)				\
	do {								\
	MAKE_LIST((cachep), (&(ptr)->slabs_full), slabs_full, nodeid);	\
	MAKE_LIST((cachep), (&(ptr)->slabs_partial), slabs_partial, nodeid); \
	MAKE_LIST((cachep), (&(ptr)->slabs_free), slabs_free, nodeid);	\
	} while (0)

<<<<<<< HEAD
#define CFLGS_OFF_SLAB		(0x80000000UL)
=======
#define CFLGS_OBJFREELIST_SLAB	(0x40000000UL)
#define CFLGS_OFF_SLAB		(0x80000000UL)
#define	OBJFREELIST_SLAB(x)	((x)->flags & CFLGS_OBJFREELIST_SLAB)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define	OFF_SLAB(x)	((x)->flags & CFLGS_OFF_SLAB)

#define BATCHREFILL_LIMIT	16
/*
 * Optimization question: fewer reaps means less probability for unnessary
 * cpucache drain/refill cycles.
 *
 * OTOH the cpuarrays can contain lots of objects,
 * which could lock up otherwise freeable slabs.
 */
<<<<<<< HEAD
#define REAPTIMEOUT_CPUC	(2*HZ)
#define REAPTIMEOUT_LIST3	(4*HZ)
=======
#define REAPTIMEOUT_AC		(2*HZ)
#define REAPTIMEOUT_NODE	(4*HZ)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#if STATS
#define	STATS_INC_ACTIVE(x)	((x)->num_active++)
#define	STATS_DEC_ACTIVE(x)	((x)->num_active--)
#define	STATS_INC_ALLOCED(x)	((x)->num_allocations++)
#define	STATS_INC_GROWN(x)	((x)->grown++)
#define	STATS_ADD_REAPED(x,y)	((x)->reaped += (y))
#define	STATS_SET_HIGH(x)						\
	do {								\
		if ((x)->num_active > (x)->high_mark)			\
			(x)->high_mark = (x)->num_active;		\
	} while (0)
#define	STATS_INC_ERR(x)	((x)->errors++)
#define	STATS_INC_NODEALLOCS(x)	((x)->node_allocs++)
#define	STATS_INC_NODEFREES(x)	((x)->node_frees++)
#define STATS_INC_ACOVERFLOW(x)   ((x)->node_overflow++)
#define	STATS_SET_FREEABLE(x, i)					\
	do {								\
		if ((x)->max_freeable < i)				\
			(x)->max_freeable = i;				\
	} while (0)
#define STATS_INC_ALLOCHIT(x)	atomic_inc(&(x)->allochit)
#define STATS_INC_ALLOCMISS(x)	atomic_inc(&(x)->allocmiss)
#define STATS_INC_FREEHIT(x)	atomic_inc(&(x)->freehit)
#define STATS_INC_FREEMISS(x)	atomic_inc(&(x)->freemiss)
#else
#define	STATS_INC_ACTIVE(x)	do { } while (0)
#define	STATS_DEC_ACTIVE(x)	do { } while (0)
#define	STATS_INC_ALLOCED(x)	do { } while (0)
#define	STATS_INC_GROWN(x)	do { } while (0)
#define	STATS_ADD_REAPED(x,y)	do { (void)(y); } while (0)
#define	STATS_SET_HIGH(x)	do { } while (0)
#define	STATS_INC_ERR(x)	do { } while (0)
#define	STATS_INC_NODEALLOCS(x)	do { } while (0)
#define	STATS_INC_NODEFREES(x)	do { } while (0)
#define STATS_INC_ACOVERFLOW(x)   do { } while (0)
#define	STATS_SET_FREEABLE(x, i) do { } while (0)
#define STATS_INC_ALLOCHIT(x)	do { } while (0)
#define STATS_INC_ALLOCMISS(x)	do { } while (0)
#define STATS_INC_FREEHIT(x)	do { } while (0)
#define STATS_INC_FREEMISS(x)	do { } while (0)
#endif

#if DEBUG

/*
 * memory layout of objects:
 * 0		: objp
 * 0 .. cachep->obj_offset - BYTES_PER_WORD - 1: padding. This ensures that
 * 		the end of an object is aligned with the end of the real
 * 		allocation. Catches writes behind the end of the allocation.
 * cachep->obj_offset - BYTES_PER_WORD .. cachep->obj_offset - 1:
 * 		redzone word.
 * cachep->obj_offset: The real object.
 * cachep->size - 2* BYTES_PER_WORD: redzone word [BYTES_PER_WORD long]
 * cachep->size - 1* BYTES_PER_WORD: last caller address
 *					[BYTES_PER_WORD long]
 */
static int obj_offset(struct kmem_cache *cachep)
{
	return cachep->obj_offset;
}

static unsigned long long *dbg_redzone1(struct kmem_cache *cachep, void *objp)
{
	BUG_ON(!(cachep->flags & SLAB_RED_ZONE));
	return (unsigned long long*) (objp + obj_offset(cachep) -
				      sizeof(unsigned long long));
}

static unsigned long long *dbg_redzone2(struct kmem_cache *cachep, void *objp)
{
	BUG_ON(!(cachep->flags & SLAB_RED_ZONE));
	if (cachep->flags & SLAB_STORE_USER)
		return (unsigned long long *)(objp + cachep->size -
					      sizeof(unsigned long long) -
					      REDZONE_ALIGN);
	return (unsigned long long *) (objp + cachep->size -
				       sizeof(unsigned long long));
}

static void **dbg_userword(struct kmem_cache *cachep, void *objp)
{
	BUG_ON(!(cachep->flags & SLAB_STORE_USER));
	return (void **)(objp + cachep->size - BYTES_PER_WORD);
}

#else

#define obj_offset(x)			0
#define dbg_redzone1(cachep, objp)	({BUG(); (unsigned long long *)NULL;})
#define dbg_redzone2(cachep, objp)	({BUG(); (unsigned long long *)NULL;})
#define dbg_userword(cachep, objp)	({BUG(); (void **)NULL;})

#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_DEBUG_SLAB_LEAK

static inline bool is_store_user_clean(struct kmem_cache *cachep)
{
	return atomic_read(&cachep->store_user_clean) == 1;
}

static inline void set_store_user_clean(struct kmem_cache *cachep)
{
	atomic_set(&cachep->store_user_clean, 1);
}

static inline void set_store_user_dirty(struct kmem_cache *cachep)
{
	if (is_store_user_clean(cachep))
		atomic_set(&cachep->store_user_clean, 0);
}

#else
static inline void set_store_user_dirty(struct kmem_cache *cachep) {}

#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Do not go above this order unless 0 objects fit into the slab or
 * overridden on the command line.
 */
#define	SLAB_MAX_ORDER_HI	1
#define	SLAB_MAX_ORDER_LO	0
static int slab_max_order = SLAB_MAX_ORDER_LO;
static bool slab_max_order_set __initdata;

static inline struct kmem_cache *virt_to_cache(const void *obj)
{
	struct page *page = virt_to_head_page(obj);
	return page->slab_cache;
}

<<<<<<< HEAD
static inline struct slab *virt_to_slab(const void *obj)
{
	struct page *page = virt_to_head_page(obj);

	VM_BUG_ON(!PageSlab(page));
	return page->slab_page;
}

static inline void *index_to_obj(struct kmem_cache *cache, struct slab *slab,
				 unsigned int idx)
{
	return slab->s_mem + cache->size * idx;
=======
static inline void *index_to_obj(struct kmem_cache *cache, struct page *page,
				 unsigned int idx)
{
	return page->s_mem + cache->size * idx;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * We want to avoid an expensive divide : (offset / cache->size)
 *   Using the fact that size is a constant for a particular cache,
 *   we can replace (offset / cache->size) by
 *   reciprocal_divide(offset, cache->reciprocal_buffer_size)
 */
static inline unsigned int obj_to_index(const struct kmem_cache *cache,
<<<<<<< HEAD
					const struct slab *slab, void *obj)
{
	u32 offset = (obj - slab->s_mem);
	return reciprocal_divide(offset, cache->reciprocal_buffer_size);
}

static struct arraycache_init initarray_generic =
    { {0, BOOT_CPUCACHE_ENTRIES, 1, 0} };

=======
					const struct page *page, void *obj)
{
	u32 offset = (obj - page->s_mem);
	return reciprocal_divide(offset, cache->reciprocal_buffer_size);
}

#define BOOT_CPUCACHE_ENTRIES	1
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* internal cache of cache description objs */
static struct kmem_cache kmem_cache_boot = {
	.batchcount = 1,
	.limit = BOOT_CPUCACHE_ENTRIES,
	.shared = 1,
	.size = sizeof(struct kmem_cache),
	.name = "kmem_cache",
};

<<<<<<< HEAD
#define BAD_ALIEN_MAGIC 0x01020304ul

#ifdef CONFIG_LOCKDEP

/*
 * Slab sometimes uses the kmalloc slabs to store the slab headers
 * for other slabs "off slab".
 * The locking for this is tricky in that it nests within the locks
 * of all other slabs in a few places; to deal with this special
 * locking we put on-slab caches into a separate lock-class.
 *
 * We set lock class for alien array caches which are up during init.
 * The lock annotation will be lost if all cpus of a node goes down and
 * then comes back up during hotplug
 */
static struct lock_class_key on_slab_l3_key;
static struct lock_class_key on_slab_alc_key;

static struct lock_class_key debugobj_l3_key;
static struct lock_class_key debugobj_alc_key;

static void slab_set_lock_classes(struct kmem_cache *cachep,
		struct lock_class_key *l3_key, struct lock_class_key *alc_key,
		int q)
{
	struct array_cache **alc;
	struct kmem_cache_node *n;
	int r;

	n = cachep->node[q];
	if (!n)
		return;

	lockdep_set_class(&n->list_lock, l3_key);
	alc = n->alien;
	/*
	 * FIXME: This check for BAD_ALIEN_MAGIC
	 * should go away when common slab code is taught to
	 * work even without alien caches.
	 * Currently, non NUMA code returns BAD_ALIEN_MAGIC
	 * for alloc_alien_cache,
	 */
	if (!alc || (unsigned long)alc == BAD_ALIEN_MAGIC)
		return;
	for_each_node(r) {
		if (alc[r])
			lockdep_set_class(&alc[r]->lock, alc_key);
	}
}

static void slab_set_debugobj_lock_classes_node(struct kmem_cache *cachep, int node)
{
	slab_set_lock_classes(cachep, &debugobj_l3_key, &debugobj_alc_key, node);
}

static void slab_set_debugobj_lock_classes(struct kmem_cache *cachep)
{
	int node;

	for_each_online_node(node)
		slab_set_debugobj_lock_classes_node(cachep, node);
}

static void init_node_lock_keys(int q)
{
	int i;

	if (slab_state < UP)
		return;

	for (i = 1; i <= KMALLOC_SHIFT_HIGH; i++) {
		struct kmem_cache_node *n;
		struct kmem_cache *cache = kmalloc_caches[i];

		if (!cache)
			continue;

		n = cache->node[q];
		if (!n || OFF_SLAB(cache))
			continue;

		slab_set_lock_classes(cache, &on_slab_l3_key,
				&on_slab_alc_key, q);
	}
}

static void on_slab_lock_classes_node(struct kmem_cache *cachep, int q)
{
	if (!cachep->node[q])
		return;

	slab_set_lock_classes(cachep, &on_slab_l3_key,
			&on_slab_alc_key, q);
}

static inline void on_slab_lock_classes(struct kmem_cache *cachep)
{
	int node;

	VM_BUG_ON(OFF_SLAB(cachep));
	for_each_node(node)
		on_slab_lock_classes_node(cachep, node);
}

static inline void init_lock_keys(void)
{
	int node;

	for_each_node(node)
		init_node_lock_keys(node);
}
#else
static void init_node_lock_keys(int q)
{
}

static inline void init_lock_keys(void)
{
}

static inline void on_slab_lock_classes(struct kmem_cache *cachep)
{
}

static inline void on_slab_lock_classes_node(struct kmem_cache *cachep, int node)
{
}

static void slab_set_debugobj_lock_classes_node(struct kmem_cache *cachep, int node)
{
}

static void slab_set_debugobj_lock_classes(struct kmem_cache *cachep)
{
}
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static DEFINE_PER_CPU(struct delayed_work, slab_reap_work);

static inline struct array_cache *cpu_cache_get(struct kmem_cache *cachep)
{
<<<<<<< HEAD
	return cachep->array[smp_processor_id()];
}

static size_t slab_mgmt_size(size_t nr_objs, size_t align)
{
	return ALIGN(sizeof(struct slab)+nr_objs*sizeof(kmem_bufctl_t), align);
=======
	return this_cpu_ptr(cachep->cpu_cache);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Calculate the number of objects and left-over bytes for a given buffer size.
 */
<<<<<<< HEAD
static void cache_estimate(unsigned long gfporder, size_t buffer_size,
			   size_t align, int flags, size_t *left_over,
			   unsigned int *num)
{
	int nr_objs;
	size_t mgmt_size;
=======
static unsigned int cache_estimate(unsigned long gfporder, size_t buffer_size,
		unsigned long flags, size_t *left_over)
{
	unsigned int num;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	size_t slab_size = PAGE_SIZE << gfporder;

	/*
	 * The slab management structure can be either off the slab or
	 * on it. For the latter case, the memory allocated for a
	 * slab is used for:
	 *
<<<<<<< HEAD
	 * - The struct slab
	 * - One kmem_bufctl_t for each object
	 * - Padding to respect alignment of @align
	 * - @buffer_size bytes for each object
=======
	 * - @buffer_size bytes for each object
	 * - One freelist_idx_t for each object
	 *
	 * We don't need to consider alignment of freelist because
	 * freelist will be at the end of slab page. The objects will be
	 * at the correct alignment.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 *
	 * If the slab management structure is off the slab, then the
	 * alignment will already be calculated into the size. Because
	 * the slabs are all pages aligned, the objects will be at the
	 * correct alignment when allocated.
	 */
<<<<<<< HEAD
	if (flags & CFLGS_OFF_SLAB) {
		mgmt_size = 0;
		nr_objs = slab_size / buffer_size;

		if (nr_objs > SLAB_LIMIT)
			nr_objs = SLAB_LIMIT;
	} else {
		/*
		 * Ignore padding for the initial guess. The padding
		 * is at most @align-1 bytes, and @buffer_size is at
		 * least @align. In the worst case, this result will
		 * be one greater than the number of objects that fit
		 * into the memory allocation when taking the padding
		 * into account.
		 */
		nr_objs = (slab_size - sizeof(struct slab)) /
			  (buffer_size + sizeof(kmem_bufctl_t));

		/*
		 * This calculated number will be either the right
		 * amount, or one greater than what we want.
		 */
		if (slab_mgmt_size(nr_objs, align) + nr_objs*buffer_size
		       > slab_size)
			nr_objs--;

		if (nr_objs > SLAB_LIMIT)
			nr_objs = SLAB_LIMIT;

		mgmt_size = slab_mgmt_size(nr_objs, align);
	}
	*num = nr_objs;
	*left_over = slab_size - nr_objs*buffer_size - mgmt_size;
=======
	if (flags & (CFLGS_OBJFREELIST_SLAB | CFLGS_OFF_SLAB)) {
		num = slab_size / buffer_size;
		*left_over = slab_size % buffer_size;
	} else {
		num = slab_size / (buffer_size + sizeof(freelist_idx_t));
		*left_over = slab_size %
			(buffer_size + sizeof(freelist_idx_t));
	}

	return num;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#if DEBUG
#define slab_error(cachep, msg) __slab_error(__func__, cachep, msg)

static void __slab_error(const char *function, struct kmem_cache *cachep,
			char *msg)
{
<<<<<<< HEAD
	printk(KERN_ERR "slab error in %s(): cache `%s': %s\n",
=======
	pr_err("slab error in %s(): cache `%s': %s\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	       function, cachep->name, msg);
	dump_stack();
	add_taint(TAINT_BAD_PAGE, LOCKDEP_NOW_UNRELIABLE);
}
#endif

/*
 * By default on NUMA we use alien caches to stage the freeing of
 * objects allocated from other nodes. This causes massive memory
 * inefficiencies when using fake NUMA setup to split memory into a
 * large number of small nodes, so it can be disabled on the command
 * line
  */

static int use_alien_caches __read_mostly = 1;
static int __init noaliencache_setup(char *s)
{
	use_alien_caches = 0;
	return 1;
}
__setup("noaliencache", noaliencache_setup);

static int __init slab_max_order_setup(char *str)
{
	get_option(&str, &slab_max_order);
	slab_max_order = slab_max_order < 0 ? 0 :
				min(slab_max_order, MAX_ORDER - 1);
	slab_max_order_set = true;

	return 1;
}
__setup("slab_max_order=", slab_max_order_setup);

#ifdef CONFIG_NUMA
/*
 * Special reaping functions for NUMA systems called from cache_reap().
 * These take care of doing round robin flushing of alien caches (containing
 * objects freed on different nodes from which they were allocated) and the
 * flushing of remote pcps by calling drain_node_pages.
 */
static DEFINE_PER_CPU(unsigned long, slab_reap_node);

static void init_reap_node(int cpu)
{
<<<<<<< HEAD
	int node;

	node = next_node(cpu_to_mem(cpu), node_online_map);
	if (node == MAX_NUMNODES)
		node = first_node(node_online_map);

	per_cpu(slab_reap_node, cpu) = node;
=======
	per_cpu(slab_reap_node, cpu) = next_node_in(cpu_to_mem(cpu),
						    node_online_map);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void next_reap_node(void)
{
	int node = __this_cpu_read(slab_reap_node);

<<<<<<< HEAD
	node = next_node(node, node_online_map);
	if (unlikely(node >= MAX_NUMNODES))
		node = first_node(node_online_map);
=======
	node = next_node_in(node, node_online_map);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__this_cpu_write(slab_reap_node, node);
}

#else
#define init_reap_node(cpu) do { } while (0)
#define next_reap_node(void) do { } while (0)
#endif

/*
 * Initiate the reap timer running on the target CPU.  We run at around 1 to 2Hz
 * via the workqueue/eventd.
 * Add the CPU number into the expiration time to minimize the possibility of
 * the CPUs getting into lockstep and contending for the global cache chain
 * lock.
 */
<<<<<<< HEAD
static void __cpuinit start_cpu_timer(int cpu)
=======
static void start_cpu_timer(int cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct delayed_work *reap_work = &per_cpu(slab_reap_work, cpu);

	/*
	 * When this gets called from do_initcalls via cpucache_init(),
	 * init_workqueues() has already run, so keventd will be setup
	 * at that time.
	 */
	if (keventd_up() && reap_work->work.func == NULL) {
		init_reap_node(cpu);
		INIT_DEFERRABLE_WORK(reap_work, cache_reap);
		schedule_delayed_work_on(cpu, reap_work,
					__round_jiffies_relative(HZ, cpu));
	}
}

<<<<<<< HEAD
static struct array_cache *alloc_arraycache(int node, int entries,
					    int batchcount, gfp_t gfp)
{
	int memsize = sizeof(void *) * entries + sizeof(struct array_cache);
	struct array_cache *nc = NULL;

	nc = kmalloc_node(memsize, gfp, node);
=======
static void init_arraycache(struct array_cache *ac, int limit, int batch)
{
	if (ac) {
		ac->avail = 0;
		ac->limit = limit;
		ac->batchcount = batch;
		ac->touched = 0;
	}
}

static struct array_cache *alloc_arraycache(int node, int entries,
					    int batchcount, gfp_t gfp)
{
	size_t memsize = sizeof(void *) * entries + sizeof(struct array_cache);
	struct array_cache *ac = NULL;

	ac = kmalloc_node(memsize, gfp, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * The array_cache structures contain pointers to free object.
	 * However, when such objects are allocated or transferred to another
	 * cache the pointers are not cleared and they could be counted as
	 * valid references during a kmemleak scan. Therefore, kmemleak must
	 * not scan such objects.
	 */
<<<<<<< HEAD
	kmemleak_no_scan(nc);
	if (nc) {
		nc->avail = 0;
		nc->limit = entries;
		nc->batchcount = batchcount;
		nc->touched = 0;
		spin_lock_init(&nc->lock);
	}
	return nc;
}

static inline bool is_slab_pfmemalloc(struct slab *slabp)
{
	struct page *page = virt_to_page(slabp->s_mem);

	return PageSlabPfmemalloc(page);
}

/* Clears pfmemalloc_active if no slabs have pfmalloc set */
static void recheck_pfmemalloc_active(struct kmem_cache *cachep,
						struct array_cache *ac)
{
	struct kmem_cache_node *n = cachep->node[numa_mem_id()];
	struct slab *slabp;
	unsigned long flags;

	if (!pfmemalloc_active)
		return;

	spin_lock_irqsave(&n->list_lock, flags);
	list_for_each_entry(slabp, &n->slabs_full, list)
		if (is_slab_pfmemalloc(slabp))
			goto out;

	list_for_each_entry(slabp, &n->slabs_partial, list)
		if (is_slab_pfmemalloc(slabp))
			goto out;

	list_for_each_entry(slabp, &n->slabs_free, list)
		if (is_slab_pfmemalloc(slabp))
			goto out;

	pfmemalloc_active = false;
out:
	spin_unlock_irqrestore(&n->list_lock, flags);
}

static void *__ac_get_obj(struct kmem_cache *cachep, struct array_cache *ac,
						gfp_t flags, bool force_refill)
{
	int i;
	void *objp = ac->entry[--ac->avail];

	/* Ensure the caller is allowed to use objects from PFMEMALLOC slab */
	if (unlikely(is_obj_pfmemalloc(objp))) {
		struct kmem_cache_node *n;

		if (gfp_pfmemalloc_allowed(flags)) {
			clear_obj_pfmemalloc(&objp);
			return objp;
		}

		/* The caller cannot use PFMEMALLOC objects, find another one */
		for (i = 0; i < ac->avail; i++) {
			/* If a !PFMEMALLOC object is found, swap them */
			if (!is_obj_pfmemalloc(ac->entry[i])) {
				objp = ac->entry[i];
				ac->entry[i] = ac->entry[ac->avail];
				ac->entry[ac->avail] = objp;
				return objp;
			}
		}

		/*
		 * If there are empty slabs on the slabs_free list and we are
		 * being forced to refill the cache, mark this one !pfmemalloc.
		 */
		n = cachep->node[numa_mem_id()];
		if (!list_empty(&n->slabs_free) && force_refill) {
			struct slab *slabp = virt_to_slab(objp);
			ClearPageSlabPfmemalloc(virt_to_head_page(slabp->s_mem));
			clear_obj_pfmemalloc(&objp);
			recheck_pfmemalloc_active(cachep, ac);
			return objp;
		}

		/* No !PFMEMALLOC objects available */
		ac->avail++;
		objp = NULL;
	}

	return objp;
}

static inline void *ac_get_obj(struct kmem_cache *cachep,
			struct array_cache *ac, gfp_t flags, bool force_refill)
{
	void *objp;

	if (unlikely(sk_memalloc_socks()))
		objp = __ac_get_obj(cachep, ac, flags, force_refill);
	else
		objp = ac->entry[--ac->avail];

	return objp;
}

static void *__ac_put_obj(struct kmem_cache *cachep, struct array_cache *ac,
								void *objp)
{
	if (unlikely(pfmemalloc_active)) {
		/* Some pfmemalloc slabs exist, check if this is one */
		struct page *page = virt_to_head_page(objp);
		if (PageSlabPfmemalloc(page))
			set_obj_pfmemalloc(&objp);
	}

	return objp;
}

static inline void ac_put_obj(struct kmem_cache *cachep, struct array_cache *ac,
								void *objp)
{
	if (unlikely(sk_memalloc_socks()))
		objp = __ac_put_obj(cachep, ac, objp);

	ac->entry[ac->avail++] = objp;
=======
	kmemleak_no_scan(ac);
	init_arraycache(ac, entries, batchcount);
	return ac;
}

static noinline void cache_free_pfmemalloc(struct kmem_cache *cachep,
					struct page *page, void *objp)
{
	struct kmem_cache_node *n;
	int page_node;
	LIST_HEAD(list);

	page_node = page_to_nid(page);
	n = get_node(cachep, page_node);

	spin_lock(&n->list_lock);
	free_block(cachep, &objp, 1, page_node, &list);
	spin_unlock(&n->list_lock);

	slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Transfer objects in one arraycache to another.
 * Locking must be handled by the caller.
 *
 * Return the number of entries transferred.
 */
static int transfer_objects(struct array_cache *to,
		struct array_cache *from, unsigned int max)
{
	/* Figure out how many entries to transfer */
	int nr = min3(from->avail, max, to->limit - to->avail);

	if (!nr)
		return 0;

	memcpy(to->entry + to->avail, from->entry + from->avail -nr,
			sizeof(void *) *nr);

	from->avail -= nr;
	to->avail += nr;
	return nr;
}

#ifndef CONFIG_NUMA

#define drain_alien_cache(cachep, alien) do { } while (0)
#define reap_alien(cachep, n) do { } while (0)

<<<<<<< HEAD
static inline struct array_cache **alloc_alien_cache(int node, int limit, gfp_t gfp)
{
	return (struct array_cache **)BAD_ALIEN_MAGIC;
}

static inline void free_alien_cache(struct array_cache **ac_ptr)
=======
static inline struct alien_cache **alloc_alien_cache(int node,
						int limit, gfp_t gfp)
{
	return NULL;
}

static inline void free_alien_cache(struct alien_cache **ac_ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
}

static inline int cache_free_alien(struct kmem_cache *cachep, void *objp)
{
	return 0;
}

static inline void *alternate_node_alloc(struct kmem_cache *cachep,
		gfp_t flags)
{
	return NULL;
}

static inline void *____cache_alloc_node(struct kmem_cache *cachep,
		 gfp_t flags, int nodeid)
{
	return NULL;
}

<<<<<<< HEAD
=======
static inline gfp_t gfp_exact_node(gfp_t flags)
{
	return flags & ~__GFP_NOFAIL;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else	/* CONFIG_NUMA */

static void *____cache_alloc_node(struct kmem_cache *, gfp_t, int);
static void *alternate_node_alloc(struct kmem_cache *, gfp_t);

<<<<<<< HEAD
static struct array_cache **alloc_alien_cache(int node, int limit, gfp_t gfp)
{
	struct array_cache **ac_ptr;
	int memsize = sizeof(void *) * nr_node_ids;
=======
static struct alien_cache *__alloc_alien_cache(int node, int entries,
						int batch, gfp_t gfp)
{
	size_t memsize = sizeof(void *) * entries + sizeof(struct alien_cache);
	struct alien_cache *alc = NULL;

	alc = kmalloc_node(memsize, gfp, node);
	if (alc) {
		kmemleak_no_scan(alc);
		init_arraycache(&alc->ac, entries, batch);
		spin_lock_init(&alc->lock);
	}
	return alc;
}

static struct alien_cache **alloc_alien_cache(int node, int limit, gfp_t gfp)
{
	struct alien_cache **alc_ptr;
	size_t memsize = sizeof(void *) * nr_node_ids;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i;

	if (limit > 1)
		limit = 12;
<<<<<<< HEAD
	ac_ptr = kzalloc_node(memsize, gfp, node);
	if (ac_ptr) {
		for_each_node(i) {
			if (i == node || !node_online(i))
				continue;
			ac_ptr[i] = alloc_arraycache(node, limit, 0xbaadf00d, gfp);
			if (!ac_ptr[i]) {
				for (i--; i >= 0; i--)
					kfree(ac_ptr[i]);
				kfree(ac_ptr);
				return NULL;
			}
		}
	}
	return ac_ptr;
}

static void free_alien_cache(struct array_cache **ac_ptr)
{
	int i;

	if (!ac_ptr)
		return;
	for_each_node(i)
	    kfree(ac_ptr[i]);
	kfree(ac_ptr);
}

static void __drain_alien_cache(struct kmem_cache *cachep,
				struct array_cache *ac, int node)
{
	struct kmem_cache_node *n = cachep->node[node];
=======
	alc_ptr = kzalloc_node(memsize, gfp, node);
	if (!alc_ptr)
		return NULL;

	for_each_node(i) {
		if (i == node || !node_online(i))
			continue;
		alc_ptr[i] = __alloc_alien_cache(node, limit, 0xbaadf00d, gfp);
		if (!alc_ptr[i]) {
			for (i--; i >= 0; i--)
				kfree(alc_ptr[i]);
			kfree(alc_ptr);
			return NULL;
		}
	}
	return alc_ptr;
}

static void free_alien_cache(struct alien_cache **alc_ptr)
{
	int i;

	if (!alc_ptr)
		return;
	for_each_node(i)
	    kfree(alc_ptr[i]);
	kfree(alc_ptr);
}

static void __drain_alien_cache(struct kmem_cache *cachep,
				struct array_cache *ac, int node,
				struct list_head *list)
{
	struct kmem_cache_node *n = get_node(cachep, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (ac->avail) {
		spin_lock(&n->list_lock);
		/*
		 * Stuff objects into the remote nodes shared array first.
		 * That way we could avoid the overhead of putting the objects
		 * into the free lists and getting them back later.
		 */
		if (n->shared)
			transfer_objects(n->shared, ac, ac->limit);

<<<<<<< HEAD
		free_block(cachep, ac->entry, ac->avail, node);
=======
		free_block(cachep, ac->entry, ac->avail, node, list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		ac->avail = 0;
		spin_unlock(&n->list_lock);
	}
}

/*
 * Called from cache_reap() to regularly drain alien caches round robin.
 */
static void reap_alien(struct kmem_cache *cachep, struct kmem_cache_node *n)
{
	int node = __this_cpu_read(slab_reap_node);

	if (n->alien) {
<<<<<<< HEAD
		struct array_cache *ac = n->alien[node];

		if (ac && ac->avail && spin_trylock_irq(&ac->lock)) {
			__drain_alien_cache(cachep, ac, node);
			spin_unlock_irq(&ac->lock);
=======
		struct alien_cache *alc = n->alien[node];
		struct array_cache *ac;

		if (alc) {
			ac = &alc->ac;
			if (ac->avail && spin_trylock_irq(&alc->lock)) {
				LIST_HEAD(list);

				__drain_alien_cache(cachep, ac, node, &list);
				spin_unlock_irq(&alc->lock);
				slabs_destroy(cachep, &list);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
}

static void drain_alien_cache(struct kmem_cache *cachep,
<<<<<<< HEAD
				struct array_cache **alien)
{
	int i = 0;
=======
				struct alien_cache **alien)
{
	int i = 0;
	struct alien_cache *alc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct array_cache *ac;
	unsigned long flags;

	for_each_online_node(i) {
<<<<<<< HEAD
		ac = alien[i];
		if (ac) {
			spin_lock_irqsave(&ac->lock, flags);
			__drain_alien_cache(cachep, ac, i);
			spin_unlock_irqrestore(&ac->lock, flags);
=======
		alc = alien[i];
		if (alc) {
			LIST_HEAD(list);

			ac = &alc->ac;
			spin_lock_irqsave(&alc->lock, flags);
			__drain_alien_cache(cachep, ac, i, &list);
			spin_unlock_irqrestore(&alc->lock, flags);
			slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
}

<<<<<<< HEAD
static inline int cache_free_alien(struct kmem_cache *cachep, void *objp)
{
	struct slab *slabp = virt_to_slab(objp);
	int nodeid = slabp->nodeid;
	struct kmem_cache_node *n;
	struct array_cache *alien = NULL;
	int node;

	node = numa_mem_id();

	/*
	 * Make sure we are not freeing a object from another node to the array
	 * cache on this cpu.
	 */
	if (likely(slabp->nodeid == node))
		return 0;

	n = cachep->node[node];
	STATS_INC_NODEFREES(cachep);
	if (n->alien && n->alien[nodeid]) {
		alien = n->alien[nodeid];
		spin_lock(&alien->lock);
		if (unlikely(alien->avail == alien->limit)) {
			STATS_INC_ACOVERFLOW(cachep);
			__drain_alien_cache(cachep, alien, nodeid);
		}
		ac_put_obj(cachep, alien, objp);
		spin_unlock(&alien->lock);
	} else {
		spin_lock(&(cachep->node[nodeid])->list_lock);
		free_block(cachep, &objp, 1, nodeid);
		spin_unlock(&(cachep->node[nodeid])->list_lock);
	}
	return 1;
}
#endif

/*
 * Allocates and initializes node for a node on each slab cache, used for
 * either memory or cpu hotplug.  If memory is being hot-added, the kmem_cache_node
 * will be allocated off-node since memory is not yet online for the new node.
 * When hotplugging memory or a cpu, existing node are not replaced if
 * already in use.
 *
 * Must hold slab_mutex.
 */
static int init_cache_node_node(int node)
{
	struct kmem_cache *cachep;
	struct kmem_cache_node *n;
	const int memsize = sizeof(struct kmem_cache_node);

	list_for_each_entry(cachep, &slab_caches, list) {
		/*
		 * Set up the size64 kmemlist for cpu before we can
		 * begin anything. Make sure some other cpu on this
		 * node has not already allocated this
		 */
		if (!cachep->node[node]) {
			n = kmalloc_node(memsize, GFP_KERNEL, node);
			if (!n)
				return -ENOMEM;
			kmem_cache_node_init(n);
			n->next_reap = jiffies + REAPTIMEOUT_LIST3 +
			    ((unsigned long)cachep) % REAPTIMEOUT_LIST3;

			/*
			 * The l3s don't come and go as CPUs come and
			 * go.  slab_mutex is sufficient
			 * protection here.
			 */
			cachep->node[node] = n;
		}

		spin_lock_irq(&cachep->node[node]->list_lock);
		cachep->node[node]->free_limit =
			(1 + nr_cpus_node(node)) *
			cachep->batchcount + cachep->num;
		spin_unlock_irq(&cachep->node[node]->list_lock);
	}
	return 0;
}

static void __cpuinit cpuup_canceled(long cpu)
=======
static int __cache_free_alien(struct kmem_cache *cachep, void *objp,
				int node, int page_node)
{
	struct kmem_cache_node *n;
	struct alien_cache *alien = NULL;
	struct array_cache *ac;
	LIST_HEAD(list);

	n = get_node(cachep, node);
	STATS_INC_NODEFREES(cachep);
	if (n->alien && n->alien[page_node]) {
		alien = n->alien[page_node];
		ac = &alien->ac;
		spin_lock(&alien->lock);
		if (unlikely(ac->avail == ac->limit)) {
			STATS_INC_ACOVERFLOW(cachep);
			__drain_alien_cache(cachep, ac, page_node, &list);
		}
		ac->entry[ac->avail++] = objp;
		spin_unlock(&alien->lock);
		slabs_destroy(cachep, &list);
	} else {
		n = get_node(cachep, page_node);
		spin_lock(&n->list_lock);
		free_block(cachep, &objp, 1, page_node, &list);
		spin_unlock(&n->list_lock);
		slabs_destroy(cachep, &list);
	}
	return 1;
}

static inline int cache_free_alien(struct kmem_cache *cachep, void *objp)
{
	int page_node = page_to_nid(virt_to_page(objp));
	int node = numa_mem_id();
	/*
	 * Make sure we are not freeing a object from another node to the array
	 * cache on this cpu.
	 */
	if (likely(node == page_node))
		return 0;

	return __cache_free_alien(cachep, objp, node, page_node);
}

/*
 * Construct gfp mask to allocate from a specific node but do not reclaim or
 * warn about failures.
 */
static inline gfp_t gfp_exact_node(gfp_t flags)
{
	return (flags | __GFP_THISNODE | __GFP_NOWARN) & ~(__GFP_RECLAIM|__GFP_NOFAIL);
}
#endif

static int init_cache_node(struct kmem_cache *cachep, int node, gfp_t gfp)
{
	struct kmem_cache_node *n;

	/*
	 * Set up the kmem_cache_node for cpu before we can
	 * begin anything. Make sure some other cpu on this
	 * node has not already allocated this
	 */
	n = get_node(cachep, node);
	if (n) {
		spin_lock_irq(&n->list_lock);
		n->free_limit = (1 + nr_cpus_node(node)) * cachep->batchcount +
				cachep->num;
		spin_unlock_irq(&n->list_lock);

		return 0;
	}

	n = kmalloc_node(sizeof(struct kmem_cache_node), gfp, node);
	if (!n)
		return -ENOMEM;

	kmem_cache_node_init(n);
	n->next_reap = jiffies + REAPTIMEOUT_NODE +
		    ((unsigned long)cachep) % REAPTIMEOUT_NODE;

	n->free_limit =
		(1 + nr_cpus_node(node)) * cachep->batchcount + cachep->num;

	/*
	 * The kmem_cache_nodes don't come and go as CPUs
	 * come and go.  slab_mutex is sufficient
	 * protection here.
	 */
	cachep->node[node] = n;

	return 0;
}

#if (defined(CONFIG_NUMA) && defined(CONFIG_MEMORY_HOTPLUG)) || defined(CONFIG_SMP)
/*
 * Allocates and initializes node for a node on each slab cache, used for
 * either memory or cpu hotplug.  If memory is being hot-added, the kmem_cache_node
 * will be allocated off-node since memory is not yet online for the new node.
 * When hotplugging memory or a cpu, existing node are not replaced if
 * already in use.
 *
 * Must hold slab_mutex.
 */
static int init_cache_node_node(int node)
{
	int ret;
	struct kmem_cache *cachep;

	list_for_each_entry(cachep, &slab_caches, list) {
		ret = init_cache_node(cachep, node, GFP_KERNEL);
		if (ret)
			return ret;
	}

	return 0;
}
#endif

static int setup_kmem_cache_node(struct kmem_cache *cachep,
				int node, gfp_t gfp, bool force_change)
{
	int ret = -ENOMEM;
	struct kmem_cache_node *n;
	struct array_cache *old_shared = NULL;
	struct array_cache *new_shared = NULL;
	struct alien_cache **new_alien = NULL;
	LIST_HEAD(list);

	if (use_alien_caches) {
		new_alien = alloc_alien_cache(node, cachep->limit, gfp);
		if (!new_alien)
			goto fail;
	}

	if (cachep->shared) {
		new_shared = alloc_arraycache(node,
			cachep->shared * cachep->batchcount, 0xbaadf00d, gfp);
		if (!new_shared)
			goto fail;
	}

	ret = init_cache_node(cachep, node, gfp);
	if (ret)
		goto fail;

	n = get_node(cachep, node);
	spin_lock_irq(&n->list_lock);
	if (n->shared && force_change) {
		free_block(cachep, n->shared->entry,
				n->shared->avail, node, &list);
		n->shared->avail = 0;
	}

	if (!n->shared || force_change) {
		old_shared = n->shared;
		n->shared = new_shared;
		new_shared = NULL;
	}

	if (!n->alien) {
		n->alien = new_alien;
		new_alien = NULL;
	}

	spin_unlock_irq(&n->list_lock);
	slabs_destroy(cachep, &list);

	/*
	 * To protect lockless access to n->shared during irq disabled context.
	 * If n->shared isn't NULL in irq disabled context, accessing to it is
	 * guaranteed to be valid until irq is re-enabled, because it will be
	 * freed after synchronize_sched().
	 */
	if (old_shared && force_change)
		synchronize_sched();

fail:
	kfree(old_shared);
	kfree(new_shared);
	free_alien_cache(new_alien);

	return ret;
}

#ifdef CONFIG_SMP

static void cpuup_canceled(long cpu)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct kmem_cache *cachep;
	struct kmem_cache_node *n = NULL;
	int node = cpu_to_mem(cpu);
	const struct cpumask *mask = cpumask_of_node(node);

	list_for_each_entry(cachep, &slab_caches, list) {
		struct array_cache *nc;
		struct array_cache *shared;
<<<<<<< HEAD
		struct array_cache **alien;

		/* cpu is dead; no one can alloc from it. */
		nc = cachep->array[cpu];
		cachep->array[cpu] = NULL;
		n = cachep->node[node];

		if (!n)
			goto free_array_cache;
=======
		struct alien_cache **alien;
		LIST_HEAD(list);

		n = get_node(cachep, node);
		if (!n)
			continue;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		spin_lock_irq(&n->list_lock);

		/* Free limit for this kmem_cache_node */
		n->free_limit -= cachep->batchcount;
<<<<<<< HEAD
		if (nc)
			free_block(cachep, nc->entry, nc->avail, node);

		if (!cpumask_empty(mask)) {
			spin_unlock_irq(&n->list_lock);
			goto free_array_cache;
=======

		/* cpu is dead; no one can alloc from it. */
		nc = per_cpu_ptr(cachep->cpu_cache, cpu);
		if (nc) {
			free_block(cachep, nc->entry, nc->avail, node, &list);
			nc->avail = 0;
		}

		if (!cpumask_empty(mask)) {
			spin_unlock_irq(&n->list_lock);
			goto free_slab;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		shared = n->shared;
		if (shared) {
			free_block(cachep, shared->entry,
<<<<<<< HEAD
				   shared->avail, node);
=======
				   shared->avail, node, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			n->shared = NULL;
		}

		alien = n->alien;
		n->alien = NULL;

		spin_unlock_irq(&n->list_lock);

		kfree(shared);
		if (alien) {
			drain_alien_cache(cachep, alien);
			free_alien_cache(alien);
		}
<<<<<<< HEAD
free_array_cache:
		kfree(nc);
=======

free_slab:
		slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	/*
	 * In the previous loop, all the objects were freed to
	 * the respective cache's slabs,  now we can go ahead and
	 * shrink each nodelist to its limit.
	 */
	list_for_each_entry(cachep, &slab_caches, list) {
<<<<<<< HEAD
		n = cachep->node[node];
		if (!n)
			continue;
		drain_freelist(cachep, n, n->free_objects);
	}
}

static int __cpuinit cpuup_prepare(long cpu)
{
	struct kmem_cache *cachep;
	struct kmem_cache_node *n = NULL;
=======
		n = get_node(cachep, node);
		if (!n)
			continue;
		drain_freelist(cachep, n, INT_MAX);
	}
}

static int cpuup_prepare(long cpu)
{
	struct kmem_cache *cachep;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int node = cpu_to_mem(cpu);
	int err;

	/*
	 * We need to do this right in the beginning since
	 * alloc_arraycache's are going to use this list.
	 * kmalloc_node allows us to add the slab to the right
	 * kmem_cache_node and not this cpu's kmem_cache_node
	 */
	err = init_cache_node_node(node);
	if (err < 0)
		goto bad;

	/*
	 * Now we can go ahead with allocating the shared arrays and
	 * array caches
	 */
	list_for_each_entry(cachep, &slab_caches, list) {
<<<<<<< HEAD
		struct array_cache *nc;
		struct array_cache *shared = NULL;
		struct array_cache **alien = NULL;

		nc = alloc_arraycache(node, cachep->limit,
					cachep->batchcount, GFP_KERNEL);
		if (!nc)
			goto bad;
		if (cachep->shared) {
			shared = alloc_arraycache(node,
				cachep->shared * cachep->batchcount,
				0xbaadf00d, GFP_KERNEL);
			if (!shared) {
				kfree(nc);
				goto bad;
			}
		}
		if (use_alien_caches) {
			alien = alloc_alien_cache(node, cachep->limit, GFP_KERNEL);
			if (!alien) {
				kfree(shared);
				kfree(nc);
				goto bad;
			}
		}
		cachep->array[cpu] = nc;
		n = cachep->node[node];
		BUG_ON(!n);

		spin_lock_irq(&n->list_lock);
		if (!n->shared) {
			/*
			 * We are serialised from CPU_DEAD or
			 * CPU_UP_CANCELLED by the cpucontrol lock
			 */
			n->shared = shared;
			shared = NULL;
		}
#ifdef CONFIG_NUMA
		if (!n->alien) {
			n->alien = alien;
			alien = NULL;
		}
#endif
		spin_unlock_irq(&n->list_lock);
		kfree(shared);
		free_alien_cache(alien);
		if (cachep->flags & SLAB_DEBUG_OBJECTS)
			slab_set_debugobj_lock_classes_node(cachep, node);
		else if (!OFF_SLAB(cachep) &&
			 !(cachep->flags & SLAB_DESTROY_BY_RCU))
			on_slab_lock_classes_node(cachep, node);
	}
	init_node_lock_keys(node);
=======
		err = setup_kmem_cache_node(cachep, node, GFP_KERNEL, false);
		if (err)
			goto bad;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
bad:
	cpuup_canceled(cpu);
	return -ENOMEM;
}

<<<<<<< HEAD
static int __cpuinit cpuup_callback(struct notifier_block *nfb,
				    unsigned long action, void *hcpu)
{
	long cpu = (long)hcpu;
	int err = 0;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		mutex_lock(&slab_mutex);
		err = cpuup_prepare(cpu);
		mutex_unlock(&slab_mutex);
		break;
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		start_cpu_timer(cpu);
		break;
#ifdef CONFIG_HOTPLUG_CPU
  	case CPU_DOWN_PREPARE:
  	case CPU_DOWN_PREPARE_FROZEN:
		/*
		 * Shutdown cache reaper. Note that the slab_mutex is
		 * held so that if cache_reap() is invoked it cannot do
		 * anything expensive but will only modify reap_work
		 * and reschedule the timer.
		*/
		cancel_delayed_work_sync(&per_cpu(slab_reap_work, cpu));
		/* Now the cache_reaper is guaranteed to be not running. */
		per_cpu(slab_reap_work, cpu).work.func = NULL;
  		break;
  	case CPU_DOWN_FAILED:
  	case CPU_DOWN_FAILED_FROZEN:
		start_cpu_timer(cpu);
  		break;
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		/*
		 * Even if all the cpus of a node are down, we don't free the
		 * kmem_cache_node of any cache. This to avoid a race between
		 * cpu_down, and a kmalloc allocation from another cpu for
		 * memory from the node of the cpu going down.  The node
		 * structure is usually allocated from kmem_cache_create() and
		 * gets destroyed at kmem_cache_destroy().
		 */
		/* fall through */
#endif
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
		mutex_lock(&slab_mutex);
		cpuup_canceled(cpu);
		mutex_unlock(&slab_mutex);
		break;
	}
	return notifier_from_errno(err);
}

static struct notifier_block __cpuinitdata cpucache_notifier = {
	&cpuup_callback, NULL, 0
};
=======
int slab_prepare_cpu(unsigned int cpu)
{
	int err;

	mutex_lock(&slab_mutex);
	err = cpuup_prepare(cpu);
	mutex_unlock(&slab_mutex);
	return err;
}

/*
 * This is called for a failed online attempt and for a successful
 * offline.
 *
 * Even if all the cpus of a node are down, we don't free the
 * kmem_list3 of any cache. This to avoid a race between cpu_down, and
 * a kmalloc allocation from another cpu for memory from the node of
 * the cpu going down.  The list3 structure is usually allocated from
 * kmem_cache_create() and gets destroyed at kmem_cache_destroy().
 */
int slab_dead_cpu(unsigned int cpu)
{
	mutex_lock(&slab_mutex);
	cpuup_canceled(cpu);
	mutex_unlock(&slab_mutex);
	return 0;
}
#endif

static int slab_online_cpu(unsigned int cpu)
{
	start_cpu_timer(cpu);
	return 0;
}

static int slab_offline_cpu(unsigned int cpu)
{
	/*
	 * Shutdown cache reaper. Note that the slab_mutex is held so
	 * that if cache_reap() is invoked it cannot do anything
	 * expensive but will only modify reap_work and reschedule the
	 * timer.
	 */
	cancel_delayed_work_sync(&per_cpu(slab_reap_work, cpu));
	/* Now the cache_reaper is guaranteed to be not running. */
	per_cpu(slab_reap_work, cpu).work.func = NULL;
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#if defined(CONFIG_NUMA) && defined(CONFIG_MEMORY_HOTPLUG)
/*
 * Drains freelist for a node on each slab cache, used for memory hot-remove.
 * Returns -EBUSY if all objects cannot be drained so that the node is not
 * removed.
 *
 * Must hold slab_mutex.
 */
static int __meminit drain_cache_node_node(int node)
{
	struct kmem_cache *cachep;
	int ret = 0;

	list_for_each_entry(cachep, &slab_caches, list) {
		struct kmem_cache_node *n;

<<<<<<< HEAD
		n = cachep->node[node];
		if (!n)
			continue;

		drain_freelist(cachep, n, n->free_objects);
=======
		n = get_node(cachep, node);
		if (!n)
			continue;

		drain_freelist(cachep, n, INT_MAX);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (!list_empty(&n->slabs_full) ||
		    !list_empty(&n->slabs_partial)) {
			ret = -EBUSY;
			break;
		}
	}
	return ret;
}

static int __meminit slab_memory_callback(struct notifier_block *self,
					unsigned long action, void *arg)
{
	struct memory_notify *mnb = arg;
	int ret = 0;
	int nid;

	nid = mnb->status_change_nid;
	if (nid < 0)
		goto out;

	switch (action) {
	case MEM_GOING_ONLINE:
		mutex_lock(&slab_mutex);
		ret = init_cache_node_node(nid);
		mutex_unlock(&slab_mutex);
		break;
	case MEM_GOING_OFFLINE:
		mutex_lock(&slab_mutex);
		ret = drain_cache_node_node(nid);
		mutex_unlock(&slab_mutex);
		break;
	case MEM_ONLINE:
	case MEM_OFFLINE:
	case MEM_CANCEL_ONLINE:
	case MEM_CANCEL_OFFLINE:
		break;
	}
out:
	return notifier_from_errno(ret);
}
#endif /* CONFIG_NUMA && CONFIG_MEMORY_HOTPLUG */

/*
 * swap the static kmem_cache_node with kmalloced memory
 */
static void __init init_list(struct kmem_cache *cachep, struct kmem_cache_node *list,
				int nodeid)
{
	struct kmem_cache_node *ptr;

	ptr = kmalloc_node(sizeof(struct kmem_cache_node), GFP_NOWAIT, nodeid);
	BUG_ON(!ptr);

	memcpy(ptr, list, sizeof(struct kmem_cache_node));
	/*
	 * Do not assume that spinlocks can be initialized via memcpy:
	 */
	spin_lock_init(&ptr->list_lock);

	MAKE_ALL_LISTS(cachep, ptr, nodeid);
	cachep->node[nodeid] = ptr;
}

/*
 * For setting up all the kmem_cache_node for cache whose buffer_size is same as
 * size of kmem_cache_node.
 */
static void __init set_up_node(struct kmem_cache *cachep, int index)
{
	int node;

	for_each_online_node(node) {
		cachep->node[node] = &init_kmem_cache_node[index + node];
		cachep->node[node]->next_reap = jiffies +
<<<<<<< HEAD
		    REAPTIMEOUT_LIST3 +
		    ((unsigned long)cachep) % REAPTIMEOUT_LIST3;
=======
		    REAPTIMEOUT_NODE +
		    ((unsigned long)cachep) % REAPTIMEOUT_NODE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/*
<<<<<<< HEAD
 * The memory after the last cpu cache pointer is used for the
 * the node pointer.
 */
static void setup_node_pointer(struct kmem_cache *cachep)
{
	cachep->node = (struct kmem_cache_node **)&cachep->array[nr_cpu_ids];
}

/*
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Initialisation.  Called after the page allocator have been initialised and
 * before smp_init().
 */
void __init kmem_cache_init(void)
{
	int i;

<<<<<<< HEAD
	kmem_cache = &kmem_cache_boot;
	setup_node_pointer(kmem_cache);

	if (num_possible_nodes() == 1)
=======
	BUILD_BUG_ON(sizeof(((struct page *)NULL)->lru) <
					sizeof(struct rcu_head));
	kmem_cache = &kmem_cache_boot;

	if (!IS_ENABLED(CONFIG_NUMA) || num_possible_nodes() == 1)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		use_alien_caches = 0;

	for (i = 0; i < NUM_INIT_LISTS; i++)
		kmem_cache_node_init(&init_kmem_cache_node[i]);

<<<<<<< HEAD
	set_up_node(kmem_cache, CACHE_CACHE);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * Fragmentation resistance on low memory - only use bigger
	 * page orders on machines with more than 32MB of memory if
	 * not overridden on the command line.
	 */
	if (!slab_max_order_set && totalram_pages > (32 << 20) >> PAGE_SHIFT)
		slab_max_order = SLAB_MAX_ORDER_HI;

	/* Bootstrap is tricky, because several objects are allocated
	 * from caches that do not exist yet:
	 * 1) initialize the kmem_cache cache: it contains the struct
	 *    kmem_cache structures of all caches, except kmem_cache itself:
	 *    kmem_cache is statically allocated.
	 *    Initially an __init data area is used for the head array and the
	 *    kmem_cache_node structures, it's replaced with a kmalloc allocated
	 *    array at the end of the bootstrap.
	 * 2) Create the first kmalloc cache.
	 *    The struct kmem_cache for the new cache is allocated normally.
	 *    An __init data area is used for the head array.
	 * 3) Create the remaining kmalloc caches, with minimally sized
	 *    head arrays.
	 * 4) Replace the __init data head arrays for kmem_cache and the first
	 *    kmalloc cache with kmalloc allocated arrays.
	 * 5) Replace the __init data for kmem_cache_node for kmem_cache and
	 *    the other cache's with kmalloc allocated memory.
	 * 6) Resize the head arrays of the kmalloc caches to their final sizes.
	 */

	/* 1) create the kmem_cache */

	/*
	 * struct kmem_cache size depends on nr_node_ids & nr_cpu_ids
	 */
	create_boot_cache(kmem_cache, "kmem_cache",
<<<<<<< HEAD
		offsetof(struct kmem_cache, array[nr_cpu_ids]) +
				  nr_node_ids * sizeof(struct kmem_cache_node *),
				  SLAB_HWCACHE_ALIGN);
	list_add(&kmem_cache->list, &slab_caches);

	/* 2+3) create the kmalloc caches */

	/*
	 * Initialize the caches that provide memory for the array cache and the
	 * kmem_cache_node structures first.  Without this, further allocations will
	 * bug.
	 */

	kmalloc_caches[INDEX_AC] = create_kmalloc_cache("kmalloc-ac",
					kmalloc_size(INDEX_AC), ARCH_KMALLOC_FLAGS);

	if (INDEX_AC != INDEX_NODE)
		kmalloc_caches[INDEX_NODE] =
			create_kmalloc_cache("kmalloc-node",
				kmalloc_size(INDEX_NODE), ARCH_KMALLOC_FLAGS);

	slab_early_init = 0;

	/* 4) Replace the bootstrap head arrays */
	{
		struct array_cache *ptr;

		ptr = kmalloc(sizeof(struct arraycache_init), GFP_NOWAIT);

		memcpy(ptr, cpu_cache_get(kmem_cache),
		       sizeof(struct arraycache_init));
		/*
		 * Do not assume that spinlocks can be initialized via memcpy:
		 */
		spin_lock_init(&ptr->lock);

		kmem_cache->array[smp_processor_id()] = ptr;

		ptr = kmalloc(sizeof(struct arraycache_init), GFP_NOWAIT);

		BUG_ON(cpu_cache_get(kmalloc_caches[INDEX_AC])
		       != &initarray_generic.cache);
		memcpy(ptr, cpu_cache_get(kmalloc_caches[INDEX_AC]),
		       sizeof(struct arraycache_init));
		/*
		 * Do not assume that spinlocks can be initialized via memcpy:
		 */
		spin_lock_init(&ptr->lock);

		kmalloc_caches[INDEX_AC]->array[smp_processor_id()] = ptr;
	}
=======
		offsetof(struct kmem_cache, node) +
				  nr_node_ids * sizeof(struct kmem_cache_node *),
				  SLAB_HWCACHE_ALIGN);
	list_add(&kmem_cache->list, &slab_caches);
	slab_state = PARTIAL;

	/*
	 * Initialize the caches that provide memory for the  kmem_cache_node
	 * structures first.  Without this, further allocations will bug.
	 */
	kmalloc_caches[INDEX_NODE] = create_kmalloc_cache("kmalloc-node",
				kmalloc_size(INDEX_NODE), ARCH_KMALLOC_FLAGS);
	slab_state = PARTIAL_NODE;
	setup_kmalloc_cache_index_table();

	slab_early_init = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* 5) Replace the bootstrap kmem_cache_node */
	{
		int nid;

		for_each_online_node(nid) {
			init_list(kmem_cache, &init_kmem_cache_node[CACHE_CACHE + nid], nid);

<<<<<<< HEAD
			init_list(kmalloc_caches[INDEX_AC],
				  &init_kmem_cache_node[SIZE_AC + nid], nid);

			if (INDEX_AC != INDEX_NODE) {
				init_list(kmalloc_caches[INDEX_NODE],
					  &init_kmem_cache_node[SIZE_NODE + nid], nid);
			}
=======
			init_list(kmalloc_caches[INDEX_NODE],
					  &init_kmem_cache_node[SIZE_NODE + nid], nid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}

	create_kmalloc_caches(ARCH_KMALLOC_FLAGS);
}

void __init kmem_cache_init_late(void)
{
	struct kmem_cache *cachep;

	slab_state = UP;

	/* 6) resize the head arrays to their final sizes */
	mutex_lock(&slab_mutex);
	list_for_each_entry(cachep, &slab_caches, list)
		if (enable_cpucache(cachep, GFP_NOWAIT))
			BUG();
	mutex_unlock(&slab_mutex);

<<<<<<< HEAD
	/* Annotate slab for lockdep -- annotate the malloc caches */
	init_lock_keys();

	/* Done! */
	slab_state = FULL;

	/*
	 * Register a cpu startup notifier callback that initializes
	 * cpu_cache_get for all new cpus
	 */
	register_cpu_notifier(&cpucache_notifier);

=======
	/* Done! */
	slab_state = FULL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_NUMA
	/*
	 * Register a memory hotplug callback that initializes and frees
	 * node.
	 */
	hotplug_memory_notifier(slab_memory_callback, SLAB_CALLBACK_PRI);
#endif

	/*
	 * The reap timers are started later, with a module init call: That part
	 * of the kernel is not yet operational.
	 */
}

static int __init cpucache_init(void)
{
<<<<<<< HEAD
	int cpu;
=======
	int ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Register the timers that return unneeded pages to the page allocator
	 */
<<<<<<< HEAD
	for_each_online_cpu(cpu)
		start_cpu_timer(cpu);
=======
	ret = cpuhp_setup_state(CPUHP_AP_ONLINE_DYN, "SLAB online",
				slab_online_cpu, slab_offline_cpu);
	WARN_ON(ret < 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Done! */
	slab_state = FULL;
	return 0;
}
__initcall(cpucache_init);

static noinline void
slab_out_of_memory(struct kmem_cache *cachep, gfp_t gfpflags, int nodeid)
{
<<<<<<< HEAD
	struct kmem_cache_node *n;
	struct slab *slabp;
	unsigned long flags;
	int node;

	printk(KERN_WARNING
		"SLAB: Unable to allocate memory on node %d (gfp=0x%x)\n",
		nodeid, gfpflags);
	printk(KERN_WARNING "  cache: %s, object size: %d, order: %d\n",
		cachep->name, cachep->size, cachep->gfporder);

	for_each_online_node(node) {
		unsigned long active_objs = 0, num_objs = 0, free_objects = 0;
		unsigned long active_slabs = 0, num_slabs = 0;

		n = cachep->node[node];
		if (!n)
			continue;

		spin_lock_irqsave(&n->list_lock, flags);
		list_for_each_entry(slabp, &n->slabs_full, list) {
			active_objs += cachep->num;
			active_slabs++;
		}
		list_for_each_entry(slabp, &n->slabs_partial, list) {
			active_objs += slabp->inuse;
			active_slabs++;
		}
		list_for_each_entry(slabp, &n->slabs_free, list)
			num_slabs++;
=======
#if DEBUG
	struct kmem_cache_node *n;
	struct page *page;
	unsigned long flags;
	int node;
	static DEFINE_RATELIMIT_STATE(slab_oom_rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);

	if ((gfpflags & __GFP_NOWARN) || !__ratelimit(&slab_oom_rs))
		return;

	pr_warn("SLAB: Unable to allocate memory on node %d, gfp=%#x(%pGg)\n",
		nodeid, gfpflags, &gfpflags);
	pr_warn("  cache: %s, object size: %d, order: %d\n",
		cachep->name, cachep->size, cachep->gfporder);

	for_each_kmem_cache_node(cachep, node, n) {
		unsigned long active_objs = 0, num_objs = 0, free_objects = 0;
		unsigned long active_slabs = 0, num_slabs = 0;
		unsigned long num_slabs_partial = 0, num_slabs_free = 0;
		unsigned long num_slabs_full;

		spin_lock_irqsave(&n->list_lock, flags);
		num_slabs = n->num_slabs;
		list_for_each_entry(page, &n->slabs_partial, lru) {
			active_objs += page->active;
			num_slabs_partial++;
		}
		list_for_each_entry(page, &n->slabs_free, lru)
			num_slabs_free++;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		free_objects += n->free_objects;
		spin_unlock_irqrestore(&n->list_lock, flags);

<<<<<<< HEAD
		num_slabs += active_slabs;
		num_objs = num_slabs * cachep->num;
		printk(KERN_WARNING
			"  node %d: slabs: %ld/%ld, objs: %ld/%ld, free: %ld\n",
			node, active_slabs, num_slabs, active_objs, num_objs,
			free_objects);
	}
}

/*
 * Interface to system's page allocator. No need to hold the cache-lock.
=======
		num_objs = num_slabs * cachep->num;
		active_slabs = num_slabs - num_slabs_free;
		num_slabs_full = num_slabs -
			(num_slabs_partial + num_slabs_free);
		active_objs += (num_slabs_full * cachep->num);

		pr_warn("  node %d: slabs: %ld/%ld, objs: %ld/%ld, free: %ld\n",
			node, active_slabs, num_slabs, active_objs, num_objs,
			free_objects);
	}
#endif
}

/*
 * Interface to system's page allocator. No need to hold the
 * kmem_cache_node ->list_lock.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * If we requested dmaable memory, we will get it. Even if we
 * did not request dmaable memory, we might get it, but that
 * would be relatively rare and ignorable.
 */
<<<<<<< HEAD
static void *kmem_getpages(struct kmem_cache *cachep, gfp_t flags, int nodeid)
{
	struct page *page;
	int nr_pages;
	int i;

#ifndef CONFIG_MMU
	/*
	 * Nommu uses slab's for process anonymous memory allocations, and thus
	 * requires __GFP_COMP to properly refcount higher order allocations
	 */
	flags |= __GFP_COMP;
#endif
=======
static struct page *kmem_getpages(struct kmem_cache *cachep, gfp_t flags,
								int nodeid)
{
	struct page *page;
	int nr_pages;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	flags |= cachep->allocflags;
	if (cachep->flags & SLAB_RECLAIM_ACCOUNT)
		flags |= __GFP_RECLAIMABLE;

<<<<<<< HEAD
	page = alloc_pages_exact_node(nodeid, flags | __GFP_NOTRACK, cachep->gfporder);
	if (!page) {
		if (!(flags & __GFP_NOWARN) && printk_ratelimit())
			slab_out_of_memory(cachep, flags, nodeid);
		return NULL;
	}

	/* Record if ALLOC_NO_WATERMARKS was set when allocating the slab */
	if (unlikely(page->pfmemalloc))
		pfmemalloc_active = true;
=======
	page = __alloc_pages_node(nodeid, flags | __GFP_NOTRACK, cachep->gfporder);
	if (!page) {
		slab_out_of_memory(cachep, flags, nodeid);
		return NULL;
	}

	if (memcg_charge_slab(page, flags, cachep->gfporder, cachep)) {
		__free_pages(page, cachep->gfporder);
		return NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	nr_pages = (1 << cachep->gfporder);
	if (cachep->flags & SLAB_RECLAIM_ACCOUNT)
		add_zone_page_state(page_zone(page),
			NR_SLAB_RECLAIMABLE, nr_pages);
	else
		add_zone_page_state(page_zone(page),
			NR_SLAB_UNRECLAIMABLE, nr_pages);
<<<<<<< HEAD
	for (i = 0; i < nr_pages; i++) {
		__SetPageSlab(page + i);

		if (page->pfmemalloc)
			SetPageSlabPfmemalloc(page + i);
	}
	memcg_bind_pages(cachep, cachep->gfporder);
=======

	__SetPageSlab(page);
	/* Record if ALLOC_NO_WATERMARKS was set when allocating the slab */
	if (sk_memalloc_socks() && page_is_pfmemalloc(page))
		SetPageSlabPfmemalloc(page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (kmemcheck_enabled && !(cachep->flags & SLAB_NOTRACK)) {
		kmemcheck_alloc_shadow(page, cachep->gfporder, flags, nodeid);

		if (cachep->ctor)
			kmemcheck_mark_uninitialized_pages(page, nr_pages);
		else
			kmemcheck_mark_unallocated_pages(page, nr_pages);
	}

<<<<<<< HEAD
	return page_address(page);
=======
	return page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Interface to system's page release.
 */
<<<<<<< HEAD
static void kmem_freepages(struct kmem_cache *cachep, void *addr)
{
	unsigned long i = (1 << cachep->gfporder);
	struct page *page = virt_to_page(addr);
	const unsigned long nr_freed = i;

	kmemcheck_free_shadow(page, cachep->gfporder);
=======
static void kmem_freepages(struct kmem_cache *cachep, struct page *page)
{
	int order = cachep->gfporder;
	unsigned long nr_freed = (1 << order);

	kmemcheck_free_shadow(page, order);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (cachep->flags & SLAB_RECLAIM_ACCOUNT)
		sub_zone_page_state(page_zone(page),
				NR_SLAB_RECLAIMABLE, nr_freed);
	else
		sub_zone_page_state(page_zone(page),
				NR_SLAB_UNRECLAIMABLE, nr_freed);
<<<<<<< HEAD
	while (i--) {
		BUG_ON(!PageSlab(page));
		__ClearPageSlabPfmemalloc(page);
		__ClearPageSlab(page);
		page++;
	}

	memcg_release_pages(cachep, cachep->gfporder);
	if (current->reclaim_state)
		current->reclaim_state->reclaimed_slab += nr_freed;
	free_memcg_kmem_pages((unsigned long)addr, cachep->gfporder);
=======

	BUG_ON(!PageSlab(page));
	__ClearPageSlabPfmemalloc(page);
	__ClearPageSlab(page);
	page_mapcount_reset(page);
	page->mapping = NULL;

	if (current->reclaim_state)
		current->reclaim_state->reclaimed_slab += nr_freed;
	memcg_uncharge_slab(page, order, cachep);
	__free_pages(page, order);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void kmem_rcu_free(struct rcu_head *head)
{
<<<<<<< HEAD
	struct slab_rcu *slab_rcu = (struct slab_rcu *)head;
	struct kmem_cache *cachep = slab_rcu->cachep;

	kmem_freepages(cachep, slab_rcu->addr);
	if (OFF_SLAB(cachep))
		kmem_cache_free(cachep->slabp_cache, slab_rcu);
}

#if DEBUG
=======
	struct kmem_cache *cachep;
	struct page *page;

	page = container_of(head, struct page, rcu_head);
	cachep = page->slab_cache;

	kmem_freepages(cachep, page);
}

#if DEBUG
static bool is_debug_pagealloc_cache(struct kmem_cache *cachep)
{
	if (debug_pagealloc_enabled() && OFF_SLAB(cachep) &&
		(cachep->size % PAGE_SIZE) == 0)
		return true;

	return false;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_DEBUG_PAGEALLOC
static void store_stackinfo(struct kmem_cache *cachep, unsigned long *addr,
			    unsigned long caller)
{
	int size = cachep->object_size;

	addr = (unsigned long *)&((char *)addr)[obj_offset(cachep)];

	if (size < 5 * sizeof(unsigned long))
		return;

	*addr++ = 0x12345678;
	*addr++ = caller;
	*addr++ = smp_processor_id();
	size -= 3 * sizeof(unsigned long);
	{
		unsigned long *sptr = &caller;
		unsigned long svalue;

		while (!kstack_end(sptr)) {
			svalue = *sptr++;
			if (kernel_text_address(svalue)) {
				*addr++ = svalue;
				size -= sizeof(unsigned long);
				if (size <= sizeof(unsigned long))
					break;
			}
		}

	}
	*addr++ = 0x87654321;
}
<<<<<<< HEAD
=======

static void slab_kernel_map(struct kmem_cache *cachep, void *objp,
				int map, unsigned long caller)
{
	if (!is_debug_pagealloc_cache(cachep))
		return;

	if (caller)
		store_stackinfo(cachep, objp, caller);

	kernel_map_pages(virt_to_page(objp), cachep->size / PAGE_SIZE, map);
}

#else
static inline void slab_kernel_map(struct kmem_cache *cachep, void *objp,
				int map, unsigned long caller) {}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

static void poison_obj(struct kmem_cache *cachep, void *addr, unsigned char val)
{
	int size = cachep->object_size;
	addr = &((char *)addr)[obj_offset(cachep)];

	memset(addr, val, size);
	*(unsigned char *)(addr + size - 1) = POISON_END;
}

static void dump_line(char *data, int offset, int limit)
{
	int i;
	unsigned char error = 0;
	int bad_count = 0;

<<<<<<< HEAD
	printk(KERN_ERR "%03x: ", offset);
=======
	pr_err("%03x: ", offset);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	for (i = 0; i < limit; i++) {
		if (data[offset + i] != POISON_FREE) {
			error = data[offset + i];
			bad_count++;
		}
	}
	print_hex_dump(KERN_CONT, "", 0, 16, 1,
			&data[offset], limit, 1);

	if (bad_count == 1) {
		error ^= POISON_FREE;
		if (!(error & (error - 1))) {
<<<<<<< HEAD
			printk(KERN_ERR "Single bit error detected. Probably "
					"bad RAM.\n");
#ifdef CONFIG_X86
			printk(KERN_ERR "Run memtest86+ or a similar memory "
					"test tool.\n");
#else
			printk(KERN_ERR "Run a memory test tool.\n");
=======
			pr_err("Single bit error detected. Probably bad RAM.\n");
#ifdef CONFIG_X86
			pr_err("Run memtest86+ or a similar memory test tool.\n");
#else
			pr_err("Run a memory test tool.\n");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
		}
	}
}
#endif

#if DEBUG

static void print_objinfo(struct kmem_cache *cachep, void *objp, int lines)
{
	int i, size;
	char *realobj;

	if (cachep->flags & SLAB_RED_ZONE) {
<<<<<<< HEAD
		printk(KERN_ERR "Redzone: 0x%llx/0x%llx.\n",
			*dbg_redzone1(cachep, objp),
			*dbg_redzone2(cachep, objp));
	}

	if (cachep->flags & SLAB_STORE_USER) {
		printk(KERN_ERR "Last user: [<%p>](%pSR)\n",
=======
		pr_err("Redzone: 0x%llx/0x%llx\n",
		       *dbg_redzone1(cachep, objp),
		       *dbg_redzone2(cachep, objp));
	}

	if (cachep->flags & SLAB_STORE_USER) {
		pr_err("Last user: [<%p>](%pSR)\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       *dbg_userword(cachep, objp),
		       *dbg_userword(cachep, objp));
	}
	realobj = (char *)objp + obj_offset(cachep);
	size = cachep->object_size;
	for (i = 0; i < size && lines; i += 16, lines--) {
		int limit;
		limit = 16;
		if (i + limit > size)
			limit = size - i;
		dump_line(realobj, i, limit);
	}
}

static void check_poison_obj(struct kmem_cache *cachep, void *objp)
{
	char *realobj;
	int size, i;
	int lines = 0;

<<<<<<< HEAD
=======
	if (is_debug_pagealloc_cache(cachep))
		return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	realobj = (char *)objp + obj_offset(cachep);
	size = cachep->object_size;

	for (i = 0; i < size; i++) {
		char exp = POISON_FREE;
		if (i == size - 1)
			exp = POISON_END;
		if (realobj[i] != exp) {
			int limit;
			/* Mismatch ! */
			/* Print header */
			if (lines == 0) {
<<<<<<< HEAD
				printk(KERN_ERR
					"Slab corruption (%s): %s start=%p, len=%d\n",
					print_tainted(), cachep->name, realobj, size);
=======
				pr_err("Slab corruption (%s): %s start=%p, len=%d\n",
				       print_tainted(), cachep->name,
				       realobj, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				print_objinfo(cachep, objp, 0);
			}
			/* Hexdump the affected line */
			i = (i / 16) * 16;
			limit = 16;
			if (i + limit > size)
				limit = size - i;
			dump_line(realobj, i, limit);
			i += 16;
			lines++;
			/* Limit to 5 lines */
			if (lines > 5)
				break;
		}
	}
	if (lines != 0) {
		/* Print some data about the neighboring objects, if they
		 * exist:
		 */
<<<<<<< HEAD
		struct slab *slabp = virt_to_slab(objp);
		unsigned int objnr;

		objnr = obj_to_index(cachep, slabp, objp);
		if (objnr) {
			objp = index_to_obj(cachep, slabp, objnr - 1);
			realobj = (char *)objp + obj_offset(cachep);
			printk(KERN_ERR "Prev obj: start=%p, len=%d\n",
			       realobj, size);
			print_objinfo(cachep, objp, 2);
		}
		if (objnr + 1 < cachep->num) {
			objp = index_to_obj(cachep, slabp, objnr + 1);
			realobj = (char *)objp + obj_offset(cachep);
			printk(KERN_ERR "Next obj: start=%p, len=%d\n",
			       realobj, size);
=======
		struct page *page = virt_to_head_page(objp);
		unsigned int objnr;

		objnr = obj_to_index(cachep, page, objp);
		if (objnr) {
			objp = index_to_obj(cachep, page, objnr - 1);
			realobj = (char *)objp + obj_offset(cachep);
			pr_err("Prev obj: start=%p, len=%d\n", realobj, size);
			print_objinfo(cachep, objp, 2);
		}
		if (objnr + 1 < cachep->num) {
			objp = index_to_obj(cachep, page, objnr + 1);
			realobj = (char *)objp + obj_offset(cachep);
			pr_err("Next obj: start=%p, len=%d\n", realobj, size);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			print_objinfo(cachep, objp, 2);
		}
	}
}
#endif

#if DEBUG
<<<<<<< HEAD
static void slab_destroy_debugcheck(struct kmem_cache *cachep, struct slab *slabp)
{
	int i;
	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, slabp, i);

		if (cachep->flags & SLAB_POISON) {
#ifdef CONFIG_DEBUG_PAGEALLOC
			if (cachep->size % PAGE_SIZE == 0 &&
					OFF_SLAB(cachep))
				kernel_map_pages(virt_to_page(objp),
					cachep->size / PAGE_SIZE, 1);
			else
				check_poison_obj(cachep, objp);
#else
			check_poison_obj(cachep, objp);
#endif
		}
		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "start of a freed object "
					   "was overwritten");
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "end of a freed object "
					   "was overwritten");
=======
static void slab_destroy_debugcheck(struct kmem_cache *cachep,
						struct page *page)
{
	int i;

	if (OBJFREELIST_SLAB(cachep) && cachep->flags & SLAB_POISON) {
		poison_obj(cachep, page->freelist - obj_offset(cachep),
			POISON_FREE);
	}

	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, page, i);

		if (cachep->flags & SLAB_POISON) {
			check_poison_obj(cachep, objp);
			slab_kernel_map(cachep, objp, 1, 0);
		}
		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "start of a freed object was overwritten");
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "end of a freed object was overwritten");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
	}
}
#else
<<<<<<< HEAD
static void slab_destroy_debugcheck(struct kmem_cache *cachep, struct slab *slabp)
=======
static void slab_destroy_debugcheck(struct kmem_cache *cachep,
						struct page *page)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
}
#endif

/**
 * slab_destroy - destroy and release all objects in a slab
 * @cachep: cache pointer being destroyed
<<<<<<< HEAD
 * @slabp: slab pointer being destroyed
 *
 * Destroy all the objs in a slab, and release the mem back to the system.
 * Before calling the slab must have been unlinked from the cache.  The
 * cache-lock is not held/needed.
 */
static void slab_destroy(struct kmem_cache *cachep, struct slab *slabp)
{
	void *addr = slabp->s_mem - slabp->colouroff;

	slab_destroy_debugcheck(cachep, slabp);
	if (unlikely(cachep->flags & SLAB_DESTROY_BY_RCU)) {
		struct slab_rcu *slab_rcu;

		slab_rcu = (struct slab_rcu *)slabp;
		slab_rcu->cachep = cachep;
		slab_rcu->addr = addr;
		call_rcu(&slab_rcu->head, kmem_rcu_free);
	} else {
		kmem_freepages(cachep, addr);
		if (OFF_SLAB(cachep))
			kmem_cache_free(cachep->slabp_cache, slabp);
=======
 * @page: page pointer being destroyed
 *
 * Destroy all the objs in a slab page, and release the mem back to the system.
 * Before calling the slab page must have been unlinked from the cache. The
 * kmem_cache_node ->list_lock is not held/needed.
 */
static void slab_destroy(struct kmem_cache *cachep, struct page *page)
{
	void *freelist;

	freelist = page->freelist;
	slab_destroy_debugcheck(cachep, page);
	if (unlikely(cachep->flags & SLAB_DESTROY_BY_RCU))
		call_rcu(&page->rcu_head, kmem_rcu_free);
	else
		kmem_freepages(cachep, page);

	/*
	 * From now on, we don't use freelist
	 * although actual page can be freed in rcu context
	 */
	if (OFF_SLAB(cachep))
		kmem_cache_free(cachep->freelist_cache, freelist);
}

static void slabs_destroy(struct kmem_cache *cachep, struct list_head *list)
{
	struct page *page, *n;

	list_for_each_entry_safe(page, n, list, lru) {
		list_del(&page->lru);
		slab_destroy(cachep, page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/**
 * calculate_slab_order - calculate size (page order) of slabs
 * @cachep: pointer to the cache that is being created
 * @size: size of objects to be created in this cache.
<<<<<<< HEAD
 * @align: required alignment for the objects.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @flags: slab allocation flags
 *
 * Also calculates the number of objects per slab.
 *
 * This could be made much more intelligent.  For now, try to avoid using
 * high order pages for slabs.  When the gfp() functions are more friendly
 * towards high-order requests, this should be changed.
 */
static size_t calculate_slab_order(struct kmem_cache *cachep,
<<<<<<< HEAD
			size_t size, size_t align, unsigned long flags)
{
	unsigned long offslab_limit;
=======
				size_t size, unsigned long flags)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	size_t left_over = 0;
	int gfporder;

	for (gfporder = 0; gfporder <= KMALLOC_MAX_ORDER; gfporder++) {
		unsigned int num;
		size_t remainder;

<<<<<<< HEAD
		cache_estimate(gfporder, size, align, flags, &remainder, &num);
		if (!num)
			continue;

		if (flags & CFLGS_OFF_SLAB) {
			/*
			 * Max number of objs-per-slab for caches which
			 * use off-slab slabs. Needed to avoid a possible
			 * looping condition in cache_grow().
			 */
			offslab_limit = size - sizeof(struct slab);
			offslab_limit /= sizeof(kmem_bufctl_t);

 			if (num > offslab_limit)
				break;
=======
		num = cache_estimate(gfporder, size, flags, &remainder);
		if (!num)
			continue;

		/* Can't handle number of objects more than SLAB_OBJ_MAX_NUM */
		if (num > SLAB_OBJ_MAX_NUM)
			break;

		if (flags & CFLGS_OFF_SLAB) {
			struct kmem_cache *freelist_cache;
			size_t freelist_size;

			freelist_size = num * sizeof(freelist_idx_t);
			freelist_cache = kmalloc_slab(freelist_size, 0u);
			if (!freelist_cache)
				continue;

			/*
			 * Needed to avoid possible looping condition
			 * in cache_grow_begin()
			 */
			if (OFF_SLAB(freelist_cache))
				continue;

			/* check if off slab has enough benefit */
			if (freelist_cache->size > cachep->size / 2)
				continue;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Found something acceptable - save it away */
		cachep->num = num;
		cachep->gfporder = gfporder;
		left_over = remainder;

		/*
		 * A VFS-reclaimable slab tends to have most allocations
		 * as GFP_NOFS and we really don't want to have to be allocating
		 * higher-order pages when we are unable to shrink dcache.
		 */
		if (flags & SLAB_RECLAIM_ACCOUNT)
			break;

		/*
		 * Large number of objects is good, but very large slabs are
		 * currently bad for the gfp()s.
		 */
		if (gfporder >= slab_max_order)
			break;

		/*
		 * Acceptable internal fragmentation?
		 */
		if (left_over * 8 <= (PAGE_SIZE << gfporder))
			break;
	}
	return left_over;
}

<<<<<<< HEAD
static int __init_refok setup_cpu_cache(struct kmem_cache *cachep, gfp_t gfp)
=======
static struct array_cache __percpu *alloc_kmem_cache_cpus(
		struct kmem_cache *cachep, int entries, int batchcount)
{
	int cpu;
	size_t size;
	struct array_cache __percpu *cpu_cache;

	size = sizeof(void *) * entries + sizeof(struct array_cache);
	cpu_cache = __alloc_percpu(size, sizeof(void *));

	if (!cpu_cache)
		return NULL;

	for_each_possible_cpu(cpu) {
		init_arraycache(per_cpu_ptr(cpu_cache, cpu),
				entries, batchcount);
	}

	return cpu_cache;
}

static int __ref setup_cpu_cache(struct kmem_cache *cachep, gfp_t gfp)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (slab_state >= FULL)
		return enable_cpucache(cachep, gfp);

<<<<<<< HEAD
	if (slab_state == DOWN) {
		/*
		 * Note: Creation of first cache (kmem_cache).
		 * The setup_node is taken care
		 * of by the caller of __kmem_cache_create
		 */
		cachep->array[smp_processor_id()] = &initarray_generic.cache;
		slab_state = PARTIAL;
	} else if (slab_state == PARTIAL) {
		/*
		 * Note: the second kmem_cache_create must create the cache
		 * that's used by kmalloc(24), otherwise the creation of
		 * further caches will BUG().
		 */
		cachep->array[smp_processor_id()] = &initarray_generic.cache;

		/*
		 * If the cache that's used by kmalloc(sizeof(kmem_cache_node)) is
		 * the second cache, then we need to set up all its node/,
		 * otherwise the creation of further caches will BUG().
		 */
		set_up_node(cachep, SIZE_AC);
		if (INDEX_AC == INDEX_NODE)
			slab_state = PARTIAL_NODE;
		else
			slab_state = PARTIAL_ARRAYCACHE;
	} else {
		/* Remaining boot caches */
		cachep->array[smp_processor_id()] =
			kmalloc(sizeof(struct arraycache_init), gfp);

		if (slab_state == PARTIAL_ARRAYCACHE) {
			set_up_node(cachep, SIZE_NODE);
			slab_state = PARTIAL_NODE;
		} else {
			int node;
			for_each_online_node(node) {
				cachep->node[node] =
				    kmalloc_node(sizeof(struct kmem_cache_node),
						gfp, node);
				BUG_ON(!cachep->node[node]);
				kmem_cache_node_init(cachep->node[node]);
			}
		}
	}
	cachep->node[numa_mem_id()]->next_reap =
			jiffies + REAPTIMEOUT_LIST3 +
			((unsigned long)cachep) % REAPTIMEOUT_LIST3;
=======
	cachep->cpu_cache = alloc_kmem_cache_cpus(cachep, 1, 1);
	if (!cachep->cpu_cache)
		return 1;

	if (slab_state == DOWN) {
		/* Creation of first cache (kmem_cache). */
		set_up_node(kmem_cache, CACHE_CACHE);
	} else if (slab_state == PARTIAL) {
		/* For kmem_cache_node */
		set_up_node(cachep, SIZE_NODE);
	} else {
		int node;

		for_each_online_node(node) {
			cachep->node[node] = kmalloc_node(
				sizeof(struct kmem_cache_node), gfp, node);
			BUG_ON(!cachep->node[node]);
			kmem_cache_node_init(cachep->node[node]);
		}
	}

	cachep->node[numa_mem_id()]->next_reap =
			jiffies + REAPTIMEOUT_NODE +
			((unsigned long)cachep) % REAPTIMEOUT_NODE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cpu_cache_get(cachep)->avail = 0;
	cpu_cache_get(cachep)->limit = BOOT_CPUCACHE_ENTRIES;
	cpu_cache_get(cachep)->batchcount = 1;
	cpu_cache_get(cachep)->touched = 0;
	cachep->batchcount = 1;
	cachep->limit = BOOT_CPUCACHE_ENTRIES;
	return 0;
}

<<<<<<< HEAD
=======
unsigned long kmem_cache_flags(unsigned long object_size,
	unsigned long flags, const char *name,
	void (*ctor)(void *))
{
	return flags;
}

struct kmem_cache *
__kmem_cache_alias(const char *name, size_t size, size_t align,
		   unsigned long flags, void (*ctor)(void *))
{
	struct kmem_cache *cachep;

	cachep = find_mergeable(size, align, flags, name, ctor);
	if (cachep) {
		cachep->refcount++;

		/*
		 * Adjust the object sizes so that we clear
		 * the complete object on kzalloc.
		 */
		cachep->object_size = max_t(int, cachep->object_size, size);
	}
	return cachep;
}

static bool set_objfreelist_slab_cache(struct kmem_cache *cachep,
			size_t size, unsigned long flags)
{
	size_t left;

	cachep->num = 0;

	if (cachep->ctor || flags & SLAB_DESTROY_BY_RCU)
		return false;

	left = calculate_slab_order(cachep, size,
			flags | CFLGS_OBJFREELIST_SLAB);
	if (!cachep->num)
		return false;

	if (cachep->num * sizeof(freelist_idx_t) > cachep->object_size)
		return false;

	cachep->colour = left / cachep->colour_off;

	return true;
}

static bool set_off_slab_cache(struct kmem_cache *cachep,
			size_t size, unsigned long flags)
{
	size_t left;

	cachep->num = 0;

	/*
	 * Always use on-slab management when SLAB_NOLEAKTRACE
	 * to avoid recursive calls into kmemleak.
	 */
	if (flags & SLAB_NOLEAKTRACE)
		return false;

	/*
	 * Size is large, assume best to place the slab management obj
	 * off-slab (should allow better packing of objs).
	 */
	left = calculate_slab_order(cachep, size, flags | CFLGS_OFF_SLAB);
	if (!cachep->num)
		return false;

	/*
	 * If the slab has been placed off-slab, and we have enough space then
	 * move it on-slab. This is at the expense of any extra colouring.
	 */
	if (left >= cachep->num * sizeof(freelist_idx_t))
		return false;

	cachep->colour = left / cachep->colour_off;

	return true;
}

static bool set_on_slab_cache(struct kmem_cache *cachep,
			size_t size, unsigned long flags)
{
	size_t left;

	cachep->num = 0;

	left = calculate_slab_order(cachep, size, flags);
	if (!cachep->num)
		return false;

	cachep->colour = left / cachep->colour_off;

	return true;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * __kmem_cache_create - Create a cache.
 * @cachep: cache management descriptor
 * @flags: SLAB flags
 *
 * Returns a ptr to the cache on success, NULL on failure.
 * Cannot be called within a int, but can be interrupted.
 * The @ctor is run when new pages are allocated by the cache.
 *
 * The flags are
 *
 * %SLAB_POISON - Poison the slab with a known test pattern (a5a5a5a5)
 * to catch references to uninitialised memory.
 *
 * %SLAB_RED_ZONE - Insert `Red' zones around the allocated memory to check
 * for buffer overruns.
 *
 * %SLAB_HWCACHE_ALIGN - Align the objects in this cache to a hardware
 * cacheline.  This can be beneficial if you're counting cycles as closely
 * as davem.
 */
int
__kmem_cache_create (struct kmem_cache *cachep, unsigned long flags)
{
<<<<<<< HEAD
	size_t left_over, slab_size, ralign;
=======
	size_t ralign = BYTES_PER_WORD;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	gfp_t gfp;
	int err;
	size_t size = cachep->size;

#if DEBUG
#if FORCED_DEBUG
	/*
	 * Enable redzoning and last user accounting, except for caches with
	 * large objects, if the increased size would increase the object size
	 * above the next power of two: caches with object sizes just above a
	 * power of two have a significant amount of internal fragmentation.
	 */
	if (size < 4096 || fls(size - 1) == fls(size-1 + REDZONE_ALIGN +
						2 * sizeof(unsigned long long)))
		flags |= SLAB_RED_ZONE | SLAB_STORE_USER;
	if (!(flags & SLAB_DESTROY_BY_RCU))
		flags |= SLAB_POISON;
#endif
<<<<<<< HEAD
	if (flags & SLAB_DESTROY_BY_RCU)
		BUG_ON(flags & SLAB_POISON);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

	/*
	 * Check that size is in terms of words.  This is needed to avoid
	 * unaligned accesses for some archs when redzoning is used, and makes
	 * sure any on-slab bufctl's are also correctly aligned.
	 */
	if (size & (BYTES_PER_WORD - 1)) {
		size += (BYTES_PER_WORD - 1);
		size &= ~(BYTES_PER_WORD - 1);
	}

<<<<<<< HEAD
	/*
	 * Redzoning and user store require word alignment or possibly larger.
	 * Note this will be overridden by architecture or caller mandated
	 * alignment if either is greater than BYTES_PER_WORD.
	 */
	if (flags & SLAB_STORE_USER)
		ralign = BYTES_PER_WORD;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (flags & SLAB_RED_ZONE) {
		ralign = REDZONE_ALIGN;
		/* If redzoning, ensure that the second redzone is suitably
		 * aligned, by adjusting the object size accordingly. */
		size += REDZONE_ALIGN - 1;
		size &= ~(REDZONE_ALIGN - 1);
	}

	/* 3) caller mandated alignment */
	if (ralign < cachep->align) {
		ralign = cachep->align;
	}
	/* disable debug if necessary */
	if (ralign > __alignof__(unsigned long long))
		flags &= ~(SLAB_RED_ZONE | SLAB_STORE_USER);
	/*
	 * 4) Store it.
	 */
	cachep->align = ralign;
<<<<<<< HEAD
=======
	cachep->colour_off = cache_line_size();
	/* Offset must be a multiple of the alignment. */
	if (cachep->colour_off < cachep->align)
		cachep->colour_off = cachep->align;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (slab_is_available())
		gfp = GFP_KERNEL;
	else
		gfp = GFP_NOWAIT;

<<<<<<< HEAD
	setup_node_pointer(cachep);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if DEBUG

	/*
	 * Both debugging options require word-alignment which is calculated
	 * into align above.
	 */
	if (flags & SLAB_RED_ZONE) {
		/* add space for red zone words */
		cachep->obj_offset += sizeof(unsigned long long);
		size += 2 * sizeof(unsigned long long);
	}
	if (flags & SLAB_STORE_USER) {
		/* user store requires one word storage behind the end of
		 * the real object. But if the second red zone needs to be
		 * aligned to 64 bits, we must allow that much space.
		 */
		if (flags & SLAB_RED_ZONE)
			size += REDZONE_ALIGN;
		else
			size += BYTES_PER_WORD;
	}
<<<<<<< HEAD
#if FORCED_DEBUG && defined(CONFIG_DEBUG_PAGEALLOC)
	if (size >= kmalloc_size(INDEX_NODE + 1)
	    && cachep->object_size > cache_line_size()
	    && ALIGN(size, cachep->align) < PAGE_SIZE) {
		cachep->obj_offset += PAGE_SIZE - ALIGN(size, cachep->align);
		size = PAGE_SIZE;
	}
#endif
#endif

	/*
	 * Determine if the slab management is 'on' or 'off' slab.
	 * (bootstrapping cannot cope with offslab caches so don't do
	 * it too early on. Always use on-slab management when
	 * SLAB_NOLEAKTRACE to avoid recursive calls into kmemleak)
	 */
	if ((size >= (PAGE_SIZE >> 3)) && !slab_early_init &&
	    !(flags & SLAB_NOLEAKTRACE))
		/*
		 * Size is large, assume best to place the slab management obj
		 * off-slab (should allow better packing of objs).
		 */
		flags |= CFLGS_OFF_SLAB;

	size = ALIGN(size, cachep->align);

	left_over = calculate_slab_order(cachep, size, cachep->align, flags);

	if (!cachep->num)
		return -E2BIG;

	slab_size = ALIGN(cachep->num * sizeof(kmem_bufctl_t)
			  + sizeof(struct slab), cachep->align);

	/*
	 * If the slab has been placed off-slab, and we have enough space then
	 * move it on-slab. This is at the expense of any extra colouring.
	 */
	if (flags & CFLGS_OFF_SLAB && left_over >= slab_size) {
		flags &= ~CFLGS_OFF_SLAB;
		left_over -= slab_size;
	}

	if (flags & CFLGS_OFF_SLAB) {
		/* really off slab. No need for manual alignment */
		slab_size =
		    cachep->num * sizeof(kmem_bufctl_t) + sizeof(struct slab);

#ifdef CONFIG_PAGE_POISONING
		/* If we're going to use the generic kernel_map_pages()
		 * poisoning, then it's going to smash the contents of
		 * the redzone and userword anyhow, so switch them off.
		 */
		if (size % PAGE_SIZE == 0 && flags & SLAB_POISON)
			flags &= ~(SLAB_RED_ZONE | SLAB_STORE_USER);
#endif
	}

	cachep->colour_off = cache_line_size();
	/* Offset must be a multiple of the alignment. */
	if (cachep->colour_off < cachep->align)
		cachep->colour_off = cachep->align;
	cachep->colour = left_over / cachep->colour_off;
	cachep->slab_size = slab_size;
	cachep->flags = flags;
	cachep->allocflags = 0;
	if (CONFIG_ZONE_DMA_FLAG && (flags & SLAB_CACHE_DMA))
=======
#endif

	kasan_cache_create(cachep, &size, &flags);

	size = ALIGN(size, cachep->align);
	/*
	 * We should restrict the number of objects in a slab to implement
	 * byte sized index. Refer comment on SLAB_OBJ_MIN_SIZE definition.
	 */
	if (FREELIST_BYTE_INDEX && size < SLAB_OBJ_MIN_SIZE)
		size = ALIGN(SLAB_OBJ_MIN_SIZE, cachep->align);

#if DEBUG
	/*
	 * To activate debug pagealloc, off-slab management is necessary
	 * requirement. In early phase of initialization, small sized slab
	 * doesn't get initialized so it would not be possible. So, we need
	 * to check size >= 256. It guarantees that all necessary small
	 * sized slab is initialized in current slab initialization sequence.
	 */
	if (debug_pagealloc_enabled() && (flags & SLAB_POISON) &&
		size >= 256 && cachep->object_size > cache_line_size()) {
		if (size < PAGE_SIZE || size % PAGE_SIZE == 0) {
			size_t tmp_size = ALIGN(size, PAGE_SIZE);

			if (set_off_slab_cache(cachep, tmp_size, flags)) {
				flags |= CFLGS_OFF_SLAB;
				cachep->obj_offset += tmp_size - size;
				size = tmp_size;
				goto done;
			}
		}
	}
#endif

	if (set_objfreelist_slab_cache(cachep, size, flags)) {
		flags |= CFLGS_OBJFREELIST_SLAB;
		goto done;
	}

	if (set_off_slab_cache(cachep, size, flags)) {
		flags |= CFLGS_OFF_SLAB;
		goto done;
	}

	if (set_on_slab_cache(cachep, size, flags))
		goto done;

	return -E2BIG;

done:
	cachep->freelist_size = cachep->num * sizeof(freelist_idx_t);
	cachep->flags = flags;
	cachep->allocflags = __GFP_COMP;
	if (flags & SLAB_CACHE_DMA)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		cachep->allocflags |= GFP_DMA;
	cachep->size = size;
	cachep->reciprocal_buffer_size = reciprocal_value(size);

<<<<<<< HEAD
	if (flags & CFLGS_OFF_SLAB) {
		cachep->slabp_cache = kmalloc_slab(slab_size, 0u);
		/*
		 * This is a possibility for one of the malloc_sizes caches.
		 * But since we go off slab only for object size greater than
		 * PAGE_SIZE/8, and malloc_sizes gets created in ascending order,
		 * this should not happen at all.
		 * But leave a BUG_ON for some lucky dude.
		 */
		BUG_ON(ZERO_OR_NULL_PTR(cachep->slabp_cache));
=======
#if DEBUG
	/*
	 * If we're going to use the generic kernel_map_pages()
	 * poisoning, then it's going to smash the contents of
	 * the redzone and userword anyhow, so switch them off.
	 */
	if (IS_ENABLED(CONFIG_PAGE_POISONING) &&
		(cachep->flags & SLAB_POISON) &&
		is_debug_pagealloc_cache(cachep))
		cachep->flags &= ~(SLAB_RED_ZONE | SLAB_STORE_USER);
#endif

	if (OFF_SLAB(cachep)) {
		cachep->freelist_cache =
			kmalloc_slab(cachep->freelist_size, 0u);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	err = setup_cpu_cache(cachep, gfp);
	if (err) {
<<<<<<< HEAD
		__kmem_cache_shutdown(cachep);
		return err;
	}

	if (flags & SLAB_DEBUG_OBJECTS) {
		/*
		 * Would deadlock through slab_destroy()->call_rcu()->
		 * debug_object_activate()->kmem_cache_alloc().
		 */
		WARN_ON_ONCE(flags & SLAB_DESTROY_BY_RCU);

		slab_set_debugobj_lock_classes(cachep);
	} else if (!OFF_SLAB(cachep) && !(flags & SLAB_DESTROY_BY_RCU))
		on_slab_lock_classes(cachep);

=======
		__kmem_cache_release(cachep);
		return err;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

#if DEBUG
static void check_irq_off(void)
{
	BUG_ON(!irqs_disabled());
}

static void check_irq_on(void)
{
	BUG_ON(irqs_disabled());
}

<<<<<<< HEAD
=======
static void check_mutex_acquired(void)
{
	BUG_ON(!mutex_is_locked(&slab_mutex));
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static void check_spinlock_acquired(struct kmem_cache *cachep)
{
#ifdef CONFIG_SMP
	check_irq_off();
<<<<<<< HEAD
	assert_spin_locked(&cachep->node[numa_mem_id()]->list_lock);
=======
	assert_spin_locked(&get_node(cachep, numa_mem_id())->list_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
}

static void check_spinlock_acquired_node(struct kmem_cache *cachep, int node)
{
#ifdef CONFIG_SMP
	check_irq_off();
<<<<<<< HEAD
	assert_spin_locked(&cachep->node[node]->list_lock);
=======
	assert_spin_locked(&get_node(cachep, node)->list_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
}

#else
#define check_irq_off()	do { } while(0)
#define check_irq_on()	do { } while(0)
<<<<<<< HEAD
=======
#define check_mutex_acquired()	do { } while(0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define check_spinlock_acquired(x) do { } while(0)
#define check_spinlock_acquired_node(x, y) do { } while(0)
#endif

<<<<<<< HEAD
static void drain_array(struct kmem_cache *cachep, struct kmem_cache_node *n,
			struct array_cache *ac,
			int force, int node);
=======
static void drain_array_locked(struct kmem_cache *cachep, struct array_cache *ac,
				int node, bool free_all, struct list_head *list)
{
	int tofree;

	if (!ac || !ac->avail)
		return;

	tofree = free_all ? ac->avail : (ac->limit + 4) / 5;
	if (tofree > ac->avail)
		tofree = (ac->avail + 1) / 2;

	free_block(cachep, ac->entry, tofree, node, list);
	ac->avail -= tofree;
	memmove(ac->entry, &(ac->entry[tofree]), sizeof(void *) * ac->avail);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void do_drain(void *arg)
{
	struct kmem_cache *cachep = arg;
	struct array_cache *ac;
	int node = numa_mem_id();
<<<<<<< HEAD

	check_irq_off();
	ac = cpu_cache_get(cachep);
	spin_lock(&cachep->node[node]->list_lock);
	free_block(cachep, ac->entry, ac->avail, node);
	spin_unlock(&cachep->node[node]->list_lock);
=======
	struct kmem_cache_node *n;
	LIST_HEAD(list);

	check_irq_off();
	ac = cpu_cache_get(cachep);
	n = get_node(cachep, node);
	spin_lock(&n->list_lock);
	free_block(cachep, ac->entry, ac->avail, node, &list);
	spin_unlock(&n->list_lock);
	slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ac->avail = 0;
}

static void drain_cpu_caches(struct kmem_cache *cachep)
{
	struct kmem_cache_node *n;
	int node;
<<<<<<< HEAD

	on_each_cpu(do_drain, cachep, 1);
	check_irq_on();
	for_each_online_node(node) {
		n = cachep->node[node];
		if (n && n->alien)
			drain_alien_cache(cachep, n->alien);
	}

	for_each_online_node(node) {
		n = cachep->node[node];
		if (n)
			drain_array(cachep, n, n->shared, 1, node);
=======
	LIST_HEAD(list);

	on_each_cpu(do_drain, cachep, 1);
	check_irq_on();
	for_each_kmem_cache_node(cachep, node, n)
		if (n->alien)
			drain_alien_cache(cachep, n->alien);

	for_each_kmem_cache_node(cachep, node, n) {
		spin_lock_irq(&n->list_lock);
		drain_array_locked(cachep, n->shared, node, true, &list);
		spin_unlock_irq(&n->list_lock);

		slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

/*
 * Remove slabs from the list of free slabs.
 * Specify the number of slabs to drain in tofree.
 *
 * Returns the actual number of slabs released.
 */
static int drain_freelist(struct kmem_cache *cache,
			struct kmem_cache_node *n, int tofree)
{
	struct list_head *p;
	int nr_freed;
<<<<<<< HEAD
	struct slab *slabp;
=======
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	nr_freed = 0;
	while (nr_freed < tofree && !list_empty(&n->slabs_free)) {

		spin_lock_irq(&n->list_lock);
		p = n->slabs_free.prev;
		if (p == &n->slabs_free) {
			spin_unlock_irq(&n->list_lock);
			goto out;
		}

<<<<<<< HEAD
		slabp = list_entry(p, struct slab, list);
#if DEBUG
		BUG_ON(slabp->inuse);
#endif
		list_del(&slabp->list);
=======
		page = list_entry(p, struct page, lru);
		list_del(&page->lru);
		n->num_slabs--;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Safe to drop the lock. The slab is no longer linked
		 * to the cache.
		 */
		n->free_objects -= cache->num;
		spin_unlock_irq(&n->list_lock);
<<<<<<< HEAD
		slab_destroy(cache, slabp);
=======
		slab_destroy(cache, page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		nr_freed++;
	}
out:
	return nr_freed;
}

<<<<<<< HEAD
/* Called with slab_mutex held to protect against cpu hotplug */
static int __cache_shrink(struct kmem_cache *cachep)
{
	int ret = 0, i = 0;
=======
int __kmem_cache_shrink(struct kmem_cache *cachep)
{
	int ret = 0;
	int node;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kmem_cache_node *n;

	drain_cpu_caches(cachep);

	check_irq_on();
<<<<<<< HEAD
	for_each_online_node(i) {
		n = cachep->node[i];
		if (!n)
			continue;

		drain_freelist(cachep, n, n->free_objects);
=======
	for_each_kmem_cache_node(cachep, node, n) {
		drain_freelist(cachep, n, INT_MAX);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		ret += !list_empty(&n->slabs_full) ||
			!list_empty(&n->slabs_partial);
	}
	return (ret ? 1 : 0);
}

<<<<<<< HEAD
/**
 * kmem_cache_shrink - Shrink a cache.
 * @cachep: The cache to shrink.
 *
 * Releases as many slabs as possible for a cache.
 * To help debugging, a zero exit status indicates all slabs were released.
 */
int kmem_cache_shrink(struct kmem_cache *cachep)
{
	int ret;
	BUG_ON(!cachep || in_interrupt());

	get_online_cpus();
	mutex_lock(&slab_mutex);
	ret = __cache_shrink(cachep);
	mutex_unlock(&slab_mutex);
	put_online_cpus();
	return ret;
}
EXPORT_SYMBOL(kmem_cache_shrink);

int __kmem_cache_shutdown(struct kmem_cache *cachep)
{
	int i;
	struct kmem_cache_node *n;
	int rc = __cache_shrink(cachep);

	if (rc)
		return rc;

	for_each_online_cpu(i)
	    kfree(cachep->array[i]);

	/* NUMA: free the node structures */
	for_each_online_node(i) {
		n = cachep->node[i];
		if (n) {
			kfree(n->shared);
			free_alien_cache(n->alien);
			kfree(n);
		}
	}
	return 0;
=======
int __kmem_cache_shutdown(struct kmem_cache *cachep)
{
	return __kmem_cache_shrink(cachep);
}

void __kmem_cache_release(struct kmem_cache *cachep)
{
	int i;
	struct kmem_cache_node *n;

	cache_random_seq_destroy(cachep);

	free_percpu(cachep->cpu_cache);

	/* NUMA: free the node structures */
	for_each_kmem_cache_node(cachep, i, n) {
		kfree(n->shared);
		free_alien_cache(n->alien);
		kfree(n);
		cachep->node[i] = NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Get the memory for a slab management obj.
<<<<<<< HEAD
 * For a slab cache when the slab descriptor is off-slab, slab descriptors
 * always come from malloc_sizes caches.  The slab descriptor cannot
 * come from the same cache which is getting created because,
 * when we are searching for an appropriate cache for these
 * descriptors in kmem_cache_create, we search through the malloc_sizes array.
 * If we are creating a malloc_sizes cache here it would not be visible to
 * kmem_find_general_cachep till the initialization is complete.
 * Hence we cannot have slabp_cache same as the original cache.
 */
static struct slab *alloc_slabmgmt(struct kmem_cache *cachep, void *objp,
				   int colour_off, gfp_t local_flags,
				   int nodeid)
{
	struct slab *slabp;

	if (OFF_SLAB(cachep)) {
		/* Slab management obj is off-slab. */
		slabp = kmem_cache_alloc_node(cachep->slabp_cache,
					      local_flags, nodeid);
		/*
		 * If the first object in the slab is leaked (it's allocated
		 * but no one has a reference to it), we want to make sure
		 * kmemleak does not treat the ->s_mem pointer as a reference
		 * to the object. Otherwise we will not report the leak.
		 */
		kmemleak_scan_area(&slabp->list, sizeof(struct list_head),
				   local_flags);
		if (!slabp)
			return NULL;
	} else {
		slabp = objp + colour_off;
		colour_off += cachep->slab_size;
	}
	slabp->inuse = 0;
	slabp->colouroff = colour_off;
	slabp->s_mem = objp + colour_off;
	slabp->nodeid = nodeid;
	slabp->free = 0;
	return slabp;
}

static inline kmem_bufctl_t *slab_bufctl(struct slab *slabp)
{
	return (kmem_bufctl_t *) (slabp + 1);
}

static void cache_init_objs(struct kmem_cache *cachep,
			    struct slab *slabp)
{
	int i;

	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, slabp, i);
#if DEBUG
		/* need to poison the objs? */
		if (cachep->flags & SLAB_POISON)
			poison_obj(cachep, objp, POISON_FREE);
=======
 *
 * For a slab cache when the slab descriptor is off-slab, the
 * slab descriptor can't come from the same cache which is being created,
 * Because if it is the case, that means we defer the creation of
 * the kmalloc_{dma,}_cache of size sizeof(slab descriptor) to this point.
 * And we eventually call down to __kmem_cache_create(), which
 * in turn looks up in the kmalloc_{dma,}_caches for the disired-size one.
 * This is a "chicken-and-egg" problem.
 *
 * So the off-slab slab descriptor shall come from the kmalloc_{dma,}_caches,
 * which are all initialized during kmem_cache_init().
 */
static void *alloc_slabmgmt(struct kmem_cache *cachep,
				   struct page *page, int colour_off,
				   gfp_t local_flags, int nodeid)
{
	void *freelist;
	void *addr = page_address(page);

	page->s_mem = addr + colour_off;
	page->active = 0;

	if (OBJFREELIST_SLAB(cachep))
		freelist = NULL;
	else if (OFF_SLAB(cachep)) {
		/* Slab management obj is off-slab. */
		freelist = kmem_cache_alloc_node(cachep->freelist_cache,
					      local_flags, nodeid);
		if (!freelist)
			return NULL;
	} else {
		/* We will use last bytes at the slab for freelist */
		freelist = addr + (PAGE_SIZE << cachep->gfporder) -
				cachep->freelist_size;
	}

	return freelist;
}

static inline freelist_idx_t get_free_obj(struct page *page, unsigned int idx)
{
	return ((freelist_idx_t *)page->freelist)[idx];
}

static inline void set_free_obj(struct page *page,
					unsigned int idx, freelist_idx_t val)
{
	((freelist_idx_t *)(page->freelist))[idx] = val;
}

static void cache_init_objs_debug(struct kmem_cache *cachep, struct page *page)
{
#if DEBUG
	int i;

	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, page, i);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (cachep->flags & SLAB_STORE_USER)
			*dbg_userword(cachep, objp) = NULL;

		if (cachep->flags & SLAB_RED_ZONE) {
			*dbg_redzone1(cachep, objp) = RED_INACTIVE;
			*dbg_redzone2(cachep, objp) = RED_INACTIVE;
		}
		/*
		 * Constructors are not allowed to allocate memory from the same
		 * cache which they are a constructor for.  Otherwise, deadlock.
		 * They must also be threaded.
		 */
<<<<<<< HEAD
		if (cachep->ctor && !(cachep->flags & SLAB_POISON))
			cachep->ctor(objp + obj_offset(cachep));

		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the"
					   " end of an object");
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the"
					   " start of an object");
		}
		if ((cachep->size % PAGE_SIZE) == 0 &&
			    OFF_SLAB(cachep) && cachep->flags & SLAB_POISON)
			kernel_map_pages(virt_to_page(objp),
					 cachep->size / PAGE_SIZE, 0);
#else
		if (cachep->ctor)
			cachep->ctor(objp);
#endif
		slab_bufctl(slabp)[i] = i + 1;
	}
	slab_bufctl(slabp)[i - 1] = BUFCTL_END;
}

static void kmem_flagcheck(struct kmem_cache *cachep, gfp_t flags)
{
	if (CONFIG_ZONE_DMA_FLAG) {
		if (flags & GFP_DMA)
			BUG_ON(!(cachep->allocflags & GFP_DMA));
		else
			BUG_ON(cachep->allocflags & GFP_DMA);
	}
}

static void *slab_get_obj(struct kmem_cache *cachep, struct slab *slabp,
				int nodeid)
{
	void *objp = index_to_obj(cachep, slabp, slabp->free);
	kmem_bufctl_t next;

	slabp->inuse++;
	next = slab_bufctl(slabp)[slabp->free];
#if DEBUG
	slab_bufctl(slabp)[slabp->free] = BUFCTL_FREE;
	WARN_ON(slabp->nodeid != nodeid);
#endif
	slabp->free = next;
=======
		if (cachep->ctor && !(cachep->flags & SLAB_POISON)) {
			kasan_unpoison_object_data(cachep,
						   objp + obj_offset(cachep));
			cachep->ctor(objp + obj_offset(cachep));
			kasan_poison_object_data(
				cachep, objp + obj_offset(cachep));
		}

		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the end of an object");
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the start of an object");
		}
		/* need to poison the objs? */
		if (cachep->flags & SLAB_POISON) {
			poison_obj(cachep, objp, POISON_FREE);
			slab_kernel_map(cachep, objp, 0, 0);
		}
	}
#endif
}

#ifdef CONFIG_SLAB_FREELIST_RANDOM
/* Hold information during a freelist initialization */
union freelist_init_state {
	struct {
		unsigned int pos;
		unsigned int *list;
		unsigned int count;
	};
	struct rnd_state rnd_state;
};

/*
 * Initialize the state based on the randomization methode available.
 * return true if the pre-computed list is available, false otherwize.
 */
static bool freelist_state_initialize(union freelist_init_state *state,
				struct kmem_cache *cachep,
				unsigned int count)
{
	bool ret;
	unsigned int rand;

	/* Use best entropy available to define a random shift */
	rand = get_random_int();

	/* Use a random state if the pre-computed list is not available */
	if (!cachep->random_seq) {
		prandom_seed_state(&state->rnd_state, rand);
		ret = false;
	} else {
		state->list = cachep->random_seq;
		state->count = count;
		state->pos = rand % count;
		ret = true;
	}
	return ret;
}

/* Get the next entry on the list and randomize it using a random shift */
static freelist_idx_t next_random_slot(union freelist_init_state *state)
{
	if (state->pos >= state->count)
		state->pos = 0;
	return state->list[state->pos++];
}

/* Swap two freelist entries */
static void swap_free_obj(struct page *page, unsigned int a, unsigned int b)
{
	swap(((freelist_idx_t *)page->freelist)[a],
		((freelist_idx_t *)page->freelist)[b]);
}

/*
 * Shuffle the freelist initialization state based on pre-computed lists.
 * return true if the list was successfully shuffled, false otherwise.
 */
static bool shuffle_freelist(struct kmem_cache *cachep, struct page *page)
{
	unsigned int objfreelist = 0, i, rand, count = cachep->num;
	union freelist_init_state state;
	bool precomputed;

	if (count < 2)
		return false;

	precomputed = freelist_state_initialize(&state, cachep, count);

	/* Take a random entry as the objfreelist */
	if (OBJFREELIST_SLAB(cachep)) {
		if (!precomputed)
			objfreelist = count - 1;
		else
			objfreelist = next_random_slot(&state);
		page->freelist = index_to_obj(cachep, page, objfreelist) +
						obj_offset(cachep);
		count--;
	}

	/*
	 * On early boot, generate the list dynamically.
	 * Later use a pre-computed list for speed.
	 */
	if (!precomputed) {
		for (i = 0; i < count; i++)
			set_free_obj(page, i, i);

		/* Fisher-Yates shuffle */
		for (i = count - 1; i > 0; i--) {
			rand = prandom_u32_state(&state.rnd_state);
			rand %= (i + 1);
			swap_free_obj(page, i, rand);
		}
	} else {
		for (i = 0; i < count; i++)
			set_free_obj(page, i, next_random_slot(&state));
	}

	if (OBJFREELIST_SLAB(cachep))
		set_free_obj(page, cachep->num - 1, objfreelist);

	return true;
}
#else
static inline bool shuffle_freelist(struct kmem_cache *cachep,
				struct page *page)
{
	return false;
}
#endif /* CONFIG_SLAB_FREELIST_RANDOM */

static void cache_init_objs(struct kmem_cache *cachep,
			    struct page *page)
{
	int i;
	void *objp;
	bool shuffled;

	cache_init_objs_debug(cachep, page);

	/* Try to randomize the freelist if enabled */
	shuffled = shuffle_freelist(cachep, page);

	if (!shuffled && OBJFREELIST_SLAB(cachep)) {
		page->freelist = index_to_obj(cachep, page, cachep->num - 1) +
						obj_offset(cachep);
	}

	for (i = 0; i < cachep->num; i++) {
		objp = index_to_obj(cachep, page, i);
		kasan_init_slab_obj(cachep, objp);

		/* constructor could break poison info */
		if (DEBUG == 0 && cachep->ctor) {
			kasan_unpoison_object_data(cachep, objp);
			cachep->ctor(objp);
			kasan_poison_object_data(cachep, objp);
		}

		if (!shuffled)
			set_free_obj(page, i, i);
	}
}

static void *slab_get_obj(struct kmem_cache *cachep, struct page *page)
{
	void *objp;

	objp = index_to_obj(cachep, page, get_free_obj(page, page->active));
	page->active++;

#if DEBUG
	if (cachep->flags & SLAB_STORE_USER)
		set_store_user_dirty(cachep);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return objp;
}

<<<<<<< HEAD
static void slab_put_obj(struct kmem_cache *cachep, struct slab *slabp,
				void *objp, int nodeid)
{
	unsigned int objnr = obj_to_index(cachep, slabp, objp);

#if DEBUG
	/* Verify that the slab belongs to the intended node */
	WARN_ON(slabp->nodeid != nodeid);

	if (slab_bufctl(slabp)[objnr] + 1 <= SLAB_LIMIT + 1) {
		printk(KERN_ERR "slab: double free detected in cache "
				"'%s', objp %p\n", cachep->name, objp);
		BUG();
	}
#endif
	slab_bufctl(slabp)[objnr] = slabp->free;
	slabp->free = objnr;
	slabp->inuse--;
=======
static void slab_put_obj(struct kmem_cache *cachep,
			struct page *page, void *objp)
{
	unsigned int objnr = obj_to_index(cachep, page, objp);
#if DEBUG
	unsigned int i;

	/* Verify double free bug */
	for (i = page->active; i < cachep->num; i++) {
		if (get_free_obj(page, i) == objnr) {
			pr_err("slab: double free detected in cache '%s', objp %p\n",
			       cachep->name, objp);
			BUG();
		}
	}
#endif
	page->active--;
	if (!page->freelist)
		page->freelist = objp + obj_offset(cachep);

	set_free_obj(page, page->active, objnr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Map pages beginning at addr to the given cache and slab. This is required
 * for the slab allocator to be able to lookup the cache and slab of a
 * virtual address for kfree, ksize, and slab debugging.
 */
<<<<<<< HEAD
static void slab_map_pages(struct kmem_cache *cache, struct slab *slab,
			   void *addr)
{
	int nr_pages;
	struct page *page;

	page = virt_to_page(addr);

	nr_pages = 1;
	if (likely(!PageCompound(page)))
		nr_pages <<= cache->gfporder;

	do {
		page->slab_cache = cache;
		page->slab_page = slab;
		page++;
	} while (--nr_pages);
=======
static void slab_map_pages(struct kmem_cache *cache, struct page *page,
			   void *freelist)
{
	page->slab_cache = cache;
	page->freelist = freelist;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Grow (by 1) the number of slabs within a cache.  This is called by
 * kmem_cache_alloc() when there are no active objs left in a cache.
 */
<<<<<<< HEAD
static int cache_grow(struct kmem_cache *cachep,
		gfp_t flags, int nodeid, void *objp)
{
	struct slab *slabp;
	size_t offset;
	gfp_t local_flags;
	struct kmem_cache_node *n;
=======
static struct page *cache_grow_begin(struct kmem_cache *cachep,
				gfp_t flags, int nodeid)
{
	void *freelist;
	size_t offset;
	gfp_t local_flags;
	int page_node;
	struct kmem_cache_node *n;
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Be lazy and only check for valid flags here,  keeping it out of the
	 * critical path in kmem_cache_alloc().
	 */
<<<<<<< HEAD
	BUG_ON(flags & GFP_SLAB_BUG_MASK);
	local_flags = flags & (GFP_CONSTRAINT_MASK|GFP_RECLAIM_MASK);

	/* Take the node list lock to change the colour_next on this node */
	check_irq_off();
	n = cachep->node[nodeid];
	spin_lock(&n->list_lock);

	/* Get colour for the slab, and cal the next value. */
	offset = n->colour_next;
	n->colour_next++;
	if (n->colour_next >= cachep->colour)
		n->colour_next = 0;
	spin_unlock(&n->list_lock);

	offset *= cachep->colour_off;

	if (local_flags & __GFP_WAIT)
		local_irq_enable();

	/*
	 * The test for missing atomic flag is performed here, rather than
	 * the more obvious place, simply to reduce the critical path length
	 * in kmem_cache_alloc(). If a caller is seriously mis-behaving they
	 * will eventually be caught here (where it matters).
	 */
	kmem_flagcheck(cachep, flags);

	/*
	 * Get mem for the objs.  Attempt to allocate a physical page from
	 * 'nodeid'.
	 */
	if (!objp)
		objp = kmem_getpages(cachep, local_flags, nodeid);
	if (!objp)
		goto failed;

	/* Get slab management. */
	slabp = alloc_slabmgmt(cachep, objp, offset,
			local_flags & ~GFP_CONSTRAINT_MASK, nodeid);
	if (!slabp)
		goto opps1;

	slab_map_pages(cachep, slabp, objp);

	cache_init_objs(cachep, slabp);

	if (local_flags & __GFP_WAIT)
		local_irq_disable();
	check_irq_off();
	spin_lock(&n->list_lock);

	/* Make slab active. */
	list_add_tail(&slabp->list, &(n->slabs_free));
	STATS_INC_GROWN(cachep);
	n->free_objects += cachep->num;
	spin_unlock(&n->list_lock);
	return 1;
opps1:
	kmem_freepages(cachep, objp);
failed:
	if (local_flags & __GFP_WAIT)
		local_irq_disable();
	return 0;
=======
	if (unlikely(flags & GFP_SLAB_BUG_MASK)) {
		gfp_t invalid_mask = flags & GFP_SLAB_BUG_MASK;
		flags &= ~GFP_SLAB_BUG_MASK;
		pr_warn("Unexpected gfp: %#x (%pGg). Fixing up to gfp: %#x (%pGg). Fix your code!\n",
				invalid_mask, &invalid_mask, flags, &flags);
		dump_stack();
	}
	local_flags = flags & (GFP_CONSTRAINT_MASK|GFP_RECLAIM_MASK);

	check_irq_off();
	if (gfpflags_allow_blocking(local_flags))
		local_irq_enable();

	/*
	 * Get mem for the objs.  Attempt to allocate a physical page from
	 * 'nodeid'.
	 */
	page = kmem_getpages(cachep, local_flags, nodeid);
	if (!page)
		goto failed;

	page_node = page_to_nid(page);
	n = get_node(cachep, page_node);

	/* Get colour for the slab, and cal the next value. */
	n->colour_next++;
	if (n->colour_next >= cachep->colour)
		n->colour_next = 0;

	offset = n->colour_next;
	if (offset >= cachep->colour)
		offset = 0;

	offset *= cachep->colour_off;

	/* Get slab management. */
	freelist = alloc_slabmgmt(cachep, page, offset,
			local_flags & ~GFP_CONSTRAINT_MASK, page_node);
	if (OFF_SLAB(cachep) && !freelist)
		goto opps1;

	slab_map_pages(cachep, page, freelist);

	kasan_poison_slab(page);
	cache_init_objs(cachep, page);

	if (gfpflags_allow_blocking(local_flags))
		local_irq_disable();

	return page;

opps1:
	kmem_freepages(cachep, page);
failed:
	if (gfpflags_allow_blocking(local_flags))
		local_irq_disable();
	return NULL;
}

static void cache_grow_end(struct kmem_cache *cachep, struct page *page)
{
	struct kmem_cache_node *n;
	void *list = NULL;

	check_irq_off();

	if (!page)
		return;

	INIT_LIST_HEAD(&page->lru);
	n = get_node(cachep, page_to_nid(page));

	spin_lock(&n->list_lock);
	if (!page->active)
		list_add_tail(&page->lru, &(n->slabs_free));
	else
		fixup_slab_list(cachep, n, page, &list);

	n->num_slabs++;
	STATS_INC_GROWN(cachep);
	n->free_objects += cachep->num - page->active;
	spin_unlock(&n->list_lock);

	fixup_objfreelist_debug(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#if DEBUG

/*
 * Perform extra freeing checks:
 * - detect bad pointers.
 * - POISON/RED_ZONE checking
 */
static void kfree_debugcheck(const void *objp)
{
	if (!virt_addr_valid(objp)) {
<<<<<<< HEAD
		printk(KERN_ERR "kfree_debugcheck: out of range ptr %lxh.\n",
=======
		pr_err("kfree_debugcheck: out of range ptr %lxh\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       (unsigned long)objp);
		BUG();
	}
}

static inline void verify_redzone_free(struct kmem_cache *cache, void *obj)
{
	unsigned long long redzone1, redzone2;

	redzone1 = *dbg_redzone1(cache, obj);
	redzone2 = *dbg_redzone2(cache, obj);

	/*
	 * Redzone is ok.
	 */
	if (redzone1 == RED_ACTIVE && redzone2 == RED_ACTIVE)
		return;

	if (redzone1 == RED_INACTIVE && redzone2 == RED_INACTIVE)
		slab_error(cache, "double free detected");
	else
		slab_error(cache, "memory outside object was overwritten");

<<<<<<< HEAD
	printk(KERN_ERR "%p: redzone 1:0x%llx, redzone 2:0x%llx.\n",
			obj, redzone1, redzone2);
=======
	pr_err("%p: redzone 1:0x%llx, redzone 2:0x%llx\n",
	       obj, redzone1, redzone2);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void *cache_free_debugcheck(struct kmem_cache *cachep, void *objp,
				   unsigned long caller)
{
<<<<<<< HEAD
	struct page *page;
	unsigned int objnr;
	struct slab *slabp;
=======
	unsigned int objnr;
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	BUG_ON(virt_to_cache(objp) != cachep);

	objp -= obj_offset(cachep);
	kfree_debugcheck(objp);
	page = virt_to_head_page(objp);

<<<<<<< HEAD
	slabp = page->slab_page;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (cachep->flags & SLAB_RED_ZONE) {
		verify_redzone_free(cachep, objp);
		*dbg_redzone1(cachep, objp) = RED_INACTIVE;
		*dbg_redzone2(cachep, objp) = RED_INACTIVE;
	}
<<<<<<< HEAD
	if (cachep->flags & SLAB_STORE_USER)
		*dbg_userword(cachep, objp) = (void *)caller;

	objnr = obj_to_index(cachep, slabp, objp);

	BUG_ON(objnr >= cachep->num);
	BUG_ON(objp != index_to_obj(cachep, slabp, objnr));

#ifdef CONFIG_DEBUG_SLAB_LEAK
	slab_bufctl(slabp)[objnr] = BUFCTL_FREE;
#endif
	if (cachep->flags & SLAB_POISON) {
#ifdef CONFIG_DEBUG_PAGEALLOC
		if ((cachep->size % PAGE_SIZE)==0 && OFF_SLAB(cachep)) {
			store_stackinfo(cachep, objp, caller);
			kernel_map_pages(virt_to_page(objp),
					 cachep->size / PAGE_SIZE, 0);
		} else {
			poison_obj(cachep, objp, POISON_FREE);
		}
#else
		poison_obj(cachep, objp, POISON_FREE);
#endif
	}
	return objp;
}

static void check_slabp(struct kmem_cache *cachep, struct slab *slabp)
{
	kmem_bufctl_t i;
	int entries = 0;

	/* Check slab's freelist to see if this obj is there. */
	for (i = slabp->free; i != BUFCTL_END; i = slab_bufctl(slabp)[i]) {
		entries++;
		if (entries > cachep->num || i >= cachep->num)
			goto bad;
	}
	if (entries != cachep->num - slabp->inuse) {
bad:
		printk(KERN_ERR "slab: Internal list corruption detected in "
			"cache '%s'(%d), slabp %p(%d). Tainted(%s). Hexdump:\n",
			cachep->name, cachep->num, slabp, slabp->inuse,
			print_tainted());
		print_hex_dump(KERN_ERR, "", DUMP_PREFIX_OFFSET, 16, 1, slabp,
			sizeof(*slabp) + cachep->num * sizeof(kmem_bufctl_t),
			1);
		BUG();
	}
}
#else
#define kfree_debugcheck(x) do { } while(0)
#define cache_free_debugcheck(x,objp,z) (objp)
#define check_slabp(x,y) do { } while(0)
#endif

static void *cache_alloc_refill(struct kmem_cache *cachep, gfp_t flags,
							bool force_refill)
{
	int batchcount;
	struct kmem_cache_node *n;
	struct array_cache *ac;
	int node;

	check_irq_off();
	node = numa_mem_id();
	if (unlikely(force_refill))
		goto force_grow;
retry:
=======
	if (cachep->flags & SLAB_STORE_USER) {
		set_store_user_dirty(cachep);
		*dbg_userword(cachep, objp) = (void *)caller;
	}

	objnr = obj_to_index(cachep, page, objp);

	BUG_ON(objnr >= cachep->num);
	BUG_ON(objp != index_to_obj(cachep, page, objnr));

	if (cachep->flags & SLAB_POISON) {
		poison_obj(cachep, objp, POISON_FREE);
		slab_kernel_map(cachep, objp, 0, caller);
	}
	return objp;
}

#else
#define kfree_debugcheck(x) do { } while(0)
#define cache_free_debugcheck(x,objp,z) (objp)
#endif

static inline void fixup_objfreelist_debug(struct kmem_cache *cachep,
						void **list)
{
#if DEBUG
	void *next = *list;
	void *objp;

	while (next) {
		objp = next - obj_offset(cachep);
		next = *(void **)next;
		poison_obj(cachep, objp, POISON_FREE);
	}
#endif
}

static inline void fixup_slab_list(struct kmem_cache *cachep,
				struct kmem_cache_node *n, struct page *page,
				void **list)
{
	/* move slabp to correct slabp list: */
	list_del(&page->lru);
	if (page->active == cachep->num) {
		list_add(&page->lru, &n->slabs_full);
		if (OBJFREELIST_SLAB(cachep)) {
#if DEBUG
			/* Poisoning will be done without holding the lock */
			if (cachep->flags & SLAB_POISON) {
				void **objp = page->freelist;

				*objp = *list;
				*list = objp;
			}
#endif
			page->freelist = NULL;
		}
	} else
		list_add(&page->lru, &n->slabs_partial);
}

/* Try to find non-pfmemalloc slab if needed */
static noinline struct page *get_valid_first_slab(struct kmem_cache_node *n,
					struct page *page, bool pfmemalloc)
{
	if (!page)
		return NULL;

	if (pfmemalloc)
		return page;

	if (!PageSlabPfmemalloc(page))
		return page;

	/* No need to keep pfmemalloc slab if we have enough free objects */
	if (n->free_objects > n->free_limit) {
		ClearPageSlabPfmemalloc(page);
		return page;
	}

	/* Move pfmemalloc slab to the end of list to speed up next search */
	list_del(&page->lru);
	if (!page->active)
		list_add_tail(&page->lru, &n->slabs_free);
	else
		list_add_tail(&page->lru, &n->slabs_partial);

	list_for_each_entry(page, &n->slabs_partial, lru) {
		if (!PageSlabPfmemalloc(page))
			return page;
	}

	list_for_each_entry(page, &n->slabs_free, lru) {
		if (!PageSlabPfmemalloc(page))
			return page;
	}

	return NULL;
}

static struct page *get_first_slab(struct kmem_cache_node *n, bool pfmemalloc)
{
	struct page *page;

	page = list_first_entry_or_null(&n->slabs_partial,
			struct page, lru);
	if (!page) {
		n->free_touched = 1;
		page = list_first_entry_or_null(&n->slabs_free,
				struct page, lru);
	}

	if (sk_memalloc_socks())
		return get_valid_first_slab(n, page, pfmemalloc);

	return page;
}

static noinline void *cache_alloc_pfmemalloc(struct kmem_cache *cachep,
				struct kmem_cache_node *n, gfp_t flags)
{
	struct page *page;
	void *obj;
	void *list = NULL;

	if (!gfp_pfmemalloc_allowed(flags))
		return NULL;

	spin_lock(&n->list_lock);
	page = get_first_slab(n, true);
	if (!page) {
		spin_unlock(&n->list_lock);
		return NULL;
	}

	obj = slab_get_obj(cachep, page);
	n->free_objects--;

	fixup_slab_list(cachep, n, page, &list);

	spin_unlock(&n->list_lock);
	fixup_objfreelist_debug(cachep, &list);

	return obj;
}

/*
 * Slab list should be fixed up by fixup_slab_list() for existing slab
 * or cache_grow_end() for new slab
 */
static __always_inline int alloc_block(struct kmem_cache *cachep,
		struct array_cache *ac, struct page *page, int batchcount)
{
	/*
	 * There must be at least one object available for
	 * allocation.
	 */
	BUG_ON(page->active >= cachep->num);

	while (page->active < cachep->num && batchcount--) {
		STATS_INC_ALLOCED(cachep);
		STATS_INC_ACTIVE(cachep);
		STATS_SET_HIGH(cachep);

		ac->entry[ac->avail++] = slab_get_obj(cachep, page);
	}

	return batchcount;
}

static void *cache_alloc_refill(struct kmem_cache *cachep, gfp_t flags)
{
	int batchcount;
	struct kmem_cache_node *n;
	struct array_cache *ac, *shared;
	int node;
	void *list = NULL;
	struct page *page;

	check_irq_off();
	node = numa_mem_id();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ac = cpu_cache_get(cachep);
	batchcount = ac->batchcount;
	if (!ac->touched && batchcount > BATCHREFILL_LIMIT) {
		/*
		 * If there was little recent activity on this cache, then
		 * perform only a partial refill.  Otherwise we could generate
		 * refill bouncing.
		 */
		batchcount = BATCHREFILL_LIMIT;
	}
<<<<<<< HEAD
	n = cachep->node[node];

	BUG_ON(ac->avail > 0 || !n);
	spin_lock(&n->list_lock);

	/* See if we can refill from the shared array */
	if (n->shared && transfer_objects(ac, n->shared, batchcount)) {
		n->shared->touched = 1;
=======
	n = get_node(cachep, node);

	BUG_ON(ac->avail > 0 || !n);
	shared = READ_ONCE(n->shared);
	if (!n->free_objects && (!shared || !shared->avail))
		goto direct_grow;

	spin_lock(&n->list_lock);
	shared = READ_ONCE(n->shared);

	/* See if we can refill from the shared array */
	if (shared && transfer_objects(ac, shared, batchcount)) {
		shared->touched = 1;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto alloc_done;
	}

	while (batchcount > 0) {
<<<<<<< HEAD
		struct list_head *entry;
		struct slab *slabp;
		/* Get slab alloc is to come from. */
		entry = n->slabs_partial.next;
		if (entry == &n->slabs_partial) {
			n->free_touched = 1;
			entry = n->slabs_free.next;
			if (entry == &n->slabs_free)
				goto must_grow;
		}

		slabp = list_entry(entry, struct slab, list);
		check_slabp(cachep, slabp);
		check_spinlock_acquired(cachep);

		/*
		 * The slab was either on partial or free list so
		 * there must be at least one object available for
		 * allocation.
		 */
		BUG_ON(slabp->inuse >= cachep->num);

		while (slabp->inuse < cachep->num && batchcount--) {
			STATS_INC_ALLOCED(cachep);
			STATS_INC_ACTIVE(cachep);
			STATS_SET_HIGH(cachep);

			ac_put_obj(cachep, ac, slab_get_obj(cachep, slabp,
									node));
		}
		check_slabp(cachep, slabp);

		/* move slabp to correct slabp list: */
		list_del(&slabp->list);
		if (slabp->free == BUFCTL_END)
			list_add(&slabp->list, &n->slabs_full);
		else
			list_add(&slabp->list, &n->slabs_partial);
=======
		/* Get slab alloc is to come from. */
		page = get_first_slab(n, false);
		if (!page)
			goto must_grow;

		check_spinlock_acquired(cachep);

		batchcount = alloc_block(cachep, ac, page, batchcount);
		fixup_slab_list(cachep, n, page, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

must_grow:
	n->free_objects -= ac->avail;
alloc_done:
	spin_unlock(&n->list_lock);
<<<<<<< HEAD

	if (unlikely(!ac->avail)) {
		int x;
force_grow:
		x = cache_grow(cachep, flags | GFP_THISNODE, node, NULL);

		/* cache_grow can reenable interrupts, then ac could change. */
		ac = cpu_cache_get(cachep);
		node = numa_mem_id();

		/* no objects in sight? abort */
		if (!x && (ac->avail == 0 || force_refill))
			return NULL;

		if (!ac->avail)		/* objects refilled by interrupt? */
			goto retry;
	}
	ac->touched = 1;

	return ac_get_obj(cachep, ac, flags, force_refill);
=======
	fixup_objfreelist_debug(cachep, &list);

direct_grow:
	if (unlikely(!ac->avail)) {
		/* Check if we can use obj in pfmemalloc slab */
		if (sk_memalloc_socks()) {
			void *obj = cache_alloc_pfmemalloc(cachep, n, flags);

			if (obj)
				return obj;
		}

		page = cache_grow_begin(cachep, gfp_exact_node(flags), node);

		/*
		 * cache_grow_begin() can reenable interrupts,
		 * then ac could change.
		 */
		ac = cpu_cache_get(cachep);
		if (!ac->avail && page)
			alloc_block(cachep, ac, page, batchcount);
		cache_grow_end(cachep, page);

		if (!ac->avail)
			return NULL;
	}
	ac->touched = 1;

	return ac->entry[--ac->avail];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void cache_alloc_debugcheck_before(struct kmem_cache *cachep,
						gfp_t flags)
{
<<<<<<< HEAD
	might_sleep_if(flags & __GFP_WAIT);
#if DEBUG
	kmem_flagcheck(cachep, flags);
#endif
=======
	might_sleep_if(gfpflags_allow_blocking(flags));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#if DEBUG
static void *cache_alloc_debugcheck_after(struct kmem_cache *cachep,
				gfp_t flags, void *objp, unsigned long caller)
{
	if (!objp)
		return objp;
	if (cachep->flags & SLAB_POISON) {
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_PAGEALLOC
		if ((cachep->size % PAGE_SIZE) == 0 && OFF_SLAB(cachep))
			kernel_map_pages(virt_to_page(objp),
					 cachep->size / PAGE_SIZE, 1);
		else
			check_poison_obj(cachep, objp);
#else
		check_poison_obj(cachep, objp);
#endif
=======
		check_poison_obj(cachep, objp);
		slab_kernel_map(cachep, objp, 1, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		poison_obj(cachep, objp, POISON_INUSE);
	}
	if (cachep->flags & SLAB_STORE_USER)
		*dbg_userword(cachep, objp) = (void *)caller;

	if (cachep->flags & SLAB_RED_ZONE) {
		if (*dbg_redzone1(cachep, objp) != RED_INACTIVE ||
				*dbg_redzone2(cachep, objp) != RED_INACTIVE) {
<<<<<<< HEAD
			slab_error(cachep, "double free, or memory outside"
						" object was overwritten");
			printk(KERN_ERR
				"%p: redzone 1:0x%llx, redzone 2:0x%llx\n",
				objp, *dbg_redzone1(cachep, objp),
				*dbg_redzone2(cachep, objp));
=======
			slab_error(cachep, "double free, or memory outside object was overwritten");
			pr_err("%p: redzone 1:0x%llx, redzone 2:0x%llx\n",
			       objp, *dbg_redzone1(cachep, objp),
			       *dbg_redzone2(cachep, objp));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		*dbg_redzone1(cachep, objp) = RED_ACTIVE;
		*dbg_redzone2(cachep, objp) = RED_ACTIVE;
	}
<<<<<<< HEAD
#ifdef CONFIG_DEBUG_SLAB_LEAK
	{
		struct slab *slabp;
		unsigned objnr;

		slabp = virt_to_head_page(objp)->slab_page;
		objnr = (unsigned)(objp - slabp->s_mem) / cachep->size;
		slab_bufctl(slabp)[objnr] = BUFCTL_ACTIVE;
	}
#endif
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	objp += obj_offset(cachep);
	if (cachep->ctor && cachep->flags & SLAB_POISON)
		cachep->ctor(objp);
	if (ARCH_SLAB_MINALIGN &&
	    ((unsigned long)objp & (ARCH_SLAB_MINALIGN-1))) {
<<<<<<< HEAD
		printk(KERN_ERR "0x%p: not aligned to ARCH_SLAB_MINALIGN=%d\n",
=======
		pr_err("0x%p: not aligned to ARCH_SLAB_MINALIGN=%d\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       objp, (int)ARCH_SLAB_MINALIGN);
	}
	return objp;
}
#else
#define cache_alloc_debugcheck_after(a,b,objp,d) (objp)
#endif

<<<<<<< HEAD
static bool slab_should_failslab(struct kmem_cache *cachep, gfp_t flags)
{
	if (cachep == kmem_cache)
		return false;

	return should_failslab(cachep->object_size, flags, cachep->flags);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void *____cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
	void *objp;
	struct array_cache *ac;
<<<<<<< HEAD
	bool force_refill = false;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	check_irq_off();

	ac = cpu_cache_get(cachep);
	if (likely(ac->avail)) {
		ac->touched = 1;
<<<<<<< HEAD
		objp = ac_get_obj(cachep, ac, flags, false);

		/*
		 * Allow for the possibility all avail objects are not allowed
		 * by the current flags
		 */
		if (objp) {
			STATS_INC_ALLOCHIT(cachep);
			goto out;
		}
		force_refill = true;
	}

	STATS_INC_ALLOCMISS(cachep);
	objp = cache_alloc_refill(cachep, flags, force_refill);
=======
		objp = ac->entry[--ac->avail];

		STATS_INC_ALLOCHIT(cachep);
		goto out;
	}

	STATS_INC_ALLOCMISS(cachep);
	objp = cache_alloc_refill(cachep, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * the 'ac' may be updated by cache_alloc_refill(),
	 * and kmemleak_erase() requires its correct value.
	 */
	ac = cpu_cache_get(cachep);

out:
	/*
	 * To avoid a false negative, if an object that is in one of the
	 * per-CPU caches is leaked, we need to make sure kmemleak doesn't
	 * treat the array pointers as a reference to the object.
	 */
	if (objp)
		kmemleak_erase(&ac->entry[ac->avail]);
	return objp;
}

#ifdef CONFIG_NUMA
/*
<<<<<<< HEAD
 * Try allocating on another node if PF_SPREAD_SLAB|PF_MEMPOLICY.
=======
 * Try allocating on another node if PFA_SPREAD_SLAB is a mempolicy is set.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * If we are in_interrupt, then process context, including cpusets and
 * mempolicy, may not apply and should not be used for allocation policy.
 */
static void *alternate_node_alloc(struct kmem_cache *cachep, gfp_t flags)
{
	int nid_alloc, nid_here;

	if (in_interrupt() || (flags & __GFP_THISNODE))
		return NULL;
	nid_alloc = nid_here = numa_mem_id();
	if (cpuset_do_slab_mem_spread() && (cachep->flags & SLAB_MEM_SPREAD))
		nid_alloc = cpuset_slab_spread_node();
	else if (current->mempolicy)
<<<<<<< HEAD
		nid_alloc = slab_node();
=======
		nid_alloc = mempolicy_slab_node();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (nid_alloc != nid_here)
		return ____cache_alloc_node(cachep, flags, nid_alloc);
	return NULL;
}

/*
 * Fallback function if there was no memory available and no objects on a
 * certain node and fall back is permitted. First we scan all the
 * available node for available objects. If that fails then we
 * perform an allocation without specifying a node. This allows the page
 * allocator to do its reclaim / fallback magic. We then insert the
 * slab into the proper nodelist and then allocate from it.
 */
static void *fallback_alloc(struct kmem_cache *cache, gfp_t flags)
{
	struct zonelist *zonelist;
<<<<<<< HEAD
	gfp_t local_flags;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct zoneref *z;
	struct zone *zone;
	enum zone_type high_zoneidx = gfp_zone(flags);
	void *obj = NULL;
<<<<<<< HEAD
=======
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int nid;
	unsigned int cpuset_mems_cookie;

	if (flags & __GFP_THISNODE)
		return NULL;

<<<<<<< HEAD
	local_flags = flags & (GFP_CONSTRAINT_MASK|GFP_RECLAIM_MASK);

retry_cpuset:
	cpuset_mems_cookie = get_mems_allowed();
	zonelist = node_zonelist(slab_node(), flags);
=======
retry_cpuset:
	cpuset_mems_cookie = read_mems_allowed_begin();
	zonelist = node_zonelist(mempolicy_slab_node(), flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

retry:
	/*
	 * Look through allowed nodes for objects available
	 * from existing per node queues.
	 */
	for_each_zone_zonelist(zone, z, zonelist, high_zoneidx) {
		nid = zone_to_nid(zone);

<<<<<<< HEAD
		if (cpuset_zone_allowed_hardwall(zone, flags) &&
			cache->node[nid] &&
			cache->node[nid]->free_objects) {
				obj = ____cache_alloc_node(cache,
					flags | GFP_THISNODE, nid);
=======
		if (cpuset_zone_allowed(zone, flags) &&
			get_node(cache, nid) &&
			get_node(cache, nid)->free_objects) {
				obj = ____cache_alloc_node(cache,
					gfp_exact_node(flags), nid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (obj)
					break;
		}
	}

	if (!obj) {
		/*
		 * This allocation will be performed within the constraints
		 * of the current cpuset / memory policy requirements.
		 * We may trigger various forms of reclaim on the allowed
		 * set and go into memory reserves if necessary.
		 */
<<<<<<< HEAD
		if (local_flags & __GFP_WAIT)
			local_irq_enable();
		kmem_flagcheck(cache, flags);
		obj = kmem_getpages(cache, local_flags, numa_mem_id());
		if (local_flags & __GFP_WAIT)
			local_irq_disable();
		if (obj) {
			/*
			 * Insert into the appropriate per node queues
			 */
			nid = page_to_nid(virt_to_page(obj));
			if (cache_grow(cache, flags, nid, obj)) {
				obj = ____cache_alloc_node(cache,
					flags | GFP_THISNODE, nid);
				if (!obj)
					/*
					 * Another processor may allocate the
					 * objects in the slab since we are
					 * not holding any locks.
					 */
					goto retry;
			} else {
				/* cache_grow already freed obj */
				obj = NULL;
			}
		}
	}

	if (unlikely(!put_mems_allowed(cpuset_mems_cookie) && !obj))
=======
		page = cache_grow_begin(cache, flags, numa_mem_id());
		cache_grow_end(cache, page);
		if (page) {
			nid = page_to_nid(page);
			obj = ____cache_alloc_node(cache,
				gfp_exact_node(flags), nid);

			/*
			 * Another processor may allocate the objects in
			 * the slab since we are not holding any locks.
			 */
			if (!obj)
				goto retry;
		}
	}

	if (unlikely(!obj && read_mems_allowed_retry(cpuset_mems_cookie)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		goto retry_cpuset;
	return obj;
}

/*
 * A interface to enable slab creation on nodeid
 */
static void *____cache_alloc_node(struct kmem_cache *cachep, gfp_t flags,
				int nodeid)
{
<<<<<<< HEAD
	struct list_head *entry;
	struct slab *slabp;
	struct kmem_cache_node *n;
	void *obj;
	int x;

	VM_BUG_ON(nodeid > num_online_nodes());
	n = cachep->node[nodeid];
	BUG_ON(!n);

retry:
	check_irq_off();
	spin_lock(&n->list_lock);
	entry = n->slabs_partial.next;
	if (entry == &n->slabs_partial) {
		n->free_touched = 1;
		entry = n->slabs_free.next;
		if (entry == &n->slabs_free)
			goto must_grow;
	}

	slabp = list_entry(entry, struct slab, list);
	check_spinlock_acquired_node(cachep, nodeid);
	check_slabp(cachep, slabp);
=======
	struct page *page;
	struct kmem_cache_node *n;
	void *obj = NULL;
	void *list = NULL;

	VM_BUG_ON(nodeid < 0 || nodeid >= MAX_NUMNODES);
	n = get_node(cachep, nodeid);
	BUG_ON(!n);

	check_irq_off();
	spin_lock(&n->list_lock);
	page = get_first_slab(n, false);
	if (!page)
		goto must_grow;

	check_spinlock_acquired_node(cachep, nodeid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	STATS_INC_NODEALLOCS(cachep);
	STATS_INC_ACTIVE(cachep);
	STATS_SET_HIGH(cachep);

<<<<<<< HEAD
	BUG_ON(slabp->inuse == cachep->num);

	obj = slab_get_obj(cachep, slabp, nodeid);
	check_slabp(cachep, slabp);
	n->free_objects--;
	/* move slabp to correct slabp list: */
	list_del(&slabp->list);

	if (slabp->free == BUFCTL_END)
		list_add(&slabp->list, &n->slabs_full);
	else
		list_add(&slabp->list, &n->slabs_partial);

	spin_unlock(&n->list_lock);
	goto done;

must_grow:
	spin_unlock(&n->list_lock);
	x = cache_grow(cachep, flags | GFP_THISNODE, nodeid, NULL);
	if (x)
		goto retry;

	return fallback_alloc(cachep, flags);

done:
	return obj;
}

/**
 * kmem_cache_alloc_node - Allocate an object on the specified node
 * @cachep: The cache to allocate from.
 * @flags: See kmalloc().
 * @nodeid: node number of the target node.
 * @caller: return address of caller, used for debug information
 *
 * Identical to kmem_cache_alloc but it will allocate memory on the given
 * node, which can improve the performance for cpu bound structures.
 *
 * Fallback to other node is possible if __GFP_THISNODE is not set.
 */
=======
	BUG_ON(page->active == cachep->num);

	obj = slab_get_obj(cachep, page);
	n->free_objects--;

	fixup_slab_list(cachep, n, page, &list);

	spin_unlock(&n->list_lock);
	fixup_objfreelist_debug(cachep, &list);
	return obj;

must_grow:
	spin_unlock(&n->list_lock);
	page = cache_grow_begin(cachep, gfp_exact_node(flags), nodeid);
	if (page) {
		/* This slab isn't counted yet so don't update free_objects */
		obj = slab_get_obj(cachep, page);
	}
	cache_grow_end(cachep, page);

	return obj ? obj : fallback_alloc(cachep, flags);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static __always_inline void *
slab_alloc_node(struct kmem_cache *cachep, gfp_t flags, int nodeid,
		   unsigned long caller)
{
	unsigned long save_flags;
	void *ptr;
	int slab_node = numa_mem_id();

	flags &= gfp_allowed_mask;
<<<<<<< HEAD

	lockdep_trace_alloc(flags);

	if (slab_should_failslab(cachep, flags))
		return NULL;

	cachep = memcg_kmem_get_cache(cachep, flags);

=======
	cachep = slab_pre_alloc_hook(cachep, flags);
	if (unlikely(!cachep))
		return NULL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cache_alloc_debugcheck_before(cachep, flags);
	local_irq_save(save_flags);

	if (nodeid == NUMA_NO_NODE)
		nodeid = slab_node;

<<<<<<< HEAD
	if (unlikely(!cachep->node[nodeid])) {
=======
	if (unlikely(!get_node(cachep, nodeid))) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/* Node not bootstrapped yet */
		ptr = fallback_alloc(cachep, flags);
		goto out;
	}

	if (nodeid == slab_node) {
		/*
		 * Use the locally cached objects if possible.
		 * However ____cache_alloc does not allow fallback
		 * to other nodes. It may fail while we still have
		 * objects on other nodes available.
		 */
		ptr = ____cache_alloc(cachep, flags);
		if (ptr)
			goto out;
	}
	/* ___cache_alloc_node can fall back to other nodes */
	ptr = ____cache_alloc_node(cachep, flags, nodeid);
  out:
	local_irq_restore(save_flags);
	ptr = cache_alloc_debugcheck_after(cachep, flags, ptr, caller);
<<<<<<< HEAD
	kmemleak_alloc_recursive(ptr, cachep->object_size, 1, cachep->flags,
				 flags);

	if (likely(ptr))
		kmemcheck_slab_alloc(cachep, flags, ptr, cachep->object_size);

	if (unlikely((flags & __GFP_ZERO) && ptr))
		memset(ptr, 0, cachep->object_size);

=======

	if (unlikely(flags & __GFP_ZERO) && ptr)
		memset(ptr, 0, cachep->object_size);

	slab_post_alloc_hook(cachep, flags, 1, &ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ptr;
}

static __always_inline void *
__do_cache_alloc(struct kmem_cache *cache, gfp_t flags)
{
	void *objp;

<<<<<<< HEAD
	if (unlikely(current->flags & (PF_SPREAD_SLAB | PF_MEMPOLICY))) {
=======
	if (current->mempolicy || cpuset_do_slab_mem_spread()) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		objp = alternate_node_alloc(cache, flags);
		if (objp)
			goto out;
	}
	objp = ____cache_alloc(cache, flags);

	/*
	 * We may just have run out of memory on the local node.
	 * ____cache_alloc_node() knows how to locate memory on other nodes
	 */
	if (!objp)
		objp = ____cache_alloc_node(cache, flags, numa_mem_id());

  out:
	return objp;
}
#else

static __always_inline void *
__do_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
	return ____cache_alloc(cachep, flags);
}

#endif /* CONFIG_NUMA */

static __always_inline void *
slab_alloc(struct kmem_cache *cachep, gfp_t flags, unsigned long caller)
{
	unsigned long save_flags;
	void *objp;

	flags &= gfp_allowed_mask;
<<<<<<< HEAD

	lockdep_trace_alloc(flags);

	if (slab_should_failslab(cachep, flags))
		return NULL;

	cachep = memcg_kmem_get_cache(cachep, flags);

=======
	cachep = slab_pre_alloc_hook(cachep, flags);
	if (unlikely(!cachep))
		return NULL;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cache_alloc_debugcheck_before(cachep, flags);
	local_irq_save(save_flags);
	objp = __do_cache_alloc(cachep, flags);
	local_irq_restore(save_flags);
	objp = cache_alloc_debugcheck_after(cachep, flags, objp, caller);
<<<<<<< HEAD
	kmemleak_alloc_recursive(objp, cachep->object_size, 1, cachep->flags,
				 flags);
	prefetchw(objp);

	if (likely(objp))
		kmemcheck_slab_alloc(cachep, flags, objp, cachep->object_size);

	if (unlikely((flags & __GFP_ZERO) && objp))
		memset(objp, 0, cachep->object_size);

=======
	prefetchw(objp);

	if (unlikely(flags & __GFP_ZERO) && objp)
		memset(objp, 0, cachep->object_size);

	slab_post_alloc_hook(cachep, flags, 1, &objp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return objp;
}

/*
<<<<<<< HEAD
 * Caller needs to acquire correct kmem_list's list_lock
 */
static void free_block(struct kmem_cache *cachep, void **objpp, int nr_objects,
		       int node)
{
	int i;
	struct kmem_cache_node *n;

	for (i = 0; i < nr_objects; i++) {
		void *objp;
		struct slab *slabp;

		clear_obj_pfmemalloc(&objpp[i]);
		objp = objpp[i];

		slabp = virt_to_slab(objp);
		n = cachep->node[node];
		list_del(&slabp->list);
		check_spinlock_acquired_node(cachep, node);
		check_slabp(cachep, slabp);
		slab_put_obj(cachep, slabp, objp, node);
		STATS_DEC_ACTIVE(cachep);
		n->free_objects++;
		check_slabp(cachep, slabp);

		/* fixup slab chains */
		if (slabp->inuse == 0) {
			if (n->free_objects > n->free_limit) {
				n->free_objects -= cachep->num;
				/* No need to drop any previously held
				 * lock here, even if we have a off-slab slab
				 * descriptor it is guaranteed to come from
				 * a different cache, refer to comments before
				 * alloc_slabmgmt.
				 */
				slab_destroy(cachep, slabp);
			} else {
				list_add(&slabp->list, &n->slabs_free);
			}
		} else {
=======
 * Caller needs to acquire correct kmem_cache_node's list_lock
 * @list: List of detached free slabs should be freed by caller
 */
static void free_block(struct kmem_cache *cachep, void **objpp,
			int nr_objects, int node, struct list_head *list)
{
	int i;
	struct kmem_cache_node *n = get_node(cachep, node);
	struct page *page;

	n->free_objects += nr_objects;

	for (i = 0; i < nr_objects; i++) {
		void *objp;
		struct page *page;

		objp = objpp[i];

		page = virt_to_head_page(objp);
		list_del(&page->lru);
		check_spinlock_acquired_node(cachep, node);
		slab_put_obj(cachep, page, objp);
		STATS_DEC_ACTIVE(cachep);

		/* fixup slab chains */
		if (page->active == 0)
			list_add(&page->lru, &n->slabs_free);
		else {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* Unconditionally move a slab to the end of the
			 * partial list on free - maximum time for the
			 * other objects to be freed, too.
			 */
<<<<<<< HEAD
			list_add_tail(&slabp->list, &n->slabs_partial);
		}
	}
=======
			list_add_tail(&page->lru, &n->slabs_partial);
		}
	}

	while (n->free_objects > n->free_limit && !list_empty(&n->slabs_free)) {
		n->free_objects -= cachep->num;

		page = list_last_entry(&n->slabs_free, struct page, lru);
		list_move(&page->lru, list);
		n->num_slabs--;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void cache_flusharray(struct kmem_cache *cachep, struct array_cache *ac)
{
	int batchcount;
	struct kmem_cache_node *n;
	int node = numa_mem_id();
<<<<<<< HEAD

	batchcount = ac->batchcount;
#if DEBUG
	BUG_ON(!batchcount || batchcount > ac->avail);
#endif
	check_irq_off();
	n = cachep->node[node];
=======
	LIST_HEAD(list);

	batchcount = ac->batchcount;

	check_irq_off();
	n = get_node(cachep, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_lock(&n->list_lock);
	if (n->shared) {
		struct array_cache *shared_array = n->shared;
		int max = shared_array->limit - shared_array->avail;
		if (max) {
			if (batchcount > max)
				batchcount = max;
			memcpy(&(shared_array->entry[shared_array->avail]),
			       ac->entry, sizeof(void *) * batchcount);
			shared_array->avail += batchcount;
			goto free_done;
		}
	}

<<<<<<< HEAD
	free_block(cachep, ac->entry, batchcount, node);
=======
	free_block(cachep, ac->entry, batchcount, node, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
free_done:
#if STATS
	{
		int i = 0;
<<<<<<< HEAD
		struct list_head *p;

		p = n->slabs_free.next;
		while (p != &(n->slabs_free)) {
			struct slab *slabp;

			slabp = list_entry(p, struct slab, list);
			BUG_ON(slabp->inuse);

			i++;
			p = p->next;
=======
		struct page *page;

		list_for_each_entry(page, &n->slabs_free, lru) {
			BUG_ON(page->active);

			i++;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}
		STATS_SET_FREEABLE(cachep, i);
	}
#endif
	spin_unlock(&n->list_lock);
<<<<<<< HEAD
=======
	slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ac->avail -= batchcount;
	memmove(ac->entry, &(ac->entry[batchcount]), sizeof(void *)*ac->avail);
}

/*
 * Release an obj back to its cache. If the obj has a constructed state, it must
 * be in this state _before_ it is released.  Called with disabled ints.
 */
static inline void __cache_free(struct kmem_cache *cachep, void *objp,
				unsigned long caller)
{
<<<<<<< HEAD
=======
	/* Put the object into the quarantine, don't touch it for now. */
	if (kasan_slab_free(cachep, objp))
		return;

	___cache_free(cachep, objp, caller);
}

void ___cache_free(struct kmem_cache *cachep, void *objp,
		unsigned long caller)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct array_cache *ac = cpu_cache_get(cachep);

	check_irq_off();
	kmemleak_free_recursive(objp, cachep->flags);
	objp = cache_free_debugcheck(cachep, objp, caller);

	kmemcheck_slab_free(cachep, objp, cachep->object_size);

	/*
	 * Skip calling cache_free_alien() when the platform is not numa.
	 * This will avoid cache misses that happen while accessing slabp (which
	 * is per page memory  reference) to get nodeid. Instead use a global
	 * variable to skip the call, which is mostly likely to be present in
	 * the cache.
	 */
	if (nr_online_nodes > 1 && cache_free_alien(cachep, objp))
		return;

<<<<<<< HEAD
	if (likely(ac->avail < ac->limit)) {
=======
	if (ac->avail < ac->limit) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		STATS_INC_FREEHIT(cachep);
	} else {
		STATS_INC_FREEMISS(cachep);
		cache_flusharray(cachep, ac);
	}

<<<<<<< HEAD
	ac_put_obj(cachep, ac, objp);
=======
	if (sk_memalloc_socks()) {
		struct page *page = virt_to_head_page(objp);

		if (unlikely(PageSlabPfmemalloc(page))) {
			cache_free_pfmemalloc(cachep, page, objp);
			return;
		}
	}

	ac->entry[ac->avail++] = objp;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * kmem_cache_alloc - Allocate an object
 * @cachep: The cache to allocate from.
 * @flags: See kmalloc().
 *
 * Allocate an object from this cache.  The flags are only relevant
 * if the cache has no available objects.
 */
void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
	void *ret = slab_alloc(cachep, flags, _RET_IP_);

<<<<<<< HEAD
=======
	kasan_slab_alloc(cachep, ret, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	trace_kmem_cache_alloc(_RET_IP_, ret,
			       cachep->object_size, cachep->size, flags);

	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc);

<<<<<<< HEAD
=======
static __always_inline void
cache_alloc_debugcheck_after_bulk(struct kmem_cache *s, gfp_t flags,
				  size_t size, void **p, unsigned long caller)
{
	size_t i;

	for (i = 0; i < size; i++)
		p[i] = cache_alloc_debugcheck_after(s, flags, p[i], caller);
}

int kmem_cache_alloc_bulk(struct kmem_cache *s, gfp_t flags, size_t size,
			  void **p)
{
	size_t i;

	s = slab_pre_alloc_hook(s, flags);
	if (!s)
		return 0;

	cache_alloc_debugcheck_before(s, flags);

	local_irq_disable();
	for (i = 0; i < size; i++) {
		void *objp = __do_cache_alloc(s, flags);

		if (unlikely(!objp))
			goto error;
		p[i] = objp;
	}
	local_irq_enable();

	cache_alloc_debugcheck_after_bulk(s, flags, size, p, _RET_IP_);

	/* Clear memory outside IRQ disabled section */
	if (unlikely(flags & __GFP_ZERO))
		for (i = 0; i < size; i++)
			memset(p[i], 0, s->object_size);

	slab_post_alloc_hook(s, flags, size, p);
	/* FIXME: Trace call missing. Christoph would like a bulk variant */
	return size;
error:
	local_irq_enable();
	cache_alloc_debugcheck_after_bulk(s, flags, i, p, _RET_IP_);
	slab_post_alloc_hook(s, flags, i, p);
	__kmem_cache_free_bulk(s, i, p);
	return 0;
}
EXPORT_SYMBOL(kmem_cache_alloc_bulk);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_TRACING
void *
kmem_cache_alloc_trace(struct kmem_cache *cachep, gfp_t flags, size_t size)
{
	void *ret;

	ret = slab_alloc(cachep, flags, _RET_IP_);

<<<<<<< HEAD
=======
	kasan_kmalloc(cachep, ret, size, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	trace_kmalloc(_RET_IP_, ret,
		      size, cachep->size, flags);
	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_trace);
#endif

#ifdef CONFIG_NUMA
<<<<<<< HEAD
=======
/**
 * kmem_cache_alloc_node - Allocate an object on the specified node
 * @cachep: The cache to allocate from.
 * @flags: See kmalloc().
 * @nodeid: node number of the target node.
 *
 * Identical to kmem_cache_alloc but it will allocate memory on the given
 * node, which can improve the performance for cpu bound structures.
 *
 * Fallback to other node is possible if __GFP_THISNODE is not set.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void *kmem_cache_alloc_node(struct kmem_cache *cachep, gfp_t flags, int nodeid)
{
	void *ret = slab_alloc_node(cachep, flags, nodeid, _RET_IP_);

<<<<<<< HEAD
=======
	kasan_slab_alloc(cachep, ret, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	trace_kmem_cache_alloc_node(_RET_IP_, ret,
				    cachep->object_size, cachep->size,
				    flags, nodeid);

	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_node);

#ifdef CONFIG_TRACING
void *kmem_cache_alloc_node_trace(struct kmem_cache *cachep,
				  gfp_t flags,
				  int nodeid,
				  size_t size)
{
	void *ret;

	ret = slab_alloc_node(cachep, flags, nodeid, _RET_IP_);

<<<<<<< HEAD
=======
	kasan_kmalloc(cachep, ret, size, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	trace_kmalloc_node(_RET_IP_, ret,
			   size, cachep->size,
			   flags, nodeid);
	return ret;
}
EXPORT_SYMBOL(kmem_cache_alloc_node_trace);
#endif

static __always_inline void *
__do_kmalloc_node(size_t size, gfp_t flags, int node, unsigned long caller)
{
	struct kmem_cache *cachep;
<<<<<<< HEAD

	cachep = kmalloc_slab(size, flags);
	if (unlikely(ZERO_OR_NULL_PTR(cachep)))
		return cachep;
	return kmem_cache_alloc_node_trace(cachep, flags, node, size);
}

#if defined(CONFIG_DEBUG_SLAB) || defined(CONFIG_TRACING)
=======
	void *ret;

	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE))
		return NULL;
	cachep = kmalloc_slab(size, flags);
	if (unlikely(ZERO_OR_NULL_PTR(cachep)))
		return cachep;
	ret = kmem_cache_alloc_node_trace(cachep, flags, node, size);
	kasan_kmalloc(cachep, ret, size, flags);

	return ret;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
	return __do_kmalloc_node(size, flags, node, _RET_IP_);
}
EXPORT_SYMBOL(__kmalloc_node);

void *__kmalloc_node_track_caller(size_t size, gfp_t flags,
		int node, unsigned long caller)
{
	return __do_kmalloc_node(size, flags, node, caller);
}
EXPORT_SYMBOL(__kmalloc_node_track_caller);
<<<<<<< HEAD
#else
void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
	return __do_kmalloc_node(size, flags, node, 0);
}
EXPORT_SYMBOL(__kmalloc_node);
#endif /* CONFIG_DEBUG_SLAB || CONFIG_TRACING */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_NUMA */

/**
 * __do_kmalloc - allocate memory
 * @size: how many bytes of memory are required.
 * @flags: the type of memory to allocate (see kmalloc).
 * @caller: function caller for debug tracking of the caller
 */
static __always_inline void *__do_kmalloc(size_t size, gfp_t flags,
					  unsigned long caller)
{
	struct kmem_cache *cachep;
	void *ret;

<<<<<<< HEAD
	/* If you want to save a few bytes .text space: replace
	 * __ with kmem_.
	 * Then kmalloc uses the uninlined functions instead of the inline
	 * functions.
	 */
=======
	if (unlikely(size > KMALLOC_MAX_CACHE_SIZE))
		return NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	cachep = kmalloc_slab(size, flags);
	if (unlikely(ZERO_OR_NULL_PTR(cachep)))
		return cachep;
	ret = slab_alloc(cachep, flags, caller);

<<<<<<< HEAD
=======
	kasan_kmalloc(cachep, ret, size, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	trace_kmalloc(caller, ret,
		      size, cachep->size, flags);

	return ret;
}

<<<<<<< HEAD

#if defined(CONFIG_DEBUG_SLAB) || defined(CONFIG_TRACING)
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void *__kmalloc(size_t size, gfp_t flags)
{
	return __do_kmalloc(size, flags, _RET_IP_);
}
EXPORT_SYMBOL(__kmalloc);

void *__kmalloc_track_caller(size_t size, gfp_t flags, unsigned long caller)
{
	return __do_kmalloc(size, flags, caller);
}
EXPORT_SYMBOL(__kmalloc_track_caller);

<<<<<<< HEAD
#else
void *__kmalloc(size_t size, gfp_t flags)
{
	return __do_kmalloc(size, flags, 0);
}
EXPORT_SYMBOL(__kmalloc);
#endif

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * kmem_cache_free - Deallocate an object
 * @cachep: The cache the allocation was from.
 * @objp: The previously allocated object.
 *
 * Free an object which was previously allocated from this
 * cache.
 */
void kmem_cache_free(struct kmem_cache *cachep, void *objp)
{
	unsigned long flags;
	cachep = cache_from_obj(cachep, objp);
	if (!cachep)
		return;

	local_irq_save(flags);
	debug_check_no_locks_freed(objp, cachep->object_size);
	if (!(cachep->flags & SLAB_DEBUG_OBJECTS))
		debug_check_no_obj_freed(objp, cachep->object_size);
	__cache_free(cachep, objp, _RET_IP_);
	local_irq_restore(flags);

	trace_kmem_cache_free(_RET_IP_, objp);
}
EXPORT_SYMBOL(kmem_cache_free);

<<<<<<< HEAD
=======
void kmem_cache_free_bulk(struct kmem_cache *orig_s, size_t size, void **p)
{
	struct kmem_cache *s;
	size_t i;

	local_irq_disable();
	for (i = 0; i < size; i++) {
		void *objp = p[i];

		if (!orig_s) /* called via kfree_bulk */
			s = virt_to_cache(objp);
		else
			s = cache_from_obj(orig_s, objp);

		debug_check_no_locks_freed(objp, s->object_size);
		if (!(s->flags & SLAB_DEBUG_OBJECTS))
			debug_check_no_obj_freed(objp, s->object_size);

		__cache_free(s, objp, _RET_IP_);
	}
	local_irq_enable();

	/* FIXME: add tracing */
}
EXPORT_SYMBOL(kmem_cache_free_bulk);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * kfree - free previously allocated memory
 * @objp: pointer returned by kmalloc.
 *
 * If @objp is NULL, no operation is performed.
 *
 * Don't free memory not originally allocated by kmalloc()
 * or you will run into trouble.
 */
void kfree(const void *objp)
{
	struct kmem_cache *c;
	unsigned long flags;

	trace_kfree(_RET_IP_, objp);

	if (unlikely(ZERO_OR_NULL_PTR(objp)))
		return;
	local_irq_save(flags);
	kfree_debugcheck(objp);
	c = virt_to_cache(objp);
	debug_check_no_locks_freed(objp, c->object_size);

	debug_check_no_obj_freed(objp, c->object_size);
	__cache_free(c, (void *)objp, _RET_IP_);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(kfree);

/*
 * This initializes kmem_cache_node or resizes various caches for all nodes.
 */
<<<<<<< HEAD
static int alloc_kmemlist(struct kmem_cache *cachep, gfp_t gfp)
{
	int node;
	struct kmem_cache_node *n;
	struct array_cache *new_shared;
	struct array_cache **new_alien = NULL;

	for_each_online_node(node) {

                if (use_alien_caches) {
                        new_alien = alloc_alien_cache(node, cachep->limit, gfp);
                        if (!new_alien)
                                goto fail;
                }

		new_shared = NULL;
		if (cachep->shared) {
			new_shared = alloc_arraycache(node,
				cachep->shared*cachep->batchcount,
					0xbaadf00d, gfp);
			if (!new_shared) {
				free_alien_cache(new_alien);
				goto fail;
			}
		}

		n = cachep->node[node];
		if (n) {
			struct array_cache *shared = n->shared;

			spin_lock_irq(&n->list_lock);

			if (shared)
				free_block(cachep, shared->entry,
						shared->avail, node);

			n->shared = new_shared;
			if (!n->alien) {
				n->alien = new_alien;
				new_alien = NULL;
			}
			n->free_limit = (1 + nr_cpus_node(node)) *
					cachep->batchcount + cachep->num;
			spin_unlock_irq(&n->list_lock);
			kfree(shared);
			free_alien_cache(new_alien);
			continue;
		}
		n = kmalloc_node(sizeof(struct kmem_cache_node), gfp, node);
		if (!n) {
			free_alien_cache(new_alien);
			kfree(new_shared);
			goto fail;
		}

		kmem_cache_node_init(n);
		n->next_reap = jiffies + REAPTIMEOUT_LIST3 +
				((unsigned long)cachep) % REAPTIMEOUT_LIST3;
		n->shared = new_shared;
		n->alien = new_alien;
		n->free_limit = (1 + nr_cpus_node(node)) *
					cachep->batchcount + cachep->num;
		cachep->node[node] = n;
	}
=======
static int setup_kmem_cache_nodes(struct kmem_cache *cachep, gfp_t gfp)
{
	int ret;
	int node;
	struct kmem_cache_node *n;

	for_each_online_node(node) {
		ret = setup_kmem_cache_node(cachep, node, gfp, true);
		if (ret)
			goto fail;

	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;

fail:
	if (!cachep->list.next) {
		/* Cache is not active yet. Roll back what we did */
		node--;
		while (node >= 0) {
<<<<<<< HEAD
			if (cachep->node[node]) {
				n = cachep->node[node];

=======
			n = get_node(cachep, node);
			if (n) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				kfree(n->shared);
				free_alien_cache(n->alien);
				kfree(n);
				cachep->node[node] = NULL;
			}
			node--;
		}
	}
	return -ENOMEM;
}

<<<<<<< HEAD
struct ccupdate_struct {
	struct kmem_cache *cachep;
	struct array_cache *new[0];
};

static void do_ccupdate_local(void *info)
{
	struct ccupdate_struct *new = info;
	struct array_cache *old;

	check_irq_off();
	old = cpu_cache_get(new->cachep);

	new->cachep->array[smp_processor_id()] = new->new[smp_processor_id()];
	new->new[smp_processor_id()] = old;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Always called with the slab_mutex held */
static int __do_tune_cpucache(struct kmem_cache *cachep, int limit,
				int batchcount, int shared, gfp_t gfp)
{
<<<<<<< HEAD
	struct ccupdate_struct *new;
	int i;

	new = kzalloc(sizeof(*new) + nr_cpu_ids * sizeof(struct array_cache *),
		      gfp);
	if (!new)
		return -ENOMEM;

	for_each_online_cpu(i) {
		new->new[i] = alloc_arraycache(cpu_to_mem(i), limit,
						batchcount, gfp);
		if (!new->new[i]) {
			for (i--; i >= 0; i--)
				kfree(new->new[i]);
			kfree(new);
			return -ENOMEM;
		}
	}
	new->cachep = cachep;

	on_each_cpu(do_ccupdate_local, (void *)new, 1);
=======
	struct array_cache __percpu *cpu_cache, *prev;
	int cpu;

	cpu_cache = alloc_kmem_cache_cpus(cachep, limit, batchcount);
	if (!cpu_cache)
		return -ENOMEM;

	prev = cachep->cpu_cache;
	cachep->cpu_cache = cpu_cache;
	kick_all_cpus_sync();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	check_irq_on();
	cachep->batchcount = batchcount;
	cachep->limit = limit;
	cachep->shared = shared;

<<<<<<< HEAD
	for_each_online_cpu(i) {
		struct array_cache *ccold = new->new[i];
		if (!ccold)
			continue;
		spin_lock_irq(&cachep->node[cpu_to_mem(i)]->list_lock);
		free_block(cachep, ccold->entry, ccold->avail, cpu_to_mem(i));
		spin_unlock_irq(&cachep->node[cpu_to_mem(i)]->list_lock);
		kfree(ccold);
	}
	kfree(new);
	return alloc_kmemlist(cachep, gfp);
=======
	if (!prev)
		goto setup_node;

	for_each_online_cpu(cpu) {
		LIST_HEAD(list);
		int node;
		struct kmem_cache_node *n;
		struct array_cache *ac = per_cpu_ptr(prev, cpu);

		node = cpu_to_mem(cpu);
		n = get_node(cachep, node);
		spin_lock_irq(&n->list_lock);
		free_block(cachep, ac->entry, ac->avail, node, &list);
		spin_unlock_irq(&n->list_lock);
		slabs_destroy(cachep, &list);
	}
	free_percpu(prev);

setup_node:
	return setup_kmem_cache_nodes(cachep, gfp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int do_tune_cpucache(struct kmem_cache *cachep, int limit,
				int batchcount, int shared, gfp_t gfp)
{
	int ret;
<<<<<<< HEAD
	struct kmem_cache *c = NULL;
	int i = 0;
=======
	struct kmem_cache *c;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ret = __do_tune_cpucache(cachep, limit, batchcount, shared, gfp);

	if (slab_state < FULL)
		return ret;

	if ((ret < 0) || !is_root_cache(cachep))
		return ret;

<<<<<<< HEAD
	VM_BUG_ON(!mutex_is_locked(&slab_mutex));
	for_each_memcg_cache_index(i) {
		c = cache_from_memcg(cachep, i);
		if (c)
			/* return value determined by the parent cache only */
			__do_tune_cpucache(c, limit, batchcount, shared, gfp);
=======
	lockdep_assert_held(&slab_mutex);
	for_each_memcg_cache(c, cachep) {
		/* return value determined by the root cache only */
		__do_tune_cpucache(c, limit, batchcount, shared, gfp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return ret;
}

/* Called with slab_mutex held always */
static int enable_cpucache(struct kmem_cache *cachep, gfp_t gfp)
{
	int err;
	int limit = 0;
	int shared = 0;
	int batchcount = 0;

<<<<<<< HEAD
=======
	err = cache_random_seq_create(cachep, cachep->num, gfp);
	if (err)
		goto end;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!is_root_cache(cachep)) {
		struct kmem_cache *root = memcg_root_cache(cachep);
		limit = root->limit;
		shared = root->shared;
		batchcount = root->batchcount;
	}

	if (limit && shared && batchcount)
		goto skip_setup;
	/*
	 * The head array serves three purposes:
	 * - create a LIFO ordering, i.e. return objects that are cache-warm
	 * - reduce the number of spinlock operations.
	 * - reduce the number of linked list operations on the slab and
	 *   bufctl chains: array operations are cheaper.
	 * The numbers are guessed, we should auto-tune as described by
	 * Bonwick.
	 */
	if (cachep->size > 131072)
		limit = 1;
	else if (cachep->size > PAGE_SIZE)
		limit = 8;
	else if (cachep->size > 1024)
		limit = 24;
	else if (cachep->size > 256)
		limit = 54;
	else
		limit = 120;

	/*
	 * CPU bound tasks (e.g. network routing) can exhibit cpu bound
	 * allocation behaviour: Most allocs on one cpu, most free operations
	 * on another cpu. For these cases, an efficient object passing between
	 * cpus is necessary. This is provided by a shared array. The array
	 * replaces Bonwick's magazine layer.
	 * On uniprocessor, it's functionally equivalent (but less efficient)
	 * to a larger limit. Thus disabled by default.
	 */
	shared = 0;
	if (cachep->size <= PAGE_SIZE && num_possible_cpus() > 1)
		shared = 8;

#if DEBUG
	/*
	 * With debugging enabled, large batchcount lead to excessively long
	 * periods with disabled local interrupts. Limit the batchcount
	 */
	if (limit > 32)
		limit = 32;
#endif
	batchcount = (limit + 1) / 2;
skip_setup:
	err = do_tune_cpucache(cachep, limit, batchcount, shared, gfp);
<<<<<<< HEAD
	if (err)
		printk(KERN_ERR "enable_cpucache failed for %s, error %d.\n",
=======
end:
	if (err)
		pr_err("enable_cpucache failed for %s, error %d\n",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       cachep->name, -err);
	return err;
}

/*
 * Drain an array if it contains any elements taking the node lock only if
 * necessary. Note that the node listlock also protects the array_cache
 * if drain_array() is used on the shared array.
 */
static void drain_array(struct kmem_cache *cachep, struct kmem_cache_node *n,
<<<<<<< HEAD
			 struct array_cache *ac, int force, int node)
{
	int tofree;

	if (!ac || !ac->avail)
		return;
	if (ac->touched && !force) {
		ac->touched = 0;
	} else {
		spin_lock_irq(&n->list_lock);
		if (ac->avail) {
			tofree = force ? ac->avail : (ac->limit + 4) / 5;
			if (tofree > ac->avail)
				tofree = (ac->avail + 1) / 2;
			free_block(cachep, ac->entry, tofree, node);
			ac->avail -= tofree;
			memmove(ac->entry, &(ac->entry[tofree]),
				sizeof(void *) * ac->avail);
		}
		spin_unlock_irq(&n->list_lock);
	}
=======
			 struct array_cache *ac, int node)
{
	LIST_HEAD(list);

	/* ac from n->shared can be freed if we don't hold the slab_mutex. */
	check_mutex_acquired();

	if (!ac || !ac->avail)
		return;

	if (ac->touched) {
		ac->touched = 0;
		return;
	}

	spin_lock_irq(&n->list_lock);
	drain_array_locked(cachep, ac, node, false, &list);
	spin_unlock_irq(&n->list_lock);

	slabs_destroy(cachep, &list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * cache_reap - Reclaim memory from caches.
 * @w: work descriptor
 *
 * Called from workqueue/eventd every few seconds.
 * Purpose:
 * - clear the per-cpu caches for this CPU.
 * - return freeable pages to the main free memory pool.
 *
 * If we cannot acquire the cache chain mutex then just give up - we'll try
 * again on the next iteration.
 */
static void cache_reap(struct work_struct *w)
{
	struct kmem_cache *searchp;
	struct kmem_cache_node *n;
	int node = numa_mem_id();
	struct delayed_work *work = to_delayed_work(w);

	if (!mutex_trylock(&slab_mutex))
		/* Give up. Setup the next iteration. */
		goto out;

	list_for_each_entry(searchp, &slab_caches, list) {
		check_irq_on();

		/*
		 * We only take the node lock if absolutely necessary and we
		 * have established with reasonable certainty that
		 * we can do some work if the lock was obtained.
		 */
<<<<<<< HEAD
		n = searchp->node[node];

		reap_alien(searchp, n);

		drain_array(searchp, n, cpu_cache_get(searchp), 0, node);
=======
		n = get_node(searchp, node);

		reap_alien(searchp, n);

		drain_array(searchp, n, cpu_cache_get(searchp), node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/*
		 * These are racy checks but it does not matter
		 * if we skip one check or scan twice.
		 */
		if (time_after(n->next_reap, jiffies))
			goto next;

<<<<<<< HEAD
		n->next_reap = jiffies + REAPTIMEOUT_LIST3;

		drain_array(searchp, n, n->shared, 0, node);
=======
		n->next_reap = jiffies + REAPTIMEOUT_NODE;

		drain_array(searchp, n, n->shared, node);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (n->free_touched)
			n->free_touched = 0;
		else {
			int freed;

			freed = drain_freelist(searchp, n, (n->free_limit +
				5 * searchp->num - 1) / (5 * searchp->num));
			STATS_ADD_REAPED(searchp, freed);
		}
next:
		cond_resched();
	}
	check_irq_on();
	mutex_unlock(&slab_mutex);
	next_reap_node();
out:
	/* Set up the next iteration */
<<<<<<< HEAD
	schedule_delayed_work(work, round_jiffies_relative(REAPTIMEOUT_CPUC));
=======
	schedule_delayed_work_on(smp_processor_id(), work,
				round_jiffies_relative(REAPTIMEOUT_AC));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_SLABINFO
void get_slabinfo(struct kmem_cache *cachep, struct slabinfo *sinfo)
{
<<<<<<< HEAD
	struct slab *slabp;
=======
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long active_objs;
	unsigned long num_objs;
	unsigned long active_slabs = 0;
	unsigned long num_slabs, free_objects = 0, shared_avail = 0;
<<<<<<< HEAD
=======
	unsigned long num_slabs_partial = 0, num_slabs_free = 0;
	unsigned long num_slabs_full = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char *name;
	char *error = NULL;
	int node;
	struct kmem_cache_node *n;

	active_objs = 0;
	num_slabs = 0;
<<<<<<< HEAD
	for_each_online_node(node) {
		n = cachep->node[node];
		if (!n)
			continue;
=======
	for_each_kmem_cache_node(cachep, node, n) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		check_irq_on();
		spin_lock_irq(&n->list_lock);

<<<<<<< HEAD
		list_for_each_entry(slabp, &n->slabs_full, list) {
			if (slabp->inuse != cachep->num && !error)
				error = "slabs_full accounting error";
			active_objs += cachep->num;
			active_slabs++;
		}
		list_for_each_entry(slabp, &n->slabs_partial, list) {
			if (slabp->inuse == cachep->num && !error)
				error = "slabs_partial inuse accounting error";
			if (!slabp->inuse && !error)
				error = "slabs_partial/inuse accounting error";
			active_objs += slabp->inuse;
			active_slabs++;
		}
		list_for_each_entry(slabp, &n->slabs_free, list) {
			if (slabp->inuse && !error)
				error = "slabs_free/inuse accounting error";
			num_slabs++;
		}
=======
		num_slabs += n->num_slabs;

		list_for_each_entry(page, &n->slabs_partial, lru) {
			if (page->active == cachep->num && !error)
				error = "slabs_partial accounting error";
			if (!page->active && !error)
				error = "slabs_partial accounting error";
			active_objs += page->active;
			num_slabs_partial++;
		}

		list_for_each_entry(page, &n->slabs_free, lru) {
			if (page->active && !error)
				error = "slabs_free accounting error";
			num_slabs_free++;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		free_objects += n->free_objects;
		if (n->shared)
			shared_avail += n->shared->avail;

		spin_unlock_irq(&n->list_lock);
	}
<<<<<<< HEAD
	num_slabs += active_slabs;
	num_objs = num_slabs * cachep->num;
=======
	num_objs = num_slabs * cachep->num;
	active_slabs = num_slabs - num_slabs_free;
	num_slabs_full = num_slabs - (num_slabs_partial + num_slabs_free);
	active_objs += (num_slabs_full * cachep->num);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (num_objs - active_objs != free_objects && !error)
		error = "free_objects accounting error";

	name = cachep->name;
	if (error)
<<<<<<< HEAD
		printk(KERN_ERR "slab: cache %s error: %s\n", name, error);
=======
		pr_err("slab: cache %s error: %s\n", name, error);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	sinfo->active_objs = active_objs;
	sinfo->num_objs = num_objs;
	sinfo->active_slabs = active_slabs;
	sinfo->num_slabs = num_slabs;
	sinfo->shared_avail = shared_avail;
	sinfo->limit = cachep->limit;
	sinfo->batchcount = cachep->batchcount;
	sinfo->shared = cachep->shared;
	sinfo->objects_per_slab = cachep->num;
	sinfo->cache_order = cachep->gfporder;
}

void slabinfo_show_stats(struct seq_file *m, struct kmem_cache *cachep)
{
#if STATS
	{			/* node stats */
		unsigned long high = cachep->high_mark;
		unsigned long allocs = cachep->num_allocations;
		unsigned long grown = cachep->grown;
		unsigned long reaped = cachep->reaped;
		unsigned long errors = cachep->errors;
		unsigned long max_freeable = cachep->max_freeable;
		unsigned long node_allocs = cachep->node_allocs;
		unsigned long node_frees = cachep->node_frees;
		unsigned long overflows = cachep->node_overflow;

<<<<<<< HEAD
		seq_printf(m, " : globalstat %7lu %6lu %5lu %4lu "
			   "%4lu %4lu %4lu %4lu %4lu",
=======
		seq_printf(m, " : globalstat %7lu %6lu %5lu %4lu %4lu %4lu %4lu %4lu %4lu",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			   allocs, high, grown,
			   reaped, errors, max_freeable, node_allocs,
			   node_frees, overflows);
	}
	/* cpu stats */
	{
		unsigned long allochit = atomic_read(&cachep->allochit);
		unsigned long allocmiss = atomic_read(&cachep->allocmiss);
		unsigned long freehit = atomic_read(&cachep->freehit);
		unsigned long freemiss = atomic_read(&cachep->freemiss);

		seq_printf(m, " : cpustat %6lu %6lu %6lu %6lu",
			   allochit, allocmiss, freehit, freemiss);
	}
#endif
}

#define MAX_SLABINFO_WRITE 128
/**
 * slabinfo_write - Tuning for the slab allocator
 * @file: unused
 * @buffer: user buffer
 * @count: data length
 * @ppos: unused
 */
ssize_t slabinfo_write(struct file *file, const char __user *buffer,
		       size_t count, loff_t *ppos)
{
	char kbuf[MAX_SLABINFO_WRITE + 1], *tmp;
	int limit, batchcount, shared, res;
	struct kmem_cache *cachep;

	if (count > MAX_SLABINFO_WRITE)
		return -EINVAL;
	if (copy_from_user(&kbuf, buffer, count))
		return -EFAULT;
	kbuf[MAX_SLABINFO_WRITE] = '\0';

	tmp = strchr(kbuf, ' ');
	if (!tmp)
		return -EINVAL;
	*tmp = '\0';
	tmp++;
	if (sscanf(tmp, " %d %d %d", &limit, &batchcount, &shared) != 3)
		return -EINVAL;

	/* Find the cache in the chain of caches. */
	mutex_lock(&slab_mutex);
	res = -EINVAL;
	list_for_each_entry(cachep, &slab_caches, list) {
		if (!strcmp(cachep->name, kbuf)) {
			if (limit < 1 || batchcount < 1 ||
					batchcount > limit || shared < 0) {
				res = 0;
			} else {
				res = do_tune_cpucache(cachep, limit,
						       batchcount, shared,
						       GFP_KERNEL);
			}
			break;
		}
	}
	mutex_unlock(&slab_mutex);
	if (res >= 0)
		res = count;
	return res;
}

#ifdef CONFIG_DEBUG_SLAB_LEAK

<<<<<<< HEAD
static void *leaks_start(struct seq_file *m, loff_t *pos)
{
	mutex_lock(&slab_mutex);
	return seq_list_start(&slab_caches, *pos);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int add_caller(unsigned long *n, unsigned long v)
{
	unsigned long *p;
	int l;
	if (!v)
		return 1;
	l = n[1];
	p = n + 2;
	while (l) {
		int i = l/2;
		unsigned long *q = p + 2 * i;
		if (*q == v) {
			q[1]++;
			return 1;
		}
		if (*q > v) {
			l = i;
		} else {
			p = q + 2;
			l -= i + 1;
		}
	}
	if (++n[1] == n[0])
		return 0;
	memmove(p + 2, p, n[1] * 2 * sizeof(unsigned long) - ((void *)p - (void *)n));
	p[0] = v;
	p[1] = 1;
	return 1;
}

<<<<<<< HEAD
static void handle_slab(unsigned long *n, struct kmem_cache *c, struct slab *s)
{
	void *p;
	int i;
	if (n[0] == n[1])
		return;
	for (i = 0, p = s->s_mem; i < c->num; i++, p += c->size) {
		if (slab_bufctl(s)[i] != BUFCTL_ACTIVE)
			continue;
		if (!add_caller(n, (unsigned long)*dbg_userword(c, p)))
=======
static void handle_slab(unsigned long *n, struct kmem_cache *c,
						struct page *page)
{
	void *p;
	int i, j;
	unsigned long v;

	if (n[0] == n[1])
		return;
	for (i = 0, p = page->s_mem; i < c->num; i++, p += c->size) {
		bool active = true;

		for (j = page->active; j < c->num; j++) {
			if (get_free_obj(page, j) == i) {
				active = false;
				break;
			}
		}

		if (!active)
			continue;

		/*
		 * probe_kernel_read() is used for DEBUG_PAGEALLOC. page table
		 * mapping is established when actual object allocation and
		 * we could mistakenly access the unmapped object in the cpu
		 * cache.
		 */
		if (probe_kernel_read(&v, dbg_userword(c, p), sizeof(v)))
			continue;

		if (!add_caller(n, v))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return;
	}
}

static void show_symbol(struct seq_file *m, unsigned long address)
{
#ifdef CONFIG_KALLSYMS
	unsigned long offset, size;
	char modname[MODULE_NAME_LEN], name[KSYM_NAME_LEN];

	if (lookup_symbol_attrs(address, &size, &offset, modname, name) == 0) {
		seq_printf(m, "%s+%#lx/%#lx", name, offset, size);
		if (modname[0])
			seq_printf(m, " [%s]", modname);
		return;
	}
#endif
	seq_printf(m, "%p", (void *)address);
}

static int leaks_show(struct seq_file *m, void *p)
{
	struct kmem_cache *cachep = list_entry(p, struct kmem_cache, list);
<<<<<<< HEAD
	struct slab *slabp;
=======
	struct page *page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct kmem_cache_node *n;
	const char *name;
	unsigned long *x = m->private;
	int node;
	int i;

	if (!(cachep->flags & SLAB_STORE_USER))
		return 0;
	if (!(cachep->flags & SLAB_RED_ZONE))
		return 0;

<<<<<<< HEAD
	/* OK, we can do it */

	x[1] = 0;

	for_each_online_node(node) {
		n = cachep->node[node];
		if (!n)
			continue;

		check_irq_on();
		spin_lock_irq(&n->list_lock);

		list_for_each_entry(slabp, &n->slabs_full, list)
			handle_slab(x, cachep, slabp);
		list_for_each_entry(slabp, &n->slabs_partial, list)
			handle_slab(x, cachep, slabp);
		spin_unlock_irq(&n->list_lock);
	}
=======
	/*
	 * Set store_user_clean and start to grab stored user information
	 * for all objects on this cache. If some alloc/free requests comes
	 * during the processing, information would be wrong so restart
	 * whole processing.
	 */
	do {
		drain_cpu_caches(cachep);
		/*
		 * drain_cpu_caches() could make kmemleak_object and
		 * debug_objects_cache dirty, so reset afterwards.
		 */
		set_store_user_clean(cachep);

		x[1] = 0;

		for_each_kmem_cache_node(cachep, node, n) {

			check_irq_on();
			spin_lock_irq(&n->list_lock);

			list_for_each_entry(page, &n->slabs_full, lru)
				handle_slab(x, cachep, page);
			list_for_each_entry(page, &n->slabs_partial, lru)
				handle_slab(x, cachep, page);
			spin_unlock_irq(&n->list_lock);
		}
	} while (!is_store_user_clean(cachep));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	name = cachep->name;
	if (x[0] == x[1]) {
		/* Increase the buffer size */
		mutex_unlock(&slab_mutex);
		m->private = kzalloc(x[0] * 4 * sizeof(unsigned long), GFP_KERNEL);
		if (!m->private) {
			/* Too bad, we are really out */
			m->private = x;
			mutex_lock(&slab_mutex);
			return -ENOMEM;
		}
		*(unsigned long *)m->private = x[0] * 2;
		kfree(x);
		mutex_lock(&slab_mutex);
		/* Now make sure this entry will be retried */
		m->count = m->size;
		return 0;
	}
	for (i = 0; i < x[1]; i++) {
		seq_printf(m, "%s: %lu ", name, x[2*i+3]);
		show_symbol(m, x[2*i+2]);
		seq_putc(m, '\n');
	}

	return 0;
}

<<<<<<< HEAD
static void *s_next(struct seq_file *m, void *p, loff_t *pos)
{
	return seq_list_next(p, &slab_caches, pos);
}

static void s_stop(struct seq_file *m, void *p)
{
	mutex_unlock(&slab_mutex);
}

static const struct seq_operations slabstats_op = {
	.start = leaks_start,
	.next = s_next,
	.stop = s_stop,
=======
static const struct seq_operations slabstats_op = {
	.start = slab_start,
	.next = slab_next,
	.stop = slab_stop,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.show = leaks_show,
};

static int slabstats_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	unsigned long *n = kzalloc(PAGE_SIZE, GFP_KERNEL);
	int ret = -ENOMEM;
	if (n) {
		ret = seq_open(file, &slabstats_op);
		if (!ret) {
			struct seq_file *m = file->private_data;
			*n = PAGE_SIZE / (2 * sizeof(unsigned long));
			m->private = n;
			n = NULL;
		}
		kfree(n);
	}
	return ret;
=======
	unsigned long *n;

	n = __seq_open_private(file, &slabstats_op, PAGE_SIZE);
	if (!n)
		return -ENOMEM;

	*n = PAGE_SIZE / (2 * sizeof(unsigned long));

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static const struct file_operations proc_slabstats_operations = {
	.open		= slabstats_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_private,
};
#endif

static int __init slab_proc_init(void)
{
#ifdef CONFIG_DEBUG_SLAB_LEAK
	proc_create("slab_allocators", 0, NULL, &proc_slabstats_operations);
#endif
	return 0;
}
module_init(slab_proc_init);
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_HARDENED_USERCOPY
/*
 * Rejects objects that are incorrectly sized.
 *
 * Returns NULL if check passes, otherwise const char * to name of cache
 * to indicate an error.
 */
const char *__check_heap_object(const void *ptr, unsigned long n,
				struct page *page)
{
	struct kmem_cache *cachep;
	unsigned int objnr;
	unsigned long offset;

	/* Find and validate object. */
	cachep = page->slab_cache;
	objnr = obj_to_index(cachep, page, (void *)ptr);
	BUG_ON(objnr >= cachep->num);

	/* Find offset within object. */
	offset = ptr - index_to_obj(cachep, page, objnr) - obj_offset(cachep);

	/* Allow address range falling entirely within object size. */
	if (offset <= cachep->object_size && n <= cachep->object_size - offset)
		return NULL;

	return cachep->name;
}
#endif /* CONFIG_HARDENED_USERCOPY */

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * ksize - get the actual amount of memory allocated for a given object
 * @objp: Pointer to the object
 *
 * kmalloc may internally round up allocations and return more memory
 * than requested. ksize() can be used to determine the actual amount of
 * memory allocated. The caller may use this additional memory, even though
 * a smaller amount of memory was initially specified with the kmalloc call.
 * The caller must guarantee that objp points to a valid object previously
 * allocated with either kmalloc() or kmem_cache_alloc(). The object
 * must not be freed during the duration of the call.
 */
size_t ksize(const void *objp)
{
<<<<<<< HEAD
=======
	size_t size;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	BUG_ON(!objp);
	if (unlikely(objp == ZERO_SIZE_PTR))
		return 0;

<<<<<<< HEAD
	return virt_to_cache(objp)->object_size;
=======
	size = virt_to_cache(objp)->object_size;
	/* We assume that ksize callers could use the whole allocated area,
	 * so we need to unpoison this area.
	 */
	kasan_unpoison_shadow(objp, size);

	return size;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(ksize);
