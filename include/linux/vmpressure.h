#ifndef __LINUX_VMPRESSURE_H
#define __LINUX_VMPRESSURE_H

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include <linux/cgroup.h>
<<<<<<< HEAD
=======
#include <linux/eventfd.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

struct vmpressure {
	unsigned long scanned;
	unsigned long reclaimed;
<<<<<<< HEAD
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	struct mutex sr_lock;
=======

	unsigned long tree_scanned;
	unsigned long tree_reclaimed;
	/* The lock is used to keep the scanned/reclaimed above in sync. */
	struct spinlock sr_lock;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/* The list of vmpressure_event structs. */
	struct list_head events;
	/* Have to grab the lock on events traversal or modifications. */
	struct mutex events_lock;

	struct work_struct work;
};

struct mem_cgroup;

#ifdef CONFIG_MEMCG
<<<<<<< HEAD
extern void vmpressure(gfp_t gfp, struct mem_cgroup *memcg,
=======
extern void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		       unsigned long scanned, unsigned long reclaimed);
extern void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg, int prio);

extern void vmpressure_init(struct vmpressure *vmpr);
<<<<<<< HEAD
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct cgroup_subsys_state *vmpressure_to_css(struct vmpressure *vmpr);
extern struct vmpressure *css_to_vmpressure(struct cgroup_subsys_state *css);
extern int vmpressure_register_event(struct cgroup *cg, struct cftype *cft,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct cgroup *cg, struct cftype *cft,
					struct eventfd_ctx *eventfd);
#else
static inline void vmpressure(gfp_t gfp, struct mem_cgroup *memcg,
=======
extern void vmpressure_cleanup(struct vmpressure *vmpr);
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct cgroup_subsys_state *vmpressure_to_css(struct vmpressure *vmpr);
extern int vmpressure_register_event(struct mem_cgroup *memcg,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct mem_cgroup *memcg,
					struct eventfd_ctx *eventfd);
#else
static inline void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			      unsigned long scanned, unsigned long reclaimed) {}
static inline void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg,
				   int prio) {}
#endif /* CONFIG_MEMCG */
#endif /* __LINUX_VMPRESSURE_H */
