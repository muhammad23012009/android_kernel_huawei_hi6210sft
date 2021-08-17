/*
<<<<<<< HEAD
 * drivers/gpu/ion/ion_mem_pool.c
=======
 * drivers/staging/android/ion/ion_mem_pool.c
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/debugfs.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/list.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/slab.h>
#include "ion_priv.h"

struct ion_page_pool_item {
	struct page *page;
	struct list_head list;
};

static void *ion_page_pool_alloc_pages(struct ion_page_pool *pool)
{
/* zero buffer later */
#if 0
	struct page *page = alloc_pages(pool->gfp_mask, pool->order);
#else
	struct page *page = alloc_pages(pool->gfp_mask & (~__GFP_ZERO), pool->order);
#endif

	if (!page)
		return NULL;
#if 0
	ion_pages_sync_for_device(NULL, page, PAGE_SIZE << pool->order,
						DMA_BIDIRECTIONAL);
#endif
=======
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include "ion_priv.h"

static void *ion_page_pool_alloc_pages(struct ion_page_pool *pool)
{
	struct page *page = alloc_pages(pool->gfp_mask, pool->order);

	if (!page)
		return NULL;
	if (!pool->cached)
		ion_pages_sync_for_device(NULL, page, PAGE_SIZE << pool->order,
					  DMA_BIDIRECTIONAL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return page;
}

static void ion_page_pool_free_pages(struct ion_page_pool *pool,
				     struct page *page)
{
	__free_pages(page, pool->order);
}

static int ion_page_pool_add(struct ion_page_pool *pool, struct page *page)
{
<<<<<<< HEAD
	struct ion_page_pool_item *item;

	item = kmalloc(sizeof(struct ion_page_pool_item), GFP_KERNEL);
	if (!item)
		return -ENOMEM;

	mutex_lock(&pool->mutex);
	item->page = page;
	if (PageHighMem(page)) {
		list_add_tail(&item->list, &pool->high_items);
		pool->high_count++;
	} else {
		list_add_tail(&item->list, &pool->low_items);
=======
	mutex_lock(&pool->mutex);
	if (PageHighMem(page)) {
		list_add_tail(&page->lru, &pool->high_items);
		pool->high_count++;
	} else {
		list_add_tail(&page->lru, &pool->low_items);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pool->low_count++;
	}
	mutex_unlock(&pool->mutex);
	return 0;
}

static struct page *ion_page_pool_remove(struct ion_page_pool *pool, bool high)
{
<<<<<<< HEAD
	struct ion_page_pool_item *item;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct page *page;

	if (high) {
		BUG_ON(!pool->high_count);
<<<<<<< HEAD
		item = list_first_entry(&pool->high_items,
					struct ion_page_pool_item, list);
		pool->high_count--;
	} else {
		BUG_ON(!pool->low_count);
		item = list_first_entry(&pool->low_items,
					struct ion_page_pool_item, list);
		pool->low_count--;
	}

	list_del(&item->list);
	page = item->page;
	kfree(item);
	return page;
}

void *ion_page_pool_alloc(struct ion_page_pool *pool)
{
	struct page *page = NULL;

	if (!pool) {
		BUG_ON(!pool);
		return NULL;
	}
=======
		page = list_first_entry(&pool->high_items, struct page, lru);
		pool->high_count--;
	} else {
		BUG_ON(!pool->low_count);
		page = list_first_entry(&pool->low_items, struct page, lru);
		pool->low_count--;
	}

	list_del(&page->lru);
	return page;
}

struct page *ion_page_pool_alloc(struct ion_page_pool *pool)
{
	struct page *page = NULL;

	BUG_ON(!pool);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	mutex_lock(&pool->mutex);
	if (pool->high_count)
		page = ion_page_pool_remove(pool, true);
	else if (pool->low_count)
		page = ion_page_pool_remove(pool, false);
	mutex_unlock(&pool->mutex);

	if (!page)
		page = ion_page_pool_alloc_pages(pool);

	return page;
}

void ion_page_pool_free(struct ion_page_pool *pool, struct page *page)
{
	int ret;

<<<<<<< HEAD
=======
	BUG_ON(pool->order != compound_order(page));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = ion_page_pool_add(pool, page);
	if (ret)
		ion_page_pool_free_pages(pool, page);
}

static int ion_page_pool_total(struct ion_page_pool *pool, bool high)
{
<<<<<<< HEAD
	int total = 0;

	total += high ? (pool->high_count + pool->low_count) *
		(1 << pool->order) :
			pool->low_count * (1 << pool->order);
	return total;
}

int ion_page_pool_shrink(struct ion_page_pool *pool, gfp_t gfp_mask,
				int nr_to_scan)
{
	int i;
	bool high;

	high = !!(gfp_mask & __GFP_HIGHMEM);

	for (i = 0; i < nr_to_scan; i++) {
=======
	int count = pool->low_count;

	if (high)
		count += pool->high_count;

	return count << pool->order;
}

int ion_page_pool_shrink(struct ion_page_pool *pool, gfp_t gfp_mask,
			 int nr_to_scan)
{
	int freed = 0;
	bool high;

	if (current_is_kswapd())
		high = true;
	else
		high = !!(gfp_mask & __GFP_HIGHMEM);

	if (nr_to_scan == 0)
		return ion_page_pool_total(pool, high);

	while (freed < nr_to_scan) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct page *page;

		mutex_lock(&pool->mutex);
		if (pool->low_count) {
			page = ion_page_pool_remove(pool, false);
		} else if (high && pool->high_count) {
			page = ion_page_pool_remove(pool, true);
		} else {
			mutex_unlock(&pool->mutex);
			break;
		}
		mutex_unlock(&pool->mutex);
		ion_page_pool_free_pages(pool, page);
<<<<<<< HEAD
	}

	return ion_page_pool_total(pool, high);
}

struct ion_page_pool *ion_page_pool_create(gfp_t gfp_mask, unsigned int order)
{
	struct ion_page_pool *pool = kmalloc(sizeof(struct ion_page_pool),
					     GFP_KERNEL);
=======
		freed += (1 << pool->order);
	}

	return freed;
}

struct ion_page_pool *ion_page_pool_create(gfp_t gfp_mask, unsigned int order,
					   bool cached)
{
	struct ion_page_pool *pool = kmalloc(sizeof(*pool), GFP_KERNEL);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!pool)
		return NULL;
	pool->high_count = 0;
	pool->low_count = 0;
	INIT_LIST_HEAD(&pool->low_items);
	INIT_LIST_HEAD(&pool->high_items);
<<<<<<< HEAD
	pool->gfp_mask = gfp_mask;
	pool->order = order;
	mutex_init(&pool->mutex);
	plist_node_init(&pool->list, order);
=======
	pool->gfp_mask = gfp_mask | __GFP_COMP;
	pool->order = order;
	mutex_init(&pool->mutex);
	plist_node_init(&pool->list, order);
	if (cached)
		pool->cached = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return pool;
}

void ion_page_pool_destroy(struct ion_page_pool *pool)
{
	kfree(pool);
}

static int __init ion_page_pool_init(void)
{
	return 0;
}
<<<<<<< HEAD

static void __exit ion_page_pool_exit(void)
{
}

module_init(ion_page_pool_init);
module_exit(ion_page_pool_exit);
=======
device_initcall(ion_page_pool_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
