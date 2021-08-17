/*
 * Back-end of the driver for virtual network devices. This portion of the
 * driver exports a 'unified' network-device interface that can be accessed
 * by any operating system that implements a compatible front end. A
 * reference front-end implementation can be found in:
 *  drivers/net/xen-netfront.c
 *
 * Copyright (c) 2002-2005, K A Fraser
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "common.h"

#include <linux/kthread.h>
#include <linux/if_vlan.h>
#include <linux/udp.h>
<<<<<<< HEAD
=======
#include <linux/highmem.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <net/tcp.h>

#include <xen/xen.h>
#include <xen/events.h>
#include <xen/interface/memory.h>
<<<<<<< HEAD

#include <asm/xen/hypercall.h>
#include <asm/xen/page.h>

/*
 * This is the maximum slots a skb can have. If a guest sends a skb
 * which exceeds this limit it is considered malicious.
 */
#define FATAL_SKB_SLOTS_DEFAULT 20
static unsigned int fatal_skb_slots = FATAL_SKB_SLOTS_DEFAULT;
module_param(fatal_skb_slots, uint, 0444);

/*
 * To avoid confusion, we define XEN_NETBK_LEGACY_SLOTS_MAX indicating
 * the maximum slots a valid packet can use. Now this value is defined
 * to be XEN_NETIF_NR_SLOTS_MIN, which is supposed to be supported by
 * all backend.
 */
#define XEN_NETBK_LEGACY_SLOTS_MAX XEN_NETIF_NR_SLOTS_MIN

typedef unsigned int pending_ring_idx_t;
#define INVALID_PENDING_RING_IDX (~0U)

struct pending_tx_info {
	struct xen_netif_tx_request req; /* coalesced tx request */
	struct xenvif *vif;
	pending_ring_idx_t head; /* head != INVALID_PENDING_RING_IDX
				  * if it is head of one or more tx
				  * reqs
				  */
};

struct netbk_rx_meta {
	int id;
	int size;
	int gso_size;
};

#define MAX_PENDING_REQS 256

/* Discriminate from any valid pending_idx value. */
#define INVALID_PENDING_IDX 0xFFFF

#define MAX_BUFFER_OFFSET PAGE_SIZE

/* extra field used in struct page */
union page_ext {
	struct {
#if BITS_PER_LONG < 64
#define IDX_WIDTH   8
#define GROUP_WIDTH (BITS_PER_LONG - IDX_WIDTH)
		unsigned int group:GROUP_WIDTH;
		unsigned int idx:IDX_WIDTH;
#else
		unsigned int group, idx;
#endif
	} e;
	void *mapping;
};

struct xen_netbk {
	wait_queue_head_t wq;
	struct task_struct *task;

	struct sk_buff_head rx_queue;
	struct sk_buff_head tx_queue;

	struct timer_list net_timer;

	struct page *mmap_pages[MAX_PENDING_REQS];

	pending_ring_idx_t pending_prod;
	pending_ring_idx_t pending_cons;
	struct list_head net_schedule_list;

	/* Protect the net_schedule_list in netif. */
	spinlock_t net_schedule_list_lock;

	atomic_t netfront_count;

	struct pending_tx_info pending_tx_info[MAX_PENDING_REQS];
	/* Coalescing tx requests before copying makes number of grant
	 * copy ops greater or equal to number of slots required. In
	 * worst case a tx request consumes 2 gnttab_copy.
	 */
	struct gnttab_copy tx_copy_ops[2*MAX_PENDING_REQS];

	u16 pending_ring[MAX_PENDING_REQS];

	/*
	 * Given MAX_BUFFER_OFFSET of 4096 the worst case is that each
	 * head/fragment page uses 2 copy operations because it
	 * straddles two buffers in the frontend.
	 */
	struct gnttab_copy grant_copy_op[2*XEN_NETIF_RX_RING_SIZE];
	struct netbk_rx_meta meta[2*XEN_NETIF_RX_RING_SIZE];
};

static struct xen_netbk *xen_netbk;
static int xen_netbk_group_nr;

/*
 * If head != INVALID_PENDING_RING_IDX, it means this tx request is head of
 * one or more merged tx requests, otherwise it is the continuation of
 * previous tx request.
 */
static inline int pending_tx_is_head(struct xen_netbk *netbk, RING_IDX idx)
{
	return netbk->pending_tx_info[idx].head != INVALID_PENDING_RING_IDX;
}

void xen_netbk_add_xenvif(struct xenvif *vif)
{
	int i;
	int min_netfront_count;
	int min_group = 0;
	struct xen_netbk *netbk;

	min_netfront_count = atomic_read(&xen_netbk[0].netfront_count);
	for (i = 0; i < xen_netbk_group_nr; i++) {
		int netfront_count = atomic_read(&xen_netbk[i].netfront_count);
		if (netfront_count < min_netfront_count) {
			min_group = i;
			min_netfront_count = netfront_count;
		}
	}

	netbk = &xen_netbk[min_group];

	vif->netbk = netbk;
	atomic_inc(&netbk->netfront_count);
}

void xen_netbk_remove_xenvif(struct xenvif *vif)
{
	struct xen_netbk *netbk = vif->netbk;
	vif->netbk = NULL;
	atomic_dec(&netbk->netfront_count);
}

static void xen_netbk_idx_release(struct xen_netbk *netbk, u16 pending_idx,
				  u8 status);
static void make_tx_response(struct xenvif *vif,
			     struct xen_netif_tx_request *txp,
			     s8       st);
static struct xen_netif_rx_response *make_rx_response(struct xenvif *vif,
					     u16      id,
					     s8       st,
					     u16      offset,
					     u16      size,
					     u16      flags);

static inline unsigned long idx_to_pfn(struct xen_netbk *netbk,
				       u16 idx)
{
	return page_to_pfn(netbk->mmap_pages[idx]);
}

static inline unsigned long idx_to_kaddr(struct xen_netbk *netbk,
					 u16 idx)
{
	return (unsigned long)pfn_to_kaddr(idx_to_pfn(netbk, idx));
}

/* extra field used in struct page */
static inline void set_page_ext(struct page *pg, struct xen_netbk *netbk,
				unsigned int idx)
{
	unsigned int group = netbk - xen_netbk;
	union page_ext ext = { .e = { .group = group + 1, .idx = idx } };

	BUILD_BUG_ON(sizeof(ext) > sizeof(ext.mapping));
	pg->mapping = ext.mapping;
}

static int get_page_ext(struct page *pg,
			unsigned int *pgroup, unsigned int *pidx)
{
	union page_ext ext = { .mapping = pg->mapping };
	struct xen_netbk *netbk;
	unsigned int group, idx;

	group = ext.e.group - 1;

	if (group < 0 || group >= xen_netbk_group_nr)
		return 0;

	netbk = &xen_netbk[group];

	idx = ext.e.idx;

	if ((idx < 0) || (idx >= MAX_PENDING_REQS))
		return 0;

	if (netbk->mmap_pages[idx] != pg)
		return 0;

	*pgroup = group;
	*pidx = idx;

	return 1;
}

/*
 * This is the amount of packet we copy rather than map, so that the
 * guest can't fiddle with the contents of the headers while we do
 * packet processing on them (netfilter, routing, etc).
 */
#define PKT_PROT_LEN    (ETH_HLEN + \
			 VLAN_HLEN + \
			 sizeof(struct iphdr) + MAX_IPOPTLEN + \
			 sizeof(struct tcphdr) + MAX_TCP_OPTION_SPACE)

=======
#include <xen/page.h>

#include <asm/xen/hypercall.h>

/* Provide an option to disable split event channels at load time as
 * event channels are limited resource. Split event channels are
 * enabled by default.
 */
bool separate_tx_rx_irq = true;
module_param(separate_tx_rx_irq, bool, 0644);

/* The time that packets can stay on the guest Rx internal queue
 * before they are dropped.
 */
unsigned int rx_drain_timeout_msecs = 10000;
module_param(rx_drain_timeout_msecs, uint, 0444);

/* The length of time before the frontend is considered unresponsive
 * because it isn't providing Rx slots.
 */
unsigned int rx_stall_timeout_msecs = 60000;
module_param(rx_stall_timeout_msecs, uint, 0444);

#define MAX_QUEUES_DEFAULT 8
unsigned int xenvif_max_queues;
module_param_named(max_queues, xenvif_max_queues, uint, 0644);
MODULE_PARM_DESC(max_queues,
		 "Maximum number of queues per virtual interface");

/*
 * This is the maximum slots a skb can have. If a guest sends a skb
 * which exceeds this limit it is considered malicious.
 */
#define FATAL_SKB_SLOTS_DEFAULT 20
static unsigned int fatal_skb_slots = FATAL_SKB_SLOTS_DEFAULT;
module_param(fatal_skb_slots, uint, 0444);

/* The amount to copy out of the first guest Tx slot into the skb's
 * linear area.  If the first slot has more data, it will be mapped
 * and put into the first frag.
 *
 * This is sized to avoid pulling headers from the frags for most
 * TCP/IP packets.
 */
#define XEN_NETBACK_TX_COPY_LEN 128

/* This is the maximum number of flows in the hash cache. */
#define XENVIF_HASH_CACHE_SIZE_DEFAULT 64
unsigned int xenvif_hash_cache_size = XENVIF_HASH_CACHE_SIZE_DEFAULT;
module_param_named(hash_cache_size, xenvif_hash_cache_size, uint, 0644);
MODULE_PARM_DESC(hash_cache_size, "Number of flows in the hash cache");

static void xenvif_idx_release(struct xenvif_queue *queue, u16 pending_idx,
			       u8 status);

static void make_tx_response(struct xenvif_queue *queue,
			     struct xen_netif_tx_request *txp,
			     unsigned int extra_count,
			     s8       st);
static void push_tx_responses(struct xenvif_queue *queue);

static inline int tx_work_todo(struct xenvif_queue *queue);

static inline unsigned long idx_to_pfn(struct xenvif_queue *queue,
				       u16 idx)
{
	return page_to_pfn(queue->mmap_pages[idx]);
}

static inline unsigned long idx_to_kaddr(struct xenvif_queue *queue,
					 u16 idx)
{
	return (unsigned long)pfn_to_kaddr(idx_to_pfn(queue, idx));
}

#define callback_param(vif, pending_idx) \
	(vif->pending_tx_info[pending_idx].callback_struct)

/* Find the containing VIF's structure from a pointer in pending_tx_info array
 */
static inline struct xenvif_queue *ubuf_to_queue(const struct ubuf_info *ubuf)
{
	u16 pending_idx = ubuf->desc;
	struct pending_tx_info *temp =
		container_of(ubuf, struct pending_tx_info, callback_struct);
	return container_of(temp - pending_idx,
			    struct xenvif_queue,
			    pending_tx_info[0]);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static u16 frag_get_pending_idx(skb_frag_t *frag)
{
	return (u16)frag->page_offset;
}

static void frag_set_pending_idx(skb_frag_t *frag, u16 pending_idx)
{
	frag->page_offset = pending_idx;
}

static inline pending_ring_idx_t pending_index(unsigned i)
{
	return i & (MAX_PENDING_REQS-1);
}

<<<<<<< HEAD
static inline pending_ring_idx_t nr_pending_reqs(struct xen_netbk *netbk)
{
	return MAX_PENDING_REQS -
		netbk->pending_prod + netbk->pending_cons;
}

static void xen_netbk_kick_thread(struct xen_netbk *netbk)
{
	wake_up(&netbk->wq);
}

static int max_required_rx_slots(struct xenvif *vif)
{
	int max = DIV_ROUND_UP(vif->dev->mtu, PAGE_SIZE);

	/* XXX FIXME: RX path dependent on MAX_SKB_FRAGS */
	if (vif->can_sg || vif->gso || vif->gso_prefix)
		max += MAX_SKB_FRAGS + 1; /* extra_info + frags */

	return max;
}

int xen_netbk_rx_ring_full(struct xenvif *vif)
{
	RING_IDX peek   = vif->rx_req_cons_peek;
	RING_IDX needed = max_required_rx_slots(vif);

	return ((vif->rx.sring->req_prod - peek) < needed) ||
	       ((vif->rx.rsp_prod_pvt + XEN_NETIF_RX_RING_SIZE - peek) < needed);
}

int xen_netbk_must_stop_queue(struct xenvif *vif)
{
	if (!xen_netbk_rx_ring_full(vif))
		return 0;

	vif->rx.sring->req_event = vif->rx_req_cons_peek +
		max_required_rx_slots(vif);
	mb(); /* request notification /then/ check the queue */

	return xen_netbk_rx_ring_full(vif);
}

/*
 * Returns true if we should start a new receive buffer instead of
 * adding 'size' bytes to a buffer which currently contains 'offset'
 * bytes.
 */
static bool start_new_rx_buffer(int offset, unsigned long size, int head)
{
	/* simple case: we have completely filled the current buffer. */
	if (offset == MAX_BUFFER_OFFSET)
		return true;

	/*
	 * complex case: start a fresh buffer if the current frag
	 * would overflow the current buffer but only if:
	 *     (i)   this frag would fit completely in the next buffer
	 * and (ii)  there is already some data in the current buffer
	 * and (iii) this is not the head buffer.
	 *
	 * Where:
	 * - (i) stops us splitting a frag into two copies
	 *   unless the frag is too large for a single buffer.
	 * - (ii) stops us from leaving a buffer pointlessly empty.
	 * - (iii) stops us leaving the first buffer
	 *   empty. Strictly speaking this is already covered
	 *   by (ii) but is explicitly checked because
	 *   netfront relies on the first buffer being
	 *   non-empty and can crash otherwise.
	 *
	 * This means we will effectively linearise small
	 * frags but do not needlessly split large buffers
	 * into multiple copies tend to give large frags their
	 * own buffers as before.
	 */
	BUG_ON(size > MAX_BUFFER_OFFSET);
	if ((offset + size > MAX_BUFFER_OFFSET) && offset && !head)
		return true;

	return false;
}

struct xenvif_count_slot_state {
	unsigned long copy_off;
	bool head;
};

unsigned int xenvif_count_frag_slots(struct xenvif *vif,
				     unsigned long offset, unsigned long size,
				     struct xenvif_count_slot_state *state)
{
	unsigned count = 0;

	offset &= ~PAGE_MASK;

	while (size > 0) {
		unsigned long bytes;

		bytes = PAGE_SIZE - offset;

		if (bytes > size)
			bytes = size;

		if (start_new_rx_buffer(state->copy_off, bytes, state->head)) {
			count++;
			state->copy_off = 0;
		}

		if (state->copy_off + bytes > MAX_BUFFER_OFFSET)
			bytes = MAX_BUFFER_OFFSET - state->copy_off;

		state->copy_off += bytes;

		offset += bytes;
		size -= bytes;

		if (offset == PAGE_SIZE)
			offset = 0;

		state->head = false;
	}

	return count;
}

/*
 * Figure out how many ring slots we're going to need to send @skb to
 * the guest. This function is essentially a dry run of
 * netbk_gop_frag_copy.
 */
unsigned int xen_netbk_count_skb_slots(struct xenvif *vif, struct sk_buff *skb)
{
	struct xenvif_count_slot_state state;
	unsigned int count;
	unsigned char *data;
	unsigned i;

	state.head = true;
	state.copy_off = 0;

	/* Slot for the first (partial) page of data. */
	count = 1;

	/* Need a slot for the GSO prefix for GSO extra data? */
	if (skb_shinfo(skb)->gso_size)
		count++;

	data = skb->data;
	while (data < skb_tail_pointer(skb)) {
		unsigned long offset = offset_in_page(data);
		unsigned long size = PAGE_SIZE - offset;

		if (data + size > skb_tail_pointer(skb))
			size = skb_tail_pointer(skb) - data;

		count += xenvif_count_frag_slots(vif, offset, size, &state);

		data += size;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		unsigned long size = skb_frag_size(&skb_shinfo(skb)->frags[i]);
		unsigned long offset = skb_shinfo(skb)->frags[i].page_offset;

		count += xenvif_count_frag_slots(vif, offset, size, &state);
	}
	return count;
}

struct netrx_pending_operations {
	unsigned copy_prod, copy_cons;
	unsigned meta_prod, meta_cons;
	struct gnttab_copy *copy;
	struct netbk_rx_meta *meta;
	int copy_off;
	grant_ref_t copy_gref;
};

static struct netbk_rx_meta *get_next_rx_buffer(struct xenvif *vif,
						struct netrx_pending_operations *npo)
{
	struct netbk_rx_meta *meta;
	struct xen_netif_rx_request req;

	RING_COPY_REQUEST(&vif->rx, vif->rx.req_cons++, &req);

	meta = npo->meta + npo->meta_prod++;
	meta->gso_size = 0;
	meta->size = 0;
	meta->id = req.id;

	npo->copy_off = 0;
	npo->copy_gref = req.gref;

	return meta;
}

/*
 * Set up the grant operations for this fragment. If it's a flipping
 * interface, we also set up the unmap request from here.
 */
static void netbk_gop_frag_copy(struct xenvif *vif, struct sk_buff *skb,
				struct netrx_pending_operations *npo,
				struct page *page, unsigned long size,
				unsigned long offset, int *head)
{
	struct gnttab_copy *copy_gop;
	struct netbk_rx_meta *meta;
	/*
	 * These variables are used iff get_page_ext returns true,
	 * in which case they are guaranteed to be initialized.
	 */
	unsigned int uninitialized_var(group), uninitialized_var(idx);
	int foreign = get_page_ext(page, &group, &idx);
	unsigned long bytes;

	/* Data must not cross a page boundary. */
	BUG_ON(size + offset > PAGE_SIZE<<compound_order(page));

	meta = npo->meta + npo->meta_prod - 1;

	/* Skip unused frames from start of page */
	page += offset >> PAGE_SHIFT;
	offset &= ~PAGE_MASK;

	while (size > 0) {
		BUG_ON(offset >= PAGE_SIZE);
		BUG_ON(npo->copy_off > MAX_BUFFER_OFFSET);

		bytes = PAGE_SIZE - offset;

		if (bytes > size)
			bytes = size;

		if (start_new_rx_buffer(npo->copy_off, bytes, *head)) {
			/*
			 * Netfront requires there to be some data in the head
			 * buffer.
			 */
			BUG_ON(*head);

			meta = get_next_rx_buffer(vif, npo);
		}

		if (npo->copy_off + bytes > MAX_BUFFER_OFFSET)
			bytes = MAX_BUFFER_OFFSET - npo->copy_off;

		copy_gop = npo->copy + npo->copy_prod++;
		copy_gop->flags = GNTCOPY_dest_gref;
		if (foreign) {
			struct xen_netbk *netbk = &xen_netbk[group];
			struct pending_tx_info *src_pend;

			src_pend = &netbk->pending_tx_info[idx];

			copy_gop->source.domid = src_pend->vif->domid;
			copy_gop->source.u.ref = src_pend->req.gref;
			copy_gop->flags |= GNTCOPY_source_gref;
		} else {
			void *vaddr = page_address(page);
			copy_gop->source.domid = DOMID_SELF;
			copy_gop->source.u.gmfn = virt_to_mfn(vaddr);
		}
		copy_gop->source.offset = offset;
		copy_gop->dest.domid = vif->domid;

		copy_gop->dest.offset = npo->copy_off;
		copy_gop->dest.u.ref = npo->copy_gref;
		copy_gop->len = bytes;

		npo->copy_off += bytes;
		meta->size += bytes;

		offset += bytes;
		size -= bytes;

		/* Next frame */
		if (offset == PAGE_SIZE && size) {
			BUG_ON(!PageCompound(page));
			page++;
			offset = 0;
		}

		/* Leave a gap for the GSO descriptor. */
		if (*head && skb_shinfo(skb)->gso_size && !vif->gso_prefix)
			vif->rx.req_cons++;

		*head = 0; /* There must be something in this buffer now. */

	}
}

/*
 * Prepare an SKB to be transmitted to the frontend.
 *
 * This function is responsible for allocating grant operations, meta
 * structures, etc.
 *
 * It returns the number of meta structures consumed. The number of
 * ring slots used is always equal to the number of meta slots used
 * plus the number of GSO descriptors used. Currently, we use either
 * zero GSO descriptors (for non-GSO packets) or one descriptor (for
 * frontend-side LRO).
 */
static int netbk_gop_skb(struct sk_buff *skb,
			 struct netrx_pending_operations *npo)
{
	struct xenvif *vif = netdev_priv(skb->dev);
	int nr_frags = skb_shinfo(skb)->nr_frags;
	int i;
	struct xen_netif_rx_request req;
	struct netbk_rx_meta *meta;
	unsigned char *data;
	int head = 1;
	int old_meta_prod;

	old_meta_prod = npo->meta_prod;

	/* Set up a GSO prefix descriptor, if necessary */
	if (skb_shinfo(skb)->gso_size && vif->gso_prefix) {
		RING_COPY_REQUEST(&vif->rx, vif->rx.req_cons++, &req);
		meta = npo->meta + npo->meta_prod++;
		meta->gso_size = skb_shinfo(skb)->gso_size;
		meta->size = 0;
		meta->id = req.id;
	}

	RING_COPY_REQUEST(&vif->rx, vif->rx.req_cons++, &req);
	meta = npo->meta + npo->meta_prod++;

	if (!vif->gso_prefix)
		meta->gso_size = skb_shinfo(skb)->gso_size;
	else
		meta->gso_size = 0;

	meta->size = 0;
	meta->id = req.id;
	npo->copy_off = 0;
	npo->copy_gref = req.gref;

	data = skb->data;
	while (data < skb_tail_pointer(skb)) {
		unsigned int offset = offset_in_page(data);
		unsigned int len = PAGE_SIZE - offset;

		if (data + len > skb_tail_pointer(skb))
			len = skb_tail_pointer(skb) - data;

		netbk_gop_frag_copy(vif, skb, npo,
				    virt_to_page(data), len, offset, &head);
		data += len;
	}

	for (i = 0; i < nr_frags; i++) {
		netbk_gop_frag_copy(vif, skb, npo,
				    skb_frag_page(&skb_shinfo(skb)->frags[i]),
				    skb_frag_size(&skb_shinfo(skb)->frags[i]),
				    skb_shinfo(skb)->frags[i].page_offset,
				    &head);
	}

	return npo->meta_prod - old_meta_prod;
}

/*
 * This is a twin to netbk_gop_skb.  Assume that netbk_gop_skb was
 * used to set up the operations on the top of
 * netrx_pending_operations, which have since been done.  Check that
 * they didn't give any errors and advance over them.
 */
static int netbk_check_gop(struct xenvif *vif, int nr_meta_slots,
			   struct netrx_pending_operations *npo)
{
	struct gnttab_copy     *copy_op;
	int status = XEN_NETIF_RSP_OKAY;
	int i;

	for (i = 0; i < nr_meta_slots; i++) {
		copy_op = npo->copy + npo->copy_cons++;
		if (copy_op->status != GNTST_okay) {
			netdev_dbg(vif->dev,
				   "Bad status %d from copy to DOM%d.\n",
				   copy_op->status, vif->domid);
			status = XEN_NETIF_RSP_ERROR;
		}
	}

	return status;
}

static void netbk_add_frag_responses(struct xenvif *vif, int status,
				     struct netbk_rx_meta *meta,
				     int nr_meta_slots)
{
	int i;
	unsigned long offset;

	/* No fragments used */
	if (nr_meta_slots <= 1)
		return;

	nr_meta_slots--;

	for (i = 0; i < nr_meta_slots; i++) {
		int flags;
		if (i == nr_meta_slots - 1)
			flags = 0;
		else
			flags = XEN_NETRXF_more_data;

		offset = 0;
		make_rx_response(vif, meta[i].id, status, offset,
				 meta[i].size, flags);
	}
}

struct skb_cb_overlay {
	int meta_slots_used;
};

static void xen_netbk_rx_action(struct xen_netbk *netbk)
{
	struct xenvif *vif = NULL, *tmp;
	s8 status;
	u16 flags;
	struct xen_netif_rx_response *resp;
	struct sk_buff_head rxq;
	struct sk_buff *skb;
	LIST_HEAD(notify);
	int ret;
	int nr_frags;
	int count;
	unsigned long offset;
	struct skb_cb_overlay *sco;

	struct netrx_pending_operations npo = {
		.copy  = netbk->grant_copy_op,
		.meta  = netbk->meta,
	};

	skb_queue_head_init(&rxq);

	count = 0;

	while ((skb = skb_dequeue(&netbk->rx_queue)) != NULL) {
		vif = netdev_priv(skb->dev);
		nr_frags = skb_shinfo(skb)->nr_frags;

		sco = (struct skb_cb_overlay *)skb->cb;
		sco->meta_slots_used = netbk_gop_skb(skb, &npo);

		count += nr_frags + 1;

		__skb_queue_tail(&rxq, skb);

		/* Filled the batch queue? */
		/* XXX FIXME: RX path dependent on MAX_SKB_FRAGS */
		if (count + MAX_SKB_FRAGS >= XEN_NETIF_RX_RING_SIZE)
			break;
	}

	BUG_ON(npo.meta_prod > ARRAY_SIZE(netbk->meta));

	if (!npo.copy_prod)
		return;

	BUG_ON(npo.copy_prod > ARRAY_SIZE(netbk->grant_copy_op));
	gnttab_batch_copy(netbk->grant_copy_op, npo.copy_prod);

	while ((skb = __skb_dequeue(&rxq)) != NULL) {
		sco = (struct skb_cb_overlay *)skb->cb;

		vif = netdev_priv(skb->dev);

		if (netbk->meta[npo.meta_cons].gso_size && vif->gso_prefix) {
			resp = RING_GET_RESPONSE(&vif->rx,
						vif->rx.rsp_prod_pvt++);

			resp->flags = XEN_NETRXF_gso_prefix | XEN_NETRXF_more_data;

			resp->offset = netbk->meta[npo.meta_cons].gso_size;
			resp->id = netbk->meta[npo.meta_cons].id;
			resp->status = sco->meta_slots_used;

			npo.meta_cons++;
			sco->meta_slots_used--;
		}


		vif->dev->stats.tx_bytes += skb->len;
		vif->dev->stats.tx_packets++;

		status = netbk_check_gop(vif, sco->meta_slots_used, &npo);

		if (sco->meta_slots_used == 1)
			flags = 0;
		else
			flags = XEN_NETRXF_more_data;

		if (skb->ip_summed == CHECKSUM_PARTIAL) /* local packet? */
			flags |= XEN_NETRXF_csum_blank | XEN_NETRXF_data_validated;
		else if (skb->ip_summed == CHECKSUM_UNNECESSARY)
			/* remote but checksummed. */
			flags |= XEN_NETRXF_data_validated;

		offset = 0;
		resp = make_rx_response(vif, netbk->meta[npo.meta_cons].id,
					status, offset,
					netbk->meta[npo.meta_cons].size,
					flags);

		if (netbk->meta[npo.meta_cons].gso_size && !vif->gso_prefix) {
			struct xen_netif_extra_info *gso =
				(struct xen_netif_extra_info *)
				RING_GET_RESPONSE(&vif->rx,
						  vif->rx.rsp_prod_pvt++);

			resp->flags |= XEN_NETRXF_extra_info;

			gso->u.gso.size = netbk->meta[npo.meta_cons].gso_size;
			gso->u.gso.type = XEN_NETIF_GSO_TYPE_TCPV4;
			gso->u.gso.pad = 0;
			gso->u.gso.features = 0;

			gso->type = XEN_NETIF_EXTRA_TYPE_GSO;
			gso->flags = 0;
		}

		netbk_add_frag_responses(vif, status,
					 netbk->meta + npo.meta_cons + 1,
					 sco->meta_slots_used);

		RING_PUSH_RESPONSES_AND_CHECK_NOTIFY(&vif->rx, ret);

		xenvif_notify_tx_completion(vif);

		if (ret && list_empty(&vif->notify_list))
			list_add_tail(&vif->notify_list, &notify);
		else
			xenvif_put(vif);
		npo.meta_cons += sco->meta_slots_used;
		dev_kfree_skb(skb);
		xenvif_put_rings(vif);
	}

	list_for_each_entry_safe(vif, tmp, &notify, notify_list) {
		notify_remote_via_irq(vif->irq);
		list_del_init(&vif->notify_list);
		xenvif_put(vif);
	}

	/* More work to do? */
	if (!skb_queue_empty(&netbk->rx_queue) &&
			!timer_pending(&netbk->net_timer))
		xen_netbk_kick_thread(netbk);
}

void xen_netbk_queue_tx_skb(struct xenvif *vif, struct sk_buff *skb)
{
	struct xen_netbk *netbk = vif->netbk;

	skb_queue_tail(&netbk->rx_queue, skb);

	xen_netbk_kick_thread(netbk);
}

static void xen_netbk_alarm(unsigned long data)
{
	struct xen_netbk *netbk = (struct xen_netbk *)data;
	xen_netbk_kick_thread(netbk);
}

static int __on_net_schedule_list(struct xenvif *vif)
{
	return !list_empty(&vif->schedule_list);
}

/* Must be called with net_schedule_list_lock held */
static void remove_from_net_schedule_list(struct xenvif *vif)
{
	if (likely(__on_net_schedule_list(vif))) {
		list_del_init(&vif->schedule_list);
		xenvif_put(vif);
	}
}

static struct xenvif *poll_net_schedule_list(struct xen_netbk *netbk)
{
	struct xenvif *vif = NULL;

	spin_lock_irq(&netbk->net_schedule_list_lock);
	if (list_empty(&netbk->net_schedule_list))
		goto out;

	vif = list_first_entry(&netbk->net_schedule_list,
			       struct xenvif, schedule_list);
	if (!vif)
		goto out;

	xenvif_get(vif);

	remove_from_net_schedule_list(vif);
out:
	spin_unlock_irq(&netbk->net_schedule_list_lock);
	return vif;
}

void xen_netbk_schedule_xenvif(struct xenvif *vif)
{
	unsigned long flags;
	struct xen_netbk *netbk = vif->netbk;

	if (__on_net_schedule_list(vif))
		goto kick;

	spin_lock_irqsave(&netbk->net_schedule_list_lock, flags);
	if (!__on_net_schedule_list(vif) &&
	    likely(xenvif_schedulable(vif))) {
		list_add_tail(&vif->schedule_list, &netbk->net_schedule_list);
		xenvif_get(vif);
	}
	spin_unlock_irqrestore(&netbk->net_schedule_list_lock, flags);

kick:
	smp_mb();
	if ((nr_pending_reqs(netbk) < (MAX_PENDING_REQS/2)) &&
	    !list_empty(&netbk->net_schedule_list))
		xen_netbk_kick_thread(netbk);
}

void xen_netbk_deschedule_xenvif(struct xenvif *vif)
{
	struct xen_netbk *netbk = vif->netbk;
	spin_lock_irq(&netbk->net_schedule_list_lock);
	remove_from_net_schedule_list(vif);
	spin_unlock_irq(&netbk->net_schedule_list_lock);
}

void xen_netbk_check_rx_xenvif(struct xenvif *vif)
{
	int more_to_do;

	RING_FINAL_CHECK_FOR_REQUESTS(&vif->tx, more_to_do);

	if (more_to_do)
		xen_netbk_schedule_xenvif(vif);
}

static void tx_add_credit(struct xenvif *vif)
=======
void xenvif_kick_thread(struct xenvif_queue *queue)
{
	wake_up(&queue->wq);
}

void xenvif_napi_schedule_or_enable_events(struct xenvif_queue *queue)
{
	int more_to_do;

	RING_FINAL_CHECK_FOR_REQUESTS(&queue->tx, more_to_do);

	if (more_to_do)
		napi_schedule(&queue->napi);
	else if (atomic_fetch_andnot(NETBK_TX_EOI | NETBK_COMMON_EOI,
				     &queue->eoi_pending) &
		 (NETBK_TX_EOI | NETBK_COMMON_EOI))
		xen_irq_lateeoi(queue->tx_irq, 0);
}

static void tx_add_credit(struct xenvif_queue *queue)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long max_burst, max_credit;

	/*
	 * Allow a burst big enough to transmit a jumbo packet of up to 128kB.
	 * Otherwise the interface can seize up due to insufficient credit.
	 */
<<<<<<< HEAD
	max_burst = max(131072UL, vif->credit_bytes);

	/* Take care that adding a new chunk of credit doesn't wrap to zero. */
	max_credit = vif->remaining_credit + vif->credit_bytes;
	if (max_credit < vif->remaining_credit)
		max_credit = ULONG_MAX; /* wrapped: clamp to ULONG_MAX */

	vif->remaining_credit = min(max_credit, max_burst);
}

static void tx_credit_callback(unsigned long data)
{
	struct xenvif *vif = (struct xenvif *)data;
	tx_add_credit(vif);
	xen_netbk_check_rx_xenvif(vif);
}

static void netbk_tx_err(struct xenvif *vif,
			 struct xen_netif_tx_request *txp, RING_IDX end)
{
	RING_IDX cons = vif->tx.req_cons;

	do {
		make_tx_response(vif, txp, XEN_NETIF_RSP_ERROR);
		if (cons == end)
			break;
		RING_COPY_REQUEST(&vif->tx, cons++, txp);
	} while (1);
	vif->tx.req_cons = cons;
	xen_netbk_check_rx_xenvif(vif);
	xenvif_put(vif);
}

static void netbk_fatal_tx_err(struct xenvif *vif)
{
	netdev_err(vif->dev, "fatal error; disabling device\n");
	xenvif_carrier_off(vif);
	xenvif_put(vif);
}

static int netbk_count_requests(struct xenvif *vif,
				struct xen_netif_tx_request *first,
				struct xen_netif_tx_request *txp,
				int work_to_do)
{
	RING_IDX cons = vif->tx.req_cons;
=======
	max_burst = max(131072UL, queue->credit_bytes);

	/* Take care that adding a new chunk of credit doesn't wrap to zero. */
	max_credit = queue->remaining_credit + queue->credit_bytes;
	if (max_credit < queue->remaining_credit)
		max_credit = ULONG_MAX; /* wrapped: clamp to ULONG_MAX */

	queue->remaining_credit = min(max_credit, max_burst);
	queue->rate_limited = false;
}

void xenvif_tx_credit_callback(unsigned long data)
{
	struct xenvif_queue *queue = (struct xenvif_queue *)data;
	tx_add_credit(queue);
	xenvif_napi_schedule_or_enable_events(queue);
}

static void xenvif_tx_err(struct xenvif_queue *queue,
			  struct xen_netif_tx_request *txp,
			  unsigned int extra_count, RING_IDX end)
{
	RING_IDX cons = queue->tx.req_cons;
	unsigned long flags;

	do {
		spin_lock_irqsave(&queue->response_lock, flags);
		make_tx_response(queue, txp, extra_count, XEN_NETIF_RSP_ERROR);
		push_tx_responses(queue);
		spin_unlock_irqrestore(&queue->response_lock, flags);
		if (cons == end)
			break;
		RING_COPY_REQUEST(&queue->tx, cons++, txp);
		extra_count = 0; /* only the first frag can have extras */
	} while (1);
	queue->tx.req_cons = cons;
}

static void xenvif_fatal_tx_err(struct xenvif *vif)
{
	netdev_err(vif->dev, "fatal error; disabling device\n");
	vif->disabled = true;
	/* Disable the vif from queue 0's kthread */
	if (vif->queues)
		xenvif_kick_thread(&vif->queues[0]);
}

static int xenvif_count_requests(struct xenvif_queue *queue,
				 struct xen_netif_tx_request *first,
				 unsigned int extra_count,
				 struct xen_netif_tx_request *txp,
				 int work_to_do)
{
	RING_IDX cons = queue->tx.req_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int slots = 0;
	int drop_err = 0;
	int more_data;

	if (!(first->flags & XEN_NETTXF_more_data))
		return 0;

	do {
		struct xen_netif_tx_request dropped_tx = { 0 };

		if (slots >= work_to_do) {
<<<<<<< HEAD
			netdev_err(vif->dev,
				   "Asked for %d slots but exceeds this limit\n",
				   work_to_do);
			netbk_fatal_tx_err(vif);
=======
			netdev_err(queue->vif->dev,
				   "Asked for %d slots but exceeds this limit\n",
				   work_to_do);
			xenvif_fatal_tx_err(queue->vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -ENODATA;
		}

		/* This guest is really using too many slots and
		 * considered malicious.
		 */
		if (unlikely(slots >= fatal_skb_slots)) {
<<<<<<< HEAD
			netdev_err(vif->dev,
				   "Malicious frontend using %d slots, threshold %u\n",
				   slots, fatal_skb_slots);
			netbk_fatal_tx_err(vif);
=======
			netdev_err(queue->vif->dev,
				   "Malicious frontend using %d slots, threshold %u\n",
				   slots, fatal_skb_slots);
			xenvif_fatal_tx_err(queue->vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -E2BIG;
		}

		/* Xen network protocol had implicit dependency on
		 * MAX_SKB_FRAGS. XEN_NETBK_LEGACY_SLOTS_MAX is set to
		 * the historical MAX_SKB_FRAGS value 18 to honor the
		 * same behavior as before. Any packet using more than
		 * 18 slots but less than fatal_skb_slots slots is
		 * dropped
		 */
		if (!drop_err && slots >= XEN_NETBK_LEGACY_SLOTS_MAX) {
			if (net_ratelimit())
<<<<<<< HEAD
				netdev_dbg(vif->dev,
=======
				netdev_dbg(queue->vif->dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					   "Too many slots (%d) exceeding limit (%d), dropping packet\n",
					   slots, XEN_NETBK_LEGACY_SLOTS_MAX);
			drop_err = -E2BIG;
		}

		if (drop_err)
			txp = &dropped_tx;

<<<<<<< HEAD
		RING_COPY_REQUEST(&vif->tx, cons + slots, txp);
=======
		RING_COPY_REQUEST(&queue->tx, cons + slots, txp);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* If the guest submitted a frame >= 64 KiB then
		 * first->size overflowed and following slots will
		 * appear to be larger than the frame.
		 *
		 * This cannot be fatal error as there are buggy
		 * frontends that do this.
		 *
		 * Consume all slots and drop the packet.
		 */
		if (!drop_err && txp->size > first->size) {
			if (net_ratelimit())
<<<<<<< HEAD
				netdev_dbg(vif->dev,
=======
				netdev_dbg(queue->vif->dev,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					   "Invalid tx request, slot size %u > remaining size %u\n",
					   txp->size, first->size);
			drop_err = -EIO;
		}

		first->size -= txp->size;
		slots++;

<<<<<<< HEAD
		if (unlikely((txp->offset + txp->size) > PAGE_SIZE)) {
			netdev_err(vif->dev, "Cross page boundary, txp->offset: %x, size: %u\n",
				 txp->offset, txp->size);
			netbk_fatal_tx_err(vif);
=======
		if (unlikely((txp->offset + txp->size) > XEN_PAGE_SIZE)) {
			netdev_err(queue->vif->dev, "Cross page boundary, txp->offset: %u, size: %u\n",
				 txp->offset, txp->size);
			xenvif_fatal_tx_err(queue->vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EINVAL;
		}

		more_data = txp->flags & XEN_NETTXF_more_data;

		if (!drop_err)
			txp++;

	} while (more_data);

	if (drop_err) {
<<<<<<< HEAD
		netbk_tx_err(vif, first, cons + slots);
=======
		xenvif_tx_err(queue, first, extra_count, cons + slots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return drop_err;
	}

	return slots;
}

<<<<<<< HEAD
static struct page *xen_netbk_alloc_page(struct xen_netbk *netbk,
					 u16 pending_idx)
{
	struct page *page;
	page = alloc_page(GFP_KERNEL|__GFP_COLD);
	if (!page)
		return NULL;
	set_page_ext(page, netbk, pending_idx);
	netbk->mmap_pages[pending_idx] = page;
	return page;
}

static struct gnttab_copy *xen_netbk_get_requests(struct xen_netbk *netbk,
						  struct xenvif *vif,
						  struct sk_buff *skb,
						  struct xen_netif_tx_request *txp,
						  struct gnttab_copy *gop)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	skb_frag_t *frags = shinfo->frags;
	u16 pending_idx = *((u16 *)skb->data);
	u16 head_idx = 0;
	int slot, start;
	struct page *page;
	pending_ring_idx_t index, start_idx = 0;
	uint16_t dst_offset;
	unsigned int nr_slots;
	struct pending_tx_info *first = NULL;

	/* At this point shinfo->nr_frags is in fact the number of
	 * slots, which can be as large as XEN_NETBK_LEGACY_SLOTS_MAX.
	 */
=======

struct xenvif_tx_cb {
	u16 pending_idx;
};

#define XENVIF_TX_CB(skb) ((struct xenvif_tx_cb *)(skb)->cb)

static inline void xenvif_tx_create_map_op(struct xenvif_queue *queue,
					   u16 pending_idx,
					   struct xen_netif_tx_request *txp,
					   unsigned int extra_count,
					   struct gnttab_map_grant_ref *mop)
{
	queue->pages_to_map[mop-queue->tx_map_ops] = queue->mmap_pages[pending_idx];
	gnttab_set_map_op(mop, idx_to_kaddr(queue, pending_idx),
			  GNTMAP_host_map | GNTMAP_readonly,
			  txp->gref, queue->vif->domid);

	memcpy(&queue->pending_tx_info[pending_idx].req, txp,
	       sizeof(*txp));
	queue->pending_tx_info[pending_idx].extra_count = extra_count;
}

static inline struct sk_buff *xenvif_alloc_skb(unsigned int size)
{
	struct sk_buff *skb =
		alloc_skb(size + NET_SKB_PAD + NET_IP_ALIGN,
			  GFP_ATOMIC | __GFP_NOWARN);
	if (unlikely(skb == NULL))
		return NULL;

	/* Packets passed to netif_rx() must have some headroom. */
	skb_reserve(skb, NET_SKB_PAD + NET_IP_ALIGN);

	/* Initialize it here to avoid later surprises */
	skb_shinfo(skb)->destructor_arg = NULL;

	return skb;
}

static struct gnttab_map_grant_ref *xenvif_get_requests(struct xenvif_queue *queue,
							struct sk_buff *skb,
							struct xen_netif_tx_request *txp,
							struct gnttab_map_grant_ref *gop,
							unsigned int frag_overflow,
							struct sk_buff *nskb)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	skb_frag_t *frags = shinfo->frags;
	u16 pending_idx = XENVIF_TX_CB(skb)->pending_idx;
	int start;
	pending_ring_idx_t index;
	unsigned int nr_slots;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	nr_slots = shinfo->nr_frags;

	/* Skip first skb fragment if it is on same page as header fragment. */
	start = (frag_get_pending_idx(&shinfo->frags[0]) == pending_idx);

<<<<<<< HEAD
	/* Coalesce tx requests, at this point the packet passed in
	 * should be <= 64K. Any packets larger than 64K have been
	 * handled in netbk_count_requests().
	 */
	for (shinfo->nr_frags = slot = start; slot < nr_slots;
	     shinfo->nr_frags++) {
		struct pending_tx_info *pending_tx_info =
			netbk->pending_tx_info;

		page = alloc_page(GFP_KERNEL|__GFP_COLD);
		if (!page)
			goto err;

		dst_offset = 0;
		first = NULL;
		while (dst_offset < PAGE_SIZE && slot < nr_slots) {
			gop->flags = GNTCOPY_source_gref;

			gop->source.u.ref = txp->gref;
			gop->source.domid = vif->domid;
			gop->source.offset = txp->offset;

			gop->dest.domid = DOMID_SELF;

			gop->dest.offset = dst_offset;
			gop->dest.u.gmfn = virt_to_mfn(page_address(page));

			if (dst_offset + txp->size > PAGE_SIZE) {
				/* This page can only merge a portion
				 * of tx request. Do not increment any
				 * pointer / counter here. The txp
				 * will be dealt with in future
				 * rounds, eventually hitting the
				 * `else` branch.
				 */
				gop->len = PAGE_SIZE - dst_offset;
				txp->offset += gop->len;
				txp->size -= gop->len;
				dst_offset += gop->len; /* quit loop */
			} else {
				/* This tx request can be merged in the page */
				gop->len = txp->size;
				dst_offset += gop->len;

				index = pending_index(netbk->pending_cons++);

				pending_idx = netbk->pending_ring[index];

				memcpy(&pending_tx_info[pending_idx].req, txp,
				       sizeof(*txp));
				xenvif_get(vif);

				pending_tx_info[pending_idx].vif = vif;

				/* Poison these fields, corresponding
				 * fields for head tx req will be set
				 * to correct values after the loop.
				 */
				netbk->mmap_pages[pending_idx] = (void *)(~0UL);
				pending_tx_info[pending_idx].head =
					INVALID_PENDING_RING_IDX;

				if (!first) {
					first = &pending_tx_info[pending_idx];
					start_idx = index;
					head_idx = pending_idx;
				}

				txp++;
				slot++;
			}

			gop++;
		}

		first->req.offset = 0;
		first->req.size = dst_offset;
		first->head = start_idx;
		set_page_ext(page, netbk, head_idx);
		netbk->mmap_pages[head_idx] = page;
		frag_set_pending_idx(&frags[shinfo->nr_frags], head_idx);
	}

	BUG_ON(shinfo->nr_frags > MAX_SKB_FRAGS);

	return gop;
err:
	/* Unwind, freeing all pages and sending error responses. */
	while (shinfo->nr_frags-- > start) {
		xen_netbk_idx_release(netbk,
				frag_get_pending_idx(&frags[shinfo->nr_frags]),
				XEN_NETIF_RSP_ERROR);
	}
	/* The head too, if necessary. */
	if (start)
		xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_ERROR);

	return NULL;
}

static int xen_netbk_tx_check_gop(struct xen_netbk *netbk,
				  struct sk_buff *skb,
				  struct gnttab_copy **gopp)
{
	struct gnttab_copy *gop = *gopp;
	u16 pending_idx = *((u16 *)skb->data);
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	struct pending_tx_info *tx_info;
	int nr_frags = shinfo->nr_frags;
	int i, err, start;
	u16 peek; /* peek into next tx request */

	/* Check status of header. */
	err = gop->status;
	if (unlikely(err))
		xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_ERROR);

	/* Skip first skb fragment if it is on same page as header fragment. */
	start = (frag_get_pending_idx(&shinfo->frags[0]) == pending_idx);

	for (i = start; i < nr_frags; i++) {
		int j, newerr;
		pending_ring_idx_t head;

		pending_idx = frag_get_pending_idx(&shinfo->frags[i]);
		tx_info = &netbk->pending_tx_info[pending_idx];
		head = tx_info->head;

		/* Check error status: if okay then remember grant handle. */
		do {
			newerr = (++gop)->status;
			if (newerr)
				break;
			peek = netbk->pending_ring[pending_index(++head)];
		} while (!pending_tx_is_head(netbk, peek));

		if (likely(!newerr)) {
			/* Had a previous error? Invalidate this fragment. */
			if (unlikely(err))
				xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_OKAY);
=======
	for (shinfo->nr_frags = start; shinfo->nr_frags < nr_slots;
	     shinfo->nr_frags++, txp++, gop++) {
		index = pending_index(queue->pending_cons++);
		pending_idx = queue->pending_ring[index];
		xenvif_tx_create_map_op(queue, pending_idx, txp, 0, gop);
		frag_set_pending_idx(&frags[shinfo->nr_frags], pending_idx);
	}

	if (frag_overflow) {

		shinfo = skb_shinfo(nskb);
		frags = shinfo->frags;

		for (shinfo->nr_frags = 0; shinfo->nr_frags < frag_overflow;
		     shinfo->nr_frags++, txp++, gop++) {
			index = pending_index(queue->pending_cons++);
			pending_idx = queue->pending_ring[index];
			xenvif_tx_create_map_op(queue, pending_idx, txp, 0,
						gop);
			frag_set_pending_idx(&frags[shinfo->nr_frags],
					     pending_idx);
		}

		skb_shinfo(skb)->frag_list = nskb;
	}

	return gop;
}

static inline void xenvif_grant_handle_set(struct xenvif_queue *queue,
					   u16 pending_idx,
					   grant_handle_t handle)
{
	if (unlikely(queue->grant_tx_handle[pending_idx] !=
		     NETBACK_INVALID_HANDLE)) {
		netdev_err(queue->vif->dev,
			   "Trying to overwrite active handle! pending_idx: 0x%x\n",
			   pending_idx);
		BUG();
	}
	queue->grant_tx_handle[pending_idx] = handle;
}

static inline void xenvif_grant_handle_reset(struct xenvif_queue *queue,
					     u16 pending_idx)
{
	if (unlikely(queue->grant_tx_handle[pending_idx] ==
		     NETBACK_INVALID_HANDLE)) {
		netdev_err(queue->vif->dev,
			   "Trying to unmap invalid handle! pending_idx: 0x%x\n",
			   pending_idx);
		BUG();
	}
	queue->grant_tx_handle[pending_idx] = NETBACK_INVALID_HANDLE;
}

static int xenvif_tx_check_gop(struct xenvif_queue *queue,
			       struct sk_buff *skb,
			       struct gnttab_map_grant_ref **gopp_map,
			       struct gnttab_copy **gopp_copy)
{
	struct gnttab_map_grant_ref *gop_map = *gopp_map;
	u16 pending_idx = XENVIF_TX_CB(skb)->pending_idx;
	/* This always points to the shinfo of the skb being checked, which
	 * could be either the first or the one on the frag_list
	 */
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	/* If this is non-NULL, we are currently checking the frag_list skb, and
	 * this points to the shinfo of the first one
	 */
	struct skb_shared_info *first_shinfo = NULL;
	int nr_frags = shinfo->nr_frags;
	const bool sharedslot = nr_frags &&
				frag_get_pending_idx(&shinfo->frags[0]) == pending_idx;
	int i, err;

	/* Check status of header. */
	err = (*gopp_copy)->status;
	if (unlikely(err)) {
		if (net_ratelimit())
			netdev_dbg(queue->vif->dev,
				   "Grant copy of header failed! status: %d pending_idx: %u ref: %u\n",
				   (*gopp_copy)->status,
				   pending_idx,
				   (*gopp_copy)->source.u.ref);
		/* The first frag might still have this slot mapped */
		if (!sharedslot)
			xenvif_idx_release(queue, pending_idx,
					   XEN_NETIF_RSP_ERROR);
	}
	(*gopp_copy)++;

check_frags:
	for (i = 0; i < nr_frags; i++, gop_map++) {
		int j, newerr;

		pending_idx = frag_get_pending_idx(&shinfo->frags[i]);

		/* Check error status: if okay then remember grant handle. */
		newerr = gop_map->status;

		if (likely(!newerr)) {
			xenvif_grant_handle_set(queue,
						pending_idx,
						gop_map->handle);
			/* Had a previous error? Invalidate this fragment. */
			if (unlikely(err)) {
				xenvif_idx_unmap(queue, pending_idx);
				/* If the mapping of the first frag was OK, but
				 * the header's copy failed, and they are
				 * sharing a slot, send an error
				 */
				if (i == 0 && sharedslot)
					xenvif_idx_release(queue, pending_idx,
							   XEN_NETIF_RSP_ERROR);
				else
					xenvif_idx_release(queue, pending_idx,
							   XEN_NETIF_RSP_OKAY);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		}

		/* Error on this fragment: respond to client with an error. */
<<<<<<< HEAD
		xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_ERROR);
=======
		if (net_ratelimit())
			netdev_dbg(queue->vif->dev,
				   "Grant map of %d. frag failed! status: %d pending_idx: %u ref: %u\n",
				   i,
				   gop_map->status,
				   pending_idx,
				   gop_map->ref);

		xenvif_idx_release(queue, pending_idx, XEN_NETIF_RSP_ERROR);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* Not the first error? Preceding frags already invalidated. */
		if (err)
			continue;

<<<<<<< HEAD
		/* First error: invalidate header and preceding fragments. */
		pending_idx = *((u16 *)skb->data);
		xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_OKAY);
		for (j = start; j < i; j++) {
			pending_idx = frag_get_pending_idx(&shinfo->frags[j]);
			xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_OKAY);
=======
		/* First error: if the header haven't shared a slot with the
		 * first frag, release it as well.
		 */
		if (!sharedslot)
			xenvif_idx_release(queue,
					   XENVIF_TX_CB(skb)->pending_idx,
					   XEN_NETIF_RSP_OKAY);

		/* Invalidate preceding fragments of this skb. */
		for (j = 0; j < i; j++) {
			pending_idx = frag_get_pending_idx(&shinfo->frags[j]);
			xenvif_idx_unmap(queue, pending_idx);
			xenvif_idx_release(queue, pending_idx,
					   XEN_NETIF_RSP_OKAY);
		}

		/* And if we found the error while checking the frag_list, unmap
		 * the first skb's frags
		 */
		if (first_shinfo) {
			for (j = 0; j < first_shinfo->nr_frags; j++) {
				pending_idx = frag_get_pending_idx(&first_shinfo->frags[j]);
				xenvif_idx_unmap(queue, pending_idx);
				xenvif_idx_release(queue, pending_idx,
						   XEN_NETIF_RSP_OKAY);
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		/* Remember the error: invalidate all subsequent fragments. */
		err = newerr;
	}

<<<<<<< HEAD
	*gopp = gop + 1;
	return err;
}

static void xen_netbk_fill_frags(struct xen_netbk *netbk, struct sk_buff *skb)
=======
	if (skb_has_frag_list(skb) && !first_shinfo) {
		first_shinfo = skb_shinfo(skb);
		shinfo = skb_shinfo(skb_shinfo(skb)->frag_list);
		nr_frags = shinfo->nr_frags;

		goto check_frags;
	}

	*gopp_map = gop_map;
	return err;
}

static void xenvif_fill_frags(struct xenvif_queue *queue, struct sk_buff *skb)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	int nr_frags = shinfo->nr_frags;
	int i;
<<<<<<< HEAD
=======
	u16 prev_pending_idx = INVALID_PENDING_IDX;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < nr_frags; i++) {
		skb_frag_t *frag = shinfo->frags + i;
		struct xen_netif_tx_request *txp;
		struct page *page;
		u16 pending_idx;

		pending_idx = frag_get_pending_idx(frag);

<<<<<<< HEAD
		txp = &netbk->pending_tx_info[pending_idx].req;
		page = virt_to_page(idx_to_kaddr(netbk, pending_idx));
=======
		/* If this is not the first frag, chain it to the previous*/
		if (prev_pending_idx == INVALID_PENDING_IDX)
			skb_shinfo(skb)->destructor_arg =
				&callback_param(queue, pending_idx);
		else
			callback_param(queue, prev_pending_idx).ctx =
				&callback_param(queue, pending_idx);

		callback_param(queue, pending_idx).ctx = NULL;
		prev_pending_idx = pending_idx;

		txp = &queue->pending_tx_info[pending_idx].req;
		page = virt_to_page(idx_to_kaddr(queue, pending_idx));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		__skb_fill_page_desc(skb, i, page, txp->offset, txp->size);
		skb->len += txp->size;
		skb->data_len += txp->size;
		skb->truesize += txp->size;

<<<<<<< HEAD
		/* Take an extra reference to offset xen_netbk_idx_release */
		get_page(netbk->mmap_pages[pending_idx]);
		xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_OKAY);
	}
}

static int xen_netbk_get_extras(struct xenvif *vif,
				struct xen_netif_extra_info *extras,
				int work_to_do)
{
	struct xen_netif_extra_info extra;
	RING_IDX cons = vif->tx.req_cons;

	do {
		if (unlikely(work_to_do-- <= 0)) {
			netdev_err(vif->dev, "Missing extra info\n");
			netbk_fatal_tx_err(vif);
			return -EBADR;
		}

		RING_COPY_REQUEST(&vif->tx, cons, &extra);
		if (unlikely(!extra.type ||
			     extra.type >= XEN_NETIF_EXTRA_TYPE_MAX)) {
			vif->tx.req_cons = ++cons;
			netdev_err(vif->dev,
				   "Invalid extra type: %d\n", extra.type);
			netbk_fatal_tx_err(vif);
=======
		/* Take an extra reference to offset network stack's put_page */
		get_page(queue->mmap_pages[pending_idx]);
	}
}

static int xenvif_get_extras(struct xenvif_queue *queue,
			     struct xen_netif_extra_info *extras,
			     unsigned int *extra_count,
			     int work_to_do)
{
	struct xen_netif_extra_info extra;
	RING_IDX cons = queue->tx.req_cons;

	do {
		if (unlikely(work_to_do-- <= 0)) {
			netdev_err(queue->vif->dev, "Missing extra info\n");
			xenvif_fatal_tx_err(queue->vif);
			return -EBADR;
		}

		RING_COPY_REQUEST(&queue->tx, cons, &extra);

		queue->tx.req_cons = ++cons;
		(*extra_count)++;

		if (unlikely(!extra.type ||
			     extra.type >= XEN_NETIF_EXTRA_TYPE_MAX)) {
			netdev_err(queue->vif->dev,
				   "Invalid extra type: %d\n", extra.type);
			xenvif_fatal_tx_err(queue->vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EINVAL;
		}

		memcpy(&extras[extra.type - 1], &extra, sizeof(extra));
<<<<<<< HEAD
		vif->tx.req_cons = ++cons;
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} while (extra.flags & XEN_NETIF_EXTRA_FLAG_MORE);

	return work_to_do;
}

<<<<<<< HEAD
static int netbk_set_skb_gso(struct xenvif *vif,
			     struct sk_buff *skb,
			     struct xen_netif_extra_info *gso)
{
	if (!gso->u.gso.size) {
		netdev_err(vif->dev, "GSO size must not be zero.\n");
		netbk_fatal_tx_err(vif);
		return -EINVAL;
	}

	/* Currently only TCPv4 S.O. is supported. */
	if (gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV4) {
		netdev_err(vif->dev, "Bad GSO type %d.\n", gso->u.gso.type);
		netbk_fatal_tx_err(vif);
=======
static int xenvif_set_skb_gso(struct xenvif *vif,
			      struct sk_buff *skb,
			      struct xen_netif_extra_info *gso)
{
	if (!gso->u.gso.size) {
		netdev_err(vif->dev, "GSO size must not be zero.\n");
		xenvif_fatal_tx_err(vif);
		return -EINVAL;
	}

	switch (gso->u.gso.type) {
	case XEN_NETIF_GSO_TYPE_TCPV4:
		skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;
		break;
	case XEN_NETIF_GSO_TYPE_TCPV6:
		skb_shinfo(skb)->gso_type = SKB_GSO_TCPV6;
		break;
	default:
		netdev_err(vif->dev, "Bad GSO type %d.\n", gso->u.gso.type);
		xenvif_fatal_tx_err(vif);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;
	}

	skb_shinfo(skb)->gso_size = gso->u.gso.size;
<<<<<<< HEAD
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;

	/* Header must be checked, and gso_segs computed. */
	skb_shinfo(skb)->gso_type |= SKB_GSO_DODGY;
	skb_shinfo(skb)->gso_segs = 0;
=======
	/* gso_segs will be calculated later */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static int checksum_setup(struct xenvif *vif, struct sk_buff *skb)
{
	struct iphdr *iph;
	int err = -EPROTO;
	int recalculate_partial_csum = 0;

	/*
	 * A GSO SKB must be CHECKSUM_PARTIAL. However some buggy
=======
static int checksum_setup(struct xenvif_queue *queue, struct sk_buff *skb)
{
	bool recalculate_partial_csum = false;

	/* A GSO SKB must be CHECKSUM_PARTIAL. However some buggy
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 * peers can fail to set NETRXF_csum_blank when sending a GSO
	 * frame. In this case force the SKB to CHECKSUM_PARTIAL and
	 * recalculate the partial checksum.
	 */
	if (skb->ip_summed != CHECKSUM_PARTIAL && skb_is_gso(skb)) {
<<<<<<< HEAD
		vif->rx_gso_checksum_fixup++;
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = 1;
=======
		queue->stats.rx_gso_checksum_fixup++;
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = true;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	/* A non-CHECKSUM_PARTIAL SKB does not require setup. */
	if (skb->ip_summed != CHECKSUM_PARTIAL)
		return 0;

<<<<<<< HEAD
	if (skb->protocol != htons(ETH_P_IP))
		goto out;

	iph = (void *)skb->data;
	switch (iph->protocol) {
	case IPPROTO_TCP:
		if (!skb_partial_csum_set(skb, 4 * iph->ihl,
					  offsetof(struct tcphdr, check)))
			goto out;

		if (recalculate_partial_csum) {
			struct tcphdr *tcph = tcp_hdr(skb);
			tcph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_TCP, 0);
		}
		break;
	case IPPROTO_UDP:
		if (!skb_partial_csum_set(skb, 4 * iph->ihl,
					  offsetof(struct udphdr, check)))
			goto out;

		if (recalculate_partial_csum) {
			struct udphdr *udph = udp_hdr(skb);
			udph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_UDP, 0);
		}
		break;
	default:
		if (net_ratelimit())
			netdev_err(vif->dev,
				   "Attempting to checksum a non-TCP/UDP packet, dropping a protocol %d packet\n",
				   iph->protocol);
		goto out;
	}

	err = 0;

out:
	return err;
}

static bool tx_credit_exceeded(struct xenvif *vif, unsigned size)
{
	u64 now = get_jiffies_64();
	u64 next_credit = vif->credit_window_start +
		msecs_to_jiffies(vif->credit_usec / 1000);

	/* Timer could already be pending in rare cases. */
	if (timer_pending(&vif->credit_timeout))
		return true;

	/* Passed the point where we can replenish credit? */
	if (time_after_eq64(now, next_credit)) {
		vif->credit_window_start = now;
		tx_add_credit(vif);
	}

	/* Still too big to send right now? Set a callback. */
	if (size > vif->remaining_credit) {
		vif->credit_timeout.data     =
			(unsigned long)vif;
		vif->credit_timeout.function =
			tx_credit_callback;
		mod_timer(&vif->credit_timeout,
			  next_credit);
		vif->credit_window_start = next_credit;

		return true;
	}
=======
	return skb_checksum_setup(skb, recalculate_partial_csum);
}

static bool tx_credit_exceeded(struct xenvif_queue *queue, unsigned size)
{
	u64 now = get_jiffies_64();
	u64 next_credit = queue->credit_window_start +
		msecs_to_jiffies(queue->credit_usec / 1000);

	/* Timer could already be pending in rare cases. */
	if (timer_pending(&queue->credit_timeout)) {
		queue->rate_limited = true;
		return true;
	}

	/* Passed the point where we can replenish credit? */
	if (time_after_eq64(now, next_credit)) {
		queue->credit_window_start = now;
		tx_add_credit(queue);
	}

	/* Still too big to send right now? Set a callback. */
	if (size > queue->remaining_credit) {
		queue->credit_timeout.data     =
			(unsigned long)queue;
		mod_timer(&queue->credit_timeout,
			  next_credit);
		queue->credit_window_start = next_credit;
		queue->rate_limited = true;

		return true;
	}

	return false;
}

/* No locking is required in xenvif_mcast_add/del() as they are
 * only ever invoked from NAPI poll. An RCU list is used because
 * xenvif_mcast_match() is called asynchronously, during start_xmit.
 */

static int xenvif_mcast_add(struct xenvif *vif, const u8 *addr)
{
	struct xenvif_mcast_addr *mcast;

	if (vif->fe_mcast_count == XEN_NETBK_MCAST_MAX) {
		if (net_ratelimit())
			netdev_err(vif->dev,
				   "Too many multicast addresses\n");
		return -ENOSPC;
	}

	mcast = kzalloc(sizeof(*mcast), GFP_ATOMIC);
	if (!mcast)
		return -ENOMEM;

	ether_addr_copy(mcast->addr, addr);
	list_add_tail_rcu(&mcast->entry, &vif->fe_mcast_addr);
	vif->fe_mcast_count++;

	return 0;
}

static void xenvif_mcast_del(struct xenvif *vif, const u8 *addr)
{
	struct xenvif_mcast_addr *mcast;

	list_for_each_entry_rcu(mcast, &vif->fe_mcast_addr, entry) {
		if (ether_addr_equal(addr, mcast->addr)) {
			--vif->fe_mcast_count;
			list_del_rcu(&mcast->entry);
			kfree_rcu(mcast, rcu);
			break;
		}
	}
}

bool xenvif_mcast_match(struct xenvif *vif, const u8 *addr)
{
	struct xenvif_mcast_addr *mcast;

	rcu_read_lock();
	list_for_each_entry_rcu(mcast, &vif->fe_mcast_addr, entry) {
		if (ether_addr_equal(addr, mcast->addr)) {
			rcu_read_unlock();
			return true;
		}
	}
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return false;
}

<<<<<<< HEAD
static unsigned xen_netbk_tx_build_gops(struct xen_netbk *netbk)
{
	struct gnttab_copy *gop = netbk->tx_copy_ops, *request_gop;
	struct sk_buff *skb;
	int ret;

	while ((nr_pending_reqs(netbk) + XEN_NETBK_LEGACY_SLOTS_MAX
		< MAX_PENDING_REQS) &&
		!list_empty(&netbk->net_schedule_list)) {
		struct xenvif *vif;
		struct xen_netif_tx_request txreq;
		struct xen_netif_tx_request txfrags[XEN_NETBK_LEGACY_SLOTS_MAX];
		struct page *page;
		struct xen_netif_extra_info extras[XEN_NETIF_EXTRA_TYPE_MAX-1];
=======
void xenvif_mcast_addr_list_free(struct xenvif *vif)
{
	/* No need for locking or RCU here. NAPI poll and TX queue
	 * are stopped.
	 */
	while (!list_empty(&vif->fe_mcast_addr)) {
		struct xenvif_mcast_addr *mcast;

		mcast = list_first_entry(&vif->fe_mcast_addr,
					 struct xenvif_mcast_addr,
					 entry);
		--vif->fe_mcast_count;
		list_del(&mcast->entry);
		kfree(mcast);
	}
}

static void xenvif_tx_build_gops(struct xenvif_queue *queue,
				     int budget,
				     unsigned *copy_ops,
				     unsigned *map_ops)
{
	struct gnttab_map_grant_ref *gop = queue->tx_map_ops;
	struct sk_buff *skb, *nskb;
	int ret;
	unsigned int frag_overflow;

	while (skb_queue_len(&queue->tx_queue) < budget) {
		struct xen_netif_tx_request txreq;
		struct xen_netif_tx_request txfrags[XEN_NETBK_LEGACY_SLOTS_MAX];
		struct xen_netif_extra_info extras[XEN_NETIF_EXTRA_TYPE_MAX-1];
		unsigned int extra_count;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		u16 pending_idx;
		RING_IDX idx;
		int work_to_do;
		unsigned int data_len;
		pending_ring_idx_t index;

<<<<<<< HEAD
		/* Get a netif from the list with work to do. */
		vif = poll_net_schedule_list(netbk);
		/* This can sometimes happen because the test of
		 * list_empty(net_schedule_list) at the top of the
		 * loop is unlocked.  Just go back and have another
		 * look.
		 */
		if (!vif)
			continue;

		if (vif->tx.sring->req_prod - vif->tx.req_cons >
		    XEN_NETIF_TX_RING_SIZE) {
			netdev_err(vif->dev,
				   "Impossible number of requests. "
				   "req_prod %d, req_cons %d, size %ld\n",
				   vif->tx.sring->req_prod, vif->tx.req_cons,
				   XEN_NETIF_TX_RING_SIZE);
			netbk_fatal_tx_err(vif);
			continue;
		}

		RING_FINAL_CHECK_FOR_REQUESTS(&vif->tx, work_to_do);
		if (!work_to_do) {
			xenvif_put(vif);
			continue;
		}

		idx = vif->tx.req_cons;
		rmb(); /* Ensure that we see the request before we copy it. */
		RING_COPY_REQUEST(&vif->tx, idx, &txreq);

		/* Credit-based scheduling. */
		if (txreq.size > vif->remaining_credit &&
		    tx_credit_exceeded(vif, txreq.size)) {
			xenvif_put(vif);
			continue;
		}

		vif->remaining_credit -= txreq.size;

		work_to_do--;
		vif->tx.req_cons = ++idx;

		memset(extras, 0, sizeof(extras));
		if (txreq.flags & XEN_NETTXF_extra_info) {
			work_to_do = xen_netbk_get_extras(vif, extras,
							  work_to_do);
			idx = vif->tx.req_cons;
			if (unlikely(work_to_do < 0))
				continue;
		}

		ret = netbk_count_requests(vif, &txreq, txfrags, work_to_do);
		if (unlikely(ret < 0))
			continue;
=======
		if (queue->tx.sring->req_prod - queue->tx.req_cons >
		    XEN_NETIF_TX_RING_SIZE) {
			netdev_err(queue->vif->dev,
				   "Impossible number of requests. "
				   "req_prod %d, req_cons %d, size %ld\n",
				   queue->tx.sring->req_prod, queue->tx.req_cons,
				   XEN_NETIF_TX_RING_SIZE);
			xenvif_fatal_tx_err(queue->vif);
			break;
		}

		work_to_do = RING_HAS_UNCONSUMED_REQUESTS(&queue->tx);
		if (!work_to_do)
			break;

		idx = queue->tx.req_cons;
		rmb(); /* Ensure that we see the request before we copy it. */
		RING_COPY_REQUEST(&queue->tx, idx, &txreq);

		/* Credit-based scheduling. */
		if (txreq.size > queue->remaining_credit &&
		    tx_credit_exceeded(queue, txreq.size))
			break;

		queue->remaining_credit -= txreq.size;

		work_to_do--;
		queue->tx.req_cons = ++idx;

		memset(extras, 0, sizeof(extras));
		extra_count = 0;
		if (txreq.flags & XEN_NETTXF_extra_info) {
			work_to_do = xenvif_get_extras(queue, extras,
						       &extra_count,
						       work_to_do);
			idx = queue->tx.req_cons;
			if (unlikely(work_to_do < 0))
				break;
		}

		if (extras[XEN_NETIF_EXTRA_TYPE_MCAST_ADD - 1].type) {
			struct xen_netif_extra_info *extra;

			extra = &extras[XEN_NETIF_EXTRA_TYPE_MCAST_ADD - 1];
			ret = xenvif_mcast_add(queue->vif, extra->u.mcast.addr);

			make_tx_response(queue, &txreq, extra_count,
					 (ret == 0) ?
					 XEN_NETIF_RSP_OKAY :
					 XEN_NETIF_RSP_ERROR);
			push_tx_responses(queue);
			continue;
		}

		if (extras[XEN_NETIF_EXTRA_TYPE_MCAST_DEL - 1].type) {
			struct xen_netif_extra_info *extra;

			extra = &extras[XEN_NETIF_EXTRA_TYPE_MCAST_DEL - 1];
			xenvif_mcast_del(queue->vif, extra->u.mcast.addr);

			make_tx_response(queue, &txreq, extra_count,
					 XEN_NETIF_RSP_OKAY);
			push_tx_responses(queue);
			continue;
		}

		ret = xenvif_count_requests(queue, &txreq, extra_count,
					    txfrags, work_to_do);
		if (unlikely(ret < 0))
			break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		idx += ret;

		if (unlikely(txreq.size < ETH_HLEN)) {
<<<<<<< HEAD
			netdev_dbg(vif->dev,
				   "Bad packet size: %d\n", txreq.size);
			netbk_tx_err(vif, &txreq, idx);
			continue;
		}

		/* No crossing a page as the payload mustn't fragment. */
		if (unlikely((txreq.offset + txreq.size) > PAGE_SIZE)) {
			netdev_err(vif->dev,
				   "txreq.offset: %x, size: %u, end: %lu\n",
				   txreq.offset, txreq.size,
				   (txreq.offset&~PAGE_MASK) + txreq.size);
			netbk_fatal_tx_err(vif);
			continue;
		}

		index = pending_index(netbk->pending_cons);
		pending_idx = netbk->pending_ring[index];

		data_len = (txreq.size > PKT_PROT_LEN &&
			    ret < XEN_NETBK_LEGACY_SLOTS_MAX) ?
			PKT_PROT_LEN : txreq.size;

		skb = alloc_skb(data_len + NET_SKB_PAD + NET_IP_ALIGN,
				GFP_ATOMIC | __GFP_NOWARN);
		if (unlikely(skb == NULL)) {
			netdev_dbg(vif->dev,
				   "Can't allocate a skb in start_xmit.\n");
			netbk_tx_err(vif, &txreq, idx);
			break;
		}

		/* Packets passed to netif_rx() must have some headroom. */
		skb_reserve(skb, NET_SKB_PAD + NET_IP_ALIGN);
=======
			netdev_dbg(queue->vif->dev,
				   "Bad packet size: %d\n", txreq.size);
			xenvif_tx_err(queue, &txreq, extra_count, idx);
			break;
		}

		/* No crossing a page as the payload mustn't fragment. */
		if (unlikely((txreq.offset + txreq.size) > XEN_PAGE_SIZE)) {
			netdev_err(queue->vif->dev,
				   "txreq.offset: %u, size: %u, end: %lu\n",
				   txreq.offset, txreq.size,
				   (unsigned long)(txreq.offset&~XEN_PAGE_MASK) + txreq.size);
			xenvif_fatal_tx_err(queue->vif);
			break;
		}

		index = pending_index(queue->pending_cons);
		pending_idx = queue->pending_ring[index];

		data_len = (txreq.size > XEN_NETBACK_TX_COPY_LEN &&
			    ret < XEN_NETBK_LEGACY_SLOTS_MAX) ?
			XEN_NETBACK_TX_COPY_LEN : txreq.size;

		skb = xenvif_alloc_skb(data_len);
		if (unlikely(skb == NULL)) {
			netdev_dbg(queue->vif->dev,
				   "Can't allocate a skb in start_xmit.\n");
			xenvif_tx_err(queue, &txreq, extra_count, idx);
			break;
		}

		skb_shinfo(skb)->nr_frags = ret;
		if (data_len < txreq.size)
			skb_shinfo(skb)->nr_frags++;
		/* At this point shinfo->nr_frags is in fact the number of
		 * slots, which can be as large as XEN_NETBK_LEGACY_SLOTS_MAX.
		 */
		frag_overflow = 0;
		nskb = NULL;
		if (skb_shinfo(skb)->nr_frags > MAX_SKB_FRAGS) {
			frag_overflow = skb_shinfo(skb)->nr_frags - MAX_SKB_FRAGS;
			BUG_ON(frag_overflow > MAX_SKB_FRAGS);
			skb_shinfo(skb)->nr_frags = MAX_SKB_FRAGS;
			nskb = xenvif_alloc_skb(0);
			if (unlikely(nskb == NULL)) {
				skb_shinfo(skb)->nr_frags = 0;
				kfree_skb(skb);
				xenvif_tx_err(queue, &txreq, extra_count, idx);
				if (net_ratelimit())
					netdev_err(queue->vif->dev,
						   "Can't allocate the frag_list skb.\n");
				break;
			}
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (extras[XEN_NETIF_EXTRA_TYPE_GSO - 1].type) {
			struct xen_netif_extra_info *gso;
			gso = &extras[XEN_NETIF_EXTRA_TYPE_GSO - 1];

<<<<<<< HEAD
			if (netbk_set_skb_gso(vif, skb, gso)) {
				/* Failure in netbk_set_skb_gso is fatal. */
				kfree_skb(skb);
				continue;
			}
		}

		/* XXX could copy straight to head */
		page = xen_netbk_alloc_page(netbk, pending_idx);
		if (!page) {
			kfree_skb(skb);
			netbk_tx_err(vif, &txreq, idx);
			continue;
		}

		gop->source.u.ref = txreq.gref;
		gop->source.domid = vif->domid;
		gop->source.offset = txreq.offset;

		gop->dest.u.gmfn = virt_to_mfn(page_address(page));
		gop->dest.domid = DOMID_SELF;
		gop->dest.offset = txreq.offset;

		gop->len = txreq.size;
		gop->flags = GNTCOPY_source_gref;

		gop++;

		memcpy(&netbk->pending_tx_info[pending_idx].req,
		       &txreq, sizeof(txreq));
		netbk->pending_tx_info[pending_idx].vif = vif;
		netbk->pending_tx_info[pending_idx].head = index;
		*((u16 *)skb->data) = pending_idx;

		__skb_put(skb, data_len);

		skb_shinfo(skb)->nr_frags = ret;
		if (data_len < txreq.size) {
			skb_shinfo(skb)->nr_frags++;
			frag_set_pending_idx(&skb_shinfo(skb)->frags[0],
					     pending_idx);
		} else {
			frag_set_pending_idx(&skb_shinfo(skb)->frags[0],
					     INVALID_PENDING_IDX);
		}

		netbk->pending_cons++;

		request_gop = xen_netbk_get_requests(netbk, vif,
						     skb, txfrags, gop);
		if (request_gop == NULL) {
			kfree_skb(skb);
			netbk_tx_err(vif, &txreq, idx);
			continue;
		}
		gop = request_gop;

		__skb_queue_tail(&netbk->tx_queue, skb);

		vif->tx.req_cons = idx;
		xen_netbk_check_rx_xenvif(vif);

		if ((gop-netbk->tx_copy_ops) >= ARRAY_SIZE(netbk->tx_copy_ops))
			break;
	}

	return gop - netbk->tx_copy_ops;
}

static void xen_netbk_tx_submit(struct xen_netbk *netbk)
{
	struct gnttab_copy *gop = netbk->tx_copy_ops;
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(&netbk->tx_queue)) != NULL) {
		struct xen_netif_tx_request *txp;
		struct xenvif *vif;
		u16 pending_idx;
		unsigned data_len;

		pending_idx = *((u16 *)skb->data);
		vif = netbk->pending_tx_info[pending_idx].vif;
		txp = &netbk->pending_tx_info[pending_idx].req;

		/* Check the remap error code. */
		if (unlikely(xen_netbk_tx_check_gop(netbk, skb, &gop))) {
			netdev_dbg(vif->dev, "netback grant failed.\n");
			skb_shinfo(skb)->nr_frags = 0;
=======
			if (xenvif_set_skb_gso(queue->vif, skb, gso)) {
				/* Failure in xenvif_set_skb_gso is fatal. */
				skb_shinfo(skb)->nr_frags = 0;
				kfree_skb(skb);
				kfree_skb(nskb);
				break;
			}
		}

		if (extras[XEN_NETIF_EXTRA_TYPE_HASH - 1].type) {
			struct xen_netif_extra_info *extra;
			enum pkt_hash_types type = PKT_HASH_TYPE_NONE;

			extra = &extras[XEN_NETIF_EXTRA_TYPE_HASH - 1];

			switch (extra->u.hash.type) {
			case _XEN_NETIF_CTRL_HASH_TYPE_IPV4:
			case _XEN_NETIF_CTRL_HASH_TYPE_IPV6:
				type = PKT_HASH_TYPE_L3;
				break;

			case _XEN_NETIF_CTRL_HASH_TYPE_IPV4_TCP:
			case _XEN_NETIF_CTRL_HASH_TYPE_IPV6_TCP:
				type = PKT_HASH_TYPE_L4;
				break;

			default:
				break;
			}

			if (type != PKT_HASH_TYPE_NONE)
				skb_set_hash(skb,
					     *(u32 *)extra->u.hash.value,
					     type);
		}

		XENVIF_TX_CB(skb)->pending_idx = pending_idx;

		__skb_put(skb, data_len);
		queue->tx_copy_ops[*copy_ops].source.u.ref = txreq.gref;
		queue->tx_copy_ops[*copy_ops].source.domid = queue->vif->domid;
		queue->tx_copy_ops[*copy_ops].source.offset = txreq.offset;

		queue->tx_copy_ops[*copy_ops].dest.u.gmfn =
			virt_to_gfn(skb->data);
		queue->tx_copy_ops[*copy_ops].dest.domid = DOMID_SELF;
		queue->tx_copy_ops[*copy_ops].dest.offset =
			offset_in_page(skb->data) & ~XEN_PAGE_MASK;

		queue->tx_copy_ops[*copy_ops].len = data_len;
		queue->tx_copy_ops[*copy_ops].flags = GNTCOPY_source_gref;

		(*copy_ops)++;

		if (data_len < txreq.size) {
			frag_set_pending_idx(&skb_shinfo(skb)->frags[0],
					     pending_idx);
			xenvif_tx_create_map_op(queue, pending_idx, &txreq,
						extra_count, gop);
			gop++;
		} else {
			frag_set_pending_idx(&skb_shinfo(skb)->frags[0],
					     INVALID_PENDING_IDX);
			memcpy(&queue->pending_tx_info[pending_idx].req,
			       &txreq, sizeof(txreq));
			queue->pending_tx_info[pending_idx].extra_count =
				extra_count;
		}

		queue->pending_cons++;

		gop = xenvif_get_requests(queue, skb, txfrags, gop,
				          frag_overflow, nskb);

		__skb_queue_tail(&queue->tx_queue, skb);

		queue->tx.req_cons = idx;

		if (((gop-queue->tx_map_ops) >= ARRAY_SIZE(queue->tx_map_ops)) ||
		    (*copy_ops >= ARRAY_SIZE(queue->tx_copy_ops)))
			break;
	}

	(*map_ops) = gop - queue->tx_map_ops;
	return;
}

/* Consolidate skb with a frag_list into a brand new one with local pages on
 * frags. Returns 0 or -ENOMEM if can't allocate new pages.
 */
static int xenvif_handle_frag_list(struct xenvif_queue *queue, struct sk_buff *skb)
{
	unsigned int offset = skb_headlen(skb);
	skb_frag_t frags[MAX_SKB_FRAGS];
	int i, f;
	struct ubuf_info *uarg;
	struct sk_buff *nskb = skb_shinfo(skb)->frag_list;

	queue->stats.tx_zerocopy_sent += 2;
	queue->stats.tx_frag_overflow++;

	xenvif_fill_frags(queue, nskb);
	/* Subtract frags size, we will correct it later */
	skb->truesize -= skb->data_len;
	skb->len += nskb->len;
	skb->data_len += nskb->len;

	/* create a brand new frags array and coalesce there */
	for (i = 0; offset < skb->len; i++) {
		struct page *page;
		unsigned int len;

		BUG_ON(i >= MAX_SKB_FRAGS);
		page = alloc_page(GFP_ATOMIC);
		if (!page) {
			int j;
			skb->truesize += skb->data_len;
			for (j = 0; j < i; j++)
				put_page(frags[j].page.p);
			return -ENOMEM;
		}

		if (offset + PAGE_SIZE < skb->len)
			len = PAGE_SIZE;
		else
			len = skb->len - offset;
		if (skb_copy_bits(skb, offset, page_address(page), len))
			BUG();

		offset += len;
		frags[i].page.p = page;
		frags[i].page_offset = 0;
		skb_frag_size_set(&frags[i], len);
	}

	/* Release all the original (foreign) frags. */
	for (f = 0; f < skb_shinfo(skb)->nr_frags; f++)
		skb_frag_unref(skb, f);
	uarg = skb_shinfo(skb)->destructor_arg;
	/* increase inflight counter to offset decrement in callback */
	atomic_inc(&queue->inflight_packets);
	uarg->callback(uarg, true);
	skb_shinfo(skb)->destructor_arg = NULL;

	/* Fill the skb with the new (local) frags. */
	memcpy(skb_shinfo(skb)->frags, frags, i * sizeof(skb_frag_t));
	skb_shinfo(skb)->nr_frags = i;
	skb->truesize += i * PAGE_SIZE;

	return 0;
}

static int xenvif_tx_submit(struct xenvif_queue *queue)
{
	struct gnttab_map_grant_ref *gop_map = queue->tx_map_ops;
	struct gnttab_copy *gop_copy = queue->tx_copy_ops;
	struct sk_buff *skb;
	int work_done = 0;

	while ((skb = __skb_dequeue(&queue->tx_queue)) != NULL) {
		struct xen_netif_tx_request *txp;
		u16 pending_idx;
		unsigned data_len;

		pending_idx = XENVIF_TX_CB(skb)->pending_idx;
		txp = &queue->pending_tx_info[pending_idx].req;

		/* Check the remap error code. */
		if (unlikely(xenvif_tx_check_gop(queue, skb, &gop_map, &gop_copy))) {
			/* If there was an error, xenvif_tx_check_gop is
			 * expected to release all the frags which were mapped,
			 * so kfree_skb shouldn't do it again
			 */
			skb_shinfo(skb)->nr_frags = 0;
			if (skb_has_frag_list(skb)) {
				struct sk_buff *nskb =
						skb_shinfo(skb)->frag_list;
				skb_shinfo(nskb)->nr_frags = 0;
			}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			kfree_skb(skb);
			continue;
		}

		data_len = skb->len;
<<<<<<< HEAD
		memcpy(skb->data,
		       (void *)(idx_to_kaddr(netbk, pending_idx)|txp->offset),
		       data_len);
=======
		callback_param(queue, pending_idx).ctx = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (data_len < txp->size) {
			/* Append the packet payload as a fragment. */
			txp->offset += data_len;
			txp->size -= data_len;
		} else {
			/* Schedule a response immediately. */
<<<<<<< HEAD
			xen_netbk_idx_release(netbk, pending_idx, XEN_NETIF_RSP_OKAY);
=======
			xenvif_idx_release(queue, pending_idx,
					   XEN_NETIF_RSP_OKAY);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		}

		if (txp->flags & XEN_NETTXF_csum_blank)
			skb->ip_summed = CHECKSUM_PARTIAL;
		else if (txp->flags & XEN_NETTXF_data_validated)
			skb->ip_summed = CHECKSUM_UNNECESSARY;

<<<<<<< HEAD
		xen_netbk_fill_frags(netbk, skb);

		/*
		 * If the initial fragment was < PKT_PROT_LEN then
		 * pull through some bytes from the other fragments to
		 * increase the linear region to PKT_PROT_LEN bytes.
		 */
		if (skb_headlen(skb) < PKT_PROT_LEN && skb_is_nonlinear(skb)) {
			int target = min_t(int, skb->len, PKT_PROT_LEN);
			__pskb_pull_tail(skb, target - skb_headlen(skb));
		}

		skb->dev      = vif->dev;
		skb->protocol = eth_type_trans(skb, skb->dev);
		skb_reset_network_header(skb);

		if (checksum_setup(vif, skb)) {
			netdev_dbg(vif->dev,
				   "Can't setup checksum in net_tx_action\n");
=======
		xenvif_fill_frags(queue, skb);

		if (unlikely(skb_has_frag_list(skb))) {
			struct sk_buff *nskb = skb_shinfo(skb)->frag_list;
			xenvif_skb_zerocopy_prepare(queue, nskb);
			if (xenvif_handle_frag_list(queue, skb)) {
				if (net_ratelimit())
					netdev_err(queue->vif->dev,
						   "Not enough memory to consolidate frag_list!\n");
				xenvif_skb_zerocopy_prepare(queue, skb);
				kfree_skb(skb);
				continue;
			}
			/* Copied all the bits from the frag list -- free it. */
			skb_frag_list_init(skb);
			kfree_skb(nskb);
		}

		skb->dev      = queue->vif->dev;
		skb->protocol = eth_type_trans(skb, skb->dev);
		skb_reset_network_header(skb);

		if (checksum_setup(queue, skb)) {
			netdev_dbg(queue->vif->dev,
				   "Can't setup checksum in net_tx_action\n");
			/* We have to set this flag to trigger the callback */
			if (skb_shinfo(skb)->destructor_arg)
				xenvif_skb_zerocopy_prepare(queue, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			kfree_skb(skb);
			continue;
		}

		skb_probe_transport_header(skb, 0);

<<<<<<< HEAD
		vif->dev->stats.rx_bytes += skb->len;
		vif->dev->stats.rx_packets++;

		xenvif_receive_skb(vif, skb);
	}
}

/* Called after netfront has transmitted */
static void xen_netbk_tx_action(struct xen_netbk *netbk)
{
	unsigned nr_gops;

	nr_gops = xen_netbk_tx_build_gops(netbk);

	if (nr_gops == 0)
		return;

	gnttab_batch_copy(netbk->tx_copy_ops, nr_gops);

	xen_netbk_tx_submit(netbk);
}

static void xen_netbk_idx_release(struct xen_netbk *netbk, u16 pending_idx,
				  u8 status)
{
	struct xenvif *vif;
	struct pending_tx_info *pending_tx_info;
	pending_ring_idx_t head;
	u16 peek; /* peek into next tx request */

	BUG_ON(netbk->mmap_pages[pending_idx] == (void *)(~0UL));

	/* Already complete? */
	if (netbk->mmap_pages[pending_idx] == NULL)
		return;

	pending_tx_info = &netbk->pending_tx_info[pending_idx];

	vif = pending_tx_info->vif;
	head = pending_tx_info->head;

	BUG_ON(!pending_tx_is_head(netbk, head));
	BUG_ON(netbk->pending_ring[pending_index(head)] != pending_idx);

	do {
		pending_ring_idx_t index;
		pending_ring_idx_t idx = pending_index(head);
		u16 info_idx = netbk->pending_ring[idx];

		pending_tx_info = &netbk->pending_tx_info[info_idx];
		make_tx_response(vif, &pending_tx_info->req, status);

		/* Setting any number other than
		 * INVALID_PENDING_RING_IDX indicates this slot is
		 * starting a new packet / ending a previous packet.
		 */
		pending_tx_info->head = 0;

		index = pending_index(netbk->pending_prod++);
		netbk->pending_ring[index] = netbk->pending_ring[info_idx];

		xenvif_put(vif);

		peek = netbk->pending_ring[pending_index(++head)];

	} while (!pending_tx_is_head(netbk, peek));

	netbk->mmap_pages[pending_idx]->mapping = 0;
	put_page(netbk->mmap_pages[pending_idx]);
	netbk->mmap_pages[pending_idx] = NULL;
}


static void make_tx_response(struct xenvif *vif,
			     struct xen_netif_tx_request *txp,
			     s8       st)
{
	RING_IDX i = vif->tx.rsp_prod_pvt;
	struct xen_netif_tx_response *resp;
	int notify;

	resp = RING_GET_RESPONSE(&vif->tx, i);
	resp->id     = txp->id;
	resp->status = st;

	if (txp->flags & XEN_NETTXF_extra_info)
		RING_GET_RESPONSE(&vif->tx, ++i)->status = XEN_NETIF_RSP_NULL;

	vif->tx.rsp_prod_pvt = ++i;
	RING_PUSH_RESPONSES_AND_CHECK_NOTIFY(&vif->tx, notify);
	if (notify)
		notify_remote_via_irq(vif->irq);
}

static struct xen_netif_rx_response *make_rx_response(struct xenvif *vif,
					     u16      id,
					     s8       st,
					     u16      offset,
					     u16      size,
					     u16      flags)
{
	RING_IDX i = vif->rx.rsp_prod_pvt;
	struct xen_netif_rx_response *resp;

	resp = RING_GET_RESPONSE(&vif->rx, i);
	resp->offset     = offset;
	resp->flags      = flags;
	resp->id         = id;
	resp->status     = (s16)size;
	if (st < 0)
		resp->status = (s16)st;

	vif->rx.rsp_prod_pvt = ++i;

	return resp;
}

static inline int rx_work_todo(struct xen_netbk *netbk)
{
	return !skb_queue_empty(&netbk->rx_queue);
}

static inline int tx_work_todo(struct xen_netbk *netbk)
{

	if ((nr_pending_reqs(netbk) + XEN_NETBK_LEGACY_SLOTS_MAX
	     < MAX_PENDING_REQS) &&
	     !list_empty(&netbk->net_schedule_list))
=======
		/* If the packet is GSO then we will have just set up the
		 * transport header offset in checksum_setup so it's now
		 * straightforward to calculate gso_segs.
		 */
		if (skb_is_gso(skb)) {
			int mss = skb_shinfo(skb)->gso_size;
			int hdrlen = skb_transport_header(skb) -
				skb_mac_header(skb) +
				tcp_hdrlen(skb);

			skb_shinfo(skb)->gso_segs =
				DIV_ROUND_UP(skb->len - hdrlen, mss);
		}

		queue->stats.rx_bytes += skb->len;
		queue->stats.rx_packets++;

		work_done++;

		/* Set this flag right before netif_receive_skb, otherwise
		 * someone might think this packet already left netback, and
		 * do a skb_copy_ubufs while we are still in control of the
		 * skb. E.g. the __pskb_pull_tail earlier can do such thing.
		 */
		if (skb_shinfo(skb)->destructor_arg) {
			xenvif_skb_zerocopy_prepare(queue, skb);
			queue->stats.tx_zerocopy_sent++;
		}

		netif_receive_skb(skb);
	}

	return work_done;
}

void xenvif_zerocopy_callback(struct ubuf_info *ubuf, bool zerocopy_success)
{
	unsigned long flags;
	pending_ring_idx_t index;
	struct xenvif_queue *queue = ubuf_to_queue(ubuf);

	/* This is the only place where we grab this lock, to protect callbacks
	 * from each other.
	 */
	spin_lock_irqsave(&queue->callback_lock, flags);
	do {
		u16 pending_idx = ubuf->desc;
		ubuf = (struct ubuf_info *) ubuf->ctx;
		BUG_ON(queue->dealloc_prod - queue->dealloc_cons >=
			MAX_PENDING_REQS);
		index = pending_index(queue->dealloc_prod);
		queue->dealloc_ring[index] = pending_idx;
		/* Sync with xenvif_tx_dealloc_action:
		 * insert idx then incr producer.
		 */
		smp_wmb();
		queue->dealloc_prod++;
	} while (ubuf);
	spin_unlock_irqrestore(&queue->callback_lock, flags);

	if (likely(zerocopy_success))
		queue->stats.tx_zerocopy_success++;
	else
		queue->stats.tx_zerocopy_fail++;
	xenvif_skb_zerocopy_complete(queue);
}

static inline void xenvif_tx_dealloc_action(struct xenvif_queue *queue)
{
	struct gnttab_unmap_grant_ref *gop;
	pending_ring_idx_t dc, dp;
	u16 pending_idx, pending_idx_release[MAX_PENDING_REQS];
	unsigned int i = 0;

	dc = queue->dealloc_cons;
	gop = queue->tx_unmap_ops;

	/* Free up any grants we have finished using */
	do {
		dp = queue->dealloc_prod;

		/* Ensure we see all indices enqueued by all
		 * xenvif_zerocopy_callback().
		 */
		smp_rmb();

		while (dc != dp) {
			BUG_ON(gop - queue->tx_unmap_ops >= MAX_PENDING_REQS);
			pending_idx =
				queue->dealloc_ring[pending_index(dc++)];

			pending_idx_release[gop - queue->tx_unmap_ops] =
				pending_idx;
			queue->pages_to_unmap[gop - queue->tx_unmap_ops] =
				queue->mmap_pages[pending_idx];
			gnttab_set_unmap_op(gop,
					    idx_to_kaddr(queue, pending_idx),
					    GNTMAP_host_map,
					    queue->grant_tx_handle[pending_idx]);
			xenvif_grant_handle_reset(queue, pending_idx);
			++gop;
		}

	} while (dp != queue->dealloc_prod);

	queue->dealloc_cons = dc;

	if (gop - queue->tx_unmap_ops > 0) {
		int ret;
		ret = gnttab_unmap_refs(queue->tx_unmap_ops,
					NULL,
					queue->pages_to_unmap,
					gop - queue->tx_unmap_ops);
		if (ret) {
			netdev_err(queue->vif->dev, "Unmap fail: nr_ops %tu ret %d\n",
				   gop - queue->tx_unmap_ops, ret);
			for (i = 0; i < gop - queue->tx_unmap_ops; ++i) {
				if (gop[i].status != GNTST_okay)
					netdev_err(queue->vif->dev,
						   " host_addr: 0x%llx handle: 0x%x status: %d\n",
						   gop[i].host_addr,
						   gop[i].handle,
						   gop[i].status);
			}
			BUG();
		}
	}

	for (i = 0; i < gop - queue->tx_unmap_ops; ++i)
		xenvif_idx_release(queue, pending_idx_release[i],
				   XEN_NETIF_RSP_OKAY);
}


/* Called after netfront has transmitted */
int xenvif_tx_action(struct xenvif_queue *queue, int budget)
{
	unsigned nr_mops, nr_cops = 0;
	int work_done, ret;

	if (unlikely(!tx_work_todo(queue)))
		return 0;

	xenvif_tx_build_gops(queue, budget, &nr_cops, &nr_mops);

	if (nr_cops == 0)
		return 0;

	gnttab_batch_copy(queue->tx_copy_ops, nr_cops);
	if (nr_mops != 0) {
		ret = gnttab_map_refs(queue->tx_map_ops,
				      NULL,
				      queue->pages_to_map,
				      nr_mops);
		if (ret) {
			unsigned int i;

			netdev_err(queue->vif->dev, "Map fail: nr %u ret %d\n",
				   nr_mops, ret);
			for (i = 0; i < nr_mops; ++i)
				WARN_ON_ONCE(queue->tx_map_ops[i].status ==
				             GNTST_okay);
		}
	}

	work_done = xenvif_tx_submit(queue);

	return work_done;
}

static void xenvif_idx_release(struct xenvif_queue *queue, u16 pending_idx,
			       u8 status)
{
	struct pending_tx_info *pending_tx_info;
	pending_ring_idx_t index;
	unsigned long flags;

	pending_tx_info = &queue->pending_tx_info[pending_idx];

	spin_lock_irqsave(&queue->response_lock, flags);

	make_tx_response(queue, &pending_tx_info->req,
			 pending_tx_info->extra_count, status);

	/* Release the pending index before pusing the Tx response so
	 * its available before a new Tx request is pushed by the
	 * frontend.
	 */
	index = pending_index(queue->pending_prod++);
	queue->pending_ring[index] = pending_idx;

	push_tx_responses(queue);

	spin_unlock_irqrestore(&queue->response_lock, flags);
}


static void make_tx_response(struct xenvif_queue *queue,
			     struct xen_netif_tx_request *txp,
			     unsigned int extra_count,
			     s8       st)
{
	RING_IDX i = queue->tx.rsp_prod_pvt;
	struct xen_netif_tx_response *resp;

	resp = RING_GET_RESPONSE(&queue->tx, i);
	resp->id     = txp->id;
	resp->status = st;

	while (extra_count-- != 0)
		RING_GET_RESPONSE(&queue->tx, ++i)->status = XEN_NETIF_RSP_NULL;

	queue->tx.rsp_prod_pvt = ++i;
}

static void push_tx_responses(struct xenvif_queue *queue)
{
	int notify;

	RING_PUSH_RESPONSES_AND_CHECK_NOTIFY(&queue->tx, notify);
	if (notify)
		notify_remote_via_irq(queue->tx_irq);
}

void xenvif_idx_unmap(struct xenvif_queue *queue, u16 pending_idx)
{
	int ret;
	struct gnttab_unmap_grant_ref tx_unmap_op;

	gnttab_set_unmap_op(&tx_unmap_op,
			    idx_to_kaddr(queue, pending_idx),
			    GNTMAP_host_map,
			    queue->grant_tx_handle[pending_idx]);
	xenvif_grant_handle_reset(queue, pending_idx);

	ret = gnttab_unmap_refs(&tx_unmap_op, NULL,
				&queue->mmap_pages[pending_idx], 1);
	if (ret) {
		netdev_err(queue->vif->dev,
			   "Unmap fail: ret: %d pending_idx: %d host_addr: %llx handle: 0x%x status: %d\n",
			   ret,
			   pending_idx,
			   tx_unmap_op.host_addr,
			   tx_unmap_op.handle,
			   tx_unmap_op.status);
		BUG();
	}
}

static inline int tx_work_todo(struct xenvif_queue *queue)
{
	if (likely(RING_HAS_UNCONSUMED_REQUESTS(&queue->tx)))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 1;

	return 0;
}

<<<<<<< HEAD
static int xen_netbk_kthread(void *data)
{
	struct xen_netbk *netbk = data;
	while (!kthread_should_stop()) {
		wait_event_interruptible(netbk->wq,
				rx_work_todo(netbk) ||
				tx_work_todo(netbk) ||
				kthread_should_stop());
		cond_resched();

		if (kthread_should_stop())
			break;

		if (rx_work_todo(netbk))
			xen_netbk_rx_action(netbk);

		if (tx_work_todo(netbk))
			xen_netbk_tx_action(netbk);
	}

	return 0;
}

void xen_netbk_unmap_frontend_rings(struct xenvif *vif)
{
	atomic_dec(&vif->ring_refcnt);
	wait_event(vif->waiting_to_unmap, atomic_read(&vif->ring_refcnt) == 0);

	if (vif->tx.sring)
		xenbus_unmap_ring_vfree(xenvif_to_xenbus_device(vif),
					vif->tx.sring);
	if (vif->rx.sring)
		xenbus_unmap_ring_vfree(xenvif_to_xenbus_device(vif),
					vif->rx.sring);
}

int xen_netbk_map_frontend_rings(struct xenvif *vif,
				 grant_ref_t tx_ring_ref,
				 grant_ref_t rx_ring_ref)
=======
static inline bool tx_dealloc_work_todo(struct xenvif_queue *queue)
{
	return queue->dealloc_cons != queue->dealloc_prod;
}

void xenvif_unmap_frontend_data_rings(struct xenvif_queue *queue)
{
	if (queue->tx.sring)
		xenbus_unmap_ring_vfree(xenvif_to_xenbus_device(queue->vif),
					queue->tx.sring);
	if (queue->rx.sring)
		xenbus_unmap_ring_vfree(xenvif_to_xenbus_device(queue->vif),
					queue->rx.sring);
}

int xenvif_map_frontend_data_rings(struct xenvif_queue *queue,
				   grant_ref_t tx_ring_ref,
				   grant_ref_t rx_ring_ref)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	void *addr;
	struct xen_netif_tx_sring *txs;
	struct xen_netif_rx_sring *rxs;

	int err = -ENOMEM;

<<<<<<< HEAD
	atomic_set(&vif->ring_refcnt, 1);

	err = xenbus_map_ring_valloc(xenvif_to_xenbus_device(vif),
				     tx_ring_ref, &addr);
=======
	err = xenbus_map_ring_valloc(xenvif_to_xenbus_device(queue->vif),
				     &tx_ring_ref, 1, &addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err)
		goto err;

	txs = (struct xen_netif_tx_sring *)addr;
<<<<<<< HEAD
	BACK_RING_INIT(&vif->tx, txs, PAGE_SIZE);

	err = xenbus_map_ring_valloc(xenvif_to_xenbus_device(vif),
				     rx_ring_ref, &addr);
=======
	BACK_RING_INIT(&queue->tx, txs, XEN_PAGE_SIZE);

	err = xenbus_map_ring_valloc(xenvif_to_xenbus_device(queue->vif),
				     &rx_ring_ref, 1, &addr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (err)
		goto err;

	rxs = (struct xen_netif_rx_sring *)addr;
<<<<<<< HEAD
	BACK_RING_INIT(&vif->rx, rxs, PAGE_SIZE);

	vif->rx_req_cons_peek = 0;
=======
	BACK_RING_INIT(&queue->rx, rxs, XEN_PAGE_SIZE);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;

err:
<<<<<<< HEAD
	xen_netbk_unmap_frontend_rings(vif);
	return err;
}

static int __init netback_init(void)
{
	int i;
	int rc = 0;
	int group;

	if (!xen_domain())
		return -ENODEV;

	if (fatal_skb_slots < XEN_NETBK_LEGACY_SLOTS_MAX) {
		printk(KERN_INFO
		       "xen-netback: fatal_skb_slots too small (%d), bump it to XEN_NETBK_LEGACY_SLOTS_MAX (%d)\n",
		       fatal_skb_slots, XEN_NETBK_LEGACY_SLOTS_MAX);
		fatal_skb_slots = XEN_NETBK_LEGACY_SLOTS_MAX;
	}

	xen_netbk_group_nr = num_online_cpus();
	xen_netbk = vzalloc(sizeof(struct xen_netbk) * xen_netbk_group_nr);
	if (!xen_netbk)
		return -ENOMEM;

	for (group = 0; group < xen_netbk_group_nr; group++) {
		struct xen_netbk *netbk = &xen_netbk[group];
		skb_queue_head_init(&netbk->rx_queue);
		skb_queue_head_init(&netbk->tx_queue);

		init_timer(&netbk->net_timer);
		netbk->net_timer.data = (unsigned long)netbk;
		netbk->net_timer.function = xen_netbk_alarm;

		netbk->pending_cons = 0;
		netbk->pending_prod = MAX_PENDING_REQS;
		for (i = 0; i < MAX_PENDING_REQS; i++)
			netbk->pending_ring[i] = i;

		init_waitqueue_head(&netbk->wq);
		netbk->task = kthread_create(xen_netbk_kthread,
					     (void *)netbk,
					     "netback/%u", group);

		if (IS_ERR(netbk->task)) {
			printk(KERN_ALERT "kthread_create() fails at netback\n");
			del_timer(&netbk->net_timer);
			rc = PTR_ERR(netbk->task);
			goto failed_init;
		}

		kthread_bind(netbk->task, group);

		INIT_LIST_HEAD(&netbk->net_schedule_list);

		spin_lock_init(&netbk->net_schedule_list_lock);

		atomic_set(&netbk->netfront_count, 0);

		wake_up_process(netbk->task);
=======
	xenvif_unmap_frontend_data_rings(queue);
	return err;
}

static bool xenvif_dealloc_kthread_should_stop(struct xenvif_queue *queue)
{
	/* Dealloc thread must remain running until all inflight
	 * packets complete.
	 */
	return kthread_should_stop() &&
		!atomic_read(&queue->inflight_packets);
}

int xenvif_dealloc_kthread(void *data)
{
	struct xenvif_queue *queue = data;

	for (;;) {
		wait_event_interruptible(queue->dealloc_wq,
					 tx_dealloc_work_todo(queue) ||
					 xenvif_dealloc_kthread_should_stop(queue));
		if (xenvif_dealloc_kthread_should_stop(queue))
			break;

		xenvif_tx_dealloc_action(queue);
		cond_resched();
	}

	/* Unmap anything remaining*/
	if (tx_dealloc_work_todo(queue))
		xenvif_tx_dealloc_action(queue);

	return 0;
}

static void make_ctrl_response(struct xenvif *vif,
			       const struct xen_netif_ctrl_request *req,
			       u32 status, u32 data)
{
	RING_IDX idx = vif->ctrl.rsp_prod_pvt;
	struct xen_netif_ctrl_response rsp = {
		.id = req->id,
		.type = req->type,
		.status = status,
		.data = data,
	};

	*RING_GET_RESPONSE(&vif->ctrl, idx) = rsp;
	vif->ctrl.rsp_prod_pvt = ++idx;
}

static void push_ctrl_response(struct xenvif *vif)
{
	int notify;

	RING_PUSH_RESPONSES_AND_CHECK_NOTIFY(&vif->ctrl, notify);
	if (notify)
		notify_remote_via_irq(vif->ctrl_irq);
}

static void process_ctrl_request(struct xenvif *vif,
				 const struct xen_netif_ctrl_request *req)
{
	u32 status = XEN_NETIF_CTRL_STATUS_NOT_SUPPORTED;
	u32 data = 0;

	switch (req->type) {
	case XEN_NETIF_CTRL_TYPE_SET_HASH_ALGORITHM:
		status = xenvif_set_hash_alg(vif, req->data[0]);
		break;

	case XEN_NETIF_CTRL_TYPE_GET_HASH_FLAGS:
		status = xenvif_get_hash_flags(vif, &data);
		break;

	case XEN_NETIF_CTRL_TYPE_SET_HASH_FLAGS:
		status = xenvif_set_hash_flags(vif, req->data[0]);
		break;

	case XEN_NETIF_CTRL_TYPE_SET_HASH_KEY:
		status = xenvif_set_hash_key(vif, req->data[0],
					     req->data[1]);
		break;

	case XEN_NETIF_CTRL_TYPE_GET_HASH_MAPPING_SIZE:
		status = XEN_NETIF_CTRL_STATUS_SUCCESS;
		data = XEN_NETBK_MAX_HASH_MAPPING_SIZE;
		break;

	case XEN_NETIF_CTRL_TYPE_SET_HASH_MAPPING_SIZE:
		status = xenvif_set_hash_mapping_size(vif,
						      req->data[0]);
		break;

	case XEN_NETIF_CTRL_TYPE_SET_HASH_MAPPING:
		status = xenvif_set_hash_mapping(vif, req->data[0],
						 req->data[1],
						 req->data[2]);
		break;

	default:
		break;
	}

	make_ctrl_response(vif, req, status, data);
	push_ctrl_response(vif);
}

static void xenvif_ctrl_action(struct xenvif *vif)
{
	for (;;) {
		RING_IDX req_prod, req_cons;

		req_prod = vif->ctrl.sring->req_prod;
		req_cons = vif->ctrl.req_cons;

		/* Make sure we can see requests before we process them. */
		rmb();

		if (req_cons == req_prod)
			break;

		while (req_cons != req_prod) {
			struct xen_netif_ctrl_request req;

			RING_COPY_REQUEST(&vif->ctrl, req_cons, &req);
			req_cons++;

			process_ctrl_request(vif, &req);
		}

		vif->ctrl.req_cons = req_cons;
		vif->ctrl.sring->req_event = req_cons + 1;
	}
}

static bool xenvif_ctrl_work_todo(struct xenvif *vif)
{
	if (likely(RING_HAS_UNCONSUMED_REQUESTS(&vif->ctrl)))
		return 1;

	return 0;
}

irqreturn_t xenvif_ctrl_irq_fn(int irq, void *data)
{
	struct xenvif *vif = data;
	unsigned int eoi_flag = XEN_EOI_FLAG_SPURIOUS;

	while (xenvif_ctrl_work_todo(vif)) {
		xenvif_ctrl_action(vif);
		eoi_flag = 0;
	}

	xen_irq_lateeoi(irq, eoi_flag);

	return IRQ_HANDLED;
}

static int __init netback_init(void)
{
	int rc = 0;

	if (!xen_domain())
		return -ENODEV;

	/* Allow as many queues as there are CPUs but max. 8 if user has not
	 * specified a value.
	 */
	if (xenvif_max_queues == 0)
		xenvif_max_queues = min_t(unsigned int, MAX_QUEUES_DEFAULT,
					  num_online_cpus());

	if (fatal_skb_slots < XEN_NETBK_LEGACY_SLOTS_MAX) {
		pr_info("fatal_skb_slots too small (%d), bump it to XEN_NETBK_LEGACY_SLOTS_MAX (%d)\n",
			fatal_skb_slots, XEN_NETBK_LEGACY_SLOTS_MAX);
		fatal_skb_slots = XEN_NETBK_LEGACY_SLOTS_MAX;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	rc = xenvif_xenbus_init();
	if (rc)
		goto failed_init;

<<<<<<< HEAD
	return 0;

failed_init:
	while (--group >= 0) {
		struct xen_netbk *netbk = &xen_netbk[group];
		for (i = 0; i < MAX_PENDING_REQS; i++) {
			if (netbk->mmap_pages[i])
				__free_page(netbk->mmap_pages[i]);
		}
		del_timer(&netbk->net_timer);
		kthread_stop(netbk->task);
	}
	vfree(xen_netbk);
	return rc;

=======
#ifdef CONFIG_DEBUG_FS
	xen_netback_dbg_root = debugfs_create_dir("xen-netback", NULL);
	if (IS_ERR_OR_NULL(xen_netback_dbg_root))
		pr_warn("Init of debugfs returned %ld!\n",
			PTR_ERR(xen_netback_dbg_root));
#endif /* CONFIG_DEBUG_FS */

	return 0;

failed_init:
	return rc;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

module_init(netback_init);

<<<<<<< HEAD
=======
static void __exit netback_fini(void)
{
#ifdef CONFIG_DEBUG_FS
	if (!IS_ERR_OR_NULL(xen_netback_dbg_root))
		debugfs_remove_recursive(xen_netback_dbg_root);
#endif /* CONFIG_DEBUG_FS */
	xenvif_xenbus_fini();
}
module_exit(netback_fini);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("xen-backend:vif");
