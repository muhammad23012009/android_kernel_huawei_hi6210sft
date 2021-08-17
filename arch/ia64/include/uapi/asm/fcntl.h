#ifndef _ASM_IA64_FCNTL_H
#define _ASM_IA64_FCNTL_H
/*
 * Modified 1998-2000
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co.
 */

#define force_o_largefile()	\
		(personality(current->personality) != PER_LINUX32)

<<<<<<< HEAD
=======
#include <linux/personality.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/fcntl.h>

#endif /* _ASM_IA64_FCNTL_H */
