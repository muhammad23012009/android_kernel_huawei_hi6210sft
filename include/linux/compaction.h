#ifndef _LINUX_COMPACTION_H
#define _LINUX_COMPACTION_H

<<<<<<< HEAD
/* Return values for compact_zone() and try_to_compact_pages() */
/* compaction didn't start as it was not possible or direct reclaim was more suitable */
#define COMPACT_SKIPPED		0
/* compaction should continue to another pageblock */
#define COMPACT_CONTINUE	1
/* direct compaction partially compacted a zone and there are suitable pages */
#define COMPACT_PARTIAL		2
/* The full zone was compacted */
#define COMPACT_COMPLETE	3
=======
/*
 * Determines how hard direct compaction should try to succeed.
 * Lower value means higher priority, analogically to reclaim priority.
 */
enum compact_priority {
	COMPACT_PRIO_SYNC_FULL,
	MIN_COMPACT_PRIORITY = COMPACT_PRIO_SYNC_FULL,
	COMPACT_PRIO_SYNC_LIGHT,
	MIN_COMPACT_COSTLY_PRIORITY = COMPACT_PRIO_SYNC_LIGHT,
	DEF_COMPACT_PRIORITY = COMPACT_PRIO_SYNC_LIGHT,
	COMPACT_PRIO_ASYNC,
	INIT_COMPACT_PRIORITY = COMPACT_PRIO_ASYNC
};

/* Return values for compact_zone() and try_to_compact_pages() */
/* When adding new states, please adjust include/trace/events/compaction.h */
enum compact_result {
	/* For more detailed tracepoint output - internal to compaction */
	COMPACT_NOT_SUITABLE_ZONE,
	/*
	 * compaction didn't start as it was not possible or direct reclaim
	 * was more suitable
	 */
	COMPACT_SKIPPED,
	/* compaction didn't start as it was deferred due to past failures */
	COMPACT_DEFERRED,

	/* compaction not active last round */
	COMPACT_INACTIVE = COMPACT_DEFERRED,

	/* For more detailed tracepoint output - internal to compaction */
	COMPACT_NO_SUITABLE_PAGE,
	/* compaction should continue to another pageblock */
	COMPACT_CONTINUE,

	/*
	 * The full zone was compacted scanned but wasn't successfull to compact
	 * suitable pages.
	 */
	COMPACT_COMPLETE,
	/*
	 * direct compaction has scanned part of the zone but wasn't successfull
	 * to compact suitable pages.
	 */
	COMPACT_PARTIAL_SKIPPED,

	/* compaction terminated prematurely due to lock contentions */
	COMPACT_CONTENDED,

	/*
	 * direct compaction terminated after concluding that the allocation
	 * should now succeed
	 */
	COMPACT_SUCCESS,
};

struct alloc_context; /* in mm/internal.h */

/*
 * Number of free order-0 pages that should be available above given watermark
 * to make sure compaction has reasonable chance of not running out of free
 * pages that it needs to isolate as migration target during its work.
 */
static inline unsigned long compact_gap(unsigned int order)
{
	/*
	 * Although all the isolations for migration are temporary, compaction
	 * free scanner may have up to 1 << order pages on its list and then
	 * try to split an (order - 1) free page. At that point, a gap of
	 * 1 << order might not be enough, so it's safer to require twice that
	 * amount. Note that the number of pages on the list is also
	 * effectively limited by COMPACT_CLUSTER_MAX, as that's the maximum
	 * that the migrate scanner can have isolated on migrate list, and free
	 * scanner is only invoked when the number of isolated free pages is
	 * lower than that. But it's not worth to complicate the formula here
	 * as a bigger gap for higher orders than strictly necessary can also
	 * improve chances of compaction success.
	 */
	return 2UL << order;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_COMPACTION
extern int sysctl_compact_memory;
extern int sysctl_compaction_handler(struct ctl_table *table, int write,
			void __user *buffer, size_t *length, loff_t *ppos);
extern int sysctl_extfrag_threshold;
extern int sysctl_extfrag_handler(struct ctl_table *table, int write,
			void __user *buffer, size_t *length, loff_t *ppos);
<<<<<<< HEAD

#ifdef CONFIG_ARM64
extern unsigned long sysctl_compact_memory_s;
extern unsigned long sysctl_compact_memory_s_min;
extern unsigned long sysctl_compact_memory_ratio;
extern unsigned long sysctl_compact_memory_ratio_min;
extern unsigned long sysctl_compact_memory_ratio_max;
extern void compact_memory_init(void);
#endif

extern int fragmentation_index(struct zone *zone, unsigned int order);
extern unsigned long try_to_compact_pages(struct zonelist *zonelist,
			int order, gfp_t gfp_mask, nodemask_t *mask,
			bool sync, bool *contended);
extern void compact_pgdat(pg_data_t *pgdat, int order);
extern void reset_isolation_suitable(pg_data_t *pgdat);
extern unsigned long compaction_suitable(struct zone *zone, int order);

/* Do not skip compaction more than 64 times */
#define COMPACT_MAX_DEFER_SHIFT 6

/*
 * Compaction is deferred when compaction fails to result in a page
 * allocation success. 1 << compact_defer_limit compactions are skipped up
 * to a limit of 1 << COMPACT_MAX_DEFER_SHIFT
 */
static inline void defer_compaction(struct zone *zone, int order)
{
	zone->compact_considered = 0;
	zone->compact_defer_shift++;

	if (order < zone->compact_order_failed)
		zone->compact_order_failed = order;

	if (zone->compact_defer_shift > COMPACT_MAX_DEFER_SHIFT)
		zone->compact_defer_shift = COMPACT_MAX_DEFER_SHIFT;
}

/* Returns true if compaction should be skipped this time */
static inline bool compaction_deferred(struct zone *zone, int order)
{
	unsigned long defer_limit = 1UL << zone->compact_defer_shift;

	if (order < zone->compact_order_failed)
		return false;

	/* Avoid possible overflow */
	if (++zone->compact_considered > defer_limit)
		zone->compact_considered = defer_limit;

	return zone->compact_considered < defer_limit;
}

/* Returns true if restarting compaction after many failures */
static inline bool compaction_restarting(struct zone *zone, int order)
{
	if (order < zone->compact_order_failed)
		return false;

	return zone->compact_defer_shift == COMPACT_MAX_DEFER_SHIFT &&
		zone->compact_considered >= 1UL << zone->compact_defer_shift;
}

#else
static inline unsigned long try_to_compact_pages(struct zonelist *zonelist,
			int order, gfp_t gfp_mask, nodemask_t *nodemask,
			bool sync, bool *contended)
{
	return COMPACT_CONTINUE;
}

static inline void compact_pgdat(pg_data_t *pgdat, int order)
{
}

static inline void reset_isolation_suitable(pg_data_t *pgdat)
{
}

static inline unsigned long compaction_suitable(struct zone *zone, int order)
{
	return COMPACT_SKIPPED;
}

static inline void defer_compaction(struct zone *zone, int order)
{
}

static inline bool compaction_deferred(struct zone *zone, int order)
{
	return true;
=======
extern int sysctl_compact_unevictable_allowed;

extern int fragmentation_index(struct zone *zone, unsigned int order);
extern enum compact_result try_to_compact_pages(gfp_t gfp_mask,
		unsigned int order, unsigned int alloc_flags,
		const struct alloc_context *ac, enum compact_priority prio);
extern void reset_isolation_suitable(pg_data_t *pgdat);
extern enum compact_result compaction_suitable(struct zone *zone, int order,
		unsigned int alloc_flags, int classzone_idx);

extern void defer_compaction(struct zone *zone, int order);
extern bool compaction_deferred(struct zone *zone, int order);
extern void compaction_defer_reset(struct zone *zone, int order,
				bool alloc_success);
extern bool compaction_restarting(struct zone *zone, int order);

/* Compaction has made some progress and retrying makes sense */
static inline bool compaction_made_progress(enum compact_result result)
{
	/*
	 * Even though this might sound confusing this in fact tells us
	 * that the compaction successfully isolated and migrated some
	 * pageblocks.
	 */
	if (result == COMPACT_SUCCESS)
		return true;

	return false;
}

/* Compaction has failed and it doesn't make much sense to keep retrying. */
static inline bool compaction_failed(enum compact_result result)
{
	/* All zones were scanned completely and still not result. */
	if (result == COMPACT_COMPLETE)
		return true;

	return false;
}

/*
 * Compaction  has backed off for some reason. It might be throttling or
 * lock contention. Retrying is still worthwhile.
 */
static inline bool compaction_withdrawn(enum compact_result result)
{
	/*
	 * Compaction backed off due to watermark checks for order-0
	 * so the regular reclaim has to try harder and reclaim something.
	 */
	if (result == COMPACT_SKIPPED)
		return true;

	/*
	 * If compaction is deferred for high-order allocations, it is
	 * because sync compaction recently failed. If this is the case
	 * and the caller requested a THP allocation, we do not want
	 * to heavily disrupt the system, so we fail the allocation
	 * instead of entering direct reclaim.
	 */
	if (result == COMPACT_DEFERRED)
		return true;

	/*
	 * If compaction in async mode encounters contention or blocks higher
	 * priority task we back off early rather than cause stalls.
	 */
	if (result == COMPACT_CONTENDED)
		return true;

	/*
	 * Page scanners have met but we haven't scanned full zones so this
	 * is a back off in fact.
	 */
	if (result == COMPACT_PARTIAL_SKIPPED)
		return true;

	return false;
}


bool compaction_zonelist_suitable(struct alloc_context *ac, int order,
					int alloc_flags);

extern int kcompactd_run(int nid);
extern void kcompactd_stop(int nid);
extern void wakeup_kcompactd(pg_data_t *pgdat, int order, int classzone_idx);

#else
static inline void reset_isolation_suitable(pg_data_t *pgdat)
{
}

static inline enum compact_result compaction_suitable(struct zone *zone, int order,
					int alloc_flags, int classzone_idx)
{
	return COMPACT_SKIPPED;
}

static inline void defer_compaction(struct zone *zone, int order)
{
}

static inline bool compaction_deferred(struct zone *zone, int order)
{
	return true;
}

static inline bool compaction_made_progress(enum compact_result result)
{
	return false;
}

static inline bool compaction_failed(enum compact_result result)
{
	return false;
}

static inline bool compaction_withdrawn(enum compact_result result)
{
	return true;
}

static inline int kcompactd_run(int nid)
{
	return 0;
}
static inline void kcompactd_stop(int nid)
{
}

static inline void wakeup_kcompactd(pg_data_t *pgdat, int order, int classzone_idx)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* CONFIG_COMPACTION */

#if defined(CONFIG_COMPACTION) && defined(CONFIG_SYSFS) && defined(CONFIG_NUMA)
<<<<<<< HEAD
=======
struct node;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int compaction_register_node(struct node *node);
extern void compaction_unregister_node(struct node *node);

#else

static inline int compaction_register_node(struct node *node)
{
	return 0;
}

static inline void compaction_unregister_node(struct node *node)
{
}
#endif /* CONFIG_COMPACTION && CONFIG_SYSFS && CONFIG_NUMA */

#endif /* _LINUX_COMPACTION_H */
