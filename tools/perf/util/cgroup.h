#ifndef __CGROUP_H__
#define __CGROUP_H__

<<<<<<< HEAD
=======
#include <linux/atomic.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct option;

struct cgroup_sel {
	char *name;
	int fd;
<<<<<<< HEAD
	int refcnt;
=======
	atomic_t refcnt;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};


extern int nr_cgroups; /* number of explicit cgroups defined */
<<<<<<< HEAD
extern void close_cgroup(struct cgroup_sel *cgrp);
extern int parse_cgroups(const struct option *opt, const char *str, int unset);
=======
void close_cgroup(struct cgroup_sel *cgrp);
int parse_cgroups(const struct option *opt, const char *str, int unset);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __CGROUP_H__ */
