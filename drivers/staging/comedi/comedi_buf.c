/*
 * comedi_buf.c
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>
<<<<<<< HEAD
=======
 * Copyright (C) 2002 Frank Mori Hess <fmhess@users.sourceforge.net>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
<<<<<<< HEAD
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

=======
 */

#include <linux/vmalloc.h>
#include <linux/slab.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include "comedidev.h"
#include "comedi_internal.h"

#ifdef PAGE_KERNEL_NOCACHE
#define COMEDI_PAGE_PROTECTION		PAGE_KERNEL_NOCACHE
#else
#define COMEDI_PAGE_PROTECTION		PAGE_KERNEL
#endif

<<<<<<< HEAD
static void __comedi_buf_free(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      unsigned n_pages)
{
	struct comedi_async *async = s->async;
	struct comedi_buf_page *buf;
	unsigned i;

	if (async->prealloc_buf) {
		vunmap(async->prealloc_buf);
		async->prealloc_buf = NULL;
		async->prealloc_bufsz = 0;
	}

	if (!async->buf_page_list)
		return;

	for (i = 0; i < n_pages; ++i) {
		buf = &async->buf_page_list[i];
		if (buf->virt_addr) {
			clear_bit(PG_reserved,
				  &(virt_to_page(buf->virt_addr)->flags));
			if (s->async_dma_dir != DMA_NONE) {
#ifdef CONFIG_HAS_DMA
				dma_free_coherent(dev->hw_dev,
=======
static void comedi_buf_map_kref_release(struct kref *kref)
{
	struct comedi_buf_map *bm =
		container_of(kref, struct comedi_buf_map, refcount);
	struct comedi_buf_page *buf;
	unsigned int i;

	if (bm->page_list) {
		for (i = 0; i < bm->n_pages; i++) {
			buf = &bm->page_list[i];
			clear_bit(PG_reserved,
				  &(virt_to_page(buf->virt_addr)->flags));
			if (bm->dma_dir != DMA_NONE) {
#ifdef CONFIG_HAS_DMA
				dma_free_coherent(bm->dma_hw_dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						  PAGE_SIZE,
						  buf->virt_addr,
						  buf->dma_addr);
#endif
			} else {
				free_page((unsigned long)buf->virt_addr);
			}
		}
<<<<<<< HEAD
	}
	vfree(async->buf_page_list);
	async->buf_page_list = NULL;
	async->n_buf_pages = 0;
=======
		vfree(bm->page_list);
	}
	if (bm->dma_dir != DMA_NONE)
		put_device(bm->dma_hw_dev);
	kfree(bm);
}

static void __comedi_buf_free(struct comedi_device *dev,
			      struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	struct comedi_buf_map *bm;
	unsigned long flags;

	if (async->prealloc_buf) {
		vunmap(async->prealloc_buf);
		async->prealloc_buf = NULL;
		async->prealloc_bufsz = 0;
	}

	spin_lock_irqsave(&s->spin_lock, flags);
	bm = async->buf_map;
	async->buf_map = NULL;
	spin_unlock_irqrestore(&s->spin_lock, flags);
	comedi_buf_map_put(bm);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void __comedi_buf_alloc(struct comedi_device *dev,
			       struct comedi_subdevice *s,
<<<<<<< HEAD
			       unsigned n_pages)
{
	struct comedi_async *async = s->async;
	struct page **pages = NULL;
	struct comedi_buf_page *buf;
	unsigned i;
=======
			       unsigned int n_pages)
{
	struct comedi_async *async = s->async;
	struct page **pages = NULL;
	struct comedi_buf_map *bm;
	struct comedi_buf_page *buf;
	unsigned long flags;
	unsigned int i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!IS_ENABLED(CONFIG_HAS_DMA) && s->async_dma_dir != DMA_NONE) {
		dev_err(dev->class_dev,
			"dma buffer allocation not supported\n");
		return;
	}

<<<<<<< HEAD
	async->buf_page_list = vzalloc(sizeof(*buf) * n_pages);
	if (async->buf_page_list)
=======
	bm = kzalloc(sizeof(*async->buf_map), GFP_KERNEL);
	if (!bm)
		return;

	kref_init(&bm->refcount);
	spin_lock_irqsave(&s->spin_lock, flags);
	async->buf_map = bm;
	spin_unlock_irqrestore(&s->spin_lock, flags);
	bm->dma_dir = s->async_dma_dir;
	if (bm->dma_dir != DMA_NONE)
		/* Need ref to hardware device to free buffer later. */
		bm->dma_hw_dev = get_device(dev->hw_dev);

	bm->page_list = vzalloc(sizeof(*buf) * n_pages);
	if (bm->page_list)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pages = vmalloc(sizeof(struct page *) * n_pages);

	if (!pages)
		return;

	for (i = 0; i < n_pages; i++) {
<<<<<<< HEAD
		buf = &async->buf_page_list[i];
		if (s->async_dma_dir != DMA_NONE)
#ifdef CONFIG_HAS_DMA
			buf->virt_addr = dma_alloc_coherent(dev->hw_dev,
=======
		buf = &bm->page_list[i];
		if (bm->dma_dir != DMA_NONE)
#ifdef CONFIG_HAS_DMA
			buf->virt_addr = dma_alloc_coherent(bm->dma_hw_dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
							    PAGE_SIZE,
							    &buf->dma_addr,
							    GFP_KERNEL |
							    __GFP_COMP);
#else
			break;
#endif
		else
			buf->virt_addr = (void *)get_zeroed_page(GFP_KERNEL);
		if (!buf->virt_addr)
			break;

		set_bit(PG_reserved, &(virt_to_page(buf->virt_addr)->flags));

		pages[i] = virt_to_page(buf->virt_addr);
	}
<<<<<<< HEAD
=======
	spin_lock_irqsave(&s->spin_lock, flags);
	bm->n_pages = i;
	spin_unlock_irqrestore(&s->spin_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* vmap the prealloc_buf if all the pages were allocated */
	if (i == n_pages)
		async->prealloc_buf = vmap(pages, n_pages, VM_MAP,
					   COMEDI_PAGE_PROTECTION);

	vfree(pages);
}

<<<<<<< HEAD
=======
void comedi_buf_map_get(struct comedi_buf_map *bm)
{
	if (bm)
		kref_get(&bm->refcount);
}

int comedi_buf_map_put(struct comedi_buf_map *bm)
{
	if (bm)
		return kref_put(&bm->refcount, comedi_buf_map_kref_release);
	return 1;
}

/* returns s->async->buf_map and increments its kref refcount */
struct comedi_buf_map *
comedi_buf_map_from_subdev_get(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	struct comedi_buf_map *bm = NULL;
	unsigned long flags;

	if (!async)
		return NULL;

	spin_lock_irqsave(&s->spin_lock, flags);
	bm = async->buf_map;
	/* only want it if buffer pages allocated */
	if (bm && bm->n_pages)
		comedi_buf_map_get(bm);
	else
		bm = NULL;
	spin_unlock_irqrestore(&s->spin_lock, flags);

	return bm;
}

bool comedi_buf_is_mmapped(struct comedi_subdevice *s)
{
	struct comedi_buf_map *bm = s->async->buf_map;

	return bm && (atomic_read(&bm->refcount.refcount) > 1);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
int comedi_buf_alloc(struct comedi_device *dev, struct comedi_subdevice *s,
		     unsigned long new_size)
{
	struct comedi_async *async = s->async;

	/* Round up new_size to multiple of PAGE_SIZE */
	new_size = (new_size + PAGE_SIZE - 1) & PAGE_MASK;

	/* if no change is required, do nothing */
	if (async->prealloc_buf && async->prealloc_bufsz == new_size)
		return 0;

	/* deallocate old buffer */
<<<<<<< HEAD
	__comedi_buf_free(dev, s, async->n_buf_pages);

	/* allocate new buffer */
	if (new_size) {
		unsigned n_pages = new_size >> PAGE_SHIFT;
=======
	__comedi_buf_free(dev, s);

	/* allocate new buffer */
	if (new_size) {
		unsigned int n_pages = new_size >> PAGE_SHIFT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		__comedi_buf_alloc(dev, s, n_pages);

		if (!async->prealloc_buf) {
			/* allocation failed */
<<<<<<< HEAD
			__comedi_buf_free(dev, s, n_pages);
			return -ENOMEM;
		}
		async->n_buf_pages = n_pages;
=======
			__comedi_buf_free(dev, s);
			return -ENOMEM;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	async->prealloc_bufsz = new_size;

	return 0;
}

<<<<<<< HEAD
void comedi_buf_reset(struct comedi_async *async)
{
=======
void comedi_buf_reset(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	async->buf_write_alloc_count = 0;
	async->buf_write_count = 0;
	async->buf_read_alloc_count = 0;
	async->buf_read_count = 0;

	async->buf_write_ptr = 0;
	async->buf_read_ptr = 0;

	async->cur_chan = 0;
<<<<<<< HEAD
=======
	async->scans_done = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	async->scan_progress = 0;
	async->munge_chan = 0;
	async->munge_count = 0;
	async->munge_ptr = 0;

	async->events = 0;
}

<<<<<<< HEAD
static unsigned int comedi_buf_write_n_available(struct comedi_async *async)
{
=======
static unsigned int comedi_buf_write_n_unalloc(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int free_end = async->buf_read_count + async->prealloc_bufsz;

	return free_end - async->buf_write_alloc_count;
}

<<<<<<< HEAD
static unsigned int __comedi_buf_write_alloc(struct comedi_async *async,
					     unsigned int nbytes,
					     int strict)
{
	unsigned int available = comedi_buf_write_n_available(async);

	if (nbytes > available)
		nbytes = strict ? 0 : available;
=======
unsigned int comedi_buf_write_n_available(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned int free_end = async->buf_read_count + async->prealloc_bufsz;

	return free_end - async->buf_write_count;
}

/**
 * comedi_buf_write_alloc() - Reserve buffer space for writing
 * @s: COMEDI subdevice.
 * @nbytes: Maximum space to reserve in bytes.
 *
 * Reserve up to @nbytes bytes of space to be written in the COMEDI acquisition
 * data buffer associated with the subdevice.  The amount reserved is limited
 * by the space available.
 *
 * Return: The amount of space reserved in bytes.
 */
unsigned int comedi_buf_write_alloc(struct comedi_subdevice *s,
				    unsigned int nbytes)
{
	struct comedi_async *async = s->async;
	unsigned int unalloc = comedi_buf_write_n_unalloc(s);

	if (nbytes > unalloc)
		nbytes = unalloc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	async->buf_write_alloc_count += nbytes;

	/*
	 * ensure the async buffer 'counts' are read and updated
	 * before we write data to the write-alloc'ed buffer space
	 */
	smp_mb();

	return nbytes;
}
<<<<<<< HEAD

/* allocates chunk for the writer from free buffer space */
unsigned int comedi_buf_write_alloc(struct comedi_async *async,
				    unsigned int nbytes)
{
	return __comedi_buf_write_alloc(async, nbytes, 0);
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
EXPORT_SYMBOL_GPL(comedi_buf_write_alloc);

/*
 * munging is applied to data by core as it passes between user
 * and kernel space
 */
<<<<<<< HEAD
static unsigned int comedi_buf_munge(struct comedi_async *async,
				     unsigned int num_bytes)
{
	struct comedi_subdevice *s = async->subdevice;
	unsigned int count = 0;
	const unsigned num_sample_bytes = bytes_per_sample(s);
=======
static unsigned int comedi_buf_munge(struct comedi_subdevice *s,
				     unsigned int num_bytes)
{
	struct comedi_async *async = s->async;
	unsigned int count = 0;
	const unsigned int num_sample_bytes = comedi_bytes_per_sample(s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!s->munge || (async->cmd.flags & CMDF_RAWDATA)) {
		async->munge_count += num_bytes;
		count = num_bytes;
	} else {
		/* don't munge partial samples */
		num_bytes -= num_bytes % num_sample_bytes;
		while (count < num_bytes) {
			int block_size = num_bytes - count;
			unsigned int buf_end;

			buf_end = async->prealloc_bufsz - async->munge_ptr;
			if (block_size > buf_end)
				block_size = buf_end;

			s->munge(s->device, s,
				 async->prealloc_buf + async->munge_ptr,
				 block_size, async->munge_chan);

			/*
			 * ensure data is munged in buffer before the
			 * async buffer munge_count is incremented
			 */
			smp_wmb();

			async->munge_chan += block_size / num_sample_bytes;
			async->munge_chan %= async->cmd.chanlist_len;
			async->munge_count += block_size;
			async->munge_ptr += block_size;
			async->munge_ptr %= async->prealloc_bufsz;
			count += block_size;
		}
	}

	return count;
}

<<<<<<< HEAD
unsigned int comedi_buf_write_n_allocated(struct comedi_async *async)
{
	return async->buf_write_alloc_count - async->buf_write_count;
}

/* transfers a chunk from writer to filled buffer space */
unsigned int comedi_buf_write_free(struct comedi_async *async,
				   unsigned int nbytes)
{
	unsigned int allocated = comedi_buf_write_n_allocated(async);
=======
unsigned int comedi_buf_write_n_allocated(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;

	return async->buf_write_alloc_count - async->buf_write_count;
}

/**
 * comedi_buf_write_free() - Free buffer space after it is written
 * @s: COMEDI subdevice.
 * @nbytes: Maximum space to free in bytes.
 *
 * Free up to @nbytes bytes of space previously reserved for writing in the
 * COMEDI acquisition data buffer associated with the subdevice.  The amount of
 * space freed is limited to the amount that was reserved.  The freed space is
 * assumed to have been filled with sample data by the writer.
 *
 * If the samples in the freed space need to be "munged", do so here.  The
 * freed space becomes available for allocation by the reader.
 *
 * Return: The amount of space freed in bytes.
 */
unsigned int comedi_buf_write_free(struct comedi_subdevice *s,
				   unsigned int nbytes)
{
	struct comedi_async *async = s->async;
	unsigned int allocated = comedi_buf_write_n_allocated(s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (nbytes > allocated)
		nbytes = allocated;

	async->buf_write_count += nbytes;
	async->buf_write_ptr += nbytes;
<<<<<<< HEAD
	comedi_buf_munge(async, async->buf_write_count - async->munge_count);
=======
	comedi_buf_munge(s, async->buf_write_count - async->munge_count);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (async->buf_write_ptr >= async->prealloc_bufsz)
		async->buf_write_ptr %= async->prealloc_bufsz;

	return nbytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_write_free);

<<<<<<< HEAD
unsigned int comedi_buf_read_n_available(struct comedi_async *async)
{
	unsigned num_bytes;
=======
/**
 * comedi_buf_read_n_available() - Determine amount of readable buffer space
 * @s: COMEDI subdevice.
 *
 * Determine the amount of readable buffer space in the COMEDI acquisition data
 * buffer associated with the subdevice.  The readable buffer space is that
 * which has been freed by the writer and "munged" to the sample data format
 * expected by COMEDI if necessary.
 *
 * Return: The amount of readable buffer space.
 */
unsigned int comedi_buf_read_n_available(struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned int num_bytes;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (!async)
		return 0;

	num_bytes = async->munge_count - async->buf_read_count;

	/*
	 * ensure the async buffer 'counts' are read before we
	 * attempt to read data from the buffer
	 */
	smp_rmb();

	return num_bytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_read_n_available);

<<<<<<< HEAD
/* allocates a chunk for the reader from filled (and munged) buffer space */
unsigned int comedi_buf_read_alloc(struct comedi_async *async,
				   unsigned int nbytes)
{
=======
/**
 * comedi_buf_read_alloc() - Reserve buffer space for reading
 * @s: COMEDI subdevice.
 * @nbytes: Maximum space to reserve in bytes.
 *
 * Reserve up to @nbytes bytes of previously written and "munged" buffer space
 * for reading in the COMEDI acquisition data buffer associated with the
 * subdevice.  The amount reserved is limited to the space available.  The
 * reader can read from the reserved space and then free it.  A reader is also
 * allowed to read from the space before reserving it as long as it determines
 * the amount of readable data available, but the space needs to be marked as
 * reserved before it can be freed.
 *
 * Return: The amount of space reserved in bytes.
 */
unsigned int comedi_buf_read_alloc(struct comedi_subdevice *s,
				   unsigned int nbytes)
{
	struct comedi_async *async = s->async;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int available;

	available = async->munge_count - async->buf_read_alloc_count;
	if (nbytes > available)
		nbytes = available;

	async->buf_read_alloc_count += nbytes;

	/*
	 * ensure the async buffer 'counts' are read before we
	 * attempt to read data from the read-alloc'ed buffer space
	 */
	smp_rmb();

	return nbytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_read_alloc);

static unsigned int comedi_buf_read_n_allocated(struct comedi_async *async)
{
	return async->buf_read_alloc_count - async->buf_read_count;
}

<<<<<<< HEAD
/* transfers control of a chunk from reader to free buffer space */
unsigned int comedi_buf_read_free(struct comedi_async *async,
				  unsigned int nbytes)
{
=======
/**
 * comedi_buf_read_free() - Free buffer space after it has been read
 * @s: COMEDI subdevice.
 * @nbytes: Maximum space to free in bytes.
 *
 * Free up to @nbytes bytes of buffer space previously reserved for reading in
 * the COMEDI acquisition data buffer associated with the subdevice.  The
 * amount of space freed is limited to the amount that was reserved.
 *
 * The freed space becomes available for allocation by the writer.
 *
 * Return: The amount of space freed in bytes.
 */
unsigned int comedi_buf_read_free(struct comedi_subdevice *s,
				  unsigned int nbytes)
{
	struct comedi_async *async = s->async;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int allocated;

	/*
	 * ensure data has been read out of buffer before
	 * the async read count is incremented
	 */
	smp_mb();

	allocated = comedi_buf_read_n_allocated(async);
	if (nbytes > allocated)
		nbytes = allocated;

	async->buf_read_count += nbytes;
	async->buf_read_ptr += nbytes;
	async->buf_read_ptr %= async->prealloc_bufsz;
	return nbytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_read_free);

<<<<<<< HEAD
int comedi_buf_put(struct comedi_async *async, short x)
{
	unsigned int n = __comedi_buf_write_alloc(async, sizeof(short), 1);

	if (n < sizeof(short)) {
		async->events |= COMEDI_CB_ERROR;
		return 0;
	}
	*(short *)(async->prealloc_buf + async->buf_write_ptr) = x;
	comedi_buf_write_free(async, sizeof(short));
	return 1;
}
EXPORT_SYMBOL_GPL(comedi_buf_put);

int comedi_buf_get(struct comedi_async *async, short *x)
{
	unsigned int n = comedi_buf_read_n_available(async);

	if (n < sizeof(short))
		return 0;
	comedi_buf_read_alloc(async, sizeof(short));
	*x = *(short *)(async->prealloc_buf + async->buf_read_ptr);
	comedi_buf_read_free(async, sizeof(short));
	return 1;
}
EXPORT_SYMBOL_GPL(comedi_buf_get);

void comedi_buf_memcpy_to(struct comedi_async *async, unsigned int offset,
			  const void *data, unsigned int num_bytes)
{
	unsigned int write_ptr = async->buf_write_ptr + offset;

	if (write_ptr >= async->prealloc_bufsz)
		write_ptr %= async->prealloc_bufsz;
=======
static void comedi_buf_memcpy_to(struct comedi_subdevice *s,
				 const void *data, unsigned int num_bytes)
{
	struct comedi_async *async = s->async;
	unsigned int write_ptr = async->buf_write_ptr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	while (num_bytes) {
		unsigned int block_size;

		if (write_ptr + num_bytes > async->prealloc_bufsz)
			block_size = async->prealloc_bufsz - write_ptr;
		else
			block_size = num_bytes;

		memcpy(async->prealloc_buf + write_ptr, data, block_size);

		data += block_size;
		num_bytes -= block_size;

		write_ptr = 0;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(comedi_buf_memcpy_to);

void comedi_buf_memcpy_from(struct comedi_async *async, unsigned int offset,
			    void *dest, unsigned int nbytes)
{
	void *src;
	unsigned int read_ptr = async->buf_read_ptr + offset;

	if (read_ptr >= async->prealloc_bufsz)
		read_ptr %= async->prealloc_bufsz;
=======

static void comedi_buf_memcpy_from(struct comedi_subdevice *s,
				   void *dest, unsigned int nbytes)
{
	void *src;
	struct comedi_async *async = s->async;
	unsigned int read_ptr = async->buf_read_ptr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	while (nbytes) {
		unsigned int block_size;

		src = async->prealloc_buf + read_ptr;

		if (nbytes >= async->prealloc_bufsz - read_ptr)
			block_size = async->prealloc_bufsz - read_ptr;
		else
			block_size = nbytes;

		memcpy(dest, src, block_size);
		nbytes -= block_size;
		dest += block_size;
		read_ptr = 0;
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(comedi_buf_memcpy_from);
=======

/**
 * comedi_buf_write_samples() - Write sample data to COMEDI buffer
 * @s: COMEDI subdevice.
 * @data: Pointer to source samples.
 * @nsamples: Number of samples to write.
 *
 * Write up to @nsamples samples to the COMEDI acquisition data buffer
 * associated with the subdevice, mark it as written and update the
 * acquisition scan progress.  If there is not enough room for the specified
 * number of samples, the number of samples written is limited to the number
 * that will fit and the %COMEDI_CB_OVERFLOW event flag is set to cause the
 * acquisition to terminate with an overrun error.  Set the %COMEDI_CB_BLOCK
 * event flag if any samples are written to cause waiting tasks to be woken
 * when the event flags are processed.
 *
 * Return: The amount of data written in bytes.
 */
unsigned int comedi_buf_write_samples(struct comedi_subdevice *s,
				      const void *data, unsigned int nsamples)
{
	unsigned int max_samples;
	unsigned int nbytes;

	/*
	 * Make sure there is enough room in the buffer for all the samples.
	 * If not, clamp the nsamples to the number that will fit, flag the
	 * buffer overrun and add the samples that fit.
	 */
	max_samples = comedi_bytes_to_samples(s, comedi_buf_write_n_unalloc(s));
	if (nsamples > max_samples) {
		dev_warn(s->device->class_dev, "buffer overrun\n");
		s->async->events |= COMEDI_CB_OVERFLOW;
		nsamples = max_samples;
	}

	if (nsamples == 0)
		return 0;

	nbytes = comedi_buf_write_alloc(s,
					comedi_samples_to_bytes(s, nsamples));
	comedi_buf_memcpy_to(s, data, nbytes);
	comedi_buf_write_free(s, nbytes);
	comedi_inc_scan_progress(s, nbytes);
	s->async->events |= COMEDI_CB_BLOCK;

	return nbytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_write_samples);

/**
 * comedi_buf_read_samples() - Read sample data from COMEDI buffer
 * @s: COMEDI subdevice.
 * @data: Pointer to destination.
 * @nsamples: Maximum number of samples to read.
 *
 * Read up to @nsamples samples from the COMEDI acquisition data buffer
 * associated with the subdevice, mark it as read and update the acquisition
 * scan progress.  Limit the number of samples read to the number available.
 * Set the %COMEDI_CB_BLOCK event flag if any samples are read to cause waiting
 * tasks to be woken when the event flags are processed.
 *
 * Return: The amount of data read in bytes.
 */
unsigned int comedi_buf_read_samples(struct comedi_subdevice *s,
				     void *data, unsigned int nsamples)
{
	unsigned int max_samples;
	unsigned int nbytes;

	/* clamp nsamples to the number of full samples available */
	max_samples = comedi_bytes_to_samples(s,
					      comedi_buf_read_n_available(s));
	if (nsamples > max_samples)
		nsamples = max_samples;

	if (nsamples == 0)
		return 0;

	nbytes = comedi_buf_read_alloc(s,
				       comedi_samples_to_bytes(s, nsamples));
	comedi_buf_memcpy_from(s, data, nbytes);
	comedi_buf_read_free(s, nbytes);
	comedi_inc_scan_progress(s, nbytes);
	s->async->events |= COMEDI_CB_BLOCK;

	return nbytes;
}
EXPORT_SYMBOL_GPL(comedi_buf_read_samples);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
