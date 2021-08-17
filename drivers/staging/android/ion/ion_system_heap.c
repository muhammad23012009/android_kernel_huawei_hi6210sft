/*
<<<<<<< HEAD
 * drivers/gpu/ion/ion_system_heap.c
=======
 * drivers/staging/android/ion/ion_system_heap.c
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

#include <asm/page.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/scatterlist.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <linux/hisi/hisi-iommu.h>
#include <linux/sizes.h>
#include "ion.h"
#include "ion_priv.h"

static gfp_t high_order_gfp_flags = (GFP_HIGHUSER | __GFP_NOWARN |
				     __GFP_NORETRY) & ~__GFP_WAIT;
static gfp_t low_order_gfp_flags  = (GFP_HIGHUSER | __GFP_NOWARN);
static unsigned int orders[] = {8, 4, 0};
static const int num_orders = ARRAY_SIZE(orders);
=======
#include "ion.h"
#include "ion_priv.h"

#define NUM_ORDERS ARRAY_SIZE(orders)

static gfp_t high_order_gfp_flags = (GFP_HIGHUSER | __GFP_ZERO | __GFP_NOWARN |
				     __GFP_NORETRY) & ~__GFP_RECLAIM;
static gfp_t low_order_gfp_flags  = (GFP_HIGHUSER | __GFP_ZERO);
static const unsigned int orders[] = {8, 4, 0};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int order_to_index(unsigned int order)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < num_orders; i++)
=======
	for (i = 0; i < NUM_ORDERS; i++)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (order == orders[i])
			return i;
	BUG();
	return -1;
}

<<<<<<< HEAD
static unsigned int order_to_size(int order)
=======
static inline unsigned int order_to_size(int order)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return PAGE_SIZE << order;
}

struct ion_system_heap {
	struct ion_heap heap;
<<<<<<< HEAD
	struct ion_page_pool **pools;
};

struct page_info {
	struct page *page;
	unsigned int order;
	struct list_head list;
};

=======
	struct ion_page_pool *uncached_pools[NUM_ORDERS];
	struct ion_page_pool *cached_pools[NUM_ORDERS];
};

/**
 * The page from page-pool are all zeroed before. We need do cache
 * clean for cached buffer. The uncached buffer are always non-cached
 * since it's allocated. So no need for non-cached pages.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct page *alloc_buffer_page(struct ion_system_heap *heap,
				      struct ion_buffer *buffer,
				      unsigned long order)
{
	bool cached = ion_buffer_cached(buffer);
<<<<<<< HEAD
	struct ion_page_pool *pool = heap->pools[order_to_index(order)];
	struct page *page;

	if (!cached) {
		page = ion_page_pool_alloc(pool);
	} else {
		gfp_t gfp_flags = low_order_gfp_flags;

		if (order >= 4)
			gfp_flags = high_order_gfp_flags;
		page = alloc_pages(gfp_flags, order);
		if (!page) {
			pr_err("%s: Out of mem, alloc_pages failed, order: %ld!\n",
					 __func__, order);
			return NULL;
		}
		/*change here: flush cache has already done by caller*/
		/*ion_pages_sync_for_device(NULL, page, PAGE_SIZE << order,*/
						/*DMA_BIDIRECTIONAL);*/
	}
	if (!page) {
		pr_err("%s: Out of mem, ion_page_pool_alloc failed, order: %ld!\n",
				 __func__, order);
		return NULL;
	}

=======
	struct ion_page_pool *pool;
	struct page *page;

	if (!cached)
		pool = heap->uncached_pools[order_to_index(order)];
	else
		pool = heap->cached_pools[order_to_index(order)];

	page = ion_page_pool_alloc(pool);

	if (cached)
		ion_pages_sync_for_device(NULL, page, PAGE_SIZE << order,
					  DMA_BIDIRECTIONAL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return page;
}

static void free_buffer_page(struct ion_system_heap *heap,
<<<<<<< HEAD
			     struct ion_buffer *buffer, struct page *page,
			     unsigned int order)
{
#if defined(CONFIG_ARCH_HI6XXX)
	bool cached = ion_buffer_cached(buffer);

	if (!cached && !(buffer->private_flags & ION_PRIV_FLAG_SHRINKER_FREE)) {
		struct ion_page_pool *pool = heap->pools[order_to_index(order)];

		ion_page_pool_free(pool, page);
	} else {
		__free_pages(page, order);
	}
#elif defined(CONFIG_ARCH_HI3630)
	bool cached = ion_buffer_cached(buffer);

	//max page in pool is 32M / 4K = 8192 pages
#define MAX_PAGES_IN_POOLS (8192)
	int page_cnt = 0;
	int i;
	for (i = 0; i < num_orders; i++) {
		struct ion_page_pool *pool = heap->pools[i];
		mutex_lock(&pool->mutex);
		page_cnt += ((1 << pool->order) * pool->high_count);
		page_cnt += ((1 << pool->order) * pool->low_count);
		mutex_unlock(&pool->mutex);
	}

	if (!cached && !(buffer->private_flags & ION_PRIV_FLAG_SHRINKER_FREE)
		&& (page_cnt <= MAX_PAGES_IN_POOLS)) {
		struct ion_page_pool *pool = heap->pools[order_to_index(order)];
		ion_page_pool_free(pool, page);
	} else {
		__free_pages(page, order);
	}
#else
	__free_pages(page, order);
#endif
}


unsigned long alloc_failed_time_stamp;
unsigned int alloc_failed_order;
static struct page_info *alloc_largest_available(struct ion_system_heap *heap,
						 struct ion_buffer *buffer,
						 unsigned long size,
						 unsigned int max_order)
{
	struct page *page;
	struct page_info *info;
	int i = 0;

	info = kmalloc(sizeof(struct page_info), GFP_KERNEL);
	if (!info) {
		pr_err("%s: kmalloc info failed!\n", __func__);
		return NULL;
	}

	if (time_before(jiffies, alloc_failed_time_stamp + 10)) {
		if (8 == alloc_failed_order)
			i = 1;
		else if (4 == alloc_failed_order)
			i = 2;
	} else {
		if (alloc_failed_order != 0)
			alloc_failed_order = 0;
	}

	for (; i < num_orders; i++) {
=======
			     struct ion_buffer *buffer, struct page *page)
{
	struct ion_page_pool *pool;
	unsigned int order = compound_order(page);
	bool cached = ion_buffer_cached(buffer);

	/* go to system */
	if (buffer->private_flags & ION_PRIV_FLAG_SHRINKER_FREE) {
		__free_pages(page, order);
		return;
	}

	if (!cached)
		pool = heap->uncached_pools[order_to_index(order)];
	else
		pool = heap->cached_pools[order_to_index(order)];

	ion_page_pool_free(pool, page);
}


static struct page *alloc_largest_available(struct ion_system_heap *heap,
					    struct ion_buffer *buffer,
					    unsigned long size,
					    unsigned int max_order)
{
	struct page *page;
	int i;

	for (i = 0; i < NUM_ORDERS; i++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (size < order_to_size(orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

		page = alloc_buffer_page(heap, buffer, orders[i]);
<<<<<<< HEAD
		if (!page) {
			alloc_failed_time_stamp = jiffies;
			alloc_failed_order = orders[i];
			continue;
		}

		info->page = page;
		info->order = orders[i];
		INIT_LIST_HEAD(&info->list);
		return info;
	}
	kfree(info);

	pr_err("%s: failed!\n", __func__);
=======
		if (!page)
			continue;

		return page;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return NULL;
}

static int ion_system_heap_allocate(struct ion_heap *heap,
<<<<<<< HEAD
				     struct ion_buffer *buffer,
				     unsigned long size, unsigned long align,
				     unsigned long flags)
=======
				    struct ion_buffer *buffer,
				    unsigned long size, unsigned long align,
				    unsigned long flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ion_system_heap *sys_heap = container_of(heap,
							struct ion_system_heap,
							heap);
	struct sg_table *table;
	struct scatterlist *sg;
<<<<<<< HEAD
	int ret;
	struct list_head pages;
	struct page_info *info, *tmp_info;
=======
	struct list_head pages;
	struct page *page, *tmp_page;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int i = 0;
	unsigned long size_remaining = PAGE_ALIGN(size);
	unsigned int max_order = orders[0];

<<<<<<< HEAD
	if (size / PAGE_SIZE > totalram_pages / 2) {
		pr_err("%s: (size / PAGE_SIZE > totalram_pages / 2)\n", __func__);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&pages);
	while (size_remaining > 0) {
		info = alloc_largest_available(sys_heap, buffer, size_remaining,
						max_order);
		if (!info) {
			pr_err("%s: alloc largest avaliable failed!\n", __func__);
			goto err;
		}
		list_add_tail(&info->list, &pages);
		size_remaining -= (1 << info->order) * PAGE_SIZE;
		max_order = info->order;
		i++;
	}
	table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table) {
		pr_err("%s: kzalloc table failed!\n", __func__);
		goto err;
	}

	ret = sg_alloc_table(table, i, GFP_KERNEL);
	if (ret) {
		pr_err("%s: sg alloc table failed!\n", __func__);
		goto err1;
	}

	sg = table->sgl;
	list_for_each_entry_safe(info, tmp_info, &pages, list) {
		struct page *page = info->page;
		sg_set_page(sg, page, (1 << info->order) * PAGE_SIZE, 0);
		sg = sg_next(sg);
		list_del(&info->list);
		kfree(info);
	}

	buffer->priv_virt = table;
	return 0;
err1:
	kfree(table);
err:
	list_for_each_entry_safe(info, tmp_info, &pages, list) {
		free_buffer_page(sys_heap, buffer, info->page, info->order);
		kfree(info);
	}
	pr_err("%s: failed!\n", __func__);
=======
	if (align > PAGE_SIZE)
		return -EINVAL;

	if (size / PAGE_SIZE > totalram_pages / 2)
		return -ENOMEM;

	INIT_LIST_HEAD(&pages);
	while (size_remaining > 0) {
		page = alloc_largest_available(sys_heap, buffer, size_remaining,
					       max_order);
		if (!page)
			goto free_pages;
		list_add_tail(&page->lru, &pages);
		size_remaining -= PAGE_SIZE << compound_order(page);
		max_order = compound_order(page);
		i++;
	}
	table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table)
		goto free_pages;

	if (sg_alloc_table(table, i, GFP_KERNEL))
		goto free_table;

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &pages, lru) {
		sg_set_page(sg, page, PAGE_SIZE << compound_order(page), 0);
		sg = sg_next(sg);
		list_del(&page->lru);
	}

	buffer->sg_table = table;
	return 0;

free_table:
	kfree(table);
free_pages:
	list_for_each_entry_safe(page, tmp_page, &pages, lru)
		free_buffer_page(sys_heap, buffer, page);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return -ENOMEM;
}

static void ion_system_heap_free(struct ion_buffer *buffer)
{
<<<<<<< HEAD
	struct ion_heap *heap = buffer->heap;
	struct ion_system_heap *sys_heap = container_of(heap,
							struct ion_system_heap,
							heap);
	struct sg_table *table = buffer->sg_table;
#if 0
	bool cached = ion_buffer_cached(buffer);
#endif
	struct scatterlist *sg;
	LIST_HEAD(pages);
	int i;
/* zero buffer when allocate */
#if 0
	/* uncached pages come from the page pools, zero them before returning
	   for security purposes (other allocations are zerod at alloc time */
	if (!cached && !(buffer->private_flags & ION_PRIV_FLAG_SHRINKER_FREE))
		ion_heap_buffer_zero(buffer);
#endif

	for_each_sg(table->sgl, sg, table->nents, i)
		free_buffer_page(sys_heap, buffer, sg_page(sg),
				get_order(sg->length));
=======
	struct ion_system_heap *sys_heap = container_of(buffer->heap,
							struct ion_system_heap,
							heap);
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg;
	int i;

	/* zero the buffer before goto page pool */
	if (!(buffer->private_flags & ION_PRIV_FLAG_SHRINKER_FREE))
		ion_heap_buffer_zero(buffer);

	for_each_sg(table->sgl, sg, table->nents, i)
		free_buffer_page(sys_heap, buffer, sg_page(sg));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sg_free_table(table);
	kfree(table);
}

<<<<<<< HEAD
static struct sg_table *ion_system_heap_map_dma(struct ion_heap *heap,
						struct ion_buffer *buffer)
{
	return buffer->priv_virt;
}

static void ion_system_heap_unmap_dma(struct ion_heap *heap,
				      struct ion_buffer *buffer)
{
	return;
}


static int ion_system_heap_shrink(struct ion_heap *heap, gfp_t gfp_mask,
					int nr_to_scan)
{
	struct ion_system_heap *sys_heap;
	int nr_total = 0;
	int i;

	sys_heap = container_of(heap, struct ion_system_heap, heap);

	for (i = 0; i < num_orders; i++) {
		struct ion_page_pool *pool = sys_heap->pools[i];
		nr_total += ion_page_pool_shrink(pool, gfp_mask, nr_to_scan);
	}

	return nr_total;
}

static void ion_system_heap_buffer_zero(struct ion_buffer *buffer)
{
	ion_heap_buffer_zero(buffer);
#if defined(CONFIG_ARCH_HI3630)
	hi3630_fc_allcpu_allcache();
#endif
	return;
}

static struct ion_heap_ops system_heap_ops = {
	.allocate = ion_system_heap_allocate,
	.free = ion_system_heap_free,
	.map_dma = ion_system_heap_map_dma,
	.unmap_dma = ion_system_heap_unmap_dma,
	.map_kernel = ion_heap_map_kernel,
	.unmap_kernel = ion_heap_unmap_kernel,
	.map_user = ion_heap_map_user,
	.map_iommu = ion_heap_map_iommu,
	.unmap_iommu = ion_heap_unmap_iommu,
	.buffer_zero = ion_system_heap_buffer_zero,
=======
static int ion_system_heap_shrink(struct ion_heap *heap, gfp_t gfp_mask,
				  int nr_to_scan)
{
	struct ion_page_pool *uncached_pool;
	struct ion_page_pool *cached_pool;
	struct ion_system_heap *sys_heap;
	int nr_total = 0;
	int i, nr_freed;
	int only_scan = 0;

	sys_heap = container_of(heap, struct ion_system_heap, heap);

	if (!nr_to_scan)
		only_scan = 1;

	for (i = 0; i < NUM_ORDERS; i++) {
		uncached_pool = sys_heap->uncached_pools[i];
		cached_pool = sys_heap->cached_pools[i];

		if (only_scan) {
			nr_total += ion_page_pool_shrink(uncached_pool,
							 gfp_mask,
							 nr_to_scan);

			nr_total += ion_page_pool_shrink(cached_pool,
							 gfp_mask,
							 nr_to_scan);
		} else {
			nr_freed = ion_page_pool_shrink(uncached_pool,
							gfp_mask,
							nr_to_scan);
			nr_to_scan -= nr_freed;
			nr_total += nr_freed;
			if (nr_to_scan <= 0)
				break;
			nr_freed = ion_page_pool_shrink(cached_pool,
							gfp_mask,
							nr_to_scan);
			nr_to_scan -= nr_freed;
			nr_total += nr_freed;
			if (nr_to_scan <= 0)
				break;
		}
	}
	return nr_total;
}

static struct ion_heap_ops system_heap_ops = {
	.allocate = ion_system_heap_allocate,
	.free = ion_system_heap_free,
	.map_kernel = ion_heap_map_kernel,
	.unmap_kernel = ion_heap_unmap_kernel,
	.map_user = ion_heap_map_user,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.shrink = ion_system_heap_shrink,
};

static int ion_system_heap_debug_show(struct ion_heap *heap, struct seq_file *s,
				      void *unused)
{

	struct ion_system_heap *sys_heap = container_of(heap,
							struct ion_system_heap,
							heap);
	int i;
<<<<<<< HEAD

	for (i = 0; i < num_orders; i++) {
		struct ion_page_pool *pool = sys_heap->pools[i];

		seq_printf(s, "%d order %u highmem pages in pool = %lu total\n",
			   pool->high_count, pool->order,
			   (1 << pool->order) * PAGE_SIZE * pool->high_count);
		seq_printf(s, "%d order %u lowmem pages in pool = %lu total\n",
			   pool->low_count, pool->order,
			   (1 << pool->order) * PAGE_SIZE * pool->low_count);
=======
	struct ion_page_pool *pool;

	for (i = 0; i < NUM_ORDERS; i++) {
		pool = sys_heap->uncached_pools[i];

		seq_printf(s, "%d order %u highmem pages uncached %lu total\n",
			   pool->high_count, pool->order,
			   (PAGE_SIZE << pool->order) * pool->high_count);
		seq_printf(s, "%d order %u lowmem pages uncached %lu total\n",
			   pool->low_count, pool->order,
			   (PAGE_SIZE << pool->order) * pool->low_count);
	}

	for (i = 0; i < NUM_ORDERS; i++) {
		pool = sys_heap->cached_pools[i];

		seq_printf(s, "%d order %u highmem pages cached %lu total\n",
			   pool->high_count, pool->order,
			   (PAGE_SIZE << pool->order) * pool->high_count);
		seq_printf(s, "%d order %u lowmem pages cached %lu total\n",
			   pool->low_count, pool->order,
			   (PAGE_SIZE << pool->order) * pool->low_count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return 0;
}

<<<<<<< HEAD
struct ion_heap *ion_system_heap_create(struct ion_platform_heap *unused)
{
	struct ion_system_heap *heap;
	int i;

	heap = kzalloc(sizeof(struct ion_system_heap), GFP_KERNEL);
	if (!heap)
		return ERR_PTR(-ENOMEM);
	heap->heap.ops = &system_heap_ops;
	heap->heap.type = ION_HEAP_TYPE_SYSTEM;
#ifdef CONFIG_ARCH_HI6XXX
	heap->heap.flags = ION_HEAP_FLAG_DEFER_FREE;
#endif
	heap->pools = kzalloc(sizeof(struct ion_page_pool *) * num_orders,
			      GFP_KERNEL);
	if (!heap->pools)
		goto err_alloc_pools;
	
	if(hisi_iommu_page_size() == SZ_8K) {
		printk(KERN_INFO"cur size %lx orders %lx\n",hisi_iommu_page_size(),num_orders);
		orders[num_orders -1] = 1;	
	}
	
	for (i = 0; i < num_orders; i++) {
		struct ion_page_pool *pool;
		gfp_t gfp_flags = low_order_gfp_flags;

		if (orders[i] >= 4)
			gfp_flags = high_order_gfp_flags;
		pool = ion_page_pool_create(gfp_flags, orders[i]);
		if (!pool)
			goto err_create_pool;
		heap->pools[i] = pool;
	}
	heap->heap.debug_show = ion_system_heap_debug_show;
	return &heap->heap;
err_create_pool:
	for (i = 0; i < num_orders; i++)
		if (heap->pools[i])
			ion_page_pool_destroy(heap->pools[i]);
	kfree(heap->pools);
err_alloc_pools:
=======
static void ion_system_heap_destroy_pools(struct ion_page_pool **pools)
{
	int i;

	for (i = 0; i < NUM_ORDERS; i++)
		if (pools[i])
			ion_page_pool_destroy(pools[i]);
}

static int ion_system_heap_create_pools(struct ion_page_pool **pools,
					bool cached)
{
	int i;

	for (i = 0; i < NUM_ORDERS; i++) {
		struct ion_page_pool *pool;
		gfp_t gfp_flags = low_order_gfp_flags;

		if (orders[i] > 4)
			gfp_flags = high_order_gfp_flags;

		pool = ion_page_pool_create(gfp_flags, orders[i], cached);
		if (!pool)
			goto err_create_pool;
		pools[i] = pool;
	}
	return 0;

err_create_pool:
	ion_system_heap_destroy_pools(pools);
	return -ENOMEM;
}

struct ion_heap *ion_system_heap_create(struct ion_platform_heap *unused)
{
	struct ion_system_heap *heap;

	heap = kzalloc(sizeof(*heap), GFP_KERNEL);
	if (!heap)
		return ERR_PTR(-ENOMEM);
	heap->heap.ops = &system_heap_ops;
	heap->heap.type = ION_HEAP_TYPE_SYSTEM;
	heap->heap.flags = ION_HEAP_FLAG_DEFER_FREE;

	if (ion_system_heap_create_pools(heap->uncached_pools, false))
		goto free_heap;

	if (ion_system_heap_create_pools(heap->cached_pools, true))
		goto destroy_uncached_pools;

	heap->heap.debug_show = ion_system_heap_debug_show;
	return &heap->heap;

destroy_uncached_pools:
	ion_system_heap_destroy_pools(heap->uncached_pools);

free_heap:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(heap);
	return ERR_PTR(-ENOMEM);
}

void ion_system_heap_destroy(struct ion_heap *heap)
{
	struct ion_system_heap *sys_heap = container_of(heap,
							struct ion_system_heap,
							heap);
	int i;

<<<<<<< HEAD
	for (i = 0; i < num_orders; i++)
		ion_page_pool_destroy(sys_heap->pools[i]);
	kfree(sys_heap->pools);
=======
	for (i = 0; i < NUM_ORDERS; i++) {
		ion_page_pool_destroy(sys_heap->uncached_pools[i]);
		ion_page_pool_destroy(sys_heap->cached_pools[i]);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(sys_heap);
}

static int ion_system_contig_heap_allocate(struct ion_heap *heap,
					   struct ion_buffer *buffer,
					   unsigned long len,
					   unsigned long align,
					   unsigned long flags)
{
	int order = get_order(len);
	struct page *page;
	struct sg_table *table;
	unsigned long i;
	int ret;

	if (align > (PAGE_SIZE << order))
		return -EINVAL;

<<<<<<< HEAD
	page = alloc_pages(low_order_gfp_flags | __GFP_ZERO, order);
=======
	page = alloc_pages(low_order_gfp_flags | __GFP_NOWARN, order);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!page)
		return -ENOMEM;

	split_page(page, order);

	len = PAGE_ALIGN(len);
	for (i = len >> PAGE_SHIFT; i < (1 << order); i++)
		__free_page(page + i);

<<<<<<< HEAD
	table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table) {
		ret = -ENOMEM;
		goto out;
=======
	table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table) {
		ret = -ENOMEM;
		goto free_pages;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	ret = sg_alloc_table(table, 1, GFP_KERNEL);
	if (ret)
<<<<<<< HEAD
		goto out;

	sg_set_page(table->sgl, page, len, 0);

	buffer->priv_virt = table;
=======
		goto free_table;

	sg_set_page(table->sgl, page, len, 0);

	buffer->sg_table = table;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	ion_pages_sync_for_device(NULL, page, len, DMA_BIDIRECTIONAL);

	return 0;

<<<<<<< HEAD
out:
	for (i = 0; i < len >> PAGE_SHIFT; i++)
		__free_page(page + i);
	kfree(table);
=======
free_table:
	kfree(table);
free_pages:
	for (i = 0; i < len >> PAGE_SHIFT; i++)
		__free_page(page + i);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static void ion_system_contig_heap_free(struct ion_buffer *buffer)
{
<<<<<<< HEAD
	struct sg_table *table = buffer->priv_virt;
=======
	struct sg_table *table = buffer->sg_table;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct page *page = sg_page(table->sgl);
	unsigned long pages = PAGE_ALIGN(buffer->size) >> PAGE_SHIFT;
	unsigned long i;

	for (i = 0; i < pages; i++)
		__free_page(page + i);
	sg_free_table(table);
	kfree(table);
}

<<<<<<< HEAD
static int ion_system_contig_heap_phys(struct ion_heap *heap,
				       struct ion_buffer *buffer,
				       ion_phys_addr_t *addr, size_t *len)
{
	struct sg_table *table = buffer->priv_virt;
	struct page *page = sg_page(table->sgl);
	*addr = page_to_phys(page);
	*len = buffer->size;
	return 0;
}

static struct sg_table *ion_system_contig_heap_map_dma(struct ion_heap *heap,
						struct ion_buffer *buffer)
{
	return buffer->priv_virt;
}

static void ion_system_contig_heap_unmap_dma(struct ion_heap *heap,
					     struct ion_buffer *buffer)
{
}

static struct ion_heap_ops kmalloc_ops = {
	.allocate = ion_system_contig_heap_allocate,
	.free = ion_system_contig_heap_free,
	.phys = ion_system_contig_heap_phys,
	.map_dma = ion_system_contig_heap_map_dma,
	.unmap_dma = ion_system_contig_heap_unmap_dma,
=======
static struct ion_heap_ops kmalloc_ops = {
	.allocate = ion_system_contig_heap_allocate,
	.free = ion_system_contig_heap_free,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.map_kernel = ion_heap_map_kernel,
	.unmap_kernel = ion_heap_unmap_kernel,
	.map_user = ion_heap_map_user,
};

struct ion_heap *ion_system_contig_heap_create(struct ion_platform_heap *unused)
{
	struct ion_heap *heap;

	heap = kzalloc(sizeof(struct ion_heap), GFP_KERNEL);
	if (!heap)
		return ERR_PTR(-ENOMEM);
	heap->ops = &kmalloc_ops;
	heap->type = ION_HEAP_TYPE_SYSTEM_CONTIG;
	return heap;
}

void ion_system_contig_heap_destroy(struct ion_heap *heap)
{
	kfree(heap);
}
<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
