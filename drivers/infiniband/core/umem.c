/*
 * Copyright (c) 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2005 Cisco Systems.  All rights reserved.
 * Copyright (c) 2005 Mellanox Technologies. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/export.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD
#include <linux/dma-attrs.h>
#include <linux/slab.h>

#include "uverbs.h"

#define IB_UMEM_MAX_PAGE_CHUNK						\
	((PAGE_SIZE - offsetof(struct ib_umem_chunk, page_list)) /	\
	 ((void *) &((struct ib_umem_chunk *) 0)->page_list[1] -	\
	  (void *) &((struct ib_umem_chunk *) 0)->page_list[0]))

static void __ib_umem_release(struct ib_device *dev, struct ib_umem *umem, int dirty)
{
	struct ib_umem_chunk *chunk, *tmp;
	int i;

	list_for_each_entry_safe(chunk, tmp, &umem->chunk_list, list) {
		ib_dma_unmap_sg(dev, chunk->page_list,
				chunk->nents, DMA_BIDIRECTIONAL);
		for (i = 0; i < chunk->nents; ++i) {
			struct page *page = sg_page(&chunk->page_list[i]);

			if (umem->writable && dirty)
				set_page_dirty_lock(page);
			put_page(page);
		}

		kfree(chunk);
	}
=======
#include <linux/slab.h>
#include <rdma/ib_umem_odp.h>

#include "uverbs.h"


static void __ib_umem_release(struct ib_device *dev, struct ib_umem *umem, int dirty)
{
	struct scatterlist *sg;
	struct page *page;
	int i;

	if (umem->nmap > 0)
		ib_dma_unmap_sg(dev, umem->sg_head.sgl,
				umem->nmap,
				DMA_BIDIRECTIONAL);

	for_each_sg(umem->sg_head.sgl, sg, umem->npages, i) {

		page = sg_page(sg);
		if (umem->writable && dirty)
			set_page_dirty_lock(page);
		put_page(page);
	}

	sg_free_table(&umem->sg_head);
	return;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ib_umem_get - Pin and DMA map userspace memory.
<<<<<<< HEAD
=======
 *
 * If access flags indicate ODP memory, avoid pinning. Instead, stores
 * the mm for future page fault handling in conjunction with MMU notifiers.
 *
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @context: userspace context to pin memory for
 * @addr: userspace virtual address to start at
 * @size: length of region to pin
 * @access: IB_ACCESS_xxx flags for memory being pinned
 * @dmasync: flush in-flight DMA when the memory region is written
 */
struct ib_umem *ib_umem_get(struct ib_ucontext *context, unsigned long addr,
			    size_t size, int access, int dmasync)
{
	struct ib_umem *umem;
	struct page **page_list;
	struct vm_area_struct **vma_list;
<<<<<<< HEAD
	struct ib_umem_chunk *chunk;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long locked;
	unsigned long lock_limit;
	unsigned long cur_base;
	unsigned long npages;
	int ret;
<<<<<<< HEAD
	int off;
	int i;
	DEFINE_DMA_ATTRS(attrs);

	if (dmasync)
		dma_set_attr(DMA_ATTR_WRITE_BARRIER, &attrs);
=======
	int i;
	unsigned long dma_attrs = 0;
	struct scatterlist *sg, *sg_list_start;
	int need_release = 0;
	unsigned int gup_flags = FOLL_WRITE;

	if (dmasync)
		dma_attrs |= DMA_ATTR_WRITE_BARRIER;

	if (!size)
		return ERR_PTR(-EINVAL);

	/*
	 * If the combination of the addr and size requested for this memory
	 * region causes an integer overflow, return error.
	 */
	if (((addr + size) < addr) ||
	    PAGE_ALIGN(addr + size) < (addr + size))
		return ERR_PTR(-EINVAL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!can_do_mlock())
		return ERR_PTR(-EPERM);

<<<<<<< HEAD
	umem = kmalloc(sizeof *umem, GFP_KERNEL);
=======
	umem = kzalloc(sizeof *umem, GFP_KERNEL);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!umem)
		return ERR_PTR(-ENOMEM);

	umem->context   = context;
	umem->length    = size;
<<<<<<< HEAD
	umem->offset    = addr & ~PAGE_MASK;
	umem->page_size = PAGE_SIZE;
	/*
	 * We ask for writable memory if any access flags other than
	 * "remote read" are set.  "Local write" and "remote write"
	 * obviously require write access.  "Remote atomic" can do
	 * things like fetch and add, which will modify memory, and
	 * "MW bind" can change permissions by binding a window.
	 */
	umem->writable  = !!(access & ~IB_ACCESS_REMOTE_READ);
=======
	umem->address   = addr;
	umem->page_size = PAGE_SIZE;
	umem->pid       = get_task_pid(current, PIDTYPE_PID);
	umem->writable   = ib_access_writable(access);

	if (access & IB_ACCESS_ON_DEMAND) {
		put_pid(umem->pid);
		ret = ib_umem_odp_get(context, umem);
		if (ret) {
			kfree(umem);
			return ERR_PTR(ret);
		}
		return umem;
	}

	umem->odp_data = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* We assume the memory is from hugetlb until proved otherwise */
	umem->hugetlb   = 1;

<<<<<<< HEAD
	INIT_LIST_HEAD(&umem->chunk_list);

	page_list = (struct page **) __get_free_page(GFP_KERNEL);
	if (!page_list) {
=======
	page_list = (struct page **) __get_free_page(GFP_KERNEL);
	if (!page_list) {
		put_pid(umem->pid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree(umem);
		return ERR_PTR(-ENOMEM);
	}

	/*
	 * if we can't alloc the vma_list, it's not so bad;
	 * just assume the memory is not hugetlb memory
	 */
	vma_list = (struct vm_area_struct **) __get_free_page(GFP_KERNEL);
	if (!vma_list)
		umem->hugetlb = 0;

<<<<<<< HEAD
	npages = PAGE_ALIGN(size + umem->offset) >> PAGE_SHIFT;
=======
	npages = ib_umem_num_pages(umem);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	down_write(&current->mm->mmap_sem);

	locked     = npages + current->mm->pinned_vm;
	lock_limit = rlimit(RLIMIT_MEMLOCK) >> PAGE_SHIFT;

	if ((locked > lock_limit) && !capable(CAP_IPC_LOCK)) {
		ret = -ENOMEM;
		goto out;
	}

	cur_base = addr & PAGE_MASK;

<<<<<<< HEAD
	ret = 0;
	while (npages) {
		ret = get_user_pages(current, current->mm, cur_base,
				     min_t(unsigned long, npages,
					   PAGE_SIZE / sizeof (struct page *)),
				     1, !umem->writable, page_list, vma_list);
=======
	if (npages == 0 || npages > UINT_MAX) {
		ret = -EINVAL;
		goto out;
	}

	ret = sg_alloc_table(&umem->sg_head, npages, GFP_KERNEL);
	if (ret)
		goto out;

	if (!umem->writable)
		gup_flags |= FOLL_FORCE;

	need_release = 1;
	sg_list_start = umem->sg_head.sgl;

	while (npages) {
		ret = get_user_pages_longterm(cur_base,
				     min_t(unsigned long, npages,
					   PAGE_SIZE / sizeof (struct page *)),
				     gup_flags, page_list, vma_list);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (ret < 0)
			goto out;

<<<<<<< HEAD
		cur_base += ret * PAGE_SIZE;
		npages   -= ret;

		off = 0;

		while (ret) {
			chunk = kmalloc(sizeof *chunk + sizeof (struct scatterlist) *
					min_t(int, ret, IB_UMEM_MAX_PAGE_CHUNK),
					GFP_KERNEL);
			if (!chunk) {
				ret = -ENOMEM;
				goto out;
			}

			chunk->nents = min_t(int, ret, IB_UMEM_MAX_PAGE_CHUNK);
			sg_init_table(chunk->page_list, chunk->nents);
			for (i = 0; i < chunk->nents; ++i) {
				if (vma_list &&
				    !is_vm_hugetlb_page(vma_list[i + off]))
					umem->hugetlb = 0;
				sg_set_page(&chunk->page_list[i], page_list[i + off], PAGE_SIZE, 0);
			}

			chunk->nmap = ib_dma_map_sg_attrs(context->device,
							  &chunk->page_list[0],
							  chunk->nents,
							  DMA_BIDIRECTIONAL,
							  &attrs);
			if (chunk->nmap <= 0) {
				for (i = 0; i < chunk->nents; ++i)
					put_page(sg_page(&chunk->page_list[i]));
				kfree(chunk);

				ret = -ENOMEM;
				goto out;
			}

			ret -= chunk->nents;
			off += chunk->nents;
			list_add_tail(&chunk->list, &umem->chunk_list);
		}

		ret = 0;
	}

out:
	if (ret < 0) {
		__ib_umem_release(context->device, umem, 0);
=======
		umem->npages += ret;
		cur_base += ret * PAGE_SIZE;
		npages   -= ret;

		for_each_sg(sg_list_start, sg, ret, i) {
			if (vma_list && !is_vm_hugetlb_page(vma_list[i]))
				umem->hugetlb = 0;

			sg_set_page(sg, page_list[i], PAGE_SIZE, 0);
		}

		/* preparing for next loop */
		sg_list_start = sg;
	}

	umem->nmap = ib_dma_map_sg_attrs(context->device,
				  umem->sg_head.sgl,
				  umem->npages,
				  DMA_BIDIRECTIONAL,
				  dma_attrs);

	if (umem->nmap <= 0) {
		ret = -ENOMEM;
		goto out;
	}

	ret = 0;

out:
	if (ret < 0) {
		if (need_release)
			__ib_umem_release(context->device, umem, 0);
		put_pid(umem->pid);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		kfree(umem);
	} else
		current->mm->pinned_vm = locked;

	up_write(&current->mm->mmap_sem);
	if (vma_list)
		free_page((unsigned long) vma_list);
	free_page((unsigned long) page_list);

	return ret < 0 ? ERR_PTR(ret) : umem;
}
EXPORT_SYMBOL(ib_umem_get);

static void ib_umem_account(struct work_struct *work)
{
	struct ib_umem *umem = container_of(work, struct ib_umem, work);

	down_write(&umem->mm->mmap_sem);
	umem->mm->pinned_vm -= umem->diff;
	up_write(&umem->mm->mmap_sem);
	mmput(umem->mm);
	kfree(umem);
}

/**
 * ib_umem_release - release memory pinned with ib_umem_get
 * @umem: umem struct to release
 */
void ib_umem_release(struct ib_umem *umem)
{
	struct ib_ucontext *context = umem->context;
	struct mm_struct *mm;
<<<<<<< HEAD
	unsigned long diff;

	__ib_umem_release(umem->context->device, umem, 1);

	mm = get_task_mm(current);
	if (!mm) {
		kfree(umem);
		return;
	}

	diff = PAGE_ALIGN(umem->length + umem->offset) >> PAGE_SHIFT;
=======
	struct task_struct *task;
	unsigned long diff;

	if (umem->odp_data) {
		ib_umem_odp_release(umem);
		return;
	}

	__ib_umem_release(umem->context->device, umem, 1);

	task = get_pid_task(umem->pid, PIDTYPE_PID);
	put_pid(umem->pid);
	if (!task)
		goto out;
	mm = get_task_mm(task);
	put_task_struct(task);
	if (!mm)
		goto out;

	diff = ib_umem_num_pages(umem);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * We may be called with the mm's mmap_sem already held.  This
	 * can happen when a userspace munmap() is the call that drops
	 * the last reference to our file and calls our release
	 * method.  If there are memory regions to destroy, we'll end
	 * up here and not be able to take the mmap_sem.  In that case
	 * we defer the vm_locked accounting to the system workqueue.
	 */
	if (context->closing) {
		if (!down_write_trylock(&mm->mmap_sem)) {
			INIT_WORK(&umem->work, ib_umem_account);
			umem->mm   = mm;
			umem->diff = diff;

			queue_work(ib_wq, &umem->work);
			return;
		}
	} else
		down_write(&mm->mmap_sem);

<<<<<<< HEAD
	current->mm->pinned_vm -= diff;
	up_write(&mm->mmap_sem);
	mmput(mm);
=======
	mm->pinned_vm -= diff;
	up_write(&mm->mmap_sem);
	mmput(mm);
out:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree(umem);
}
EXPORT_SYMBOL(ib_umem_release);

int ib_umem_page_count(struct ib_umem *umem)
{
<<<<<<< HEAD
	struct ib_umem_chunk *chunk;
	int shift;
	int i;
	int n;
=======
	int shift;
	int i;
	int n;
	struct scatterlist *sg;

	if (umem->odp_data)
		return ib_umem_num_pages(umem);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	shift = ilog2(umem->page_size);

	n = 0;
<<<<<<< HEAD
	list_for_each_entry(chunk, &umem->chunk_list, list)
		for (i = 0; i < chunk->nmap; ++i)
			n += sg_dma_len(&chunk->page_list[i]) >> shift;
=======
	for_each_sg(umem->sg_head.sgl, sg, umem->nmap, i)
		n += sg_dma_len(sg) >> shift;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return n;
}
EXPORT_SYMBOL(ib_umem_page_count);
<<<<<<< HEAD
=======

/*
 * Copy from the given ib_umem's pages to the given buffer.
 *
 * umem - the umem to copy from
 * offset - offset to start copying from
 * dst - destination buffer
 * length - buffer length
 *
 * Returns 0 on success, or an error code.
 */
int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      size_t length)
{
	size_t end = offset + length;
	int ret;

	if (offset > umem->length || length > umem->length - offset) {
		pr_err("ib_umem_copy_from not in range. offset: %zd umem length: %zd end: %zd\n",
		       offset, umem->length, end);
		return -EINVAL;
	}

	ret = sg_pcopy_to_buffer(umem->sg_head.sgl, umem->npages, dst, length,
				 offset + ib_umem_offset(umem));

	if (ret < 0)
		return ret;
	else if (ret != length)
		return -EINVAL;
	else
		return 0;
}
EXPORT_SYMBOL(ib_umem_copy_from);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
