/*
 *	Copyright (C) 1992, 1998 Linus Torvalds, Ingo Molnar
 *
 * This file contains the lowest level x86-specific interrupt
 * entry, irq-stacks and irq statistics code. All the remaining
 * irq logic is done by the generic kernel/irq/ code and
 * by the x86-specific irq controller code. (e.g. i8259.c and
 * io_apic.c.)
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
=======
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/kernel_stat.h>
#include <linux/notifier.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/percpu.h>
#include <linux/mm.h>

#include <asm/apic.h>
<<<<<<< HEAD

DEFINE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);
EXPORT_PER_CPU_SYMBOL(irq_stat);

DEFINE_PER_CPU(struct pt_regs *, irq_regs);
EXPORT_PER_CPU_SYMBOL(irq_regs);
=======
#include <asm/nospec-branch.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_DEBUG_STACKOVERFLOW

int sysctl_panic_on_stackoverflow __read_mostly;

/* Debugging check for stack overflow: is there less than 1KB free? */
static int check_stack_overflow(void)
{
	long sp;

	__asm__ __volatile__("andl %%esp,%0" :
			     "=r" (sp) : "0" (THREAD_SIZE - 1));

	return sp < (sizeof(struct thread_info) + STACK_WARN);
}

static void print_stack_overflow(void)
{
	printk(KERN_WARNING "low stack detected by irq handler\n");
	dump_stack();
	if (sysctl_panic_on_stackoverflow)
		panic("low stack detected by irq handler - check messages\n");
}

#else
static inline int check_stack_overflow(void) { return 0; }
static inline void print_stack_overflow(void) { }
#endif

<<<<<<< HEAD
/*
 * per-CPU IRQ handling contexts (thread information and stack)
 */
union irq_ctx {
	struct thread_info      tinfo;
	u32                     stack[THREAD_SIZE/sizeof(u32)];
} __attribute__((aligned(THREAD_SIZE)));

static DEFINE_PER_CPU(union irq_ctx *, hardirq_ctx);
static DEFINE_PER_CPU(union irq_ctx *, softirq_ctx);
=======
DEFINE_PER_CPU(struct irq_stack *, hardirq_stack);
DEFINE_PER_CPU(struct irq_stack *, softirq_stack);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static void call_on_stack(void *func, void *stack)
{
	asm volatile("xchgl	%%ebx,%%esp	\n"
<<<<<<< HEAD
		     "call	*%%edi		\n"
		     "movl	%%ebx,%%esp	\n"
		     : "=b" (stack)
		     : "0" (stack),
		       "D"(func)
		     : "memory", "cc", "edx", "ecx", "eax");
}

static inline int
execute_on_irq_stack(int overflow, struct irq_desc *desc, int irq)
{
	union irq_ctx *curctx, *irqctx;
	u32 *isp, arg1, arg2;

	curctx = (union irq_ctx *) current_thread_info();
	irqctx = __this_cpu_read(hardirq_ctx);
=======
		     CALL_NOSPEC
		     "movl	%%ebx,%%esp	\n"
		     : "=b" (stack)
		     : "0" (stack),
		       [thunk_target] "D"(func)
		     : "memory", "cc", "edx", "ecx", "eax");
}

static inline void *current_stack(void)
{
	return (void *)(current_stack_pointer & ~(THREAD_SIZE - 1));
}

static inline int execute_on_irq_stack(int overflow, struct irq_desc *desc)
{
	struct irq_stack *curstk, *irqstk;
	u32 *isp, *prev_esp, arg1;

	curstk = (struct irq_stack *) current_stack();
	irqstk = __this_cpu_read(hardirq_stack);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * this is where we switch to the IRQ stack. However, if we are
	 * already using the IRQ stack (because we interrupted a hardirq
	 * handler) we can't do that and just have to keep using the
	 * current stack (which is the irq stack already after all)
	 */
<<<<<<< HEAD
	if (unlikely(curctx == irqctx))
		return 0;

	/* build the stack frame on the IRQ stack */
	isp = (u32 *) ((char *)irqctx + sizeof(*irqctx));
	irqctx->tinfo.task = curctx->tinfo.task;
	irqctx->tinfo.previous_esp = current_stack_pointer;

	/* Copy the preempt_count so that the [soft]irq checks work. */
	irqctx->tinfo.preempt_count = curctx->tinfo.preempt_count;
=======
	if (unlikely(curstk == irqstk))
		return 0;

	isp = (u32 *) ((char *)irqstk + sizeof(*irqstk));

	/* Save the next esp at the bottom of the stack */
	prev_esp = (u32 *)irqstk;
	*prev_esp = current_stack_pointer;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (unlikely(overflow))
		call_on_stack(print_stack_overflow, isp);

	asm volatile("xchgl	%%ebx,%%esp	\n"
<<<<<<< HEAD
		     "call	*%%edi		\n"
		     "movl	%%ebx,%%esp	\n"
		     : "=a" (arg1), "=d" (arg2), "=b" (isp)
		     :  "0" (irq),   "1" (desc),  "2" (isp),
			"D" (desc->handle_irq)
=======
		     CALL_NOSPEC
		     "movl	%%ebx,%%esp	\n"
		     : "=a" (arg1), "=b" (isp)
		     :  "0" (desc),   "1" (isp),
			[thunk_target] "D" (desc->handle_irq)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		     : "memory", "cc", "ecx");
	return 1;
}

/*
 * allocate per-cpu stacks for hardirq and for softirq processing
 */
<<<<<<< HEAD
void __cpuinit irq_ctx_init(int cpu)
{
	union irq_ctx *irqctx;

	if (per_cpu(hardirq_ctx, cpu))
		return;

	irqctx = page_address(alloc_pages_node(cpu_to_node(cpu),
					       THREADINFO_GFP,
					       THREAD_SIZE_ORDER));
	memset(&irqctx->tinfo, 0, sizeof(struct thread_info));
	irqctx->tinfo.cpu		= cpu;
	irqctx->tinfo.preempt_count	= HARDIRQ_OFFSET;
	irqctx->tinfo.addr_limit	= MAKE_MM_SEG(0);

	per_cpu(hardirq_ctx, cpu) = irqctx;

	irqctx = page_address(alloc_pages_node(cpu_to_node(cpu),
					       THREADINFO_GFP,
					       THREAD_SIZE_ORDER));
	memset(&irqctx->tinfo, 0, sizeof(struct thread_info));
	irqctx->tinfo.cpu		= cpu;
	irqctx->tinfo.addr_limit	= MAKE_MM_SEG(0);

	per_cpu(softirq_ctx, cpu) = irqctx;

	printk(KERN_DEBUG "CPU %u irqstacks, hard=%p soft=%p\n",
	       cpu, per_cpu(hardirq_ctx, cpu),  per_cpu(softirq_ctx, cpu));
}

asmlinkage void do_softirq(void)
{
	unsigned long flags;
	struct thread_info *curctx;
	union irq_ctx *irqctx;
	u32 *isp;

	if (in_interrupt())
		return;

	local_irq_save(flags);

	if (local_softirq_pending()) {
		curctx = current_thread_info();
		irqctx = __this_cpu_read(softirq_ctx);
		irqctx->tinfo.task = curctx->task;
		irqctx->tinfo.previous_esp = current_stack_pointer;

		/* build the stack frame on the softirq stack */
		isp = (u32 *) ((char *)irqctx + sizeof(*irqctx));

		call_on_stack(__do_softirq, isp);
		/*
		 * Shouldn't happen, we returned above if in_interrupt():
		 */
		WARN_ON_ONCE(softirq_count());
	}

	local_irq_restore(flags);
}

bool handle_irq(unsigned irq, struct pt_regs *regs)
{
	struct irq_desc *desc;
	int overflow;

	overflow = check_stack_overflow();

	desc = irq_to_desc(irq);
	if (unlikely(!desc))
		return false;

	if (user_mode_vm(regs) || !execute_on_irq_stack(overflow, desc, irq)) {
		if (unlikely(overflow))
			print_stack_overflow();
		desc->handle_irq(irq, desc);
=======
void irq_ctx_init(int cpu)
{
	struct irq_stack *irqstk;

	if (per_cpu(hardirq_stack, cpu))
		return;

	irqstk = page_address(alloc_pages_node(cpu_to_node(cpu),
					       THREADINFO_GFP,
					       THREAD_SIZE_ORDER));
	per_cpu(hardirq_stack, cpu) = irqstk;

	irqstk = page_address(alloc_pages_node(cpu_to_node(cpu),
					       THREADINFO_GFP,
					       THREAD_SIZE_ORDER));
	per_cpu(softirq_stack, cpu) = irqstk;

	printk(KERN_DEBUG "CPU %u irqstacks, hard=%p soft=%p\n",
	       cpu, per_cpu(hardirq_stack, cpu),  per_cpu(softirq_stack, cpu));
}

void do_softirq_own_stack(void)
{
	struct irq_stack *irqstk;
	u32 *isp, *prev_esp;

	irqstk = __this_cpu_read(softirq_stack);

	/* build the stack frame on the softirq stack */
	isp = (u32 *) ((char *)irqstk + sizeof(*irqstk));

	/* Push the previous esp onto the stack */
	prev_esp = (u32 *)irqstk;
	*prev_esp = current_stack_pointer;

	call_on_stack(__do_softirq, isp);
}

bool handle_irq(struct irq_desc *desc, struct pt_regs *regs)
{
	int overflow = check_stack_overflow();

	if (IS_ERR_OR_NULL(desc))
		return false;

	if (user_mode(regs) || !execute_on_irq_stack(overflow, desc)) {
		if (unlikely(overflow))
			print_stack_overflow();
		generic_handle_irq_desc(desc);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}

	return true;
}
