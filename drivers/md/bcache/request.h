#ifndef _BCACHE_REQUEST_H_
#define _BCACHE_REQUEST_H_

<<<<<<< HEAD
#include <linux/cgroup.h>

struct search {
	/* Stack frame for bio_complete */
	struct closure		cl;

	struct bcache_device	*d;
	struct task_struct	*task;

	struct bbio		bio;
	struct bio		*orig_bio;
	struct bio		*cache_miss;
	unsigned		cache_bio_sectors;

	unsigned		recoverable:1;
	unsigned		unaligned_bvec:1;

	unsigned		write:1;
	unsigned		writeback:1;

	/* IO error returned to s->bio */
	short			error;
	unsigned long		start_time;

	/* Anything past op->keys won't get zeroed in do_bio_hook */
	struct btree_op		op;
};

void bch_cache_read_endio(struct bio *, int);
int bch_get_congested(struct cache_set *);
void bch_insert_data(struct closure *cl);
void bch_btree_insert_async(struct closure *);
void bch_cache_read_endio(struct bio *, int);

void bch_open_buckets_free(struct cache_set *);
int bch_open_buckets_alloc(struct cache_set *);
=======
struct data_insert_op {
	struct closure		cl;
	struct cache_set	*c;
	struct bio		*bio;
	struct workqueue_struct *wq;

	unsigned		inode;
	uint16_t		write_point;
	uint16_t		write_prio;
	short			error;

	union {
		uint16_t	flags;

	struct {
		unsigned	bypass:1;
		unsigned	writeback:1;
		unsigned	flush_journal:1;
		unsigned	csum:1;

		unsigned	replace:1;
		unsigned	replace_collision:1;

		unsigned	insert_data_done:1;
	};
	};

	struct keylist		insert_keys;
	BKEY_PADDED(replace_key);
};

unsigned bch_get_congested(struct cache_set *);
void bch_data_insert(struct closure *cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void bch_cached_dev_request_init(struct cached_dev *dc);
void bch_flash_dev_request_init(struct bcache_device *d);

extern struct kmem_cache *bch_search_cache, *bch_passthrough_cache;

<<<<<<< HEAD
struct bch_cgroup {
#ifdef CONFIG_CGROUP_BCACHE
	struct cgroup_subsys_state	css;
#endif
	/*
	 * We subtract one from the index into bch_cache_modes[], so that
	 * default == -1; this makes it so the rest match up with d->cache_mode,
	 * and we use d->cache_mode if cgrp->cache_mode < 0
	 */
	short				cache_mode;
	bool				verify;
	struct cache_stat_collector	stats;
};

struct bch_cgroup *bch_bio_to_cgroup(struct bio *bio);

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _BCACHE_REQUEST_H_ */
