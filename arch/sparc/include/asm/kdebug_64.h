#ifndef _SPARC64_KDEBUG_H
#define _SPARC64_KDEBUG_H

struct pt_regs;

<<<<<<< HEAD
extern void bad_trap(struct pt_regs *, long);
=======
void bad_trap(struct pt_regs *, long);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/* Grossly misnamed. */
enum die_val {
	DIE_OOPS = 1,
	DIE_DEBUG,	/* ta 0x70 */
	DIE_DEBUG_2,	/* ta 0x71 */
	DIE_DIE,
	DIE_TRAP,
	DIE_TRAP_TL1,
	DIE_CALL,
	DIE_NMI,
	DIE_NMIWATCHDOG,
};

#endif
