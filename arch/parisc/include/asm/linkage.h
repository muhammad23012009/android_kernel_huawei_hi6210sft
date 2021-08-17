#ifndef __ASM_PARISC_LINKAGE_H
#define __ASM_PARISC_LINKAGE_H

<<<<<<< HEAD
=======
#include <asm/dwarf.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifndef __ALIGN
#define __ALIGN         .align 4
#define __ALIGN_STR     ".align 4"
#endif

/*
 * In parisc assembly a semicolon marks a comment while a
 * exclamation mark is used to separate independent lines.
 */
<<<<<<< HEAD
=======
#define ASM_NL	!

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef __ASSEMBLY__

#define ENTRY(name) \
	.export name !\
	ALIGN !\
name:

#ifdef CONFIG_64BIT
#define ENDPROC(name) \
	END(name)
#else
#define ENDPROC(name) \
	.type name, @function !\
	END(name)
#endif

<<<<<<< HEAD
=======
#define ENTRY_CFI(name) \
	ENTRY(name)	ASM_NL\
	CFI_STARTPROC

#define ENDPROC_CFI(name) \
	ENDPROC(name)	ASM_NL\
	CFI_ENDPROC

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ASSEMBLY__ */

#endif  /* __ASM_PARISC_LINKAGE_H */
