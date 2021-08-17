#ifndef BOOT_COMPRESSED_MISC_H
#define BOOT_COMPRESSED_MISC_H

/*
<<<<<<< HEAD
 * we have to be careful, because no indirections are allowed here, and
 * paravirt_ops is a kind of one. As it will only run in baremetal anyway,
 * we just keep it from happening
 */
#undef CONFIG_PARAVIRT
#ifdef CONFIG_X86_32
#define _ASM_X86_DESC_H 1
#endif
=======
 * Special hack: we have to be careful, because no indirections are allowed here,
 * and paravirt_ops is a kind of one. As it will only run in baremetal anyway,
 * we just keep it from happening. (This list needs to be extended when new
 * paravirt and debugging variants are added.)
 */
#undef CONFIG_PARAVIRT
#undef CONFIG_PARAVIRT_SPINLOCKS
#undef CONFIG_PAGE_TABLE_ISOLATION
#undef CONFIG_KASAN
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <linux/linkage.h>
#include <linux/screen_info.h>
#include <linux/elf.h>
#include <linux/io.h>
#include <asm/page.h>
#include <asm/boot.h>
#include <asm/bootparam.h>
<<<<<<< HEAD
#include <asm/bootparam_utils.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#define BOOT_BOOT_H
#include "../ctype.h"

<<<<<<< HEAD
/* misc.c */
extern struct boot_params *real_mode;		/* Pointer to real-mode data */
void __putstr(const char *s);
#define error_putstr(__x)  __putstr(__x)
=======
#ifdef CONFIG_X86_64
#define memptr long
#else
#define memptr unsigned
#endif

/* misc.c */
extern memptr free_mem_ptr;
extern memptr free_mem_end_ptr;
extern struct boot_params *boot_params;
void __putstr(const char *s);
void __puthex(unsigned long value);
#define error_putstr(__x)  __putstr(__x)
#define error_puthex(__x)  __puthex(__x)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_X86_VERBOSE_BOOTUP

#define debug_putstr(__x)  __putstr(__x)
<<<<<<< HEAD
=======
#define debug_puthex(__x)  __puthex(__x)
#define debug_putaddr(__x) { \
		debug_putstr(#__x ": 0x"); \
		debug_puthex((unsigned long)(__x)); \
		debug_putstr("\n"); \
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#else

static inline void debug_putstr(const char *s)
{ }
<<<<<<< HEAD

#endif

#ifdef CONFIG_EARLY_PRINTK

/* cmdline.c */
int cmdline_find_option(const char *option, char *buffer, int bufsize);
int cmdline_find_option_bool(const char *option);

/* early_serial_console.c */
extern int early_serial_base;
void console_init(void);

#else

/* early_serial_console.c */
static const int early_serial_base;
static inline void console_init(void)
{ }

=======
static inline void debug_puthex(const char *s)
{ }
#define debug_putaddr(x) /* */

#endif

#if CONFIG_EARLY_PRINTK || CONFIG_RANDOMIZE_BASE
/* cmdline.c */
int cmdline_find_option(const char *option, char *buffer, int bufsize);
int cmdline_find_option_bool(const char *option);
#endif


#if CONFIG_RANDOMIZE_BASE
/* kaslr.c */
void choose_random_location(unsigned long input,
			    unsigned long input_size,
			    unsigned long *output,
			    unsigned long output_size,
			    unsigned long *virt_addr);
/* cpuflags.c */
bool has_cpuflag(int flag);
#else
static inline void choose_random_location(unsigned long input,
					  unsigned long input_size,
					  unsigned long *output,
					  unsigned long output_size,
					  unsigned long *virt_addr)
{
}
#endif

#ifdef CONFIG_X86_64
void initialize_identity_maps(void);
void add_identity_map(unsigned long start, unsigned long size);
void finalize_identity_maps(void);
extern unsigned char _pgtable[];
#else
static inline void initialize_identity_maps(void)
{ }
static inline void add_identity_map(unsigned long start, unsigned long size)
{ }
static inline void finalize_identity_maps(void)
{ }
#endif

#ifdef CONFIG_EARLY_PRINTK
/* early_serial_console.c */
extern int early_serial_base;
void console_init(void);
#else
static const int early_serial_base;
static inline void console_init(void)
{ }
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif
