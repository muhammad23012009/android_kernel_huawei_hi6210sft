#ifndef _XT_RATEEST_H
#define _XT_RATEEST_H

struct xt_rateest {
	/* keep lock and bstats on same cache line to speedup xt_rateest_tg() */
	struct gnet_stats_basic_packed	bstats;
	spinlock_t			lock;
	/* keep rstats and lock on same cache line to speedup xt_rateest_mt() */
<<<<<<< HEAD
	struct gnet_stats_rate_est	rstats;
=======
	struct gnet_stats_rate_est64	rstats;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* following fields not accessed in hot path */
	struct hlist_node		list;
	char				name[IFNAMSIZ];
	unsigned int			refcnt;
	struct gnet_estimator		params;
	struct rcu_head			rcu;
};

<<<<<<< HEAD
extern struct xt_rateest *xt_rateest_lookup(const char *name);
extern void xt_rateest_put(struct xt_rateest *est);
=======
struct xt_rateest *xt_rateest_lookup(const char *name);
void xt_rateest_put(struct xt_rateest *est);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _XT_RATEEST_H */
