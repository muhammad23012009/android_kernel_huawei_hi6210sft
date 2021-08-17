/*
 * Copyright (C) 2011 STRATO AG
 * written by Arne Jansen <sensille@gmx.net>
 * Distributed under the GNU GPL license version 2.
 */

#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/export.h>
#include "ulist.h"
=======
#include "ulist.h"
#include "ctree.h"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

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
 *
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * A sample usage for ulists is the enumeration of directed graphs without
 * visiting a node twice. The pseudo-code could look like this:
 *
 * ulist = ulist_alloc();
 * ulist_add(ulist, root);
 * ULIST_ITER_INIT(&uiter);
 *
 * while ((elem = ulist_next(ulist, &uiter)) {
 * 	for (all child nodes n in elem)
 *		ulist_add(ulist, n);
 *	do something useful with the node;
 * }
 * ulist_free(ulist);
 *
<<<<<<< HEAD
 * This assumes the graph nodes are adressable by u64. This stems from the
=======
 * This assumes the graph nodes are addressable by u64. This stems from the
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * usage for tree enumeration in btrfs, where the logical addresses are
 * 64 bit.
 *
 * It is also useful for tree enumeration which could be done elegantly
 * recursively, but is not possible due to kernel stack limitations. The
 * loop would be similar to the above.
 */

/**
 * ulist_init - freshly initialize a ulist
 * @ulist:	the ulist to initialize
 *
 * Note: don't use this function to init an already used ulist, use
 * ulist_reinit instead.
 */
void ulist_init(struct ulist *ulist)
{
<<<<<<< HEAD
	ulist->nnodes = 0;
	ulist->nodes = ulist->int_nodes;
	ulist->nodes_alloced = ULIST_SIZE;
	ulist->root = RB_ROOT;
}
EXPORT_SYMBOL(ulist_init);
=======
	INIT_LIST_HEAD(&ulist->nodes);
	ulist->root = RB_ROOT;
	ulist->nnodes = 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * ulist_fini - free up additionally allocated memory for the ulist
 * @ulist:	the ulist from which to free the additional memory
 *
 * This is useful in cases where the base 'struct ulist' has been statically
 * allocated.
 */
<<<<<<< HEAD
void ulist_fini(struct ulist *ulist)
{
	/*
	 * The first ULIST_SIZE elements are stored inline in struct ulist.
	 * Only if more elements are alocated they need to be freed.
	 */
	if (ulist->nodes_alloced > ULIST_SIZE)
		kfree(ulist->nodes);
	ulist->nodes_alloced = 0;	/* in case ulist_fini is called twice */
	ulist->root = RB_ROOT;
}
EXPORT_SYMBOL(ulist_fini);
=======
static void ulist_fini(struct ulist *ulist)
{
	struct ulist_node *node;
	struct ulist_node *next;

	list_for_each_entry_safe(node, next, &ulist->nodes, list) {
		kfree(node);
	}
	ulist->root = RB_ROOT;
	INIT_LIST_HEAD(&ulist->nodes);
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * ulist_reinit - prepare a ulist for reuse
 * @ulist:	ulist to be reused
 *
 * Free up all additional memory allocated for the list elements and reinit
 * the ulist.
 */
void ulist_reinit(struct ulist *ulist)
{
	ulist_fini(ulist);
	ulist_init(ulist);
}
<<<<<<< HEAD
EXPORT_SYMBOL(ulist_reinit);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * ulist_alloc - dynamically allocate a ulist
 * @gfp_mask:	allocation flags to for base allocation
 *
 * The allocated ulist will be returned in an initialized state.
 */
struct ulist *ulist_alloc(gfp_t gfp_mask)
{
	struct ulist *ulist = kmalloc(sizeof(*ulist), gfp_mask);

	if (!ulist)
		return NULL;

	ulist_init(ulist);

	return ulist;
}
<<<<<<< HEAD
EXPORT_SYMBOL(ulist_alloc);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * ulist_free - free dynamically allocated ulist
 * @ulist:	ulist to free
 *
 * It is not necessary to call ulist_fini before.
 */
void ulist_free(struct ulist *ulist)
{
	if (!ulist)
		return;
	ulist_fini(ulist);
	kfree(ulist);
}
<<<<<<< HEAD
EXPORT_SYMBOL(ulist_free);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static struct ulist_node *ulist_rbtree_search(struct ulist *ulist, u64 val)
{
	struct rb_node *n = ulist->root.rb_node;
	struct ulist_node *u = NULL;

	while (n) {
		u = rb_entry(n, struct ulist_node, rb_node);
		if (u->val < val)
			n = n->rb_right;
		else if (u->val > val)
			n = n->rb_left;
		else
			return u;
	}
	return NULL;
}

<<<<<<< HEAD
=======
static void ulist_rbtree_erase(struct ulist *ulist, struct ulist_node *node)
{
	rb_erase(&node->rb_node, &ulist->root);
	list_del(&node->list);
	kfree(node);
	BUG_ON(ulist->nnodes == 0);
	ulist->nnodes--;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int ulist_rbtree_insert(struct ulist *ulist, struct ulist_node *ins)
{
	struct rb_node **p = &ulist->root.rb_node;
	struct rb_node *parent = NULL;
	struct ulist_node *cur = NULL;

	while (*p) {
		parent = *p;
		cur = rb_entry(parent, struct ulist_node, rb_node);

		if (cur->val < ins->val)
			p = &(*p)->rb_right;
		else if (cur->val > ins->val)
			p = &(*p)->rb_left;
		else
			return -EEXIST;
	}
	rb_link_node(&ins->rb_node, parent, p);
	rb_insert_color(&ins->rb_node, &ulist->root);
	return 0;
}

/**
 * ulist_add - add an element to the ulist
 * @ulist:	ulist to add the element to
 * @val:	value to add to ulist
 * @aux:	auxiliary value to store along with val
 * @gfp_mask:	flags to use for allocation
 *
 * Note: locking must be provided by the caller. In case of rwlocks write
 *       locking is needed
 *
 * Add an element to a ulist. The @val will only be added if it doesn't
 * already exist. If it is added, the auxiliary value @aux is stored along with
 * it. In case @val already exists in the ulist, @aux is ignored, even if
 * it differs from the already stored value.
 *
 * ulist_add returns 0 if @val already exists in ulist and 1 if @val has been
 * inserted.
 * In case of allocation failure -ENOMEM is returned and the ulist stays
 * unaltered.
 */
int ulist_add(struct ulist *ulist, u64 val, u64 aux, gfp_t gfp_mask)
{
	return ulist_add_merge(ulist, val, aux, NULL, gfp_mask);
}

int ulist_add_merge(struct ulist *ulist, u64 val, u64 aux,
		    u64 *old_aux, gfp_t gfp_mask)
{
<<<<<<< HEAD
	int ret = 0;
	struct ulist_node *node = NULL;
=======
	int ret;
	struct ulist_node *node;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	node = ulist_rbtree_search(ulist, val);
	if (node) {
		if (old_aux)
			*old_aux = node->aux;
		return 0;
	}
<<<<<<< HEAD

	if (ulist->nnodes >= ulist->nodes_alloced) {
		u64 new_alloced = ulist->nodes_alloced + 128;
		struct ulist_node *new_nodes;
		void *old = NULL;
		int i;

		for (i = 0; i < ulist->nnodes; i++)
			rb_erase(&ulist->nodes[i].rb_node, &ulist->root);

		/*
		 * if nodes_alloced == ULIST_SIZE no memory has been allocated
		 * yet, so pass NULL to krealloc
		 */
		if (ulist->nodes_alloced > ULIST_SIZE)
			old = ulist->nodes;

		new_nodes = krealloc(old, sizeof(*new_nodes) * new_alloced,
				     gfp_mask);
		if (!new_nodes)
			return -ENOMEM;

		if (!old)
			memcpy(new_nodes, ulist->int_nodes,
			       sizeof(ulist->int_nodes));

		ulist->nodes = new_nodes;
		ulist->nodes_alloced = new_alloced;

		/*
		 * krealloc actually uses memcpy, which does not copy rb_node
		 * pointers, so we have to do it ourselves.  Otherwise we may
		 * be bitten by crashes.
		 */
		for (i = 0; i < ulist->nnodes; i++) {
			ret = ulist_rbtree_insert(ulist, &ulist->nodes[i]);
			if (ret < 0)
				return ret;
		}
	}
	ulist->nodes[ulist->nnodes].val = val;
	ulist->nodes[ulist->nnodes].aux = aux;
	ret = ulist_rbtree_insert(ulist, &ulist->nodes[ulist->nnodes]);
	BUG_ON(ret);
	++ulist->nnodes;

	return 1;
}
EXPORT_SYMBOL(ulist_add);
=======
	node = kmalloc(sizeof(*node), gfp_mask);
	if (!node)
		return -ENOMEM;

	node->val = val;
	node->aux = aux;

	ret = ulist_rbtree_insert(ulist, node);
	ASSERT(!ret);
	list_add_tail(&node->list, &ulist->nodes);
	ulist->nnodes++;

	return 1;
}

/*
 * ulist_del - delete one node from ulist
 * @ulist:	ulist to remove node from
 * @val:	value to delete
 * @aux:	aux to delete
 *
 * The deletion will only be done when *BOTH* val and aux matches.
 * Return 0 for successful delete.
 * Return > 0 for not found.
 */
int ulist_del(struct ulist *ulist, u64 val, u64 aux)
{
	struct ulist_node *node;

	node = ulist_rbtree_search(ulist, val);
	/* Not found */
	if (!node)
		return 1;

	if (node->aux != aux)
		return 1;

	/* Found and delete */
	ulist_rbtree_erase(ulist, node);
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * ulist_next - iterate ulist
 * @ulist:	ulist to iterate
 * @uiter:	iterator variable, initialized with ULIST_ITER_INIT(&iterator)
 *
 * Note: locking must be provided by the caller. In case of rwlocks only read
 *       locking is needed
 *
 * This function is used to iterate an ulist.
 * It returns the next element from the ulist or %NULL when the
 * end is reached. No guarantee is made with respect to the order in which
 * the elements are returned. They might neither be returned in order of
 * addition nor in ascending order.
 * It is allowed to call ulist_add during an enumeration. Newly added items
 * are guaranteed to show up in the running enumeration.
 */
struct ulist_node *ulist_next(struct ulist *ulist, struct ulist_iterator *uiter)
{
<<<<<<< HEAD
	if (ulist->nnodes == 0)
		return NULL;
	if (uiter->i < 0 || uiter->i >= ulist->nnodes)
		return NULL;

	return &ulist->nodes[uiter->i++];
}
EXPORT_SYMBOL(ulist_next);
=======
	struct ulist_node *node;

	if (list_empty(&ulist->nodes))
		return NULL;
	if (uiter->cur_list && uiter->cur_list->next == &ulist->nodes)
		return NULL;
	if (uiter->cur_list) {
		uiter->cur_list = uiter->cur_list->next;
	} else {
		uiter->cur_list = ulist->nodes.next;
	}
	node = list_entry(uiter->cur_list, struct ulist_node, list);
	return node;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
