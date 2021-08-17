/*
 * netprio_cgroup.h			Control Group Priority set
 *
 *
 * Authors:	Neil Horman <nhorman@tuxdriver.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#ifndef _NETPRIO_CGROUP_H
#define _NETPRIO_CGROUP_H
<<<<<<< HEAD
=======

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>

<<<<<<< HEAD

#if IS_ENABLED(CONFIG_NETPRIO_CGROUP)
=======
#if IS_ENABLED(CONFIG_CGROUP_NET_PRIO)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct netprio_map {
	struct rcu_head rcu;
	u32 priomap_len;
	u32 priomap[];
};

<<<<<<< HEAD
struct cgroup_netprio_state {
	struct cgroup_subsys_state css;
};

extern void sock_update_netprioidx(struct sock *sk);

#if IS_BUILTIN(CONFIG_NETPRIO_CGROUP)

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline u32 task_netprioidx(struct task_struct *p)
{
	struct cgroup_subsys_state *css;
	u32 idx;

	rcu_read_lock();
<<<<<<< HEAD
	css = task_subsys_state(p, net_prio_subsys_id);
=======
	css = task_css(p, net_prio_cgrp_id);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	idx = css->cgroup->id;
	rcu_read_unlock();
	return idx;
}

<<<<<<< HEAD
#elif IS_MODULE(CONFIG_NETPRIO_CGROUP)

static inline u32 task_netprioidx(struct task_struct *p)
{
	struct cgroup_subsys_state *css;
	u32 idx = 0;

	rcu_read_lock();
	css = task_subsys_state(p, net_prio_subsys_id);
	if (css)
		idx = css->cgroup->id;
	rcu_read_unlock();
	return idx;
}
#endif

#else /* !CONFIG_NETPRIO_CGROUP */
=======
static inline void sock_update_netprioidx(struct sock_cgroup_data *skcd)
{
	if (in_interrupt())
		return;

	sock_cgroup_set_prioidx(skcd, task_netprioidx(current));
}

#else /* !CONFIG_CGROUP_NET_PRIO */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline u32 task_netprioidx(struct task_struct *p)
{
	return 0;
}

<<<<<<< HEAD
#define sock_update_netprioidx(sk)

#endif /* CONFIG_NETPRIO_CGROUP */

=======
static inline void sock_update_netprioidx(struct sock_cgroup_data *skcd)
{
}

#endif /* CONFIG_CGROUP_NET_PRIO */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif  /* _NET_CLS_CGROUP_H */
