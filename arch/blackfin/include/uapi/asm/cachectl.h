/*
 * based on the mips/cachectl.h
 *
 * Copyright 2010 Analog Devices Inc.
 * Copyright (C) 1994, 1995, 1996 by Ralf Baechle
 *
 * Licensed under the GPL-2 or later.
 */

<<<<<<< HEAD
#ifndef	_ASM_CACHECTL
#define	_ASM_CACHECTL
=======
#ifndef _UAPI_ASM_CACHECTL
#define _UAPI_ASM_CACHECTL
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Options for cacheflush system call
 */
#define	ICACHE	(1<<0)		/* flush instruction cache        */
#define	DCACHE	(1<<1)		/* writeback and flush data cache */
#define	BCACHE	(ICACHE|DCACHE)	/* flush both caches              */

<<<<<<< HEAD
#endif	/* _ASM_CACHECTL */
=======
#endif /* _UAPI_ASM_CACHECTL */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
