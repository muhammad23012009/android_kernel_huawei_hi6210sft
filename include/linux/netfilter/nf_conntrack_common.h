#ifndef _NF_CONNTRACK_COMMON_H
#define _NF_CONNTRACK_COMMON_H

#include <uapi/linux/netfilter/nf_conntrack_common.h>

struct ip_conntrack_stat {
<<<<<<< HEAD
	unsigned int searched;
	unsigned int found;
	unsigned int new;
	unsigned int invalid;
	unsigned int ignore;
	unsigned int delete;
	unsigned int delete_list;
=======
	unsigned int found;
	unsigned int invalid;
	unsigned int ignore;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned int insert;
	unsigned int insert_failed;
	unsigned int drop;
	unsigned int early_drop;
	unsigned int error;
	unsigned int expect_new;
	unsigned int expect_create;
	unsigned int expect_delete;
	unsigned int search_restart;
};

/* call to create an explicit dependency on nf_conntrack. */
<<<<<<< HEAD
extern void need_conntrack(void);
=======
void need_conntrack(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _NF_CONNTRACK_COMMON_H */
