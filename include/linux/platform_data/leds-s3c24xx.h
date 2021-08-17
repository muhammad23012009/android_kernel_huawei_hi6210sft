<<<<<<< HEAD
/* arch/arm/mach-s3c2410/include/mach/leds-gpio.h
 *
=======
/*
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (c) 2006 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX - LEDs GPIO connector
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

<<<<<<< HEAD
#ifndef __ASM_ARCH_LEDSGPIO_H
#define __ASM_ARCH_LEDSGPIO_H "leds-gpio.h"
=======
#ifndef __LEDS_S3C24XX_H
#define __LEDS_S3C24XX_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define S3C24XX_LEDF_ACTLOW	(1<<0)		/* LED is on when GPIO low */
#define S3C24XX_LEDF_TRISTATE	(1<<1)		/* tristate to turn off */

struct s3c24xx_led_platdata {
	unsigned int		 gpio;
	unsigned int		 flags;

	char			*name;
	char			*def_trigger;
};

<<<<<<< HEAD
#endif /* __ASM_ARCH_LEDSGPIO_H */
=======
#endif /* __LEDS_S3C24XX_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
