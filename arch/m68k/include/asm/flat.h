/*
<<<<<<< HEAD
 * include/asm-m68knommu/flat.h -- uClinux flat-format executables
=======
 * flat.h -- uClinux flat-format executables
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */

#ifndef __M68KNOMMU_FLAT_H__
#define __M68KNOMMU_FLAT_H__

#define	flat_argvp_envp_on_stack()		1
#define	flat_old_ram_flag(flags)		(flags)
#define	flat_reloc_valid(reloc, size)		((reloc) <= (size))
<<<<<<< HEAD
#define	flat_get_addr_from_rp(rp, relval, flags, p)	get_unaligned(rp)
#define	flat_put_addr_at_rp(rp, val, relval)	put_unaligned(val,rp)
=======
#define	flat_get_addr_from_rp(rp, relval, flags, p) \
	({ unsigned long __val; __get_user_unaligned(__val, rp); __val; })
#define	flat_put_addr_at_rp(rp, val, relval)	__put_user_unaligned(val, rp)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define	flat_get_relocate_addr(rel)		(rel)

static inline int flat_set_persistent(unsigned long relval,
				      unsigned long *persistent)
{
	return 0;
}

<<<<<<< HEAD
=======
#define FLAT_PLAT_INIT(regs) \
	do { \
		if (current->mm) \
			(regs)->d5 = current->mm->start_data; \
	} while (0)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __M68KNOMMU_FLAT_H__ */
