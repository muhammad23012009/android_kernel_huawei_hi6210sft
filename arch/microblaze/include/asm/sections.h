/*
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MICROBLAZE_SECTIONS_H
#define _ASM_MICROBLAZE_SECTIONS_H

#include <asm-generic/sections.h>

# ifndef __ASSEMBLY__
extern char _ssbss[], _esbss[];
extern unsigned long __ivt_start[], __ivt_end[];
<<<<<<< HEAD
extern char _etext[], _stext[];
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern u32 _fdt_start[], _fdt_end[];

# endif /* !__ASSEMBLY__ */
#endif /* _ASM_MICROBLAZE_SECTIONS_H */
