/*
 * Copyright (C) 2011 STRATO AG
 * written by Arne Jansen <sensille@gmx.net>
 * Distributed under the GNU GPL license version 2.
 *
 */

#ifndef __ULIST__
#define __ULIST__

#include <linux/list.h>
#include <linux/rbtree.h>

/*
 * ulist is a generic data structure to hold a collection of unique u64
 * values. The only operations it supports is adding to the list and
 * enumerating it.
 * It is possible to store an auxiliary value along with the key.
 *
<<<<<<< HEAD
 * The implementation is preliminary and can probably be sped up
 * significantly. A first step would be to store the values in an rbtree
 * as soon as ULIST_SIZE is exceeded.
 */

/*
 * number of elements statically allocated inside struct ulist
 */
#define ULIST_SIZE 16

struct ulist_iterator {
	int i;
=======
 */
struct ulist_iterator {
#ifdef CONFIG_BTRFS_DEBUG
	int i;
#endif
	struct list_head *cur_list;  /* hint to start search */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * element of the list
 */
struct ulist_node {
	u64 val;		/* value to store */
	u64 aux;		/* auxiliary value saved along with the val */
<<<<<<< HEAD
=======

#ifdef CONFIG_BTRFS_DEBUG
	int seqnum;		/* sequence number this node is added */
#endif

	struct list_head list;  /* used to link node */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct rb_node rb_node;	/* used to speed up search */
};

struct ulist {
	/*
	 * number of elements stored in list
	 */
	unsigned long nnodes;

<<<<<<< HEAD
	/*
	 * number of nodes we already have room for
	 */
	unsigned long nodes_alloced;

	/*
	 * pointer to the array storing the elements. The first ULIST_SIZE
	 * elements are stored inline. In this case the it points to int_nodes.
	 * After exceeding ULIST_SIZE, dynamic memory is allocated.
	 */
	struct ulist_node *nodes;

	struct rb_root root;

	/*
	 * inline storage space for the first ULIST_SIZE entries
	 */
	struct ulist_node int_nodes[ULIST_SIZE];
};

void ulist_init(struct ulist *ulist);
void ulist_fini(struct ulist *ulist);
=======
	struct list_head nodes;
	struct rb_root root;
};

void ulist_init(struct ulist *ulist);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void ulist_reinit(struct ulist *ulist);
struct ulist *ulist_alloc(gfp_t gfp_mask);
void ulist_free(struct ulist *ulist);
int ulist_add(struct ulist *ulist, u64 val, u64 aux, gfp_t gfp_mask);
int ulist_add_merge(struct ulist *ulist, u64 val, u64 aux,
		    u64 *old_aux, gfp_t gfp_mask);
<<<<<<< HEAD
struct ulist_node *ulist_next(struct ulist *ulist,
			      struct ulist_iterator *uiter);

#define ULIST_ITER_INIT(uiter) ((uiter)->i = 0)
=======
int ulist_del(struct ulist *ulist, u64 val, u64 aux);

/* just like ulist_add_merge() but take a pointer for the aux data */
static inline int ulist_add_merge_ptr(struct ulist *ulist, u64 val, void *aux,
				      void **old_aux, gfp_t gfp_mask)
{
#if BITS_PER_LONG == 32
	u64 old64 = (uintptr_t)*old_aux;
	int ret = ulist_add_merge(ulist, val, (uintptr_t)aux, &old64, gfp_mask);
	*old_aux = (void *)((uintptr_t)old64);
	return ret;
#else
	return ulist_add_merge(ulist, val, (u64)aux, (u64 *)old_aux, gfp_mask);
#endif
}

struct ulist_node *ulist_next(struct ulist *ulist,
			      struct ulist_iterator *uiter);

#define ULIST_ITER_INIT(uiter) ((uiter)->cur_list = NULL)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
