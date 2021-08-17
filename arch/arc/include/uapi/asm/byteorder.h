/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARC_BYTEORDER_H
#define __ASM_ARC_BYTEORDER_H

<<<<<<< HEAD
#ifdef CONFIG_CPU_BIG_ENDIAN
=======
#ifdef __BIG_ENDIAN__
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/byteorder/big_endian.h>
#else
#include <linux/byteorder/little_endian.h>
#endif

#endif /* ASM_ARC_BYTEORDER_H */
