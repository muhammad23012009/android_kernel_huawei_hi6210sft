#ifndef _LINUX_MMAN_H
#define _LINUX_MMAN_H

#include <linux/mm.h>
#include <linux/percpu_counter.h>

#include <linux/atomic.h>
#include <uapi/linux/mman.h>

extern int sysctl_overcommit_memory;
extern int sysctl_overcommit_ratio;
<<<<<<< HEAD
extern struct percpu_counter vm_committed_as;

=======
extern unsigned long sysctl_overcommit_kbytes;
extern struct percpu_counter vm_committed_as;

#ifdef CONFIG_SMP
extern s32 vm_committed_as_batch;
#else
#define vm_committed_as_batch 0
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
unsigned long vm_memory_committed(void);

static inline void vm_acct_memory(long pages)
{
<<<<<<< HEAD
	percpu_counter_add(&vm_committed_as, pages);
=======
	__percpu_counter_add(&vm_committed_as, pages, vm_committed_as_batch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void vm_unacct_memory(long pages)
{
	vm_acct_memory(-pages);
}

/*
 * Allow architectures to handle additional protection bits
 */

#ifndef arch_calc_vm_prot_bits
<<<<<<< HEAD
#define arch_calc_vm_prot_bits(prot) 0
=======
#define arch_calc_vm_prot_bits(prot, pkey) 0
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#ifndef arch_vm_get_page_prot
#define arch_vm_get_page_prot(vm_flags) __pgprot(0)
#endif

#ifndef arch_validate_prot
/*
 * This is called from mprotect().  PROT_GROWSDOWN and PROT_GROWSUP have
 * already been masked out.
 *
 * Returns true if the prot flags are valid
 */
<<<<<<< HEAD
static inline int arch_validate_prot(unsigned long prot)
=======
static inline bool arch_validate_prot(unsigned long prot)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return (prot & ~(PROT_READ | PROT_WRITE | PROT_EXEC | PROT_SEM)) == 0;
}
#define arch_validate_prot arch_validate_prot
#endif

/*
 * Optimisation macro.  It is equivalent to:
 *      (x & bit1) ? bit2 : 0
 * but this version is faster.
 * ("bit1" and "bit2" must be single bits)
 */
#define _calc_vm_trans(x, bit1, bit2) \
<<<<<<< HEAD
  ((bit1) <= (bit2) ? ((x) & (bit1)) * ((bit2) / (bit1)) \
   : ((x) & (bit1)) / ((bit1) / (bit2)))
=======
  ((!(bit1) || !(bit2)) ? 0 : \
  ((bit1) <= (bit2) ? ((x) & (bit1)) * ((bit2) / (bit1)) \
   : ((x) & (bit1)) / ((bit1) / (bit2))))
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * Combine the mmap "prot" argument into "vm_flags" used internally.
 */
static inline unsigned long
<<<<<<< HEAD
calc_vm_prot_bits(unsigned long prot)
=======
calc_vm_prot_bits(unsigned long prot, unsigned long pkey)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return _calc_vm_trans(prot, PROT_READ,  VM_READ ) |
	       _calc_vm_trans(prot, PROT_WRITE, VM_WRITE) |
	       _calc_vm_trans(prot, PROT_EXEC,  VM_EXEC) |
<<<<<<< HEAD
	       arch_calc_vm_prot_bits(prot);
=======
	       arch_calc_vm_prot_bits(prot, pkey);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * Combine the mmap "flags" argument into "vm_flags" used internally.
 */
static inline unsigned long
calc_vm_flag_bits(unsigned long flags)
{
	return _calc_vm_trans(flags, MAP_GROWSDOWN,  VM_GROWSDOWN ) |
	       _calc_vm_trans(flags, MAP_DENYWRITE,  VM_DENYWRITE ) |
	       _calc_vm_trans(flags, MAP_LOCKED,     VM_LOCKED    );
}
<<<<<<< HEAD
=======

unsigned long vm_commit_limit(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _LINUX_MMAN_H */
