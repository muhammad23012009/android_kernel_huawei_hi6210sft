/*
 *
 * Copyright (c) 2009, Microsoft Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Authors:
 *   Haiyang Zhang <haiyangz@microsoft.com>
 *   Hank Janssen  <hjanssen@microsoft.com>
 *   K. Y. Srinivasan <kys@microsoft.com>
 *
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/hyperv.h>
<<<<<<< HEAD
=======
#include <linux/uio.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include "hyperv_vmbus.h"

void hv_begin_read(struct hv_ring_buffer_info *rbi)
{
	rbi->ring_buffer->interrupt_mask = 1;
<<<<<<< HEAD
	mb();
=======
	virt_mb();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

u32 hv_end_read(struct hv_ring_buffer_info *rbi)
{
<<<<<<< HEAD
	u32 read;
	u32 write;

	rbi->ring_buffer->interrupt_mask = 0;
	mb();
=======

	rbi->ring_buffer->interrupt_mask = 0;
	virt_mb();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Now check to see if the ring buffer is still empty.
	 * If it is not, we raced and we need to process new
	 * incoming messages.
	 */
<<<<<<< HEAD
	hv_get_ringbuffer_availbytes(rbi, &read, &write);

	return read;
=======
	return hv_get_bytes_to_read(rbi);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * When we write to the ring buffer, check if the host needs to
 * be signaled. Here is the details of this protocol:
 *
 *	1. The host guarantees that while it is draining the
 *	   ring buffer, it will set the interrupt_mask to
 *	   indicate it does not need to be interrupted when
 *	   new data is placed.
 *
 *	2. The host guarantees that it will completely drain
 *	   the ring buffer before exiting the read loop. Further,
 *	   once the ring buffer is empty, it will clear the
 *	   interrupt_mask and re-check to see if new data has
 *	   arrived.
<<<<<<< HEAD
 */

static bool hv_need_to_signal(u32 old_write, struct hv_ring_buffer_info *rbi)
{
	mb();
	if (rbi->ring_buffer->interrupt_mask)
		return false;

=======
 *
 * KYS: Oct. 30, 2016:
 * It looks like Windows hosts have logic to deal with DOS attacks that
 * can be triggered if it receives interrupts when it is not expecting
 * the interrupt. The host expects interrupts only when the ring
 * transitions from empty to non-empty (or full to non full on the guest
 * to host ring).
 * So, base the signaling decision solely on the ring state until the
 * host logic is fixed.
 */

static void hv_signal_on_write(u32 old_write, struct vmbus_channel *channel,
			       bool kick_q)
{
	struct hv_ring_buffer_info *rbi = &channel->outbound;

	virt_mb();
	if (READ_ONCE(rbi->ring_buffer->interrupt_mask))
		return;

	/* check interrupt_mask before read_index */
	virt_rmb();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * This is the only case we need to signal when the
	 * ring transitions from being empty to non-empty.
	 */
<<<<<<< HEAD
	if (old_write == rbi->ring_buffer->read_index)
		return true;

	return false;
}

/*
 * To optimize the flow management on the send-side,
 * when the sender is blocked because of lack of
 * sufficient space in the ring buffer, potential the
 * consumer of the ring buffer can signal the producer.
 * This is controlled by the following parameters:
 *
 * 1. pending_send_sz: This is the size in bytes that the
 *    producer is trying to send.
 * 2. The feature bit feat_pending_send_sz set to indicate if
 *    the consumer of the ring will signal when the ring
 *    state transitions from being full to a state where
 *    there is room for the producer to send the pending packet.
 */

static bool hv_need_to_signal_on_read(u32 old_rd,
					 struct hv_ring_buffer_info *rbi)
{
	u32 prev_write_sz;
	u32 cur_write_sz;
	u32 r_size;
	u32 write_loc = rbi->ring_buffer->write_index;
	u32 read_loc = rbi->ring_buffer->read_index;
	u32 pending_sz = rbi->ring_buffer->pending_send_sz;

	/*
	 * If the other end is not blocked on write don't bother.
	 */
	if (pending_sz == 0)
		return false;

	r_size = rbi->ring_datasize;
	cur_write_sz = write_loc >= read_loc ? r_size - (write_loc - read_loc) :
			read_loc - write_loc;

	prev_write_sz = write_loc >= old_rd ? r_size - (write_loc - old_rd) :
			old_rd - write_loc;


	if ((prev_write_sz < pending_sz) && (cur_write_sz >= pending_sz))
		return true;

	return false;
}

/*
 * hv_get_next_write_location()
 *
 * Get the next write location for the specified ring buffer
 *
 */
=======
	if (old_write == READ_ONCE(rbi->ring_buffer->read_index))
		vmbus_setevent(channel);

	return;
}

/* Get the next write location for the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline u32
hv_get_next_write_location(struct hv_ring_buffer_info *ring_info)
{
	u32 next = ring_info->ring_buffer->write_index;

	return next;
}

<<<<<<< HEAD
/*
 * hv_set_next_write_location()
 *
 * Set the next write location for the specified ring buffer
 *
 */
=======
/* Set the next write location for the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void
hv_set_next_write_location(struct hv_ring_buffer_info *ring_info,
		     u32 next_write_location)
{
	ring_info->ring_buffer->write_index = next_write_location;
}

<<<<<<< HEAD
/*
 * hv_get_next_read_location()
 *
 * Get the next read location for the specified ring buffer
 */
=======
/* Get the next read location for the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline u32
hv_get_next_read_location(struct hv_ring_buffer_info *ring_info)
{
	u32 next = ring_info->ring_buffer->read_index;

	return next;
}

/*
<<<<<<< HEAD
 * hv_get_next_readlocation_withoffset()
 *
 * Get the next read location + offset for the specified ring buffer.
 * This allows the caller to skip
=======
 * Get the next read location + offset for the specified ring buffer.
 * This allows the caller to skip.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static inline u32
hv_get_next_readlocation_withoffset(struct hv_ring_buffer_info *ring_info,
				 u32 offset)
{
	u32 next = ring_info->ring_buffer->read_index;

	next += offset;
	next %= ring_info->ring_datasize;

	return next;
}

<<<<<<< HEAD
/*
 *
 * hv_set_next_read_location()
 *
 * Set the next read location for the specified ring buffer
 *
 */
=======
/* Set the next read location for the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline void
hv_set_next_read_location(struct hv_ring_buffer_info *ring_info,
		    u32 next_read_location)
{
	ring_info->ring_buffer->read_index = next_read_location;
<<<<<<< HEAD
}


/*
 *
 * hv_get_ring_buffer()
 *
 * Get the start of the ring buffer
 */
static inline void *
hv_get_ring_buffer(struct hv_ring_buffer_info *ring_info)
{
	return (void *)ring_info->ring_buffer->buffer;
}


/*
 *
 * hv_get_ring_buffersize()
 *
 * Get the size of the ring buffer
 */
=======
	ring_info->priv_read_index = next_read_location;
}

/* Get the size of the ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline u32
hv_get_ring_buffersize(struct hv_ring_buffer_info *ring_info)
{
	return ring_info->ring_datasize;
}

<<<<<<< HEAD
/*
 *
 * hv_get_ring_bufferindices()
 *
 * Get the read and write indices as u64 of the specified ring buffer
 *
 */
=======
/* Get the read and write indices as u64 of the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline u64
hv_get_ring_bufferindices(struct hv_ring_buffer_info *ring_info)
{
	return (u64)ring_info->ring_buffer->write_index << 32;
}

/*
<<<<<<< HEAD
 *
 * hv_copyfrom_ringbuffer()
 *
 * Helper routine to copy to source from ring buffer.
 * Assume there is enough room. Handles wrap-around in src case only!!
 *
=======
 * Helper routine to copy to source from ring buffer.
 * Assume there is enough room. Handles wrap-around in src case only!!
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static u32 hv_copyfrom_ringbuffer(
	struct hv_ring_buffer_info	*ring_info,
	void				*dest,
	u32				destlen,
	u32				start_read_offset)
{
	void *ring_buffer = hv_get_ring_buffer(ring_info);
	u32 ring_buffer_size = hv_get_ring_buffersize(ring_info);

<<<<<<< HEAD
	u32 frag_len;

	/* wrap-around detected at the src */
	if (destlen > ring_buffer_size - start_read_offset) {
		frag_len = ring_buffer_size - start_read_offset;

		memcpy(dest, ring_buffer + start_read_offset, frag_len);
		memcpy(dest + frag_len, ring_buffer, destlen - frag_len);
	} else

		memcpy(dest, ring_buffer + start_read_offset, destlen);

=======
	memcpy(dest, ring_buffer + start_read_offset, destlen);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	start_read_offset += destlen;
	start_read_offset %= ring_buffer_size;

	return start_read_offset;
}


/*
<<<<<<< HEAD
 *
 * hv_copyto_ringbuffer()
 *
 * Helper routine to copy from source to ring buffer.
 * Assume there is enough room. Handles wrap-around in dest case only!!
 *
=======
 * Helper routine to copy from source to ring buffer.
 * Assume there is enough room. Handles wrap-around in dest case only!!
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static u32 hv_copyto_ringbuffer(
	struct hv_ring_buffer_info	*ring_info,
	u32				start_write_offset,
	void				*src,
	u32				srclen)
{
	void *ring_buffer = hv_get_ring_buffer(ring_info);
	u32 ring_buffer_size = hv_get_ring_buffersize(ring_info);
<<<<<<< HEAD
	u32 frag_len;

	/* wrap-around detected! */
	if (srclen > ring_buffer_size - start_write_offset) {
		frag_len = ring_buffer_size - start_write_offset;
		memcpy(ring_buffer + start_write_offset, src, frag_len);
		memcpy(ring_buffer, src + frag_len, srclen - frag_len);
	} else
		memcpy(ring_buffer + start_write_offset, src, srclen);
=======

	memcpy(ring_buffer + start_write_offset, src, srclen);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	start_write_offset += srclen;
	start_write_offset %= ring_buffer_size;

	return start_write_offset;
}

<<<<<<< HEAD
/*
 *
 * hv_ringbuffer_get_debuginfo()
 *
 * Get various debug metrics for the specified ring buffer
 *
 */
=======
/* Get various debug metrics for the specified ring buffer. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void hv_ringbuffer_get_debuginfo(struct hv_ring_buffer_info *ring_info,
			    struct hv_ring_buffer_debug_info *debug_info)
{
	u32 bytes_avail_towrite;
	u32 bytes_avail_toread;

	if (ring_info->ring_buffer) {
		hv_get_ringbuffer_availbytes(ring_info,
					&bytes_avail_toread,
					&bytes_avail_towrite);

		debug_info->bytes_avail_toread = bytes_avail_toread;
		debug_info->bytes_avail_towrite = bytes_avail_towrite;
		debug_info->current_read_index =
			ring_info->ring_buffer->read_index;
		debug_info->current_write_index =
			ring_info->ring_buffer->write_index;
		debug_info->current_interrupt_mask =
			ring_info->ring_buffer->interrupt_mask;
	}
}

<<<<<<< HEAD
/*
 *
 * hv_ringbuffer_init()
 *
 *Initialize the ring buffer
 *
 */
int hv_ringbuffer_init(struct hv_ring_buffer_info *ring_info,
		   void *buffer, u32 buflen)
{
	if (sizeof(struct hv_ring_buffer) != PAGE_SIZE)
		return -EINVAL;

	memset(ring_info, 0, sizeof(struct hv_ring_buffer_info));

	ring_info->ring_buffer = (struct hv_ring_buffer *)buffer;
	ring_info->ring_buffer->read_index =
		ring_info->ring_buffer->write_index = 0;

	ring_info->ring_size = buflen;
	ring_info->ring_datasize = buflen - sizeof(struct hv_ring_buffer);
=======
/* Initialize the ring buffer. */
int hv_ringbuffer_init(struct hv_ring_buffer_info *ring_info,
		       struct page *pages, u32 page_cnt)
{
	int i;
	struct page **pages_wraparound;

	BUILD_BUG_ON((sizeof(struct hv_ring_buffer) != PAGE_SIZE));

	memset(ring_info, 0, sizeof(struct hv_ring_buffer_info));

	/*
	 * First page holds struct hv_ring_buffer, do wraparound mapping for
	 * the rest.
	 */
	pages_wraparound = kzalloc(sizeof(struct page *) * (page_cnt * 2 - 1),
				   GFP_KERNEL);
	if (!pages_wraparound)
		return -ENOMEM;

	pages_wraparound[0] = pages;
	for (i = 0; i < 2 * (page_cnt - 1); i++)
		pages_wraparound[i + 1] = &pages[i % (page_cnt - 1) + 1];

	ring_info->ring_buffer = (struct hv_ring_buffer *)
		vmap(pages_wraparound, page_cnt * 2 - 1, VM_MAP, PAGE_KERNEL);

	kfree(pages_wraparound);


	if (!ring_info->ring_buffer)
		return -ENOMEM;

	ring_info->ring_buffer->read_index =
		ring_info->ring_buffer->write_index = 0;

	/* Set the feature bit for enabling flow control. */
	ring_info->ring_buffer->feature_bits.value = 1;

	ring_info->ring_size = page_cnt << PAGE_SHIFT;
	ring_info->ring_datasize = ring_info->ring_size -
		sizeof(struct hv_ring_buffer);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_init(&ring_info->ring_lock);

	return 0;
}

<<<<<<< HEAD
/*
 *
 * hv_ringbuffer_cleanup()
 *
 * Cleanup the ring buffer
 *
 */
void hv_ringbuffer_cleanup(struct hv_ring_buffer_info *ring_info)
{
}

/*
 *
 * hv_ringbuffer_write()
 *
 * Write to the ring buffer
 *
 */
int hv_ringbuffer_write(struct hv_ring_buffer_info *outring_info,
		    struct scatterlist *sglist, u32 sgcount, bool *signal)
{
	int i = 0;
	u32 bytes_avail_towrite;
	u32 bytes_avail_toread;
	u32 totalbytes_towrite = 0;

	struct scatterlist *sg;
	u32 next_write_location;
	u32 old_write;
	u64 prev_indices = 0;
	unsigned long flags;

	for_each_sg(sglist, sg, sgcount, i)
	{
		totalbytes_towrite += sg->length;
	}

	totalbytes_towrite += sizeof(u64);

	spin_lock_irqsave(&outring_info->ring_lock, flags);

	hv_get_ringbuffer_availbytes(outring_info,
				&bytes_avail_toread,
				&bytes_avail_towrite);


	/* If there is only room for the packet, assume it is full. */
	/* Otherwise, the next time around, we think the ring buffer */
	/* is empty since the read index == write index */
	if (bytes_avail_towrite <= totalbytes_towrite) {
		spin_unlock_irqrestore(&outring_info->ring_lock, flags);
=======
/* Cleanup the ring buffer. */
void hv_ringbuffer_cleanup(struct hv_ring_buffer_info *ring_info)
{
	vunmap(ring_info->ring_buffer);
}

/* Write to the ring buffer. */
int hv_ringbuffer_write(struct vmbus_channel *channel,
		    struct kvec *kv_list, u32 kv_count, bool lock,
		    bool kick_q)
{
	int i = 0;
	u32 bytes_avail_towrite;
	u32 totalbytes_towrite = 0;

	u32 next_write_location;
	u32 old_write;
	u64 prev_indices = 0;
	unsigned long flags = 0;
	struct hv_ring_buffer_info *outring_info = &channel->outbound;

	if (channel->rescind)
		return -ENODEV;

	for (i = 0; i < kv_count; i++)
		totalbytes_towrite += kv_list[i].iov_len;

	totalbytes_towrite += sizeof(u64);

	if (lock)
		spin_lock_irqsave(&outring_info->ring_lock, flags);

	bytes_avail_towrite = hv_get_bytes_to_write(outring_info);

	/*
	 * If there is only room for the packet, assume it is full.
	 * Otherwise, the next time around, we think the ring buffer
	 * is empty since the read index == write index.
	 */
	if (bytes_avail_towrite <= totalbytes_towrite) {
		if (lock)
			spin_unlock_irqrestore(&outring_info->ring_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EAGAIN;
	}

	/* Write to the ring buffer */
	next_write_location = hv_get_next_write_location(outring_info);

	old_write = next_write_location;

<<<<<<< HEAD
	for_each_sg(sglist, sg, sgcount, i)
	{
		next_write_location = hv_copyto_ringbuffer(outring_info,
						     next_write_location,
						     sg_virt(sg),
						     sg->length);
=======
	for (i = 0; i < kv_count; i++) {
		next_write_location = hv_copyto_ringbuffer(outring_info,
						     next_write_location,
						     kv_list[i].iov_base,
						     kv_list[i].iov_len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* Set previous packet start */
	prev_indices = hv_get_ring_bufferindices(outring_info);

	next_write_location = hv_copyto_ringbuffer(outring_info,
					     next_write_location,
					     &prev_indices,
					     sizeof(u64));

	/* Issue a full memory barrier before updating the write index */
<<<<<<< HEAD
	mb();
=======
	virt_mb();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Now, update the write location */
	hv_set_next_write_location(outring_info, next_write_location);


<<<<<<< HEAD
	spin_unlock_irqrestore(&outring_info->ring_lock, flags);

	*signal = hv_need_to_signal(old_write, outring_info);
	return 0;
}


/*
 *
 * hv_ringbuffer_peek()
 *
 * Read without advancing the read index
 *
 */
int hv_ringbuffer_peek(struct hv_ring_buffer_info *Inring_info,
		   void *Buffer, u32 buflen)
{
	u32 bytes_avail_towrite;
	u32 bytes_avail_toread;
	u32 next_read_location = 0;
	unsigned long flags;

	spin_lock_irqsave(&Inring_info->ring_lock, flags);

	hv_get_ringbuffer_availbytes(Inring_info,
				&bytes_avail_toread,
				&bytes_avail_towrite);

	/* Make sure there is something to read */
	if (bytes_avail_toread < buflen) {

		spin_unlock_irqrestore(&Inring_info->ring_lock, flags);

		return -EAGAIN;
	}

	/* Convert to byte offset */
	next_read_location = hv_get_next_read_location(Inring_info);

	next_read_location = hv_copyfrom_ringbuffer(Inring_info,
						Buffer,
						buflen,
						next_read_location);

	spin_unlock_irqrestore(&Inring_info->ring_lock, flags);
=======
	if (lock)
		spin_unlock_irqrestore(&outring_info->ring_lock, flags);

	hv_signal_on_write(old_write, channel, kick_q);

	if (channel->rescind)
		return -ENODEV;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD

/*
 *
 * hv_ringbuffer_read()
 *
 * Read and advance the read index
 *
 */
int hv_ringbuffer_read(struct hv_ring_buffer_info *inring_info, void *buffer,
		   u32 buflen, u32 offset, bool *signal)
{
	u32 bytes_avail_towrite;
	u32 bytes_avail_toread;
	u32 next_read_location = 0;
	u64 prev_indices = 0;
	unsigned long flags;
	u32 old_read;
=======
int hv_ringbuffer_read(struct vmbus_channel *channel,
		       void *buffer, u32 buflen, u32 *buffer_actual_len,
		       u64 *requestid, bool raw)
{
	u32 bytes_avail_toread;
	u32 next_read_location = 0;
	u64 prev_indices = 0;
	struct vmpacket_descriptor desc;
	u32 offset;
	u32 packetlen;
	int ret = 0;
	struct hv_ring_buffer_info *inring_info = &channel->inbound;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (buflen <= 0)
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&inring_info->ring_lock, flags);

	hv_get_ringbuffer_availbytes(inring_info,
				&bytes_avail_toread,
				&bytes_avail_towrite);

	old_read = bytes_avail_toread;

	/* Make sure there is something to read */
	if (bytes_avail_toread < buflen) {
		spin_unlock_irqrestore(&inring_info->ring_lock, flags);

		return -EAGAIN;
	}
=======

	*buffer_actual_len = 0;
	*requestid = 0;

	bytes_avail_toread = hv_get_bytes_to_read(inring_info);
	/* Make sure there is something to read */
	if (bytes_avail_toread < sizeof(desc)) {
		/*
		 * No error is set when there is even no header, drivers are
		 * supposed to analyze buffer_actual_len.
		 */
		return ret;
	}

	init_cached_read_index(channel);
	next_read_location = hv_get_next_read_location(inring_info);
	next_read_location = hv_copyfrom_ringbuffer(inring_info, &desc,
						    sizeof(desc),
						    next_read_location);

	offset = raw ? 0 : (desc.offset8 << 3);
	packetlen = (desc.len8 << 3) - offset;
	*buffer_actual_len = packetlen;
	*requestid = desc.trans_id;

	if (bytes_avail_toread < packetlen + offset)
		return -EAGAIN;

	if (packetlen > buflen)
		return -ENOBUFS;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	next_read_location =
		hv_get_next_readlocation_withoffset(inring_info, offset);

	next_read_location = hv_copyfrom_ringbuffer(inring_info,
						buffer,
<<<<<<< HEAD
						buflen,
=======
						packetlen,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						next_read_location);

	next_read_location = hv_copyfrom_ringbuffer(inring_info,
						&prev_indices,
						sizeof(u64),
						next_read_location);

<<<<<<< HEAD
	/* Make sure all reads are done before we update the read index since */
	/* the writer may start writing to the read area once the read index */
	/*is updated */
	mb();
=======
	/*
	 * Make sure all reads are done before we update the read index since
	 * the writer may start writing to the read area once the read index
	 * is updated.
	 */
	virt_mb();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Update the read index */
	hv_set_next_read_location(inring_info, next_read_location);

<<<<<<< HEAD
	spin_unlock_irqrestore(&inring_info->ring_lock, flags);

	*signal = hv_need_to_signal_on_read(old_read, inring_info);

	return 0;
=======
	hv_signal_on_read(channel);

	return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
