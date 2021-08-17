#ifndef _UAPI_ASM_ALPHA_PARAM_H
#define _UAPI_ASM_ALPHA_PARAM_H

<<<<<<< HEAD
/* ??? Gross.  I don't want to parameterize this, and supposedly the
   hardware ignores reprogramming.  We also need userland buy-in to the 
   change in HZ, since this is visible in the wait4 resources etc.  */

#ifndef __KERNEL__
#define HZ		1024
#endif
=======
#define HZ		1024
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define EXEC_PAGESIZE	8192

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif

#define MAXHOSTNAMELEN	64	/* max length of hostname */

<<<<<<< HEAD

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _UAPI_ASM_ALPHA_PARAM_H */
