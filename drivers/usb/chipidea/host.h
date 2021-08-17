#ifndef __DRIVERS_USB_CHIPIDEA_HOST_H
#define __DRIVERS_USB_CHIPIDEA_HOST_H

#ifdef CONFIG_USB_CHIPIDEA_HOST

<<<<<<< HEAD
int ci_hdrc_host_init(struct ci13xxx *ci);

#else

static inline int ci_hdrc_host_init(struct ci13xxx *ci)
=======
int ci_hdrc_host_init(struct ci_hdrc *ci);
void ci_hdrc_host_destroy(struct ci_hdrc *ci);
void ci_hdrc_host_driver_init(void);

#else

static inline int ci_hdrc_host_init(struct ci_hdrc *ci)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return -ENXIO;
}

<<<<<<< HEAD
=======
static inline void ci_hdrc_host_destroy(struct ci_hdrc *ci)
{

}

static void ci_hdrc_host_driver_init(void)
{

}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif /* __DRIVERS_USB_CHIPIDEA_HOST_H */
