#ifndef __LINUX_I2C_TSC2007_H
#define __LINUX_I2C_TSC2007_H

/* linux/i2c/tsc2007.h */

struct tsc2007_platform_data {
	u16	model;				/* 2007. */
	u16	x_plate_ohms;	/* must be non-zero value */
	u16	max_rt; /* max. resistance above which samples are ignored */
<<<<<<< HEAD
	unsigned long poll_delay; /* delay (in ms) after pen-down event
				     before polling starts */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	unsigned long poll_period; /* time (in ms) between samples */
	int	fuzzx; /* fuzz factor for X, Y and pressure axes */
	int	fuzzy;
	int	fuzzz;

<<<<<<< HEAD
	int	(*get_pendown_state)(void);
	void	(*clear_penirq)(void);		/* If needed, clear 2nd level
						   interrupt source */
=======
	int	(*get_pendown_state)(struct device *);
	/* If needed, clear 2nd level interrupt source */
	void	(*clear_penirq)(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	int	(*init_platform_hw)(void);
	void	(*exit_platform_hw)(void);
};

#endif
