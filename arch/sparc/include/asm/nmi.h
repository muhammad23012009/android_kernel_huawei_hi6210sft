#ifndef __NMI_H
#define __NMI_H

<<<<<<< HEAD
extern int __init nmi_init(void);
extern void perfctr_irq(int irq, struct pt_regs *regs);
extern void nmi_adjust_hz(unsigned int new_hz);

extern atomic_t nmi_active;

extern void start_nmi_watchdog(void *unused);
extern void stop_nmi_watchdog(void *unused);
=======
int __init nmi_init(void);
void perfctr_irq(int irq, struct pt_regs *regs);
void nmi_adjust_hz(unsigned int new_hz);

extern atomic_t nmi_active;

void start_nmi_watchdog(void *unused);
void stop_nmi_watchdog(void *unused);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif /* __NMI_H */
