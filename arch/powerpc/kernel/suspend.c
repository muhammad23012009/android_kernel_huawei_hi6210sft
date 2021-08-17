/*
 * Suspend support specific for power.
 *
 * Distribute under GPLv2
 *
 * Copyright (c) 2002 Pavel Machek <pavel@ucw.cz>
 * Copyright (c) 2001 Patrick Mochel <mochel@osdl.org>
 */

#include <linux/mm.h>
#include <asm/page.h>
<<<<<<< HEAD

/* References to section boundaries */
extern const void __nosave_begin, __nosave_end;
=======
#include <asm/sections.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 *	pfn_is_nosave - check if given pfn is in the 'nosave' section
 */

int pfn_is_nosave(unsigned long pfn)
{
	unsigned long nosave_begin_pfn = __pa(&__nosave_begin) >> PAGE_SHIFT;
	unsigned long nosave_end_pfn = PAGE_ALIGN(__pa(&__nosave_end)) >> PAGE_SHIFT;
	return (pfn >= nosave_begin_pfn) && (pfn < nosave_end_pfn);
}
