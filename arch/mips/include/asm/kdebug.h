#ifndef _ASM_MIPS_KDEBUG_H
#define _ASM_MIPS_KDEBUG_H

#include <linux/notifier.h>

enum die_val {
	DIE_OOPS = 1,
	DIE_FP,
	DIE_TRAP,
	DIE_RI,
	DIE_PAGE_FAULT,
	DIE_BREAK,
<<<<<<< HEAD
	DIE_SSTEPBP
=======
	DIE_SSTEPBP,
	DIE_MSAFP,
	DIE_UPROBE,
	DIE_UPROBE_XOL,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
};

#endif /* _ASM_MIPS_KDEBUG_H */
