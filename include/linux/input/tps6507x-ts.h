/* linux/i2c/tps6507x-ts.h
 *
 * Functions to access TPS65070 touch screen chip.
 *
 * Copyright (c) 2009 RidgeRun (todd.fischer@ridgerun.com)
 *
 *
 *  For licencing details see kernel-base/COPYING
 */

#ifndef __LINUX_I2C_TPS6507X_TS_H
#define __LINUX_I2C_TPS6507X_TS_H

/* Board specific touch screen initial values */
struct touchscreen_init_data {
	int	poll_period;	/* ms */
<<<<<<< HEAD
	int	vref;		/* non-zero to leave vref on */
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__u16	min_pressure;	/* min reading to be treated as a touch */
	__u16	vendor;
	__u16	product;
	__u16	version;
};

#endif /*  __LINUX_I2C_TPS6507X_TS_H */
