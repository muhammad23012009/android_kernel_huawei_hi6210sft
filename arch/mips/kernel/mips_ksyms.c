/*
 * Export MIPS-specific functions needed for loadable modules.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 97, 98, 99, 2000, 01, 03, 04, 05, 12 by Ralf Baechle
 * Copyright (C) 1999, 2000, 01 Silicon Graphics, Inc.
 */
#include <linux/interrupt.h>
#include <linux/export.h>
#include <asm/checksum.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <asm/ftrace.h>
#include <asm/fpu.h>
<<<<<<< HEAD

extern void *__bzero(void *__s, size_t __count);
=======
#include <asm/msa.h>

extern void *__bzero_kernel(void *__s, size_t __count);
extern void *__bzero(void *__s, size_t __count);
extern long __strncpy_from_kernel_nocheck_asm(char *__to,
					      const char *__from, long __len);
extern long __strncpy_from_kernel_asm(char *__to, const char *__from,
				      long __len);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern long __strncpy_from_user_nocheck_asm(char *__to,
					    const char *__from, long __len);
extern long __strncpy_from_user_asm(char *__to, const char *__from,
				    long __len);
<<<<<<< HEAD
extern long __strlen_user_nocheck_asm(const char *s);
extern long __strlen_user_asm(const char *s);
=======
extern long __strlen_kernel_asm(const char *s);
extern long __strlen_user_asm(const char *s);
extern long __strnlen_kernel_nocheck_asm(const char *s);
extern long __strnlen_kernel_asm(const char *s);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern long __strnlen_user_nocheck_asm(const char *s);
extern long __strnlen_user_asm(const char *s);

/*
 * Core architecture code
 */
<<<<<<< HEAD
#ifdef CONFIG_CPU_R4K_FPU
EXPORT_SYMBOL_GPL(_save_fp);
=======
EXPORT_SYMBOL_GPL(_save_fp);
#ifdef CONFIG_CPU_HAS_MSA
EXPORT_SYMBOL_GPL(_save_msa);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

/*
 * String functions
 */
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);

/*
 * Functions that operate on entire pages.  Mostly used by memory management.
 */
EXPORT_SYMBOL(clear_page);
EXPORT_SYMBOL(copy_page);

/*
 * Userspace access stuff.
 */
EXPORT_SYMBOL(__copy_user);
EXPORT_SYMBOL(__copy_user_inatomic);
<<<<<<< HEAD
EXPORT_SYMBOL(__bzero);
EXPORT_SYMBOL(__strncpy_from_user_nocheck_asm);
EXPORT_SYMBOL(__strncpy_from_user_asm);
EXPORT_SYMBOL(__strlen_user_nocheck_asm);
EXPORT_SYMBOL(__strlen_user_asm);
EXPORT_SYMBOL(__strnlen_user_nocheck_asm);
EXPORT_SYMBOL(__strnlen_user_asm);

EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_nocheck);
EXPORT_SYMBOL(__csum_partial_copy_user);
=======
#ifdef CONFIG_EVA
EXPORT_SYMBOL(__copy_from_user_eva);
EXPORT_SYMBOL(__copy_in_user_eva);
EXPORT_SYMBOL(__copy_to_user_eva);
EXPORT_SYMBOL(__copy_user_inatomic_eva);
EXPORT_SYMBOL(__bzero_kernel);
#endif
EXPORT_SYMBOL(__bzero);
EXPORT_SYMBOL(__strncpy_from_kernel_nocheck_asm);
EXPORT_SYMBOL(__strncpy_from_kernel_asm);
EXPORT_SYMBOL(__strncpy_from_user_nocheck_asm);
EXPORT_SYMBOL(__strncpy_from_user_asm);
EXPORT_SYMBOL(__strlen_kernel_asm);
EXPORT_SYMBOL(__strlen_user_asm);
EXPORT_SYMBOL(__strnlen_kernel_nocheck_asm);
EXPORT_SYMBOL(__strnlen_kernel_asm);
EXPORT_SYMBOL(__strnlen_user_nocheck_asm);
EXPORT_SYMBOL(__strnlen_user_asm);

#ifndef CONFIG_CPU_MIPSR6
EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_nocheck);
EXPORT_SYMBOL(__csum_partial_copy_kernel);
EXPORT_SYMBOL(__csum_partial_copy_to_user);
EXPORT_SYMBOL(__csum_partial_copy_from_user);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

EXPORT_SYMBOL(invalid_pte_table);
#ifdef CONFIG_FUNCTION_TRACER
/* _mcount is defined in arch/mips/kernel/mcount.S */
EXPORT_SYMBOL(_mcount);
#endif
