/*
 * net/core/netprio_cgroup.c	Priority Control Group
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Neil Horman <nhorman@tuxdriver.com>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/cgroup.h>
#include <linux/rcupdate.h>
#include <linux/atomic.h>
#include <net/rtnetlink.h>
#include <net/pkt_cls.h>
#include <net/sock.h>
#include <net/netprio_cgroup.h>

#include <linux/fdtable.h>

<<<<<<< HEAD
#define PRIOMAP_MIN_SZ		128

static inline struct cgroup_netprio_state *cgrp_netprio_state(struct cgroup *cgrp)
{
	return container_of(cgroup_subsys_state(cgrp, net_prio_subsys_id),
			    struct cgroup_netprio_state, css);
}

/*
 * Extend @dev->priomap so that it's large enough to accomodate
=======
/*
 * netprio allocates per-net_device priomap array which is indexed by
 * css->id.  Limiting css ID to 16bits doesn't lose anything.
 */
#define NETPRIO_ID_MAX		USHRT_MAX

#define PRIOMAP_MIN_SZ		128

/*
 * Extend @dev->priomap so that it's large enough to accommodate
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @target_idx.  @dev->priomap.priomap_len > @target_idx after successful
 * return.  Must be called under rtnl lock.
 */
static int extend_netdev_table(struct net_device *dev, u32 target_idx)
{
	struct netprio_map *old, *new;
	size_t new_sz, new_len;

	/* is the existing priomap large enough? */
	old = rtnl_dereference(dev->priomap);
	if (old && old->priomap_len > target_idx)
		return 0;

	/*
	 * Determine the new size.  Let's keep it power-of-two.  We start
	 * from PRIOMAP_MIN_SZ and double it until it's large enough to
	 * accommodate @target_idx.
	 */
	new_sz = PRIOMAP_MIN_SZ;
	while (true) {
		new_len = (new_sz - offsetof(struct netprio_map, priomap)) /
			sizeof(new->priomap[0]);
		if (new_len > target_idx)
			break;
		new_sz *= 2;
		/* overflowed? */
		if (WARN_ON(new_sz < PRIOMAP_MIN_SZ))
			return -ENOSPC;
	}

	/* allocate & copy */
	new = kzalloc(new_sz, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	if (old)
		memcpy(new->priomap, old->priomap,
		       old->priomap_len * sizeof(old->priomap[0]));

	new->priomap_len = new_len;

	/* install the new priomap */
	rcu_assign_pointer(dev->priomap, new);
	if (old)
		kfree_rcu(old, rcu);
	return 0;
}

/**
 * netprio_prio - return the effective netprio of a cgroup-net_device pair
<<<<<<< HEAD
 * @cgrp: cgroup part of the target pair
=======
 * @css: css part of the target pair
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * @dev: net_device part of the target pair
 *
 * Should be called under RCU read or rtnl lock.
 */
<<<<<<< HEAD
static u32 netprio_prio(struct cgroup *cgrp, struct net_device *dev)
{
	struct netprio_map *map = rcu_dereference_rtnl(dev->priomap);

	if (map && cgrp->id < map->priomap_len)
		return map->priomap[cgrp->id];
=======
static u32 netprio_prio(struct cgroup_subsys_state *css, struct net_device *dev)
{
	struct netprio_map *map = rcu_dereference_rtnl(dev->priomap);
	int id = css->cgroup->id;

	if (map && id < map->priomap_len)
		return map->priomap[id];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

/**
 * netprio_set_prio - set netprio on a cgroup-net_device pair
<<<<<<< HEAD
 * @cgrp: cgroup part of the target pair
 * @dev: net_device part of the target pair
 * @prio: prio to set
 *
 * Set netprio to @prio on @cgrp-@dev pair.  Should be called under rtnl
 * lock and may fail under memory pressure for non-zero @prio.
 */
static int netprio_set_prio(struct cgroup *cgrp, struct net_device *dev,
			    u32 prio)
{
	struct netprio_map *map;
=======
 * @css: css part of the target pair
 * @dev: net_device part of the target pair
 * @prio: prio to set
 *
 * Set netprio to @prio on @css-@dev pair.  Should be called under rtnl
 * lock and may fail under memory pressure for non-zero @prio.
 */
static int netprio_set_prio(struct cgroup_subsys_state *css,
			    struct net_device *dev, u32 prio)
{
	struct netprio_map *map;
	int id = css->cgroup->id;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	/* avoid extending priomap for zero writes */
	map = rtnl_dereference(dev->priomap);
<<<<<<< HEAD
	if (!prio && (!map || map->priomap_len <= cgrp->id))
		return 0;

	ret = extend_netdev_table(dev, cgrp->id);
=======
	if (!prio && (!map || map->priomap_len <= id))
		return 0;

	ret = extend_netdev_table(dev, id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret)
		return ret;

	map = rtnl_dereference(dev->priomap);
<<<<<<< HEAD
	map->priomap[cgrp->id] = prio;
	return 0;
}

static struct cgroup_subsys_state *cgrp_css_alloc(struct cgroup *cgrp)
{
	struct cgroup_netprio_state *cs;

	cs = kzalloc(sizeof(*cs), GFP_KERNEL);
	if (!cs)
		return ERR_PTR(-ENOMEM);

	return &cs->css;
}

static int cgrp_css_online(struct cgroup *cgrp)
{
	struct cgroup *parent = cgrp->parent;
	struct net_device *dev;
	int ret = 0;

	if (!parent)
=======
	map->priomap[id] = prio;
	return 0;
}

static struct cgroup_subsys_state *
cgrp_css_alloc(struct cgroup_subsys_state *parent_css)
{
	struct cgroup_subsys_state *css;

	css = kzalloc(sizeof(*css), GFP_KERNEL);
	if (!css)
		return ERR_PTR(-ENOMEM);

	return css;
}

static int cgrp_css_online(struct cgroup_subsys_state *css)
{
	struct cgroup_subsys_state *parent_css = css->parent;
	struct net_device *dev;
	int ret = 0;

	if (css->id > NETPRIO_ID_MAX)
		return -ENOSPC;

	if (!parent_css)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 0;

	rtnl_lock();
	/*
	 * Inherit prios from the parent.  As all prios are set during
	 * onlining, there is no need to clear them on offline.
	 */
	for_each_netdev(&init_net, dev) {
<<<<<<< HEAD
		u32 prio = netprio_prio(parent, dev);

		ret = netprio_set_prio(cgrp, dev, prio);
=======
		u32 prio = netprio_prio(parent_css, dev);

		ret = netprio_set_prio(css, dev, prio);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		if (ret)
			break;
	}
	rtnl_unlock();
	return ret;
}

<<<<<<< HEAD
static void cgrp_css_free(struct cgroup *cgrp)
{
	kfree(cgrp_netprio_state(cgrp));
}

static u64 read_prioidx(struct cgroup *cgrp, struct cftype *cft)
{
	return cgrp->id;
}

static int read_priomap(struct cgroup *cont, struct cftype *cft,
			struct cgroup_map_cb *cb)
=======
static void cgrp_css_free(struct cgroup_subsys_state *css)
{
	kfree(css);
}

static u64 read_prioidx(struct cgroup_subsys_state *css, struct cftype *cft)
{
	return css->cgroup->id;
}

static int read_priomap(struct seq_file *sf, void *v)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct net_device *dev;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev)
<<<<<<< HEAD
		cb->fill(cb, dev->name, netprio_prio(cont, dev));
=======
		seq_printf(sf, "%s %u\n", dev->name,
			   netprio_prio(seq_css(sf), dev));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	rcu_read_unlock();
	return 0;
}

<<<<<<< HEAD
static int write_priomap(struct cgroup *cgrp, struct cftype *cft,
			 const char *buffer)
=======
static ssize_t write_priomap(struct kernfs_open_file *of,
			     char *buf, size_t nbytes, loff_t off)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	char devname[IFNAMSIZ + 1];
	struct net_device *dev;
	u32 prio;
	int ret;

<<<<<<< HEAD
	if (sscanf(buffer, "%"__stringify(IFNAMSIZ)"s %u", devname, &prio) != 2)
=======
	if (sscanf(buf, "%"__stringify(IFNAMSIZ)"s %u", devname, &prio) != 2)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -EINVAL;

	dev = dev_get_by_name(&init_net, devname);
	if (!dev)
		return -ENODEV;

<<<<<<< HEAD
	rtnl_lock();

	ret = netprio_set_prio(cgrp, dev, prio);

	rtnl_unlock();
	dev_put(dev);
	return ret;
=======
	cgroup_sk_alloc_disable();

	rtnl_lock();

	ret = netprio_set_prio(of_css(of), dev, prio);

	rtnl_unlock();
	dev_put(dev);
	return ret ?: nbytes;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static int update_netprio(const void *v, struct file *file, unsigned n)
{
	int err;
	struct socket *sock = sock_from_file(file, &err);
<<<<<<< HEAD
	if (sock)
		sock->sk->sk_cgrp_prioidx = (u32)(unsigned long)v;
	return 0;
}

static void net_prio_attach(struct cgroup *cgrp, struct cgroup_taskset *tset)
{
	struct task_struct *p;
	void *v;

	cgroup_taskset_for_each(p, cgrp, tset) {
		task_lock(p);
		v = (void *)(unsigned long)task_netprioidx(p);
=======
	if (sock) {
		spin_lock(&cgroup_sk_update_lock);
		sock_cgroup_set_prioidx(&sock->sk->sk_cgrp_data,
					(unsigned long)v);
		spin_unlock(&cgroup_sk_update_lock);
	}
	return 0;
}

static void net_prio_attach(struct cgroup_taskset *tset)
{
	struct task_struct *p;
	struct cgroup_subsys_state *css;

	cgroup_sk_alloc_disable();

	cgroup_taskset_for_each(p, css, tset) {
		void *v = (void *)(unsigned long)css->cgroup->id;

		task_lock(p);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		iterate_fd(p->files, 0, update_netprio, v);
		task_unlock(p);
	}
}

static struct cftype ss_files[] = {
	{
		.name = "prioidx",
		.read_u64 = read_prioidx,
	},
	{
		.name = "ifpriomap",
<<<<<<< HEAD
		.read_map = read_priomap,
		.write_string = write_priomap,
=======
		.seq_show = read_priomap,
		.write = write_priomap,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{ }	/* terminate */
};

<<<<<<< HEAD
struct cgroup_subsys net_prio_subsys = {
	.name		= "net_prio",
=======
struct cgroup_subsys net_prio_cgrp_subsys = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.css_alloc	= cgrp_css_alloc,
	.css_online	= cgrp_css_online,
	.css_free	= cgrp_css_free,
	.attach		= net_prio_attach,
<<<<<<< HEAD
	.subsys_id	= net_prio_subsys_id,
	.base_cftypes	= ss_files,
	.module		= THIS_MODULE,
=======
	.legacy_cftypes	= ss_files,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

static int netprio_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev = ptr;
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	struct netprio_map *old;

	/*
	 * Note this is called with rtnl_lock held so we have update side
	 * protection on our rcu assignments
	 */

	switch (event) {
	case NETDEV_UNREGISTER:
		old = rtnl_dereference(dev->priomap);
		RCU_INIT_POINTER(dev->priomap, NULL);
		if (old)
			kfree_rcu(old, rcu);
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block netprio_device_notifier = {
	.notifier_call = netprio_device_event
};

static int __init init_cgroup_netprio(void)
{
<<<<<<< HEAD
	int ret;

	ret = cgroup_load_subsys(&net_prio_subsys);
	if (ret)
		goto out;

	register_netdevice_notifier(&netprio_device_notifier);

out:
	return ret;
}

static void __exit exit_cgroup_netprio(void)
{
	struct netprio_map *old;
	struct net_device *dev;

	unregister_netdevice_notifier(&netprio_device_notifier);

	cgroup_unload_subsys(&net_prio_subsys);

	rtnl_lock();
	for_each_netdev(&init_net, dev) {
		old = rtnl_dereference(dev->priomap);
		RCU_INIT_POINTER(dev->priomap, NULL);
		if (old)
			kfree_rcu(old, rcu);
	}
	rtnl_unlock();
}

module_init(init_cgroup_netprio);
module_exit(exit_cgroup_netprio);
=======
	register_netdevice_notifier(&netprio_device_notifier);
	return 0;
}

subsys_initcall(init_cgroup_netprio);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
MODULE_LICENSE("GPL v2");
