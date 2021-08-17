/*
 * Transactional memory support routines to reclaim and recheckpoint
 * transactional process state.
 *
 * Copyright 2012 Matt Evans & Michael Neuling, IBM Corporation.
 */

#include <uapi/asm/tm.h>

<<<<<<< HEAD
#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
extern void do_load_up_transact_fpu(struct thread_struct *thread);
extern void do_load_up_transact_altivec(struct thread_struct *thread);
#endif
=======
#ifndef __ASSEMBLY__
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern void tm_enable(void);
extern void tm_reclaim(struct thread_struct *thread,
		       unsigned long orig_msr, uint8_t cause);
<<<<<<< HEAD
=======
extern void tm_reclaim_current(uint8_t cause);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern void tm_recheckpoint(struct thread_struct *thread,
			    unsigned long orig_msr);
extern void tm_abort(uint8_t cause);
extern void tm_save_sprs(struct thread_struct *thread);
extern void tm_restore_sprs(struct thread_struct *thread);
<<<<<<< HEAD
=======

#endif /* __ASSEMBLY__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
