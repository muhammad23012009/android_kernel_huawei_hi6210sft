#ifndef _ASM_POWERPC_BYTEORDER_H
#define _ASM_POWERPC_BYTEORDER_H

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
<<<<<<< HEAD
#include <linux/byteorder/big_endian.h>
=======
#ifdef __LITTLE_ENDIAN__
#include <linux/byteorder/little_endian.h>
#else
#include <linux/byteorder/big_endian.h>
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_POWERPC_BYTEORDER_H */
