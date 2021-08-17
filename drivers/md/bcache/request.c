/*
 * Main bcache entry point - handle a read or a write request and decide what to
 * do with it; the make_request functions are called by the block layer.
 *
 * Copyright 2010, 2011 Kent Overstreet <kent.overstreet@gmail.com>
 * Copyright 2012 Google, Inc.
 */

#include "bcache.h"
#include "btree.h"
#include "debug.h"
#include "request.h"
<<<<<<< HEAD

#include <linux/cgroup.h>
#include <linux/module.h>
#include <linux/hash.h>
#include <linux/random.h>
#include "blk-cgroup.h"
=======
#include "writeback.h"

#include <linux/module.h>
#include <linux/hash.h>
#include <linux/random.h>
#include <linux/backing-dev.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <trace/events/bcache.h>

#define CUTOFF_CACHE_ADD	95
#define CUTOFF_CACHE_READA	90
<<<<<<< HEAD
#define CUTOFF_WRITEBACK	50
#define CUTOFF_WRITEBACK_SYNC	75

struct kmem_cache *bch_search_cache;

static void check_should_skip(struct cached_dev *, struct search *);

/* Cgroup interface */

#ifdef CONFIG_CGROUP_BCACHE
static struct bch_cgroup bcache_default_cgroup = { .cache_mode = -1 };

static struct bch_cgroup *cgroup_to_bcache(struct cgroup *cgroup)
{
	struct cgroup_subsys_state *css;
	return cgroup &&
		(css = cgroup_subsys_state(cgroup, bcache_subsys_id))
		? container_of(css, struct bch_cgroup, css)
		: &bcache_default_cgroup;
}

struct bch_cgroup *bch_bio_to_cgroup(struct bio *bio)
{
	struct cgroup_subsys_state *css = bio->bi_css
		? cgroup_subsys_state(bio->bi_css->cgroup, bcache_subsys_id)
		: task_subsys_state(current, bcache_subsys_id);

	return css
		? container_of(css, struct bch_cgroup, css)
		: &bcache_default_cgroup;
}

static ssize_t cache_mode_read(struct cgroup *cgrp, struct cftype *cft,
			struct file *file,
			char __user *buf, size_t nbytes, loff_t *ppos)
{
	char tmp[1024];
	int len = bch_snprint_string_list(tmp, PAGE_SIZE, bch_cache_modes,
					  cgroup_to_bcache(cgrp)->cache_mode + 1);

	if (len < 0)
		return len;

	return simple_read_from_buffer(buf, nbytes, ppos, tmp, len);
}

static int cache_mode_write(struct cgroup *cgrp, struct cftype *cft,
			    const char *buf)
{
	int v = bch_read_string_list(buf, bch_cache_modes);
	if (v < 0)
		return v;

	cgroup_to_bcache(cgrp)->cache_mode = v - 1;
	return 0;
}

static u64 bch_verify_read(struct cgroup *cgrp, struct cftype *cft)
{
	return cgroup_to_bcache(cgrp)->verify;
}

static int bch_verify_write(struct cgroup *cgrp, struct cftype *cft, u64 val)
{
	cgroup_to_bcache(cgrp)->verify = val;
	return 0;
}

static u64 bch_cache_hits_read(struct cgroup *cgrp, struct cftype *cft)
{
	struct bch_cgroup *bcachecg = cgroup_to_bcache(cgrp);
	return atomic_read(&bcachecg->stats.cache_hits);
}

static u64 bch_cache_misses_read(struct cgroup *cgrp, struct cftype *cft)
{
	struct bch_cgroup *bcachecg = cgroup_to_bcache(cgrp);
	return atomic_read(&bcachecg->stats.cache_misses);
}

static u64 bch_cache_bypass_hits_read(struct cgroup *cgrp,
					 struct cftype *cft)
{
	struct bch_cgroup *bcachecg = cgroup_to_bcache(cgrp);
	return atomic_read(&bcachecg->stats.cache_bypass_hits);
}

static u64 bch_cache_bypass_misses_read(struct cgroup *cgrp,
					   struct cftype *cft)
{
	struct bch_cgroup *bcachecg = cgroup_to_bcache(cgrp);
	return atomic_read(&bcachecg->stats.cache_bypass_misses);
}

static struct cftype bch_files[] = {
	{
		.name		= "cache_mode",
		.read		= cache_mode_read,
		.write_string	= cache_mode_write,
	},
	{
		.name		= "verify",
		.read_u64	= bch_verify_read,
		.write_u64	= bch_verify_write,
	},
	{
		.name		= "cache_hits",
		.read_u64	= bch_cache_hits_read,
	},
	{
		.name		= "cache_misses",
		.read_u64	= bch_cache_misses_read,
	},
	{
		.name		= "cache_bypass_hits",
		.read_u64	= bch_cache_bypass_hits_read,
	},
	{
		.name		= "cache_bypass_misses",
		.read_u64	= bch_cache_bypass_misses_read,
	},
	{ }	/* terminate */
};

static void init_bch_cgroup(struct bch_cgroup *cg)
{
	cg->cache_mode = -1;
}

static struct cgroup_subsys_state *bcachecg_create(struct cgroup *cgroup)
{
	struct bch_cgroup *cg;

	cg = kzalloc(sizeof(*cg), GFP_KERNEL);
	if (!cg)
		return ERR_PTR(-ENOMEM);
	init_bch_cgroup(cg);
	return &cg->css;
}

static void bcachecg_destroy(struct cgroup *cgroup)
{
	struct bch_cgroup *cg = cgroup_to_bcache(cgroup);
	free_css_id(&bcache_subsys, &cg->css);
	kfree(cg);
}

struct cgroup_subsys bcache_subsys = {
	.create		= bcachecg_create,
	.destroy	= bcachecg_destroy,
	.subsys_id	= bcache_subsys_id,
	.name		= "bcache",
	.module		= THIS_MODULE,
};
EXPORT_SYMBOL_GPL(bcache_subsys);
#endif

static unsigned cache_mode(struct cached_dev *dc, struct bio *bio)
{
#ifdef CONFIG_CGROUP_BCACHE
	int r = bch_bio_to_cgroup(bio)->cache_mode;
	if (r >= 0)
		return r;
#endif
=======

struct kmem_cache *bch_search_cache;

static void bch_data_insert_start(struct closure *);

static unsigned cache_mode(struct cached_dev *dc, struct bio *bio)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return BDEV_CACHE_MODE(&dc->sb);
}

static bool verify(struct cached_dev *dc, struct bio *bio)
{
<<<<<<< HEAD
#ifdef CONFIG_CGROUP_BCACHE
	if (bch_bio_to_cgroup(bio)->verify)
		return true;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return dc->verify;
}

static void bio_csum(struct bio *bio, struct bkey *k)
{
<<<<<<< HEAD
	struct bio_vec *bv;
	uint64_t csum = 0;
	int i;

	bio_for_each_segment(bv, bio, i) {
		void *d = kmap(bv->bv_page) + bv->bv_offset;
		csum = bch_crc64_update(csum, d, bv->bv_len);
		kunmap(bv->bv_page);
=======
	struct bio_vec bv;
	struct bvec_iter iter;
	uint64_t csum = 0;

	bio_for_each_segment(bv, bio, iter) {
		void *d = kmap(bv.bv_page) + bv.bv_offset;
		csum = bch_crc64_update(csum, d, bv.bv_len);
		kunmap(bv.bv_page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	k->ptr[KEY_PTRS(k)] = csum & (~0ULL >> 1);
}

/* Insert data into cache */

<<<<<<< HEAD
static void bio_invalidate(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);
	struct bio *bio = op->cache_bio;

	pr_debug("invalidating %i sectors from %llu",
		 bio_sectors(bio), (uint64_t) bio->bi_sector);

	while (bio_sectors(bio)) {
		unsigned len = min(bio_sectors(bio), 1U << 14);

		if (bch_keylist_realloc(&op->keys, 0, op->c))
			goto out;

		bio->bi_sector	+= len;
		bio->bi_size	-= len << 9;

		bch_keylist_add(&op->keys,
				&KEY(op->inode, bio->bi_sector, len));
	}

	op->insert_data_done = true;
	bio_put(bio);
out:
	continue_at(cl, bch_journal, bcache_wq);
}

struct open_bucket {
	struct list_head	list;
	struct task_struct	*last;
	unsigned		sectors_free;
	BKEY_PADDED(key);
};

void bch_open_buckets_free(struct cache_set *c)
{
	struct open_bucket *b;

	while (!list_empty(&c->data_buckets)) {
		b = list_first_entry(&c->data_buckets,
				     struct open_bucket, list);
		list_del(&b->list);
		kfree(b);
	}
}

int bch_open_buckets_alloc(struct cache_set *c)
{
	int i;

	spin_lock_init(&c->data_bucket_lock);

	for (i = 0; i < 6; i++) {
		struct open_bucket *b = kzalloc(sizeof(*b), GFP_KERNEL);
		if (!b)
			return -ENOMEM;

		list_add(&b->list, &c->data_buckets);
	}

	return 0;
}

/*
 * We keep multiple buckets open for writes, and try to segregate different
 * write streams for better cache utilization: first we look for a bucket where
 * the last write to it was sequential with the current write, and failing that
 * we look for a bucket that was last used by the same task.
 *
 * The ideas is if you've got multiple tasks pulling data into the cache at the
 * same time, you'll get better cache utilization if you try to segregate their
 * data and preserve locality.
 *
 * For example, say you've starting Firefox at the same time you're copying a
 * bunch of files. Firefox will likely end up being fairly hot and stay in the
 * cache awhile, but the data you copied might not be; if you wrote all that
 * data to the same buckets it'd get invalidated at the same time.
 *
 * Both of those tasks will be doing fairly random IO so we can't rely on
 * detecting sequential IO to segregate their data, but going off of the task
 * should be a sane heuristic.
 */
static struct open_bucket *pick_data_bucket(struct cache_set *c,
					    const struct bkey *search,
					    struct task_struct *task,
					    struct bkey *alloc)
{
	struct open_bucket *ret, *ret_task = NULL;

	list_for_each_entry_reverse(ret, &c->data_buckets, list)
		if (!bkey_cmp(&ret->key, search))
			goto found;
		else if (ret->last == task)
			ret_task = ret;

	ret = ret_task ?: list_first_entry(&c->data_buckets,
					   struct open_bucket, list);
found:
	if (!ret->sectors_free && KEY_PTRS(alloc)) {
		ret->sectors_free = c->sb.bucket_size;
		bkey_copy(&ret->key, alloc);
		bkey_init(alloc);
	}

	if (!ret->sectors_free)
		ret = NULL;

	return ret;
}

/*
 * Allocates some space in the cache to write to, and k to point to the newly
 * allocated space, and updates KEY_SIZE(k) and KEY_OFFSET(k) (to point to the
 * end of the newly allocated space).
 *
 * May allocate fewer sectors than @sectors, KEY_SIZE(k) indicates how many
 * sectors were actually allocated.
 *
 * If s->writeback is true, will not fail.
 */
static bool bch_alloc_sectors(struct bkey *k, unsigned sectors,
			      struct search *s)
{
	struct cache_set *c = s->op.c;
	struct open_bucket *b;
	BKEY_PADDED(key) alloc;
	struct closure cl, *w = NULL;
	unsigned i;

	if (s->writeback) {
		closure_init_stack(&cl);
		w = &cl;
	}

	/*
	 * We might have to allocate a new bucket, which we can't do with a
	 * spinlock held. So if we have to allocate, we drop the lock, allocate
	 * and then retry. KEY_PTRS() indicates whether alloc points to
	 * allocated bucket(s).
	 */

	bkey_init(&alloc.key);
	spin_lock(&c->data_bucket_lock);

	while (!(b = pick_data_bucket(c, k, s->task, &alloc.key))) {
		unsigned watermark = s->op.write_prio
			? WATERMARK_MOVINGGC
			: WATERMARK_NONE;

		spin_unlock(&c->data_bucket_lock);

		if (bch_bucket_alloc_set(c, watermark, &alloc.key, 1, w))
			return false;

		spin_lock(&c->data_bucket_lock);
	}

	/*
	 * If we had to allocate, we might race and not need to allocate the
	 * second time we call find_data_bucket(). If we allocated a bucket but
	 * didn't use it, drop the refcount bch_bucket_alloc_set() took:
	 */
	if (KEY_PTRS(&alloc.key))
		__bkey_put(c, &alloc.key);

	for (i = 0; i < KEY_PTRS(&b->key); i++)
		EBUG_ON(ptr_stale(c, &b->key, i));

	/* Set up the pointer to the space we're allocating: */

	for (i = 0; i < KEY_PTRS(&b->key); i++)
		k->ptr[i] = b->key.ptr[i];

	sectors = min(sectors, b->sectors_free);

	SET_KEY_OFFSET(k, KEY_OFFSET(k) + sectors);
	SET_KEY_SIZE(k, sectors);
	SET_KEY_PTRS(k, KEY_PTRS(&b->key));

	/*
	 * Move b to the end of the lru, and keep track of what this bucket was
	 * last used for:
	 */
	list_move_tail(&b->list, &c->data_buckets);
	bkey_copy_key(&b->key, k);
	b->last = s->task;

	b->sectors_free	-= sectors;

	for (i = 0; i < KEY_PTRS(&b->key); i++) {
		SET_PTR_OFFSET(&b->key, i, PTR_OFFSET(&b->key, i) + sectors);

		atomic_long_add(sectors,
				&PTR_CACHE(c, &b->key, i)->sectors_written);
	}

	if (b->sectors_free < c->sb.block_size)
		b->sectors_free = 0;

	/*
	 * k takes refcounts on the buckets it points to until it's inserted
	 * into the btree, but if we're done with this bucket we just transfer
	 * get_data_bucket()'s refcount.
	 */
	if (b->sectors_free)
		for (i = 0; i < KEY_PTRS(&b->key); i++)
			atomic_inc(&PTR_BUCKET(c, &b->key, i)->pin);

	spin_unlock(&c->data_bucket_lock);
	return true;
}

static void bch_insert_data_error(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);
=======
static void bch_data_insert_keys(struct closure *cl)
{
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);
	atomic_t *journal_ref = NULL;
	struct bkey *replace_key = op->replace ? &op->replace_key : NULL;
	int ret;

	/*
	 * If we're looping, might already be waiting on
	 * another journal write - can't wait on more than one journal write at
	 * a time
	 *
	 * XXX: this looks wrong
	 */
#if 0
	while (atomic_read(&s->cl.remaining) & CLOSURE_WAITING)
		closure_sync(&s->cl);
#endif

	if (!op->replace)
		journal_ref = bch_journal(op->c, &op->insert_keys,
					  op->flush_journal ? cl : NULL);

	ret = bch_btree_insert(op->c, &op->insert_keys,
			       journal_ref, replace_key);
	if (ret == -ESRCH) {
		op->replace_collision = true;
	} else if (ret) {
		op->error		= -ENOMEM;
		op->insert_data_done	= true;
	}

	if (journal_ref)
		atomic_dec_bug(journal_ref);

	if (!op->insert_data_done) {
		continue_at(cl, bch_data_insert_start, op->wq);
		return;
	}

	bch_keylist_free(&op->insert_keys);
	closure_return(cl);
}

static int bch_keylist_realloc(struct keylist *l, unsigned u64s,
			       struct cache_set *c)
{
	size_t oldsize = bch_keylist_nkeys(l);
	size_t newsize = oldsize + u64s;

	/*
	 * The journalling code doesn't handle the case where the keys to insert
	 * is bigger than an empty write: If we just return -ENOMEM here,
	 * bio_insert() and bio_invalidate() will insert the keys created so far
	 * and finish the rest when the keylist is empty.
	 */
	if (newsize * sizeof(uint64_t) > block_bytes(c) - sizeof(struct jset))
		return -ENOMEM;

	return __bch_keylist_realloc(l, u64s);
}

static void bch_data_invalidate(struct closure *cl)
{
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);
	struct bio *bio = op->bio;

	pr_debug("invalidating %i sectors from %llu",
		 bio_sectors(bio), (uint64_t) bio->bi_iter.bi_sector);

	while (bio_sectors(bio)) {
		unsigned sectors = min(bio_sectors(bio),
				       1U << (KEY_SIZE_BITS - 1));

		if (bch_keylist_realloc(&op->insert_keys, 2, op->c))
			goto out;

		bio->bi_iter.bi_sector	+= sectors;
		bio->bi_iter.bi_size	-= sectors << 9;

		bch_keylist_add(&op->insert_keys,
				&KEY(op->inode, bio->bi_iter.bi_sector, sectors));
	}

	op->insert_data_done = true;
	bio_put(bio);
out:
	continue_at(cl, bch_data_insert_keys, op->wq);
}

static void bch_data_insert_error(struct closure *cl)
{
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Our data write just errored, which means we've got a bunch of keys to
	 * insert that point to data that wasn't succesfully written.
	 *
	 * We don't have to insert those keys but we still have to invalidate
	 * that region of the cache - so, if we just strip off all the pointers
	 * from the keys we'll accomplish just that.
	 */

<<<<<<< HEAD
	struct bkey *src = op->keys.bottom, *dst = op->keys.bottom;

	while (src != op->keys.top) {
		struct bkey *n = bkey_next(src);

		SET_KEY_PTRS(src, 0);
		bkey_copy(dst, src);
=======
	struct bkey *src = op->insert_keys.keys, *dst = op->insert_keys.keys;

	while (src != op->insert_keys.top) {
		struct bkey *n = bkey_next(src);

		SET_KEY_PTRS(src, 0);
		memmove(dst, src, bkey_bytes(src));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		dst = bkey_next(dst);
		src = n;
	}

<<<<<<< HEAD
	op->keys.top = dst;

	bch_journal(cl);
}

static void bch_insert_data_endio(struct bio *bio, int error)
{
	struct closure *cl = bio->bi_private;
	struct btree_op *op = container_of(cl, struct btree_op, cl);
	struct search *s = container_of(op, struct search, op);

	if (error) {
		/* TODO: We could try to recover from this. */
		if (s->writeback)
			s->error = error;
		else if (s->write)
			set_closure_fn(cl, bch_insert_data_error, bcache_wq);
=======
	op->insert_keys.top = dst;

	bch_data_insert_keys(cl);
}

static void bch_data_insert_endio(struct bio *bio)
{
	struct closure *cl = bio->bi_private;
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);

	if (bio->bi_error) {
		/* TODO: We could try to recover from this. */
		if (op->writeback)
			op->error = bio->bi_error;
		else if (!op->replace)
			set_closure_fn(cl, bch_data_insert_error, op->wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		else
			set_closure_fn(cl, NULL, NULL);
	}

<<<<<<< HEAD
	bch_bbio_endio(op->c, bio, error, "writing data to cache");
}

static void bch_insert_data_loop(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);
	struct search *s = container_of(op, struct search, op);
	struct bio *bio = op->cache_bio, *n;

	if (op->skip)
		return bio_invalidate(cl);

	if (atomic_sub_return(bio_sectors(bio), &op->c->sectors_to_gc) < 0) {
		set_gc_sectors(op->c);
		bch_queue_gc(op->c);
	}

	/*
	 * Journal writes are marked REQ_FLUSH; if the original write was a
	 * flush, it'll wait on the journal write.
	 */
	bio->bi_rw &= ~(REQ_FLUSH|REQ_FUA);
=======
	bch_bbio_endio(op->c, bio, bio->bi_error, "writing data to cache");
}

static void bch_data_insert_start(struct closure *cl)
{
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);
	struct bio *bio = op->bio, *n;

	if (op->bypass)
		return bch_data_invalidate(cl);

	if (atomic_sub_return(bio_sectors(bio), &op->c->sectors_to_gc) < 0)
		wake_up_gc(op->c);

	/*
	 * Journal writes are marked REQ_PREFLUSH; if the original write was a
	 * flush, it'll wait on the journal write.
	 */
	bio->bi_opf &= ~(REQ_PREFLUSH|REQ_FUA);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	do {
		unsigned i;
		struct bkey *k;
<<<<<<< HEAD
		struct bio_set *split = s->d
			? s->d->bio_split : op->c->bio_split;

		/* 1 for the device pointer and 1 for the chksum */
		if (bch_keylist_realloc(&op->keys,
					1 + (op->csum ? 1 : 0),
					op->c))
			continue_at(cl, bch_journal, bcache_wq);

		k = op->keys.top;
		bkey_init(k);
		SET_KEY_INODE(k, op->inode);
		SET_KEY_OFFSET(k, bio->bi_sector);

		if (!bch_alloc_sectors(k, bio_sectors(bio), s))
			goto err;

		n = bch_bio_split(bio, KEY_SIZE(k), GFP_NOIO, split);
		if (!n) {
			__bkey_put(op->c, k);
			continue_at(cl, bch_insert_data_loop, bcache_wq);
		}

		n->bi_end_io	= bch_insert_data_endio;
		n->bi_private	= cl;

		if (s->writeback) {
=======
		struct bio_set *split = op->c->bio_split;

		/* 1 for the device pointer and 1 for the chksum */
		if (bch_keylist_realloc(&op->insert_keys,
					3 + (op->csum ? 1 : 0),
					op->c)) {
			continue_at(cl, bch_data_insert_keys, op->wq);
			return;
		}

		k = op->insert_keys.top;
		bkey_init(k);
		SET_KEY_INODE(k, op->inode);
		SET_KEY_OFFSET(k, bio->bi_iter.bi_sector);

		if (!bch_alloc_sectors(op->c, k, bio_sectors(bio),
				       op->write_point, op->write_prio,
				       op->writeback))
			goto err;

		n = bio_next_split(bio, KEY_SIZE(k), GFP_NOIO, split);

		n->bi_end_io	= bch_data_insert_endio;
		n->bi_private	= cl;

		if (op->writeback) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			SET_KEY_DIRTY(k, true);

			for (i = 0; i < KEY_PTRS(k); i++)
				SET_GC_MARK(PTR_BUCKET(op->c, k, i),
					    GC_MARK_DIRTY);
		}

		SET_KEY_CSUM(k, op->csum);
		if (KEY_CSUM(k))
			bio_csum(n, k);

<<<<<<< HEAD
		pr_debug("%s", pkey(k));
		bch_keylist_push(&op->keys);

		trace_bcache_cache_insert(n, n->bi_sector, n->bi_bdev);
		n->bi_rw |= REQ_WRITE;
=======
		trace_bcache_cache_insert(k);
		bch_keylist_push(&op->insert_keys);

		bio_set_op_attrs(n, REQ_OP_WRITE, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		bch_submit_bbio(n, op->c, k, 0);
	} while (n != bio);

	op->insert_data_done = true;
<<<<<<< HEAD
	continue_at(cl, bch_journal, bcache_wq);
err:
	/* bch_alloc_sectors() blocks if s->writeback = true */
	BUG_ON(s->writeback);
=======
	continue_at(cl, bch_data_insert_keys, op->wq);
	return;
err:
	/* bch_alloc_sectors() blocks if s->writeback = true */
	BUG_ON(op->writeback);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * But if it's not a writeback write we'd rather just bail out if
	 * there aren't any buckets ready to write to - it might take awhile and
	 * we might be starving btree writes for gc or something.
	 */

<<<<<<< HEAD
	if (s->write) {
=======
	if (!op->replace) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Writethrough write: We can't complete the write until we've
		 * updated the index. But we don't want to delay the write while
		 * we wait for buckets to be freed up, so just invalidate the
		 * rest of the write.
		 */
<<<<<<< HEAD
		op->skip = true;
		return bio_invalidate(cl);
=======
		op->bypass = true;
		return bch_data_invalidate(cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		/*
		 * From a cache miss, we can just insert the keys for the data
		 * we have written or bail out if we didn't do anything.
		 */
		op->insert_data_done = true;
		bio_put(bio);

<<<<<<< HEAD
		if (!bch_keylist_empty(&op->keys))
			continue_at(cl, bch_journal, bcache_wq);
=======
		if (!bch_keylist_empty(&op->insert_keys))
			continue_at(cl, bch_data_insert_keys, op->wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		else
			closure_return(cl);
	}
}

/**
<<<<<<< HEAD
 * bch_insert_data - stick some data in the cache
=======
 * bch_data_insert - stick some data in the cache
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This is the starting point for any data to end up in a cache device; it could
 * be from a normal write, or a writeback write, or a write to a flash only
 * volume - it's also used by the moving garbage collector to compact data in
 * mostly empty buckets.
 *
 * It first writes the data to the cache, creating a list of keys to be inserted
 * (if the data had to be fragmented there will be multiple keys); after the
 * data is written it calls bch_journal, and after the keys have been added to
 * the next journal write they're inserted into the btree.
 *
<<<<<<< HEAD
 * It inserts the data in op->cache_bio; bi_sector is used for the key offset,
 * and op->inode is used for the key inode.
 *
 * If op->skip is true, instead of inserting the data it invalidates the region
 * of the cache represented by op->cache_bio and op->inode.
 */
void bch_insert_data(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);

	bch_keylist_init(&op->keys);
	bio_get(op->cache_bio);
	bch_insert_data_loop(cl);
}

void bch_btree_insert_async(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);
	struct search *s = container_of(op, struct search, op);

	if (bch_btree_insert(op, op->c)) {
		s->error		= -ENOMEM;
		op->insert_data_done	= true;
	}

	if (op->insert_data_done) {
		bch_keylist_free(&op->keys);
		closure_return(cl);
	} else
		continue_at(cl, bch_insert_data_loop, bcache_wq);
}

/* Common code for the make_request functions */

static void request_endio(struct bio *bio, int error)
{
	struct closure *cl = bio->bi_private;

	if (error) {
		struct search *s = container_of(cl, struct search, cl);
		s->error = error;
		/* Only cache read errors are recoverable */
		s->recoverable = false;
	}

	bio_put(bio);
	closure_put(cl);
}

void bch_cache_read_endio(struct bio *bio, int error)
=======
 * It inserts the data in s->cache_bio; bi_sector is used for the key offset,
 * and op->inode is used for the key inode.
 *
 * If s->bypass is true, instead of inserting the data it invalidates the
 * region of the cache represented by s->cache_bio and op->inode.
 */
void bch_data_insert(struct closure *cl)
{
	struct data_insert_op *op = container_of(cl, struct data_insert_op, cl);

	trace_bcache_write(op->c, op->inode, op->bio,
			   op->writeback, op->bypass);

	bch_keylist_init(&op->insert_keys);
	bio_get(op->bio);
	bch_data_insert_start(cl);
}

/* Congested? */

unsigned bch_get_congested(struct cache_set *c)
{
	int i;
	long rand;

	if (!c->congested_read_threshold_us &&
	    !c->congested_write_threshold_us)
		return 0;

	i = (local_clock_us() - c->congested_last_us) / 1024;
	if (i < 0)
		return 0;

	i += atomic_read(&c->congested);
	if (i >= 0)
		return 0;

	i += CONGESTED_MAX;

	if (i > 0)
		i = fract_exp_two(i, 6);

	rand = get_random_int();
	i -= bitmap_weight(&rand, BITS_PER_LONG);

	return i > 0 ? i : 1;
}

static void add_sequential(struct task_struct *t)
{
	ewma_add(t->sequential_io_avg,
		 t->sequential_io, 8, 0);

	t->sequential_io = 0;
}

static struct hlist_head *iohash(struct cached_dev *dc, uint64_t k)
{
	return &dc->io_hash[hash_64(k, RECENT_IO_BITS)];
}

static bool check_should_bypass(struct cached_dev *dc, struct bio *bio)
{
	struct cache_set *c = dc->disk.c;
	unsigned mode = cache_mode(dc, bio);
	unsigned sectors, congested = bch_get_congested(c);
	struct task_struct *task = current;
	struct io *i;

	if (test_bit(BCACHE_DEV_DETACHING, &dc->disk.flags) ||
	    c->gc_stats.in_use > CUTOFF_CACHE_ADD ||
	    (bio_op(bio) == REQ_OP_DISCARD))
		goto skip;

	if (mode == CACHE_MODE_NONE ||
	    (mode == CACHE_MODE_WRITEAROUND &&
	     op_is_write(bio_op(bio))))
		goto skip;

	if (bio->bi_iter.bi_sector & (c->sb.block_size - 1) ||
	    bio_sectors(bio) & (c->sb.block_size - 1)) {
		pr_debug("skipping unaligned io");
		goto skip;
	}

	if (bypass_torture_test(dc)) {
		if ((get_random_int() & 3) == 3)
			goto skip;
		else
			goto rescale;
	}

	if (!congested && !dc->sequential_cutoff)
		goto rescale;

	if (!congested &&
	    mode == CACHE_MODE_WRITEBACK &&
	    op_is_write(bio_op(bio)) &&
	    (bio->bi_opf & REQ_SYNC))
		goto rescale;

	spin_lock(&dc->io_lock);

	hlist_for_each_entry(i, iohash(dc, bio->bi_iter.bi_sector), hash)
		if (i->last == bio->bi_iter.bi_sector &&
		    time_before(jiffies, i->jiffies))
			goto found;

	i = list_first_entry(&dc->io_lru, struct io, lru);

	add_sequential(task);
	i->sequential = 0;
found:
	if (i->sequential + bio->bi_iter.bi_size > i->sequential)
		i->sequential	+= bio->bi_iter.bi_size;

	i->last			 = bio_end_sector(bio);
	i->jiffies		 = jiffies + msecs_to_jiffies(5000);
	task->sequential_io	 = i->sequential;

	hlist_del(&i->hash);
	hlist_add_head(&i->hash, iohash(dc, i->last));
	list_move_tail(&i->lru, &dc->io_lru);

	spin_unlock(&dc->io_lock);

	sectors = max(task->sequential_io,
		      task->sequential_io_avg) >> 9;

	if (dc->sequential_cutoff &&
	    sectors >= dc->sequential_cutoff >> 9) {
		trace_bcache_bypass_sequential(bio);
		goto skip;
	}

	if (congested && sectors >= congested) {
		trace_bcache_bypass_congested(bio);
		goto skip;
	}

rescale:
	bch_rescale_priorities(c, bio_sectors(bio));
	return false;
skip:
	bch_mark_sectors_bypassed(c, dc, bio_sectors(bio));
	return true;
}

/* Cache lookup */

struct search {
	/* Stack frame for bio_complete */
	struct closure		cl;

	struct bbio		bio;
	struct bio		*orig_bio;
	struct bio		*cache_miss;
	struct bcache_device	*d;

	unsigned		insert_bio_sectors;
	unsigned		recoverable:1;
	unsigned		write:1;
	unsigned		read_dirty_data:1;
	unsigned		cache_missed:1;

	unsigned long		start_time;

	struct btree_op		op;
	struct data_insert_op	iop;
};

static void bch_cache_read_endio(struct bio *bio)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct bbio *b = container_of(bio, struct bbio, bio);
	struct closure *cl = bio->bi_private;
	struct search *s = container_of(cl, struct search, cl);

	/*
	 * If the bucket was reused while our bio was in flight, we might have
	 * read the wrong data. Set s->error but not error so it doesn't get
	 * counted against the cache device, but we'll still reread the data
	 * from the backing device.
	 */

<<<<<<< HEAD
	if (error)
		s->error = error;
	else if (ptr_stale(s->op.c, &b->key, 0)) {
		atomic_long_inc(&s->op.c->cache_read_races);
		s->error = -EINTR;
	}

	bch_bbio_endio(s->op.c, bio, error, "reading from cache");
=======
	if (bio->bi_error)
		s->iop.error = bio->bi_error;
	else if (!KEY_DIRTY(&b->key) &&
		 ptr_stale(s->iop.c, &b->key, 0)) {
		atomic_long_inc(&s->iop.c->cache_read_races);
		s->iop.error = -EINTR;
	}

	bch_bbio_endio(s->iop.c, bio, bio->bi_error, "reading from cache");
}

/*
 * Read from a single key, handling the initial cache miss if the key starts in
 * the middle of the bio
 */
static int cache_lookup_fn(struct btree_op *op, struct btree *b, struct bkey *k)
{
	struct search *s = container_of(op, struct search, op);
	struct bio *n, *bio = &s->bio.bio;
	struct bkey *bio_key;
	unsigned ptr;

	if (bkey_cmp(k, &KEY(s->iop.inode, bio->bi_iter.bi_sector, 0)) <= 0)
		return MAP_CONTINUE;

	if (KEY_INODE(k) != s->iop.inode ||
	    KEY_START(k) > bio->bi_iter.bi_sector) {
		unsigned bio_sectors = bio_sectors(bio);
		unsigned sectors = KEY_INODE(k) == s->iop.inode
			? min_t(uint64_t, INT_MAX,
				KEY_START(k) - bio->bi_iter.bi_sector)
			: INT_MAX;

		int ret = s->d->cache_miss(b, s, bio, sectors);
		if (ret != MAP_CONTINUE)
			return ret;

		/* if this was a complete miss we shouldn't get here */
		BUG_ON(bio_sectors <= sectors);
	}

	if (!KEY_SIZE(k))
		return MAP_CONTINUE;

	/* XXX: figure out best pointer - for multiple cache devices */
	ptr = 0;

	PTR_BUCKET(b->c, k, ptr)->prio = INITIAL_PRIO;

	if (KEY_DIRTY(k))
		s->read_dirty_data = true;

	n = bio_next_split(bio, min_t(uint64_t, INT_MAX,
				      KEY_OFFSET(k) - bio->bi_iter.bi_sector),
			   GFP_NOIO, s->d->bio_split);

	bio_key = &container_of(n, struct bbio, bio)->key;
	bch_bkey_copy_single_ptr(bio_key, k, ptr);

	bch_cut_front(&KEY(s->iop.inode, n->bi_iter.bi_sector, 0), bio_key);
	bch_cut_back(&KEY(s->iop.inode, bio_end_sector(n), 0), bio_key);

	n->bi_end_io	= bch_cache_read_endio;
	n->bi_private	= &s->cl;

	/*
	 * The bucket we're reading from might be reused while our bio
	 * is in flight, and we could then end up reading the wrong
	 * data.
	 *
	 * We guard against this by checking (in cache_read_endio()) if
	 * the pointer is stale again; if so, we treat it as an error
	 * and reread from the backing device (but we don't pass that
	 * error up anywhere).
	 */

	__bch_submit_bbio(n, b->c);
	return n == bio ? MAP_DONE : MAP_CONTINUE;
}

static void cache_lookup(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, iop.cl);
	struct bio *bio = &s->bio.bio;
	int ret;

	bch_btree_op_init(&s->op, -1);

	ret = bch_btree_map_keys(&s->op, s->iop.c,
				 &KEY(s->iop.inode, bio->bi_iter.bi_sector, 0),
				 cache_lookup_fn, MAP_END_KEY);
	if (ret == -EAGAIN) {
		continue_at(cl, cache_lookup, bcache_wq);
		return;
	}

	closure_return(cl);
}

/* Common code for the make_request functions */

static void request_endio(struct bio *bio)
{
	struct closure *cl = bio->bi_private;

	if (bio->bi_error) {
		struct search *s = container_of(cl, struct search, cl);
		s->iop.error = bio->bi_error;
		/* Only cache read errors are recoverable */
		s->recoverable = false;
	}

	bio_put(bio);
	closure_put(cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void bio_complete(struct search *s)
{
	if (s->orig_bio) {
<<<<<<< HEAD
		int cpu, rw = bio_data_dir(s->orig_bio);
		unsigned long duration = jiffies - s->start_time;

		cpu = part_stat_lock();
		part_round_stats(cpu, &s->d->disk->part0);
		part_stat_add(cpu, &s->d->disk->part0, ticks[rw], duration);
		part_stat_unlock();

		trace_bcache_request_end(s, s->orig_bio);
		bio_endio(s->orig_bio, s->error);
=======
		generic_end_io_acct(bio_data_dir(s->orig_bio),
				    &s->d->disk->part0, s->start_time);

		trace_bcache_request_end(s->d, s->orig_bio);
		s->orig_bio->bi_error = s->iop.error;
		bio_endio(s->orig_bio);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		s->orig_bio = NULL;
	}
}

<<<<<<< HEAD
static void do_bio_hook(struct search *s)
{
	struct bio *bio = &s->bio.bio;
	memcpy(bio, s->orig_bio, sizeof(struct bio));

	bio->bi_end_io		= request_endio;
	bio->bi_private		= &s->cl;
	atomic_set(&bio->bi_cnt, 3);
=======
static void do_bio_hook(struct search *s, struct bio *orig_bio)
{
	struct bio *bio = &s->bio.bio;

	bio_init(bio);
	__bio_clone_fast(bio, orig_bio);
	bio->bi_end_io		= request_endio;
	bio->bi_private		= &s->cl;

	bio_cnt_set(bio, 3);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void search_free(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
<<<<<<< HEAD
	bio_complete(s);

	if (s->op.cache_bio)
		bio_put(s->op.cache_bio);

	if (s->unaligned_bvec)
		mempool_free(s->bio.bio.bi_io_vec, s->d->unaligned_bvec);

=======

	if (s->iop.bio)
		bio_put(s->iop.bio);

	bio_complete(s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	closure_debug_destroy(cl);
	mempool_free(s, s->d->c->search);
}

<<<<<<< HEAD
static struct search *search_alloc(struct bio *bio, struct bcache_device *d)
{
	struct bio_vec *bv;
	struct search *s = mempool_alloc(d->c->search, GFP_NOIO);
	memset(s, 0, offsetof(struct search, op.keys));

	__closure_init(&s->cl, NULL);

	s->op.inode		= d->id;
	s->op.c			= d->c;
	s->d			= d;
	s->op.lock		= -1;
	s->task			= current;
	s->orig_bio		= bio;
	s->write		= (bio->bi_rw & REQ_WRITE) != 0;
	s->op.flush_journal	= (bio->bi_rw & (REQ_FLUSH|REQ_FUA)) != 0;
	s->op.skip		= (bio->bi_rw & REQ_DISCARD) != 0;
	s->recoverable		= 1;
	s->start_time		= jiffies;
	do_bio_hook(s);

	if (bio->bi_size != bio_segments(bio) * PAGE_SIZE) {
		bv = mempool_alloc(d->unaligned_bvec, GFP_NOIO);
		memcpy(bv, bio_iovec(bio),
		       sizeof(struct bio_vec) * bio_segments(bio));

		s->bio.bio.bi_io_vec	= bv;
		s->unaligned_bvec	= 1;
	}
=======
static inline struct search *search_alloc(struct bio *bio,
					  struct bcache_device *d)
{
	struct search *s;

	s = mempool_alloc(d->c->search, GFP_NOIO);

	closure_init(&s->cl, NULL);
	do_bio_hook(s, bio);

	s->orig_bio		= bio;
	s->cache_miss		= NULL;
	s->cache_missed		= 0;
	s->d			= d;
	s->recoverable		= 1;
	s->write		= op_is_write(bio_op(bio));
	s->read_dirty_data	= 0;
	s->start_time		= jiffies;

	s->iop.c		= d->c;
	s->iop.bio		= NULL;
	s->iop.inode		= d->id;
	s->iop.write_point	= hash_long((unsigned long) current, 16);
	s->iop.write_prio	= 0;
	s->iop.error		= 0;
	s->iop.flags		= 0;
	s->iop.flush_journal	= (bio->bi_opf & (REQ_PREFLUSH|REQ_FUA)) != 0;
	s->iop.wq		= bcache_wq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return s;
}

<<<<<<< HEAD
static void btree_read_async(struct closure *cl)
{
	struct btree_op *op = container_of(cl, struct btree_op, cl);

	int ret = btree_root(search_recurse, op->c, op);

	if (ret == -EAGAIN)
		continue_at(cl, btree_read_async, bcache_wq);

	closure_return(cl);
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Cached devices */

static void cached_dev_bio_complete(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);

	search_free(cl);
	cached_dev_put(dc);
}

/* Process reads */

<<<<<<< HEAD
static void cached_dev_read_complete(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);

	if (s->op.insert_collision)
		bch_mark_cache_miss_collision(s);

	if (s->op.cache_bio) {
		int i;
		struct bio_vec *bv;

		__bio_for_each_segment(bv, s->op.cache_bio, i, 0)
			__free_page(bv->bv_page);
	}
=======
static void cached_dev_cache_miss_done(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);

	if (s->iop.replace_collision)
		bch_mark_cache_miss_collision(s->iop.c, s->d);

	if (s->iop.bio)
		bio_free_pages(s->iop.bio);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	cached_dev_bio_complete(cl);
}

<<<<<<< HEAD
static void request_read_error(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
	struct bio_vec *bv;
	int i;

	if (s->recoverable) {
		/* The cache read failed, but we can retry from the backing
		 * device.
		 */
		pr_debug("recovering at sector %llu",
			 (uint64_t) s->orig_bio->bi_sector);

		s->error = 0;
		bv = s->bio.bio.bi_io_vec;
		do_bio_hook(s);
		s->bio.bio.bi_io_vec = bv;

		if (!s->unaligned_bvec)
			bio_for_each_segment(bv, s->orig_bio, i)
				bv->bv_offset = 0, bv->bv_len = PAGE_SIZE;
		else
			memcpy(s->bio.bio.bi_io_vec,
			       bio_iovec(s->orig_bio),
			       sizeof(struct bio_vec) *
			       bio_segments(s->orig_bio));

		/* XXX: invalidate cache */

		trace_bcache_read_retry(&s->bio.bio);
		closure_bio_submit(&s->bio.bio, &s->cl, s->d);
	}

	continue_at(cl, cached_dev_read_complete, NULL);
}

static void request_read_done(struct closure *cl)
=======
static void cached_dev_read_error(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
	struct bio *bio = &s->bio.bio;

	/*
	 * If read request hit dirty data (s->read_dirty_data is true),
	 * then recovery a failed read request from cached device may
	 * get a stale data back. So read failure recovery is only
	 * permitted when read request hit clean data in cache device,
	 * or when cache read race happened.
	 */
	if (s->recoverable && !s->read_dirty_data) {
		/* Retry from the backing device: */
		trace_bcache_read_retry(s->orig_bio);

		s->iop.error = 0;
		do_bio_hook(s, s->orig_bio);

		/* XXX: invalidate cache */

		closure_bio_submit(bio, cl);
	}

	continue_at(cl, cached_dev_cache_miss_done, NULL);
}

static void cached_dev_read_done(struct closure *cl)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct search *s = container_of(cl, struct search, cl);
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);

	/*
<<<<<<< HEAD
	 * s->cache_bio != NULL implies that we had a cache miss; cache_bio now
	 * contains data ready to be inserted into the cache.
=======
	 * We had a cache miss; cache_bio now contains data ready to be inserted
	 * into the cache.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 *
	 * First, we copy the data we just read from cache_bio's bounce buffers
	 * to the buffers the original bio pointed to:
	 */

<<<<<<< HEAD
	if (s->op.cache_bio) {
		struct bio_vec *src, *dst;
		unsigned src_offset, dst_offset, bytes;
		void *dst_ptr;

		bio_reset(s->op.cache_bio);
		s->op.cache_bio->bi_sector	= s->cache_miss->bi_sector;
		s->op.cache_bio->bi_bdev	= s->cache_miss->bi_bdev;
		s->op.cache_bio->bi_size	= s->cache_bio_sectors << 9;
		bch_bio_map(s->op.cache_bio, NULL);

		src = bio_iovec(s->op.cache_bio);
		dst = bio_iovec(s->cache_miss);
		src_offset = src->bv_offset;
		dst_offset = dst->bv_offset;
		dst_ptr = kmap(dst->bv_page);

		while (1) {
			if (dst_offset == dst->bv_offset + dst->bv_len) {
				kunmap(dst->bv_page);
				dst++;
				if (dst == bio_iovec_idx(s->cache_miss,
						s->cache_miss->bi_vcnt))
					break;

				dst_offset = dst->bv_offset;
				dst_ptr = kmap(dst->bv_page);
			}

			if (src_offset == src->bv_offset + src->bv_len) {
				src++;
				if (src == bio_iovec_idx(s->op.cache_bio,
						 s->op.cache_bio->bi_vcnt))
					BUG();

				src_offset = src->bv_offset;
			}

			bytes = min(dst->bv_offset + dst->bv_len - dst_offset,
				    src->bv_offset + src->bv_len - src_offset);

			memcpy(dst_ptr + dst_offset,
			       page_address(src->bv_page) + src_offset,
			       bytes);

			src_offset	+= bytes;
			dst_offset	+= bytes;
		}
=======
	if (s->iop.bio) {
		bio_reset(s->iop.bio);
		s->iop.bio->bi_iter.bi_sector = s->cache_miss->bi_iter.bi_sector;
		s->iop.bio->bi_bdev = s->cache_miss->bi_bdev;
		s->iop.bio->bi_iter.bi_size = s->insert_bio_sectors << 9;
		bch_bio_map(s->iop.bio, NULL);

		bio_copy_data(s->cache_miss, s->iop.bio);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		bio_put(s->cache_miss);
		s->cache_miss = NULL;
	}

<<<<<<< HEAD
	if (verify(dc, &s->bio.bio) && s->recoverable)
		bch_data_verify(s);

	bio_complete(s);

	if (s->op.cache_bio &&
	    !test_bit(CACHE_SET_STOPPING, &s->op.c->flags)) {
		s->op.type = BTREE_REPLACE;
		closure_call(&s->op.cl, bch_insert_data, NULL, cl);
	}

	continue_at(cl, cached_dev_read_complete, NULL);
}

static void request_read_done_bh(struct closure *cl)
=======
	if (verify(dc, &s->bio.bio) && s->recoverable && !s->read_dirty_data)
		bch_data_verify(dc, s->orig_bio);

	bio_complete(s);

	if (s->iop.bio &&
	    !test_bit(CACHE_SET_STOPPING, &s->iop.c->flags)) {
		BUG_ON(!s->iop.replace);
		closure_call(&s->iop.cl, bch_data_insert, NULL, cl);
	}

	continue_at(cl, cached_dev_cache_miss_done, NULL);
}

static void cached_dev_read_done_bh(struct closure *cl)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct search *s = container_of(cl, struct search, cl);
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);

<<<<<<< HEAD
	bch_mark_cache_accounting(s, !s->cache_miss, s->op.skip);

	if (s->error)
		continue_at_nobarrier(cl, request_read_error, bcache_wq);
	else if (s->op.cache_bio || verify(dc, &s->bio.bio))
		continue_at_nobarrier(cl, request_read_done, bcache_wq);
	else
		continue_at_nobarrier(cl, cached_dev_read_complete, NULL);
=======
	bch_mark_cache_accounting(s->iop.c, s->d,
				  !s->cache_missed, s->iop.bypass);
	trace_bcache_read(s->orig_bio, !s->cache_miss, s->iop.bypass);

	if (s->iop.error)
		continue_at_nobarrier(cl, cached_dev_read_error, bcache_wq);
	else if (s->iop.bio || verify(dc, &s->bio.bio))
		continue_at_nobarrier(cl, cached_dev_read_done, bcache_wq);
	else
		continue_at_nobarrier(cl, cached_dev_bio_complete, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cached_dev_cache_miss(struct btree *b, struct search *s,
				 struct bio *bio, unsigned sectors)
{
<<<<<<< HEAD
	int ret = 0;
	unsigned reada;
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);
	struct bio *miss;

	miss = bch_bio_split(bio, sectors, GFP_NOIO, s->d->bio_split);
	if (!miss)
		return -EAGAIN;

	if (miss == bio)
		s->op.lookup_done = true;

	miss->bi_end_io		= request_endio;
	miss->bi_private	= &s->cl;

	if (s->cache_miss || s->op.skip)
		goto out_submit;

	if (miss != bio ||
	    (bio->bi_rw & REQ_RAHEAD) ||
	    (bio->bi_rw & REQ_META) ||
	    s->op.c->gc_stats.in_use >= CUTOFF_CACHE_READA)
		reada = 0;
	else {
		reada = min(dc->readahead >> 9,
			    sectors - bio_sectors(miss));

		if (bio_end(miss) + reada > bdev_sectors(miss->bi_bdev))
			reada = bdev_sectors(miss->bi_bdev) - bio_end(miss);
	}

	s->cache_bio_sectors = bio_sectors(miss) + reada;
	s->op.cache_bio = bio_alloc_bioset(GFP_NOWAIT,
			DIV_ROUND_UP(s->cache_bio_sectors, PAGE_SECTORS),
			dc->disk.bio_split);

	if (!s->op.cache_bio)
		goto out_submit;

	s->op.cache_bio->bi_sector	= miss->bi_sector;
	s->op.cache_bio->bi_bdev	= miss->bi_bdev;
	s->op.cache_bio->bi_size	= s->cache_bio_sectors << 9;

	s->op.cache_bio->bi_end_io	= request_endio;
	s->op.cache_bio->bi_private	= &s->cl;

	/* btree_search_recurse()'s btree iterator is no good anymore */
	ret = -EINTR;
	if (!bch_btree_insert_check_key(b, &s->op, s->op.cache_bio))
		goto out_put;

	bch_bio_map(s->op.cache_bio, NULL);
	if (bch_bio_alloc_pages(s->op.cache_bio, __GFP_NOWARN|GFP_NOIO))
		goto out_put;

	s->cache_miss = miss;
	bio_get(s->op.cache_bio);

	trace_bcache_cache_miss(s->orig_bio);
	closure_bio_submit(s->op.cache_bio, &s->cl, s->d);

	return ret;
out_put:
	bio_put(s->op.cache_bio);
	s->op.cache_bio = NULL;
out_submit:
	closure_bio_submit(miss, &s->cl, s->d);
	return ret;
}

static void request_read(struct cached_dev *dc, struct search *s)
{
	struct closure *cl = &s->cl;

	check_should_skip(dc, s);
	closure_call(&s->op.cl, btree_read_async, NULL, cl);

	continue_at(cl, request_read_done_bh, NULL);
=======
	int ret = MAP_CONTINUE;
	unsigned reada = 0;
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);
	struct bio *miss, *cache_bio;

	s->cache_missed = 1;

	if (s->cache_miss || s->iop.bypass) {
		miss = bio_next_split(bio, sectors, GFP_NOIO, s->d->bio_split);
		ret = miss == bio ? MAP_DONE : MAP_CONTINUE;
		goto out_submit;
	}

	if (!(bio->bi_opf & REQ_RAHEAD) &&
	    !(bio->bi_opf & REQ_META) &&
	    s->iop.c->gc_stats.in_use < CUTOFF_CACHE_READA)
		reada = min_t(sector_t, dc->readahead >> 9,
			      bdev_sectors(bio->bi_bdev) - bio_end_sector(bio));

	s->insert_bio_sectors = min(sectors, bio_sectors(bio) + reada);

	s->iop.replace_key = KEY(s->iop.inode,
				 bio->bi_iter.bi_sector + s->insert_bio_sectors,
				 s->insert_bio_sectors);

	ret = bch_btree_insert_check_key(b, &s->op, &s->iop.replace_key);
	if (ret)
		return ret;

	s->iop.replace = true;

	miss = bio_next_split(bio, sectors, GFP_NOIO, s->d->bio_split);

	/* btree_search_recurse()'s btree iterator is no good anymore */
	ret = miss == bio ? MAP_DONE : -EINTR;

	cache_bio = bio_alloc_bioset(GFP_NOWAIT,
			DIV_ROUND_UP(s->insert_bio_sectors, PAGE_SECTORS),
			dc->disk.bio_split);
	if (!cache_bio)
		goto out_submit;

	cache_bio->bi_iter.bi_sector	= miss->bi_iter.bi_sector;
	cache_bio->bi_bdev		= miss->bi_bdev;
	cache_bio->bi_iter.bi_size	= s->insert_bio_sectors << 9;

	cache_bio->bi_end_io	= request_endio;
	cache_bio->bi_private	= &s->cl;

	bch_bio_map(cache_bio, NULL);
	if (bio_alloc_pages(cache_bio, __GFP_NOWARN|GFP_NOIO))
		goto out_put;

	if (reada)
		bch_mark_cache_readahead(s->iop.c, s->d);

	s->cache_miss	= miss;
	s->iop.bio	= cache_bio;
	bio_get(cache_bio);
	closure_bio_submit(cache_bio, &s->cl);

	return ret;
out_put:
	bio_put(cache_bio);
out_submit:
	miss->bi_end_io		= request_endio;
	miss->bi_private	= &s->cl;
	closure_bio_submit(miss, &s->cl);
	return ret;
}

static void cached_dev_read(struct cached_dev *dc, struct search *s)
{
	struct closure *cl = &s->cl;

	closure_call(&s->iop.cl, cache_lookup, NULL, cl);
	continue_at(cl, cached_dev_read_done_bh, NULL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Process writes */

static void cached_dev_write_complete(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
	struct cached_dev *dc = container_of(s->d, struct cached_dev, disk);

	up_read_non_owner(&dc->writeback_lock);
	cached_dev_bio_complete(cl);
}

<<<<<<< HEAD
static bool should_writeback(struct cached_dev *dc, struct bio *bio)
{
	unsigned threshold = (bio->bi_rw & REQ_SYNC)
		? CUTOFF_WRITEBACK_SYNC
		: CUTOFF_WRITEBACK;

	return !atomic_read(&dc->disk.detaching) &&
		cache_mode(dc, bio) == CACHE_MODE_WRITEBACK &&
		dc->disk.c->gc_stats.in_use < threshold;
}

static void request_write(struct cached_dev *dc, struct search *s)
{
	struct closure *cl = &s->cl;
	struct bio *bio = &s->bio.bio;
	struct bkey start, end;
	start = KEY(dc->disk.id, bio->bi_sector, 0);
	end = KEY(dc->disk.id, bio_end(bio), 0);

	bch_keybuf_check_overlapping(&s->op.c->moving_gc_keys, &start, &end);

	check_should_skip(dc, s);
	down_read_non_owner(&dc->writeback_lock);

	if (bch_keybuf_check_overlapping(&dc->writeback_keys, &start, &end)) {
		s->op.skip	= false;
		s->writeback	= true;
	}

	if (bio->bi_rw & REQ_DISCARD)
		goto skip;

	if (s->op.skip)
		goto skip;

	if (should_writeback(dc, s->orig_bio))
		s->writeback = true;

	if (!s->writeback) {
		s->op.cache_bio = bio_clone_bioset(bio, GFP_NOIO,
						   dc->disk.bio_split);

		trace_bcache_writethrough(s->orig_bio);
		closure_bio_submit(bio, cl, s->d);
	} else {
		trace_bcache_writeback(s->orig_bio);
		bch_writeback_add(dc, bio_sectors(bio));
		s->op.cache_bio = bio;

		if (bio->bi_rw & REQ_FLUSH) {
=======
static void cached_dev_write(struct cached_dev *dc, struct search *s)
{
	struct closure *cl = &s->cl;
	struct bio *bio = &s->bio.bio;
	struct bkey start = KEY(dc->disk.id, bio->bi_iter.bi_sector, 0);
	struct bkey end = KEY(dc->disk.id, bio_end_sector(bio), 0);

	bch_keybuf_check_overlapping(&s->iop.c->moving_gc_keys, &start, &end);

	down_read_non_owner(&dc->writeback_lock);
	if (bch_keybuf_check_overlapping(&dc->writeback_keys, &start, &end)) {
		/*
		 * We overlap with some dirty data undergoing background
		 * writeback, force this write to writeback
		 */
		s->iop.bypass = false;
		s->iop.writeback = true;
	}

	/*
	 * Discards aren't _required_ to do anything, so skipping if
	 * check_overlapping returned true is ok
	 *
	 * But check_overlapping drops dirty keys for which io hasn't started,
	 * so we still want to call it.
	 */
	if (bio_op(bio) == REQ_OP_DISCARD)
		s->iop.bypass = true;

	if (should_writeback(dc, s->orig_bio,
			     cache_mode(dc, bio),
			     s->iop.bypass)) {
		s->iop.bypass = false;
		s->iop.writeback = true;
	}

	if (s->iop.bypass) {
		s->iop.bio = s->orig_bio;
		bio_get(s->iop.bio);

		if ((bio_op(bio) != REQ_OP_DISCARD) ||
		    blk_queue_discard(bdev_get_queue(dc->bdev)))
			closure_bio_submit(bio, cl);
	} else if (s->iop.writeback) {
		bch_writeback_add(dc);
		s->iop.bio = bio;

		if (bio->bi_opf & REQ_PREFLUSH) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			/* Also need to send a flush to the backing device */
			struct bio *flush = bio_alloc_bioset(GFP_NOIO, 0,
							     dc->disk.bio_split);

<<<<<<< HEAD
			flush->bi_rw	= WRITE_FLUSH;
			flush->bi_bdev	= bio->bi_bdev;
			flush->bi_end_io = request_endio;
			flush->bi_private = cl;

			closure_bio_submit(flush, cl, s->d);
		}
	}
out:
	closure_call(&s->op.cl, bch_insert_data, NULL, cl);
	continue_at(cl, cached_dev_write_complete, NULL);
skip:
	s->op.skip = true;
	s->op.cache_bio = s->orig_bio;
	bio_get(s->op.cache_bio);
	trace_bcache_write_skip(s->orig_bio);

	if ((bio->bi_rw & REQ_DISCARD) &&
	    !blk_queue_discard(bdev_get_queue(dc->bdev)))
		goto out;

	closure_bio_submit(bio, cl, s->d);
	goto out;
}

static void request_nodata(struct cached_dev *dc, struct search *s)
{
	struct closure *cl = &s->cl;
	struct bio *bio = &s->bio.bio;

	if (bio->bi_rw & REQ_DISCARD) {
		request_write(dc, s);
		return;
	}

	if (s->op.flush_journal)
		bch_journal_meta(s->op.c, cl);

	closure_bio_submit(bio, cl, s->d);
=======
			flush->bi_bdev	= bio->bi_bdev;
			flush->bi_end_io = request_endio;
			flush->bi_private = cl;
			bio_set_op_attrs(flush, REQ_OP_WRITE, WRITE_FLUSH);

			closure_bio_submit(flush, cl);
		}
	} else {
		s->iop.bio = bio_clone_fast(bio, GFP_NOIO, dc->disk.bio_split);

		closure_bio_submit(bio, cl);
	}

	closure_call(&s->iop.cl, bch_data_insert, NULL, cl);
	continue_at(cl, cached_dev_write_complete, NULL);
}

static void cached_dev_nodata(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);
	struct bio *bio = &s->bio.bio;

	if (s->iop.flush_journal)
		bch_journal_meta(s->iop.c, cl);

	/* If it's a flush, we send the flush to the backing device too */
	closure_bio_submit(bio, cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	continue_at(cl, cached_dev_bio_complete, NULL);
}

/* Cached devices - read & write stuff */

<<<<<<< HEAD
int bch_get_congested(struct cache_set *c)
{
	int i;

	if (!c->congested_read_threshold_us &&
	    !c->congested_write_threshold_us)
		return 0;

	i = (local_clock_us() - c->congested_last_us) / 1024;
	if (i < 0)
		return 0;

	i += atomic_read(&c->congested);
	if (i >= 0)
		return 0;

	i += CONGESTED_MAX;

	return i <= 0 ? 1 : fract_exp_two(i, 6);
}

static void add_sequential(struct task_struct *t)
{
	ewma_add(t->sequential_io_avg,
		 t->sequential_io, 8, 0);

	t->sequential_io = 0;
}

static struct hlist_head *iohash(struct cached_dev *dc, uint64_t k)
{
	return &dc->io_hash[hash_64(k, RECENT_IO_BITS)];
}

static void check_should_skip(struct cached_dev *dc, struct search *s)
{
	struct cache_set *c = s->op.c;
	struct bio *bio = &s->bio.bio;

	long rand;
	int cutoff = bch_get_congested(c);
	unsigned mode = cache_mode(dc, bio);

	if (atomic_read(&dc->disk.detaching) ||
	    c->gc_stats.in_use > CUTOFF_CACHE_ADD ||
	    (bio->bi_rw & REQ_DISCARD))
		goto skip;

	if (mode == CACHE_MODE_NONE ||
	    (mode == CACHE_MODE_WRITEAROUND &&
	     (bio->bi_rw & REQ_WRITE)))
		goto skip;

	if (bio->bi_sector   & (c->sb.block_size - 1) ||
	    bio_sectors(bio) & (c->sb.block_size - 1)) {
		pr_debug("skipping unaligned io");
		goto skip;
	}

	if (!cutoff) {
		cutoff = dc->sequential_cutoff >> 9;

		if (!cutoff)
			goto rescale;

		if (mode == CACHE_MODE_WRITEBACK &&
		    (bio->bi_rw & REQ_WRITE) &&
		    (bio->bi_rw & REQ_SYNC))
			goto rescale;
	}

	if (dc->sequential_merge) {
		struct io *i;

		spin_lock(&dc->io_lock);

		hlist_for_each_entry(i, iohash(dc, bio->bi_sector), hash)
			if (i->last == bio->bi_sector &&
			    time_before(jiffies, i->jiffies))
				goto found;

		i = list_first_entry(&dc->io_lru, struct io, lru);

		add_sequential(s->task);
		i->sequential = 0;
found:
		if (i->sequential + bio->bi_size > i->sequential)
			i->sequential	+= bio->bi_size;

		i->last			 = bio_end(bio);
		i->jiffies		 = jiffies + msecs_to_jiffies(5000);
		s->task->sequential_io	 = i->sequential;

		hlist_del(&i->hash);
		hlist_add_head(&i->hash, iohash(dc, i->last));
		list_move_tail(&i->lru, &dc->io_lru);

		spin_unlock(&dc->io_lock);
	} else {
		s->task->sequential_io = bio->bi_size;

		add_sequential(s->task);
	}

	rand = get_random_int();
	cutoff -= bitmap_weight(&rand, BITS_PER_LONG);

	if (cutoff <= (int) (max(s->task->sequential_io,
				 s->task->sequential_io_avg) >> 9))
		goto skip;

rescale:
	bch_rescale_priorities(c, bio_sectors(bio));
	return;
skip:
	bch_mark_sectors_bypassed(s, bio_sectors(bio));
	s->op.skip = true;
}

static void cached_dev_make_request(struct request_queue *q, struct bio *bio)
=======
static blk_qc_t cached_dev_make_request(struct request_queue *q,
					struct bio *bio)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct search *s;
	struct bcache_device *d = bio->bi_bdev->bd_disk->private_data;
	struct cached_dev *dc = container_of(d, struct cached_dev, disk);
<<<<<<< HEAD
	int cpu, rw = bio_data_dir(bio);

	cpu = part_stat_lock();
	part_stat_inc(cpu, &d->disk->part0, ios[rw]);
	part_stat_add(cpu, &d->disk->part0, sectors[rw], bio_sectors(bio));
	part_stat_unlock();

	bio->bi_bdev = dc->bdev;
	bio->bi_sector += dc->sb.data_offset;

	if (cached_dev_get(dc)) {
		s = search_alloc(bio, d);
		trace_bcache_request_start(s, bio);

		if (!bio_has_data(bio))
			request_nodata(dc, s);
		else if (rw)
			request_write(dc, s);
		else
			request_read(dc, s);
	} else {
		if ((bio->bi_rw & REQ_DISCARD) &&
		    !blk_queue_discard(bdev_get_queue(dc->bdev)))
			bio_endio(bio, 0);
		else
			bch_generic_make_request(bio, &d->bio_split_hook);
	}
=======
	int rw = bio_data_dir(bio);

	generic_start_io_acct(rw, bio_sectors(bio), &d->disk->part0);

	bio->bi_bdev = dc->bdev;
	bio->bi_iter.bi_sector += dc->sb.data_offset;

	if (cached_dev_get(dc)) {
		s = search_alloc(bio, d);
		trace_bcache_request_start(s->d, bio);

		if (!bio->bi_iter.bi_size) {
			/*
			 * can't call bch_journal_meta from under
			 * generic_make_request
			 */
			continue_at_nobarrier(&s->cl,
					      cached_dev_nodata,
					      bcache_wq);
		} else {
			s->iop.bypass = check_should_bypass(dc, bio);

			if (rw)
				cached_dev_write(dc, s);
			else
				cached_dev_read(dc, s);
		}
	} else {
		if ((bio_op(bio) == REQ_OP_DISCARD) &&
		    !blk_queue_discard(bdev_get_queue(dc->bdev)))
			bio_endio(bio);
		else
			generic_make_request(bio);
	}

	return BLK_QC_T_NONE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int cached_dev_ioctl(struct bcache_device *d, fmode_t mode,
			    unsigned int cmd, unsigned long arg)
{
	struct cached_dev *dc = container_of(d, struct cached_dev, disk);
	return __blkdev_driver_ioctl(dc->bdev, mode, cmd, arg);
}

static int cached_dev_congested(void *data, int bits)
{
	struct bcache_device *d = data;
	struct cached_dev *dc = container_of(d, struct cached_dev, disk);
	struct request_queue *q = bdev_get_queue(dc->bdev);
	int ret = 0;

	if (bdi_congested(&q->backing_dev_info, bits))
		return 1;

	if (cached_dev_get(dc)) {
		unsigned i;
		struct cache *ca;

		for_each_cache(ca, d->c, i) {
			q = bdev_get_queue(ca->bdev);
			ret |= bdi_congested(&q->backing_dev_info, bits);
		}

		cached_dev_put(dc);
	}

	return ret;
}

void bch_cached_dev_request_init(struct cached_dev *dc)
{
	struct gendisk *g = dc->disk.disk;

	g->queue->make_request_fn		= cached_dev_make_request;
	g->queue->backing_dev_info.congested_fn = cached_dev_congested;
	dc->disk.cache_miss			= cached_dev_cache_miss;
	dc->disk.ioctl				= cached_dev_ioctl;
}

/* Flash backed devices */

static int flash_dev_cache_miss(struct btree *b, struct search *s,
				struct bio *bio, unsigned sectors)
{
<<<<<<< HEAD
	/* Zero fill bio */

	while (bio->bi_idx != bio->bi_vcnt) {
		struct bio_vec *bv = bio_iovec(bio);
		unsigned j = min(bv->bv_len >> 9, sectors);

		void *p = kmap(bv->bv_page);
		memset(p + bv->bv_offset, 0, j << 9);
		kunmap(bv->bv_page);

		bv->bv_len	-= j << 9;
		bv->bv_offset	+= j << 9;

		if (bv->bv_len)
			return 0;

		bio->bi_sector	+= j;
		bio->bi_size	-= j << 9;

		bio->bi_idx++;
		sectors		-= j;
	}

	s->op.lookup_done = true;

	return 0;
}

static void flash_dev_make_request(struct request_queue *q, struct bio *bio)
=======
	unsigned bytes = min(sectors, bio_sectors(bio)) << 9;

	swap(bio->bi_iter.bi_size, bytes);
	zero_fill_bio(bio);
	swap(bio->bi_iter.bi_size, bytes);

	bio_advance(bio, bytes);

	if (!bio->bi_iter.bi_size)
		return MAP_DONE;

	return MAP_CONTINUE;
}

static void flash_dev_nodata(struct closure *cl)
{
	struct search *s = container_of(cl, struct search, cl);

	if (s->iop.flush_journal)
		bch_journal_meta(s->iop.c, cl);

	continue_at(cl, search_free, NULL);
}

static blk_qc_t flash_dev_make_request(struct request_queue *q,
					     struct bio *bio)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct search *s;
	struct closure *cl;
	struct bcache_device *d = bio->bi_bdev->bd_disk->private_data;
<<<<<<< HEAD
	int cpu, rw = bio_data_dir(bio);

	cpu = part_stat_lock();
	part_stat_inc(cpu, &d->disk->part0, ios[rw]);
	part_stat_add(cpu, &d->disk->part0, sectors[rw], bio_sectors(bio));
	part_stat_unlock();
=======
	int rw = bio_data_dir(bio);

	generic_start_io_acct(rw, bio_sectors(bio), &d->disk->part0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	s = search_alloc(bio, d);
	cl = &s->cl;
	bio = &s->bio.bio;

<<<<<<< HEAD
	trace_bcache_request_start(s, bio);

	if (bio_has_data(bio) && !rw) {
		closure_call(&s->op.cl, btree_read_async, NULL, cl);
	} else if (bio_has_data(bio) || s->op.skip) {
		bch_keybuf_check_overlapping(&s->op.c->moving_gc_keys,
					     &KEY(d->id, bio->bi_sector, 0),
					     &KEY(d->id, bio_end(bio), 0));

		s->writeback	= true;
		s->op.cache_bio	= bio;

		closure_call(&s->op.cl, bch_insert_data, NULL, cl);
	} else {
		/* No data - probably a cache flush */
		if (s->op.flush_journal)
			bch_journal_meta(s->op.c, cl);
	}

	continue_at(cl, search_free, NULL);
=======
	trace_bcache_request_start(s->d, bio);

	if (!bio->bi_iter.bi_size) {
		/*
		 * can't call bch_journal_meta from under
		 * generic_make_request
		 */
		continue_at_nobarrier(&s->cl,
				      flash_dev_nodata,
				      bcache_wq);
		return BLK_QC_T_NONE;
	} else if (rw) {
		bch_keybuf_check_overlapping(&s->iop.c->moving_gc_keys,
					&KEY(d->id, bio->bi_iter.bi_sector, 0),
					&KEY(d->id, bio_end_sector(bio), 0));

		s->iop.bypass		= (bio_op(bio) == REQ_OP_DISCARD) != 0;
		s->iop.writeback	= true;
		s->iop.bio		= bio;

		closure_call(&s->iop.cl, bch_data_insert, NULL, cl);
	} else {
		closure_call(&s->iop.cl, cache_lookup, NULL, cl);
	}

	continue_at(cl, search_free, NULL);
	return BLK_QC_T_NONE;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int flash_dev_ioctl(struct bcache_device *d, fmode_t mode,
			   unsigned int cmd, unsigned long arg)
{
	return -ENOTTY;
}

static int flash_dev_congested(void *data, int bits)
{
	struct bcache_device *d = data;
	struct request_queue *q;
	struct cache *ca;
	unsigned i;
	int ret = 0;

	for_each_cache(ca, d->c, i) {
		q = bdev_get_queue(ca->bdev);
		ret |= bdi_congested(&q->backing_dev_info, bits);
	}

	return ret;
}

void bch_flash_dev_request_init(struct bcache_device *d)
{
	struct gendisk *g = d->disk;

	g->queue->make_request_fn		= flash_dev_make_request;
	g->queue->backing_dev_info.congested_fn = flash_dev_congested;
	d->cache_miss				= flash_dev_cache_miss;
	d->ioctl				= flash_dev_ioctl;
}

void bch_request_exit(void)
{
<<<<<<< HEAD
#ifdef CONFIG_CGROUP_BCACHE
	cgroup_unload_subsys(&bcache_subsys);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (bch_search_cache)
		kmem_cache_destroy(bch_search_cache);
}

int __init bch_request_init(void)
{
	bch_search_cache = KMEM_CACHE(search, 0);
	if (!bch_search_cache)
		return -ENOMEM;

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_BCACHE
	cgroup_load_subsys(&bcache_subsys);
	init_bch_cgroup(&bcache_default_cgroup);

	cgroup_add_cftypes(&bcache_subsys, bch_files);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}
