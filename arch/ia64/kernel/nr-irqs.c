/*
 * calculate
 * NR_IRQS = max(IA64_NATIVE_NR_IRQS, XEN_NR_IRQS, FOO_NR_IRQS...)
 * depending on config.
 * This must be calculated before processing asm-offset.c.
 */

#define ASM_OFFSETS_C 1

#include <linux/kbuild.h>
#include <linux/threads.h>
#include <asm/native/irq.h>
<<<<<<< HEAD
#include <asm/xen/irq.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

void foo(void)
{
	union paravirt_nr_irqs_max {
		char ia64_native_nr_irqs[IA64_NATIVE_NR_IRQS];
<<<<<<< HEAD
#ifdef CONFIG_XEN
		char xen_nr_irqs[XEN_NR_IRQS];
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	};

	DEFINE(NR_IRQS, sizeof (union paravirt_nr_irqs_max));
}
