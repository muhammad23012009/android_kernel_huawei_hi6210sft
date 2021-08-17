<<<<<<< HEAD
/* arch/arm/plat-samsung/include/plat/ts.h
 *
=======
/*
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * Copyright (c) 2005 Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

<<<<<<< HEAD
#ifndef __ASM_ARM_TS_H
#define __ASM_ARM_TS_H

struct s3c2410_ts_mach_info {
       int             delay;
       int             presc;
       int             oversampling_shift;
	void    (*cfg_gpio)(struct platform_device *dev);
};

extern void s3c24xx_ts_set_platdata(struct s3c2410_ts_mach_info *);
=======
#ifndef __TOUCHSCREEN_S3C2410_H
#define __TOUCHSCREEN_S3C2410_H

struct s3c2410_ts_mach_info {
	int delay;
	int presc;
	int oversampling_shift;
	void (*cfg_gpio)(struct platform_device *dev);
};

extern void s3c24xx_ts_set_platdata(struct s3c2410_ts_mach_info *);
extern void s3c64xx_ts_set_platdata(struct s3c2410_ts_mach_info *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* defined by architecture to configure gpio */
extern void s3c24xx_ts_cfg_gpio(struct platform_device *dev);

<<<<<<< HEAD
#endif /* __ASM_ARM_TS_H */
=======
#endif /*__TOUCHSCREEN_S3C2410_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
