#ifndef __ASM_TRAPS_H
#define __ASM_TRAPS_H

#ifdef __KERNEL__
struct pt_regs;

/* traps.c */
void parisc_terminate(char *msg, struct pt_regs *regs,
<<<<<<< HEAD
		int code, unsigned long offset);

/* mm/fault.c */
=======
		int code, unsigned long offset) __noreturn __cold;

void die_if_kernel(char *str, struct pt_regs *regs, long err);

/* mm/fault.c */
const char *trap_name(unsigned long code);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
void do_page_fault(struct pt_regs *regs, unsigned long code,
		unsigned long address);
#endif

#endif
