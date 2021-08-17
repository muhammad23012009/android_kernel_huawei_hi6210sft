<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/msr.h>

EXPORT_SYMBOL(rdmsr_safe_regs);
EXPORT_SYMBOL(wrmsr_safe_regs);
