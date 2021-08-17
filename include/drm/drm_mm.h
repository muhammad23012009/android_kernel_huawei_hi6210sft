/**************************************************************************
 *
 * Copyright 2006-2008 Tungsten Graphics, Inc., Cedar Park, TX. USA.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 **************************************************************************/
/*
 * Authors:
 * Thomas Hellstrom <thomas-at-tungstengraphics-dot-com>
 */

#ifndef _DRM_MM_H_
#define _DRM_MM_H_

/*
 * Generic range manager structs
 */
<<<<<<< HEAD
#include <linux/list.h>
=======
#include <linux/bug.h>
#include <linux/rbtree.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_DEBUG_FS
#include <linux/seq_file.h>
#endif

<<<<<<< HEAD
struct drm_mm_node {
	struct list_head node_list;
	struct list_head hole_stack;
=======
enum drm_mm_search_flags {
	DRM_MM_SEARCH_DEFAULT =		0,
	DRM_MM_SEARCH_BEST =		1 << 0,
	DRM_MM_SEARCH_BELOW =		1 << 1,
};

enum drm_mm_allocator_flags {
	DRM_MM_CREATE_DEFAULT =		0,
	DRM_MM_CREATE_TOP =		1 << 0,
};

#define DRM_MM_BOTTOMUP DRM_MM_SEARCH_DEFAULT, DRM_MM_CREATE_DEFAULT
#define DRM_MM_TOPDOWN DRM_MM_SEARCH_BELOW, DRM_MM_CREATE_TOP

struct drm_mm_node {
	struct list_head node_list;
	struct list_head hole_stack;
	struct rb_node rb;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned hole_follows : 1;
	unsigned scanned_block : 1;
	unsigned scanned_prev_free : 1;
	unsigned scanned_next_free : 1;
	unsigned scanned_preceeds_hole : 1;
	unsigned allocated : 1;
	unsigned long color;
<<<<<<< HEAD
	unsigned long start;
	unsigned long size;
=======
	u64 start;
	u64 size;
	u64 __subtree_last;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct drm_mm *mm;
};

struct drm_mm {
	/* List of all memory nodes that immediately precede a free hole. */
	struct list_head hole_stack;
	/* head_node.node_list is the list of all memory nodes, ordered
	 * according to the (increasing) start address of the memory node. */
	struct drm_mm_node head_node;
<<<<<<< HEAD
	struct list_head unused_nodes;
	int num_unused;
	spinlock_t unused_lock;
	unsigned int scan_check_range : 1;
	unsigned scan_alignment;
	unsigned long scan_color;
	unsigned long scan_size;
	unsigned long scan_hit_start;
	unsigned long scan_hit_end;
	unsigned scanned_blocks;
	unsigned long scan_start;
	unsigned long scan_end;
	struct drm_mm_node *prev_scanned_node;

	void (*color_adjust)(struct drm_mm_node *node, unsigned long color,
			     unsigned long *start, unsigned long *end);
};

=======
	/* Keep an interval_tree for fast lookup of drm_mm_nodes by address. */
	struct rb_root interval_tree;

	unsigned int scan_check_range : 1;
	unsigned scan_alignment;
	unsigned long scan_color;
	u64 scan_size;
	u64 scan_hit_start;
	u64 scan_hit_end;
	unsigned scanned_blocks;
	u64 scan_start;
	u64 scan_end;
	struct drm_mm_node *prev_scanned_node;

	void (*color_adjust)(struct drm_mm_node *node, unsigned long color,
			     u64 *start, u64 *end);
};

/**
 * drm_mm_node_allocated - checks whether a node is allocated
 * @node: drm_mm_node to check
 *
 * Drivers should use this helpers for proper encapusulation of drm_mm
 * internals.
 *
 * Returns:
 * True if the @node is allocated.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline bool drm_mm_node_allocated(struct drm_mm_node *node)
{
	return node->allocated;
}

<<<<<<< HEAD
=======
/**
 * drm_mm_initialized - checks whether an allocator is initialized
 * @mm: drm_mm to check
 *
 * Drivers should use this helpers for proper encapusulation of drm_mm
 * internals.
 *
 * Returns:
 * True if the @mm is initialized.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline bool drm_mm_initialized(struct drm_mm *mm)
{
	return mm->hole_stack.next;
}

<<<<<<< HEAD
static inline unsigned long __drm_mm_hole_node_start(struct drm_mm_node *hole_node)
=======
static inline u64 __drm_mm_hole_node_start(struct drm_mm_node *hole_node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return hole_node->start + hole_node->size;
}

<<<<<<< HEAD
static inline unsigned long drm_mm_hole_node_start(struct drm_mm_node *hole_node)
=======
/**
 * drm_mm_hole_node_start - computes the start of the hole following @node
 * @hole_node: drm_mm_node which implicitly tracks the following hole
 *
 * This is useful for driver-sepific debug dumpers. Otherwise drivers should not
 * inspect holes themselves. Drivers must check first whether a hole indeed
 * follows by looking at node->hole_follows.
 *
 * Returns:
 * Start of the subsequent hole.
 */
static inline u64 drm_mm_hole_node_start(struct drm_mm_node *hole_node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	BUG_ON(!hole_node->hole_follows);
	return __drm_mm_hole_node_start(hole_node);
}

<<<<<<< HEAD
static inline unsigned long __drm_mm_hole_node_end(struct drm_mm_node *hole_node)
{
	return list_entry(hole_node->node_list.next,
			  struct drm_mm_node, node_list)->start;
}

static inline unsigned long drm_mm_hole_node_end(struct drm_mm_node *hole_node)
=======
static inline u64 __drm_mm_hole_node_end(struct drm_mm_node *hole_node)
{
	return list_next_entry(hole_node, node_list)->start;
}

/**
 * drm_mm_hole_node_end - computes the end of the hole following @node
 * @hole_node: drm_mm_node which implicitly tracks the following hole
 *
 * This is useful for driver-sepific debug dumpers. Otherwise drivers should not
 * inspect holes themselves. Drivers must check first whether a hole indeed
 * follows by looking at node->hole_follows.
 *
 * Returns:
 * End of the subsequent hole.
 */
static inline u64 drm_mm_hole_node_end(struct drm_mm_node *hole_node)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return __drm_mm_hole_node_end(hole_node);
}

<<<<<<< HEAD
#define drm_mm_for_each_node(entry, mm) list_for_each_entry(entry, \
						&(mm)->head_node.node_list, \
						node_list)
#define drm_mm_for_each_scanned_node_reverse(entry, n, mm) \
	for (entry = (mm)->prev_scanned_node, \
		next = entry ? list_entry(entry->node_list.next, \
			struct drm_mm_node, node_list) : NULL; \
	     entry != NULL; entry = next, \
		next = entry ? list_entry(entry->node_list.next, \
			struct drm_mm_node, node_list) : NULL) \

/* Note that we need to unroll list_for_each_entry in order to inline
 * setting hole_start and hole_end on each iteration and keep the
 * macro sane.
 */
#define drm_mm_for_each_hole(entry, mm, hole_start, hole_end) \
	for (entry = list_entry((mm)->hole_stack.next, struct drm_mm_node, hole_stack); \
=======
/**
 * drm_mm_for_each_node - iterator to walk over all allocated nodes
 * @entry: drm_mm_node structure to assign to in each iteration step
 * @mm: drm_mm allocator to walk
 *
 * This iterator walks over all nodes in the range allocator. It is implemented
 * with list_for_each, so not save against removal of elements.
 */
#define drm_mm_for_each_node(entry, mm) list_for_each_entry(entry, \
						&(mm)->head_node.node_list, \
						node_list)

#define __drm_mm_for_each_hole(entry, mm, hole_start, hole_end, backwards) \
	for (entry = list_entry((backwards) ? (mm)->hole_stack.prev : (mm)->hole_stack.next, struct drm_mm_node, hole_stack); \
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	     &entry->hole_stack != &(mm)->hole_stack ? \
	     hole_start = drm_mm_hole_node_start(entry), \
	     hole_end = drm_mm_hole_node_end(entry), \
	     1 : 0; \
<<<<<<< HEAD
	     entry = list_entry(entry->hole_stack.next, struct drm_mm_node, hole_stack))
=======
	     entry = list_entry((backwards) ? entry->hole_stack.prev : entry->hole_stack.next, struct drm_mm_node, hole_stack))

/**
 * drm_mm_for_each_hole - iterator to walk over all holes
 * @entry: drm_mm_node used internally to track progress
 * @mm: drm_mm allocator to walk
 * @hole_start: ulong variable to assign the hole start to on each iteration
 * @hole_end: ulong variable to assign the hole end to on each iteration
 *
 * This iterator walks over all holes in the range allocator. It is implemented
 * with list_for_each, so not save against removal of elements. @entry is used
 * internally and will not reflect a real drm_mm_node for the very first hole.
 * Hence users of this iterator may not access it.
 *
 * Implementation Note:
 * We need to inline list_for_each_entry in order to be able to set hole_start
 * and hole_end on each iteration while keeping the macro sane.
 *
 * The __drm_mm_for_each_hole version is similar, but with added support for
 * going backwards.
 */
#define drm_mm_for_each_hole(entry, mm, hole_start, hole_end) \
	__drm_mm_for_each_hole(entry, mm, hole_start, hole_end, 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Basic range manager support (drm_mm.c)
 */
<<<<<<< HEAD
extern struct drm_mm_node *drm_mm_create_block(struct drm_mm *mm,
					       unsigned long start,
					       unsigned long size,
					       bool atomic);
extern struct drm_mm_node *drm_mm_get_block_generic(struct drm_mm_node *node,
						    unsigned long size,
						    unsigned alignment,
						    unsigned long color,
						    int atomic);
extern struct drm_mm_node *drm_mm_get_block_range_generic(
						struct drm_mm_node *node,
						unsigned long size,
						unsigned alignment,
						unsigned long color,
						unsigned long start,
						unsigned long end,
						int atomic);
static inline struct drm_mm_node *drm_mm_get_block(struct drm_mm_node *parent,
						   unsigned long size,
						   unsigned alignment)
{
	return drm_mm_get_block_generic(parent, size, alignment, 0, 0);
}
static inline struct drm_mm_node *drm_mm_get_block_atomic(struct drm_mm_node *parent,
							  unsigned long size,
							  unsigned alignment)
{
	return drm_mm_get_block_generic(parent, size, alignment, 0, 1);
}
static inline struct drm_mm_node *drm_mm_get_block_range(
						struct drm_mm_node *parent,
						unsigned long size,
						unsigned alignment,
						unsigned long start,
						unsigned long end)
{
	return drm_mm_get_block_range_generic(parent, size, alignment, 0,
					      start, end, 0);
}
static inline struct drm_mm_node *drm_mm_get_color_block_range(
						struct drm_mm_node *parent,
						unsigned long size,
						unsigned alignment,
						unsigned long color,
						unsigned long start,
						unsigned long end)
{
	return drm_mm_get_block_range_generic(parent, size, alignment, color,
					      start, end, 0);
}
static inline struct drm_mm_node *drm_mm_get_block_atomic_range(
						struct drm_mm_node *parent,
						unsigned long size,
						unsigned alignment,
						unsigned long start,
						unsigned long end)
{
	return drm_mm_get_block_range_generic(parent, size, alignment, 0,
						start, end, 1);
}

extern int drm_mm_insert_node(struct drm_mm *mm,
			      struct drm_mm_node *node,
			      unsigned long size,
			      unsigned alignment);
extern int drm_mm_insert_node_in_range(struct drm_mm *mm,
				       struct drm_mm_node *node,
				       unsigned long size,
				       unsigned alignment,
				       unsigned long start,
				       unsigned long end);
extern int drm_mm_insert_node_generic(struct drm_mm *mm,
				      struct drm_mm_node *node,
				      unsigned long size,
				      unsigned alignment,
				      unsigned long color);
extern int drm_mm_insert_node_in_range_generic(struct drm_mm *mm,
				       struct drm_mm_node *node,
				       unsigned long size,
				       unsigned alignment,
				       unsigned long color,
				       unsigned long start,
				       unsigned long end);
extern void drm_mm_put_block(struct drm_mm_node *cur);
extern void drm_mm_remove_node(struct drm_mm_node *node);
extern void drm_mm_replace_node(struct drm_mm_node *old, struct drm_mm_node *new);
extern struct drm_mm_node *drm_mm_search_free_generic(const struct drm_mm *mm,
						      unsigned long size,
						      unsigned alignment,
						      unsigned long color,
						      bool best_match);
extern struct drm_mm_node *drm_mm_search_free_in_range_generic(
						const struct drm_mm *mm,
						unsigned long size,
						unsigned alignment,
						unsigned long color,
						unsigned long start,
						unsigned long end,
						bool best_match);
static inline struct drm_mm_node *drm_mm_search_free(const struct drm_mm *mm,
						     unsigned long size,
						     unsigned alignment,
						     bool best_match)
{
	return drm_mm_search_free_generic(mm,size, alignment, 0, best_match);
}
static inline  struct drm_mm_node *drm_mm_search_free_in_range(
						const struct drm_mm *mm,
						unsigned long size,
						unsigned alignment,
						unsigned long start,
						unsigned long end,
						bool best_match)
{
	return drm_mm_search_free_in_range_generic(mm, size, alignment, 0,
						   start, end, best_match);
}
static inline struct drm_mm_node *drm_mm_search_free_color(const struct drm_mm *mm,
							   unsigned long size,
							   unsigned alignment,
							   unsigned long color,
							   bool best_match)
{
	return drm_mm_search_free_generic(mm,size, alignment, color, best_match);
}
static inline  struct drm_mm_node *drm_mm_search_free_in_range_color(
						const struct drm_mm *mm,
						unsigned long size,
						unsigned alignment,
						unsigned long color,
						unsigned long start,
						unsigned long end,
						bool best_match)
{
	return drm_mm_search_free_in_range_generic(mm, size, alignment, color,
						   start, end, best_match);
}
extern int drm_mm_init(struct drm_mm *mm,
		       unsigned long start,
		       unsigned long size);
extern void drm_mm_takedown(struct drm_mm *mm);
extern int drm_mm_clean(struct drm_mm *mm);
extern int drm_mm_pre_get(struct drm_mm *mm);

static inline struct drm_mm *drm_get_mm(struct drm_mm_node *block)
{
	return block->mm;
}

void drm_mm_init_scan(struct drm_mm *mm,
		      unsigned long size,
		      unsigned alignment,
		      unsigned long color);
void drm_mm_init_scan_with_range(struct drm_mm *mm,
				 unsigned long size,
				 unsigned alignment,
				 unsigned long color,
				 unsigned long start,
				 unsigned long end);
int drm_mm_scan_add_block(struct drm_mm_node *node);
int drm_mm_scan_remove_block(struct drm_mm_node *node);

extern void drm_mm_debug_table(struct drm_mm *mm, const char *prefix);
=======
int drm_mm_reserve_node(struct drm_mm *mm, struct drm_mm_node *node);

int drm_mm_insert_node_generic(struct drm_mm *mm,
			       struct drm_mm_node *node,
			       u64 size,
			       unsigned alignment,
			       unsigned long color,
			       enum drm_mm_search_flags sflags,
			       enum drm_mm_allocator_flags aflags);
/**
 * drm_mm_insert_node - search for space and insert @node
 * @mm: drm_mm to allocate from
 * @node: preallocate node to insert
 * @size: size of the allocation
 * @alignment: alignment of the allocation
 * @flags: flags to fine-tune the allocation
 *
 * This is a simplified version of drm_mm_insert_node_generic() with @color set
 * to 0.
 *
 * The preallocated node must be cleared to 0.
 *
 * Returns:
 * 0 on success, -ENOSPC if there's no suitable hole.
 */
static inline int drm_mm_insert_node(struct drm_mm *mm,
				     struct drm_mm_node *node,
				     u64 size,
				     unsigned alignment,
				     enum drm_mm_search_flags flags)
{
	return drm_mm_insert_node_generic(mm, node, size, alignment, 0, flags,
					  DRM_MM_CREATE_DEFAULT);
}

int drm_mm_insert_node_in_range_generic(struct drm_mm *mm,
					struct drm_mm_node *node,
					u64 size,
					unsigned alignment,
					unsigned long color,
					u64 start,
					u64 end,
					enum drm_mm_search_flags sflags,
					enum drm_mm_allocator_flags aflags);
/**
 * drm_mm_insert_node_in_range - ranged search for space and insert @node
 * @mm: drm_mm to allocate from
 * @node: preallocate node to insert
 * @size: size of the allocation
 * @alignment: alignment of the allocation
 * @start: start of the allowed range for this node
 * @end: end of the allowed range for this node
 * @flags: flags to fine-tune the allocation
 *
 * This is a simplified version of drm_mm_insert_node_in_range_generic() with
 * @color set to 0.
 *
 * The preallocated node must be cleared to 0.
 *
 * Returns:
 * 0 on success, -ENOSPC if there's no suitable hole.
 */
static inline int drm_mm_insert_node_in_range(struct drm_mm *mm,
					      struct drm_mm_node *node,
					      u64 size,
					      unsigned alignment,
					      u64 start,
					      u64 end,
					      enum drm_mm_search_flags flags)
{
	return drm_mm_insert_node_in_range_generic(mm, node, size, alignment,
						   0, start, end, flags,
						   DRM_MM_CREATE_DEFAULT);
}

void drm_mm_remove_node(struct drm_mm_node *node);
void drm_mm_replace_node(struct drm_mm_node *old, struct drm_mm_node *new);
void drm_mm_init(struct drm_mm *mm,
		 u64 start,
		 u64 size);
void drm_mm_takedown(struct drm_mm *mm);
bool drm_mm_clean(struct drm_mm *mm);

struct drm_mm_node *
drm_mm_interval_first(struct drm_mm *mm, u64 start, u64 last);

struct drm_mm_node *
drm_mm_interval_next(struct drm_mm_node *node, u64 start, u64 last);

void drm_mm_init_scan(struct drm_mm *mm,
		      u64 size,
		      unsigned alignment,
		      unsigned long color);
void drm_mm_init_scan_with_range(struct drm_mm *mm,
				 u64 size,
				 unsigned alignment,
				 unsigned long color,
				 u64 start,
				 u64 end);
bool drm_mm_scan_add_block(struct drm_mm_node *node);
bool drm_mm_scan_remove_block(struct drm_mm_node *node);

void drm_mm_debug_table(struct drm_mm *mm, const char *prefix);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_DEBUG_FS
int drm_mm_dump_table(struct seq_file *m, struct drm_mm *mm);
#endif

#endif
