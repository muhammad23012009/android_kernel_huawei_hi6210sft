/*
 * net/sched/sch_generic.c	Generic packet scheduler routines.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *              Jamal Hadi Salim, <hadi@cyberus.ca> 990601
 *              - Ingress support
 */

#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/init.h>
#include <linux/rcupdate.h>
#include <linux/list.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/if_vlan.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <net/sch_generic.h>
#include <net/pkt_sched.h>
#include <net/dst.h>

<<<<<<< HEAD
=======
/* Qdisc to use by default */
const struct Qdisc_ops *default_qdisc_ops = &pfifo_fast_ops;
EXPORT_SYMBOL(default_qdisc_ops);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Main transmission queue. */

/* Modifications to data participating in scheduling must be protected with
 * qdisc_lock(qdisc) spinlock.
 *
 * The idea is the following:
 * - enqueue, dequeue are serialized via qdisc root lock
 * - ingress filtering is also serialized via qdisc root lock
 * - updates to tree and tree walking are only done under the rtnl mutex.
 */

static inline int dev_requeue_skb(struct sk_buff *skb, struct Qdisc *q)
{
<<<<<<< HEAD
	skb_dst_force(skb);
	q->gso_skb = skb;
	q->qstats.requeues++;
=======
	q->gso_skb = skb;
	q->qstats.requeues++;
	qdisc_qstats_backlog_inc(q, skb);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	q->q.qlen++;	/* it's still part of the queue */
	__netif_schedule(q);

	return 0;
}

<<<<<<< HEAD
static inline struct sk_buff *dequeue_skb(struct Qdisc *q)
=======
static void try_bulk_dequeue_skb(struct Qdisc *q,
				 struct sk_buff *skb,
				 const struct netdev_queue *txq,
				 int *packets)
{
	int bytelimit = qdisc_avail_bulklimit(txq) - skb->len;

	while (bytelimit > 0) {
		struct sk_buff *nskb = q->dequeue(q);

		if (!nskb)
			break;

		bytelimit -= nskb->len; /* covers GSO len */
		skb->next = nskb;
		skb = nskb;
		(*packets)++; /* GSO counts as one pkt */
	}
	skb->next = NULL;
}

/* This variant of try_bulk_dequeue_skb() makes sure
 * all skbs in the chain are for the same txq
 */
static void try_bulk_dequeue_skb_slow(struct Qdisc *q,
				      struct sk_buff *skb,
				      int *packets)
{
	int mapping = skb_get_queue_mapping(skb);
	struct sk_buff *nskb;
	int cnt = 0;

	do {
		nskb = q->dequeue(q);
		if (!nskb)
			break;
		if (unlikely(skb_get_queue_mapping(nskb) != mapping)) {
			q->skb_bad_txq = nskb;
			qdisc_qstats_backlog_inc(q, nskb);
			q->q.qlen++;
			break;
		}
		skb->next = nskb;
		skb = nskb;
	} while (++cnt < 8);
	(*packets) += cnt;
	skb->next = NULL;
}

/* Note that dequeue_skb can possibly return a SKB list (via skb->next).
 * A requeued skb (via q->gso_skb) can also be a SKB list.
 */
static struct sk_buff *dequeue_skb(struct Qdisc *q, bool *validate,
				   int *packets)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct sk_buff *skb = q->gso_skb;
	const struct netdev_queue *txq = q->dev_queue;

<<<<<<< HEAD
	if (unlikely(skb)) {
		/* check the reason of requeuing without tx lock first */
		txq = netdev_get_tx_queue(txq->dev, skb_get_queue_mapping(skb));
		if (!netif_xmit_frozen_or_stopped(txq)) {
			q->gso_skb = NULL;
			q->q.qlen--;
		} else
			skb = NULL;
	} else {
		if (!(q->flags & TCQ_F_ONETXQUEUE) || !netif_xmit_frozen_or_stopped(txq))
			skb = q->dequeue(q);
	}

	return skb;
}

static inline int handle_dev_cpu_collision(struct sk_buff *skb,
					   struct netdev_queue *dev_queue,
					   struct Qdisc *q)
{
	int ret;

	if (unlikely(dev_queue->xmit_lock_owner == smp_processor_id())) {
		/*
		 * Same CPU holding the lock. It may be a transient
		 * configuration error, when hard_start_xmit() recurses. We
		 * detect it by checking xmit owner and drop the packet when
		 * deadloop is detected. Return OK to try the next skb.
		 */
		kfree_skb(skb);
		net_warn_ratelimited("Dead loop on netdevice %s, fix it urgently!\n",
				     dev_queue->dev->name);
		ret = qdisc_qlen(q);
	} else {
		/*
		 * Another cpu is holding lock, requeue & delay xmits for
		 * some time.
		 */
		__this_cpu_inc(softnet_data.cpu_collision);
		ret = dev_requeue_skb(skb, q);
	}

	return ret;
}

/*
 * Transmit one skb, and handle the return status as required. Holding the
 * __QDISC_STATE_RUNNING bit guarantees that only one CPU can execute this
 * function.
=======
	*packets = 1;
	if (unlikely(skb)) {
		/* skb in gso_skb were already validated */
		*validate = false;
		/* check the reason of requeuing without tx lock first */
		txq = skb_get_tx_queue(txq->dev, skb);
		if (!netif_xmit_frozen_or_stopped(txq)) {
			q->gso_skb = NULL;
			qdisc_qstats_backlog_dec(q, skb);
			q->q.qlen--;
		} else
			skb = NULL;
		return skb;
	}
	*validate = true;
	skb = q->skb_bad_txq;
	if (unlikely(skb)) {
		/* check the reason of requeuing without tx lock first */
		txq = skb_get_tx_queue(txq->dev, skb);
		if (!netif_xmit_frozen_or_stopped(txq)) {
			q->skb_bad_txq = NULL;
			qdisc_qstats_backlog_dec(q, skb);
			q->q.qlen--;
			goto bulk;
		}
		return NULL;
	}
	if (!(q->flags & TCQ_F_ONETXQUEUE) ||
	    !netif_xmit_frozen_or_stopped(txq))
		skb = q->dequeue(q);
	if (skb) {
bulk:
		if (qdisc_may_bulk(q))
			try_bulk_dequeue_skb(q, skb, txq, packets);
		else
			try_bulk_dequeue_skb_slow(q, skb, packets);
	}
	return skb;
}

/*
 * Transmit possibly several skbs, and handle the return status as
 * required. Owning running seqcount bit guarantees that
 * only one CPU can execute this function.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Returns to the caller:
 *				0  - queue is empty or throttled.
 *				>0 - queue is not empty.
 */
int sch_direct_xmit(struct sk_buff *skb, struct Qdisc *q,
		    struct net_device *dev, struct netdev_queue *txq,
<<<<<<< HEAD
		    spinlock_t *root_lock)
=======
		    spinlock_t *root_lock, bool validate)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int ret = NETDEV_TX_BUSY;

	/* And release qdisc */
	spin_unlock(root_lock);

<<<<<<< HEAD
	HARD_TX_LOCK(dev, txq, smp_processor_id());
	if (!netif_xmit_frozen_or_stopped(txq))
		ret = dev_hard_start_xmit(skb, dev, txq);

	HARD_TX_UNLOCK(dev, txq);

=======
	/* Note that we validate skb (GSO, checksum, ...) outside of locks */
	if (validate)
		skb = validate_xmit_skb_list(skb, dev);

	if (likely(skb)) {
		HARD_TX_LOCK(dev, txq, smp_processor_id());
		if (!netif_xmit_frozen_or_stopped(txq))
			skb = dev_hard_start_xmit(skb, dev, txq, &ret);

		HARD_TX_UNLOCK(dev, txq);
	} else {
		spin_lock(root_lock);
		return qdisc_qlen(q);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	spin_lock(root_lock);

	if (dev_xmit_complete(ret)) {
		/* Driver sent out skb successfully or skb was consumed */
		ret = qdisc_qlen(q);
<<<<<<< HEAD
	} else if (ret == NETDEV_TX_LOCKED) {
		/* Driver try lock failed */
		ret = handle_dev_cpu_collision(skb, txq, q);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	} else {
		/* Driver returned NETDEV_TX_BUSY - requeue skb */
		if (unlikely(ret != NETDEV_TX_BUSY))
			net_warn_ratelimited("BUG %s code %d qlen %d\n",
					     dev->name, ret, q->q.qlen);

		ret = dev_requeue_skb(skb, q);
	}

	if (ret && netif_xmit_frozen_or_stopped(txq))
		ret = 0;

	return ret;
}

/*
 * NOTE: Called under qdisc_lock(q) with locally disabled BH.
 *
<<<<<<< HEAD
 * __QDISC_STATE_RUNNING guarantees only one CPU can process
=======
 * running seqcount guarantees only one CPU can process
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * this qdisc at a time. qdisc_lock(q) serializes queue accesses for
 * this queue.
 *
 *  netif_tx_lock serializes accesses to device driver.
 *
 *  qdisc_lock(q) and netif_tx_lock are mutually exclusive,
 *  if one is grabbed, another must be free.
 *
 * Note, that this procedure can be called by a watchdog timer
 *
 * Returns to the caller:
 *				0  - queue is empty or throttled.
 *				>0 - queue is not empty.
 *
 */
<<<<<<< HEAD
static inline int qdisc_restart(struct Qdisc *q)
=======
static inline int qdisc_restart(struct Qdisc *q, int *packets)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct netdev_queue *txq;
	struct net_device *dev;
	spinlock_t *root_lock;
	struct sk_buff *skb;
<<<<<<< HEAD

	/* Dequeue packet */
	skb = dequeue_skb(q);
	if (unlikely(!skb))
		return 0;
	WARN_ON_ONCE(skb_dst_is_noref(skb));
	root_lock = qdisc_lock(q);
	dev = qdisc_dev(q);
	txq = netdev_get_tx_queue(dev, skb_get_queue_mapping(skb));

	return sch_direct_xmit(skb, q, dev, txq, root_lock);
=======
	bool validate;

	/* Dequeue packet */
	skb = dequeue_skb(q, &validate, packets);
	if (unlikely(!skb))
		return 0;

	root_lock = qdisc_lock(q);
	dev = qdisc_dev(q);
	txq = skb_get_tx_queue(dev, skb);

	return sch_direct_xmit(skb, q, dev, txq, root_lock, validate);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __qdisc_run(struct Qdisc *q)
{
	int quota = weight_p;
<<<<<<< HEAD

	while (qdisc_restart(q)) {
=======
	int packets;

	while (qdisc_restart(q, &packets)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		/*
		 * Ordered by possible occurrence: Postpone processing if
		 * 1. we've exceeded packet quota
		 * 2. another process needs the CPU;
		 */
<<<<<<< HEAD
		if (--quota <= 0 || need_resched()) {
=======
		quota -= packets;
		if (quota <= 0 || need_resched()) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			__netif_schedule(q);
			break;
		}
	}

	qdisc_run_end(q);
}

unsigned long dev_trans_start(struct net_device *dev)
{
<<<<<<< HEAD
	unsigned long val, res = dev->trans_start;
	unsigned int i;

	for (i = 0; i < dev->num_tx_queues; i++) {
=======
	unsigned long val, res;
	unsigned int i;

	if (is_vlan_dev(dev))
		dev = vlan_dev_real_dev(dev);
	res = netdev_get_tx_queue(dev, 0)->trans_start;
	for (i = 1; i < dev->num_tx_queues; i++) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		val = netdev_get_tx_queue(dev, i)->trans_start;
		if (val && time_after(val, res))
			res = val;
	}
<<<<<<< HEAD
	dev->trans_start = res;
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return res;
}
EXPORT_SYMBOL(dev_trans_start);

static void dev_watchdog(unsigned long arg)
{
	struct net_device *dev = (struct net_device *)arg;

	netif_tx_lock(dev);
	if (!qdisc_tx_is_noop(dev)) {
		if (netif_device_present(dev) &&
		    netif_running(dev) &&
		    netif_carrier_ok(dev)) {
			int some_queue_timedout = 0;
			unsigned int i;
			unsigned long trans_start;

			for (i = 0; i < dev->num_tx_queues; i++) {
				struct netdev_queue *txq;

				txq = netdev_get_tx_queue(dev, i);
<<<<<<< HEAD
				/*
				 * old device drivers set dev->trans_start
				 */
				trans_start = txq->trans_start ? : dev->trans_start;
=======
				trans_start = txq->trans_start;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				if (netif_xmit_stopped(txq) &&
				    time_after(jiffies, (trans_start +
							 dev->watchdog_timeo))) {
					some_queue_timedout = 1;
					txq->trans_timeout++;
					break;
				}
			}

			if (some_queue_timedout) {
				WARN_ONCE(1, KERN_INFO "NETDEV WATCHDOG: %s (%s): transmit queue %u timed out\n",
				       dev->name, netdev_drivername(dev), i);
				dev->netdev_ops->ndo_tx_timeout(dev);
			}
			if (!mod_timer(&dev->watchdog_timer,
				       round_jiffies(jiffies +
						     dev->watchdog_timeo)))
				dev_hold(dev);
		}
	}
	netif_tx_unlock(dev);

	dev_put(dev);
}

void __netdev_watchdog_up(struct net_device *dev)
{
	if (dev->netdev_ops->ndo_tx_timeout) {
		if (dev->watchdog_timeo <= 0)
			dev->watchdog_timeo = 5*HZ;
		if (!mod_timer(&dev->watchdog_timer,
			       round_jiffies(jiffies + dev->watchdog_timeo)))
			dev_hold(dev);
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(__netdev_watchdog_up);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void dev_watchdog_up(struct net_device *dev)
{
	__netdev_watchdog_up(dev);
}

static void dev_watchdog_down(struct net_device *dev)
{
	netif_tx_lock_bh(dev);
	if (del_timer(&dev->watchdog_timer))
		dev_put(dev);
	netif_tx_unlock_bh(dev);
}

/**
 *	netif_carrier_on - set carrier
 *	@dev: network device
 *
 * Device has detected that carrier.
 */
void netif_carrier_on(struct net_device *dev)
{
	if (test_and_clear_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
		if (dev->reg_state == NETREG_UNINITIALIZED)
			return;
<<<<<<< HEAD
=======
		atomic_inc(&dev->carrier_changes);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		linkwatch_fire_event(dev);
		if (netif_running(dev))
			__netdev_watchdog_up(dev);
	}
}
EXPORT_SYMBOL(netif_carrier_on);

/**
 *	netif_carrier_off - clear carrier
 *	@dev: network device
 *
 * Device has detected loss of carrier.
 */
void netif_carrier_off(struct net_device *dev)
{
	if (!test_and_set_bit(__LINK_STATE_NOCARRIER, &dev->state)) {
		if (dev->reg_state == NETREG_UNINITIALIZED)
			return;
<<<<<<< HEAD
=======
		atomic_inc(&dev->carrier_changes);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		linkwatch_fire_event(dev);
	}
}
EXPORT_SYMBOL(netif_carrier_off);

/* "NOOP" scheduler: the best scheduler, recommended for all interfaces
   under all circumstances. It is difficult to invent anything faster or
   cheaper.
 */

<<<<<<< HEAD
static int noop_enqueue(struct sk_buff *skb, struct Qdisc * qdisc)
{
	kfree_skb(skb);
	return NET_XMIT_CN;
}

static struct sk_buff *noop_dequeue(struct Qdisc * qdisc)
=======
static int noop_enqueue(struct sk_buff *skb, struct Qdisc *qdisc,
			struct sk_buff **to_free)
{
	__qdisc_drop(skb, to_free);
	return NET_XMIT_CN;
}

static struct sk_buff *noop_dequeue(struct Qdisc *qdisc)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return NULL;
}

struct Qdisc_ops noop_qdisc_ops __read_mostly = {
	.id		=	"noop",
	.priv_size	=	0,
	.enqueue	=	noop_enqueue,
	.dequeue	=	noop_dequeue,
	.peek		=	noop_dequeue,
	.owner		=	THIS_MODULE,
};

static struct netdev_queue noop_netdev_queue = {
	.qdisc		=	&noop_qdisc,
	.qdisc_sleeping	=	&noop_qdisc,
};

struct Qdisc noop_qdisc = {
	.enqueue	=	noop_enqueue,
	.dequeue	=	noop_dequeue,
	.flags		=	TCQ_F_BUILTIN,
	.ops		=	&noop_qdisc_ops,
<<<<<<< HEAD
	.list		=	LIST_HEAD_INIT(noop_qdisc.list),
	.q.lock		=	__SPIN_LOCK_UNLOCKED(noop_qdisc.q.lock),
	.dev_queue	=	&noop_netdev_queue,
=======
	.q.lock		=	__SPIN_LOCK_UNLOCKED(noop_qdisc.q.lock),
	.dev_queue	=	&noop_netdev_queue,
	.running	=	SEQCNT_ZERO(noop_qdisc.running),
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.busylock	=	__SPIN_LOCK_UNLOCKED(noop_qdisc.busylock),
};
EXPORT_SYMBOL(noop_qdisc);

<<<<<<< HEAD
static struct Qdisc_ops noqueue_qdisc_ops __read_mostly = {
	.id		=	"noqueue",
	.priv_size	=	0,
=======
static int noqueue_init(struct Qdisc *qdisc, struct nlattr *opt)
{
	/* register_qdisc() assigns a default of noop_enqueue if unset,
	 * but __dev_queue_xmit() treats noqueue only as such
	 * if this is NULL - so clear it here. */
	qdisc->enqueue = NULL;
	return 0;
}

struct Qdisc_ops noqueue_qdisc_ops __read_mostly = {
	.id		=	"noqueue",
	.priv_size	=	0,
	.init		=	noqueue_init,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.enqueue	=	noop_enqueue,
	.dequeue	=	noop_dequeue,
	.peek		=	noop_dequeue,
	.owner		=	THIS_MODULE,
};

<<<<<<< HEAD
static struct Qdisc noqueue_qdisc;
static struct netdev_queue noqueue_netdev_queue = {
	.qdisc		=	&noqueue_qdisc,
	.qdisc_sleeping	=	&noqueue_qdisc,
};

static struct Qdisc noqueue_qdisc = {
	.enqueue	=	NULL,
	.dequeue	=	noop_dequeue,
	.flags		=	TCQ_F_BUILTIN,
	.ops		=	&noqueue_qdisc_ops,
	.list		=	LIST_HEAD_INIT(noqueue_qdisc.list),
	.q.lock		=	__SPIN_LOCK_UNLOCKED(noqueue_qdisc.q.lock),
	.dev_queue	=	&noqueue_netdev_queue,
	.busylock	=	__SPIN_LOCK_UNLOCKED(noqueue_qdisc.busylock),
};


=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static const u8 prio2band[TC_PRIO_MAX + 1] = {
	1, 2, 2, 2, 1, 2, 0, 0 , 1, 1, 1, 1, 1, 1, 1, 1
};

/* 3-band FIFO queue: old style, but should be a bit faster than
   generic prio+fifo combination.
 */

#define PFIFO_FAST_BANDS 3

/*
 * Private data for a pfifo_fast scheduler containing:
 * 	- queues for the three band
 * 	- bitmap indicating which of the bands contain skbs
 */
struct pfifo_fast_priv {
	u32 bitmap;
<<<<<<< HEAD
	struct sk_buff_head q[PFIFO_FAST_BANDS];
=======
	struct qdisc_skb_head q[PFIFO_FAST_BANDS];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/*
 * Convert a bitmap to the first band number where an skb is queued, where:
 * 	bitmap=0 means there are no skbs on any band.
 * 	bitmap=1 means there is an skb on band 0.
 *	bitmap=7 means there are skbs on all 3 bands, etc.
 */
static const int bitmap2band[] = {-1, 0, 1, 0, 2, 0, 1, 0};

<<<<<<< HEAD
static inline struct sk_buff_head *band2list(struct pfifo_fast_priv *priv,
=======
static inline struct qdisc_skb_head *band2list(struct pfifo_fast_priv *priv,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					     int band)
{
	return priv->q + band;
}

<<<<<<< HEAD
static int pfifo_fast_enqueue(struct sk_buff *skb, struct Qdisc *qdisc)
{
	if (skb_queue_len(&qdisc->q) < qdisc_dev(qdisc)->tx_queue_len) {
		int band = prio2band[skb->priority & TC_PRIO_MAX];
		struct pfifo_fast_priv *priv = qdisc_priv(qdisc);
		struct sk_buff_head *list = band2list(priv, band);
=======
static int pfifo_fast_enqueue(struct sk_buff *skb, struct Qdisc *qdisc,
			      struct sk_buff **to_free)
{
	if (qdisc->q.qlen < qdisc_dev(qdisc)->tx_queue_len) {
		int band = prio2band[skb->priority & TC_PRIO_MAX];
		struct pfifo_fast_priv *priv = qdisc_priv(qdisc);
		struct qdisc_skb_head *list = band2list(priv, band);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		priv->bitmap |= (1 << band);
		qdisc->q.qlen++;
		return __qdisc_enqueue_tail(skb, qdisc, list);
	}

<<<<<<< HEAD
	return qdisc_drop(skb, qdisc);
=======
	return qdisc_drop(skb, qdisc, to_free);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static struct sk_buff *pfifo_fast_dequeue(struct Qdisc *qdisc)
{
	struct pfifo_fast_priv *priv = qdisc_priv(qdisc);
	int band = bitmap2band[priv->bitmap];

	if (likely(band >= 0)) {
<<<<<<< HEAD
		struct sk_buff_head *list = band2list(priv, band);
		struct sk_buff *skb = __qdisc_dequeue_head(qdisc, list);

		qdisc->q.qlen--;
		if (skb_queue_empty(list))
=======
		struct qdisc_skb_head *qh = band2list(priv, band);
		struct sk_buff *skb = __qdisc_dequeue_head(qh);

		if (likely(skb != NULL)) {
			qdisc_qstats_backlog_dec(qdisc, skb);
			qdisc_bstats_update(qdisc, skb);
		}

		qdisc->q.qlen--;
		if (qh->qlen == 0)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			priv->bitmap &= ~(1 << band);

		return skb;
	}

	return NULL;
}

static struct sk_buff *pfifo_fast_peek(struct Qdisc *qdisc)
{
	struct pfifo_fast_priv *priv = qdisc_priv(qdisc);
	int band = bitmap2band[priv->bitmap];

	if (band >= 0) {
<<<<<<< HEAD
		struct sk_buff_head *list = band2list(priv, band);

		return skb_peek(list);
=======
		struct qdisc_skb_head *qh = band2list(priv, band);

		return qh->head;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return NULL;
}

static void pfifo_fast_reset(struct Qdisc *qdisc)
{
	int prio;
	struct pfifo_fast_priv *priv = qdisc_priv(qdisc);

	for (prio = 0; prio < PFIFO_FAST_BANDS; prio++)
<<<<<<< HEAD
		__qdisc_reset_queue(qdisc, band2list(priv, prio));
=======
		__qdisc_reset_queue(band2list(priv, prio));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	priv->bitmap = 0;
	qdisc->qstats.backlog = 0;
	qdisc->q.qlen = 0;
}

static int pfifo_fast_dump(struct Qdisc *qdisc, struct sk_buff *skb)
{
	struct tc_prio_qopt opt = { .bands = PFIFO_FAST_BANDS };

	memcpy(&opt.priomap, prio2band, TC_PRIO_MAX + 1);
	if (nla_put(skb, TCA_OPTIONS, sizeof(opt), &opt))
		goto nla_put_failure;
	return skb->len;

nla_put_failure:
	return -1;
}

static int pfifo_fast_init(struct Qdisc *qdisc, struct nlattr *opt)
{
	int prio;
	struct pfifo_fast_priv *priv = qdisc_priv(qdisc);

	for (prio = 0; prio < PFIFO_FAST_BANDS; prio++)
<<<<<<< HEAD
		skb_queue_head_init(band2list(priv, prio));
=======
		qdisc_skb_head_init(band2list(priv, prio));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* Can by-pass the queue discipline */
	qdisc->flags |= TCQ_F_CAN_BYPASS;
	return 0;
}

struct Qdisc_ops pfifo_fast_ops __read_mostly = {
	.id		=	"pfifo_fast",
	.priv_size	=	sizeof(struct pfifo_fast_priv),
	.enqueue	=	pfifo_fast_enqueue,
	.dequeue	=	pfifo_fast_dequeue,
	.peek		=	pfifo_fast_peek,
	.init		=	pfifo_fast_init,
	.reset		=	pfifo_fast_reset,
	.dump		=	pfifo_fast_dump,
	.owner		=	THIS_MODULE,
};
EXPORT_SYMBOL(pfifo_fast_ops);

static struct lock_class_key qdisc_tx_busylock;
<<<<<<< HEAD

struct Qdisc *qdisc_alloc(struct netdev_queue *dev_queue,
			  struct Qdisc_ops *ops)
=======
static struct lock_class_key qdisc_running_key;

struct Qdisc *qdisc_alloc(struct netdev_queue *dev_queue,
			  const struct Qdisc_ops *ops)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	void *p;
	struct Qdisc *sch;
	unsigned int size = QDISC_ALIGN(sizeof(*sch)) + ops->priv_size;
	int err = -ENOBUFS;
	struct net_device *dev = dev_queue->dev;

	p = kzalloc_node(size, GFP_KERNEL,
			 netdev_queue_numa_node_read(dev_queue));

	if (!p)
		goto errout;
	sch = (struct Qdisc *) QDISC_ALIGN((unsigned long) p);
	/* if we got non aligned memory, ask more and do alignment ourself */
	if (sch != p) {
		kfree(p);
		p = kzalloc_node(size + QDISC_ALIGNTO - 1, GFP_KERNEL,
				 netdev_queue_numa_node_read(dev_queue));
		if (!p)
			goto errout;
		sch = (struct Qdisc *) QDISC_ALIGN((unsigned long) p);
		sch->padded = (char *) sch - (char *) p;
	}
<<<<<<< HEAD
	INIT_LIST_HEAD(&sch->list);
	skb_queue_head_init(&sch->q);
=======
	qdisc_skb_head_init(&sch->q);
	spin_lock_init(&sch->q.lock);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	spin_lock_init(&sch->busylock);
	lockdep_set_class(&sch->busylock,
			  dev->qdisc_tx_busylock ?: &qdisc_tx_busylock);

<<<<<<< HEAD
=======
	seqcount_init(&sch->running);
	lockdep_set_class(&sch->running,
			  dev->qdisc_running_key ?: &qdisc_running_key);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sch->ops = ops;
	sch->enqueue = ops->enqueue;
	sch->dequeue = ops->dequeue;
	sch->dev_queue = dev_queue;
	dev_hold(dev);
	atomic_set(&sch->refcnt, 1);

	return sch;
errout:
	return ERR_PTR(err);
}

struct Qdisc *qdisc_create_dflt(struct netdev_queue *dev_queue,
<<<<<<< HEAD
				struct Qdisc_ops *ops, unsigned int parentid)
{
	struct Qdisc *sch;

	sch = qdisc_alloc(dev_queue, ops);
	if (IS_ERR(sch))
		goto errout;
=======
				const struct Qdisc_ops *ops,
				unsigned int parentid)
{
	struct Qdisc *sch;

	if (!try_module_get(ops->owner))
		return NULL;

	sch = qdisc_alloc(dev_queue, ops);
	if (IS_ERR(sch)) {
		module_put(ops->owner);
		return NULL;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	sch->parent = parentid;

	if (!ops->init || ops->init(sch, NULL) == 0)
		return sch;

	qdisc_destroy(sch);
<<<<<<< HEAD
errout:
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return NULL;
}
EXPORT_SYMBOL(qdisc_create_dflt);

/* Under qdisc_lock(qdisc) and BH! */

void qdisc_reset(struct Qdisc *qdisc)
{
	const struct Qdisc_ops *ops = qdisc->ops;

	if (ops->reset)
		ops->reset(qdisc);

<<<<<<< HEAD
	if (qdisc->gso_skb) {
		kfree_skb(qdisc->gso_skb);
		qdisc->gso_skb = NULL;
		qdisc->q.qlen = 0;
	}
=======
	kfree_skb(qdisc->skb_bad_txq);
	qdisc->skb_bad_txq = NULL;

	if (qdisc->gso_skb) {
		kfree_skb_list(qdisc->gso_skb);
		qdisc->gso_skb = NULL;
	}
	qdisc->q.qlen = 0;
	qdisc->qstats.backlog = 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
EXPORT_SYMBOL(qdisc_reset);

static void qdisc_rcu_free(struct rcu_head *head)
{
	struct Qdisc *qdisc = container_of(head, struct Qdisc, rcu_head);

<<<<<<< HEAD
=======
	if (qdisc_is_percpu_stats(qdisc)) {
		free_percpu(qdisc->cpu_bstats);
		free_percpu(qdisc->cpu_qstats);
	}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	kfree((char *) qdisc - qdisc->padded);
}

void qdisc_destroy(struct Qdisc *qdisc)
{
<<<<<<< HEAD
	const struct Qdisc_ops  *ops = qdisc->ops;
=======
	const struct Qdisc_ops *ops;

	if (!qdisc)
		return;
	ops = qdisc->ops;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (qdisc->flags & TCQ_F_BUILTIN ||
	    !atomic_dec_and_test(&qdisc->refcnt))
		return;

#ifdef CONFIG_NET_SCHED
<<<<<<< HEAD
	qdisc_list_del(qdisc);
=======
	qdisc_hash_del(qdisc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	qdisc_put_stab(rtnl_dereference(qdisc->stab));
#endif
	gen_kill_estimator(&qdisc->bstats, &qdisc->rate_est);
	if (ops->reset)
		ops->reset(qdisc);
	if (ops->destroy)
		ops->destroy(qdisc);

	module_put(ops->owner);
	dev_put(qdisc_dev(qdisc));

<<<<<<< HEAD
	kfree_skb(qdisc->gso_skb);
=======
	kfree_skb_list(qdisc->gso_skb);
	kfree_skb(qdisc->skb_bad_txq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/*
	 * gen_estimator est_timer() might access qdisc->q.lock,
	 * wait a RCU grace period before freeing qdisc.
	 */
	call_rcu(&qdisc->rcu_head, qdisc_rcu_free);
}
EXPORT_SYMBOL(qdisc_destroy);

/* Attach toplevel qdisc to device queue. */
struct Qdisc *dev_graft_qdisc(struct netdev_queue *dev_queue,
			      struct Qdisc *qdisc)
{
	struct Qdisc *oqdisc = dev_queue->qdisc_sleeping;
	spinlock_t *root_lock;

	root_lock = qdisc_lock(oqdisc);
	spin_lock_bh(root_lock);

	/* Prune old scheduler */
	if (oqdisc && atomic_read(&oqdisc->refcnt) <= 1)
		qdisc_reset(oqdisc);

	/* ... and graft new one */
	if (qdisc == NULL)
		qdisc = &noop_qdisc;
	dev_queue->qdisc_sleeping = qdisc;
	rcu_assign_pointer(dev_queue->qdisc, &noop_qdisc);

	spin_unlock_bh(root_lock);

	return oqdisc;
}
EXPORT_SYMBOL(dev_graft_qdisc);

static void attach_one_default_qdisc(struct net_device *dev,
				     struct netdev_queue *dev_queue,
				     void *_unused)
{
<<<<<<< HEAD
	struct Qdisc *qdisc = &noqueue_qdisc;

	if (dev->tx_queue_len) {
		qdisc = qdisc_create_dflt(dev_queue,
					  &pfifo_fast_ops, TC_H_ROOT);
		if (!qdisc) {
			netdev_info(dev, "activation failed\n");
			return;
		}
		if (!netif_is_multiqueue(dev))
			qdisc->flags |= TCQ_F_ONETXQUEUE;
	}
=======
	struct Qdisc *qdisc;
	const struct Qdisc_ops *ops = default_qdisc_ops;

	if (dev->priv_flags & IFF_NO_QUEUE)
		ops = &noqueue_qdisc_ops;

	qdisc = qdisc_create_dflt(dev_queue, ops, TC_H_ROOT);
	if (!qdisc) {
		netdev_info(dev, "activation failed\n");
		return;
	}
	if (!netif_is_multiqueue(dev))
		qdisc->flags |= TCQ_F_ONETXQUEUE | TCQ_F_NOPARENT;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev_queue->qdisc_sleeping = qdisc;
}

static void attach_default_qdiscs(struct net_device *dev)
{
	struct netdev_queue *txq;
	struct Qdisc *qdisc;

	txq = netdev_get_tx_queue(dev, 0);

<<<<<<< HEAD
	if (!netif_is_multiqueue(dev) || dev->tx_queue_len == 0) {
=======
	if (!netif_is_multiqueue(dev) ||
	    dev->priv_flags & IFF_NO_QUEUE) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		netdev_for_each_tx_queue(dev, attach_one_default_qdisc, NULL);
		dev->qdisc = txq->qdisc_sleeping;
		atomic_inc(&dev->qdisc->refcnt);
	} else {
		qdisc = qdisc_create_dflt(txq, &mq_qdisc_ops, TC_H_ROOT);
		if (qdisc) {
<<<<<<< HEAD
			qdisc->ops->attach(qdisc);
			dev->qdisc = qdisc;
		}
	}
=======
			dev->qdisc = qdisc;
			qdisc->ops->attach(qdisc);
		}
	}
#ifdef CONFIG_NET_SCHED
	if (dev->qdisc)
		qdisc_hash_add(dev->qdisc);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void transition_one_qdisc(struct net_device *dev,
				 struct netdev_queue *dev_queue,
				 void *_need_watchdog)
{
	struct Qdisc *new_qdisc = dev_queue->qdisc_sleeping;
	int *need_watchdog_p = _need_watchdog;

	if (!(new_qdisc->flags & TCQ_F_BUILTIN))
		clear_bit(__QDISC_STATE_DEACTIVATED, &new_qdisc->state);

	rcu_assign_pointer(dev_queue->qdisc, new_qdisc);
<<<<<<< HEAD
	if (need_watchdog_p && new_qdisc != &noqueue_qdisc) {
=======
	if (need_watchdog_p) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_queue->trans_start = 0;
		*need_watchdog_p = 1;
	}
}

void dev_activate(struct net_device *dev)
{
	int need_watchdog;

	/* No queueing discipline is attached to device;
<<<<<<< HEAD
	   create default one i.e. pfifo_fast for devices,
	   which need queueing and noqueue_qdisc for
	   virtual interfaces
=======
	 * create default one for devices, which need queueing
	 * and noqueue_qdisc for virtual interfaces
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */

	if (dev->qdisc == &noop_qdisc)
		attach_default_qdiscs(dev);

	if (!netif_carrier_ok(dev))
		/* Delay activation until next carrier-on event */
		return;

	need_watchdog = 0;
	netdev_for_each_tx_queue(dev, transition_one_qdisc, &need_watchdog);
	if (dev_ingress_queue(dev))
		transition_one_qdisc(dev, dev_ingress_queue(dev), NULL);

	if (need_watchdog) {
<<<<<<< HEAD
		dev->trans_start = jiffies;
=======
		netif_trans_update(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dev_watchdog_up(dev);
	}
}
EXPORT_SYMBOL(dev_activate);

static void dev_deactivate_queue(struct net_device *dev,
				 struct netdev_queue *dev_queue,
				 void *_qdisc_default)
{
	struct Qdisc *qdisc_default = _qdisc_default;
	struct Qdisc *qdisc;

<<<<<<< HEAD
	qdisc = dev_queue->qdisc;
=======
	qdisc = rtnl_dereference(dev_queue->qdisc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (qdisc) {
		spin_lock_bh(qdisc_lock(qdisc));

		if (!(qdisc->flags & TCQ_F_BUILTIN))
			set_bit(__QDISC_STATE_DEACTIVATED, &qdisc->state);

		rcu_assign_pointer(dev_queue->qdisc, qdisc_default);
		qdisc_reset(qdisc);

		spin_unlock_bh(qdisc_lock(qdisc));
	}
}

static bool some_qdisc_is_busy(struct net_device *dev)
{
	unsigned int i;

	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *dev_queue;
		spinlock_t *root_lock;
		struct Qdisc *q;
		int val;

		dev_queue = netdev_get_tx_queue(dev, i);
		q = dev_queue->qdisc_sleeping;
		root_lock = qdisc_lock(q);

		spin_lock_bh(root_lock);

		val = (qdisc_is_running(q) ||
		       test_bit(__QDISC_STATE_SCHED, &q->state));

		spin_unlock_bh(root_lock);

		if (val)
			return true;
	}
	return false;
}

/**
 * 	dev_deactivate_many - deactivate transmissions on several devices
 * 	@head: list of devices to deactivate
 *
 *	This function returns only when all outstanding transmissions
 *	have completed, unless all devices are in dismantle phase.
 */
void dev_deactivate_many(struct list_head *head)
{
	struct net_device *dev;
	bool sync_needed = false;

<<<<<<< HEAD
	list_for_each_entry(dev, head, unreg_list) {
=======
	list_for_each_entry(dev, head, close_list) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		netdev_for_each_tx_queue(dev, dev_deactivate_queue,
					 &noop_qdisc);
		if (dev_ingress_queue(dev))
			dev_deactivate_queue(dev, dev_ingress_queue(dev),
					     &noop_qdisc);

		dev_watchdog_down(dev);
		sync_needed |= !dev->dismantle;
	}

	/* Wait for outstanding qdisc-less dev_queue_xmit calls.
	 * This is avoided if all devices are in dismantle phase :
	 * Caller will call synchronize_net() for us
	 */
	if (sync_needed)
		synchronize_net();

	/* Wait for outstanding qdisc_run calls. */
<<<<<<< HEAD
	list_for_each_entry(dev, head, unreg_list)
=======
	list_for_each_entry(dev, head, close_list)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		while (some_qdisc_is_busy(dev))
			yield();
}

void dev_deactivate(struct net_device *dev)
{
	LIST_HEAD(single);

<<<<<<< HEAD
	list_add(&dev->unreg_list, &single);
=======
	list_add(&dev->close_list, &single);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev_deactivate_many(&single);
	list_del(&single);
}
EXPORT_SYMBOL(dev_deactivate);

static void dev_init_scheduler_queue(struct net_device *dev,
				     struct netdev_queue *dev_queue,
				     void *_qdisc)
{
	struct Qdisc *qdisc = _qdisc;

<<<<<<< HEAD
	dev_queue->qdisc = qdisc;
=======
	rcu_assign_pointer(dev_queue->qdisc, qdisc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	dev_queue->qdisc_sleeping = qdisc;
}

void dev_init_scheduler(struct net_device *dev)
{
	dev->qdisc = &noop_qdisc;
	netdev_for_each_tx_queue(dev, dev_init_scheduler_queue, &noop_qdisc);
	if (dev_ingress_queue(dev))
		dev_init_scheduler_queue(dev, dev_ingress_queue(dev), &noop_qdisc);

	setup_timer(&dev->watchdog_timer, dev_watchdog, (unsigned long)dev);
}

static void shutdown_scheduler_queue(struct net_device *dev,
				     struct netdev_queue *dev_queue,
				     void *_qdisc_default)
{
	struct Qdisc *qdisc = dev_queue->qdisc_sleeping;
	struct Qdisc *qdisc_default = _qdisc_default;

	if (qdisc) {
		rcu_assign_pointer(dev_queue->qdisc, qdisc_default);
		dev_queue->qdisc_sleeping = qdisc_default;

		qdisc_destroy(qdisc);
	}
}

void dev_shutdown(struct net_device *dev)
{
	netdev_for_each_tx_queue(dev, shutdown_scheduler_queue, &noop_qdisc);
	if (dev_ingress_queue(dev))
		shutdown_scheduler_queue(dev, dev_ingress_queue(dev), &noop_qdisc);
	qdisc_destroy(dev->qdisc);
	dev->qdisc = &noop_qdisc;

	WARN_ON(timer_pending(&dev->watchdog_timer));
}

void psched_ratecfg_precompute(struct psched_ratecfg *r,
<<<<<<< HEAD
			       const struct tc_ratespec *conf)
{
	u64 factor;
	u64 mult;
	int shift;

	memset(r, 0, sizeof(*r));
	r->overhead = conf->overhead;
	r->rate_bps = (u64)conf->rate << 3;
	r->linklayer = (conf->linklayer & TC_LINKLAYER_MASK);
	r->mult = 1;
	/*
	 * Calibrate mult, shift so that token counting is accurate
	 * for smallest packet size (64 bytes).  Token (time in ns) is
	 * computed as (bytes * 8) * NSEC_PER_SEC / rate_bps.  It will
	 * work as long as the smallest packet transfer time can be
	 * accurately represented in nanosec.
	 */
	if (r->rate_bps > 0) {
		/*
		 * Higher shift gives better accuracy.  Find the largest
		 * shift such that mult fits in 32 bits.
		 */
		for (shift = 0; shift < 16; shift++) {
			r->shift = shift;
			factor = 8LLU * NSEC_PER_SEC * (1 << r->shift);
			mult = div64_u64(factor, r->rate_bps);
			if (mult > UINT_MAX)
				break;
		}

		r->shift = shift - 1;
		factor = 8LLU * NSEC_PER_SEC * (1 << r->shift);
		r->mult = div64_u64(factor, r->rate_bps);
=======
			       const struct tc_ratespec *conf,
			       u64 rate64)
{
	memset(r, 0, sizeof(*r));
	r->overhead = conf->overhead;
	r->rate_bytes_ps = max_t(u64, conf->rate, rate64);
	r->linklayer = (conf->linklayer & TC_LINKLAYER_MASK);
	r->mult = 1;
	/*
	 * The deal here is to replace a divide by a reciprocal one
	 * in fast path (a reciprocal divide is a multiply and a shift)
	 *
	 * Normal formula would be :
	 *  time_in_ns = (NSEC_PER_SEC * len) / rate_bps
	 *
	 * We compute mult/shift to use instead :
	 *  time_in_ns = (len * mult) >> shift;
	 *
	 * We try to get the highest possible mult value for accuracy,
	 * but have to make sure no overflows will ever happen.
	 */
	if (r->rate_bytes_ps > 0) {
		u64 factor = NSEC_PER_SEC;

		for (;;) {
			r->mult = div64_u64(factor, r->rate_bytes_ps);
			if (r->mult & (1U << 31) || factor & (1ULL << 63))
				break;
			factor <<= 1;
			r->shift++;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}
EXPORT_SYMBOL(psched_ratecfg_precompute);
