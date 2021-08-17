/*
 * Copyright 2004-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

<<<<<<< HEAD
#ifndef _BFIN_FCNTL_H
#define _BFIN_FCNTL_H
=======
#ifndef _UAPI_BFIN_FCNTL_H
#define _UAPI_BFIN_FCNTL_H
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define O_DIRECTORY	 040000	/* must be a directory */
#define O_NOFOLLOW	0100000	/* don't follow links */
#define O_DIRECT	0200000	/* direct disk access hint - currently ignored */
#define O_LARGEFILE	0400000

#include <asm-generic/fcntl.h>

<<<<<<< HEAD
#endif
=======
#endif /* _UAPI_BFIN_FCNTL_H */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
