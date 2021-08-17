#ifndef __NET_GEN_STATS_H
#define __NET_GEN_STATS_H

#include <linux/gen_stats.h>
#include <linux/socket.h>
#include <linux/rtnetlink.h>
#include <linux/pkt_sched.h>

<<<<<<< HEAD
=======
struct gnet_stats_basic_cpu {
	struct gnet_stats_basic_packed bstats;
	struct u64_stats_sync syncp;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct gnet_dump {
	spinlock_t *      lock;
	struct sk_buff *  skb;
	struct nlattr *   tail;

	/* Backward compatibility */
	int               compat_tc_stats;
	int               compat_xstats;
<<<<<<< HEAD
=======
	int               padattr;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	void *            xstats;
	int               xstats_len;
	struct tc_stats   tc_stats;
};

<<<<<<< HEAD
extern int gnet_stats_start_copy(struct sk_buff *skb, int type,
				 spinlock_t *lock, struct gnet_dump *d);

extern int gnet_stats_start_copy_compat(struct sk_buff *skb, int type,
					int tc_stats_type,int xstats_type,
					spinlock_t *lock, struct gnet_dump *d);

extern int gnet_stats_copy_basic(struct gnet_dump *d,
				 struct gnet_stats_basic_packed *b);
extern int gnet_stats_copy_rate_est(struct gnet_dump *d,
				    const struct gnet_stats_basic_packed *b,
				    struct gnet_stats_rate_est *r);
extern int gnet_stats_copy_queue(struct gnet_dump *d,
				 struct gnet_stats_queue *q);
extern int gnet_stats_copy_app(struct gnet_dump *d, void *st, int len);

extern int gnet_stats_finish_copy(struct gnet_dump *d);

extern int gen_new_estimator(struct gnet_stats_basic_packed *bstats,
			     struct gnet_stats_rate_est *rate_est,
			     spinlock_t *stats_lock, struct nlattr *opt);
extern void gen_kill_estimator(struct gnet_stats_basic_packed *bstats,
			       struct gnet_stats_rate_est *rate_est);
extern int gen_replace_estimator(struct gnet_stats_basic_packed *bstats,
				 struct gnet_stats_rate_est *rate_est,
				 spinlock_t *stats_lock, struct nlattr *opt);
extern bool gen_estimator_active(const struct gnet_stats_basic_packed *bstats,
				 const struct gnet_stats_rate_est *rate_est);
=======
int gnet_stats_start_copy(struct sk_buff *skb, int type, spinlock_t *lock,
			  struct gnet_dump *d, int padattr);

int gnet_stats_start_copy_compat(struct sk_buff *skb, int type,
				 int tc_stats_type, int xstats_type,
				 spinlock_t *lock, struct gnet_dump *d,
				 int padattr);

int gnet_stats_copy_basic(const seqcount_t *running,
			  struct gnet_dump *d,
			  struct gnet_stats_basic_cpu __percpu *cpu,
			  struct gnet_stats_basic_packed *b);
void __gnet_stats_copy_basic(const seqcount_t *running,
			     struct gnet_stats_basic_packed *bstats,
			     struct gnet_stats_basic_cpu __percpu *cpu,
			     struct gnet_stats_basic_packed *b);
int gnet_stats_copy_rate_est(struct gnet_dump *d,
			     const struct gnet_stats_basic_packed *b,
			     struct gnet_stats_rate_est64 *r);
int gnet_stats_copy_queue(struct gnet_dump *d,
			  struct gnet_stats_queue __percpu *cpu_q,
			  struct gnet_stats_queue *q, __u32 qlen);
int gnet_stats_copy_app(struct gnet_dump *d, void *st, int len);

int gnet_stats_finish_copy(struct gnet_dump *d);

int gen_new_estimator(struct gnet_stats_basic_packed *bstats,
		      struct gnet_stats_basic_cpu __percpu *cpu_bstats,
		      struct gnet_stats_rate_est64 *rate_est,
		      spinlock_t *stats_lock,
		      seqcount_t *running, struct nlattr *opt);
void gen_kill_estimator(struct gnet_stats_basic_packed *bstats,
			struct gnet_stats_rate_est64 *rate_est);
int gen_replace_estimator(struct gnet_stats_basic_packed *bstats,
			  struct gnet_stats_basic_cpu __percpu *cpu_bstats,
			  struct gnet_stats_rate_est64 *rate_est,
			  spinlock_t *stats_lock,
			  seqcount_t *running, struct nlattr *opt);
bool gen_estimator_active(const struct gnet_stats_basic_packed *bstats,
			  const struct gnet_stats_rate_est64 *rate_est);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
