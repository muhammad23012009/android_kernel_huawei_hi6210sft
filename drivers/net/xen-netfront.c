/*
 * Virtual network driver for conversing with remote driver backends.
 *
 * Copyright (c) 2002-2005, K A Fraser
 * Copyright (c) 2005, XenSource Ltd
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

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/if_ether.h>
#include <net/tcp.h>
#include <linux/udp.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <net/ip.h>

<<<<<<< HEAD
#include <asm/xen/page.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <xen/xen.h>
#include <xen/xenbus.h>
#include <xen/events.h>
#include <xen/page.h>
#include <xen/platform_pci.h>
#include <xen/grant_table.h>

#include <xen/interface/io/netif.h>
#include <xen/interface/memory.h>
#include <xen/interface/grant_table.h>

<<<<<<< HEAD
=======
/* Module parameters */
static unsigned int xennet_max_queues;
module_param_named(max_queues, xennet_max_queues, uint, 0644);
MODULE_PARM_DESC(max_queues,
		 "Maximum number of queues per virtual interface");

#define XENNET_TIMEOUT  (5 * HZ)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const struct ethtool_ops xennet_ethtool_ops;

struct netfront_cb {
	int pull_to;
};

#define NETFRONT_SKB_CB(skb)	((struct netfront_cb *)((skb)->cb))

#define RX_COPY_THRESHOLD 256

#define GRANT_INVALID_REF	0

<<<<<<< HEAD
#define NET_TX_RING_SIZE __CONST_RING_SIZE(xen_netif_tx, PAGE_SIZE)
#define NET_RX_RING_SIZE __CONST_RING_SIZE(xen_netif_rx, PAGE_SIZE)
#define TX_MAX_TARGET min_t(int, NET_TX_RING_SIZE, 256)

struct netfront_stats {
	u64			rx_packets;
	u64			tx_packets;
	u64			rx_bytes;
	u64			tx_bytes;
	struct u64_stats_sync	syncp;
};

struct netfront_info {
	struct list_head list;
	struct net_device *netdev;

	struct napi_struct napi;

	unsigned int evtchn;
	struct xenbus_device *xbdev;
=======
#define NET_TX_RING_SIZE __CONST_RING_SIZE(xen_netif_tx, XEN_PAGE_SIZE)
#define NET_RX_RING_SIZE __CONST_RING_SIZE(xen_netif_rx, XEN_PAGE_SIZE)

/* Minimum number of Rx slots (includes slot for GSO metadata). */
#define NET_RX_SLOTS_MIN (XEN_NETIF_NR_SLOTS_MIN + 1)

/* Queue name is interface name with "-qNNN" appended */
#define QUEUE_NAME_SIZE (IFNAMSIZ + 6)

/* IRQ name is queue name with "-tx" or "-rx" appended */
#define IRQ_NAME_SIZE (QUEUE_NAME_SIZE + 3)

static DECLARE_WAIT_QUEUE_HEAD(module_wq);

struct netfront_stats {
	u64			packets;
	u64			bytes;
	struct u64_stats_sync	syncp;
};

struct netfront_info;

struct netfront_queue {
	unsigned int id; /* Queue ID, 0-based */
	char name[QUEUE_NAME_SIZE]; /* DEVNAME-qN */
	struct netfront_info *info;

	struct napi_struct napi;

	/* Split event channels support, tx_* == rx_* when using
	 * single event channel.
	 */
	unsigned int tx_evtchn, rx_evtchn;
	unsigned int tx_irq, rx_irq;
	/* Only used when split event channels support is enabled */
	char tx_irq_name[IRQ_NAME_SIZE]; /* DEVNAME-qN-tx */
	char rx_irq_name[IRQ_NAME_SIZE]; /* DEVNAME-qN-rx */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spinlock_t   tx_lock;
	struct xen_netif_tx_front_ring tx;
	int tx_ring_ref;

	/*
	 * {tx,rx}_skbs store outstanding skbuffs. Free tx_skb entries
	 * are linked from tx_skb_freelist through skb_entry.link.
	 *
	 *  NB. Freelist index entries are always going to be less than
	 *  PAGE_OFFSET, whereas pointers to skbs will always be equal or
	 *  greater than PAGE_OFFSET: we use this property to distinguish
	 *  them.
	 */
	union skb_entry {
		struct sk_buff *skb;
		unsigned long link;
	} tx_skbs[NET_TX_RING_SIZE];
	grant_ref_t gref_tx_head;
	grant_ref_t grant_tx_ref[NET_TX_RING_SIZE];
	struct page *grant_tx_page[NET_TX_RING_SIZE];
	unsigned tx_skb_freelist;

	spinlock_t   rx_lock ____cacheline_aligned_in_smp;
	struct xen_netif_rx_front_ring rx;
	int rx_ring_ref;

<<<<<<< HEAD
	/* Receive-ring batched refills. */
#define RX_MIN_TARGET 8
#define RX_DFL_MIN_TARGET 64
#define RX_MAX_TARGET min_t(int, NET_RX_RING_SIZE, 256)
	unsigned rx_min_target, rx_max_target, rx_target;
	struct sk_buff_head rx_batch;

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct timer_list rx_refill_timer;

	struct sk_buff *rx_skbs[NET_RX_RING_SIZE];
	grant_ref_t gref_rx_head;
	grant_ref_t grant_rx_ref[NET_RX_RING_SIZE];
<<<<<<< HEAD

	unsigned long rx_pfn_array[NET_RX_RING_SIZE];
	struct multicall_entry rx_mcl[NET_RX_RING_SIZE+1];
	struct mmu_update rx_mmu[NET_RX_RING_SIZE];

	/* Statistics */
	struct netfront_stats __percpu *stats;

	unsigned long rx_gso_checksum_fixup;
=======
};

struct netfront_info {
	struct list_head list;
	struct net_device *netdev;

	struct xenbus_device *xbdev;

	/* Multi-queue support */
	struct netfront_queue *queues;

	/* Statistics */
	struct netfront_stats __percpu *rx_stats;
	struct netfront_stats __percpu *tx_stats;

	atomic_t rx_gso_checksum_fixup;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

struct netfront_rx_info {
	struct xen_netif_rx_response rx;
	struct xen_netif_extra_info extras[XEN_NETIF_EXTRA_TYPE_MAX - 1];
};

static void skb_entry_set_link(union skb_entry *list, unsigned short id)
{
	list->link = id;
}

static int skb_entry_is_link(const union skb_entry *list)
{
	BUILD_BUG_ON(sizeof(list->skb) != sizeof(list->link));
	return (unsigned long)list->skb < PAGE_OFFSET;
}

/*
 * Access macros for acquiring freeing slots in tx_skbs[].
 */

static void add_id_to_freelist(unsigned *head, union skb_entry *list,
			       unsigned short id)
{
	skb_entry_set_link(&list[id], *head);
	*head = id;
}

static unsigned short get_id_from_freelist(unsigned *head,
					   union skb_entry *list)
{
	unsigned int id = *head;
	*head = list[id].link;
	return id;
}

static int xennet_rxidx(RING_IDX idx)
{
	return idx & (NET_RX_RING_SIZE - 1);
}

<<<<<<< HEAD
static struct sk_buff *xennet_get_rx_skb(struct netfront_info *np,
					 RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	struct sk_buff *skb = np->rx_skbs[i];
	np->rx_skbs[i] = NULL;
	return skb;
}

static grant_ref_t xennet_get_rx_ref(struct netfront_info *np,
					    RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	grant_ref_t ref = np->grant_rx_ref[i];
	np->grant_rx_ref[i] = GRANT_INVALID_REF;
=======
static struct sk_buff *xennet_get_rx_skb(struct netfront_queue *queue,
					 RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	struct sk_buff *skb = queue->rx_skbs[i];
	queue->rx_skbs[i] = NULL;
	return skb;
}

static grant_ref_t xennet_get_rx_ref(struct netfront_queue *queue,
					    RING_IDX ri)
{
	int i = xennet_rxidx(ri);
	grant_ref_t ref = queue->grant_rx_ref[i];
	queue->grant_rx_ref[i] = GRANT_INVALID_REF;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ref;
}

#ifdef CONFIG_SYSFS
<<<<<<< HEAD
static int xennet_sysfs_addif(struct net_device *netdev);
static void xennet_sysfs_delif(struct net_device *netdev);
#else /* !CONFIG_SYSFS */
#define xennet_sysfs_addif(dev) (0)
#define xennet_sysfs_delif(dev) do { } while (0)
=======
static const struct attribute_group xennet_dev_group;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

static bool xennet_can_sg(struct net_device *dev)
{
	return dev->features & NETIF_F_SG;
}


static void rx_refill_timeout(unsigned long data)
{
<<<<<<< HEAD
	struct net_device *dev = (struct net_device *)data;
	struct netfront_info *np = netdev_priv(dev);
	napi_schedule(&np->napi);
}

static int netfront_tx_slot_available(struct netfront_info *np)
{
	return (np->tx.req_prod_pvt - np->tx.rsp_cons) <
		(TX_MAX_TARGET - MAX_SKB_FRAGS - 2);
}

static void xennet_maybe_wake_tx(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);

	if (unlikely(netif_queue_stopped(dev)) &&
	    netfront_tx_slot_available(np) &&
	    likely(netif_running(dev)))
		netif_wake_queue(dev);
}

static void xennet_alloc_rx_buffers(struct net_device *dev)
{
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct sk_buff *skb;
	struct page *page;
	int i, batch_target, notify;
	RING_IDX req_prod = np->rx.req_prod_pvt;
	grant_ref_t ref;
	unsigned long pfn;
	void *vaddr;
	struct xen_netif_rx_request *req;

	if (unlikely(!netif_carrier_ok(dev)))
		return;

	/*
	 * Allocate skbuffs greedily, even though we batch updates to the
	 * receive ring. This creates a less bursty demand on the memory
	 * allocator, so should reduce the chance of failed allocation requests
	 * both for ourself and for other kernel subsystems.
	 */
	batch_target = np->rx_target - (req_prod - np->rx.rsp_cons);
	for (i = skb_queue_len(&np->rx_batch); i < batch_target; i++) {
		skb = __netdev_alloc_skb(dev, RX_COPY_THRESHOLD + NET_IP_ALIGN,
					 GFP_ATOMIC | __GFP_NOWARN);
		if (unlikely(!skb))
			goto no_skb;

		/* Align ip header to a 16 bytes boundary */
		skb_reserve(skb, NET_IP_ALIGN);

		page = alloc_page(GFP_ATOMIC | __GFP_NOWARN);
		if (!page) {
			kfree_skb(skb);
no_skb:
			/* Any skbuffs queued for refill? Force them out. */
			if (i != 0)
				goto refill;
			/* Could not allocate any skbuffs. Try again later. */
			mod_timer(&np->rx_refill_timer,
				  jiffies + (HZ/10));
			break;
		}

		skb_add_rx_frag(skb, 0, page, 0, 0, PAGE_SIZE);
		__skb_queue_tail(&np->rx_batch, skb);
	}

	/* Is the batch large enough to be worthwhile? */
	if (i < (np->rx_target/2)) {
		if (req_prod > np->rx.sring->req_prod)
			goto push;
		return;
	}

	/* Adjust our fill target if we risked running out of buffers. */
	if (((req_prod - np->rx.sring->rsp_prod) < (np->rx_target / 4)) &&
	    ((np->rx_target *= 2) > np->rx_max_target))
		np->rx_target = np->rx_max_target;

 refill:
	for (i = 0; ; i++) {
		skb = __skb_dequeue(&np->rx_batch);
		if (skb == NULL)
			break;

		skb->dev = dev;

		id = xennet_rxidx(req_prod + i);

		BUG_ON(np->rx_skbs[id]);
		np->rx_skbs[id] = skb;

		ref = gnttab_claim_grant_reference(&np->gref_rx_head);
		BUG_ON((signed short)ref < 0);
		np->grant_rx_ref[id] = ref;

		pfn = page_to_pfn(skb_frag_page(&skb_shinfo(skb)->frags[0]));
		vaddr = page_address(skb_frag_page(&skb_shinfo(skb)->frags[0]));

		req = RING_GET_REQUEST(&np->rx, req_prod + i);
		gnttab_grant_foreign_access_ref(ref,
						np->xbdev->otherend_id,
						pfn_to_mfn(pfn),
						0);

=======
	struct netfront_queue *queue = (struct netfront_queue *)data;
	napi_schedule(&queue->napi);
}

static int netfront_tx_slot_available(struct netfront_queue *queue)
{
	return (queue->tx.req_prod_pvt - queue->tx.rsp_cons) <
		(NET_TX_RING_SIZE - XEN_NETIF_NR_SLOTS_MIN - 1);
}

static void xennet_maybe_wake_tx(struct netfront_queue *queue)
{
	struct net_device *dev = queue->info->netdev;
	struct netdev_queue *dev_queue = netdev_get_tx_queue(dev, queue->id);

	if (unlikely(netif_tx_queue_stopped(dev_queue)) &&
	    netfront_tx_slot_available(queue) &&
	    likely(netif_running(dev)))
		netif_tx_wake_queue(netdev_get_tx_queue(dev, queue->id));
}


static struct sk_buff *xennet_alloc_one_rx_buffer(struct netfront_queue *queue)
{
	struct sk_buff *skb;
	struct page *page;

	skb = __netdev_alloc_skb(queue->info->netdev,
				 RX_COPY_THRESHOLD + NET_IP_ALIGN,
				 GFP_ATOMIC | __GFP_NOWARN);
	if (unlikely(!skb))
		return NULL;

	page = alloc_page(GFP_ATOMIC | __GFP_NOWARN);
	if (!page) {
		kfree_skb(skb);
		return NULL;
	}
	skb_add_rx_frag(skb, 0, page, 0, 0, PAGE_SIZE);

	/* Align ip header to a 16 bytes boundary */
	skb_reserve(skb, NET_IP_ALIGN);
	skb->dev = queue->info->netdev;

	return skb;
}


static void xennet_alloc_rx_buffers(struct netfront_queue *queue)
{
	RING_IDX req_prod = queue->rx.req_prod_pvt;
	int notify;
	int err = 0;

	if (unlikely(!netif_carrier_ok(queue->info->netdev)))
		return;

	for (req_prod = queue->rx.req_prod_pvt;
	     req_prod - queue->rx.rsp_cons < NET_RX_RING_SIZE;
	     req_prod++) {
		struct sk_buff *skb;
		unsigned short id;
		grant_ref_t ref;
		struct page *page;
		struct xen_netif_rx_request *req;

		skb = xennet_alloc_one_rx_buffer(queue);
		if (!skb) {
			err = -ENOMEM;
			break;
		}

		id = xennet_rxidx(req_prod);

		BUG_ON(queue->rx_skbs[id]);
		queue->rx_skbs[id] = skb;

		ref = gnttab_claim_grant_reference(&queue->gref_rx_head);
		WARN_ON_ONCE(IS_ERR_VALUE((unsigned long)(int)ref));
		queue->grant_rx_ref[id] = ref;

		page = skb_frag_page(&skb_shinfo(skb)->frags[0]);

		req = RING_GET_REQUEST(&queue->rx, req_prod);
		gnttab_page_grant_foreign_access_ref_one(ref,
							 queue->info->xbdev->otherend_id,
							 page,
							 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		req->id = id;
		req->gref = ref;
	}

<<<<<<< HEAD
	wmb();		/* barrier so backend seens requests */

	/* Above is a suitable barrier to ensure backend will see requests. */
	np->rx.req_prod_pvt = req_prod + i;
 push:
	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->rx, notify);
	if (notify)
		notify_remote_via_irq(np->netdev->irq);
=======
	queue->rx.req_prod_pvt = req_prod;

	/* Try again later if there are not enough requests or skb allocation
	 * failed.
	 * Enough requests is quantified as the sum of newly created slots and
	 * the unconsumed slots at the backend.
	 */
	if (req_prod - queue->rx.rsp_cons < NET_RX_SLOTS_MIN ||
	    unlikely(err)) {
		mod_timer(&queue->rx_refill_timer, jiffies + (HZ/10));
		return;
	}

	wmb();		/* barrier so backend seens requests */

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&queue->rx, notify);
	if (notify)
		notify_remote_via_irq(queue->rx_irq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int xennet_open(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD

	napi_enable(&np->napi);

	spin_lock_bh(&np->rx_lock);
	if (netif_carrier_ok(dev)) {
		xennet_alloc_rx_buffers(dev);
		np->rx.sring->rsp_event = np->rx.rsp_cons + 1;
		if (RING_HAS_UNCONSUMED_RESPONSES(&np->rx))
			napi_schedule(&np->napi);
	}
	spin_unlock_bh(&np->rx_lock);

	netif_start_queue(dev);
=======
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i = 0;
	struct netfront_queue *queue = NULL;

	if (!np->queues)
		return -ENODEV;

	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		napi_enable(&queue->napi);

		spin_lock_bh(&queue->rx_lock);
		if (netif_carrier_ok(dev)) {
			xennet_alloc_rx_buffers(queue);
			queue->rx.sring->rsp_event = queue->rx.rsp_cons + 1;
			if (RING_HAS_UNCONSUMED_RESPONSES(&queue->rx))
				napi_schedule(&queue->napi);
		}
		spin_unlock_bh(&queue->rx_lock);
	}

	netif_tx_start_all_queues(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static void xennet_tx_buf_gc(struct net_device *dev)
{
	RING_IDX cons, prod;
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct sk_buff *skb;

	BUG_ON(!netif_carrier_ok(dev));

	do {
		prod = np->tx.sring->rsp_prod;
		rmb(); /* Ensure we see responses up to 'rp'. */

		for (cons = np->tx.rsp_cons; cons != prod; cons++) {
			struct xen_netif_tx_response *txrsp;

			txrsp = RING_GET_RESPONSE(&np->tx, cons);
=======
static void xennet_tx_buf_gc(struct netfront_queue *queue)
{
	RING_IDX cons, prod;
	unsigned short id;
	struct sk_buff *skb;
	bool more_to_do;

	BUG_ON(!netif_carrier_ok(queue->info->netdev));

	do {
		prod = queue->tx.sring->rsp_prod;
		rmb(); /* Ensure we see responses up to 'rp'. */

		for (cons = queue->tx.rsp_cons; cons != prod; cons++) {
			struct xen_netif_tx_response *txrsp;

			txrsp = RING_GET_RESPONSE(&queue->tx, cons);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			if (txrsp->status == XEN_NETIF_RSP_NULL)
				continue;

			id  = txrsp->id;
<<<<<<< HEAD
			skb = np->tx_skbs[id].skb;
			if (unlikely(gnttab_query_foreign_access(
				np->grant_tx_ref[id]) != 0)) {
				printk(KERN_ALERT "xennet_tx_buf_gc: warning "
				       "-- grant still in use by backend "
				       "domain.\n");
				BUG();
			}
			gnttab_end_foreign_access_ref(
				np->grant_tx_ref[id], GNTMAP_readonly);
			gnttab_release_grant_reference(
				&np->gref_tx_head, np->grant_tx_ref[id]);
			np->grant_tx_ref[id] = GRANT_INVALID_REF;
			np->grant_tx_page[id] = NULL;
			add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, id);
			dev_kfree_skb_irq(skb);
		}

		np->tx.rsp_cons = prod;

		/*
		 * Set a new event, then check for race with update of tx_cons.
		 * Note that it is essential to schedule a callback, no matter
		 * how few buffers are pending. Even if there is space in the
		 * transmit ring, higher layers may be blocked because too much
		 * data is outstanding: in such cases notification from Xen is
		 * likely to be the only kick that we'll get.
		 */
		np->tx.sring->rsp_event =
			prod + ((np->tx.sring->req_prod - prod) >> 1) + 1;
		mb();		/* update shared area */
	} while ((cons == prod) && (prod != np->tx.sring->rsp_prod));

	xennet_maybe_wake_tx(dev);
}

static void xennet_make_frags(struct sk_buff *skb, struct net_device *dev,
			      struct xen_netif_tx_request *tx)
{
	struct netfront_info *np = netdev_priv(dev);
	char *data = skb->data;
	unsigned long mfn;
	RING_IDX prod = np->tx.req_prod_pvt;
	int frags = skb_shinfo(skb)->nr_frags;
	unsigned int offset = offset_in_page(data);
	unsigned int len = skb_headlen(skb);
	unsigned int id;
	grant_ref_t ref;
	int i;

	/* While the header overlaps a page boundary (including being
	   larger than a page), split it it into page-sized chunks. */
	while (len > PAGE_SIZE - offset) {
		tx->size = PAGE_SIZE - offset;
		tx->flags |= XEN_NETTXF_more_data;
		len -= tx->size;
		data += tx->size;
		offset = 0;

		id = get_id_from_freelist(&np->tx_skb_freelist, np->tx_skbs);
		np->tx_skbs[id].skb = skb_get(skb);
		tx = RING_GET_REQUEST(&np->tx, prod++);
		tx->id = id;
		ref = gnttab_claim_grant_reference(&np->gref_tx_head);
		BUG_ON((signed short)ref < 0);

		mfn = virt_to_mfn(data);
		gnttab_grant_foreign_access_ref(ref, np->xbdev->otherend_id,
						mfn, GNTMAP_readonly);

		np->grant_tx_page[id] = virt_to_page(data);
		tx->gref = np->grant_tx_ref[id] = ref;
		tx->offset = offset;
		tx->size = len;
		tx->flags = 0;
	}

	/* Grant backend access to each skb fragment page. */
	for (i = 0; i < frags; i++) {
		skb_frag_t *frag = skb_shinfo(skb)->frags + i;
		struct page *page = skb_frag_page(frag);

		len = skb_frag_size(frag);
		offset = frag->page_offset;

		/* Skip unused frames from start of page */
		page += offset >> PAGE_SHIFT;
		offset &= ~PAGE_MASK;

		while (len > 0) {
			unsigned long bytes;

			bytes = PAGE_SIZE - offset;
			if (bytes > len)
				bytes = len;

			tx->flags |= XEN_NETTXF_more_data;

			id = get_id_from_freelist(&np->tx_skb_freelist,
						  np->tx_skbs);
			np->tx_skbs[id].skb = skb_get(skb);
			tx = RING_GET_REQUEST(&np->tx, prod++);
			tx->id = id;
			ref = gnttab_claim_grant_reference(&np->gref_tx_head);
			BUG_ON((signed short)ref < 0);

			mfn = pfn_to_mfn(page_to_pfn(page));
			gnttab_grant_foreign_access_ref(ref,
							np->xbdev->otherend_id,
							mfn, GNTMAP_readonly);

			np->grant_tx_page[id] = page;
			tx->gref = np->grant_tx_ref[id] = ref;
			tx->offset = offset;
			tx->size = bytes;
			tx->flags = 0;

			offset += bytes;
			len -= bytes;

			/* Next frame */
			if (offset == PAGE_SIZE && len) {
				BUG_ON(!PageCompound(page));
				page++;
				offset = 0;
			}
		}
	}

	np->tx.req_prod_pvt = prod;
}

/*
 * Count how many ring slots are required to send the frags of this
 * skb. Each frag might be a compound page.
 */
static int xennet_count_skb_frag_slots(struct sk_buff *skb)
{
	int i, frags = skb_shinfo(skb)->nr_frags;
	int pages = 0;
=======
			skb = queue->tx_skbs[id].skb;
			if (unlikely(gnttab_query_foreign_access(
				queue->grant_tx_ref[id]) != 0)) {
				pr_alert("%s: warning -- grant still in use by backend domain\n",
					 __func__);
				BUG();
			}
			gnttab_end_foreign_access_ref(
				queue->grant_tx_ref[id], GNTMAP_readonly);
			gnttab_release_grant_reference(
				&queue->gref_tx_head, queue->grant_tx_ref[id]);
			queue->grant_tx_ref[id] = GRANT_INVALID_REF;
			queue->grant_tx_page[id] = NULL;
			add_id_to_freelist(&queue->tx_skb_freelist, queue->tx_skbs, id);
			dev_kfree_skb_irq(skb);
		}

		queue->tx.rsp_cons = prod;

		RING_FINAL_CHECK_FOR_RESPONSES(&queue->tx, more_to_do);
	} while (more_to_do);

	xennet_maybe_wake_tx(queue);
}

struct xennet_gnttab_make_txreq {
	struct netfront_queue *queue;
	struct sk_buff *skb;
	struct page *page;
	struct xen_netif_tx_request *tx; /* Last request */
	unsigned int size;
};

static void xennet_tx_setup_grant(unsigned long gfn, unsigned int offset,
				  unsigned int len, void *data)
{
	struct xennet_gnttab_make_txreq *info = data;
	unsigned int id;
	struct xen_netif_tx_request *tx;
	grant_ref_t ref;
	/* convenient aliases */
	struct page *page = info->page;
	struct netfront_queue *queue = info->queue;
	struct sk_buff *skb = info->skb;

	id = get_id_from_freelist(&queue->tx_skb_freelist, queue->tx_skbs);
	tx = RING_GET_REQUEST(&queue->tx, queue->tx.req_prod_pvt++);
	ref = gnttab_claim_grant_reference(&queue->gref_tx_head);
	WARN_ON_ONCE(IS_ERR_VALUE((unsigned long)(int)ref));

	gnttab_grant_foreign_access_ref(ref, queue->info->xbdev->otherend_id,
					gfn, GNTMAP_readonly);

	queue->tx_skbs[id].skb = skb;
	queue->grant_tx_page[id] = page;
	queue->grant_tx_ref[id] = ref;

	tx->id = id;
	tx->gref = ref;
	tx->offset = offset;
	tx->size = len;
	tx->flags = 0;

	info->tx = tx;
	info->size += tx->size;
}

static struct xen_netif_tx_request *xennet_make_first_txreq(
	struct netfront_queue *queue, struct sk_buff *skb,
	struct page *page, unsigned int offset, unsigned int len)
{
	struct xennet_gnttab_make_txreq info = {
		.queue = queue,
		.skb = skb,
		.page = page,
		.size = 0,
	};

	gnttab_for_one_grant(page, offset, len, xennet_tx_setup_grant, &info);

	return info.tx;
}

static void xennet_make_one_txreq(unsigned long gfn, unsigned int offset,
				  unsigned int len, void *data)
{
	struct xennet_gnttab_make_txreq *info = data;

	info->tx->flags |= XEN_NETTXF_more_data;
	skb_get(info->skb);
	xennet_tx_setup_grant(gfn, offset, len, data);
}

static struct xen_netif_tx_request *xennet_make_txreqs(
	struct netfront_queue *queue, struct xen_netif_tx_request *tx,
	struct sk_buff *skb, struct page *page,
	unsigned int offset, unsigned int len)
{
	struct xennet_gnttab_make_txreq info = {
		.queue = queue,
		.skb = skb,
		.tx = tx,
	};

	/* Skip unused frames from start of page */
	page += offset >> PAGE_SHIFT;
	offset &= ~PAGE_MASK;

	while (len) {
		info.page = page;
		info.size = 0;

		gnttab_foreach_grant_in_range(page, offset, len,
					      xennet_make_one_txreq,
					      &info);

		page++;
		offset = 0;
		len -= info.size;
	}

	return info.tx;
}

/*
 * Count how many ring slots are required to send this skb. Each frag
 * might be a compound page.
 */
static int xennet_count_skb_slots(struct sk_buff *skb)
{
	int i, frags = skb_shinfo(skb)->nr_frags;
	int slots;

	slots = gnttab_count_grant(offset_in_page(skb->data),
				   skb_headlen(skb));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (i = 0; i < frags; i++) {
		skb_frag_t *frag = skb_shinfo(skb)->frags + i;
		unsigned long size = skb_frag_size(frag);
		unsigned long offset = frag->page_offset;

		/* Skip unused frames from start of page */
		offset &= ~PAGE_MASK;

<<<<<<< HEAD
		pages += PFN_UP(offset + size);
	}

	return pages;
}

static int xennet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	unsigned short id;
	struct netfront_info *np = netdev_priv(dev);
	struct netfront_stats *stats = this_cpu_ptr(np->stats);
	struct xen_netif_tx_request *tx;
	char *data = skb->data;
	RING_IDX i;
	grant_ref_t ref;
	unsigned long mfn;
	int notify;
	int slots;
	unsigned int offset = offset_in_page(data);
	unsigned int len = skb_headlen(skb);
	unsigned long flags;
=======
		slots += gnttab_count_grant(offset, size);
	}

	return slots;
}

static u16 xennet_select_queue(struct net_device *dev, struct sk_buff *skb,
			       void *accel_priv, select_queue_fallback_t fallback)
{
	unsigned int num_queues = dev->real_num_tx_queues;
	u32 hash;
	u16 queue_idx;

	/* First, check if there is only one queue */
	if (num_queues == 1) {
		queue_idx = 0;
	} else {
		hash = skb_get_hash(skb);
		queue_idx = hash % num_queues;
	}

	return queue_idx;
}

#define MAX_XEN_SKB_FRAGS (65536 / XEN_PAGE_SIZE + 1)

static int xennet_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
	struct netfront_stats *tx_stats = this_cpu_ptr(np->tx_stats);
	struct xen_netif_tx_request *tx, *first_tx;
	unsigned int i;
	int notify;
	int slots;
	struct page *page;
	unsigned int offset;
	unsigned int len;
	unsigned long flags;
	struct netfront_queue *queue = NULL;
	unsigned int num_queues = dev->real_num_tx_queues;
	u16 queue_index;
	struct sk_buff *nskb;

	/* Drop the packet if no queues are set up */
	if (num_queues < 1)
		goto drop;
	/* Determine which queue to transmit this SKB on */
	queue_index = skb_get_queue_mapping(skb);
	queue = &np->queues[queue_index];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* If skb->len is too big for wire format, drop skb and alert
	 * user about misconfiguration.
	 */
	if (unlikely(skb->len > XEN_NETIF_MAX_TX_SIZE)) {
		net_alert_ratelimited(
			"xennet: skb->len = %u, too big for wire format\n",
			skb->len);
		goto drop;
	}

<<<<<<< HEAD
	slots = DIV_ROUND_UP(offset + len, PAGE_SIZE) +
		xennet_count_skb_frag_slots(skb);
	if (unlikely(slots > MAX_SKB_FRAGS + 1)) {
		net_alert_ratelimited(
			"xennet: skb rides the rocket: %d slots\n", slots);
		goto drop;
	}

	spin_lock_irqsave(&np->tx_lock, flags);
=======
	slots = xennet_count_skb_slots(skb);
	if (unlikely(slots > MAX_XEN_SKB_FRAGS + 1)) {
		net_dbg_ratelimited("xennet: skb rides the rocket: %d slots, %d bytes\n",
				    slots, skb->len);
		if (skb_linearize(skb))
			goto drop;
	}

	page = virt_to_page(skb->data);
	offset = offset_in_page(skb->data);

	/* The first req should be at least ETH_HLEN size or the packet will be
	 * dropped by netback.
	 */
	if (unlikely(PAGE_SIZE - offset < ETH_HLEN)) {
		nskb = skb_copy(skb, GFP_ATOMIC);
		if (!nskb)
			goto drop;
		dev_kfree_skb_any(skb);
		skb = nskb;
		page = virt_to_page(skb->data);
		offset = offset_in_page(skb->data);
	}

	len = skb_headlen(skb);

	spin_lock_irqsave(&queue->tx_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (unlikely(!netif_carrier_ok(dev) ||
		     (slots > 1 && !xennet_can_sg(dev)) ||
		     netif_needs_gso(skb, netif_skb_features(skb)))) {
<<<<<<< HEAD
		spin_unlock_irqrestore(&np->tx_lock, flags);
		goto drop;
	}

	i = np->tx.req_prod_pvt;

	id = get_id_from_freelist(&np->tx_skb_freelist, np->tx_skbs);
	np->tx_skbs[id].skb = skb;

	tx = RING_GET_REQUEST(&np->tx, i);

	tx->id   = id;
	ref = gnttab_claim_grant_reference(&np->gref_tx_head);
	BUG_ON((signed short)ref < 0);
	mfn = virt_to_mfn(data);
	gnttab_grant_foreign_access_ref(
		ref, np->xbdev->otherend_id, mfn, GNTMAP_readonly);
	np->grant_tx_page[id] = virt_to_page(data);
	tx->gref = np->grant_tx_ref[id] = ref;
	tx->offset = offset;
	tx->size = len;

	tx->flags = 0;
=======
		spin_unlock_irqrestore(&queue->tx_lock, flags);
		goto drop;
	}

	/* First request for the linear area. */
	first_tx = tx = xennet_make_first_txreq(queue, skb,
						page, offset, len);
	offset += tx->size;
	if (offset == PAGE_SIZE) {
		page++;
		offset = 0;
	}
	len -= tx->size;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		/* local packet? */
		tx->flags |= XEN_NETTXF_csum_blank | XEN_NETTXF_data_validated;
	else if (skb->ip_summed == CHECKSUM_UNNECESSARY)
		/* remote but checksummed. */
		tx->flags |= XEN_NETTXF_data_validated;

<<<<<<< HEAD
=======
	/* Optional extra info after the first request. */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (skb_shinfo(skb)->gso_size) {
		struct xen_netif_extra_info *gso;

		gso = (struct xen_netif_extra_info *)
<<<<<<< HEAD
			RING_GET_REQUEST(&np->tx, ++i);
=======
			RING_GET_REQUEST(&queue->tx, queue->tx.req_prod_pvt++);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		tx->flags |= XEN_NETTXF_extra_info;

		gso->u.gso.size = skb_shinfo(skb)->gso_size;
<<<<<<< HEAD
		gso->u.gso.type = XEN_NETIF_GSO_TYPE_TCPV4;
=======
		gso->u.gso.type = (skb_shinfo(skb)->gso_type & SKB_GSO_TCPV6) ?
			XEN_NETIF_GSO_TYPE_TCPV6 :
			XEN_NETIF_GSO_TYPE_TCPV4;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		gso->u.gso.pad = 0;
		gso->u.gso.features = 0;

		gso->type = XEN_NETIF_EXTRA_TYPE_GSO;
		gso->flags = 0;
	}

<<<<<<< HEAD
	np->tx.req_prod_pvt = i + 1;

	xennet_make_frags(skb, dev, tx);
	tx->size = skb->len;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&np->tx, notify);
	if (notify)
		notify_remote_via_irq(np->netdev->irq);

	u64_stats_update_begin(&stats->syncp);
	stats->tx_bytes += skb->len;
	stats->tx_packets++;
	u64_stats_update_end(&stats->syncp);

	/* Note: It is not safe to access skb after xennet_tx_buf_gc()! */
	xennet_tx_buf_gc(dev);

	if (!netfront_tx_slot_available(np))
		netif_stop_queue(dev);

	spin_unlock_irqrestore(&np->tx_lock, flags);
=======
	/* Requests for the rest of the linear area. */
	tx = xennet_make_txreqs(queue, tx, skb, page, offset, len);

	/* Requests for all the frags. */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
		tx = xennet_make_txreqs(queue, tx, skb,
					skb_frag_page(frag), frag->page_offset,
					skb_frag_size(frag));
	}

	/* First request has the packet length. */
	first_tx->size = skb->len;

	RING_PUSH_REQUESTS_AND_CHECK_NOTIFY(&queue->tx, notify);
	if (notify)
		notify_remote_via_irq(queue->tx_irq);

	u64_stats_update_begin(&tx_stats->syncp);
	tx_stats->bytes += skb->len;
	tx_stats->packets++;
	u64_stats_update_end(&tx_stats->syncp);

	/* Note: It is not safe to access skb after xennet_tx_buf_gc()! */
	xennet_tx_buf_gc(queue);

	if (!netfront_tx_slot_available(queue))
		netif_tx_stop_queue(netdev_get_tx_queue(dev, queue->id));

	spin_unlock_irqrestore(&queue->tx_lock, flags);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return NETDEV_TX_OK;

 drop:
	dev->stats.tx_dropped++;
<<<<<<< HEAD
	dev_kfree_skb(skb);
=======
	dev_kfree_skb_any(skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return NETDEV_TX_OK;
}

static int xennet_close(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
	netif_stop_queue(np->netdev);
	napi_disable(&np->napi);
	return 0;
}

static void xennet_move_rx_slot(struct netfront_info *np, struct sk_buff *skb,
				grant_ref_t ref)
{
	int new = xennet_rxidx(np->rx.req_prod_pvt);

	BUG_ON(np->rx_skbs[new]);
	np->rx_skbs[new] = skb;
	np->grant_rx_ref[new] = ref;
	RING_GET_REQUEST(&np->rx, np->rx.req_prod_pvt)->id = new;
	RING_GET_REQUEST(&np->rx, np->rx.req_prod_pvt)->gref = ref;
	np->rx.req_prod_pvt++;
}

static int xennet_get_extras(struct netfront_info *np,
=======
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i;
	struct netfront_queue *queue;
	netif_tx_stop_all_queues(np->netdev);
	for (i = 0; i < num_queues; ++i) {
		queue = &np->queues[i];
		napi_disable(&queue->napi);
	}
	return 0;
}

static void xennet_move_rx_slot(struct netfront_queue *queue, struct sk_buff *skb,
				grant_ref_t ref)
{
	int new = xennet_rxidx(queue->rx.req_prod_pvt);

	BUG_ON(queue->rx_skbs[new]);
	queue->rx_skbs[new] = skb;
	queue->grant_rx_ref[new] = ref;
	RING_GET_REQUEST(&queue->rx, queue->rx.req_prod_pvt)->id = new;
	RING_GET_REQUEST(&queue->rx, queue->rx.req_prod_pvt)->gref = ref;
	queue->rx.req_prod_pvt++;
}

static int xennet_get_extras(struct netfront_queue *queue,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			     struct xen_netif_extra_info *extras,
			     RING_IDX rp)

{
	struct xen_netif_extra_info *extra;
<<<<<<< HEAD
	struct device *dev = &np->netdev->dev;
	RING_IDX cons = np->rx.rsp_cons;
=======
	struct device *dev = &queue->info->netdev->dev;
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int err = 0;

	do {
		struct sk_buff *skb;
		grant_ref_t ref;

		if (unlikely(cons + 1 == rp)) {
			if (net_ratelimit())
				dev_warn(dev, "Missing extra info\n");
			err = -EBADR;
			break;
		}

		extra = (struct xen_netif_extra_info *)
<<<<<<< HEAD
			RING_GET_RESPONSE(&np->rx, ++cons);
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (unlikely(!extra->type ||
			     extra->type >= XEN_NETIF_EXTRA_TYPE_MAX)) {
			if (net_ratelimit())
				dev_warn(dev, "Invalid extra type: %d\n",
					extra->type);
			err = -EINVAL;
		} else {
			memcpy(&extras[extra->type - 1], extra,
			       sizeof(*extra));
		}

<<<<<<< HEAD
		skb = xennet_get_rx_skb(np, cons);
		ref = xennet_get_rx_ref(np, cons);
		xennet_move_rx_slot(np, skb, ref);
	} while (extra->flags & XEN_NETIF_EXTRA_FLAG_MORE);

	np->rx.rsp_cons = cons;
	return err;
}

static int xennet_get_responses(struct netfront_info *np,
=======
		skb = xennet_get_rx_skb(queue, cons);
		ref = xennet_get_rx_ref(queue, cons);
		xennet_move_rx_slot(queue, skb, ref);
	} while (extra->flags & XEN_NETIF_EXTRA_FLAG_MORE);

	queue->rx.rsp_cons = cons;
	return err;
}

static int xennet_get_responses(struct netfront_queue *queue,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				struct netfront_rx_info *rinfo, RING_IDX rp,
				struct sk_buff_head *list)
{
	struct xen_netif_rx_response *rx = &rinfo->rx;
	struct xen_netif_extra_info *extras = rinfo->extras;
<<<<<<< HEAD
	struct device *dev = &np->netdev->dev;
	RING_IDX cons = np->rx.rsp_cons;
	struct sk_buff *skb = xennet_get_rx_skb(np, cons);
	grant_ref_t ref = xennet_get_rx_ref(np, cons);
	int max = MAX_SKB_FRAGS + (rx->status <= RX_COPY_THRESHOLD);
=======
	struct device *dev = &queue->info->netdev->dev;
	RING_IDX cons = queue->rx.rsp_cons;
	struct sk_buff *skb = xennet_get_rx_skb(queue, cons);
	grant_ref_t ref = xennet_get_rx_ref(queue, cons);
	int max = XEN_NETIF_NR_SLOTS_MIN + (rx->status <= RX_COPY_THRESHOLD);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int slots = 1;
	int err = 0;
	unsigned long ret;

	if (rx->flags & XEN_NETRXF_extra_info) {
<<<<<<< HEAD
		err = xennet_get_extras(np, extras, rp);
		cons = np->rx.rsp_cons;
=======
		err = xennet_get_extras(queue, extras, rp);
		cons = queue->rx.rsp_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	for (;;) {
		if (unlikely(rx->status < 0 ||
<<<<<<< HEAD
			     rx->offset + rx->status > PAGE_SIZE)) {
			if (net_ratelimit())
				dev_warn(dev, "rx->offset: %x, size: %u\n",
					 rx->offset, rx->status);
			xennet_move_rx_slot(np, skb, ref);
=======
			     rx->offset + rx->status > XEN_PAGE_SIZE)) {
			if (net_ratelimit())
				dev_warn(dev, "rx->offset: %u, size: %d\n",
					 rx->offset, rx->status);
			xennet_move_rx_slot(queue, skb, ref);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			err = -EINVAL;
			goto next;
		}

		/*
		 * This definitely indicates a bug, either in this driver or in
		 * the backend driver. In future this should flag the bad
		 * situation to the system controller to reboot the backend.
		 */
		if (ref == GRANT_INVALID_REF) {
			if (net_ratelimit())
				dev_warn(dev, "Bad rx response id %d.\n",
					 rx->id);
			err = -EINVAL;
			goto next;
		}

		ret = gnttab_end_foreign_access_ref(ref, 0);
		BUG_ON(!ret);

<<<<<<< HEAD
		gnttab_release_grant_reference(&np->gref_rx_head, ref);
=======
		gnttab_release_grant_reference(&queue->gref_rx_head, ref);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		__skb_queue_tail(list, skb);

next:
		if (!(rx->flags & XEN_NETRXF_more_data))
			break;

		if (cons + slots == rp) {
			if (net_ratelimit())
				dev_warn(dev, "Need more slots\n");
			err = -ENOENT;
			break;
		}

<<<<<<< HEAD
		rx = RING_GET_RESPONSE(&np->rx, cons + slots);
		skb = xennet_get_rx_skb(np, cons + slots);
		ref = xennet_get_rx_ref(np, cons + slots);
=======
		rx = RING_GET_RESPONSE(&queue->rx, cons + slots);
		skb = xennet_get_rx_skb(queue, cons + slots);
		ref = xennet_get_rx_ref(queue, cons + slots);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		slots++;
	}

	if (unlikely(slots > max)) {
		if (net_ratelimit())
			dev_warn(dev, "Too many slots\n");
		err = -E2BIG;
	}

	if (unlikely(err))
<<<<<<< HEAD
		np->rx.rsp_cons = cons + slots;
=======
		queue->rx.rsp_cons = cons + slots;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return err;
}

static int xennet_set_skb_gso(struct sk_buff *skb,
			      struct xen_netif_extra_info *gso)
{
	if (!gso->u.gso.size) {
		if (net_ratelimit())
<<<<<<< HEAD
			printk(KERN_WARNING "GSO size must not be zero.\n");
		return -EINVAL;
	}

	/* Currently only TCPv4 S.O. is supported. */
	if (gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV4) {
		if (net_ratelimit())
			printk(KERN_WARNING "Bad GSO type %d.\n", gso->u.gso.type);
=======
			pr_warn("GSO size must not be zero\n");
		return -EINVAL;
	}

	if (gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV4 &&
	    gso->u.gso.type != XEN_NETIF_GSO_TYPE_TCPV6) {
		if (net_ratelimit())
			pr_warn("Bad GSO type %d\n", gso->u.gso.type);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;
	}

	skb_shinfo(skb)->gso_size = gso->u.gso.size;
<<<<<<< HEAD
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;
=======
	skb_shinfo(skb)->gso_type =
		(gso->u.gso.type == XEN_NETIF_GSO_TYPE_TCPV4) ?
		SKB_GSO_TCPV4 :
		SKB_GSO_TCPV6;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Header must be checked, and gso_segs computed. */
	skb_shinfo(skb)->gso_type |= SKB_GSO_DODGY;
	skb_shinfo(skb)->gso_segs = 0;

	return 0;
}

<<<<<<< HEAD
static RING_IDX xennet_fill_frags(struct netfront_info *np,
				  struct sk_buff *skb,
				  struct sk_buff_head *list)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	RING_IDX cons = np->rx.rsp_cons;
=======
static int xennet_fill_frags(struct netfront_queue *queue,
			     struct sk_buff *skb,
			     struct sk_buff_head *list)
{
	RING_IDX cons = queue->rx.rsp_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct sk_buff *nskb;

	while ((nskb = __skb_dequeue(list))) {
		struct xen_netif_rx_response *rx =
<<<<<<< HEAD
			RING_GET_RESPONSE(&np->rx, ++cons);
		skb_frag_t *nfrag = &skb_shinfo(nskb)->frags[0];

		if (shinfo->nr_frags == MAX_SKB_FRAGS) {
			unsigned int pull_to = NETFRONT_SKB_CB(skb)->pull_to;

			BUG_ON(pull_to <= skb_headlen(skb));
			__pskb_pull_tail(skb, pull_to - skb_headlen(skb));
		}
		BUG_ON(shinfo->nr_frags >= MAX_SKB_FRAGS);

		skb_add_rx_frag(skb, shinfo->nr_frags, skb_frag_page(nfrag),
=======
			RING_GET_RESPONSE(&queue->rx, ++cons);
		skb_frag_t *nfrag = &skb_shinfo(nskb)->frags[0];

		if (skb_shinfo(skb)->nr_frags == MAX_SKB_FRAGS) {
			unsigned int pull_to = NETFRONT_SKB_CB(skb)->pull_to;

			BUG_ON(pull_to < skb_headlen(skb));
			__pskb_pull_tail(skb, pull_to - skb_headlen(skb));
		}
		if (unlikely(skb_shinfo(skb)->nr_frags >= MAX_SKB_FRAGS)) {
			queue->rx.rsp_cons = ++cons + skb_queue_len(list);
			kfree_skb(nskb);
			return -ENOENT;
		}

		skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags,
				skb_frag_page(nfrag),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				rx->offset, rx->status, PAGE_SIZE);

		skb_shinfo(nskb)->nr_frags = 0;
		kfree_skb(nskb);
	}

<<<<<<< HEAD
	return cons;
=======
	queue->rx.rsp_cons = cons;

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int checksum_setup(struct net_device *dev, struct sk_buff *skb)
{
<<<<<<< HEAD
	struct iphdr *iph;
	unsigned char *th;
	int err = -EPROTO;
	int recalculate_partial_csum = 0;
=======
	bool recalculate_partial_csum = false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * A GSO SKB must be CHECKSUM_PARTIAL. However some buggy
	 * peers can fail to set NETRXF_csum_blank when sending a GSO
	 * frame. In this case force the SKB to CHECKSUM_PARTIAL and
	 * recalculate the partial checksum.
	 */
	if (skb->ip_summed != CHECKSUM_PARTIAL && skb_is_gso(skb)) {
		struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
		np->rx_gso_checksum_fixup++;
		skb->ip_summed = CHECKSUM_PARTIAL;
		recalculate_partial_csum = 1;
=======
		atomic_inc(&np->rx_gso_checksum_fixup);
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
	th = skb->data + 4 * iph->ihl;
	if (th >= skb_tail_pointer(skb))
		goto out;

	skb->csum_start = th - skb->head;
	switch (iph->protocol) {
	case IPPROTO_TCP:
		skb->csum_offset = offsetof(struct tcphdr, check);

		if (recalculate_partial_csum) {
			struct tcphdr *tcph = (struct tcphdr *)th;
			tcph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_TCP, 0);
		}
		break;
	case IPPROTO_UDP:
		skb->csum_offset = offsetof(struct udphdr, check);

		if (recalculate_partial_csum) {
			struct udphdr *udph = (struct udphdr *)th;
			udph->check = ~csum_tcpudp_magic(iph->saddr, iph->daddr,
							 skb->len - iph->ihl*4,
							 IPPROTO_UDP, 0);
		}
		break;
	default:
		if (net_ratelimit())
			printk(KERN_ERR "Attempting to checksum a non-"
			       "TCP/UDP packet, dropping a protocol"
			       " %d packet", iph->protocol);
		goto out;
	}

	if ((th + skb->csum_offset + 2) > skb_tail_pointer(skb))
		goto out;

	err = 0;

out:
	return err;
}

static int handle_incoming_queue(struct net_device *dev,
				 struct sk_buff_head *rxq)
{
	struct netfront_info *np = netdev_priv(dev);
	struct netfront_stats *stats = this_cpu_ptr(np->stats);
=======
	return skb_checksum_setup(skb, recalculate_partial_csum);
}

static int handle_incoming_queue(struct netfront_queue *queue,
				 struct sk_buff_head *rxq)
{
	struct netfront_stats *rx_stats = this_cpu_ptr(queue->info->rx_stats);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int packets_dropped = 0;
	struct sk_buff *skb;

	while ((skb = __skb_dequeue(rxq)) != NULL) {
		int pull_to = NETFRONT_SKB_CB(skb)->pull_to;

		if (pull_to > skb_headlen(skb))
			__pskb_pull_tail(skb, pull_to - skb_headlen(skb));

		/* Ethernet work: Delayed to here as it peeks the header. */
<<<<<<< HEAD
		skb->protocol = eth_type_trans(skb, dev);

		if (checksum_setup(dev, skb)) {
			kfree_skb(skb);
			packets_dropped++;
			dev->stats.rx_errors++;
			continue;
		}

		u64_stats_update_begin(&stats->syncp);
		stats->rx_packets++;
		stats->rx_bytes += skb->len;
		u64_stats_update_end(&stats->syncp);

		/* Pass it up. */
		netif_receive_skb(skb);
=======
		skb->protocol = eth_type_trans(skb, queue->info->netdev);
		skb_reset_network_header(skb);

		if (checksum_setup(queue->info->netdev, skb)) {
			kfree_skb(skb);
			packets_dropped++;
			queue->info->netdev->stats.rx_errors++;
			continue;
		}

		u64_stats_update_begin(&rx_stats->syncp);
		rx_stats->packets++;
		rx_stats->bytes += skb->len;
		u64_stats_update_end(&rx_stats->syncp);

		/* Pass it up. */
		napi_gro_receive(&queue->napi, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return packets_dropped;
}

static int xennet_poll(struct napi_struct *napi, int budget)
{
<<<<<<< HEAD
	struct netfront_info *np = container_of(napi, struct netfront_info, napi);
	struct net_device *dev = np->netdev;
=======
	struct netfront_queue *queue = container_of(napi, struct netfront_queue, napi);
	struct net_device *dev = queue->info->netdev;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct sk_buff *skb;
	struct netfront_rx_info rinfo;
	struct xen_netif_rx_response *rx = &rinfo.rx;
	struct xen_netif_extra_info *extras = rinfo.extras;
	RING_IDX i, rp;
	int work_done;
	struct sk_buff_head rxq;
	struct sk_buff_head errq;
	struct sk_buff_head tmpq;
<<<<<<< HEAD
	unsigned long flags;
	int err;

	spin_lock(&np->rx_lock);
=======
	int err;

	spin_lock(&queue->rx_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	skb_queue_head_init(&rxq);
	skb_queue_head_init(&errq);
	skb_queue_head_init(&tmpq);

<<<<<<< HEAD
	rp = np->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'rp'. */

	i = np->rx.rsp_cons;
	work_done = 0;
	while ((i != rp) && (work_done < budget)) {
		memcpy(rx, RING_GET_RESPONSE(&np->rx, i), sizeof(*rx));
		memset(extras, 0, sizeof(rinfo.extras));

		err = xennet_get_responses(np, &rinfo, rp, &tmpq);
=======
	rp = queue->rx.sring->rsp_prod;
	rmb(); /* Ensure we see queued responses up to 'rp'. */

	i = queue->rx.rsp_cons;
	work_done = 0;
	while ((i != rp) && (work_done < budget)) {
		memcpy(rx, RING_GET_RESPONSE(&queue->rx, i), sizeof(*rx));
		memset(extras, 0, sizeof(rinfo.extras));

		err = xennet_get_responses(queue, &rinfo, rp, &tmpq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (unlikely(err)) {
err:
			while ((skb = __skb_dequeue(&tmpq)))
				__skb_queue_tail(&errq, skb);
			dev->stats.rx_errors++;
<<<<<<< HEAD
			i = np->rx.rsp_cons;
=======
			i = queue->rx.rsp_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			continue;
		}

		skb = __skb_dequeue(&tmpq);

		if (extras[XEN_NETIF_EXTRA_TYPE_GSO - 1].type) {
			struct xen_netif_extra_info *gso;
			gso = &extras[XEN_NETIF_EXTRA_TYPE_GSO - 1];

			if (unlikely(xennet_set_skb_gso(skb, gso))) {
				__skb_queue_head(&tmpq, skb);
<<<<<<< HEAD
				np->rx.rsp_cons += skb_queue_len(&tmpq);
=======
				queue->rx.rsp_cons += skb_queue_len(&tmpq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				goto err;
			}
		}

		NETFRONT_SKB_CB(skb)->pull_to = rx->status;
		if (NETFRONT_SKB_CB(skb)->pull_to > RX_COPY_THRESHOLD)
			NETFRONT_SKB_CB(skb)->pull_to = RX_COPY_THRESHOLD;

		skb_shinfo(skb)->frags[0].page_offset = rx->offset;
		skb_frag_size_set(&skb_shinfo(skb)->frags[0], rx->status);
		skb->data_len = rx->status;
		skb->len += rx->status;

<<<<<<< HEAD
		i = xennet_fill_frags(np, skb, &tmpq);
=======
		if (unlikely(xennet_fill_frags(queue, skb, &tmpq)))
			goto err;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (rx->flags & XEN_NETRXF_csum_blank)
			skb->ip_summed = CHECKSUM_PARTIAL;
		else if (rx->flags & XEN_NETRXF_data_validated)
			skb->ip_summed = CHECKSUM_UNNECESSARY;

		__skb_queue_tail(&rxq, skb);

<<<<<<< HEAD
		np->rx.rsp_cons = ++i;
=======
		i = ++queue->rx.rsp_cons;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		work_done++;
	}

	__skb_queue_purge(&errq);

<<<<<<< HEAD
	work_done -= handle_incoming_queue(dev, &rxq);

	/* If we get a callback with very few responses, reduce fill target. */
	/* NB. Note exponential increase, linear decrease. */
	if (((np->rx.req_prod_pvt - np->rx.sring->rsp_prod) >
	     ((3*np->rx_target) / 4)) &&
	    (--np->rx_target < np->rx_min_target))
		np->rx_target = np->rx_min_target;

	xennet_alloc_rx_buffers(dev);
=======
	work_done -= handle_incoming_queue(queue, &rxq);

	xennet_alloc_rx_buffers(queue);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (work_done < budget) {
		int more_to_do = 0;

<<<<<<< HEAD
		local_irq_save(flags);

		RING_FINAL_CHECK_FOR_RESPONSES(&np->rx, more_to_do);
		if (!more_to_do)
			__napi_complete(napi);

		local_irq_restore(flags);
	}

	spin_unlock(&np->rx_lock);
=======
		napi_complete(napi);

		RING_FINAL_CHECK_FOR_RESPONSES(&queue->rx, more_to_do);
		if (more_to_do)
			napi_schedule(napi);
	}

	spin_unlock(&queue->rx_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return work_done;
}

static int xennet_change_mtu(struct net_device *dev, int mtu)
{
<<<<<<< HEAD
	int max = xennet_can_sg(dev) ?
		XEN_NETIF_MAX_TX_SIZE - MAX_TCP_HEADER : ETH_DATA_LEN;
=======
	int max = xennet_can_sg(dev) ? XEN_NETIF_MAX_TX_SIZE : ETH_DATA_LEN;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (mtu > max)
		return -EINVAL;
	dev->mtu = mtu;
	return 0;
}

static struct rtnl_link_stats64 *xennet_get_stats64(struct net_device *dev,
						    struct rtnl_link_stats64 *tot)
{
	struct netfront_info *np = netdev_priv(dev);
	int cpu;

	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		struct netfront_stats *stats = per_cpu_ptr(np->stats, cpu);
=======
		struct netfront_stats *rx_stats = per_cpu_ptr(np->rx_stats, cpu);
		struct netfront_stats *tx_stats = per_cpu_ptr(np->tx_stats, cpu);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		u64 rx_packets, rx_bytes, tx_packets, tx_bytes;
		unsigned int start;

		do {
<<<<<<< HEAD
			start = u64_stats_fetch_begin_bh(&stats->syncp);

			rx_packets = stats->rx_packets;
			tx_packets = stats->tx_packets;
			rx_bytes = stats->rx_bytes;
			tx_bytes = stats->tx_bytes;
		} while (u64_stats_fetch_retry_bh(&stats->syncp, start));
=======
			start = u64_stats_fetch_begin_irq(&tx_stats->syncp);
			tx_packets = tx_stats->packets;
			tx_bytes = tx_stats->bytes;
		} while (u64_stats_fetch_retry_irq(&tx_stats->syncp, start));

		do {
			start = u64_stats_fetch_begin_irq(&rx_stats->syncp);
			rx_packets = rx_stats->packets;
			rx_bytes = rx_stats->bytes;
		} while (u64_stats_fetch_retry_irq(&rx_stats->syncp, start));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		tot->rx_packets += rx_packets;
		tot->tx_packets += tx_packets;
		tot->rx_bytes   += rx_bytes;
		tot->tx_bytes   += tx_bytes;
	}

	tot->rx_errors  = dev->stats.rx_errors;
	tot->tx_dropped = dev->stats.tx_dropped;

	return tot;
}

<<<<<<< HEAD
static void xennet_release_tx_bufs(struct netfront_info *np)
=======
static void xennet_release_tx_bufs(struct netfront_queue *queue)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sk_buff *skb;
	int i;

	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		/* Skip over entries which are actually freelist references */
<<<<<<< HEAD
		if (skb_entry_is_link(&np->tx_skbs[i]))
			continue;

		skb = np->tx_skbs[i].skb;
		get_page(np->grant_tx_page[i]);
		gnttab_end_foreign_access(np->grant_tx_ref[i],
					  GNTMAP_readonly,
					  (unsigned long)page_address(np->grant_tx_page[i]));
		np->grant_tx_page[i] = NULL;
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
		add_id_to_freelist(&np->tx_skb_freelist, np->tx_skbs, i);
=======
		if (skb_entry_is_link(&queue->tx_skbs[i]))
			continue;

		skb = queue->tx_skbs[i].skb;
		get_page(queue->grant_tx_page[i]);
		gnttab_end_foreign_access(queue->grant_tx_ref[i],
					  GNTMAP_readonly,
					  (unsigned long)page_address(queue->grant_tx_page[i]));
		queue->grant_tx_page[i] = NULL;
		queue->grant_tx_ref[i] = GRANT_INVALID_REF;
		add_id_to_freelist(&queue->tx_skb_freelist, queue->tx_skbs, i);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_kfree_skb_irq(skb);
	}
}

<<<<<<< HEAD
static void xennet_release_rx_bufs(struct netfront_info *np)
{
	int id, ref;

	spin_lock_bh(&np->rx_lock);
=======
static void xennet_release_rx_bufs(struct netfront_queue *queue)
{
	int id, ref;

	spin_lock_bh(&queue->rx_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	for (id = 0; id < NET_RX_RING_SIZE; id++) {
		struct sk_buff *skb;
		struct page *page;

<<<<<<< HEAD
		skb = np->rx_skbs[id];
		if (!skb)
			continue;

		ref = np->grant_rx_ref[id];
=======
		skb = queue->rx_skbs[id];
		if (!skb)
			continue;

		ref = queue->grant_rx_ref[id];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ref == GRANT_INVALID_REF)
			continue;

		page = skb_frag_page(&skb_shinfo(skb)->frags[0]);

		/* gnttab_end_foreign_access() needs a page ref until
		 * foreign access is ended (which may be deferred).
		 */
		get_page(page);
		gnttab_end_foreign_access(ref, 0,
					  (unsigned long)page_address(page));
<<<<<<< HEAD
		np->grant_rx_ref[id] = GRANT_INVALID_REF;
=======
		queue->grant_rx_ref[id] = GRANT_INVALID_REF;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		kfree_skb(skb);
	}

<<<<<<< HEAD
	spin_unlock_bh(&np->rx_lock);
}

static void xennet_uninit(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
	xennet_release_tx_bufs(np);
	xennet_release_rx_bufs(np);
	gnttab_free_grant_references(np->gref_tx_head);
	gnttab_free_grant_references(np->gref_rx_head);
=======
	spin_unlock_bh(&queue->rx_lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static netdev_features_t xennet_fix_features(struct net_device *dev,
	netdev_features_t features)
{
	struct netfront_info *np = netdev_priv(dev);
	int val;

	if (features & NETIF_F_SG) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend, "feature-sg",
				 "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_SG;
	}

<<<<<<< HEAD
=======
	if (features & NETIF_F_IPV6_CSUM) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-ipv6-csum-offload", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_IPV6_CSUM;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (features & NETIF_F_TSO) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-gso-tcpv4", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_TSO;
	}

<<<<<<< HEAD
=======
	if (features & NETIF_F_TSO6) {
		if (xenbus_scanf(XBT_NIL, np->xbdev->otherend,
				 "feature-gso-tcpv6", "%d", &val) < 0)
			val = 0;

		if (!val)
			features &= ~NETIF_F_TSO6;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return features;
}

static int xennet_set_features(struct net_device *dev,
	netdev_features_t features)
{
	if (!(features & NETIF_F_SG) && dev->mtu > ETH_DATA_LEN) {
		netdev_info(dev, "Reducing MTU because no SG offload");
		dev->mtu = ETH_DATA_LEN;
	}

	return 0;
}

<<<<<<< HEAD
static irqreturn_t xennet_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	struct netfront_info *np = netdev_priv(dev);
	unsigned long flags;

	spin_lock_irqsave(&np->tx_lock, flags);

	if (likely(netif_carrier_ok(dev))) {
		xennet_tx_buf_gc(dev);
		/* Under tx_lock: protects access to rx shared-ring indexes. */
		if (RING_HAS_UNCONSUMED_RESPONSES(&np->rx))
			napi_schedule(&np->napi);
	}

	spin_unlock_irqrestore(&np->tx_lock, flags);
=======
static irqreturn_t xennet_tx_interrupt(int irq, void *dev_id)
{
	struct netfront_queue *queue = dev_id;
	unsigned long flags;

	spin_lock_irqsave(&queue->tx_lock, flags);
	xennet_tx_buf_gc(queue);
	spin_unlock_irqrestore(&queue->tx_lock, flags);

	return IRQ_HANDLED;
}

static irqreturn_t xennet_rx_interrupt(int irq, void *dev_id)
{
	struct netfront_queue *queue = dev_id;
	struct net_device *dev = queue->info->netdev;

	if (likely(netif_carrier_ok(dev) &&
		   RING_HAS_UNCONSUMED_RESPONSES(&queue->rx)))
		napi_schedule(&queue->napi);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return IRQ_HANDLED;
}

<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
static void xennet_poll_controller(struct net_device *dev)
{
	xennet_interrupt(0, dev);
=======
static irqreturn_t xennet_interrupt(int irq, void *dev_id)
{
	xennet_tx_interrupt(irq, dev_id);
	xennet_rx_interrupt(irq, dev_id);
	return IRQ_HANDLED;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void xennet_poll_controller(struct net_device *dev)
{
	/* Poll each queue */
	struct netfront_info *info = netdev_priv(dev);
	unsigned int num_queues = dev->real_num_tx_queues;
	unsigned int i;
	for (i = 0; i < num_queues; ++i)
		xennet_interrupt(0, &info->queues[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif

static const struct net_device_ops xennet_netdev_ops = {
	.ndo_open            = xennet_open,
<<<<<<< HEAD
	.ndo_uninit          = xennet_uninit,
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.ndo_stop            = xennet_close,
	.ndo_start_xmit      = xennet_start_xmit,
	.ndo_change_mtu	     = xennet_change_mtu,
	.ndo_get_stats64     = xennet_get_stats64,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr   = eth_validate_addr,
	.ndo_fix_features    = xennet_fix_features,
	.ndo_set_features    = xennet_set_features,
<<<<<<< HEAD
=======
	.ndo_select_queue    = xennet_select_queue,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = xennet_poll_controller,
#endif
};

<<<<<<< HEAD
static struct net_device *xennet_create_dev(struct xenbus_device *dev)
{
	int i, err;
	struct net_device *netdev;
	struct netfront_info *np;

	netdev = alloc_etherdev(sizeof(struct netfront_info));
=======
static void xennet_free_netdev(struct net_device *netdev)
{
	struct netfront_info *np = netdev_priv(netdev);

	free_percpu(np->rx_stats);
	free_percpu(np->tx_stats);
	free_netdev(netdev);
}

static struct net_device *xennet_create_dev(struct xenbus_device *dev)
{
	int err;
	struct net_device *netdev;
	struct netfront_info *np;

	netdev = alloc_etherdev_mq(sizeof(struct netfront_info), xennet_max_queues);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!netdev)
		return ERR_PTR(-ENOMEM);

	np                   = netdev_priv(netdev);
	np->xbdev            = dev;

<<<<<<< HEAD
	spin_lock_init(&np->tx_lock);
	spin_lock_init(&np->rx_lock);

	skb_queue_head_init(&np->rx_batch);
	np->rx_target     = RX_DFL_MIN_TARGET;
	np->rx_min_target = RX_DFL_MIN_TARGET;
	np->rx_max_target = RX_MAX_TARGET;

	init_timer(&np->rx_refill_timer);
	np->rx_refill_timer.data = (unsigned long)netdev;
	np->rx_refill_timer.function = rx_refill_timeout;

	err = -ENOMEM;
	np->stats = alloc_percpu(struct netfront_stats);
	if (np->stats == NULL)
		goto exit;

	/* Initialise tx_skbs as a free chain containing every entry. */
	np->tx_skb_freelist = 0;
	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		skb_entry_set_link(&np->tx_skbs[i], i+1);
		np->grant_tx_ref[i] = GRANT_INVALID_REF;
	}

	/* Clear out rx_skbs */
	for (i = 0; i < NET_RX_RING_SIZE; i++) {
		np->rx_skbs[i] = NULL;
		np->grant_rx_ref[i] = GRANT_INVALID_REF;
		np->grant_tx_page[i] = NULL;
	}

	/* A grant for every tx ring slot */
	if (gnttab_alloc_grant_references(TX_MAX_TARGET,
					  &np->gref_tx_head) < 0) {
		printk(KERN_ALERT "#### netfront can't alloc tx grant refs\n");
		err = -ENOMEM;
		goto exit_free_stats;
	}
	/* A grant for every rx ring slot */
	if (gnttab_alloc_grant_references(RX_MAX_TARGET,
					  &np->gref_rx_head) < 0) {
		printk(KERN_ALERT "#### netfront can't alloc rx grant refs\n");
		err = -ENOMEM;
		goto exit_free_tx;
	}

	netdev->netdev_ops	= &xennet_netdev_ops;

	netif_napi_add(netdev, &np->napi, xennet_poll, 64);
	netdev->features        = NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
				  NETIF_F_GSO_ROBUST;
	netdev->hw_features	= NETIF_F_IP_CSUM | NETIF_F_SG | NETIF_F_TSO;
=======
	np->queues = NULL;

	err = -ENOMEM;
	np->rx_stats = netdev_alloc_pcpu_stats(struct netfront_stats);
	if (np->rx_stats == NULL)
		goto exit;
	np->tx_stats = netdev_alloc_pcpu_stats(struct netfront_stats);
	if (np->tx_stats == NULL)
		goto exit;

	netdev->netdev_ops	= &xennet_netdev_ops;

	netdev->features        = NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
				  NETIF_F_GSO_ROBUST;
	netdev->hw_features	= NETIF_F_SG |
				  NETIF_F_IPV6_CSUM |
				  NETIF_F_TSO | NETIF_F_TSO6;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
         * Assume that all hw features are available for now. This set
         * will be adjusted by the call to netdev_update_features() in
         * xennet_connect() which is the earliest point where we can
         * negotiate with the backend regarding supported features.
         */
	netdev->features |= netdev->hw_features;

<<<<<<< HEAD
	SET_ETHTOOL_OPS(netdev, &xennet_ethtool_ops);
	SET_NETDEV_DEV(netdev, &dev->dev);

	netif_set_gso_max_size(netdev, XEN_NETIF_MAX_TX_SIZE - MAX_TCP_HEADER);

=======
	netdev->ethtool_ops = &xennet_ethtool_ops;
	SET_NETDEV_DEV(netdev, &dev->dev);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	np->netdev = netdev;

	netif_carrier_off(netdev);

<<<<<<< HEAD
	return netdev;

 exit_free_tx:
	gnttab_free_grant_references(np->gref_tx_head);
 exit_free_stats:
	free_percpu(np->stats);
 exit:
	free_netdev(netdev);
=======
	do {
		xenbus_switch_state(dev, XenbusStateInitialising);
		err = wait_event_timeout(module_wq,
				 xenbus_read_driver_state(dev->otherend) !=
				 XenbusStateClosed &&
				 xenbus_read_driver_state(dev->otherend) !=
				 XenbusStateUnknown, XENNET_TIMEOUT);
	} while (!err);

	return netdev;

 exit:
	xennet_free_netdev(netdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ERR_PTR(err);
}

/**
 * Entry point to this code when a new device is created.  Allocate the basic
 * structures and the ring buffers for communication with the backend, and
 * inform the backend of the appropriate details for those.
 */
static int netfront_probe(struct xenbus_device *dev,
			  const struct xenbus_device_id *id)
{
	int err;
	struct net_device *netdev;
	struct netfront_info *info;

	netdev = xennet_create_dev(dev);
	if (IS_ERR(netdev)) {
		err = PTR_ERR(netdev);
		xenbus_dev_fatal(dev, err, "creating netdev");
		return err;
	}

	info = netdev_priv(netdev);
	dev_set_drvdata(&dev->dev, info);
<<<<<<< HEAD

	err = register_netdev(info->netdev);
	if (err) {
		printk(KERN_WARNING "%s: register_netdev err=%d\n",
		       __func__, err);
		goto fail;
	}

	err = xennet_sysfs_addif(info->netdev);
	if (err) {
		unregister_netdev(info->netdev);
		printk(KERN_WARNING "%s: add sysfs failed err=%d\n",
		       __func__, err);
		goto fail;
	}

	return 0;

 fail:
	free_netdev(netdev);
	dev_set_drvdata(&dev->dev, NULL);
	return err;
=======
#ifdef CONFIG_SYSFS
	info->netdev->sysfs_groups[0] = &xennet_dev_group;
#endif

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void xennet_end_access(int ref, void *page)
{
	/* This frees the page as a side-effect */
	if (ref != GRANT_INVALID_REF)
		gnttab_end_foreign_access(ref, 0, (unsigned long)page);
}

static void xennet_disconnect_backend(struct netfront_info *info)
{
<<<<<<< HEAD
	/* Stop old i/f to prevent errors whilst we rebuild the state. */
	spin_lock_bh(&info->rx_lock);
	spin_lock_irq(&info->tx_lock);
	netif_carrier_off(info->netdev);
	spin_unlock_irq(&info->tx_lock);
	spin_unlock_bh(&info->rx_lock);

	if (info->netdev->irq)
		unbind_from_irqhandler(info->netdev->irq, info->netdev);
	info->evtchn = info->netdev->irq = 0;

	/* End access and free the pages */
	xennet_end_access(info->tx_ring_ref, info->tx.sring);
	xennet_end_access(info->rx_ring_ref, info->rx.sring);

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->tx.sring = NULL;
	info->rx.sring = NULL;
=======
	unsigned int i = 0;
	unsigned int num_queues = info->netdev->real_num_tx_queues;

	netif_carrier_off(info->netdev);

	for (i = 0; i < num_queues && info->queues; ++i) {
		struct netfront_queue *queue = &info->queues[i];

		del_timer_sync(&queue->rx_refill_timer);

		if (queue->tx_irq && (queue->tx_irq == queue->rx_irq))
			unbind_from_irqhandler(queue->tx_irq, queue);
		if (queue->tx_irq && (queue->tx_irq != queue->rx_irq)) {
			unbind_from_irqhandler(queue->tx_irq, queue);
			unbind_from_irqhandler(queue->rx_irq, queue);
		}
		queue->tx_evtchn = queue->rx_evtchn = 0;
		queue->tx_irq = queue->rx_irq = 0;

		if (netif_running(info->netdev))
			napi_synchronize(&queue->napi);

		xennet_release_tx_bufs(queue);
		xennet_release_rx_bufs(queue);
		gnttab_free_grant_references(queue->gref_tx_head);
		gnttab_free_grant_references(queue->gref_rx_head);

		/* End access and free the pages */
		xennet_end_access(queue->tx_ring_ref, queue->tx.sring);
		xennet_end_access(queue->rx_ring_ref, queue->rx.sring);

		queue->tx_ring_ref = GRANT_INVALID_REF;
		queue->rx_ring_ref = GRANT_INVALID_REF;
		queue->tx.sring = NULL;
		queue->rx.sring = NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * We are reconnecting to the backend, due to a suspend/resume, or a backend
 * driver restart.  We tear down our netif structure and recreate it, but
 * leave the device-layer structures intact so that this is transparent to the
 * rest of the kernel.
 */
static int netfront_resume(struct xenbus_device *dev)
{
	struct netfront_info *info = dev_get_drvdata(&dev->dev);

	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xennet_disconnect_backend(info);
	return 0;
}

static int xen_net_read_mac(struct xenbus_device *dev, u8 mac[])
{
	char *s, *e, *macstr;
	int i;

	macstr = s = xenbus_read(XBT_NIL, dev->nodename, "mac", NULL);
	if (IS_ERR(macstr))
		return PTR_ERR(macstr);

	for (i = 0; i < ETH_ALEN; i++) {
		mac[i] = simple_strtoul(s, &e, 16);
		if ((s == e) || (*e != ((i == ETH_ALEN-1) ? '\0' : ':'))) {
			kfree(macstr);
			return -ENOENT;
		}
		s = e+1;
	}

	kfree(macstr);
	return 0;
}

<<<<<<< HEAD
static int setup_netfront(struct xenbus_device *dev, struct netfront_info *info)
{
	struct xen_netif_tx_sring *txs;
	struct xen_netif_rx_sring *rxs;
	int err;
	struct net_device *netdev = info->netdev;

	info->tx_ring_ref = GRANT_INVALID_REF;
	info->rx_ring_ref = GRANT_INVALID_REF;
	info->rx.sring = NULL;
	info->tx.sring = NULL;
	netdev->irq = 0;

	err = xen_net_read_mac(dev, netdev->dev_addr);
	if (err) {
		xenbus_dev_fatal(dev, err, "parsing %s/mac", dev->nodename);
		goto fail;
	}
=======
static int setup_netfront_single(struct netfront_queue *queue)
{
	int err;

	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->tx_evtchn);
	if (err < 0)
		goto fail;

	err = bind_evtchn_to_irqhandler(queue->tx_evtchn,
					xennet_interrupt,
					0, queue->info->netdev->name, queue);
	if (err < 0)
		goto bind_fail;
	queue->rx_evtchn = queue->tx_evtchn;
	queue->rx_irq = queue->tx_irq = err;

	return 0;

bind_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->tx_evtchn);
	queue->tx_evtchn = 0;
fail:
	return err;
}

static int setup_netfront_split(struct netfront_queue *queue)
{
	int err;

	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->tx_evtchn);
	if (err < 0)
		goto fail;
	err = xenbus_alloc_evtchn(queue->info->xbdev, &queue->rx_evtchn);
	if (err < 0)
		goto alloc_rx_evtchn_fail;

	snprintf(queue->tx_irq_name, sizeof(queue->tx_irq_name),
		 "%s-tx", queue->name);
	err = bind_evtchn_to_irqhandler(queue->tx_evtchn,
					xennet_tx_interrupt,
					0, queue->tx_irq_name, queue);
	if (err < 0)
		goto bind_tx_fail;
	queue->tx_irq = err;

	snprintf(queue->rx_irq_name, sizeof(queue->rx_irq_name),
		 "%s-rx", queue->name);
	err = bind_evtchn_to_irqhandler(queue->rx_evtchn,
					xennet_rx_interrupt,
					0, queue->rx_irq_name, queue);
	if (err < 0)
		goto bind_rx_fail;
	queue->rx_irq = err;

	return 0;

bind_rx_fail:
	unbind_from_irqhandler(queue->tx_irq, queue);
	queue->tx_irq = 0;
bind_tx_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->rx_evtchn);
	queue->rx_evtchn = 0;
alloc_rx_evtchn_fail:
	xenbus_free_evtchn(queue->info->xbdev, queue->tx_evtchn);
	queue->tx_evtchn = 0;
fail:
	return err;
}

static int setup_netfront(struct xenbus_device *dev,
			struct netfront_queue *queue, unsigned int feature_split_evtchn)
{
	struct xen_netif_tx_sring *txs;
	struct xen_netif_rx_sring *rxs;
	grant_ref_t gref;
	int err;

	queue->tx_ring_ref = GRANT_INVALID_REF;
	queue->rx_ring_ref = GRANT_INVALID_REF;
	queue->rx.sring = NULL;
	queue->tx.sring = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	txs = (struct xen_netif_tx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!txs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating tx ring page");
		goto fail;
	}
	SHARED_RING_INIT(txs);
<<<<<<< HEAD
	FRONT_RING_INIT(&info->tx, txs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(txs));
	if (err < 0) {
		free_page((unsigned long)txs);
		goto fail;
	}

	info->tx_ring_ref = err;
=======
	FRONT_RING_INIT(&queue->tx, txs, XEN_PAGE_SIZE);

	err = xenbus_grant_ring(dev, txs, 1, &gref);
	if (err < 0)
		goto grant_tx_ring_fail;
	queue->tx_ring_ref = gref;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rxs = (struct xen_netif_rx_sring *)get_zeroed_page(GFP_NOIO | __GFP_HIGH);
	if (!rxs) {
		err = -ENOMEM;
		xenbus_dev_fatal(dev, err, "allocating rx ring page");
<<<<<<< HEAD
		goto fail;
	}
	SHARED_RING_INIT(rxs);
	FRONT_RING_INIT(&info->rx, rxs, PAGE_SIZE);

	err = xenbus_grant_ring(dev, virt_to_mfn(rxs));
	if (err < 0) {
		free_page((unsigned long)rxs);
		goto fail;
	}
	info->rx_ring_ref = err;

	err = xenbus_alloc_evtchn(dev, &info->evtchn);
	if (err)
		goto fail;

	err = bind_evtchn_to_irqhandler(info->evtchn, xennet_interrupt,
					0, netdev->name, netdev);
	if (err < 0)
		goto fail;
	netdev->irq = err;
	return 0;

 fail:
	return err;
}

=======
		goto alloc_rx_ring_fail;
	}
	SHARED_RING_INIT(rxs);
	FRONT_RING_INIT(&queue->rx, rxs, XEN_PAGE_SIZE);

	err = xenbus_grant_ring(dev, rxs, 1, &gref);
	if (err < 0)
		goto grant_rx_ring_fail;
	queue->rx_ring_ref = gref;

	if (feature_split_evtchn)
		err = setup_netfront_split(queue);
	/* setup single event channel if
	 *  a) feature-split-event-channels == 0
	 *  b) feature-split-event-channels == 1 but failed to setup
	 */
	if (!feature_split_evtchn || (feature_split_evtchn && err))
		err = setup_netfront_single(queue);

	if (err)
		goto alloc_evtchn_fail;

	return 0;

	/* If we fail to setup netfront, it is safe to just revoke access to
	 * granted pages because backend is not accessing it at this point.
	 */
alloc_evtchn_fail:
	gnttab_end_foreign_access_ref(queue->rx_ring_ref, 0);
grant_rx_ring_fail:
	free_page((unsigned long)rxs);
alloc_rx_ring_fail:
	gnttab_end_foreign_access_ref(queue->tx_ring_ref, 0);
grant_tx_ring_fail:
	free_page((unsigned long)txs);
fail:
	return err;
}

/* Queue-specific initialisation
 * This used to be done in xennet_create_dev() but must now
 * be run per-queue.
 */
static int xennet_init_queue(struct netfront_queue *queue)
{
	unsigned short i;
	int err = 0;
	char *devid;

	spin_lock_init(&queue->tx_lock);
	spin_lock_init(&queue->rx_lock);

	setup_timer(&queue->rx_refill_timer, rx_refill_timeout,
		    (unsigned long)queue);

	devid = strrchr(queue->info->xbdev->nodename, '/') + 1;
	snprintf(queue->name, sizeof(queue->name), "vif%s-q%u",
		 devid, queue->id);

	/* Initialise tx_skbs as a free chain containing every entry. */
	queue->tx_skb_freelist = 0;
	for (i = 0; i < NET_TX_RING_SIZE; i++) {
		skb_entry_set_link(&queue->tx_skbs[i], i+1);
		queue->grant_tx_ref[i] = GRANT_INVALID_REF;
		queue->grant_tx_page[i] = NULL;
	}

	/* Clear out rx_skbs */
	for (i = 0; i < NET_RX_RING_SIZE; i++) {
		queue->rx_skbs[i] = NULL;
		queue->grant_rx_ref[i] = GRANT_INVALID_REF;
	}

	/* A grant for every tx ring slot */
	if (gnttab_alloc_grant_references(NET_TX_RING_SIZE,
					  &queue->gref_tx_head) < 0) {
		pr_alert("can't alloc tx grant refs\n");
		err = -ENOMEM;
		goto exit;
	}

	/* A grant for every rx ring slot */
	if (gnttab_alloc_grant_references(NET_RX_RING_SIZE,
					  &queue->gref_rx_head) < 0) {
		pr_alert("can't alloc rx grant refs\n");
		err = -ENOMEM;
		goto exit_free_tx;
	}

	return 0;

 exit_free_tx:
	gnttab_free_grant_references(queue->gref_tx_head);
 exit:
	return err;
}

static int write_queue_xenstore_keys(struct netfront_queue *queue,
			   struct xenbus_transaction *xbt, int write_hierarchical)
{
	/* Write the queue-specific keys into XenStore in the traditional
	 * way for a single queue, or in a queue subkeys for multiple
	 * queues.
	 */
	struct xenbus_device *dev = queue->info->xbdev;
	int err;
	const char *message;
	char *path;
	size_t pathsize;

	/* Choose the correct place to write the keys */
	if (write_hierarchical) {
		pathsize = strlen(dev->nodename) + 10;
		path = kzalloc(pathsize, GFP_KERNEL);
		if (!path) {
			err = -ENOMEM;
			message = "out of memory while writing ring references";
			goto error;
		}
		snprintf(path, pathsize, "%s/queue-%u",
				dev->nodename, queue->id);
	} else {
		path = (char *)dev->nodename;
	}

	/* Write ring references */
	err = xenbus_printf(*xbt, path, "tx-ring-ref", "%u",
			queue->tx_ring_ref);
	if (err) {
		message = "writing tx-ring-ref";
		goto error;
	}

	err = xenbus_printf(*xbt, path, "rx-ring-ref", "%u",
			queue->rx_ring_ref);
	if (err) {
		message = "writing rx-ring-ref";
		goto error;
	}

	/* Write event channels; taking into account both shared
	 * and split event channel scenarios.
	 */
	if (queue->tx_evtchn == queue->rx_evtchn) {
		/* Shared event channel */
		err = xenbus_printf(*xbt, path,
				"event-channel", "%u", queue->tx_evtchn);
		if (err) {
			message = "writing event-channel";
			goto error;
		}
	} else {
		/* Split event channels */
		err = xenbus_printf(*xbt, path,
				"event-channel-tx", "%u", queue->tx_evtchn);
		if (err) {
			message = "writing event-channel-tx";
			goto error;
		}

		err = xenbus_printf(*xbt, path,
				"event-channel-rx", "%u", queue->rx_evtchn);
		if (err) {
			message = "writing event-channel-rx";
			goto error;
		}
	}

	if (write_hierarchical)
		kfree(path);
	return 0;

error:
	if (write_hierarchical)
		kfree(path);
	xenbus_dev_fatal(dev, err, "%s", message);
	return err;
}

static void xennet_destroy_queues(struct netfront_info *info)
{
	unsigned int i;

	for (i = 0; i < info->netdev->real_num_tx_queues; i++) {
		struct netfront_queue *queue = &info->queues[i];

		if (netif_running(info->netdev))
			napi_disable(&queue->napi);
		netif_napi_del(&queue->napi);
	}

	kfree(info->queues);
	info->queues = NULL;
}

static int xennet_create_queues(struct netfront_info *info,
				unsigned int *num_queues)
{
	unsigned int i;
	int ret;

	info->queues = kcalloc(*num_queues, sizeof(struct netfront_queue),
			       GFP_KERNEL);
	if (!info->queues)
		return -ENOMEM;

	for (i = 0; i < *num_queues; i++) {
		struct netfront_queue *queue = &info->queues[i];

		queue->id = i;
		queue->info = info;

		ret = xennet_init_queue(queue);
		if (ret < 0) {
			dev_warn(&info->xbdev->dev,
				 "only created %d queues\n", i);
			*num_queues = i;
			break;
		}

		netif_napi_add(queue->info->netdev, &queue->napi,
			       xennet_poll, 64);
		if (netif_running(info->netdev))
			napi_enable(&queue->napi);
	}

	netif_set_real_num_tx_queues(info->netdev, *num_queues);

	if (*num_queues == 0) {
		dev_err(&info->xbdev->dev, "no queues\n");
		return -EINVAL;
	}
	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Common code used when first setting up, and when resuming. */
static int talk_to_netback(struct xenbus_device *dev,
			   struct netfront_info *info)
{
	const char *message;
	struct xenbus_transaction xbt;
	int err;
<<<<<<< HEAD

	/* Create shared ring, alloc event channel. */
	err = setup_netfront(dev, info);
	if (err)
		goto out;
=======
	unsigned int feature_split_evtchn;
	unsigned int i = 0;
	unsigned int max_queues = 0;
	struct netfront_queue *queue = NULL;
	unsigned int num_queues = 1;

	info->netdev->irq = 0;

	/* Check if backend supports multiple queues */
	err = xenbus_scanf(XBT_NIL, info->xbdev->otherend,
			   "multi-queue-max-queues", "%u", &max_queues);
	if (err < 0)
		max_queues = 1;
	num_queues = min(max_queues, xennet_max_queues);

	/* Check feature-split-event-channels */
	err = xenbus_scanf(XBT_NIL, info->xbdev->otherend,
			   "feature-split-event-channels", "%u",
			   &feature_split_evtchn);
	if (err < 0)
		feature_split_evtchn = 0;

	/* Read mac addr. */
	err = xen_net_read_mac(dev, info->netdev->dev_addr);
	if (err) {
		xenbus_dev_fatal(dev, err, "parsing %s/mac", dev->nodename);
		goto out_unlocked;
	}

	rtnl_lock();
	if (info->queues)
		xennet_destroy_queues(info);

	err = xennet_create_queues(info, &num_queues);
	if (err < 0) {
		xenbus_dev_fatal(dev, err, "creating queues");
		kfree(info->queues);
		info->queues = NULL;
		goto out;
	}
	rtnl_unlock();

	/* Create shared ring, alloc event channel -- for each queue */
	for (i = 0; i < num_queues; ++i) {
		queue = &info->queues[i];
		err = setup_netfront(dev, queue, feature_split_evtchn);
		if (err)
			goto destroy_ring;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

again:
	err = xenbus_transaction_start(&xbt);
	if (err) {
		xenbus_dev_fatal(dev, err, "starting transaction");
		goto destroy_ring;
	}

<<<<<<< HEAD
	err = xenbus_printf(xbt, dev->nodename, "tx-ring-ref", "%u",
			    info->tx_ring_ref);
	if (err) {
		message = "writing tx ring-ref";
		goto abort_transaction;
	}
	err = xenbus_printf(xbt, dev->nodename, "rx-ring-ref", "%u",
			    info->rx_ring_ref);
	if (err) {
		message = "writing rx ring-ref";
		goto abort_transaction;
	}
	err = xenbus_printf(xbt, dev->nodename,
			    "event-channel", "%u", info->evtchn);
	if (err) {
		message = "writing event-channel";
		goto abort_transaction;
	}

=======
	if (xenbus_exists(XBT_NIL,
			  info->xbdev->otherend, "multi-queue-max-queues")) {
		/* Write the number of queues */
		err = xenbus_printf(xbt, dev->nodename,
				    "multi-queue-num-queues", "%u", num_queues);
		if (err) {
			message = "writing multi-queue-num-queues";
			goto abort_transaction_no_dev_fatal;
		}
	}

	if (num_queues == 1) {
		err = write_queue_xenstore_keys(&info->queues[0], &xbt, 0); /* flat */
		if (err)
			goto abort_transaction_no_dev_fatal;
	} else {
		/* Write the keys for each queue */
		for (i = 0; i < num_queues; ++i) {
			queue = &info->queues[i];
			err = write_queue_xenstore_keys(queue, &xbt, 1); /* hierarchical */
			if (err)
				goto abort_transaction_no_dev_fatal;
		}
	}

	/* The remaining keys are not queue-specific */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	err = xenbus_printf(xbt, dev->nodename, "request-rx-copy", "%u",
			    1);
	if (err) {
		message = "writing request-rx-copy";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-rx-notify", "%d", 1);
	if (err) {
		message = "writing feature-rx-notify";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-sg", "%d", 1);
	if (err) {
		message = "writing feature-sg";
		goto abort_transaction;
	}

	err = xenbus_printf(xbt, dev->nodename, "feature-gso-tcpv4", "%d", 1);
	if (err) {
		message = "writing feature-gso-tcpv4";
		goto abort_transaction;
	}

<<<<<<< HEAD
=======
	err = xenbus_write(xbt, dev->nodename, "feature-gso-tcpv6", "1");
	if (err) {
		message = "writing feature-gso-tcpv6";
		goto abort_transaction;
	}

	err = xenbus_write(xbt, dev->nodename, "feature-ipv6-csum-offload",
			   "1");
	if (err) {
		message = "writing feature-ipv6-csum-offload";
		goto abort_transaction;
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	err = xenbus_transaction_end(xbt, 0);
	if (err) {
		if (err == -EAGAIN)
			goto again;
		xenbus_dev_fatal(dev, err, "completing transaction");
		goto destroy_ring;
	}

	return 0;

 abort_transaction:
<<<<<<< HEAD
	xenbus_transaction_end(xbt, 1);
	xenbus_dev_fatal(dev, err, "%s", message);
 destroy_ring:
	xennet_disconnect_backend(info);
 out:
=======
	xenbus_dev_fatal(dev, err, "%s", message);
abort_transaction_no_dev_fatal:
	xenbus_transaction_end(xbt, 1);
 destroy_ring:
	xennet_disconnect_backend(info);
	rtnl_lock();
	xennet_destroy_queues(info);
 out:
	rtnl_unlock();
out_unlocked:
	device_unregister(&dev->dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return err;
}

static int xennet_connect(struct net_device *dev)
{
	struct netfront_info *np = netdev_priv(dev);
<<<<<<< HEAD
	int i, requeue_idx, err;
	struct sk_buff *skb;
	grant_ref_t ref;
	struct xen_netif_rx_request *req;
	unsigned int feature_rx_copy;
=======
	unsigned int num_queues = 0;
	int err;
	unsigned int feature_rx_copy;
	unsigned int j = 0;
	struct netfront_queue *queue = NULL;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err = xenbus_scanf(XBT_NIL, np->xbdev->otherend,
			   "feature-rx-copy", "%u", &feature_rx_copy);
	if (err != 1)
		feature_rx_copy = 0;

	if (!feature_rx_copy) {
		dev_info(&dev->dev,
			 "backend does not support copying receive path\n");
		return -ENODEV;
	}

	err = talk_to_netback(np->xbdev, np);
	if (err)
		return err;

<<<<<<< HEAD
	rtnl_lock();
	netdev_update_features(dev);
	rtnl_unlock();

	spin_lock_bh(&np->rx_lock);
	spin_lock_irq(&np->tx_lock);

	/* Step 1: Discard all pending TX packet fragments. */
	xennet_release_tx_bufs(np);

	/* Step 2: Rebuild the RX buffer freelist and the RX ring itself. */
	for (requeue_idx = 0, i = 0; i < NET_RX_RING_SIZE; i++) {
		skb_frag_t *frag;
		const struct page *page;
		if (!np->rx_skbs[i])
			continue;

		skb = np->rx_skbs[requeue_idx] = xennet_get_rx_skb(np, i);
		ref = np->grant_rx_ref[requeue_idx] = xennet_get_rx_ref(np, i);
		req = RING_GET_REQUEST(&np->rx, requeue_idx);

		frag = &skb_shinfo(skb)->frags[0];
		page = skb_frag_page(frag);
		gnttab_grant_foreign_access_ref(
			ref, np->xbdev->otherend_id,
			pfn_to_mfn(page_to_pfn(page)),
			0);
		req->gref = ref;
		req->id   = requeue_idx;

		requeue_idx++;
	}

	np->rx.req_prod_pvt = requeue_idx;

	/*
	 * Step 3: All public and private state should now be sane.  Get
=======
	/* talk_to_netback() sets the correct number of queues */
	num_queues = dev->real_num_tx_queues;

	if (dev->reg_state == NETREG_UNINITIALIZED) {
		err = register_netdev(dev);
		if (err) {
			pr_warn("%s: register_netdev err=%d\n", __func__, err);
			device_unregister(&np->xbdev->dev);
			return err;
		}
	}

	rtnl_lock();
	netdev_update_features(dev);
	rtnl_unlock();

	/*
	 * All public and private state should now be sane.  Get
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 * ready to start sending and receiving packets and give the driver
	 * domain a kick because we've probably just requeued some
	 * packets.
	 */
	netif_carrier_on(np->netdev);
<<<<<<< HEAD
	notify_remote_via_irq(np->netdev->irq);
	xennet_tx_buf_gc(dev);
	xennet_alloc_rx_buffers(dev);

	spin_unlock_irq(&np->tx_lock);
	spin_unlock_bh(&np->rx_lock);
=======
	for (j = 0; j < num_queues; ++j) {
		queue = &np->queues[j];

		notify_remote_via_irq(queue->tx_irq);
		if (queue->tx_irq != queue->rx_irq)
			notify_remote_via_irq(queue->rx_irq);

		spin_lock_irq(&queue->tx_lock);
		xennet_tx_buf_gc(queue);
		spin_unlock_irq(&queue->tx_lock);

		spin_lock_bh(&queue->rx_lock);
		xennet_alloc_rx_buffers(queue);
		spin_unlock_bh(&queue->rx_lock);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

/**
 * Callback received when the backend's state changes.
 */
static void netback_changed(struct xenbus_device *dev,
			    enum xenbus_state backend_state)
{
	struct netfront_info *np = dev_get_drvdata(&dev->dev);
	struct net_device *netdev = np->netdev;

	dev_dbg(&dev->dev, "%s\n", xenbus_strstate(backend_state));

<<<<<<< HEAD
=======
	wake_up_all(&module_wq);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	switch (backend_state) {
	case XenbusStateInitialising:
	case XenbusStateInitialised:
	case XenbusStateReconfiguring:
	case XenbusStateReconfigured:
	case XenbusStateUnknown:
<<<<<<< HEAD
	case XenbusStateClosed:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		break;

	case XenbusStateInitWait:
		if (dev->state != XenbusStateInitialising)
			break;
		if (xennet_connect(netdev) != 0)
			break;
		xenbus_switch_state(dev, XenbusStateConnected);
		break;

	case XenbusStateConnected:
		netdev_notify_peers(netdev);
		break;

<<<<<<< HEAD
=======
	case XenbusStateClosed:
		if (dev->state == XenbusStateClosed)
			break;
		/* Missed the backend's CLOSING state -- fallthrough */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case XenbusStateClosing:
		xenbus_frontend_closed(dev);
		break;
	}
}

static const struct xennet_stat {
	char name[ETH_GSTRING_LEN];
	u16 offset;
} xennet_stats[] = {
	{
		"rx_gso_checksum_fixup",
		offsetof(struct netfront_info, rx_gso_checksum_fixup)
	},
};

static int xennet_get_sset_count(struct net_device *dev, int string_set)
{
	switch (string_set) {
	case ETH_SS_STATS:
		return ARRAY_SIZE(xennet_stats);
	default:
		return -EINVAL;
	}
}

static void xennet_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 * data)
{
	void *np = netdev_priv(dev);
	int i;

	for (i = 0; i < ARRAY_SIZE(xennet_stats); i++)
<<<<<<< HEAD
		data[i] = *(unsigned long *)(np + xennet_stats[i].offset);
=======
		data[i] = atomic_read((atomic_t *)(np + xennet_stats[i].offset));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void xennet_get_strings(struct net_device *dev, u32 stringset, u8 * data)
{
	int i;

	switch (stringset) {
	case ETH_SS_STATS:
		for (i = 0; i < ARRAY_SIZE(xennet_stats); i++)
			memcpy(data + i * ETH_GSTRING_LEN,
			       xennet_stats[i].name, ETH_GSTRING_LEN);
		break;
	}
}

static const struct ethtool_ops xennet_ethtool_ops =
{
	.get_link = ethtool_op_get_link,

	.get_sset_count = xennet_get_sset_count,
	.get_ethtool_stats = xennet_get_ethtool_stats,
	.get_strings = xennet_get_strings,
};

#ifdef CONFIG_SYSFS
<<<<<<< HEAD
static ssize_t show_rxbuf_min(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);

	return sprintf(buf, "%u\n", info->rx_min_target);
}

static ssize_t store_rxbuf_min(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *np = netdev_priv(netdev);
	char *endp;
	unsigned long target;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	target = simple_strtoul(buf, &endp, 0);
	if (endp == buf)
		return -EBADMSG;

	if (target < RX_MIN_TARGET)
		target = RX_MIN_TARGET;
	if (target > RX_MAX_TARGET)
		target = RX_MAX_TARGET;

	spin_lock_bh(&np->rx_lock);
	if (target > np->rx_max_target)
		np->rx_max_target = target;
	np->rx_min_target = target;
	if (target > np->rx_target)
		np->rx_target = target;

	xennet_alloc_rx_buffers(netdev);

	spin_unlock_bh(&np->rx_lock);
	return len;
}

static ssize_t show_rxbuf_max(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);

	return sprintf(buf, "%u\n", info->rx_max_target);
}

static ssize_t store_rxbuf_max(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t len)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *np = netdev_priv(netdev);
=======
static ssize_t show_rxbuf(struct device *dev,
			  struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%lu\n", NET_RX_RING_SIZE);
}

static ssize_t store_rxbuf(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t len)
{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	char *endp;
	unsigned long target;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	target = simple_strtoul(buf, &endp, 0);
	if (endp == buf)
		return -EBADMSG;

<<<<<<< HEAD
	if (target < RX_MIN_TARGET)
		target = RX_MIN_TARGET;
	if (target > RX_MAX_TARGET)
		target = RX_MAX_TARGET;

	spin_lock_bh(&np->rx_lock);
	if (target < np->rx_min_target)
		np->rx_min_target = target;
	np->rx_max_target = target;
	if (target < np->rx_target)
		np->rx_target = target;

	xennet_alloc_rx_buffers(netdev);

	spin_unlock_bh(&np->rx_lock);
	return len;
}

static ssize_t show_rxbuf_cur(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct net_device *netdev = to_net_dev(dev);
	struct netfront_info *info = netdev_priv(netdev);

	return sprintf(buf, "%u\n", info->rx_target);
}

static struct device_attribute xennet_attrs[] = {
	__ATTR(rxbuf_min, S_IRUGO|S_IWUSR, show_rxbuf_min, store_rxbuf_min),
	__ATTR(rxbuf_max, S_IRUGO|S_IWUSR, show_rxbuf_max, store_rxbuf_max),
	__ATTR(rxbuf_cur, S_IRUGO, show_rxbuf_cur, NULL),
};

static int xennet_sysfs_addif(struct net_device *netdev)
{
	int i;
	int err;

	for (i = 0; i < ARRAY_SIZE(xennet_attrs); i++) {
		err = device_create_file(&netdev->dev,
					   &xennet_attrs[i]);
		if (err)
			goto fail;
	}
	return 0;

 fail:
	while (--i >= 0)
		device_remove_file(&netdev->dev, &xennet_attrs[i]);
	return err;
}

static void xennet_sysfs_delif(struct net_device *netdev)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(xennet_attrs); i++)
		device_remove_file(&netdev->dev, &xennet_attrs[i]);
}

#endif /* CONFIG_SYSFS */

static const struct xenbus_device_id netfront_ids[] = {
	{ "vif" },
	{ "" }
};


=======
	/* rxbuf_min and rxbuf_max are no longer configurable. */

	return len;
}

static DEVICE_ATTR(rxbuf_min, S_IRUGO|S_IWUSR, show_rxbuf, store_rxbuf);
static DEVICE_ATTR(rxbuf_max, S_IRUGO|S_IWUSR, show_rxbuf, store_rxbuf);
static DEVICE_ATTR(rxbuf_cur, S_IRUGO, show_rxbuf, NULL);

static struct attribute *xennet_dev_attrs[] = {
	&dev_attr_rxbuf_min.attr,
	&dev_attr_rxbuf_max.attr,
	&dev_attr_rxbuf_cur.attr,
	NULL
};

static const struct attribute_group xennet_dev_group = {
	.attrs = xennet_dev_attrs
};
#endif /* CONFIG_SYSFS */

static void xennet_bus_close(struct xenbus_device *dev)
{
	int ret;

	if (xenbus_read_driver_state(dev->otherend) == XenbusStateClosed)
		return;
	do {
		xenbus_switch_state(dev, XenbusStateClosing);
		ret = wait_event_timeout(module_wq,
				   xenbus_read_driver_state(dev->otherend) ==
				   XenbusStateClosing ||
				   xenbus_read_driver_state(dev->otherend) ==
				   XenbusStateClosed ||
				   xenbus_read_driver_state(dev->otherend) ==
				   XenbusStateUnknown,
				   XENNET_TIMEOUT);
	} while (!ret);

	if (xenbus_read_driver_state(dev->otherend) == XenbusStateClosed)
		return;

	do {
		xenbus_switch_state(dev, XenbusStateClosed);
		ret = wait_event_timeout(module_wq,
				   xenbus_read_driver_state(dev->otherend) ==
				   XenbusStateClosed ||
				   xenbus_read_driver_state(dev->otherend) ==
				   XenbusStateUnknown,
				   XENNET_TIMEOUT);
	} while (!ret);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int xennet_remove(struct xenbus_device *dev)
{
	struct netfront_info *info = dev_get_drvdata(&dev->dev);

<<<<<<< HEAD
	dev_dbg(&dev->dev, "%s\n", dev->nodename);

	xennet_disconnect_backend(info);

	xennet_sysfs_delif(info->netdev);

	unregister_netdev(info->netdev);

	del_timer_sync(&info->rx_refill_timer);

	free_percpu(info->stats);

	free_netdev(info->netdev);
=======
	xennet_bus_close(dev);
	xennet_disconnect_backend(info);

	if (info->netdev->reg_state == NETREG_REGISTERED)
		unregister_netdev(info->netdev);

	if (info->queues) {
		rtnl_lock();
		xennet_destroy_queues(info);
		rtnl_unlock();
	}
	xennet_free_netdev(info->netdev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return 0;
}

<<<<<<< HEAD
static DEFINE_XENBUS_DRIVER(netfront, ,
=======
static const struct xenbus_device_id netfront_ids[] = {
	{ "vif" },
	{ "" }
};

static struct xenbus_driver netfront_driver = {
	.ids = netfront_ids,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.probe = netfront_probe,
	.remove = xennet_remove,
	.resume = netfront_resume,
	.otherend_changed = netback_changed,
<<<<<<< HEAD
);
=======
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int __init netif_init(void)
{
	if (!xen_domain())
		return -ENODEV;

<<<<<<< HEAD
	if (xen_hvm_domain() && !xen_platform_pci_unplug)
		return -ENODEV;

	printk(KERN_INFO "Initialising Xen virtual ethernet driver.\n");
=======
	if (!xen_has_pv_nic_devices())
		return -ENODEV;

	pr_info("Initialising Xen virtual ethernet driver\n");

	/* Allow as many queues as there are CPUs if user has not
	 * specified a value.
	 */
	if (xennet_max_queues == 0)
		xennet_max_queues = num_online_cpus();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return xenbus_register_frontend(&netfront_driver);
}
module_init(netif_init);


static void __exit netif_exit(void)
{
	xenbus_unregister_driver(&netfront_driver);
}
module_exit(netif_exit);

MODULE_DESCRIPTION("Xen virtual network device frontend");
MODULE_LICENSE("GPL");
MODULE_ALIAS("xen:vif");
MODULE_ALIAS("xennet");
