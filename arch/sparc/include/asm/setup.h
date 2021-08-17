/*
 *	Just a place holder. 
 */
#ifndef _SPARC_SETUP_H
#define _SPARC_SETUP_H

<<<<<<< HEAD
#include <uapi/asm/setup.h>

=======
#include <linux/interrupt.h>

#include <uapi/asm/setup.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern char reboot_command[];

#ifdef CONFIG_SPARC32
/* The CPU that was used for booting
 * Only sun4d + leon may have boot_cpu_id != 0
 */
extern unsigned char boot_cpu_id;

<<<<<<< HEAD
extern unsigned long empty_zero_page;
=======
extern unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern int serial_console;
static inline int con_is_present(void)
{
	return serial_console ? 0 : 1;
}
<<<<<<< HEAD
#endif

extern void sun_do_break(void);
=======

/* from irq_32.c */
extern volatile unsigned char *fdc_status;
extern char *pdma_vaddr;
extern unsigned long pdma_size;
extern volatile int doing_pdma;

/* This is software state */
extern char *pdma_base;
extern unsigned long pdma_areasize;

int sparc_floppy_request_irq(unsigned int irq, irq_handler_t irq_handler);

/* setup_32.c */
extern unsigned long cmdline_memory_size;

/* devices.c */
void __init device_scan(void);

/* unaligned_32.c */
unsigned long safe_compute_effective_address(struct pt_regs *, unsigned int);

#endif

#ifdef CONFIG_SPARC64
void __init start_early_boot(void);

/* unaligned_64.c */
int handle_ldf_stq(u32 insn, struct pt_regs *regs);
void handle_ld_nf(u32 insn, struct pt_regs *regs);

/* init_64.c */
extern atomic_t dcpage_flushes;
extern atomic_t dcpage_flushes_xcall;

extern int sysctl_tsb_ratio;

#ifdef CONFIG_SERIAL_SUNHV
void sunhv_migrate_hvcons_irq(int cpu);
#endif
#endif
void sun_do_break(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
extern int stop_a_enabled;
extern int scons_pwroff;

#endif /* _SPARC_SETUP_H */
