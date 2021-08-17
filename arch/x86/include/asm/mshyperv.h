#ifndef _ASM_X86_MSHYPER_H
#define _ASM_X86_MSHYPER_H

#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/interrupt.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/hyperv.h>

struct ms_hyperv_info {
	u32 features;
<<<<<<< HEAD
=======
	u32 misc_features;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	u32 hints;
};

extern struct ms_hyperv_info ms_hyperv;

void hyperv_callback_vector(void);
<<<<<<< HEAD
void hyperv_vector_handler(struct pt_regs *regs);
void hv_register_vmbus_handler(int irq, irq_handler_t handler);

=======
#ifdef CONFIG_TRACING
#define trace_hyperv_callback_vector hyperv_callback_vector
#endif
void hyperv_vector_handler(struct pt_regs *regs);
void hv_setup_vmbus_irq(void (*handler)(void));
void hv_remove_vmbus_irq(void);

void hv_setup_kexec_handler(void (*handler)(void));
void hv_remove_kexec_handler(void);
void hv_setup_crash_handler(void (*handler)(struct pt_regs *regs));
void hv_remove_crash_handler(void);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
