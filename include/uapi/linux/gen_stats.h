#ifndef __LINUX_GEN_STATS_H
#define __LINUX_GEN_STATS_H

#include <linux/types.h>

enum {
	TCA_STATS_UNSPEC,
	TCA_STATS_BASIC,
	TCA_STATS_RATE_EST,
	TCA_STATS_QUEUE,
	TCA_STATS_APP,
<<<<<<< HEAD
=======
	TCA_STATS_RATE_EST64,
	TCA_STATS_PAD,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__TCA_STATS_MAX,
};
#define TCA_STATS_MAX (__TCA_STATS_MAX - 1)

/**
 * struct gnet_stats_basic - byte/packet throughput statistics
 * @bytes: number of seen bytes
 * @packets: number of seen packets
 */
struct gnet_stats_basic {
	__u64	bytes;
	__u32	packets;
};
struct gnet_stats_basic_packed {
	__u64	bytes;
	__u32	packets;
} __attribute__ ((packed));

/**
 * struct gnet_stats_rate_est - rate estimator
 * @bps: current byte rate
 * @pps: current packet rate
 */
struct gnet_stats_rate_est {
	__u32	bps;
	__u32	pps;
};

/**
<<<<<<< HEAD
=======
 * struct gnet_stats_rate_est64 - rate estimator
 * @bps: current byte rate
 * @pps: current packet rate
 */
struct gnet_stats_rate_est64 {
	__u64	bps;
	__u64	pps;
};

/**
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * struct gnet_stats_queue - queuing statistics
 * @qlen: queue length
 * @backlog: backlog size of queue
 * @drops: number of dropped packets
 * @requeues: number of requeues
 * @overlimits: number of enqueues over the limit
 */
struct gnet_stats_queue {
	__u32	qlen;
	__u32	backlog;
	__u32	drops;
	__u32	requeues;
	__u32	overlimits;
};

/**
 * struct gnet_estimator - rate estimator configuration
 * @interval: sampling period
 * @ewma_log: the log of measurement window weight
 */
struct gnet_estimator {
	signed char	interval;
	unsigned char	ewma_log;
};


#endif /* __LINUX_GEN_STATS_H */
