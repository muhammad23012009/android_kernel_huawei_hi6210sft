/*
 * Copyright (C) 2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _ASM_ARC_SERIAL_H
#define _ASM_ARC_SERIAL_H

/*
<<<<<<< HEAD
 * early-8250 requires BASE_BAUD to be defined and includes this header.
 * We put in a typical value:
 * 	(core clk / 16) - i.e. UART samples 16 times per sec.
 * Athough in multi-platform-image this might not work, specially if the
 * clk driving the UART is different.
 * We can't use DeviceTree as this is typically for early serial.
 */

#include <asm/clk.h>

#define BASE_BAUD	(arc_get_core_freq() / 16)

/*
 * This is definitely going to break early 8250 consoles on multi-platform
 * images but hey, it won't add any code complexity for a debug feature of
 * one broken driver.
 */
#ifdef CONFIG_ARC_PLAT_TB10X
#undef BASE_BAUD
#define BASE_BAUD	(arc_get_core_freq() / 16 / 3)
#endif
=======
 * early 8250 (now earlycon) requires BASE_BAUD to be defined in this header.
 * However to still determine it dynamically (for multi-platform images)
 * we do this in a helper by parsing the FDT early
 */

extern unsigned int __init arc_early_base_baud(void);

#define BASE_BAUD	arc_early_base_baud()
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_ARC_SERIAL_H */
