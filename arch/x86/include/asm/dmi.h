#ifndef _ASM_X86_DMI_H
#define _ASM_X86_DMI_H

#include <linux/compiler.h>
#include <linux/init.h>
<<<<<<< HEAD

#include <asm/io.h>
=======
#include <linux/io.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/setup.h>

static __always_inline __init void *dmi_alloc(unsigned len)
{
	return extend_brk(len, sizeof(int));
}

/* Use early IO mappings for DMI because it's initialized early */
<<<<<<< HEAD
#define dmi_ioremap early_ioremap
#define dmi_iounmap early_iounmap
=======
#define dmi_early_remap		early_ioremap
#define dmi_early_unmap		early_iounmap
#define dmi_remap		ioremap_cache
#define dmi_unmap		iounmap
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* _ASM_X86_DMI_H */
