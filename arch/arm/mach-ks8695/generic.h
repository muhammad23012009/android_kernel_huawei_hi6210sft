/*
 * arch/arm/mach-ks8695/generic.h
 *
 * Copyright (C) 2006 Ben Dooks <ben@simtec.co.uk>
 * Copyright (C) 2006 Simtec Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/

extern __init void ks8695_map_io(void);
extern __init void ks8695_init_irq(void);
<<<<<<< HEAD
extern void ks8695_restart(char, const char *);
=======
extern void ks8695_restart(enum reboot_mode, const char *);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void ks8695_timer_init(void);
