/*
 *  S390 version
 *
 *  Derived from "include/asm-i386/ucontext.h"
 */

#ifndef _ASM_S390_UCONTEXT_H
#define _ASM_S390_UCONTEXT_H

<<<<<<< HEAD
#define UC_EXTENDED	0x00000001

#ifndef __s390x__

=======
#define UC_GPRS_HIGH	1	/* uc_mcontext_ext has valid high gprs */
#define UC_VXRS		2	/* uc_mcontext_ext has valid vector regs */

/*
 * The struct ucontext_extended describes how the registers are stored
 * on a rt signal frame. Please note that the structure is not fixed,
 * if new CPU registers are added to the user state the size of the
 * struct ucontext_extended will increase.
 */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct ucontext_extended {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	_sigregs	  uc_mcontext;
<<<<<<< HEAD
	unsigned long	  uc_sigmask[2];
	unsigned long	  uc_gprs_high[16];
};

#endif

=======
	sigset_t	  uc_sigmask;
	/* Allow for uc_sigmask growth.  Glibc uses a 1024-bit sigset_t.  */
	unsigned char	  __unused[128 - sizeof(sigset_t)];
	_sigregs_ext	  uc_mcontext_ext;
};

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	_sigregs          uc_mcontext;
<<<<<<< HEAD
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
=======
	sigset_t	  uc_sigmask;
	/* Allow for uc_sigmask growth.  Glibc uses a 1024-bit sigset_t.  */
	unsigned char	  __unused[128 - sizeof(sigset_t)];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* !_ASM_S390_UCONTEXT_H */
