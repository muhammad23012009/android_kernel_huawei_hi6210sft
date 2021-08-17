#ifndef __NETNS_X_TABLES_H
#define __NETNS_X_TABLES_H

#include <linux/list.h>
<<<<<<< HEAD
#include <linux/netfilter.h>
=======
#include <linux/netfilter_defs.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct ebt_table;

struct netns_xt {
	struct list_head tables[NFPROTO_NUMPROTO];
	bool notrack_deprecated_warning;
<<<<<<< HEAD
=======
	bool clusterip_deprecated_warning;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#if defined(CONFIG_BRIDGE_NF_EBTABLES) || \
    defined(CONFIG_BRIDGE_NF_EBTABLES_MODULE)
	struct ebt_table *broute_table;
	struct ebt_table *frame_filter;
	struct ebt_table *frame_nat;
#endif
};
#endif
