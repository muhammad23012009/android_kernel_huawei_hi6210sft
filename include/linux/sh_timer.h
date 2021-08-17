#ifndef __SH_TIMER_H__
#define __SH_TIMER_H__

struct sh_timer_config {
<<<<<<< HEAD
	char *name;
	long channel_offset;
	int timer_bit;
	unsigned long clockevent_rating;
	unsigned long clocksource_rating;
=======
	unsigned int channels_mask;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* __SH_TIMER_H__ */
