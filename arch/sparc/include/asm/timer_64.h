/* timer.h: System timer definitions for sun5.
 *
 * Copyright (C) 1997, 2008 David S. Miller (davem@davemloft.net)
 */

#ifndef _SPARC64_TIMER_H
#define _SPARC64_TIMER_H

#include <linux/types.h>
#include <linux/init.h>

struct sparc64_tick_ops {
	unsigned long long (*get_tick)(void);
	int (*add_compare)(unsigned long);
	unsigned long softint_mask;
	void (*disable_irq)(void);

	void (*init_tick)(void);
	unsigned long (*add_tick)(unsigned long);

	char *name;
};

extern struct sparc64_tick_ops *tick_ops;

<<<<<<< HEAD
extern unsigned long sparc64_get_clock_tick(unsigned int cpu);
extern void setup_sparc64_timer(void);
extern void __init time_init(void);
=======
unsigned long sparc64_get_clock_tick(unsigned int cpu);
void setup_sparc64_timer(void);
void __init time_init(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _SPARC64_TIMER_H */
