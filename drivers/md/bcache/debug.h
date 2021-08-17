#ifndef _BCACHE_DEBUG_H
#define _BCACHE_DEBUG_H

<<<<<<< HEAD
/* Btree/bkey debug printing */

#define KEYHACK_SIZE 80
struct keyprint_hack {
	char s[KEYHACK_SIZE];
};

struct keyprint_hack bch_pkey(const struct bkey *k);
struct keyprint_hack bch_pbtree(const struct btree *b);
#define pkey(k)		(&bch_pkey(k).s[0])
#define pbtree(b)	(&bch_pbtree(b).s[0])

#ifdef CONFIG_BCACHE_EDEBUG

unsigned bch_count_data(struct btree *);
void bch_check_key_order_msg(struct btree *, struct bset *, const char *, ...);
void bch_check_keys(struct btree *, const char *, ...);

#define bch_check_key_order(b, i)			\
	bch_check_key_order_msg(b, i, "keys out of order")
#define EBUG_ON(cond)		BUG_ON(cond)

#else /* EDEBUG */

#define bch_count_data(b)				0
#define bch_check_key_order(b, i)			do {} while (0)
#define bch_check_key_order_msg(b, i, ...)		do {} while (0)
#define bch_check_keys(b, ...)				do {} while (0)
#define EBUG_ON(cond)					do {} while (0)

#endif

#ifdef CONFIG_BCACHE_DEBUG

void bch_btree_verify(struct btree *, struct bset *);
void bch_data_verify(struct search *);

#else /* DEBUG */

static inline void bch_btree_verify(struct btree *b, struct bset *i) {}
static inline void bch_data_verify(struct search *s) {};
=======
struct bio;
struct cached_dev;
struct cache_set;

#ifdef CONFIG_BCACHE_DEBUG

void bch_btree_verify(struct btree *);
void bch_data_verify(struct cached_dev *, struct bio *);

#define expensive_debug_checks(c)	((c)->expensive_debug_checks)
#define key_merging_disabled(c)		((c)->key_merging_disabled)
#define bypass_torture_test(d)		((d)->bypass_torture_test)

#else /* DEBUG */

static inline void bch_btree_verify(struct btree *b) {}
static inline void bch_data_verify(struct cached_dev *dc, struct bio *bio) {}

#define expensive_debug_checks(c)	0
#define key_merging_disabled(c)		0
#define bypass_torture_test(d)		0
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif

#ifdef CONFIG_DEBUG_FS
void bch_debug_init_cache_set(struct cache_set *);
#else
static inline void bch_debug_init_cache_set(struct cache_set *c) {}
#endif

#endif
