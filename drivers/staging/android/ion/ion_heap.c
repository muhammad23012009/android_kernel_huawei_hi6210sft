/*
<<<<<<< HEAD
 * drivers/gpu/ion/ion_heap.c
=======
 * drivers/staging/android/ion/ion_heap.c
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

#include <linux/err.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <linux/rtmutex.h>
#include <linux/sched.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include "ion.h"
#include "ion_priv.h"

#include <linux/hisi/hisi-iommu.h>

=======
#include "ion.h"
#include "ion_priv.h"

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void *ion_heap_map_kernel(struct ion_heap *heap,
			  struct ion_buffer *buffer)
{
	struct scatterlist *sg;
	int i, j;
	void *vaddr;
	pgprot_t pgprot;
	struct sg_table *table = buffer->sg_table;
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;
	struct page **pages = vmalloc(sizeof(struct page *) * npages);
	struct page **tmp = pages;

	if (!pages)
<<<<<<< HEAD
		return NULL;
=======
		return ERR_PTR(-ENOMEM);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (buffer->flags & ION_FLAG_CACHED)
		pgprot = PAGE_KERNEL;
	else
		pgprot = pgprot_writecombine(PAGE_KERNEL);

	for_each_sg(table->sgl, sg, table->nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);

		BUG_ON(i >= npages);
		for (j = 0; j < npages_this_entry; j++)
			*(tmp++) = page++;
	}
	vaddr = vmap(pages, npages, VM_MAP, pgprot);
	vfree(pages);

<<<<<<< HEAD
	if (vaddr == NULL)
=======
	if (!vaddr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return ERR_PTR(-ENOMEM);

	return vaddr;
}

void ion_heap_unmap_kernel(struct ion_heap *heap,
			   struct ion_buffer *buffer)
{
	vunmap(buffer->vaddr);
}

int ion_heap_map_user(struct ion_heap *heap, struct ion_buffer *buffer,
		      struct vm_area_struct *vma)
{
	struct sg_table *table = buffer->sg_table;
	unsigned long addr = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	struct scatterlist *sg;
	int i;
	int ret;

	for_each_sg(table->sgl, sg, table->nents, i) {
		struct page *page = sg_page(sg);
		unsigned long remainder = vma->vm_end - addr;
		unsigned long len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		} else if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}
		len = min(len, remainder);
		ret = remap_pfn_range(vma, addr, page_to_pfn(page), len,
<<<<<<< HEAD
				vma->vm_page_prot);
=======
				      vma->vm_page_prot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ret)
			return ret;
		addr += len;
		if (addr >= vma->vm_end)
			return 0;
	}
	return 0;
}

<<<<<<< HEAD
int ion_heap_map_iommu(struct ion_buffer *buffer,
			struct ion_iommu_map *map_data)
{
	struct sg_table *table = buffer->sg_table;
	int ret;

	ret = hisi_iommu_map_domain(table->sgl, &map_data->format);
	if (ret) {
		pr_err("%s: iommu map failed, heap: %s\n", __func__,
			buffer->heap->name);
	}
	return ret;
}

void ion_heap_unmap_iommu(struct ion_iommu_map *map_data)
{
	int ret;
	ret = hisi_iommu_unmap_domain(&map_data->format);
	if (ret) {
		pr_err("%s: iommu unmap failed, heap: %s\n", __func__,
			map_data->buffer->heap->name);
	}
}

static int ion_heap_clear_pages(struct page **pages, int num, pgprot_t pgprot)
{
	void *addr = vm_map_ram(pages, num, -1, pgprot);
=======
static int ion_heap_clear_pages(struct page **pages, int num, pgprot_t pgprot)
{
	void *addr = vmap(pages, num, VM_MAP, pgprot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!addr)
		return -ENOMEM;
	memset(addr, 0, PAGE_SIZE * num);
<<<<<<< HEAD
	vm_unmap_ram(addr, num);
=======
	vunmap(addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
#if 0

int ion_heap_sglist_zero(struct scatterlist *sgl, unsigned int nents,
						pgprot_t pgprot)
=======
static int ion_heap_sglist_zero(struct scatterlist *sgl, unsigned int nents,
				pgprot_t pgprot)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int p = 0;
	int ret = 0;
	struct sg_page_iter piter;
	struct page *pages[32];

	for_each_sg_page(sgl, &piter, nents, 0) {
		pages[p++] = sg_page_iter_page(&piter);
		if (p == ARRAY_SIZE(pages)) {
			ret = ion_heap_clear_pages(pages, p, pgprot);
			if (ret)
				return ret;
			p = 0;
		}
	}
	if (p)
		ret = ion_heap_clear_pages(pages, p, pgprot);

	return ret;
}

<<<<<<< HEAD
#else

static int ion_heap_clear_pages_for_large_buffer(struct page **pages, int num, pgprot_t pgprot)
{
	void *vaddr;

	vaddr = vmap(pages, num, VM_MAP, pgprot);
	if (!vaddr)
		return -ENOMEM;
	memset(vaddr, 0, PAGE_SIZE * num);
	vunmap(vaddr);

	return 0;
}

int ion_heap_sglist_zero(struct scatterlist *sgl, unsigned int nents,
						pgprot_t pgprot)
{
	struct scatterlist *sg;
	int i;
#ifdef CONFIG_ARM64	
	for_each_sg(sgl, sg, nents, i) {
		struct page *p = sg_page(sg);
		unsigned int len = sg->length;

		memset(page_address(p), 0, len);
	}

	return 0;
#else
	int j, k = 0;
	int ret = 0;
	struct page **pages;
	unsigned int max_npages = 1024;

	pages = kzalloc(sizeof(struct page *) * max_npages, GFP_KERNEL);
	if (!pages) {
		pr_err("%s: allocate pages failed!\n", __func__);
		return -ENOMEM;
	}

	for_each_sg(sgl, sg, nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);

		for (j = 0; j < npages_this_entry; j++) {
			pages[k++] = page++;
			if (k >= max_npages) {
				ret = ion_heap_clear_pages_for_large_buffer(pages, k, pgprot);
				if (ret) {
					pr_err("%s: ion_heap_clear_pages failed!\n", __func__);
					goto done;
				}
				k = 0;
			}
		}
	}

	if (k) {
		ret = ion_heap_clear_pages_for_large_buffer(pages, k, pgprot);

		if (ret)
			pr_err("%s: failed! k: %d\n", __func__, k);
	}

done:
	kfree(pages);
	return ret;
#endif
}
#endif

int ion_heap_buffer_zero(struct ion_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;

	return ion_heap_sglist_zero(table->sgl, table->nents, PAGE_KERNEL);
=======
int ion_heap_buffer_zero(struct ion_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	pgprot_t pgprot;

	if (buffer->flags & ION_FLAG_CACHED)
		pgprot = PAGE_KERNEL;
	else
		pgprot = pgprot_writecombine(PAGE_KERNEL);

	return ion_heap_sglist_zero(table->sgl, table->nents, pgprot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int ion_heap_pages_zero(struct page *page, size_t size, pgprot_t pgprot)
{
	struct scatterlist sg;

	sg_init_table(&sg, 1);
	sg_set_page(&sg, page, size, 0);
	return ion_heap_sglist_zero(&sg, 1, pgprot);
}

void ion_heap_freelist_add(struct ion_heap *heap, struct ion_buffer *buffer)
{
	spin_lock(&heap->free_lock);
	list_add(&buffer->list, &heap->free_list);
	heap->free_list_size += buffer->size;
	spin_unlock(&heap->free_lock);
	wake_up(&heap->waitqueue);
}

size_t ion_heap_freelist_size(struct ion_heap *heap)
{
	size_t size;

	spin_lock(&heap->free_lock);
	size = heap->free_list_size;
	spin_unlock(&heap->free_lock);

	return size;
}

static size_t _ion_heap_freelist_drain(struct ion_heap *heap, size_t size,
<<<<<<< HEAD
				bool skip_pools)
=======
				       bool skip_pools)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ion_buffer *buffer;
	size_t total_drained = 0;

	if (ion_heap_freelist_size(heap) == 0)
		return 0;

	spin_lock(&heap->free_lock);
	if (size == 0)
		size = heap->free_list_size;

	while (!list_empty(&heap->free_list)) {
		if (total_drained >= size)
			break;
		buffer = list_first_entry(&heap->free_list, struct ion_buffer,
					  list);
		list_del(&buffer->list);
		heap->free_list_size -= buffer->size;
		if (skip_pools)
			buffer->private_flags |= ION_PRIV_FLAG_SHRINKER_FREE;
		total_drained += buffer->size;
		spin_unlock(&heap->free_lock);
		ion_buffer_destroy(buffer);
		spin_lock(&heap->free_lock);
	}
	spin_unlock(&heap->free_lock);

	return total_drained;
}

size_t ion_heap_freelist_drain(struct ion_heap *heap, size_t size)
{
	return _ion_heap_freelist_drain(heap, size, false);
}

size_t ion_heap_freelist_shrink(struct ion_heap *heap, size_t size)
{
	return _ion_heap_freelist_drain(heap, size, true);
}

static int ion_heap_deferred_free(void *data)
{
	struct ion_heap *heap = data;

	while (true) {
		struct ion_buffer *buffer;

		wait_event_freezable(heap->waitqueue,
				     ion_heap_freelist_size(heap) > 0);

		spin_lock(&heap->free_lock);
		if (list_empty(&heap->free_list)) {
			spin_unlock(&heap->free_lock);
			continue;
		}
		buffer = list_first_entry(&heap->free_list, struct ion_buffer,
					  list);
		list_del(&buffer->list);
		heap->free_list_size -= buffer->size;
		spin_unlock(&heap->free_lock);
		ion_buffer_destroy(buffer);
	}

	return 0;
}

int ion_heap_init_deferred_free(struct ion_heap *heap)
{
	struct sched_param param = { .sched_priority = 0 };

	INIT_LIST_HEAD(&heap->free_list);
	init_waitqueue_head(&heap->waitqueue);
	heap->task = kthread_run(ion_heap_deferred_free, heap,
				 "%s", heap->name);
<<<<<<< HEAD
	sched_setscheduler(heap->task, SCHED_IDLE, &param);
	if (IS_ERR(heap->task)) {
		pr_err("%s: creating thread for deferred free failed\n",
		       __func__);
		return PTR_RET(heap->task);
	}
	return 0;
}

static int ion_heap_shrink(struct shrinker *shrinker, struct shrink_control *sc)
=======
	if (IS_ERR(heap->task)) {
		pr_err("%s: creating thread for deferred free failed\n",
		       __func__);
		return PTR_ERR_OR_ZERO(heap->task);
	}
	sched_setscheduler(heap->task, SCHED_IDLE, &param);
	return 0;
}

static unsigned long ion_heap_shrink_count(struct shrinker *shrinker,
					   struct shrink_control *sc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct ion_heap *heap = container_of(shrinker, struct ion_heap,
					     shrinker);
	int total = 0;
<<<<<<< HEAD
=======

	total = ion_heap_freelist_size(heap) / PAGE_SIZE;
	if (heap->ops->shrink)
		total += heap->ops->shrink(heap, sc->gfp_mask, 0);
	return total;
}

static unsigned long ion_heap_shrink_scan(struct shrinker *shrinker,
					  struct shrink_control *sc)
{
	struct ion_heap *heap = container_of(shrinker, struct ion_heap,
					     shrinker);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int freed = 0;
	int to_scan = sc->nr_to_scan;

	if (to_scan == 0)
<<<<<<< HEAD
		goto out;
=======
		return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * shrink the free list first, no point in zeroing the memory if we're
	 * just going to reclaim it. Also, skip any possible page pooling.
	 */
	if (heap->flags & ION_HEAP_FLAG_DEFER_FREE)
		freed = ion_heap_freelist_shrink(heap, to_scan * PAGE_SIZE) /
				PAGE_SIZE;

	to_scan -= freed;
<<<<<<< HEAD
	if (to_scan < 0)
		to_scan = 0;

out:
	total = ion_heap_freelist_size(heap) / PAGE_SIZE;
	if (heap->ops->shrink)
		total += heap->ops->shrink(heap, sc->gfp_mask, to_scan);
	return total;
=======
	if (to_scan <= 0)
		return freed;

	if (heap->ops->shrink)
		freed += heap->ops->shrink(heap, sc->gfp_mask, to_scan);
	return freed;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void ion_heap_init_shrinker(struct ion_heap *heap)
{
<<<<<<< HEAD
	heap->shrinker.shrink = ion_heap_shrink;
=======
	heap->shrinker.count_objects = ion_heap_shrink_count;
	heap->shrinker.scan_objects = ion_heap_shrink_scan;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	heap->shrinker.seeks = DEFAULT_SEEKS;
	heap->shrinker.batch = 0;
	register_shrinker(&heap->shrinker);
}

struct ion_heap *ion_heap_create(struct ion_platform_heap *heap_data)
{
	struct ion_heap *heap = NULL;

	switch (heap_data->type) {
	case ION_HEAP_TYPE_SYSTEM_CONTIG:
<<<<<<< HEAD
		heap = ion_system_contig_heap_create(heap_data);
		break;
=======
		pr_err("%s: Heap type is disabled: %d\n", __func__,
		       heap_data->type);
		return ERR_PTR(-EINVAL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case ION_HEAP_TYPE_SYSTEM:
		heap = ion_system_heap_create(heap_data);
		break;
	case ION_HEAP_TYPE_CARVEOUT:
		heap = ion_carveout_heap_create(heap_data);
		break;
	case ION_HEAP_TYPE_CHUNK:
		heap = ion_chunk_heap_create(heap_data);
		break;
	case ION_HEAP_TYPE_DMA:
		heap = ion_cma_heap_create(heap_data);
		break;
<<<<<<< HEAD
	case ION_HEAP_TYPE_CPUDRAW:
		heap = ion_cpudraw_heap_create(heap_data);
		break;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		pr_err("%s: Invalid heap type %d\n", __func__,
		       heap_data->type);
		return ERR_PTR(-EINVAL);
	}

	if (IS_ERR_OR_NULL(heap)) {
		pr_err("%s: error creating heap %s type %d base %lu size %zu\n",
		       __func__, heap_data->name, heap_data->type,
		       heap_data->base, heap_data->size);
		return ERR_PTR(-EINVAL);
	}

	heap->name = heap_data->name;
	heap->id = heap_data->id;
	return heap;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(ion_heap_create);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void ion_heap_destroy(struct ion_heap *heap)
{
	if (!heap)
		return;

	switch (heap->type) {
	case ION_HEAP_TYPE_SYSTEM_CONTIG:
<<<<<<< HEAD
		ion_system_contig_heap_destroy(heap);
=======
		pr_err("%s: Heap type is disabled: %d\n", __func__,
		       heap->type);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;
	case ION_HEAP_TYPE_SYSTEM:
		ion_system_heap_destroy(heap);
		break;
	case ION_HEAP_TYPE_CARVEOUT:
		ion_carveout_heap_destroy(heap);
		break;
	case ION_HEAP_TYPE_CHUNK:
		ion_chunk_heap_destroy(heap);
		break;
	case ION_HEAP_TYPE_DMA:
		ion_cma_heap_destroy(heap);
		break;
<<<<<<< HEAD
	case ION_HEAP_TYPE_CPUDRAW:
		ion_cpudraw_heap_destroy(heap);
		break;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	default:
		pr_err("%s: Invalid heap type %d\n", __func__,
		       heap->type);
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(ion_heap_destroy);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
