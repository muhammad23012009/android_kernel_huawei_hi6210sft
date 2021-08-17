#ifndef _H8300_SIGNAL_H
#define _H8300_SIGNAL_H

#include <uapi/asm/signal.h>

/* Most things should be clean enough to redefine this at will, if care
   is taken to make libc match.  */

#define _NSIG		64
#define _NSIG_BPW	32
#define _NSIG_WORDS	(_NSIG / _NSIG_BPW)

typedef unsigned long old_sigset_t;		/* at least 32 bits */

typedef struct {
	unsigned long sig[_NSIG_WORDS];
} sigset_t;

#define __ARCH_HAS_SA_RESTORER
<<<<<<< HEAD

#include <asm/sigcontext.h>
#undef __HAVE_ARCH_SIG_BITOPS
=======
#include <asm/sigcontext.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _H8300_SIGNAL_H */
