/*
 * Moving/copying garbage collector
 *
 * Copyright 2012 Google, Inc.
 */

#include "bcache.h"
#include "btree.h"
#include "debug.h"
#include "request.h"

<<<<<<< HEAD
struct moving_io {
	struct keybuf_key	*w;
	struct search		s;
=======
#include <trace/events/bcache.h>

struct moving_io {
	struct closure		cl;
	struct keybuf_key	*w;
	struct data_insert_op	op;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct bbio		bio;
};

static bool moving_pred(struct keybuf *buf, struct bkey *k)
{
	struct cache_set *c = container_of(buf, struct cache_set,
					   moving_gc_keys);
	unsigned i;

<<<<<<< HEAD
	for (i = 0; i < KEY_PTRS(k); i++) {
		struct cache *ca = PTR_CACHE(c, k, i);
		struct bucket *g = PTR_BUCKET(c, k, i);

		if (GC_SECTORS_USED(g) < ca->gc_move_threshold)
			return true;
	}
=======
	for (i = 0; i < KEY_PTRS(k); i++)
		if (ptr_available(c, k, i) &&
		    GC_MOVE(PTR_BUCKET(c, k, i)))
			return true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return false;
}

/* Moving GC - IO loop */

static void moving_io_destructor(struct closure *cl)
{
<<<<<<< HEAD
	struct moving_io *io = container_of(cl, struct moving_io, s.cl);
=======
	struct moving_io *io = container_of(cl, struct moving_io, cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(io);
}

static void write_moving_finish(struct closure *cl)
{
<<<<<<< HEAD
	struct moving_io *io = container_of(cl, struct moving_io, s.cl);
	struct bio *bio = &io->bio.bio;
	struct bio_vec *bv = bio_iovec_idx(bio, bio->bi_vcnt);

	while (bv-- != bio->bi_io_vec)
		__free_page(bv->bv_page);

	pr_debug("%s %s", io->s.op.insert_collision
		 ? "collision moving" : "moved",
		 pkey(&io->w->key));

	bch_keybuf_del(&io->s.op.c->moving_gc_keys, io->w);

	atomic_dec_bug(&io->s.op.c->in_flight);
	closure_wake_up(&io->s.op.c->moving_gc_wait);
=======
	struct moving_io *io = container_of(cl, struct moving_io, cl);
	struct bio *bio = &io->bio.bio;

	bio_free_pages(bio);

	if (io->op.replace_collision)
		trace_bcache_gc_copy_collision(&io->w->key);

	bch_keybuf_del(&io->op.c->moving_gc_keys, io->w);

	up(&io->op.c->moving_in_flight);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	closure_return_with_destructor(cl, moving_io_destructor);
}

<<<<<<< HEAD
static void read_moving_endio(struct bio *bio, int error)
{
	struct moving_io *io = container_of(bio->bi_private,
					    struct moving_io, s.cl);

	if (error)
		io->s.error = error;

	bch_bbio_endio(io->s.op.c, bio, error, "reading data to move");
=======
static void read_moving_endio(struct bio *bio)
{
	struct bbio *b = container_of(bio, struct bbio, bio);
	struct moving_io *io = container_of(bio->bi_private,
					    struct moving_io, cl);

	if (bio->bi_error)
		io->op.error = bio->bi_error;
	else if (!KEY_DIRTY(&b->key) &&
		 ptr_stale(io->op.c, &b->key, 0)) {
		io->op.error = -EINTR;
	}

	bch_bbio_endio(io->op.c, bio, bio->bi_error, "reading data to move");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void moving_init(struct moving_io *io)
{
	struct bio *bio = &io->bio.bio;

	bio_init(bio);
	bio_get(bio);
	bio_set_prio(bio, IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE, 0));

<<<<<<< HEAD
	bio->bi_size		= KEY_SIZE(&io->w->key) << 9;
	bio->bi_max_vecs	= DIV_ROUND_UP(KEY_SIZE(&io->w->key),
					       PAGE_SECTORS);
	bio->bi_private		= &io->s.cl;
=======
	bio->bi_iter.bi_size	= KEY_SIZE(&io->w->key) << 9;
	bio->bi_max_vecs	= DIV_ROUND_UP(KEY_SIZE(&io->w->key),
					       PAGE_SECTORS);
	bio->bi_private		= &io->cl;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	bio->bi_io_vec		= bio->bi_inline_vecs;
	bch_bio_map(bio, NULL);
}

static void write_moving(struct closure *cl)
{
<<<<<<< HEAD
	struct search *s = container_of(cl, struct search, cl);
	struct moving_io *io = container_of(s, struct moving_io, s);

	if (!s->error) {
		trace_bcache_write_moving(&io->bio.bio);

		moving_init(io);

		io->bio.bio.bi_sector	= KEY_START(&io->w->key);
		s->op.lock		= -1;
		s->op.write_prio	= 1;
		s->op.cache_bio		= &io->bio.bio;

		s->writeback		= KEY_DIRTY(&io->w->key);
		s->op.csum		= KEY_CSUM(&io->w->key);

		s->op.type = BTREE_REPLACE;
		bkey_copy(&s->op.replace, &io->w->key);

		closure_init(&s->op.cl, cl);
		bch_insert_data(&s->op.cl);
	}

	continue_at(cl, write_moving_finish, NULL);
=======
	struct moving_io *io = container_of(cl, struct moving_io, cl);
	struct data_insert_op *op = &io->op;

	if (!op->error) {
		moving_init(io);

		io->bio.bio.bi_iter.bi_sector = KEY_START(&io->w->key);
		op->write_prio		= 1;
		op->bio			= &io->bio.bio;

		op->writeback		= KEY_DIRTY(&io->w->key);
		op->csum		= KEY_CSUM(&io->w->key);

		bkey_copy(&op->replace_key, &io->w->key);
		op->replace		= true;

		closure_call(&op->cl, bch_data_insert, NULL, cl);
	}

	continue_at(cl, write_moving_finish, op->wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void read_moving_submit(struct closure *cl)
{
<<<<<<< HEAD
	struct search *s = container_of(cl, struct search, cl);
	struct moving_io *io = container_of(s, struct moving_io, s);
	struct bio *bio = &io->bio.bio;

	trace_bcache_read_moving(bio);
	bch_submit_bbio(bio, s->op.c, &io->w->key, 0);

	continue_at(cl, write_moving, bch_gc_wq);
}

static void read_moving(struct closure *cl)
{
	struct cache_set *c = container_of(cl, struct cache_set, moving_gc);
	struct keybuf_key *w;
	struct moving_io *io;
	struct bio *bio;
=======
	struct moving_io *io = container_of(cl, struct moving_io, cl);
	struct bio *bio = &io->bio.bio;

	bch_submit_bbio(bio, io->op.c, &io->w->key, 0);

	continue_at(cl, write_moving, io->op.wq);
}

static void read_moving(struct cache_set *c)
{
	struct keybuf_key *w;
	struct moving_io *io;
	struct bio *bio;
	struct closure cl;

	closure_init_stack(&cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* XXX: if we error, background writeback could stall indefinitely */

	while (!test_bit(CACHE_SET_STOPPING, &c->flags)) {
<<<<<<< HEAD
		w = bch_keybuf_next_rescan(c, &c->moving_gc_keys, &MAX_KEY);
		if (!w)
			break;

=======
		w = bch_keybuf_next_rescan(c, &c->moving_gc_keys,
					   &MAX_KEY, moving_pred);
		if (!w)
			break;

		if (ptr_stale(c, &w->key, 0)) {
			bch_keybuf_del(&c->moving_gc_keys, w);
			continue;
		}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		io = kzalloc(sizeof(struct moving_io) + sizeof(struct bio_vec)
			     * DIV_ROUND_UP(KEY_SIZE(&w->key), PAGE_SECTORS),
			     GFP_KERNEL);
		if (!io)
			goto err;

		w->private	= io;
		io->w		= w;
<<<<<<< HEAD
		io->s.op.inode	= KEY_INODE(&w->key);
		io->s.op.c	= c;
=======
		io->op.inode	= KEY_INODE(&w->key);
		io->op.c	= c;
		io->op.wq	= c->moving_gc_wq;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		moving_init(io);
		bio = &io->bio.bio;

<<<<<<< HEAD
		bio->bi_rw	= READ;
		bio->bi_end_io	= read_moving_endio;

		if (bch_bio_alloc_pages(bio, GFP_KERNEL))
			goto err;

		pr_debug("%s", pkey(&w->key));

		closure_call(&io->s.cl, read_moving_submit, NULL, &c->gc.cl);

		if (atomic_inc_return(&c->in_flight) >= 64) {
			closure_wait_event(&c->moving_gc_wait, cl,
					   atomic_read(&c->in_flight) < 64);
			continue_at(cl, read_moving, bch_gc_wq);
		}
=======
		bio_set_op_attrs(bio, REQ_OP_READ, 0);
		bio->bi_end_io	= read_moving_endio;

		if (bio_alloc_pages(bio, GFP_KERNEL))
			goto err;

		trace_bcache_gc_copy(&w->key);

		down(&c->moving_in_flight);
		closure_call(&io->cl, read_moving_submit, NULL, &cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (0) {
err:		if (!IS_ERR_OR_NULL(w->private))
			kfree(w->private);

		bch_keybuf_del(&c->moving_gc_keys, w);
	}

<<<<<<< HEAD
	closure_return(cl);
=======
	closure_sync(&cl);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static bool bucket_cmp(struct bucket *l, struct bucket *r)
{
	return GC_SECTORS_USED(l) < GC_SECTORS_USED(r);
}

static unsigned bucket_heap_top(struct cache *ca)
{
<<<<<<< HEAD
	return GC_SECTORS_USED(heap_peek(&ca->heap));
}

void bch_moving_gc(struct closure *cl)
{
	struct cache_set *c = container_of(cl, struct cache_set, gc.cl);
=======
	struct bucket *b;
	return (b = heap_peek(&ca->heap)) ? GC_SECTORS_USED(b) : 0;
}

void bch_moving_gc(struct cache_set *c)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct cache *ca;
	struct bucket *b;
	unsigned i;

	if (!c->copy_gc_enabled)
<<<<<<< HEAD
		closure_return(cl);
=======
		return;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&c->bucket_lock);

	for_each_cache(ca, c, i) {
		unsigned sectors_to_move = 0;
		unsigned reserve_sectors = ca->sb.bucket_size *
<<<<<<< HEAD
			min(fifo_used(&ca->free), ca->free.size / 2);
=======
			fifo_used(&ca->free[RESERVE_MOVINGGC]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		ca->heap.used = 0;

		for_each_bucket(b, ca) {
<<<<<<< HEAD
			if (!GC_SECTORS_USED(b))
=======
			if (GC_MARK(b) == GC_MARK_METADATA ||
			    !GC_SECTORS_USED(b) ||
			    GC_SECTORS_USED(b) == ca->sb.bucket_size ||
			    atomic_read(&b->pin))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				continue;

			if (!heap_full(&ca->heap)) {
				sectors_to_move += GC_SECTORS_USED(b);
				heap_add(&ca->heap, b, bucket_cmp);
			} else if (bucket_cmp(b, heap_peek(&ca->heap))) {
				sectors_to_move -= bucket_heap_top(ca);
				sectors_to_move += GC_SECTORS_USED(b);

				ca->heap.data[0] = b;
				heap_sift(&ca->heap, 0, bucket_cmp);
			}
		}

		while (sectors_to_move > reserve_sectors) {
			heap_pop(&ca->heap, b, bucket_cmp);
			sectors_to_move -= GC_SECTORS_USED(b);
		}

<<<<<<< HEAD
		ca->gc_move_threshold = bucket_heap_top(ca);

		pr_debug("threshold %u", ca->gc_move_threshold);
=======
		while (heap_pop(&ca->heap, b, bucket_cmp))
			SET_GC_MOVE(b, 1);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	mutex_unlock(&c->bucket_lock);

	c->moving_gc_keys.last_scanned = ZERO_KEY;

<<<<<<< HEAD
	closure_init(&c->moving_gc, cl);
	read_moving(&c->moving_gc);

	closure_return(cl);
=======
	read_moving(c);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void bch_moving_init_cache_set(struct cache_set *c)
{
<<<<<<< HEAD
	bch_keybuf_init(&c->moving_gc_keys, moving_pred);
=======
	bch_keybuf_init(&c->moving_gc_keys);
	sema_init(&c->moving_in_flight, 64);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
