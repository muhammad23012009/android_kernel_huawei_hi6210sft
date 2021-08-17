/*
 * Based on arch/arm/kernel/traps.c
 *
 * Copyright (C) 1995-2009 Russell King
 * Copyright (C) 2012 ARM Ltd.
<<<<<<< HEAD
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

<<<<<<< HEAD
=======
#include <linux/bug.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/signal.h>
#include <linux/personality.h>
#include <linux/kallsyms.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>
#include <linux/kdebug.h>
#include <linux/module.h>
#include <linux/kexec.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

#include <asm/atomic.h>
<<<<<<< HEAD
#include <asm/debug-monitors.h>
=======
#include <asm/barrier.h>
#include <asm/bug.h>
#include <asm/debug-monitors.h>
#include <asm/esr.h>
#include <asm/insn.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm/traps.h>
#include <asm/stacktrace.h>
#include <asm/exception.h>
#include <asm/system_misc.h>
<<<<<<< HEAD
=======
#include <asm/sysreg.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static const char *handler[]= {
	"Synchronous Abort",
	"IRQ",
	"FIQ",
	"Error"
};

<<<<<<< HEAD
int show_unhandled_signals = 1;

#ifdef CONFIG_DETECT_HUNG_TASK
typedef void (*funcptr2)(unsigned long, unsigned long);
extern void add_hw_hungtask_hook(funcptr2 printhook);
static funcptr2 hw_hung_task_hook;
void add_hw_hungtask_hook(funcptr2 printhook)
{
	hw_hung_task_hook = printhook;
}
#endif

/*
 * Dump out the contents of some memory nicely...
=======
int show_unhandled_signals = 0;

/*
 * Dump out the contents of some kernel memory nicely...
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 */
static void dump_mem(const char *lvl, const char *str, unsigned long bottom,
		     unsigned long top)
{
	unsigned long first;
	mm_segment_t fs;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
<<<<<<< HEAD
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
=======
	 * to safely read from kernel space.
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

	printk("%s%s(0x%016lx to 0x%016lx)\n", lvl, str, bottom, top);

	for (first = bottom & ~31; first < top; first += 32) {
		unsigned long p;
		char str[sizeof(" 12345678") * 8 + 1];

		memset(str, ' ', sizeof(str));
		str[sizeof(str) - 1] = '\0';

<<<<<<< HEAD
		for (p = first, i = 0; i < 8 && p < top; i++, p += 4) {
			if (p >= bottom && p < top) {
				unsigned int val;
				if (__get_user(val, (unsigned int *)p) == 0)
					sprintf(str + i * 9, " %08x", val);
				else
					sprintf(str + i * 9, " ????????");
=======
		for (p = first, i = 0; i < (32 / 8)
					&& p < top; i++, p += 8) {
			if (p >= bottom && p < top) {
				unsigned long val;

				if (__get_user(val, (unsigned long *)p) == 0)
					sprintf(str + i * 17, " %016lx", val);
				else
					sprintf(str + i * 17, " ????????????????");
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
			}
		}
		printk("%s%04lx:%s\n", lvl, first & 0xffff, str);
	}

	set_fs(fs);
}

<<<<<<< HEAD
static void dump_backtrace_entry(unsigned long where, unsigned long stack)
{
	print_ip_sym(where);
	if (in_exception_text(where))
		dump_mem("", "Exception stack", stack,
			 stack + sizeof(struct pt_regs));
}

static void dump_instr(const char *lvl, struct pt_regs *regs)
{
	unsigned long addr = instruction_pointer(regs);
	mm_segment_t fs;
	char str[sizeof("00000000 ") * 5 + 2 + 1], *p = str;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

	for (i = -4; i < 1; i++) {
		unsigned int val, bad;

		bad = __get_user(val, &((u32 *)addr)[i]);
=======
static void dump_backtrace_entry(unsigned long where)
{
	/*
	 * Note that 'where' can have a physical address, but it's not handled.
	 */
	print_ip_sym(where);
}

static void __dump_instr(const char *lvl, struct pt_regs *regs)
{
	unsigned long addr = instruction_pointer(regs);
	char str[sizeof("00000000 ") * 5 + 2 + 1], *p = str;
	int i;

	for (i = -4; i < 1; i++) {
		unsigned int val, bad;

		bad = get_user(val, &((u32 *)addr)[i]);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		if (!bad)
			p += sprintf(p, i == 0 ? "(%08x) " : "%08x ", val);
		else {
			p += sprintf(p, "bad PC value");
			break;
		}
	}
	printk("%sCode: %s\n", lvl, str);
<<<<<<< HEAD

	set_fs(fs);
=======
}

static void dump_instr(const char *lvl, struct pt_regs *regs)
{
	if (!user_mode(regs)) {
		mm_segment_t fs = get_fs();
		set_fs(KERNEL_DS);
		__dump_instr(lvl, regs);
		set_fs(fs);
	} else {
		__dump_instr(lvl, regs);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk)
{
	struct stackframe frame;
<<<<<<< HEAD
=======
	unsigned long irq_stack_ptr;
	int skip;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	pr_debug("%s(regs = %p tsk = %p)\n", __func__, regs, tsk);

	if (!tsk)
		tsk = current;

<<<<<<< HEAD
	if (regs) {
		frame.fp = regs->regs[29];
		frame.sp = regs->sp;
		frame.pc = regs->pc;
	} else if (tsk == current) {
=======
	/*
	 * Switching between stacks is valid when tracing current and in
	 * non-preemptible context.
	 */
	if (tsk == current && !preemptible())
		irq_stack_ptr = IRQ_STACK_PTR(smp_processor_id());
	else
		irq_stack_ptr = 0;

	if (tsk == current) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		frame.fp = (unsigned long)__builtin_frame_address(0);
		frame.sp = current_stack_pointer;
		frame.pc = (unsigned long)dump_backtrace;
	} else {
		/*
		 * task blocked in __switch_to
		 */
		frame.fp = thread_saved_fp(tsk);
		frame.sp = thread_saved_sp(tsk);
		frame.pc = thread_saved_pc(tsk);
	}
<<<<<<< HEAD

	pr_emerg("Call trace:\n");
	while (1) {
		unsigned long where = frame.pc;
		int ret;

		ret = unwind_frame(&frame);
		if (ret < 0)
			break;
		dump_backtrace_entry(where, frame.sp);
=======
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame.graph = tsk->curr_ret_stack;
#endif

	skip = !!regs;
	printk("Call trace:\n");
	while (1) {
		unsigned long where = frame.pc;
		unsigned long stack;
		int ret;

		/* skip until specified stack frame */
		if (!skip) {
			dump_backtrace_entry(where);
		} else if (frame.fp == regs->regs[29]) {
			skip = 0;
			/*
			 * Mostly, this is the case where this function is
			 * called in panic/abort. As exception handler's
			 * stack frame does not contain the corresponding pc
			 * at which an exception has taken place, use regs->pc
			 * instead.
			 */
			dump_backtrace_entry(regs->pc);
		}
		ret = unwind_frame(tsk, &frame);
		if (ret < 0)
			break;
		stack = frame.sp;
		if (in_exception_text(where)) {
			/*
			 * If we switched to the irq_stack before calling this
			 * exception handler, then the pt_regs will be on the
			 * task stack. The easiest way to tell is if the large
			 * pt_regs would overlap with the end of the irq_stack.
			 */
			if (stack < irq_stack_ptr &&
			    (stack + sizeof(struct pt_regs)) > irq_stack_ptr)
				stack = IRQ_STACK_TO_TASK_STACK(irq_stack_ptr);

			dump_mem("", "Exception stack", stack,
				 stack + sizeof(struct pt_regs));
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
}

void show_stack(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace(NULL, tsk);
	barrier();
}

<<<<<<< HEAD
void show_stack_ex(struct pt_regs *regs, struct task_struct *tsk)
{
	if (!user_mode(regs) || in_interrupt()) {
		dump_backtrace(regs, tsk);
		barrier();
	} else {
		pr_info("Call trace: in user_mode!\n");
	}
}

#include <linux/huawei/rdr.h>
#include <linux/huawei/rdr_private.h>
#ifdef CONFIG_HISI_RDR
/*
 * Dump out the contents of some memory nicely...
 */
static void dump_mem_for_rdr(const char *lvl, const char *str, unsigned long bottom,
		     unsigned long top)
{
	unsigned long first;
	mm_segment_t fs;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

	//printk("%s%s(0x%016lx to 0x%016lx)\n", lvl, str, bottom, top);

	for (first = bottom & ~31; first < top; first += 32) {
		unsigned long p;
		char str[sizeof(" 12345678") * 8 + 1];

		memset(str, ' ', sizeof(str));
		str[sizeof(str) - 1] = '\0';

		for (p = first, i = 0; i < 8 && p < top; i++, p += 4) {
			if (p >= bottom && p < top) {
				unsigned int val;
				if (__get_user(val, (unsigned int *)p) == 0)
					sprintf(str + i * 9, " %08x", val);
				else
					sprintf(str + i * 9, " ????????");
			}
		}
		//printk("%s%04lx:%s\n", lvl, first & 0xffff, str);
	}

	set_fs(fs);
}

static void dump_backtrace_entry_for_rdr(unsigned long where,
        unsigned long from)
{
#ifdef CONFIG_DETECT_HUNG_TASK
    /* < DTS2013120902611 wangdedong 00204535 2013.12.9 begin */
    if (hw_hung_task_hook != NULL)
        hw_hung_task_hook(where, from);
    /* DTS2013120902611 wangdedong 00204535 2013.12.9 end > */
#endif
#if 0
	print_ip_sym(where);
	if (in_exception_text(where))
		dump_mem_for_rdr("", "Exception stack", stack,
			 stack + sizeof(struct pt_regs));
#endif
}

static void dump_backtrace_for_rdr(struct pt_regs *regs,
		struct task_struct *tsk)
{
	struct stackframe frame;
	const register unsigned long current_sp asm ("sp");

	if (!tsk)
		tsk = current;
	if (regs) {
		frame.fp = regs->regs[29];
		frame.sp = regs->sp;
		frame.pc = regs->pc;
	} else if (tsk == current) {
		frame.fp = (unsigned long)__builtin_frame_address(0);
		frame.sp = current_sp;
		frame.pc = (unsigned long)dump_backtrace;
	} else {
		/*
		 * task blocked in __switch_to
		 */
		frame.fp = thread_saved_fp(tsk);
		frame.sp = thread_saved_sp(tsk);
		frame.pc = thread_saved_pc(tsk);
		//printk("xiehongliang. else\n");
	}

	while (1) {
		unsigned long where = frame.pc;
		int ret;

		ret = unwind_frame(&frame);
		if (ret < 0)
			break;
		dump_backtrace_entry_for_rdr(where, frame.sp);
	}
}
void show_stack_for_rdr(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace_for_rdr(NULL, tsk);
	barrier();
}
void dump_stack_bl(struct task_struct *tsk)
{
	show_stack_for_rdr(tsk, NULL);
}

static rdr_funcptr_3 p_exc_hook;
unsigned int arm_exc_type = 0xffff;

void exc_hook_add(rdr_funcptr_3 p_hook_func)
{
	p_exc_hook = p_hook_func;
}
EXPORT_SYMBOL(exc_hook_add);

void exc_hook_delete(void)
{
	p_exc_hook = NULL;
}
EXPORT_SYMBOL(exc_hook_delete);
#else
void dump_stack_bl(struct task_struct *tsk)
{
}

void exc_hook_add(rdr_funcptr_3 p_hook_func)
{
}

void exc_hook_delete(void)
{
}
#endif


=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifdef CONFIG_PREEMPT
#define S_PREEMPT " PREEMPT"
#else
#define S_PREEMPT ""
#endif
<<<<<<< HEAD
#ifdef CONFIG_SMP
#define S_SMP " SMP"
#else
#define S_SMP ""
#endif
=======
#define S_SMP " SMP"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static int __die(const char *str, int err, struct thread_info *thread,
		 struct pt_regs *regs)
{
	struct task_struct *tsk = thread->task;
	static int die_counter;
	int ret;

	pr_emerg("Internal error: %s: %x [#%d]" S_PREEMPT S_SMP "\n",
		 str, err, ++die_counter);

	/* trap and error numbers are mostly meaningless on ARM */
	ret = notify_die(DIE_OOPS, str, regs, err, 0, SIGSEGV);
	if (ret == NOTIFY_STOP)
		return ret;

	print_modules();
	__show_regs(regs);
	pr_emerg("Process %.*s (pid: %d, stack limit = 0x%p)\n",
		 TASK_COMM_LEN, tsk->comm, task_pid_nr(tsk), thread + 1);

<<<<<<< HEAD
	if (!user_mode(regs) || in_interrupt()) {
=======
	if (!user_mode(regs)) {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		dump_mem(KERN_EMERG, "Stack: ", regs->sp,
			 THREAD_SIZE + (unsigned long)task_stack_page(tsk));
		dump_backtrace(regs, tsk);
		dump_instr(KERN_EMERG, regs);
	}

	return ret;
}

static DEFINE_RAW_SPINLOCK(die_lock);

/*
 * This function is protected against re-entrancy.
 */
void die(const char *str, struct pt_regs *regs, int err)
{
	struct thread_info *thread = current_thread_info();
	int ret;
<<<<<<< HEAD

	oops_enter();

	raw_spin_lock_irq(&die_lock);
=======
	unsigned long flags;

	raw_spin_lock_irqsave(&die_lock, flags);

	oops_enter();

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	console_verbose();
	bust_spinlocks(1);
	ret = __die(str, err, thread, regs);

	if (regs && kexec_should_crash(thread->task))
		crash_kexec(regs);

	bust_spinlocks(0);
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
<<<<<<< HEAD
	raw_spin_unlock_irq(&die_lock);

#ifdef CONFIG_HISI_RDR
	if (NULL != p_exc_hook) /*excute exc hook func*/
		p_exc_hook(current, arm_exc_type, regs);
#endif
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	oops_exit();

	if (in_interrupt())
		panic("Fatal exception in interrupt");
	if (panic_on_oops)
		panic("Fatal exception");
<<<<<<< HEAD
=======

	raw_spin_unlock_irqrestore(&die_lock, flags);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (ret != NOTIFY_STOP)
		do_exit(SIGSEGV);
}

void arm64_notify_die(const char *str, struct pt_regs *regs,
		      struct siginfo *info, int err)
{
<<<<<<< HEAD
	if (user_mode(regs))
		force_sig_info(info->si_signo, info, current);
	else
		die(str, regs, err);
}

static LIST_HEAD(undef_hook);

void register_undef_hook(struct undef_hook *hook)
{
	list_add(&hook->node, &undef_hook);
}

static int call_undef_hook(struct pt_regs *regs, unsigned int instr)
{
	struct undef_hook *hook;
	int (*fn)(struct pt_regs *regs, unsigned int instr) = NULL;

	list_for_each_entry(hook, &undef_hook, node)
		if ((instr & hook->instr_mask) == hook->instr_val &&
		    (regs->pstate & hook->pstate_mask) == hook->pstate_val)
			fn = hook->fn;

	return fn ? fn(regs, instr) : 1;
}

asmlinkage void __exception do_undefinstr(struct pt_regs *regs)
{
	u32 instr;
	siginfo_t info;
	void __user *pc = (void __user *)instruction_pointer(regs);

	/* check for AArch32 breakpoint instructions */
	if (!aarch32_break_handler(regs))
		return;
	if (user_mode(regs)) {
		if (compat_thumb_mode(regs)) {
			if (get_user(instr, (u16 __user *)pc))
				goto die_sig;
			if (is_wide_instruction(instr)) {
				u32 instr2;
				if (get_user(instr2, (u16 __user *)pc+1))
					goto die_sig;
				instr <<= 16;
				instr |= instr2;
			}
		} else if (get_user(instr, (u32 __user *)pc)) {
			goto die_sig;
		}
	} else {
		/* kernel mode */
		instr = *((u32 *)pc);
	}

	if (call_undef_hook(regs, instr) == 0)
		return;

die_sig:
	if (show_unhandled_signals && unhandled_signal(current, SIGILL) &&
	    printk_ratelimit()) {
		pr_info("%s[%d]: undefined instruction: pc=%p\n",
			current->comm, task_pid_nr(current), pc);
		dump_instr(KERN_INFO, regs);
	}

	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLOPC;
	info.si_addr  = pc;

#ifdef CONFIG_HISI_RDR
	arm_exc_type = DUMP_ARM_VEC_UNDEF;
#endif

	arm64_notify_die("Oops - undefined instruction", regs, &info, 0);
=======
	if (user_mode(regs)) {
		current->thread.fault_address = 0;
		current->thread.fault_code = err;
		force_sig_info(info->si_signo, info, current);
	} else {
		die(str, regs, err);
	}
}

static LIST_HEAD(undef_hook);
static DEFINE_RAW_SPINLOCK(undef_lock);

void register_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_add(&hook->node, &undef_hook);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

void unregister_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_del(&hook->node);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

static int call_undef_hook(struct pt_regs *regs)
{
	struct undef_hook *hook;
	unsigned long flags;
	u32 instr;
	int (*fn)(struct pt_regs *regs, u32 instr) = NULL;
	void __user *pc = (void __user *)instruction_pointer(regs);

	if (!user_mode(regs))
		return 1;

	if (compat_thumb_mode(regs)) {
		/* 16-bit Thumb instruction */
		if (get_user(instr, (u16 __user *)pc))
			goto exit;
		instr = le16_to_cpu(instr);
		if (aarch32_insn_is_wide(instr)) {
			u32 instr2;

			if (get_user(instr2, (u16 __user *)(pc + 2)))
				goto exit;
			instr2 = le16_to_cpu(instr2);
			instr = (instr << 16) | instr2;
		}
	} else {
		/* 32-bit ARM instruction */
		if (get_user(instr, (u32 __user *)pc))
			goto exit;
		instr = le32_to_cpu(instr);
	}

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_for_each_entry(hook, &undef_hook, node)
		if ((instr & hook->instr_mask) == hook->instr_val &&
			(regs->pstate & hook->pstate_mask) == hook->pstate_val)
			fn = hook->fn;

	raw_spin_unlock_irqrestore(&undef_lock, flags);
exit:
	return fn ? fn(regs, instr) : 1;
}

static void force_signal_inject(int signal, int code, struct pt_regs *regs,
				unsigned long address)
{
	siginfo_t info;
	void __user *pc = (void __user *)instruction_pointer(regs);
	const char *desc;

	switch (signal) {
	case SIGILL:
		desc = "undefined instruction";
		break;
	case SIGSEGV:
		desc = "illegal memory access";
		break;
	default:
		desc = "bad mode";
		break;
	}

	if (unhandled_signal(current, signal) &&
	    show_unhandled_signals_ratelimited()) {
		pr_info("%s[%d]: %s: pc=%p\n",
			current->comm, task_pid_nr(current), desc, pc);
		dump_instr(KERN_INFO, regs);
	}

	info.si_signo = signal;
	info.si_errno = 0;
	info.si_code  = code;
	info.si_addr  = pc;

	arm64_notify_die(desc, regs, &info, 0);
}

/*
 * Set up process info to signal segmentation fault - called on access error.
 */
void arm64_notify_segfault(struct pt_regs *regs, unsigned long addr)
{
	int code;

	down_read(&current->mm->mmap_sem);
	if (find_vma(current->mm, addr) == NULL)
		code = SEGV_MAPERR;
	else
		code = SEGV_ACCERR;
	up_read(&current->mm->mmap_sem);

	force_signal_inject(SIGSEGV, code, regs, addr);
}

asmlinkage void __exception do_undefinstr(struct pt_regs *regs)
{
	/* check for AArch32 breakpoint instructions */
	if (!aarch32_break_handler(regs))
		return;

	if (call_undef_hook(regs) == 0)
		return;

	force_signal_inject(SIGILL, ILL_ILLOPC, regs, 0);
}

int cpu_enable_cache_maint_trap(void *__unused)
{
	config_sctlr_el1(SCTLR_EL1_UCI, 0);
	return 0;
}

#define __user_cache_maint(insn, address, res)			\
	if (address >= user_addr_max()) {			\
		res = -EFAULT;					\
	} else {						\
		uaccess_ttbr0_enable();				\
		asm volatile (					\
			"1:	" insn ", %1\n"			\
			"	mov	%w0, #0\n"		\
			"2:\n"					\
			"	.pushsection .fixup,\"ax\"\n"	\
			"	.align	2\n"			\
			"3:	mov	%w0, %w2\n"		\
			"	b	2b\n"			\
			"	.popsection\n"			\
			_ASM_EXTABLE(1b, 3b)			\
			: "=r" (res)				\
			: "r" (address), "i" (-EFAULT));	\
		uaccess_ttbr0_disable();			\
	}

static void user_cache_maint_handler(unsigned int esr, struct pt_regs *regs)
{
	unsigned long address;
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;
	int crm = (esr & ESR_ELx_SYS64_ISS_CRM_MASK) >> ESR_ELx_SYS64_ISS_CRM_SHIFT;
	int ret = 0;

	address = (rt == 31) ? 0 : untagged_addr(regs->regs[rt]);

	switch (crm) {
	case ESR_ELx_SYS64_ISS_CRM_DC_CVAU:	/* DC CVAU, gets promoted */
		__user_cache_maint("dc civac", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_DC_CVAC:	/* DC CVAC, gets promoted */
		__user_cache_maint("dc civac", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_DC_CIVAC:	/* DC CIVAC */
		__user_cache_maint("dc civac", address, ret);
		break;
	case ESR_ELx_SYS64_ISS_CRM_IC_IVAU:	/* IC IVAU */
		__user_cache_maint("ic ivau", address, ret);
		break;
	default:
		force_signal_inject(SIGILL, ILL_ILLOPC, regs, 0);
		return;
	}

	if (ret)
		arm64_notify_segfault(regs, address);
	else
		regs->pc += 4;
}

static void ctr_read_handler(unsigned int esr, struct pt_regs *regs)
{
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;

	regs->regs[rt] = arm64_ftr_reg_ctrel0.sys_val;
	regs->pc += 4;
}

static void cntvct_read_handler(unsigned int esr, struct pt_regs *regs)
{
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;

	isb();
	if (rt != 31)
		regs->regs[rt] = arch_counter_get_cntvct();
	regs->pc += 4;
}

static void cntfrq_read_handler(unsigned int esr, struct pt_regs *regs)
{
	int rt = (esr & ESR_ELx_SYS64_ISS_RT_MASK) >> ESR_ELx_SYS64_ISS_RT_SHIFT;

	if (rt != 31)
		regs->regs[rt] = read_sysreg(cntfrq_el0);
	regs->pc += 4;
}

struct sys64_hook {
	unsigned int esr_mask;
	unsigned int esr_val;
	void (*handler)(unsigned int esr, struct pt_regs *regs);
};

static struct sys64_hook sys64_hooks[] = {
	{
		.esr_mask = ESR_ELx_SYS64_ISS_EL0_CACHE_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_EL0_CACHE_OP_VAL,
		.handler = user_cache_maint_handler,
	},
	{
		/* Trap read access to CTR_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CTR_READ,
		.handler = ctr_read_handler,
	},
	{
		/* Trap read access to CNTVCT_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CNTVCT,
		.handler = cntvct_read_handler,
	},
	{
		/* Trap read access to CNTFRQ_EL0 */
		.esr_mask = ESR_ELx_SYS64_ISS_SYS_OP_MASK,
		.esr_val = ESR_ELx_SYS64_ISS_SYS_CNTFRQ,
		.handler = cntfrq_read_handler,
	},
	{},
};

asmlinkage void __exception do_sysinstr(unsigned int esr, struct pt_regs *regs)
{
	struct sys64_hook *hook;

	for (hook = sys64_hooks; hook->handler; hook++)
		if ((hook->esr_mask & esr) == hook->esr_val) {
			hook->handler(esr, regs);
			return;
		}

	force_signal_inject(SIGILL, ILL_ILLOPC, regs, 0);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

long compat_arm_syscall(struct pt_regs *regs);

asmlinkage long do_ni_syscall(struct pt_regs *regs)
{
#ifdef CONFIG_COMPAT
	long ret;
	if (is_compat_task()) {
		ret = compat_arm_syscall(regs);
		if (ret != -ENOSYS)
			return ret;
	}
#endif

<<<<<<< HEAD
	if (show_unhandled_signals && printk_ratelimit()) {
		pr_info("%s[%d]: syscall %d\n", current->comm,
			task_pid_nr(current), (int)regs->syscallno);
		dump_instr("", regs);
		if (user_mode(regs))
			__show_regs(regs);
	}

	return sys_ni_syscall();
}


#ifdef CONFIG_SMP
extern void smp_send_cpus_backtrace(struct pt_regs *regs);
void trigger_cpus_backtrace(struct pt_regs *regs)
{
	smp_send_cpus_backtrace(regs);
}
#else
void trigger_cpus_backtrace(struct pt_regs *regs)
{
	show_regs(regs);
	show_stack_ex(regs, NULL);
}
#endif

/*
 * fiq_dump handles the case in the fiq exception vector.
 */
 #define BAD_FIQ     2
asmlinkage void fiq_dump(struct pt_regs *regs, unsigned int esr)
{
	pr_crit("fiq_dump Begin\n");

	console_verbose();
	trigger_cpus_backtrace(regs);

	pr_crit("fiq_dump End\n");
	while (1);
=======
	return sys_ni_syscall();
}

static const char *esr_class_str[] = {
	[0 ... ESR_ELx_EC_MAX]		= "UNRECOGNIZED EC",
	[ESR_ELx_EC_UNKNOWN]		= "Unknown/Uncategorized",
	[ESR_ELx_EC_WFx]		= "WFI/WFE",
	[ESR_ELx_EC_CP15_32]		= "CP15 MCR/MRC",
	[ESR_ELx_EC_CP15_64]		= "CP15 MCRR/MRRC",
	[ESR_ELx_EC_CP14_MR]		= "CP14 MCR/MRC",
	[ESR_ELx_EC_CP14_LS]		= "CP14 LDC/STC",
	[ESR_ELx_EC_FP_ASIMD]		= "ASIMD",
	[ESR_ELx_EC_CP10_ID]		= "CP10 MRC/VMRS",
	[ESR_ELx_EC_CP14_64]		= "CP14 MCRR/MRRC",
	[ESR_ELx_EC_ILL]		= "PSTATE.IL",
	[ESR_ELx_EC_SVC32]		= "SVC (AArch32)",
	[ESR_ELx_EC_HVC32]		= "HVC (AArch32)",
	[ESR_ELx_EC_SMC32]		= "SMC (AArch32)",
	[ESR_ELx_EC_SVC64]		= "SVC (AArch64)",
	[ESR_ELx_EC_HVC64]		= "HVC (AArch64)",
	[ESR_ELx_EC_SMC64]		= "SMC (AArch64)",
	[ESR_ELx_EC_SYS64]		= "MSR/MRS (AArch64)",
	[ESR_ELx_EC_IMP_DEF]		= "EL3 IMP DEF",
	[ESR_ELx_EC_IABT_LOW]		= "IABT (lower EL)",
	[ESR_ELx_EC_IABT_CUR]		= "IABT (current EL)",
	[ESR_ELx_EC_PC_ALIGN]		= "PC Alignment",
	[ESR_ELx_EC_DABT_LOW]		= "DABT (lower EL)",
	[ESR_ELx_EC_DABT_CUR]		= "DABT (current EL)",
	[ESR_ELx_EC_SP_ALIGN]		= "SP Alignment",
	[ESR_ELx_EC_FP_EXC32]		= "FP (AArch32)",
	[ESR_ELx_EC_FP_EXC64]		= "FP (AArch64)",
	[ESR_ELx_EC_SERROR]		= "SError",
	[ESR_ELx_EC_BREAKPT_LOW]	= "Breakpoint (lower EL)",
	[ESR_ELx_EC_BREAKPT_CUR]	= "Breakpoint (current EL)",
	[ESR_ELx_EC_SOFTSTP_LOW]	= "Software Step (lower EL)",
	[ESR_ELx_EC_SOFTSTP_CUR]	= "Software Step (current EL)",
	[ESR_ELx_EC_WATCHPT_LOW]	= "Watchpoint (lower EL)",
	[ESR_ELx_EC_WATCHPT_CUR]	= "Watchpoint (current EL)",
	[ESR_ELx_EC_BKPT32]		= "BKPT (AArch32)",
	[ESR_ELx_EC_VECTOR32]		= "Vector catch (AArch32)",
	[ESR_ELx_EC_BRK64]		= "BRK (AArch64)",
};

const char *esr_get_class_string(u32 esr)
{
	return esr_class_str[ESR_ELx_EC(esr)];
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/*
 * bad_mode handles the impossible case in the exception vector. This is always
 * fatal.
 */
asmlinkage void bad_mode(struct pt_regs *regs, int reason, unsigned int esr)
{
	console_verbose();

<<<<<<< HEAD
	pr_crit("Bad mode in %s handler detected, code 0x%08x\n",
		handler[reason], esr);

	die("Oops - bad mode", regs, 0);
=======
	pr_crit("Bad mode in %s handler detected on CPU%d, code 0x%08x -- %s\n",
		handler[reason], smp_processor_id(), esr,
		esr_get_class_string(esr));

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	local_irq_disable();
	panic("bad mode");
}

/*
 * bad_el0_sync handles unexpected, but potentially recoverable synchronous
 * exceptions taken from EL0. Unlike bad_mode, this returns.
 */
asmlinkage void bad_el0_sync(struct pt_regs *regs, int reason, unsigned int esr)
{
	siginfo_t info;
	void __user *pc = (void __user *)instruction_pointer(regs);
	console_verbose();

<<<<<<< HEAD
	pr_crit("Bad EL0 synchronous exception detected on CPU%d, code 0x%08x\n",
		smp_processor_id(), esr);
=======
	pr_crit("Bad EL0 synchronous exception detected on CPU%d, code 0x%08x -- %s\n",
		smp_processor_id(), esr, esr_get_class_string(esr));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	__show_regs(regs);

	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLOPC;
	info.si_addr  = pc;

<<<<<<< HEAD
#ifdef CONFIG_HISI_RDR
	arm_exc_type = DUMP_ARM_VEC_RESET;
#endif
=======
	current->thread.fault_address = 0;
	current->thread.fault_code = 0;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	force_sig_info(info.si_signo, &info, current);
}

void __pte_error(const char *file, int line, unsigned long val)
{
<<<<<<< HEAD
	pr_crit("%s:%d: bad pte %016lx.\n", file, line, val);
=======
	pr_err("%s:%d: bad pte %016lx.\n", file, line, val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __pmd_error(const char *file, int line, unsigned long val)
{
<<<<<<< HEAD
	pr_crit("%s:%d: bad pmd %016lx.\n", file, line, val);
=======
	pr_err("%s:%d: bad pmd %016lx.\n", file, line, val);
}

void __pud_error(const char *file, int line, unsigned long val)
{
	pr_err("%s:%d: bad pud %016lx.\n", file, line, val);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

void __pgd_error(const char *file, int line, unsigned long val)
{
<<<<<<< HEAD
	pr_crit("%s:%d: bad pgd %016lx.\n", file, line, val);
}

void __init trap_init(void)
{
	return;
=======
	pr_err("%s:%d: bad pgd %016lx.\n", file, line, val);
}

/* GENERIC_BUG traps */

int is_valid_bugaddr(unsigned long addr)
{
	/*
	 * bug_handler() only called for BRK #BUG_BRK_IMM.
	 * So the answer is trivial -- any spurious instances with no
	 * bug table entry will be rejected by report_bug() and passed
	 * back to the debug-monitors code and handled as a fatal
	 * unexpected debug exception.
	 */
	return 1;
}

static int bug_handler(struct pt_regs *regs, unsigned int esr)
{
	if (user_mode(regs))
		return DBG_HOOK_ERROR;

	switch (report_bug(regs->pc, regs)) {
	case BUG_TRAP_TYPE_BUG:
		die("Oops - BUG", regs, 0);
		break;

	case BUG_TRAP_TYPE_WARN:
		/* Ideally, report_bug() should backtrace for us... but no. */
		dump_backtrace(regs, NULL);
		break;

	default:
		/* unknown/unrecognised bug trap type */
		return DBG_HOOK_ERROR;
	}

	/* If thread survives, skip over the BUG instruction and continue: */
	regs->pc += AARCH64_INSN_SIZE;	/* skip BRK and resume */
	return DBG_HOOK_HANDLED;
}

static struct break_hook bug_break_hook = {
	.esr_val = 0xf2000000 | BUG_BRK_IMM,
	.esr_mask = 0xffffffff,
	.fn = bug_handler,
};

/*
 * Initial handler for AArch64 BRK exceptions
 * This handler only used until debug_traps_init().
 */
int __init early_brk64(unsigned long addr, unsigned int esr,
		struct pt_regs *regs)
{
	return bug_handler(regs, esr) != DBG_HOOK_HANDLED;
}

/* This registration must happen early, before debug_traps_init(). */
void __init trap_init(void)
{
	register_break_hook(&bug_break_hook);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
