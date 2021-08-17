/*
 * sysctl_net_ipv4.c: sysctl interface to net IPV4 subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/ipv4 directory entry (empty =) ). [MS]
 */

#include <linux/mm.h>
#include <linux/module.h>
#include <linux/sysctl.h>
#include <linux/igmp.h>
#include <linux/inetdevice.h>
#include <linux/seqlock.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/nsproxy.h>
#include <linux/swap.h>
#include <net/snmp.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/route.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/cipso_ipv4.h>
#include <net/inet_frag.h>
#include <net/ping.h>
<<<<<<< HEAD
#include <net/tcp_memcontrol.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int zero;
static int one = 1;
static int four = 4;
<<<<<<< HEAD
=======
static int thousand = 1000;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int gso_max_segs = GSO_MAX_SEGS;
static int tcp_retr1_max = 255;
static int ip_local_port_range_min[] = { 1, 1 };
static int ip_local_port_range_max[] = { 65535, 65535 };
static int tcp_adv_win_scale_min = -31;
static int tcp_adv_win_scale_max = 31;
<<<<<<< HEAD
=======
static int tcp_min_snd_mss_min = TCP_MIN_SND_MSS;
static int tcp_min_snd_mss_max = 65535;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static int ip_ttl_min = 1;
static int ip_ttl_max = 255;
static int tcp_syn_retries_min = 1;
static int tcp_syn_retries_max = MAX_TCP_SYNCNT;
static int ip_ping_group_range_min[] = { 0, 0 };
static int ip_ping_group_range_max[] = { GID_T_MAX, GID_T_MAX };
<<<<<<< HEAD

/* Update system visible IP port range */
static void set_local_port_range(int range[2])
{
	write_seqlock(&sysctl_local_ports.lock);
	sysctl_local_ports.range[0] = range[0];
	sysctl_local_ports.range[1] = range[1];
	write_sequnlock(&sysctl_local_ports.lock);
}

/* Validate changes from /proc interface. */
static int ipv4_local_port_range(ctl_table *table, int write,
				 void __user *buffer,
				 size_t *lenp, loff_t *ppos)
{
	int ret;
	int range[2];
	ctl_table tmp = {
=======
static int one_day_secs = 24 * 3600;

/* Update system visible IP port range */
static void set_local_port_range(struct net *net, int range[2])
{
	bool same_parity = !((range[0] ^ range[1]) & 1);

	write_seqlock_bh(&net->ipv4.ip_local_ports.lock);
	if (same_parity && !net->ipv4.ip_local_ports.warned) {
		net->ipv4.ip_local_ports.warned = true;
		pr_err_ratelimited("ip_local_port_range: prefer different parity for start/end values.\n");
	}
	net->ipv4.ip_local_ports.range[0] = range[0];
	net->ipv4.ip_local_ports.range[1] = range[1];
	write_sequnlock_bh(&net->ipv4.ip_local_ports.lock);
}

/* Validate changes from /proc interface. */
static int ipv4_local_port_range(struct ctl_table *table, int write,
				 void __user *buffer,
				 size_t *lenp, loff_t *ppos)
{
	struct net *net =
		container_of(table->data, struct net, ipv4.ip_local_ports.range);
	int ret;
	int range[2];
	struct ctl_table tmp = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.data = &range,
		.maxlen = sizeof(range),
		.mode = table->mode,
		.extra1 = &ip_local_port_range_min,
		.extra2 = &ip_local_port_range_max,
	};

<<<<<<< HEAD
	inet_get_local_port_range(range, range + 1);
=======
	inet_get_local_port_range(net, &range[0], &range[1]);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	ret = proc_dointvec_minmax(&tmp, write, buffer, lenp, ppos);

	if (write && ret == 0) {
		if (range[1] < range[0])
			ret = -EINVAL;
		else
<<<<<<< HEAD
			set_local_port_range(range);
=======
			set_local_port_range(net, range);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return ret;
}


static void inet_get_ping_group_range_table(struct ctl_table *table, kgid_t *low, kgid_t *high)
{
	kgid_t *data = table->data;
<<<<<<< HEAD
	unsigned int seq;
	do {
		seq = read_seqbegin(&sysctl_local_ports.lock);

		*low = data[0];
		*high = data[1];
	} while (read_seqretry(&sysctl_local_ports.lock, seq));
=======
	struct net *net =
		container_of(table->data, struct net, ipv4.ping_group_range.range);
	unsigned int seq;
	do {
		seq = read_seqbegin(&net->ipv4.ping_group_range.lock);

		*low = data[0];
		*high = data[1];
	} while (read_seqretry(&net->ipv4.ping_group_range.lock, seq));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/* Update system visible IP port range */
static void set_ping_group_range(struct ctl_table *table, kgid_t low, kgid_t high)
{
	kgid_t *data = table->data;
<<<<<<< HEAD
	write_seqlock(&sysctl_local_ports.lock);
	data[0] = low;
	data[1] = high;
	write_sequnlock(&sysctl_local_ports.lock);
}

/* Validate changes from /proc interface. */
static int ipv4_ping_group_range(ctl_table *table, int write,
=======
	struct net *net =
		container_of(table->data, struct net, ipv4.ping_group_range.range);
	write_seqlock(&net->ipv4.ping_group_range.lock);
	data[0] = low;
	data[1] = high;
	write_sequnlock(&net->ipv4.ping_group_range.lock);
}

/* Validate changes from /proc interface. */
static int ipv4_ping_group_range(struct ctl_table *table, int write,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				 void __user *buffer,
				 size_t *lenp, loff_t *ppos)
{
	struct user_namespace *user_ns = current_user_ns();
	int ret;
	gid_t urange[2];
	kgid_t low, high;
<<<<<<< HEAD
	ctl_table tmp = {
=======
	struct ctl_table tmp = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.data = &urange,
		.maxlen = sizeof(urange),
		.mode = table->mode,
		.extra1 = &ip_ping_group_range_min,
		.extra2 = &ip_ping_group_range_max,
	};

	inet_get_ping_group_range_table(table, &low, &high);
	urange[0] = from_kgid_munged(user_ns, low);
	urange[1] = from_kgid_munged(user_ns, high);
	ret = proc_dointvec_minmax(&tmp, write, buffer, lenp, ppos);

	if (write && ret == 0) {
		low = make_kgid(user_ns, urange[0]);
		high = make_kgid(user_ns, urange[1]);
<<<<<<< HEAD
		if (!gid_valid(low) || !gid_valid(high) ||
		    (urange[1] < urange[0]) || gid_lt(high, low)) {
=======
		if (!gid_valid(low) || !gid_valid(high))
			return -EINVAL;
		if (urange[1] < urange[0] || gid_lt(high, low)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			low = make_kgid(&init_user_ns, 1);
			high = make_kgid(&init_user_ns, 0);
		}
		set_ping_group_range(table, low, high);
	}

	return ret;
}

/* Validate changes from /proc interface. */
<<<<<<< HEAD
static int proc_tcp_default_init_rwnd(ctl_table *ctl, int write,
=======
static int proc_tcp_default_init_rwnd(struct ctl_table *ctl, int write,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				      void __user *buffer,
				      size_t *lenp, loff_t *ppos)
{
	int old_value = *(int *)ctl->data;
	int ret = proc_dointvec(ctl, write, buffer, lenp, ppos);
	int new_value = *(int *)ctl->data;

	if (write && ret == 0 && (new_value < 3 || new_value > 100))
		*(int *)ctl->data = old_value;

	return ret;
}

<<<<<<< HEAD
static int proc_tcp_congestion_control(ctl_table *ctl, int write,
				       void __user *buffer, size_t *lenp, loff_t *ppos)
{
	char val[TCP_CA_NAME_MAX];
	ctl_table tbl = {
=======
static int proc_tcp_congestion_control(struct ctl_table *ctl, int write,
				       void __user *buffer, size_t *lenp, loff_t *ppos)
{
	char val[TCP_CA_NAME_MAX];
	struct ctl_table tbl = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.data = val,
		.maxlen = TCP_CA_NAME_MAX,
	};
	int ret;

	tcp_get_default_congestion_control(val);

	ret = proc_dostring(&tbl, write, buffer, lenp, ppos);
	if (write && ret == 0)
		ret = tcp_set_default_congestion_control(val);
	return ret;
}

<<<<<<< HEAD
static int proc_tcp_available_congestion_control(ctl_table *ctl,
=======
static int proc_tcp_available_congestion_control(struct ctl_table *ctl,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
						 int write,
						 void __user *buffer, size_t *lenp,
						 loff_t *ppos)
{
<<<<<<< HEAD
	ctl_table tbl = { .maxlen = TCP_CA_BUF_MAX, };
=======
	struct ctl_table tbl = { .maxlen = TCP_CA_BUF_MAX, };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	tbl.data = kmalloc(tbl.maxlen, GFP_USER);
	if (!tbl.data)
		return -ENOMEM;
	tcp_get_available_congestion_control(tbl.data, TCP_CA_BUF_MAX);
	ret = proc_dostring(&tbl, write, buffer, lenp, ppos);
	kfree(tbl.data);
	return ret;
}

<<<<<<< HEAD
static int proc_allowed_congestion_control(ctl_table *ctl,
=======
static int proc_allowed_congestion_control(struct ctl_table *ctl,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
					   int write,
					   void __user *buffer, size_t *lenp,
					   loff_t *ppos)
{
<<<<<<< HEAD
	ctl_table tbl = { .maxlen = TCP_CA_BUF_MAX };
=======
	struct ctl_table tbl = { .maxlen = TCP_CA_BUF_MAX };
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int ret;

	tbl.data = kmalloc(tbl.maxlen, GFP_USER);
	if (!tbl.data)
		return -ENOMEM;

	tcp_get_allowed_congestion_control(tbl.data, tbl.maxlen);
	ret = proc_dostring(&tbl, write, buffer, lenp, ppos);
	if (write && ret == 0)
		ret = tcp_set_allowed_congestion_control(tbl.data);
	kfree(tbl.data);
	return ret;
}

<<<<<<< HEAD
static int ipv4_tcp_mem(ctl_table *ctl, int write,
			   void __user *buffer, size_t *lenp,
			   loff_t *ppos)
{
	int ret;
	unsigned long vec[3];
	struct net *net = current->nsproxy->net_ns;
#ifdef CONFIG_MEMCG_KMEM
	struct mem_cgroup *memcg;
#endif

	ctl_table tmp = {
		.data = &vec,
		.maxlen = sizeof(vec),
		.mode = ctl->mode,
	};

	if (!write) {
		ctl->data = &net->ipv4.sysctl_tcp_mem;
		return proc_doulongvec_minmax(ctl, write, buffer, lenp, ppos);
	}

	ret = proc_doulongvec_minmax(&tmp, write, buffer, lenp, ppos);
	if (ret)
		return ret;

#ifdef CONFIG_MEMCG_KMEM
	rcu_read_lock();
	memcg = mem_cgroup_from_task(current);

	tcp_prot_mem(memcg, vec[0], 0);
	tcp_prot_mem(memcg, vec[1], 1);
	tcp_prot_mem(memcg, vec[2], 2);
	rcu_read_unlock();
#endif

	net->ipv4.sysctl_tcp_mem[0] = vec[0];
	net->ipv4.sysctl_tcp_mem[1] = vec[1];
	net->ipv4.sysctl_tcp_mem[2] = vec[2];

	return 0;
}

static int proc_tcp_fastopen_key(ctl_table *ctl, int write, void __user *buffer,
				 size_t *lenp, loff_t *ppos)
{
	ctl_table tbl = { .maxlen = (TCP_FASTOPEN_KEY_LENGTH * 2 + 10) };
	struct tcp_fastopen_context *ctxt;
	int ret;
	u32  user_key[4]; /* 16 bytes, matching TCP_FASTOPEN_KEY_LENGTH */
=======
static int proc_tcp_fastopen_key(struct ctl_table *ctl, int write,
				 void __user *buffer, size_t *lenp,
				 loff_t *ppos)
{
	struct ctl_table tbl = { .maxlen = (TCP_FASTOPEN_KEY_LENGTH * 2 + 10) };
	struct tcp_fastopen_context *ctxt;
	u32  user_key[4]; /* 16 bytes, matching TCP_FASTOPEN_KEY_LENGTH */
	__le32 key[4];
	int ret, i;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tbl.data = kmalloc(tbl.maxlen, GFP_KERNEL);
	if (!tbl.data)
		return -ENOMEM;

	rcu_read_lock();
	ctxt = rcu_dereference(tcp_fastopen_ctx);
	if (ctxt)
<<<<<<< HEAD
		memcpy(user_key, ctxt->key, TCP_FASTOPEN_KEY_LENGTH);
	else
		memset(user_key, 0, sizeof(user_key));
	rcu_read_unlock();

=======
		memcpy(key, ctxt->key, TCP_FASTOPEN_KEY_LENGTH);
	else
		memset(key, 0, sizeof(key));
	rcu_read_unlock();

	for (i = 0; i < ARRAY_SIZE(key); i++)
		user_key[i] = le32_to_cpu(key[i]);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	snprintf(tbl.data, tbl.maxlen, "%08x-%08x-%08x-%08x",
		user_key[0], user_key[1], user_key[2], user_key[3]);
	ret = proc_dostring(&tbl, write, buffer, lenp, ppos);

	if (write && ret == 0) {
		if (sscanf(tbl.data, "%x-%x-%x-%x", user_key, user_key + 1,
			   user_key + 2, user_key + 3) != 4) {
			ret = -EINVAL;
			goto bad_key;
		}
<<<<<<< HEAD
		tcp_fastopen_reset_cipher(user_key, TCP_FASTOPEN_KEY_LENGTH);
=======
		/* Generate a dummy secret but don't publish it. This
		 * is needed so we don't regenerate a new key on the
		 * first invocation of tcp_fastopen_cookie_gen
		 */
		tcp_fastopen_init_key_once(false);

		for (i = 0; i < ARRAY_SIZE(user_key); i++)
			key[i] = cpu_to_le32(user_key[i]);

		tcp_fastopen_reset_cipher(key, TCP_FASTOPEN_KEY_LENGTH);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

bad_key:
	pr_debug("proc FO key set 0x%x-%x-%x-%x <- 0x%s: %u\n",
<<<<<<< HEAD
	       user_key[0], user_key[1], user_key[2], user_key[3],
=======
		 user_key[0], user_key[1], user_key[2], user_key[3],
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	       (char *)tbl.data, ret);
	kfree(tbl.data);
	return ret;
}

static struct ctl_table ipv4_table[] = {
	{
		.procname	= "tcp_timestamps",
		.data		= &sysctl_tcp_timestamps,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_window_scaling",
		.data		= &sysctl_tcp_window_scaling,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_sack",
		.data		= &sysctl_tcp_sack,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_retrans_collapse",
		.data		= &sysctl_tcp_retrans_collapse,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
		.procname	= "ip_default_ttl",
		.data		= &sysctl_ip_default_ttl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &ip_ttl_min,
		.extra2		= &ip_ttl_max,
	},
	{
		.procname	= "ip_no_pmtu_disc",
		.data		= &ipv4_config.no_pmtu_disc,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_nonlocal_bind",
		.data		= &sysctl_ip_nonlocal_bind,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_syn_retries",
		.data		= &sysctl_tcp_syn_retries,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &tcp_syn_retries_min,
		.extra2		= &tcp_syn_retries_max
	},
	{
		.procname	= "tcp_synack_retries",
		.data		= &sysctl_tcp_synack_retries,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "tcp_max_orphans",
		.data		= &sysctl_tcp_max_orphans,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_max_tw_buckets",
		.data		= &tcp_death_row.sysctl_max_tw_buckets,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
		.procname	= "ip_early_demux",
		.data		= &sysctl_ip_early_demux,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_dynaddr",
		.data		= &sysctl_ip_dynaddr,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_keepalive_time",
		.data		= &sysctl_tcp_keepalive_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "tcp_keepalive_probes",
		.data		= &sysctl_tcp_keepalive_probes,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_keepalive_intvl",
		.data		= &sysctl_tcp_keepalive_intvl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "tcp_retries1",
		.data		= &sysctl_tcp_retries1,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra2		= &tcp_retr1_max
	},
	{
		.procname	= "tcp_retries2",
		.data		= &sysctl_tcp_retries2,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_fin_timeout",
		.data		= &sysctl_tcp_fin_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#ifdef CONFIG_SYN_COOKIES
	{
		.procname	= "tcp_syncookies",
		.data		= &sysctl_tcp_syncookies,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#endif
	{
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "tcp_fastopen",
		.data		= &sysctl_tcp_fastopen,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_fastopen_key",
		.mode		= 0600,
		.maxlen		= ((TCP_FASTOPEN_KEY_LENGTH * 2) + 10),
		.proc_handler	= proc_tcp_fastopen_key,
	},
	{
		.procname	= "tcp_tw_recycle",
		.data		= &tcp_death_row.sysctl_tw_recycle,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_abort_on_overflow",
		.data		= &sysctl_tcp_abort_on_overflow,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_stdurg",
		.data		= &sysctl_tcp_stdurg,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_rfc1337",
		.data		= &sysctl_tcp_rfc1337,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_max_syn_backlog",
		.data		= &sysctl_max_syn_backlog,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
		.procname	= "ip_local_port_range",
		.data		= &sysctl_local_ports.range,
		.maxlen		= sizeof(sysctl_local_ports.range),
		.mode		= 0644,
		.proc_handler	= ipv4_local_port_range,
	},
	{
		.procname	= "ip_local_reserved_ports",
		.data		= NULL, /* initialized in sysctl_ipv4_init */
		.maxlen		= 65536,
		.mode		= 0644,
		.proc_handler	= proc_do_large_bitmap,
	},
	{
		.procname	= "igmp_max_memberships",
		.data		= &sysctl_igmp_max_memberships,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "igmp_max_msf",
		.data		= &sysctl_igmp_max_msf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "inet_peer_threshold",
		.data		= &inet_peer_threshold,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "inet_peer_minttl",
		.data		= &inet_peer_minttl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "inet_peer_maxttl",
		.data		= &inet_peer_maxttl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
<<<<<<< HEAD
		.procname	= "tcp_orphan_retries",
		.data		= &sysctl_tcp_orphan_retries,
=======
		.procname	= "tcp_fack",
		.data		= &sysctl_tcp_fack,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
		.procname	= "tcp_fack",
		.data		= &sysctl_tcp_fack,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_reordering",
		.data		= &sysctl_tcp_reordering,
=======
		.procname	= "tcp_recovery",
		.data		= &sysctl_tcp_recovery,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_max_reordering",
		.data		= &sysctl_tcp_max_reordering,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_dsack",
		.data		= &sysctl_tcp_dsack,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
=======
		.procname	= "tcp_mem",
		.maxlen		= sizeof(sysctl_tcp_mem),
		.data		= &sysctl_tcp_mem,
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
	},
	{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "tcp_wmem",
		.data		= &sysctl_tcp_wmem,
		.maxlen		= sizeof(sysctl_tcp_wmem),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
	},
	{
		.procname	= "tcp_rmem",
		.data		= &sysctl_tcp_rmem,
		.maxlen		= sizeof(sysctl_tcp_rmem),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
	},
	{
		.procname	= "tcp_app_win",
		.data		= &sysctl_tcp_app_win,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_adv_win_scale",
		.data		= &sysctl_tcp_adv_win_scale,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &tcp_adv_win_scale_min,
		.extra2		= &tcp_adv_win_scale_max,
	},
	{
		.procname	= "tcp_tw_reuse",
		.data		= &sysctl_tcp_tw_reuse,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_frto",
		.data		= &sysctl_tcp_frto,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
=======
		.procname	= "tcp_min_rtt_wlen",
		.data		= &sysctl_tcp_min_rtt_wlen,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &one_day_secs
	},
	{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "tcp_low_latency",
		.data		= &sysctl_tcp_low_latency,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_no_metrics_save",
		.data		= &sysctl_tcp_nometrics_save,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_moderate_rcvbuf",
		.data		= &sysctl_tcp_moderate_rcvbuf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_tso_win_divisor",
		.data		= &sysctl_tcp_tso_win_divisor,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_congestion_control",
		.mode		= 0644,
		.maxlen		= TCP_CA_NAME_MAX,
		.proc_handler	= proc_tcp_congestion_control,
	},
	{
<<<<<<< HEAD
		.procname	= "tcp_mtu_probing",
		.data		= &sysctl_tcp_mtu_probing,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_base_mss",
		.data		= &sysctl_tcp_base_mss,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "tcp_workaround_signed_windows",
		.data		= &sysctl_tcp_workaround_signed_windows,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_limit_output_bytes",
		.data		= &sysctl_tcp_limit_output_bytes,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_challenge_ack_limit",
		.data		= &sysctl_tcp_challenge_ack_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
<<<<<<< HEAD
#ifdef CONFIG_NET_DMA
	{
		.procname	= "tcp_dma_copybreak",
		.data		= &sysctl_tcp_dma_copybreak,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{
		.procname	= "tcp_slow_start_after_idle",
		.data		= &sysctl_tcp_slow_start_after_idle,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#ifdef CONFIG_NETLABEL
	{
		.procname	= "cipso_cache_enable",
		.data		= &cipso_v4_cache_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "cipso_cache_bucket_size",
		.data		= &cipso_v4_cache_bucketsize,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "cipso_rbm_optfmt",
		.data		= &cipso_v4_rbm_optfmt,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "cipso_rbm_strictvalid",
		.data		= &cipso_v4_rbm_strictvalid,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#endif /* CONFIG_NETLABEL */
	{
		.procname	= "tcp_available_congestion_control",
		.maxlen		= TCP_CA_BUF_MAX,
		.mode		= 0444,
		.proc_handler   = proc_tcp_available_congestion_control,
	},
	{
		.procname	= "tcp_allowed_congestion_control",
		.maxlen		= TCP_CA_BUF_MAX,
		.mode		= 0644,
		.proc_handler   = proc_allowed_congestion_control,
	},
	{
<<<<<<< HEAD
		.procname	= "tcp_max_ssthresh",
		.data		= &sysctl_tcp_max_ssthresh,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname       = "tcp_thin_linear_timeouts",
		.data           = &sysctl_tcp_thin_linear_timeouts,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec
	},
	{
		.procname       = "tcp_thin_dupack",
		.data           = &sysctl_tcp_thin_dupack,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec
	},
	{
		.procname	= "tcp_early_retrans",
		.data		= &sysctl_tcp_early_retrans,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &four,
	},
	{
		.procname	= "tcp_min_tso_segs",
		.data		= &sysctl_tcp_min_tso_segs,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
<<<<<<< HEAD
		.extra1		= &zero,
		.extra2		= &gso_max_segs,
	},
	{
=======
		.extra1		= &one,
		.extra2		= &gso_max_segs,
	},
	{
		.procname	= "tcp_pacing_ss_ratio",
		.data		= &sysctl_tcp_pacing_ss_ratio,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &thousand,
	},
	{
		.procname	= "tcp_pacing_ca_ratio",
		.data		= &sysctl_tcp_pacing_ca_ratio,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &thousand,
	},
	{
		.procname	= "tcp_autocorking",
		.data		= &sysctl_tcp_autocorking,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &one,
	},
	{
		.procname	= "tcp_invalid_ratelimit",
		.data		= &sysctl_tcp_invalid_ratelimit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_ms_jiffies,
	},
	{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname       = "tcp_default_init_rwnd",
		.data           = &sysctl_tcp_default_init_rwnd,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_tcp_default_init_rwnd
	},
	{
<<<<<<< HEAD
=======
		.procname	= "icmp_msgs_per_sec",
		.data		= &sysctl_icmp_msgs_per_sec,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
	},
	{
		.procname	= "icmp_msgs_burst",
		.data		= &sysctl_icmp_msgs_burst,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
	},
	{
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.procname	= "udp_mem",
		.data		= &sysctl_udp_mem,
		.maxlen		= sizeof(sysctl_udp_mem),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
	},
	{
		.procname	= "udp_rmem_min",
		.data		= &sysctl_udp_rmem_min,
		.maxlen		= sizeof(sysctl_udp_rmem_min),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one
	},
	{
		.procname	= "udp_wmem_min",
		.data		= &sysctl_udp_wmem_min,
		.maxlen		= sizeof(sysctl_udp_wmem_min),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one
	},
	{ }
};

static struct ctl_table ipv4_net_table[] = {
	{
		.procname	= "icmp_echo_ignore_all",
		.data		= &init_net.ipv4.sysctl_icmp_echo_ignore_all,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "icmp_echo_ignore_broadcasts",
		.data		= &init_net.ipv4.sysctl_icmp_echo_ignore_broadcasts,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "icmp_ignore_bogus_error_responses",
		.data		= &init_net.ipv4.sysctl_icmp_ignore_bogus_error_responses,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "icmp_errors_use_inbound_ifaddr",
		.data		= &init_net.ipv4.sysctl_icmp_errors_use_inbound_ifaddr,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "icmp_ratelimit",
		.data		= &init_net.ipv4.sysctl_icmp_ratelimit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_ms_jiffies,
	},
	{
		.procname	= "icmp_ratemask",
		.data		= &init_net.ipv4.sysctl_icmp_ratemask,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ping_group_range",
<<<<<<< HEAD
		.data		= &init_net.ipv4.sysctl_ping_group_range,
=======
		.data		= &init_net.ipv4.ping_group_range.range,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		.maxlen		= sizeof(gid_t)*2,
		.mode		= 0644,
		.proc_handler	= ipv4_ping_group_range,
	},
	{
		.procname	= "tcp_ecn",
		.data		= &init_net.ipv4.sysctl_tcp_ecn,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
<<<<<<< HEAD
		.procname	= "tcp_mem",
		.maxlen		= sizeof(init_net.ipv4.sysctl_tcp_mem),
		.mode		= 0644,
		.proc_handler	= ipv4_tcp_mem,
=======
		.procname	= "tcp_ecn_fallback",
		.data		= &init_net.ipv4.sysctl_tcp_ecn_fallback,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_dynaddr",
		.data		= &init_net.ipv4.sysctl_ip_dynaddr,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_early_demux",
		.data		= &init_net.ipv4.sysctl_ip_early_demux,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_default_ttl",
		.data		= &init_net.ipv4.sysctl_ip_default_ttl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &ip_ttl_min,
		.extra2		= &ip_ttl_max,
	},
	{
		.procname	= "ip_local_port_range",
		.maxlen		= sizeof(init_net.ipv4.ip_local_ports.range),
		.data		= &init_net.ipv4.ip_local_ports.range,
		.mode		= 0644,
		.proc_handler	= ipv4_local_port_range,
	},
	{
		.procname	= "ip_local_reserved_ports",
		.data		= &init_net.ipv4.sysctl_local_reserved_ports,
		.maxlen		= 65536,
		.mode		= 0644,
		.proc_handler	= proc_do_large_bitmap,
	},
	{
		.procname	= "ip_no_pmtu_disc",
		.data		= &init_net.ipv4.sysctl_ip_no_pmtu_disc,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ip_forward_use_pmtu",
		.data		= &init_net.ipv4.sysctl_ip_fwd_use_pmtu,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "ip_nonlocal_bind",
		.data		= &init_net.ipv4.sysctl_ip_nonlocal_bind,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	},
	{
		.procname	= "fwmark_reflect",
		.data		= &init_net.ipv4.sysctl_fwmark_reflect,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_fwmark_accept",
		.data		= &init_net.ipv4.sysctl_tcp_fwmark_accept,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
=======
#ifdef CONFIG_NET_L3_MASTER_DEV
	{
		.procname	= "tcp_l3mdev_accept",
		.data		= &init_net.ipv4.sysctl_tcp_l3mdev_accept,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &one,
	},
#endif
	{
		.procname	= "tcp_mtu_probing",
		.data		= &init_net.ipv4.sysctl_tcp_mtu_probing,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_base_mss",
		.data		= &init_net.ipv4.sysctl_tcp_base_mss,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_min_snd_mss",
		.data		= &init_net.ipv4.sysctl_tcp_min_snd_mss,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &tcp_min_snd_mss_min,
		.extra2		= &tcp_min_snd_mss_max,
	},
	{
		.procname	= "tcp_probe_threshold",
		.data		= &init_net.ipv4.sysctl_tcp_probe_threshold,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "tcp_probe_interval",
		.data		= &init_net.ipv4.sysctl_tcp_probe_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "igmp_link_local_mcast_reports",
		.data		= &init_net.ipv4.sysctl_igmp_llm_reports,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "igmp_max_memberships",
		.data		= &init_net.ipv4.sysctl_igmp_max_memberships,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "igmp_max_msf",
		.data		= &init_net.ipv4.sysctl_igmp_max_msf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#ifdef CONFIG_IP_MULTICAST
	{
		.procname	= "igmp_qrv",
		.data		= &init_net.ipv4.sysctl_igmp_qrv,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one
	},
#endif
	{
		.procname	= "tcp_keepalive_time",
		.data		= &init_net.ipv4.sysctl_tcp_keepalive_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "tcp_keepalive_probes",
		.data		= &init_net.ipv4.sysctl_tcp_keepalive_probes,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_keepalive_intvl",
		.data		= &init_net.ipv4.sysctl_tcp_keepalive_intvl,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "tcp_syn_retries",
		.data		= &init_net.ipv4.sysctl_tcp_syn_retries,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &tcp_syn_retries_min,
		.extra2		= &tcp_syn_retries_max
	},
	{
		.procname	= "tcp_synack_retries",
		.data		= &init_net.ipv4.sysctl_tcp_synack_retries,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#ifdef CONFIG_SYN_COOKIES
	{
		.procname	= "tcp_syncookies",
		.data		= &init_net.ipv4.sysctl_tcp_syncookies,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
#endif
	{
		.procname	= "tcp_reordering",
		.data		= &init_net.ipv4.sysctl_tcp_reordering,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_retries1",
		.data		= &init_net.ipv4.sysctl_tcp_retries1,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra2		= &tcp_retr1_max
	},
	{
		.procname	= "tcp_retries2",
		.data		= &init_net.ipv4.sysctl_tcp_retries2,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_orphan_retries",
		.data		= &init_net.ipv4.sysctl_tcp_orphan_retries,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "tcp_fin_timeout",
		.data		= &init_net.ipv4.sysctl_tcp_fin_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "tcp_notsent_lowat",
		.data		= &init_net.ipv4.sysctl_tcp_notsent_lowat,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_douintvec,
	},
#ifdef CONFIG_IP_ROUTE_MULTIPATH
	{
		.procname	= "fib_multipath_use_neigh",
		.data		= &init_net.ipv4.sysctl_fib_multipath_use_neigh,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &one,
	},
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	{ }
};

static __net_init int ipv4_sysctl_init_net(struct net *net)
{
	struct ctl_table *table;

	table = ipv4_net_table;
	if (!net_eq(net, &init_net)) {
<<<<<<< HEAD
		table = kmemdup(table, sizeof(ipv4_net_table), GFP_KERNEL);
		if (table == NULL)
			goto err_alloc;

		table[0].data =
			&net->ipv4.sysctl_icmp_echo_ignore_all;
		table[1].data =
			&net->ipv4.sysctl_icmp_echo_ignore_broadcasts;
		table[2].data =
			&net->ipv4.sysctl_icmp_ignore_bogus_error_responses;
		table[3].data =
			&net->ipv4.sysctl_icmp_errors_use_inbound_ifaddr;
		table[4].data =
			&net->ipv4.sysctl_icmp_ratelimit;
		table[5].data =
			&net->ipv4.sysctl_icmp_ratemask;
		table[6].data =
			&net->ipv4.sysctl_ping_group_range;
		table[7].data =
			&net->ipv4.sysctl_tcp_ecn;

		/* Don't export sysctls to unprivileged users */
		if (net->user_ns != &init_user_ns)
			table[0].procname = NULL;
	}

	/*
	 * Sane defaults - nobody may create ping sockets.
	 * Boot scripts should set this to distro-specific group.
	 */
	net->ipv4.sysctl_ping_group_range[0] = make_kgid(&init_user_ns, 1);
	net->ipv4.sysctl_ping_group_range[1] = make_kgid(&init_user_ns, 0);

	tcp_init_mem(net);

	net->ipv4.ipv4_hdr = register_net_sysctl(net, "net/ipv4", table);
	if (net->ipv4.ipv4_hdr == NULL)
		goto err_reg;

	return 0;

=======
		int i;

		table = kmemdup(table, sizeof(ipv4_net_table), GFP_KERNEL);
		if (!table)
			goto err_alloc;

		/* Update the variables to point into the current struct net */
		for (i = 0; i < ARRAY_SIZE(ipv4_net_table) - 1; i++)
			table[i].data += (void *)net - (void *)&init_net;
	}

	net->ipv4.ipv4_hdr = register_net_sysctl(net, "net/ipv4", table);
	if (!net->ipv4.ipv4_hdr)
		goto err_reg;

	net->ipv4.sysctl_local_reserved_ports = kzalloc(65536 / 8, GFP_KERNEL);
	if (!net->ipv4.sysctl_local_reserved_ports)
		goto err_ports;

	return 0;

err_ports:
	unregister_net_sysctl_table(net->ipv4.ipv4_hdr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
err_reg:
	if (!net_eq(net, &init_net))
		kfree(table);
err_alloc:
	return -ENOMEM;
}

static __net_exit void ipv4_sysctl_exit_net(struct net *net)
{
	struct ctl_table *table;

<<<<<<< HEAD
=======
	kfree(net->ipv4.sysctl_local_reserved_ports);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	table = net->ipv4.ipv4_hdr->ctl_table_arg;
	unregister_net_sysctl_table(net->ipv4.ipv4_hdr);
	kfree(table);
}

static __net_initdata struct pernet_operations ipv4_sysctl_ops = {
	.init = ipv4_sysctl_init_net,
	.exit = ipv4_sysctl_exit_net,
};

static __init int sysctl_ipv4_init(void)
{
	struct ctl_table_header *hdr;
<<<<<<< HEAD
	struct ctl_table *i;

	for (i = ipv4_table; i->procname; i++) {
		if (strcmp(i->procname, "ip_local_reserved_ports") == 0) {
			i->data = sysctl_local_reserved_ports;
			break;
		}
	}
	if (!i->procname)
		return -EINVAL;

	hdr = register_net_sysctl(&init_net, "net/ipv4", ipv4_table);
	if (hdr == NULL)
=======

	hdr = register_net_sysctl(&init_net, "net/ipv4", ipv4_table);
	if (!hdr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return -ENOMEM;

	if (register_pernet_subsys(&ipv4_sysctl_ops)) {
		unregister_net_sysctl_table(hdr);
		return -ENOMEM;
	}

	return 0;
}

__initcall(sysctl_ipv4_init);
