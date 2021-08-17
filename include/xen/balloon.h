/******************************************************************************
 * Xen balloon functionality
 */

#define RETRY_UNLIMITED	0

struct balloon_stats {
	/* We aim for 'current allocation' == 'target allocation'. */
	unsigned long current_pages;
	unsigned long target_pages;
<<<<<<< HEAD
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
=======
	unsigned long target_unpopulated;
	/* Number of pages in high- and low-memory balloons. */
	unsigned long balloon_low;
	unsigned long balloon_high;
	unsigned long total_pages;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long schedule_delay;
	unsigned long max_schedule_delay;
	unsigned long retry_count;
	unsigned long max_retry_count;
<<<<<<< HEAD
#ifdef CONFIG_XEN_BALLOON_MEMORY_HOTPLUG
	unsigned long hotplug_pages;
	unsigned long balloon_hotplug;
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

extern struct balloon_stats balloon_stats;

void balloon_set_new_target(unsigned long target);

<<<<<<< HEAD
int alloc_xenballooned_pages(int nr_pages, struct page **pages,
		bool highmem);
=======
int alloc_xenballooned_pages(int nr_pages, struct page **pages);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void free_xenballooned_pages(int nr_pages, struct page **pages);

struct device;
#ifdef CONFIG_XEN_SELFBALLOONING
extern int register_xen_selfballooning(struct device *dev);
#else
static inline int register_xen_selfballooning(struct device *dev)
{
	return -ENOSYS;
}
#endif
