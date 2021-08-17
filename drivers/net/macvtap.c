#include <linux/etherdevice.h>
#include <linux/if_macvlan.h>
#include <linux/if_vlan.h>
#include <linux/interrupt.h>
#include <linux/nsproxy.h>
#include <linux/compat.h>
#include <linux/if_tun.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/cache.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/idr.h>
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/uio.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <net/net_namespace.h>
#include <net/rtnetlink.h>
#include <net/sock.h>
#include <linux/virtio_net.h>
<<<<<<< HEAD
=======
#include <linux/skb_array.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * A macvtap queue is the central object of this driver, it connects
 * an open character device to a macvlan interface. There can be
 * multiple queues on one interface, which map back to queues
 * implemented in hardware on the underlying device.
 *
 * macvtap_proto is used to allocate queues through the sock allocation
 * mechanism.
 *
<<<<<<< HEAD
 * TODO: multiqueue support is currently not implemented, even though
 * macvtap is basically prepared for that. We will need to add this
 * here as well as in virtio-net and qemu to get line rate on 10gbit
 * adapters from a guest.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
struct macvtap_queue {
	struct sock sk;
	struct socket sock;
	struct socket_wq wq;
	int vnet_hdr_sz;
	struct macvlan_dev __rcu *vlan;
	struct file *file;
	unsigned int flags;
<<<<<<< HEAD
};

=======
	u16 queue_index;
	bool enabled;
	struct list_head next;
	struct skb_array skb_array;
};

#define MACVTAP_FEATURES (IFF_VNET_HDR | IFF_MULTI_QUEUE)

#define MACVTAP_VNET_LE 0x80000000
#define MACVTAP_VNET_BE 0x40000000

#ifdef CONFIG_TUN_VNET_CROSS_LE
static inline bool macvtap_legacy_is_little_endian(struct macvtap_queue *q)
{
	return q->flags & MACVTAP_VNET_BE ? false :
		virtio_legacy_is_little_endian();
}

static long macvtap_get_vnet_be(struct macvtap_queue *q, int __user *sp)
{
	int s = !!(q->flags & MACVTAP_VNET_BE);

	if (put_user(s, sp))
		return -EFAULT;

	return 0;
}

static long macvtap_set_vnet_be(struct macvtap_queue *q, int __user *sp)
{
	int s;

	if (get_user(s, sp))
		return -EFAULT;

	if (s)
		q->flags |= MACVTAP_VNET_BE;
	else
		q->flags &= ~MACVTAP_VNET_BE;

	return 0;
}
#else
static inline bool macvtap_legacy_is_little_endian(struct macvtap_queue *q)
{
	return virtio_legacy_is_little_endian();
}

static long macvtap_get_vnet_be(struct macvtap_queue *q, int __user *argp)
{
	return -EINVAL;
}

static long macvtap_set_vnet_be(struct macvtap_queue *q, int __user *argp)
{
	return -EINVAL;
}
#endif /* CONFIG_TUN_VNET_CROSS_LE */

static inline bool macvtap_is_little_endian(struct macvtap_queue *q)
{
	return q->flags & MACVTAP_VNET_LE ||
		macvtap_legacy_is_little_endian(q);
}

static inline u16 macvtap16_to_cpu(struct macvtap_queue *q, __virtio16 val)
{
	return __virtio16_to_cpu(macvtap_is_little_endian(q), val);
}

static inline __virtio16 cpu_to_macvtap16(struct macvtap_queue *q, u16 val)
{
	return __cpu_to_virtio16(macvtap_is_little_endian(q), val);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct proto macvtap_proto = {
	.name = "macvtap",
	.owner = THIS_MODULE,
	.obj_size = sizeof (struct macvtap_queue),
};

/*
 * Variables for dealing with macvtaps device numbers.
 */
static dev_t macvtap_major;
#define MACVTAP_NUM_DEVS (1U << MINORBITS)
static DEFINE_MUTEX(minor_lock);
static DEFINE_IDR(minor_idr);

#define GOODCOPY_LEN 128
<<<<<<< HEAD
static struct class *macvtap_class;
=======
static const void *macvtap_net_namespace(struct device *d)
{
	struct net_device *dev = to_net_dev(d->parent);
	return dev_net(dev);
}

static struct class macvtap_class = {
	.name = "macvtap",
	.owner = THIS_MODULE,
	.ns_type = &net_ns_type_operations,
	.namespace = macvtap_net_namespace,
};
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static struct cdev macvtap_cdev;

static const struct proto_ops macvtap_socket_ops;

<<<<<<< HEAD
=======
#define TUN_OFFLOADS (NETIF_F_HW_CSUM | NETIF_F_TSO_ECN | NETIF_F_TSO | \
		      NETIF_F_TSO6 | NETIF_F_UFO)
#define RX_OFFLOADS (NETIF_F_GRO | NETIF_F_LRO)
#define TAP_FEATURES (NETIF_F_GSO | NETIF_F_SG | NETIF_F_FRAGLIST)

static struct macvlan_dev *macvtap_get_vlan_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler_data);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * RCU usage:
 * The macvtap_queue and the macvlan_dev are loosely coupled, the
 * pointers from one to the other can only be read while rcu_read_lock
<<<<<<< HEAD
 * or macvtap_lock is held.
=======
 * or rtnl is held.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * Both the file and the macvlan_dev hold a reference on the macvtap_queue
 * through sock_hold(&q->sk). When the macvlan_dev goes away first,
 * q->vlan becomes inaccessible. When the files gets closed,
 * macvtap_get_queue() fails.
 *
 * There may still be references to the struct sock inside of the
 * queue from outbound SKBs, but these never reference back to the
 * file or the dev. The data structure is freed through __sk_free
 * when both our references and any pending SKBs are gone.
 */
<<<<<<< HEAD
static DEFINE_SPINLOCK(macvtap_lock);

/*
 * get_slot: return a [unused/occupied] slot in vlan->taps[]:
 *	- if 'q' is NULL, return the first empty slot;
 *	- otherwise, return the slot this pointer occupies.
 */
static int get_slot(struct macvlan_dev *vlan, struct macvtap_queue *q)
{
	int i;

	for (i = 0; i < MAX_MACVTAP_QUEUES; i++) {
		if (rcu_dereference_protected(vlan->taps[i],
					      lockdep_is_held(&macvtap_lock)) == q)
			return i;
	}

	/* Should never happen */
	BUG_ON(1);
}

static int macvtap_set_queue(struct net_device *dev, struct file *file,
				struct macvtap_queue *q)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	int index;
	int err = -EBUSY;

	spin_lock(&macvtap_lock);
	if (vlan->numvtaps == MAX_MACVTAP_QUEUES)
		goto out;

	err = 0;
	index = get_slot(vlan, NULL);
	rcu_assign_pointer(q->vlan, vlan);
	rcu_assign_pointer(vlan->taps[index], q);
	sock_hold(&q->sk);

	q->file = file;
	file->private_data = q;

	vlan->numvtaps++;

out:
	spin_unlock(&macvtap_lock);
	return err;
=======

static int macvtap_enable_queue(struct net_device *dev, struct file *file,
				struct macvtap_queue *q)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	int err = -EINVAL;

	ASSERT_RTNL();

	if (q->enabled)
		goto out;

	err = 0;
	rcu_assign_pointer(vlan->taps[vlan->numvtaps], q);
	q->queue_index = vlan->numvtaps;
	q->enabled = true;

	vlan->numvtaps++;
out:
	return err;
}

/* Requires RTNL */
static int macvtap_set_queue(struct net_device *dev, struct file *file,
			     struct macvtap_queue *q)
{
	struct macvlan_dev *vlan = netdev_priv(dev);

	if (vlan->numqueues == MAX_MACVTAP_QUEUES)
		return -EBUSY;

	rcu_assign_pointer(q->vlan, vlan);
	rcu_assign_pointer(vlan->taps[vlan->numvtaps], q);
	sock_hold(&q->sk);

	q->file = file;
	q->queue_index = vlan->numvtaps;
	q->enabled = true;
	file->private_data = q;
	list_add_tail(&q->next, &vlan->queue_list);

	vlan->numvtaps++;
	vlan->numqueues++;

	return 0;
}

static int macvtap_disable_queue(struct macvtap_queue *q)
{
	struct macvlan_dev *vlan;
	struct macvtap_queue *nq;

	ASSERT_RTNL();
	if (!q->enabled)
		return -EINVAL;

	vlan = rtnl_dereference(q->vlan);

	if (vlan) {
		int index = q->queue_index;
		BUG_ON(index >= vlan->numvtaps);
		nq = rtnl_dereference(vlan->taps[vlan->numvtaps - 1]);
		nq->queue_index = index;

		rcu_assign_pointer(vlan->taps[index], nq);
		RCU_INIT_POINTER(vlan->taps[vlan->numvtaps - 1], NULL);
		q->enabled = false;

		vlan->numvtaps--;
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * The file owning the queue got closed, give up both
 * the reference that the files holds as well as the
 * one from the macvlan_dev if that still exists.
 *
 * Using the spinlock makes sure that we don't get
 * to the queue again after destroying it.
 */
static void macvtap_put_queue(struct macvtap_queue *q)
{
	struct macvlan_dev *vlan;

<<<<<<< HEAD
	spin_lock(&macvtap_lock);
	vlan = rcu_dereference_protected(q->vlan,
					 lockdep_is_held(&macvtap_lock));
	if (vlan) {
		int index = get_slot(vlan, q);

		RCU_INIT_POINTER(vlan->taps[index], NULL);
		RCU_INIT_POINTER(q->vlan, NULL);
		sock_put(&q->sk);
		--vlan->numvtaps;
	}

	spin_unlock(&macvtap_lock);
=======
	rtnl_lock();
	vlan = rtnl_dereference(q->vlan);

	if (vlan) {
		if (q->enabled)
			BUG_ON(macvtap_disable_queue(q));

		vlan->numqueues--;
		RCU_INIT_POINTER(q->vlan, NULL);
		sock_put(&q->sk);
		list_del_init(&q->next);
	}

	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	synchronize_rcu();
	sock_put(&q->sk);
}

/*
 * Select a queue based on the rxq of the device on which this packet
 * arrived. If the incoming device is not mq, calculate a flow hash
 * to select a queue. If all fails, find the first available queue.
 * Cache vlan->numvtaps since it can become zero during the execution
 * of this function.
 */
static struct macvtap_queue *macvtap_get_queue(struct net_device *dev,
					       struct sk_buff *skb)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	struct macvtap_queue *tap = NULL;
<<<<<<< HEAD
	int numvtaps = vlan->numvtaps;
=======
	/* Access to taps array is protected by rcu, but access to numvtaps
	 * isn't. Below we use it to lookup a queue, but treat it as a hint
	 * and validate that the result isn't NULL - in case we are
	 * racing against queue removal.
	 */
	int numvtaps = ACCESS_ONCE(vlan->numvtaps);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__u32 rxq;

	if (!numvtaps)
		goto out;

<<<<<<< HEAD
	/* Check if we can use flow to select a queue */
	rxq = skb_get_rxhash(skb);
	if (rxq) {
		tap = rcu_dereference(vlan->taps[rxq % numvtaps]);
		if (tap)
			goto out;
=======
	if (numvtaps == 1)
		goto single;

	/* Check if we can use flow to select a queue */
	rxq = skb_get_hash(skb);
	if (rxq) {
		tap = rcu_dereference(vlan->taps[rxq % numvtaps]);
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	if (likely(skb_rx_queue_recorded(skb))) {
		rxq = skb_get_rx_queue(skb);

		while (unlikely(rxq >= numvtaps))
			rxq -= numvtaps;

		tap = rcu_dereference(vlan->taps[rxq]);
<<<<<<< HEAD
		if (tap)
			goto out;
	}

	/* Everything failed - find first available queue */
	for (rxq = 0; rxq < MAX_MACVTAP_QUEUES; rxq++) {
		tap = rcu_dereference(vlan->taps[rxq]);
		if (tap)
			break;
	}

=======
		goto out;
	}

single:
	tap = rcu_dereference(vlan->taps[0]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out:
	return tap;
}

/*
 * The net_device is going away, give up the reference
 * that it holds on all queues and safely set the pointer
 * from the queues to NULL.
 */
static void macvtap_del_queues(struct net_device *dev)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
<<<<<<< HEAD
	struct macvtap_queue *q, *qlist[MAX_MACVTAP_QUEUES];
	int i, j = 0;

	/* macvtap_put_queue can free some slots, so go through all slots */
	spin_lock(&macvtap_lock);
	for (i = 0; i < MAX_MACVTAP_QUEUES && vlan->numvtaps; i++) {
		q = rcu_dereference_protected(vlan->taps[i],
					      lockdep_is_held(&macvtap_lock));
		if (q) {
			qlist[j++] = q;
			RCU_INIT_POINTER(vlan->taps[i], NULL);
			RCU_INIT_POINTER(q->vlan, NULL);
			vlan->numvtaps--;
		}
	}
	BUG_ON(vlan->numvtaps != 0);
	/* guarantee that any future macvtap_set_queue will fail */
	vlan->numvtaps = MAX_MACVTAP_QUEUES;
	spin_unlock(&macvtap_lock);

	synchronize_rcu();

	for (--j; j >= 0; j--)
		sock_put(&qlist[j]->sk);
}

/*
 * Forward happens for data that gets sent from one macvlan
 * endpoint to another one in bridge mode. We just take
 * the skb and put it into the receive queue.
 */
static int macvtap_forward(struct net_device *dev, struct sk_buff *skb)
{
	struct macvtap_queue *q = macvtap_get_queue(dev, skb);
	if (!q)
		goto drop;

	if (skb_queue_len(&q->sk.sk_receive_queue) >= dev->tx_queue_len)
		goto drop;

	skb_queue_tail(&q->sk.sk_receive_queue, skb);
	wake_up_interruptible_poll(sk_sleep(&q->sk), POLLIN | POLLRDNORM | POLLRDBAND);
	return NET_RX_SUCCESS;

drop:
	kfree_skb(skb);
	return NET_RX_DROP;
}

/*
 * Receive is for data from the external interface (lowerdev),
 * in case of macvtap, we can treat that the same way as
 * forward, which macvlan cannot.
 */
static int macvtap_receive(struct sk_buff *skb)
{
	skb_push(skb, ETH_HLEN);
	return macvtap_forward(skb->dev, skb);
=======
	struct macvtap_queue *q, *tmp;

	ASSERT_RTNL();
	list_for_each_entry_safe(q, tmp, &vlan->queue_list, next) {
		list_del_init(&q->next);
		RCU_INIT_POINTER(q->vlan, NULL);
		if (q->enabled)
			vlan->numvtaps--;
		vlan->numqueues--;
		sock_put(&q->sk);
	}
	BUG_ON(vlan->numvtaps);
	BUG_ON(vlan->numqueues);
	/* guarantee that any future macvtap_set_queue will fail */
	vlan->numvtaps = MAX_MACVTAP_QUEUES;
}

static rx_handler_result_t macvtap_handle_frame(struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;
	struct net_device *dev = skb->dev;
	struct macvlan_dev *vlan;
	struct macvtap_queue *q;
	netdev_features_t features = TAP_FEATURES;

	vlan = macvtap_get_vlan_rcu(dev);
	if (!vlan)
		return RX_HANDLER_PASS;

	q = macvtap_get_queue(dev, skb);
	if (!q)
		return RX_HANDLER_PASS;

	if (__skb_array_full(&q->skb_array))
		goto drop;

	skb_push(skb, ETH_HLEN);

	/* Apply the forward feature mask so that we perform segmentation
	 * according to users wishes.  This only works if VNET_HDR is
	 * enabled.
	 */
	if (q->flags & IFF_VNET_HDR)
		features |= vlan->tap_features;
	if (netif_needs_gso(skb, features)) {
		struct sk_buff *segs = __skb_gso_segment(skb, features, false);

		if (IS_ERR(segs))
			goto drop;

		if (!segs) {
			if (skb_array_produce(&q->skb_array, skb))
				goto drop;
			goto wake_up;
		}

		consume_skb(skb);
		while (segs) {
			struct sk_buff *nskb = segs->next;

			segs->next = NULL;
			if (skb_array_produce(&q->skb_array, segs)) {
				kfree_skb(segs);
				kfree_skb_list(nskb);
				break;
			}
			segs = nskb;
		}
	} else {
		/* If we receive a partial checksum and the tap side
		 * doesn't support checksum offload, compute the checksum.
		 * Note: it doesn't matter which checksum feature to
		 *        check, we either support them all or none.
		 */
		if (skb->ip_summed == CHECKSUM_PARTIAL &&
		    !(features & NETIF_F_CSUM_MASK) &&
		    skb_checksum_help(skb))
			goto drop;
		if (skb_array_produce(&q->skb_array, skb))
			goto drop;
	}

wake_up:
	wake_up_interruptible_poll(sk_sleep(&q->sk), POLLIN | POLLRDNORM | POLLRDBAND);
	return RX_HANDLER_CONSUMED;

drop:
	/* Count errors/drops only here, thus don't care about args. */
	macvlan_count_rx(vlan, 0, 0, 0);
	kfree_skb(skb);
	return RX_HANDLER_CONSUMED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int macvtap_get_minor(struct macvlan_dev *vlan)
{
	int retval = -ENOMEM;

	mutex_lock(&minor_lock);
	retval = idr_alloc(&minor_idr, vlan, 1, MACVTAP_NUM_DEVS, GFP_KERNEL);
	if (retval >= 0) {
		vlan->minor = retval;
	} else if (retval == -ENOSPC) {
		printk(KERN_ERR "too many macvtap devices\n");
		retval = -EINVAL;
	}
	mutex_unlock(&minor_lock);
	return retval < 0 ? retval : 0;
}

static void macvtap_free_minor(struct macvlan_dev *vlan)
{
	mutex_lock(&minor_lock);
	if (vlan->minor) {
		idr_remove(&minor_idr, vlan->minor);
		vlan->minor = 0;
	}
	mutex_unlock(&minor_lock);
}

static struct net_device *dev_get_by_macvtap_minor(int minor)
{
	struct net_device *dev = NULL;
	struct macvlan_dev *vlan;

	mutex_lock(&minor_lock);
	vlan = idr_find(&minor_idr, minor);
	if (vlan) {
		dev = vlan->dev;
		dev_hold(dev);
	}
	mutex_unlock(&minor_lock);
	return dev;
}

static int macvtap_newlink(struct net *src_net,
			   struct net_device *dev,
			   struct nlattr *tb[],
			   struct nlattr *data[])
{
<<<<<<< HEAD
	/* Don't put anything that may fail after macvlan_common_newlink
	 * because we can't undo what it does.
	 */
	return macvlan_common_newlink(src_net, dev, tb, data,
				      macvtap_receive, macvtap_forward);
=======
	struct macvlan_dev *vlan = netdev_priv(dev);
	int err;

	INIT_LIST_HEAD(&vlan->queue_list);

	/* Since macvlan supports all offloads by default, make
	 * tap support all offloads also.
	 */
	vlan->tap_features = TUN_OFFLOADS;

	err = netdev_rx_handler_register(dev, macvtap_handle_frame, vlan);
	if (err)
		return err;

	/* Don't put anything that may fail after macvlan_common_newlink
	 * because we can't undo what it does.
	 */
	err = macvlan_common_newlink(src_net, dev, tb, data);
	if (err) {
		netdev_rx_handler_unregister(dev);
		return err;
	}

	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void macvtap_dellink(struct net_device *dev,
			    struct list_head *head)
{
<<<<<<< HEAD
=======
	netdev_rx_handler_unregister(dev);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	macvtap_del_queues(dev);
	macvlan_dellink(dev, head);
}

static void macvtap_setup(struct net_device *dev)
{
	macvlan_common_setup(dev);
	dev->tx_queue_len = TUN_READQ_SIZE;
}

static struct rtnl_link_ops macvtap_link_ops __read_mostly = {
	.kind		= "macvtap",
	.setup		= macvtap_setup,
	.newlink	= macvtap_newlink,
	.dellink	= macvtap_dellink,
};


static void macvtap_sock_write_space(struct sock *sk)
{
	wait_queue_head_t *wqueue;

	if (!sock_writeable(sk) ||
<<<<<<< HEAD
	    !test_and_clear_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags))
=======
	    !test_and_clear_bit(SOCKWQ_ASYNC_NOSPACE, &sk->sk_socket->flags))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return;

	wqueue = sk_sleep(sk);
	if (wqueue && waitqueue_active(wqueue))
		wake_up_interruptible_poll(wqueue, POLLOUT | POLLWRNORM | POLLWRBAND);
}

static void macvtap_sock_destruct(struct sock *sk)
{
<<<<<<< HEAD
	skb_queue_purge(&sk->sk_receive_queue);
=======
	struct macvtap_queue *q = container_of(sk, struct macvtap_queue, sk);

	skb_array_cleanup(&q->skb_array);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int macvtap_open(struct inode *inode, struct file *file)
{
	struct net *net = current->nsproxy->net_ns;
<<<<<<< HEAD
	struct net_device *dev = dev_get_by_macvtap_minor(iminor(inode));
	struct macvtap_queue *q;
	int err;

	err = -ENODEV;
	if (!dev)
		goto out;

	err = -ENOMEM;
	q = (struct macvtap_queue *)sk_alloc(net, AF_UNSPEC, GFP_KERNEL,
					     &macvtap_proto);
	if (!q)
		goto out;

	q->sock.wq = &q->wq;
=======
	struct net_device *dev;
	struct macvtap_queue *q;
	int err = -ENODEV;

	rtnl_lock();
	dev = dev_get_by_macvtap_minor(iminor(inode));
	if (!dev)
		goto err;

	err = -ENOMEM;
	q = (struct macvtap_queue *)sk_alloc(net, AF_UNSPEC, GFP_KERNEL,
					     &macvtap_proto, 0);
	if (!q)
		goto err;
	if (skb_array_init(&q->skb_array, dev->tx_queue_len, GFP_KERNEL)) {
		sk_free(&q->sk);
		goto err;
	}

	RCU_INIT_POINTER(q->sock.wq, &q->wq);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	init_waitqueue_head(&q->wq.wait);
	q->sock.type = SOCK_RAW;
	q->sock.state = SS_CONNECTED;
	q->sock.file = file;
	q->sock.ops = &macvtap_socket_ops;
	sock_init_data(&q->sock, &q->sk);
	q->sk.sk_write_space = macvtap_sock_write_space;
	q->sk.sk_destruct = macvtap_sock_destruct;
	q->flags = IFF_VNET_HDR | IFF_NO_PI | IFF_TAP;
	q->vnet_hdr_sz = sizeof(struct virtio_net_hdr);

	/*
	 * so far only KVM virtio_net uses macvtap, enable zero copy between
	 * guest kernel and host kernel when lower device supports zerocopy
	 *
	 * The macvlan supports zerocopy iff the lower device supports zero
	 * copy so we don't have to look at the lower device directly.
	 */
	if ((dev->features & NETIF_F_HIGHDMA) && (dev->features & NETIF_F_SG))
		sock_set_flag(&q->sk, SOCK_ZEROCOPY);

	err = macvtap_set_queue(dev, file, q);
<<<<<<< HEAD
	if (err)
		sock_put(&q->sk);

out:
	if (dev)
		dev_put(dev);

=======
	if (err) {
		/* macvtap_sock_destruct() will take care of freeing skb_array */
		goto err_put;
	}

	dev_put(dev);

	rtnl_unlock();
	return err;

err_put:
	sock_put(&q->sk);
err:
	if (dev)
		dev_put(dev);

	rtnl_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return err;
}

static int macvtap_release(struct inode *inode, struct file *file)
{
	struct macvtap_queue *q = file->private_data;
	macvtap_put_queue(q);
	return 0;
}

static unsigned int macvtap_poll(struct file *file, poll_table * wait)
{
	struct macvtap_queue *q = file->private_data;
	unsigned int mask = POLLERR;

	if (!q)
		goto out;

	mask = 0;
	poll_wait(file, &q->wq.wait, wait);

<<<<<<< HEAD
	if (!skb_queue_empty(&q->sk.sk_receive_queue))
		mask |= POLLIN | POLLRDNORM;

	if (sock_writeable(&q->sk) ||
	    (!test_and_set_bit(SOCK_ASYNC_NOSPACE, &q->sock.flags) &&
=======
	if (!skb_array_empty(&q->skb_array))
		mask |= POLLIN | POLLRDNORM;

	if (sock_writeable(&q->sk) ||
	    (!test_and_set_bit(SOCKWQ_ASYNC_NOSPACE, &q->sock.flags) &&
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	     sock_writeable(&q->sk)))
		mask |= POLLOUT | POLLWRNORM;

out:
	return mask;
}

static inline struct sk_buff *macvtap_alloc_skb(struct sock *sk, size_t prepad,
						size_t len, size_t linear,
						int noblock, int *err)
{
	struct sk_buff *skb;

	/* Under a page?  Don't bother with paged skb. */
	if (prepad + len < PAGE_SIZE || !linear)
		linear = len;

	skb = sock_alloc_send_pskb(sk, prepad + linear, len - linear, noblock,
<<<<<<< HEAD
				   err);
=======
				   err, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (!skb)
		return NULL;

	skb_reserve(skb, prepad);
	skb_put(skb, linear);
	skb->data_len = len - linear;
	skb->len += len - linear;

	return skb;
}

<<<<<<< HEAD
/* set skb frags from iovec, this can move to core network code for reuse */
static int zerocopy_sg_from_iovec(struct sk_buff *skb, const struct iovec *from,
				  int offset, size_t count)
{
	int len = iov_length(from, count) - offset;
	int copy = skb_headlen(skb);
	int size, offset1 = 0;
	int i = 0;

	/* Skip over from offset */
	while (count && (offset >= from->iov_len)) {
		offset -= from->iov_len;
		++from;
		--count;
	}

	/* copy up to skb headlen */
	while (count && (copy > 0)) {
		size = min_t(unsigned int, copy, from->iov_len - offset);
		if (copy_from_user(skb->data + offset1, from->iov_base + offset,
				   size))
			return -EFAULT;
		if (copy > size) {
			++from;
			--count;
			offset = 0;
		} else
			offset += size;
		copy -= size;
		offset1 += size;
	}

	if (len == offset1)
		return 0;

	while (count--) {
		struct page *page[MAX_SKB_FRAGS];
		int num_pages;
		unsigned long base;
		unsigned long truesize;

		len = from->iov_len - offset;
		if (!len) {
			offset = 0;
			++from;
			continue;
		}
		base = (unsigned long)from->iov_base + offset;
		size = ((base & ~PAGE_MASK) + len + ~PAGE_MASK) >> PAGE_SHIFT;
		if (i + size > MAX_SKB_FRAGS)
			return -EMSGSIZE;
		num_pages = get_user_pages_fast(base, size, 0, &page[i]);
		if (num_pages != size) {
			int j;

			for (j = 0; j < num_pages; j++)
				put_page(page[i + j]);
			return -EFAULT;
		}
		truesize = size * PAGE_SIZE;
		skb->data_len += len;
		skb->len += len;
		skb->truesize += truesize;
		atomic_add(truesize, &skb->sk->sk_wmem_alloc);
		while (len) {
			int off = base & ~PAGE_MASK;
			int size = min_t(int, len, PAGE_SIZE - off);
			__skb_fill_page_desc(skb, i, page[i], off, size);
			skb_shinfo(skb)->nr_frags++;
			/* increase sk_wmem_alloc */
			base += size;
			len -= size;
			i++;
		}
		offset = 0;
		++from;
	}
	return 0;
}

/*
 * macvtap_skb_from_vnet_hdr and macvtap_skb_to_vnet_hdr should
 * be shared with the tun/tap driver.
 */
static int macvtap_skb_from_vnet_hdr(struct sk_buff *skb,
				     struct virtio_net_hdr *vnet_hdr)
{
	unsigned short gso_type = 0;
	if (vnet_hdr->gso_type != VIRTIO_NET_HDR_GSO_NONE) {
		switch (vnet_hdr->gso_type & ~VIRTIO_NET_HDR_GSO_ECN) {
		case VIRTIO_NET_HDR_GSO_TCPV4:
			gso_type = SKB_GSO_TCPV4;
			break;
		case VIRTIO_NET_HDR_GSO_TCPV6:
			gso_type = SKB_GSO_TCPV6;
			break;
		case VIRTIO_NET_HDR_GSO_UDP:
			gso_type = SKB_GSO_UDP;
			break;
		default:
			return -EINVAL;
		}

		if (vnet_hdr->gso_type & VIRTIO_NET_HDR_GSO_ECN)
			gso_type |= SKB_GSO_TCP_ECN;

		if (vnet_hdr->gso_size == 0)
			return -EINVAL;
	}

	if (vnet_hdr->flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) {
		if (!skb_partial_csum_set(skb, vnet_hdr->csum_start,
					  vnet_hdr->csum_offset))
			return -EINVAL;
	}

	if (vnet_hdr->gso_type != VIRTIO_NET_HDR_GSO_NONE) {
		skb_shinfo(skb)->gso_size = vnet_hdr->gso_size;
		skb_shinfo(skb)->gso_type = gso_type;

		/* Header must be checked, and gso_segs computed. */
		skb_shinfo(skb)->gso_type |= SKB_GSO_DODGY;
		skb_shinfo(skb)->gso_segs = 0;
	}
	return 0;
}

static int macvtap_skb_to_vnet_hdr(const struct sk_buff *skb,
				   struct virtio_net_hdr *vnet_hdr)
{
	memset(vnet_hdr, 0, sizeof(*vnet_hdr));

	if (skb_is_gso(skb)) {
		struct skb_shared_info *sinfo = skb_shinfo(skb);

		/* This is a hint as to how much should be linear. */
		vnet_hdr->hdr_len = skb_headlen(skb);
		vnet_hdr->gso_size = sinfo->gso_size;
		if (sinfo->gso_type & SKB_GSO_TCPV4)
			vnet_hdr->gso_type = VIRTIO_NET_HDR_GSO_TCPV4;
		else if (sinfo->gso_type & SKB_GSO_TCPV6)
			vnet_hdr->gso_type = VIRTIO_NET_HDR_GSO_TCPV6;
		else if (sinfo->gso_type & SKB_GSO_UDP)
			vnet_hdr->gso_type = VIRTIO_NET_HDR_GSO_UDP;
		else
			BUG();
		if (sinfo->gso_type & SKB_GSO_TCP_ECN)
			vnet_hdr->gso_type |= VIRTIO_NET_HDR_GSO_ECN;
	} else
		vnet_hdr->gso_type = VIRTIO_NET_HDR_GSO_NONE;

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		vnet_hdr->flags = VIRTIO_NET_HDR_F_NEEDS_CSUM;
		vnet_hdr->csum_start = skb_checksum_start_offset(skb);
		if (vlan_tx_tag_present(skb))
			vnet_hdr->csum_start += VLAN_HLEN;
		vnet_hdr->csum_offset = skb->csum_offset;
	} else if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
		vnet_hdr->flags = VIRTIO_NET_HDR_F_DATA_VALID;
	} /* else everything is zero */

	return 0;
}

static unsigned long iov_pages(const struct iovec *iv, int offset,
			       unsigned long nr_segs)
{
	unsigned long seg, base;
	int pages = 0, len, size;

	while (nr_segs && (offset >= iv->iov_len)) {
		offset -= iv->iov_len;
		++iv;
		--nr_segs;
	}

	for (seg = 0; seg < nr_segs; seg++) {
		base = (unsigned long)iv[seg].iov_base + offset;
		len = iv[seg].iov_len - offset;
		size = ((base & ~PAGE_MASK) + len + ~PAGE_MASK) >> PAGE_SHIFT;
		pages += size;
		offset = 0;
	}

	return pages;
}

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Neighbour code has some assumptions on HH_DATA_MOD alignment */
#define MACVTAP_RESERVE HH_DATA_OFF(ETH_HLEN)

/* Get packet from user space buffer */
static ssize_t macvtap_get_user(struct macvtap_queue *q, struct msghdr *m,
<<<<<<< HEAD
				const struct iovec *iv, unsigned long total_len,
				size_t count, int noblock)
=======
				struct iov_iter *from, int noblock)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int good_linear = SKB_MAX_HEAD(MACVTAP_RESERVE);
	struct sk_buff *skb;
	struct macvlan_dev *vlan;
<<<<<<< HEAD
=======
	unsigned long total_len = iov_iter_count(from);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long len = total_len;
	int err;
	struct virtio_net_hdr vnet_hdr = { 0 };
	int vnet_hdr_len = 0;
	int copylen = 0;
<<<<<<< HEAD
	bool zerocopy = false;
	size_t linear;

	if (q->flags & IFF_VNET_HDR) {
		vnet_hdr_len = ACCESS_ONCE(q->vnet_hdr_sz);
=======
	int depth;
	bool zerocopy = false;
	size_t linear;
	ssize_t n;

	if (q->flags & IFF_VNET_HDR) {
		vnet_hdr_len = READ_ONCE(q->vnet_hdr_sz);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		err = -EINVAL;
		if (len < vnet_hdr_len)
			goto err;
		len -= vnet_hdr_len;

<<<<<<< HEAD
		err = memcpy_fromiovecend((void *)&vnet_hdr, iv, 0,
					   sizeof(vnet_hdr));
		if (err < 0)
			goto err;
		if ((vnet_hdr.flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&
		     vnet_hdr.csum_start + vnet_hdr.csum_offset + 2 >
							vnet_hdr.hdr_len)
			vnet_hdr.hdr_len = vnet_hdr.csum_start +
						vnet_hdr.csum_offset + 2;
		err = -EINVAL;
		if (vnet_hdr.hdr_len > len)
=======
		err = -EFAULT;
		n = copy_from_iter(&vnet_hdr, sizeof(vnet_hdr), from);
		if (n != sizeof(vnet_hdr))
			goto err;
		iov_iter_advance(from, vnet_hdr_len - sizeof(vnet_hdr));
		if ((vnet_hdr.flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&
		     macvtap16_to_cpu(q, vnet_hdr.csum_start) +
		     macvtap16_to_cpu(q, vnet_hdr.csum_offset) + 2 >
			     macvtap16_to_cpu(q, vnet_hdr.hdr_len))
			vnet_hdr.hdr_len = cpu_to_macvtap16(q,
				 macvtap16_to_cpu(q, vnet_hdr.csum_start) +
				 macvtap16_to_cpu(q, vnet_hdr.csum_offset) + 2);
		err = -EINVAL;
		if (macvtap16_to_cpu(q, vnet_hdr.hdr_len) > len)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			goto err;
	}

	err = -EINVAL;
	if (unlikely(len < ETH_HLEN))
		goto err;

<<<<<<< HEAD
	err = -EMSGSIZE;
	if (unlikely(count > UIO_MAXIOV))
		goto err;

	if (m && m->msg_control && sock_flag(&q->sk, SOCK_ZEROCOPY)) {
		copylen = vnet_hdr.hdr_len ? vnet_hdr.hdr_len : GOODCOPY_LEN;
		if (copylen > good_linear)
			copylen = good_linear;
		linear = copylen;
		if (iov_pages(iv, vnet_hdr_len + copylen, count)
		    <= MAX_SKB_FRAGS)
=======
	if (m && m->msg_control && sock_flag(&q->sk, SOCK_ZEROCOPY)) {
		struct iov_iter i;

		copylen = vnet_hdr.hdr_len ?
			macvtap16_to_cpu(q, vnet_hdr.hdr_len) : GOODCOPY_LEN;
		if (copylen > good_linear)
			copylen = good_linear;
		else if (copylen < ETH_HLEN)
			copylen = ETH_HLEN;
		linear = copylen;
		i = *from;
		iov_iter_advance(&i, copylen);
		if (iov_iter_npages(&i, INT_MAX) <= MAX_SKB_FRAGS)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			zerocopy = true;
	}

	if (!zerocopy) {
		copylen = len;
<<<<<<< HEAD
		if (vnet_hdr.hdr_len > good_linear)
			linear = good_linear;
		else
			linear = vnet_hdr.hdr_len;
=======
		linear = macvtap16_to_cpu(q, vnet_hdr.hdr_len);
		if (linear > good_linear)
			linear = good_linear;
		else if (linear < ETH_HLEN)
			linear = ETH_HLEN;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	skb = macvtap_alloc_skb(&q->sk, MACVTAP_RESERVE, copylen,
				linear, noblock, &err);
	if (!skb)
		goto err;

	if (zerocopy)
<<<<<<< HEAD
		err = zerocopy_sg_from_iovec(skb, iv, vnet_hdr_len, count);
	else {
		err = skb_copy_datagram_from_iovec(skb, 0, iv, vnet_hdr_len,
						   len);
		if (!err && m && m->msg_control) {
			struct ubuf_info *uarg = m->msg_control;
			uarg->callback(uarg, false);
		}
	}
=======
		err = zerocopy_sg_from_iter(skb, from);
	else
		err = skb_copy_datagram_from_iter(skb, 0, from, len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (err)
		goto err_kfree;

	skb_set_network_header(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	skb->protocol = eth_hdr(skb)->h_proto;

	if (vnet_hdr_len) {
<<<<<<< HEAD
		err = macvtap_skb_from_vnet_hdr(skb, &vnet_hdr);
=======
		err = virtio_net_hdr_to_skb(skb, &vnet_hdr,
					    macvtap_is_little_endian(q));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (err)
			goto err_kfree;
	}

	skb_probe_transport_header(skb, ETH_HLEN);

<<<<<<< HEAD
	rcu_read_lock_bh();
	vlan = rcu_dereference_bh(q->vlan);
=======
	/* Move network header to the right position for VLAN tagged packets */
	if ((skb->protocol == htons(ETH_P_8021Q) ||
	     skb->protocol == htons(ETH_P_8021AD)) &&
	    __vlan_get_protocol(skb, skb->protocol, &depth) != 0)
		skb_set_network_header(skb, depth);

	rcu_read_lock();
	vlan = rcu_dereference(q->vlan);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	/* copy skb_ubuf_info for callback when skb has no error */
	if (zerocopy) {
		skb_shinfo(skb)->destructor_arg = m->msg_control;
		skb_shinfo(skb)->tx_flags |= SKBTX_DEV_ZEROCOPY;
		skb_shinfo(skb)->tx_flags |= SKBTX_SHARED_FRAG;
<<<<<<< HEAD
	}
	if (vlan)
		macvlan_start_xmit(skb, vlan->dev);
	else
		kfree_skb(skb);
	rcu_read_unlock_bh();
=======
	} else if (m && m->msg_control) {
		struct ubuf_info *uarg = m->msg_control;
		uarg->callback(uarg, false);
	}

	if (vlan) {
		skb->dev = vlan->dev;
		dev_queue_xmit(skb);
	} else {
		kfree_skb(skb);
	}
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return total_len;

err_kfree:
	kfree_skb(skb);

err:
<<<<<<< HEAD
	rcu_read_lock_bh();
	vlan = rcu_dereference_bh(q->vlan);
	if (vlan)
		vlan->dev->stats.tx_dropped++;
	rcu_read_unlock_bh();
=======
	rcu_read_lock();
	vlan = rcu_dereference(q->vlan);
	if (vlan)
		this_cpu_inc(vlan->pcpu_stats->tx_dropped);
	rcu_read_unlock();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	return err;
}

<<<<<<< HEAD
static ssize_t macvtap_aio_write(struct kiocb *iocb, const struct iovec *iv,
				 unsigned long count, loff_t pos)
{
	struct file *file = iocb->ki_filp;
	ssize_t result = -ENOLINK;
	struct macvtap_queue *q = file->private_data;

	result = macvtap_get_user(q, NULL, iv, iov_length(iv, count), count,
				  file->f_flags & O_NONBLOCK);
	return result;
=======
static ssize_t macvtap_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct macvtap_queue *q = file->private_data;

	return macvtap_get_user(q, NULL, from, file->f_flags & O_NONBLOCK);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Put packet to the user space buffer */
static ssize_t macvtap_put_user(struct macvtap_queue *q,
				const struct sk_buff *skb,
<<<<<<< HEAD
				const struct iovec *iv, int len)
=======
				struct iov_iter *iter)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	int ret;
	int vnet_hdr_len = 0;
	int vlan_offset = 0;
<<<<<<< HEAD
	int copied, total;

	if (q->flags & IFF_VNET_HDR) {
		struct virtio_net_hdr vnet_hdr;
		vnet_hdr_len = ACCESS_ONCE(q->vnet_hdr_sz);
		if ((len -= vnet_hdr_len) < 0)
			return -EINVAL;

		ret = macvtap_skb_to_vnet_hdr(skb, &vnet_hdr);
		if (ret)
			return ret;

		if (memcpy_toiovecend(iv, (void *)&vnet_hdr, 0, sizeof(vnet_hdr)))
			return -EFAULT;
	}
	total = copied = vnet_hdr_len;
	total += skb->len;

	if (!vlan_tx_tag_present(skb))
		len = min_t(int, skb->len, len);
	else {
		int copy;
=======
	int total;

	if (q->flags & IFF_VNET_HDR) {
		struct virtio_net_hdr vnet_hdr;
		vnet_hdr_len = READ_ONCE(q->vnet_hdr_sz);
		if (iov_iter_count(iter) < vnet_hdr_len)
			return -EINVAL;

		ret = virtio_net_hdr_from_skb(skb, &vnet_hdr,
					      macvtap_is_little_endian(q), true);
		if (ret)
			BUG();

		if (copy_to_iter(&vnet_hdr, sizeof(vnet_hdr), iter) !=
		    sizeof(vnet_hdr))
			return -EFAULT;

		iov_iter_advance(iter, vnet_hdr_len - sizeof(vnet_hdr));
	}
	total = vnet_hdr_len;
	total += skb->len;

	if (skb_vlan_tag_present(skb)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		struct {
			__be16 h_vlan_proto;
			__be16 h_vlan_TCI;
		} veth;
<<<<<<< HEAD
		veth.h_vlan_proto = htons(ETH_P_8021Q);
		veth.h_vlan_TCI = htons(vlan_tx_tag_get(skb));

		vlan_offset = offsetof(struct vlan_ethhdr, h_vlan_proto);
		len = min_t(int, skb->len + VLAN_HLEN, len);
		total += VLAN_HLEN;

		copy = min_t(int, vlan_offset, len);
		ret = skb_copy_datagram_const_iovec(skb, 0, iv, copied, copy);
		len -= copy;
		copied += copy;
		if (ret || !len)
			goto done;

		copy = min_t(int, sizeof(veth), len);
		ret = memcpy_toiovecend(iv, (void *)&veth, copied, copy);
		len -= copy;
		copied += copy;
		if (ret || !len)
			goto done;
	}

	ret = skb_copy_datagram_const_iovec(skb, vlan_offset, iv, copied, len);
=======
		veth.h_vlan_proto = skb->vlan_proto;
		veth.h_vlan_TCI = htons(skb_vlan_tag_get(skb));

		vlan_offset = offsetof(struct vlan_ethhdr, h_vlan_proto);
		total += VLAN_HLEN;

		ret = skb_copy_datagram_iter(skb, 0, iter, vlan_offset);
		if (ret || !iov_iter_count(iter))
			goto done;

		ret = copy_to_iter(&veth, sizeof(veth), iter);
		if (ret != sizeof(veth) || !iov_iter_count(iter))
			goto done;
	}

	ret = skb_copy_datagram_iter(skb, vlan_offset, iter,
				     skb->len - vlan_offset);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

done:
	return ret ? ret : total;
}

<<<<<<< HEAD
static ssize_t macvtap_do_read(struct macvtap_queue *q, struct kiocb *iocb,
			       const struct iovec *iv, unsigned long len,
=======
static ssize_t macvtap_do_read(struct macvtap_queue *q,
			       struct iov_iter *to,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			       int noblock)
{
	DEFINE_WAIT(wait);
	struct sk_buff *skb;
	ssize_t ret = 0;

<<<<<<< HEAD
	while (len) {
		prepare_to_wait(sk_sleep(&q->sk), &wait, TASK_INTERRUPTIBLE);

		/* Read frames from the queue */
		skb = skb_dequeue(&q->sk.sk_receive_queue);
		if (!skb) {
			if (noblock) {
				ret = -EAGAIN;
				break;
			}
			if (signal_pending(current)) {
				ret = -ERESTARTSYS;
				break;
			}
			/* Nothing to read, let's sleep */
			schedule();
			continue;
		}
		ret = macvtap_put_user(q, skb, iv, len);
		kfree_skb(skb);
		break;
	}

	finish_wait(sk_sleep(&q->sk), &wait);
	return ret;
}

static ssize_t macvtap_aio_read(struct kiocb *iocb, const struct iovec *iv,
				unsigned long count, loff_t pos)
{
	struct file *file = iocb->ki_filp;
	struct macvtap_queue *q = file->private_data;
	ssize_t len, ret = 0;

	len = iov_length(iv, count);
	if (len < 0) {
		ret = -EINVAL;
		goto out;
	}

	ret = macvtap_do_read(q, iocb, iv, len, file->f_flags & O_NONBLOCK);
	ret = min_t(ssize_t, ret, len);
	if (ret > 0)
		iocb->ki_pos = ret;
out:
	return ret;
}

=======
	if (!iov_iter_count(to))
		return 0;

	while (1) {
		if (!noblock)
			prepare_to_wait(sk_sleep(&q->sk), &wait,
					TASK_INTERRUPTIBLE);

		/* Read frames from the queue */
		skb = skb_array_consume(&q->skb_array);
		if (skb)
			break;
		if (noblock) {
			ret = -EAGAIN;
			break;
		}
		if (signal_pending(current)) {
			ret = -ERESTARTSYS;
			break;
		}
		/* Nothing to read, let's sleep */
		schedule();
	}
	if (!noblock)
		finish_wait(sk_sleep(&q->sk), &wait);

	if (skb) {
		ret = macvtap_put_user(q, skb, to);
		if (unlikely(ret < 0))
			kfree_skb(skb);
		else
			consume_skb(skb);
	}
	return ret;
}

static ssize_t macvtap_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *file = iocb->ki_filp;
	struct macvtap_queue *q = file->private_data;
	ssize_t len = iov_iter_count(to), ret;

	ret = macvtap_do_read(q, to, file->f_flags & O_NONBLOCK);
	ret = min_t(ssize_t, ret, len);
	if (ret > 0)
		iocb->ki_pos = ret;
	return ret;
}

static struct macvlan_dev *macvtap_get_vlan(struct macvtap_queue *q)
{
	struct macvlan_dev *vlan;

	ASSERT_RTNL();
	vlan = rtnl_dereference(q->vlan);
	if (vlan)
		dev_hold(vlan->dev);

	return vlan;
}

static void macvtap_put_vlan(struct macvlan_dev *vlan)
{
	dev_put(vlan->dev);
}

static int macvtap_ioctl_set_queue(struct file *file, unsigned int flags)
{
	struct macvtap_queue *q = file->private_data;
	struct macvlan_dev *vlan;
	int ret;

	vlan = macvtap_get_vlan(q);
	if (!vlan)
		return -EINVAL;

	if (flags & IFF_ATTACH_QUEUE)
		ret = macvtap_enable_queue(vlan->dev, file, q);
	else if (flags & IFF_DETACH_QUEUE)
		ret = macvtap_disable_queue(q);
	else
		ret = -EINVAL;

	macvtap_put_vlan(vlan);
	return ret;
}

static int set_offload(struct macvtap_queue *q, unsigned long arg)
{
	struct macvlan_dev *vlan;
	netdev_features_t features;
	netdev_features_t feature_mask = 0;

	vlan = rtnl_dereference(q->vlan);
	if (!vlan)
		return -ENOLINK;

	features = vlan->dev->features;

	if (arg & TUN_F_CSUM) {
		feature_mask = NETIF_F_HW_CSUM;

		if (arg & (TUN_F_TSO4 | TUN_F_TSO6)) {
			if (arg & TUN_F_TSO_ECN)
				feature_mask |= NETIF_F_TSO_ECN;
			if (arg & TUN_F_TSO4)
				feature_mask |= NETIF_F_TSO;
			if (arg & TUN_F_TSO6)
				feature_mask |= NETIF_F_TSO6;
		}

		if (arg & TUN_F_UFO)
			feature_mask |= NETIF_F_UFO;
	}

	/* tun/tap driver inverts the usage for TSO offloads, where
	 * setting the TSO bit means that the userspace wants to
	 * accept TSO frames and turning it off means that user space
	 * does not support TSO.
	 * For macvtap, we have to invert it to mean the same thing.
	 * When user space turns off TSO, we turn off GSO/LRO so that
	 * user-space will not receive TSO frames.
	 */
	if (feature_mask & (NETIF_F_TSO | NETIF_F_TSO6 | NETIF_F_UFO))
		features |= RX_OFFLOADS;
	else
		features &= ~RX_OFFLOADS;

	/* tap_features are the same as features on tun/tap and
	 * reflect user expectations.
	 */
	vlan->tap_features = feature_mask;
	vlan->set_features = features;
	netdev_update_features(vlan->dev);

	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * provide compatibility with generic tun/tap interface
 */
static long macvtap_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct macvtap_queue *q = file->private_data;
	struct macvlan_dev *vlan;
	void __user *argp = (void __user *)arg;
	struct ifreq __user *ifr = argp;
	unsigned int __user *up = argp;
<<<<<<< HEAD
	unsigned int u;
	int __user *sp = argp;
=======
	unsigned short u;
	int __user *sp = argp;
	struct sockaddr sa;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int s;
	int ret;

	switch (cmd) {
	case TUNSETIFF:
		/* ignore the name, just look at flags */
		if (get_user(u, &ifr->ifr_flags))
			return -EFAULT;

		ret = 0;
<<<<<<< HEAD
		if ((u & ~IFF_VNET_HDR) != (IFF_NO_PI | IFF_TAP))
			ret = -EINVAL;
		else
			q->flags = u;
=======
		if ((u & ~MACVTAP_FEATURES) != (IFF_NO_PI | IFF_TAP))
			ret = -EINVAL;
		else
			q->flags = (q->flags & ~MACVTAP_FEATURES) | u;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		return ret;

	case TUNGETIFF:
<<<<<<< HEAD
		rcu_read_lock_bh();
		vlan = rcu_dereference_bh(q->vlan);
		if (vlan)
			dev_hold(vlan->dev);
		rcu_read_unlock_bh();

		if (!vlan)
			return -ENOLINK;

		ret = 0;
		if (copy_to_user(&ifr->ifr_name, vlan->dev->name, IFNAMSIZ) ||
		    put_user(q->flags, &ifr->ifr_flags))
			ret = -EFAULT;
		dev_put(vlan->dev);
		return ret;

	case TUNGETFEATURES:
		if (put_user(IFF_TAP | IFF_NO_PI | IFF_VNET_HDR, up))
=======
		rtnl_lock();
		vlan = macvtap_get_vlan(q);
		if (!vlan) {
			rtnl_unlock();
			return -ENOLINK;
		}

		ret = 0;
		u = q->flags;
		if (copy_to_user(&ifr->ifr_name, vlan->dev->name, IFNAMSIZ) ||
		    put_user(u, &ifr->ifr_flags))
			ret = -EFAULT;
		macvtap_put_vlan(vlan);
		rtnl_unlock();
		return ret;

	case TUNSETQUEUE:
		if (get_user(u, &ifr->ifr_flags))
			return -EFAULT;
		rtnl_lock();
		ret = macvtap_ioctl_set_queue(file, u);
		rtnl_unlock();
		return ret;

	case TUNGETFEATURES:
		if (put_user(IFF_TAP | IFF_NO_PI | MACVTAP_FEATURES, up))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			return -EFAULT;
		return 0;

	case TUNSETSNDBUF:
<<<<<<< HEAD
		if (get_user(u, up))
			return -EFAULT;

		q->sk.sk_sndbuf = u;
=======
		if (get_user(s, sp))
			return -EFAULT;
		if (s <= 0)
			return -EINVAL;

		q->sk.sk_sndbuf = s;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;

	case TUNGETVNETHDRSZ:
		s = q->vnet_hdr_sz;
		if (put_user(s, sp))
			return -EFAULT;
		return 0;

	case TUNSETVNETHDRSZ:
		if (get_user(s, sp))
			return -EFAULT;
		if (s < (int)sizeof(struct virtio_net_hdr))
			return -EINVAL;

		q->vnet_hdr_sz = s;
		return 0;

<<<<<<< HEAD
=======
	case TUNGETVNETLE:
		s = !!(q->flags & MACVTAP_VNET_LE);
		if (put_user(s, sp))
			return -EFAULT;
		return 0;

	case TUNSETVNETLE:
		if (get_user(s, sp))
			return -EFAULT;
		if (s)
			q->flags |= MACVTAP_VNET_LE;
		else
			q->flags &= ~MACVTAP_VNET_LE;
		return 0;

	case TUNGETVNETBE:
		return macvtap_get_vnet_be(q, sp);

	case TUNSETVNETBE:
		return macvtap_set_vnet_be(q, sp);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	case TUNSETOFFLOAD:
		/* let the user check for future flags */
		if (arg & ~(TUN_F_CSUM | TUN_F_TSO4 | TUN_F_TSO6 |
			    TUN_F_TSO_ECN | TUN_F_UFO))
			return -EINVAL;

<<<<<<< HEAD
		/* TODO: only accept frames with the features that
			 got enabled for forwarded frames */
		if (!(q->flags & IFF_VNET_HDR))
			return  -EINVAL;
		return 0;
=======
		rtnl_lock();
		ret = set_offload(q, arg);
		rtnl_unlock();
		return ret;

	case SIOCGIFHWADDR:
		rtnl_lock();
		vlan = macvtap_get_vlan(q);
		if (!vlan) {
			rtnl_unlock();
			return -ENOLINK;
		}
		ret = 0;
		u = vlan->dev->type;
		if (copy_to_user(&ifr->ifr_name, vlan->dev->name, IFNAMSIZ) ||
		    copy_to_user(&ifr->ifr_hwaddr.sa_data, vlan->dev->dev_addr, ETH_ALEN) ||
		    put_user(u, &ifr->ifr_hwaddr.sa_family))
			ret = -EFAULT;
		macvtap_put_vlan(vlan);
		rtnl_unlock();
		return ret;

	case SIOCSIFHWADDR:
		if (copy_from_user(&sa, &ifr->ifr_hwaddr, sizeof(sa)))
			return -EFAULT;
		rtnl_lock();
		vlan = macvtap_get_vlan(q);
		if (!vlan) {
			rtnl_unlock();
			return -ENOLINK;
		}
		ret = dev_set_mac_address(vlan->dev, &sa);
		macvtap_put_vlan(vlan);
		rtnl_unlock();
		return ret;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	default:
		return -EINVAL;
	}
}

#ifdef CONFIG_COMPAT
static long macvtap_compat_ioctl(struct file *file, unsigned int cmd,
				 unsigned long arg)
{
	return macvtap_ioctl(file, cmd, (unsigned long)compat_ptr(arg));
}
#endif

static const struct file_operations macvtap_fops = {
	.owner		= THIS_MODULE,
	.open		= macvtap_open,
	.release	= macvtap_release,
<<<<<<< HEAD
	.aio_read	= macvtap_aio_read,
	.aio_write	= macvtap_aio_write,
=======
	.read_iter	= macvtap_read_iter,
	.write_iter	= macvtap_write_iter,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.poll		= macvtap_poll,
	.llseek		= no_llseek,
	.unlocked_ioctl	= macvtap_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= macvtap_compat_ioctl,
#endif
};

<<<<<<< HEAD
static int macvtap_sendmsg(struct kiocb *iocb, struct socket *sock,
			   struct msghdr *m, size_t total_len)
{
	struct macvtap_queue *q = container_of(sock, struct macvtap_queue, sock);
	return macvtap_get_user(q, m, m->msg_iov, total_len, m->msg_iovlen,
			    m->msg_flags & MSG_DONTWAIT);
}

static int macvtap_recvmsg(struct kiocb *iocb, struct socket *sock,
			   struct msghdr *m, size_t total_len,
			   int flags)
=======
static int macvtap_sendmsg(struct socket *sock, struct msghdr *m,
			   size_t total_len)
{
	struct macvtap_queue *q = container_of(sock, struct macvtap_queue, sock);
	return macvtap_get_user(q, m, &m->msg_iter, m->msg_flags & MSG_DONTWAIT);
}

static int macvtap_recvmsg(struct socket *sock, struct msghdr *m,
			   size_t total_len, int flags)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct macvtap_queue *q = container_of(sock, struct macvtap_queue, sock);
	int ret;
	if (flags & ~(MSG_DONTWAIT|MSG_TRUNC))
		return -EINVAL;
<<<<<<< HEAD
	ret = macvtap_do_read(q, iocb, m->msg_iov, total_len,
			  flags & MSG_DONTWAIT);
=======
	ret = macvtap_do_read(q, &m->msg_iter, flags & MSG_DONTWAIT);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret > total_len) {
		m->msg_flags |= MSG_TRUNC;
		ret = flags & MSG_TRUNC ? ret : total_len;
	}
	return ret;
}

<<<<<<< HEAD
=======
static int macvtap_peek_len(struct socket *sock)
{
	struct macvtap_queue *q = container_of(sock, struct macvtap_queue,
					       sock);
	return skb_array_peek_len(&q->skb_array);
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Ops structure to mimic raw sockets with tun */
static const struct proto_ops macvtap_socket_ops = {
	.sendmsg = macvtap_sendmsg,
	.recvmsg = macvtap_recvmsg,
<<<<<<< HEAD
=======
	.peek_len = macvtap_peek_len,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

/* Get an underlying socket object from tun file.  Returns error unless file is
 * attached to a device.  The returned object works like a packet socket, it
 * can be used for sock_sendmsg/sock_recvmsg.  The caller is responsible for
 * holding a reference to the file for as long as the socket is in use. */
struct socket *macvtap_get_socket(struct file *file)
{
	struct macvtap_queue *q;
	if (file->f_op != &macvtap_fops)
		return ERR_PTR(-EINVAL);
	q = file->private_data;
	if (!q)
		return ERR_PTR(-EBADFD);
	return &q->sock;
}
EXPORT_SYMBOL_GPL(macvtap_get_socket);

<<<<<<< HEAD
static int macvtap_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = ptr;
=======
static int macvtap_queue_resize(struct macvlan_dev *vlan)
{
	struct net_device *dev = vlan->dev;
	struct macvtap_queue *q;
	struct skb_array **arrays;
	int n = vlan->numqueues;
	int ret, i = 0;

	arrays = kmalloc(sizeof *arrays * n, GFP_KERNEL);
	if (!arrays)
		return -ENOMEM;

	list_for_each_entry(q, &vlan->queue_list, next)
		arrays[i++] = &q->skb_array;

	ret = skb_array_resize_multiple(arrays, n,
					dev->tx_queue_len, GFP_KERNEL);

	kfree(arrays);
	return ret;
}

static int macvtap_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct macvlan_dev *vlan;
	struct device *classdev;
	dev_t devt;
	int err;
<<<<<<< HEAD
=======
	char tap_name[IFNAMSIZ];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (dev->rtnl_link_ops != &macvtap_link_ops)
		return NOTIFY_DONE;

<<<<<<< HEAD
=======
	snprintf(tap_name, IFNAMSIZ, "tap%d", dev->ifindex);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	vlan = netdev_priv(dev);

	switch (event) {
	case NETDEV_REGISTER:
		/* Create the device node here after the network device has
		 * been registered but before register_netdevice has
		 * finished running.
		 */
		err = macvtap_get_minor(vlan);
		if (err)
			return notifier_from_errno(err);

		devt = MKDEV(MAJOR(macvtap_major), vlan->minor);
<<<<<<< HEAD
		classdev = device_create(macvtap_class, &dev->dev, devt,
					 dev, "tap%d", dev->ifindex);
=======
		classdev = device_create(&macvtap_class, &dev->dev, devt,
					 dev, tap_name);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (IS_ERR(classdev)) {
			macvtap_free_minor(vlan);
			return notifier_from_errno(PTR_ERR(classdev));
		}
<<<<<<< HEAD
		break;
	case NETDEV_UNREGISTER:
		devt = MKDEV(MAJOR(macvtap_major), vlan->minor);
		device_destroy(macvtap_class, devt);
		macvtap_free_minor(vlan);
		break;
=======
		err = sysfs_create_link(&dev->dev.kobj, &classdev->kobj,
					tap_name);
		if (err)
			return notifier_from_errno(err);
		break;
	case NETDEV_UNREGISTER:
		/* vlan->minor == 0 if NETDEV_REGISTER above failed */
		if (vlan->minor == 0)
			break;
		sysfs_remove_link(&dev->dev.kobj, tap_name);
		devt = MKDEV(MAJOR(macvtap_major), vlan->minor);
		device_destroy(&macvtap_class, devt);
		macvtap_free_minor(vlan);
		break;
	case NETDEV_CHANGE_TX_QUEUE_LEN:
		if (macvtap_queue_resize(vlan))
			return NOTIFY_BAD;
		break;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return NOTIFY_DONE;
}

static struct notifier_block macvtap_notifier_block __read_mostly = {
	.notifier_call	= macvtap_device_event,
};

static int macvtap_init(void)
{
	int err;

	err = alloc_chrdev_region(&macvtap_major, 0,
				MACVTAP_NUM_DEVS, "macvtap");
	if (err)
		goto out1;

	cdev_init(&macvtap_cdev, &macvtap_fops);
	err = cdev_add(&macvtap_cdev, macvtap_major, MACVTAP_NUM_DEVS);
	if (err)
		goto out2;

<<<<<<< HEAD
	macvtap_class = class_create(THIS_MODULE, "macvtap");
	if (IS_ERR(macvtap_class)) {
		err = PTR_ERR(macvtap_class);
		goto out3;
	}
=======
	err = class_register(&macvtap_class);
	if (err)
		goto out3;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	err = register_netdevice_notifier(&macvtap_notifier_block);
	if (err)
		goto out4;

	err = macvlan_link_register(&macvtap_link_ops);
	if (err)
		goto out5;

	return 0;

out5:
	unregister_netdevice_notifier(&macvtap_notifier_block);
out4:
<<<<<<< HEAD
	class_unregister(macvtap_class);
=======
	class_unregister(&macvtap_class);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
out3:
	cdev_del(&macvtap_cdev);
out2:
	unregister_chrdev_region(macvtap_major, MACVTAP_NUM_DEVS);
out1:
	return err;
}
module_init(macvtap_init);

static void macvtap_exit(void)
{
	rtnl_link_unregister(&macvtap_link_ops);
	unregister_netdevice_notifier(&macvtap_notifier_block);
<<<<<<< HEAD
	class_unregister(macvtap_class);
	cdev_del(&macvtap_cdev);
	unregister_chrdev_region(macvtap_major, MACVTAP_NUM_DEVS);
=======
	class_unregister(&macvtap_class);
	cdev_del(&macvtap_cdev);
	unregister_chrdev_region(macvtap_major, MACVTAP_NUM_DEVS);
	idr_destroy(&minor_idr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
module_exit(macvtap_exit);

MODULE_ALIAS_RTNL_LINK("macvtap");
MODULE_AUTHOR("Arnd Bergmann <arnd@arndb.de>");
MODULE_LICENSE("GPL");
