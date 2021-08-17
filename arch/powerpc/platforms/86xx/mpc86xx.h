/*
 * Copyright 2006 Freescale Semiconductor Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifndef __MPC86XX_H__
#define __MPC86XX_H__

/*
 * Declaration for the various functions exported by the
 * mpc86xx_* files. Mostly for use by mpc86xx_setup().
 */

extern void mpc86xx_smp_init(void);
extern void mpc86xx_init_irq(void);
<<<<<<< HEAD
=======
extern long mpc86xx_time_init(void);
extern int mpc86xx_common_publish_devices(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif	/* __MPC86XX_H__ */
