<<<<<<< HEAD
/* linux/arch/arm/plat-s3c/include/plat/hwmon.h
 *
=======
/*
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright 2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C - HWMon interface for ADC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

<<<<<<< HEAD
#ifndef __ASM_ARCH_ADC_HWMON_H
#define __ASM_ARCH_ADC_HWMON_H __FILE__
=======
#ifndef __HWMON_S3C_H__
#define __HWMON_S3C_H__
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/**
 * s3c_hwmon_chcfg - channel configuration
 * @name: The name to give this channel.
 * @mult: Multiply the ADC value read by this.
 * @div: Divide the value from the ADC by this.
 *
 * The value read from the ADC is converted to a value that
 * hwmon expects (mV) by result = (value_read * @mult) / @div.
 */
struct s3c_hwmon_chcfg {
	const char	*name;
	unsigned int	mult;
	unsigned int	div;
};

/**
 * s3c_hwmon_pdata - HWMON platform data
 * @in: One configuration for each possible channel used.
 */
struct s3c_hwmon_pdata {
	struct s3c_hwmon_chcfg	*in[8];
};

/**
 * s3c_hwmon_set_platdata - Set platform data for S3C HWMON device
 * @pd: Platform data to register to device.
 *
 * Register the given platform data for use with the S3C HWMON device.
 * The call will copy the platform data, so the board definitions can
 * make the structure itself __initdata.
 */
extern void __init s3c_hwmon_set_platdata(struct s3c_hwmon_pdata *pd);

<<<<<<< HEAD
#endif /* __ASM_ARCH_ADC_HWMON_H */

=======
#endif /* __HWMON_S3C_H__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
