/******************************************************************************
 *
 * Name: aclinux.h - OS specific defines, etc. for Linux
 *
 *****************************************************************************/

/*
<<<<<<< HEAD
 * Copyright (C) 2000 - 2013, Intel Corp.
=======
 * Copyright (C) 2000 - 2016, Intel Corp.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef __ACLINUX_H__
#define __ACLINUX_H__

<<<<<<< HEAD
=======
#ifdef __KERNEL__

/* ACPICA external files should not include ACPICA headers directly. */

#if !defined(BUILDING_ACPICA) && !defined(_LINUX_ACPI_H)
#error "Please don't include <acpi/acpi.h> directly, include <linux/acpi.h> instead."
#endif

#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/* Common (in-kernel/user-space) ACPICA configuration */

#define ACPI_USE_SYSTEM_CLIBRARY
#define ACPI_USE_DO_WHILE_0
<<<<<<< HEAD
#define ACPI_MUTEX_TYPE             ACPI_BINARY_SEMAPHORE


#ifdef __KERNEL__

=======

#ifdef __KERNEL__

#define ACPI_USE_SYSTEM_INTTYPES

/* Kernel specific ACPICA configuration */

#ifdef CONFIG_ACPI_REDUCED_HARDWARE_ONLY
#define ACPI_REDUCED_HARDWARE 1
#endif

#ifdef CONFIG_ACPI_DEBUGGER
#define ACPI_DEBUGGER
#endif

#ifdef CONFIG_ACPI_DEBUG
#define ACPI_MUTEX_DEBUG
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/sched.h>
#include <linux/atomic.h>
<<<<<<< HEAD
#include <asm/div64.h>
#include <asm/acpi.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>
#include <asm/current.h>
=======
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>
#ifdef EXPORT_ACPI_INTERFACES
#include <linux/export.h>
#endif
#ifdef CONFIG_ACPI
#include <asm/acenv.h>
#endif

#define ACPI_INIT_FUNCTION __init

#ifndef CONFIG_ACPI

/* External globals for __KERNEL__, stubs is needed */

#define ACPI_GLOBAL(t,a)
#define ACPI_INIT_GLOBAL(t,a,b)

/* Generating stubs for configurable ACPICA macros */

#define ACPI_NO_MEM_ALLOCATIONS

/* Generating stubs for configurable ACPICA functions */

#define ACPI_NO_ERROR_MESSAGES
#undef ACPI_DEBUG_OUTPUT

/* External interface for __KERNEL__, stub is needed */

#define ACPI_EXTERNAL_RETURN_STATUS(prototype) \
	static ACPI_INLINE prototype {return(AE_NOT_CONFIGURED);}
#define ACPI_EXTERNAL_RETURN_OK(prototype) \
	static ACPI_INLINE prototype {return(AE_OK);}
#define ACPI_EXTERNAL_RETURN_VOID(prototype) \
	static ACPI_INLINE prototype {return;}
#define ACPI_EXTERNAL_RETURN_UINT32(prototype) \
	static ACPI_INLINE prototype {return(0);}
#define ACPI_EXTERNAL_RETURN_PTR(prototype) \
	static ACPI_INLINE prototype {return(NULL);}

#endif				/* CONFIG_ACPI */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Host-dependent types and defines for in-kernel ACPICA */

#define ACPI_MACHINE_WIDTH          BITS_PER_LONG
#define ACPI_EXPORT_SYMBOL(symbol)  EXPORT_SYMBOL(symbol);
#define strtoul                     simple_strtoul

#define acpi_cache_t                        struct kmem_cache
#define acpi_spinlock                       spinlock_t *
#define acpi_cpu_flags                      unsigned long

<<<<<<< HEAD
#else /* !__KERNEL__ */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
=======
/* Use native linux version of acpi_os_allocate_zeroed */

#define USE_NATIVE_ALLOCATE_ZEROED

/*
 * Overrides for in-kernel ACPICA
 */
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_initialize
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_terminate
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_allocate
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_allocate_zeroed
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_free
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_acquire_object
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_thread_id
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_create_lock

/*
 * OSL interfaces used by debugger/disassembler
 */
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_readable
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_writable
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_initialize_command_signals
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_terminate_command_signals

/*
 * OSL interfaces used by utilities
 */
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_redirect_output
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_name
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_index
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_table_by_address
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_open_directory
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_get_next_filename
#define ACPI_USE_ALTERNATE_PROTOTYPE_acpi_os_close_directory

#define ACPI_MSG_ERROR          KERN_ERR "ACPI Error: "
#define ACPI_MSG_EXCEPTION      KERN_ERR "ACPI Exception: "
#define ACPI_MSG_WARNING        KERN_WARNING "ACPI Warning: "
#define ACPI_MSG_INFO           KERN_INFO "ACPI: "

#define ACPI_MSG_BIOS_ERROR     KERN_ERR "ACPI BIOS Error (bug): "
#define ACPI_MSG_BIOS_WARNING   KERN_WARNING "ACPI BIOS Warning (bug): "

#else				/* !__KERNEL__ */

#define ACPI_USE_STANDARD_HEADERS

#ifdef ACPI_USE_STANDARD_HEADERS
#include <unistd.h>
#endif

/* Define/disable kernel-specific declarators */

#ifndef __init
#define __init
#endif
#ifndef __iomem
#define __iomem
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Host-dependent types and defines for user-space ACPICA */

#define ACPI_FLUSH_CPU_CACHE()
#define ACPI_CAST_PTHREAD_T(pthread) ((acpi_thread_id) (pthread))

<<<<<<< HEAD
#if defined(__ia64__) || defined(__x86_64__)
=======
#if defined(__ia64__)    || defined(__x86_64__) ||\
	defined(__aarch64__) || defined(__PPC64__)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#define ACPI_MACHINE_WIDTH          64
#define COMPILER_DEPENDENT_INT64    long
#define COMPILER_DEPENDENT_UINT64   unsigned long
#else
#define ACPI_MACHINE_WIDTH          32
#define COMPILER_DEPENDENT_INT64    long long
#define COMPILER_DEPENDENT_UINT64   unsigned long long
#define ACPI_USE_NATIVE_DIVIDE
#endif

#ifndef __cdecl
#define __cdecl
#endif

<<<<<<< HEAD
#endif /* __KERNEL__ */

/* Linux uses GCC */

#include <acpi/platform/acgcc.h>

#ifdef __KERNEL__
#include <acpi/actypes.h>
/*
 * Overrides for in-kernel ACPICA
 */
static inline acpi_thread_id acpi_os_get_thread_id(void)
{
	return (acpi_thread_id)(unsigned long)current;
}

/*
 * The irqs_disabled() check is for resume from RAM.
 * Interrupts are off during resume, just like they are for boot.
 * However, boot has  (system_state != SYSTEM_RUNNING)
 * to quiet __might_sleep() in kmalloc() and resume does not.
 */
static inline void *acpi_os_allocate(acpi_size size)
{
	return kmalloc(size, irqs_disabled() ? GFP_ATOMIC : GFP_KERNEL);
}

static inline void *acpi_os_allocate_zeroed(acpi_size size)
{
	return kzalloc(size, irqs_disabled() ? GFP_ATOMIC : GFP_KERNEL);
}

static inline void *acpi_os_acquire_object(acpi_cache_t * cache)
{
	return kmem_cache_zalloc(cache,
		irqs_disabled() ? GFP_ATOMIC : GFP_KERNEL);
}

#define ACPI_ALLOCATE(a)        acpi_os_allocate(a)
#define ACPI_ALLOCATE_ZEROED(a) acpi_os_allocate_zeroed(a)
#define ACPI_FREE(a)            kfree(a)

#ifndef CONFIG_PREEMPT
/*
 * Used within ACPICA to show where it is safe to preempt execution
 * when CONFIG_PREEMPT=n
 */
#define ACPI_PREEMPTION_POINT() \
	do { \
		if (!irqs_disabled()) \
			cond_resched(); \
	} while (0)
#endif

/*
 * When lockdep is enabled, the spin_lock_init() macro stringifies it's
 * argument and uses that as a name for the lock in debugging.
 * By executing spin_lock_init() in a macro the key changes from "lock" for
 * all locks to the name of the argument of acpi_os_create_lock(), which
 * prevents lockdep from reporting false positives for ACPICA locks.
 */
#define acpi_os_create_lock(__handle)				\
({								\
	spinlock_t *lock = ACPI_ALLOCATE(sizeof(*lock));	\
								\
	if (lock) {						\
		*(__handle) = lock;				\
		spin_lock_init(*(__handle));			\
	}							\
	lock ? AE_OK : AE_NO_MEMORY;				\
})

#endif /* __KERNEL__ */

#endif /* __ACLINUX_H__ */
=======
#endif				/* __KERNEL__ */

#endif				/* __ACLINUX_H__ */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
