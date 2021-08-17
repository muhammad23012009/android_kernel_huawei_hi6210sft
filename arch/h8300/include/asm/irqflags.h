#ifndef _H8300_IRQFLAGS_H
#define _H8300_IRQFLAGS_H

<<<<<<< HEAD
static inline unsigned long arch_local_save_flags(void)
{
	unsigned long flags;
	asm volatile ("stc ccr,%w0" : "=r" (flags));
=======
#ifdef CONFIG_CPU_H8300H
typedef unsigned char h8300flags;

static inline h8300flags arch_local_save_flags(void)
{
	h8300flags flags;

	__asm__ volatile ("stc ccr,%w0" : "=r" (flags));
	return flags;
}

static inline void arch_local_irq_disable(void)
{
	__asm__ volatile ("orc  #0xc0,ccr");
}

static inline void arch_local_irq_enable(void)
{
	__asm__ volatile ("andc #0x3f,ccr");
}

static inline h8300flags arch_local_irq_save(void)
{
	h8300flags flags;

	__asm__ volatile ("stc ccr,%w0\n\t"
		      "orc  #0xc0,ccr" : "=r" (flags));
	return flags;
}

static inline void arch_local_irq_restore(h8300flags flags)
{
	__asm__ volatile ("ldc %w0,ccr" : : "r" (flags) : "cc");
}

static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return (flags & 0xc0) == 0xc0;
}
#endif
#ifdef CONFIG_CPU_H8S
typedef unsigned short h8300flags;

static inline h8300flags arch_local_save_flags(void)
{
	h8300flags flags;

	__asm__ volatile ("stc ccr,%w0\n\tstc exr,%x0" : "=r" (flags));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return flags;
}

static inline void arch_local_irq_disable(void)
{
<<<<<<< HEAD
	asm volatile ("orc  #0x80,ccr" : : : "memory");
=======
	__asm__ volatile ("orc #0x80,ccr\n\t");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline void arch_local_irq_enable(void)
{
<<<<<<< HEAD
	asm volatile ("andc #0x7f,ccr" : : : "memory");
}

static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	asm volatile ("ldc %w0,ccr" : : "r" (flags) : "memory");
}

static inline bool arch_irqs_disabled_flags(unsigned long flags)
{
	return (flags & 0x80) == 0x80;
}

static inline bool arch_irqs_disabled(void)
=======
	__asm__ volatile ("andc #0x7f,ccr\n\t"
		      "andc #0xf0,exr\n\t");
}

static inline h8300flags arch_local_irq_save(void)
{
	h8300flags flags;

	__asm__ volatile ("stc ccr,%w0\n\t"
		      "stc exr,%x0\n\t"
		      "orc  #0x80,ccr\n\t"
		      : "=r" (flags));
	return flags;
}

static inline void arch_local_irq_restore(h8300flags flags)
{
	__asm__ volatile ("ldc %w0,ccr\n\t"
		      "ldc %x0,exr"
		      : : "r" (flags) : "cc");
}

static inline int arch_irqs_disabled_flags(h8300flags flags)
{
	return (flags & 0x0080) == 0x0080;
}

#endif

static inline int arch_irqs_disabled(void)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* _H8300_IRQFLAGS_H */
