#ifndef _ASM_X86_VDSO_H
#define _ASM_X86_VDSO_H

<<<<<<< HEAD
#if defined CONFIG_X86_32 || defined CONFIG_COMPAT
extern const char VDSO32_PRELINK[];

/*
 * Given a pointer to the vDSO image, find the pointer to VDSO32_name
 * as that symbol is defined in the vDSO sources or linker script.
 */
#define VDSO32_SYMBOL(base, name)					\
({									\
	extern const char VDSO32_##name[];				\
	(void __user *)(VDSO32_##name - VDSO32_PRELINK +		\
			(unsigned long)(base));				\
})
#endif

/*
 * These symbols are defined with the addresses in the vsyscall page.
 * See vsyscall-sigreturn.S.
 */
extern void __user __kernel_sigreturn;
extern void __user __kernel_rt_sigreturn;

/*
 * These symbols are defined by vdso32.S to mark the bounds
 * of the ELF DSO images included therein.
 */
extern const char vdso32_int80_start, vdso32_int80_end;
extern const char vdso32_syscall_start, vdso32_syscall_end;
extern const char vdso32_sysenter_start, vdso32_sysenter_end;
=======
#include <asm/page_types.h>
#include <linux/linkage.h>
#include <linux/init.h>

#ifndef __ASSEMBLER__

#include <linux/mm_types.h>

struct vdso_image {
	void *data;
	unsigned long size;   /* Always a multiple of PAGE_SIZE */

	unsigned long alt, alt_len;

	long sym_vvar_start;  /* Negative offset to the vvar area */

	long sym_vvar_page;
	long sym_hpet_page;
	long sym_pvclock_page;
	long sym_VDSO32_NOTE_MASK;
	long sym___kernel_sigreturn;
	long sym___kernel_rt_sigreturn;
	long sym___kernel_vsyscall;
	long sym_int80_landing_pad;
};

#ifdef CONFIG_X86_64
extern const struct vdso_image vdso_image_64;
#endif

#ifdef CONFIG_X86_X32
extern const struct vdso_image vdso_image_x32;
#endif

#if defined CONFIG_X86_32 || defined CONFIG_COMPAT
extern const struct vdso_image vdso_image_32;
#endif

extern void __init init_vdso_image(const struct vdso_image *image);

extern int map_vdso_once(const struct vdso_image *image, unsigned long addr);

#endif /* __ASSEMBLER__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_X86_VDSO_H */
