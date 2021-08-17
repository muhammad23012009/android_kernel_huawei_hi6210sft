/*
 * Dynamic function tracer architecture backend.
 *
<<<<<<< HEAD
 * Copyright IBM Corp. 2009
=======
 * Copyright IBM Corp. 2009,2014
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *		Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

<<<<<<< HEAD
=======
#include <linux/moduleloader.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/hardirq.h>
#include <linux/uaccess.h>
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kprobes.h>
#include <trace/syscall.h>
#include <asm/asm-offsets.h>
<<<<<<< HEAD

#ifdef CONFIG_DYNAMIC_FTRACE

void ftrace_disable_code(void);
void ftrace_enable_insn(void);

#ifdef CONFIG_64BIT
/*
 * The 64-bit mcount code looks like this:
 *	stg	%r14,8(%r15)		# offset 0
 * >	larl	%r1,<&counter>		# offset 6
 * >	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * Total length is 24 bytes. The middle two instructions of the mcount
 * block get overwritten by ftrace_make_nop / ftrace_make_call.
 * The 64-bit enabled ftrace code block looks like this:
 *	stg	%r14,8(%r15)		# offset 0
 * >	lg	%r1,__LC_FTRACE_FUNC	# offset 6
 * >	lgr	%r0,%r0			# offset 12
 * >	basr	%r14,%r1		# offset 16
 *	lg	%r14,8(%15)		# offset 18
 * The return points of the mcount/ftrace function have the same offset 18.
 * The 64-bit disable ftrace code block looks like this:
 *	stg	%r14,8(%r15)		# offset 0
 * >	jg	.+18			# offset 6
 * >	lgr	%r0,%r0			# offset 12
 * >	basr	%r14,%r1		# offset 16
 *	lg	%r14,8(%15)		# offset 18
 * The jg instruction branches to offset 24 to skip as many instructions
 * as possible.
 */
asm(
	"	.align	4\n"
	"ftrace_disable_code:\n"
	"	jg	0f\n"
	"	lgr	%r0,%r0\n"
	"	basr	%r14,%r1\n"
	"0:\n"
	"	.align	4\n"
	"ftrace_enable_insn:\n"
	"	lg	%r1,"__stringify(__LC_FTRACE_FUNC)"\n");

#define FTRACE_INSN_SIZE	6

#else /* CONFIG_64BIT */
/*
 * The 31-bit mcount code looks like this:
 *	st	%r14,4(%r15)		# offset 0
 * >	bras	%r1,0f			# offset 4
 * >	.long	_mcount			# offset 8
 * >	.long	<&counter>		# offset 12
 * > 0:	l	%r14,0(%r1)		# offset 16
 * >	l	%r1,4(%r1)		# offset 20
 *	basr	%r14,%r14		# offset 24
 *	l	%r14,4(%r15)		# offset 26
 * Total length is 30 bytes. The twenty bytes starting from offset 4
 * to offset 24 get overwritten by ftrace_make_nop / ftrace_make_call.
 * The 31-bit enabled ftrace code block looks like this:
 *	st	%r14,4(%r15)		# offset 0
 * >	l	%r14,__LC_FTRACE_FUNC	# offset 4
 * >	j	0f			# offset 8
 * >	.fill	12,1,0x07		# offset 12
 *   0:	basr	%r14,%r14		# offset 24
 *	l	%r14,4(%r14)		# offset 26
 * The return points of the mcount/ftrace function have the same offset 26.
 * The 31-bit disabled ftrace code block looks like this:
 *	st	%r14,4(%r15)		# offset 0
 * >	j	.+26			# offset 4
 * >	j	0f			# offset 8
 * >	.fill	12,1,0x07		# offset 12
 *   0:	basr	%r14,%r14		# offset 24
 *	l	%r14,4(%r14)		# offset 26
 * The j instruction branches to offset 30 to skip as many instructions
 * as possible.
 */
asm(
	"	.align	4\n"
	"ftrace_disable_code:\n"
	"	j	1f\n"
	"	j	0f\n"
	"	.fill	12,1,0x07\n"
	"0:	basr	%r14,%r14\n"
	"1:\n"
	"	.align	4\n"
	"ftrace_enable_insn:\n"
	"	l	%r14,"__stringify(__LC_FTRACE_FUNC)"\n");

#define FTRACE_INSN_SIZE	4

#endif /* CONFIG_64BIT */

=======
#include <asm/cacheflush.h>
#include "entry.h"

/*
 * The mcount code looks like this:
 *	stg	%r14,8(%r15)		# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * Total length is 24 bytes. Only the first instruction will be patched
 * by ftrace_make_call / ftrace_make_nop.
 * The enabled ftrace code block looks like this:
 * >	brasl	%r0,ftrace_caller	# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * The ftrace function gets called with a non-standard C function call ABI
 * where r0 contains the return address. It is also expected that the called
 * function only clobbers r0 and r1, but restores r2-r15.
 * For module code we can't directly jump to ftrace caller, but need a
 * trampoline (ftrace_plt), which clobbers also r1.
 * The return point of the ftrace function has offset 24, so execution
 * continues behind the mcount block.
 * The disabled ftrace code block looks like this:
 * >	jg	.+24			# offset 0
 *	larl	%r1,<&counter>		# offset 6
 *	brasl	%r14,_mcount		# offset 12
 *	lg	%r14,8(%r15)		# offset 18
 * The jg instruction branches to offset 24 to skip as many instructions
 * as possible.
 * In case we use gcc's hotpatch feature the original and also the disabled
 * function prologue contains only a single six byte instruction and looks
 * like this:
 * >	brcl	0,0			# offset 0
 * To enable ftrace the code gets patched like above and afterwards looks
 * like this:
 * >	brasl	%r0,ftrace_caller	# offset 0
 */

void *ftrace_func __read_mostly = ftrace_stub;
unsigned long ftrace_plt;

static inline void ftrace_generate_orig_insn(struct ftrace_insn *insn)
{
#ifdef CC_USING_HOTPATCH
	/* brcl 0,0 */
	insn->opc = 0xc004;
	insn->disp = 0;
#else
	/* stg r14,8(r15) */
	insn->opc = 0xe3e0;
	insn->disp = 0xf0080024;
#endif
}

static inline int is_kprobe_on_ftrace(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	if (insn->opc == BREAKPOINT_INSTRUCTION)
		return 1;
#endif
	return 0;
}

static inline void ftrace_generate_kprobe_nop_insn(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	insn->opc = BREAKPOINT_INSTRUCTION;
	insn->disp = KPROBE_ON_FTRACE_NOP;
#endif
}

static inline void ftrace_generate_kprobe_call_insn(struct ftrace_insn *insn)
{
#ifdef CONFIG_KPROBES
	insn->opc = BREAKPOINT_INSTRUCTION;
	insn->disp = KPROBE_ON_FTRACE_CALL;
#endif
}

int ftrace_modify_call(struct dyn_ftrace *rec, unsigned long old_addr,
		       unsigned long addr)
{
	return 0;
}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

int ftrace_make_nop(struct module *mod, struct dyn_ftrace *rec,
		    unsigned long addr)
{
<<<<<<< HEAD
	if (probe_kernel_write((void *) rec->ip, ftrace_disable_code,
			       MCOUNT_INSN_SIZE))
		return -EPERM;
=======
	struct ftrace_insn orig, new, old;

	if (probe_kernel_read(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	if (addr == MCOUNT_ADDR) {
		/* Initial code replacement */
		ftrace_generate_orig_insn(&orig);
		ftrace_generate_nop_insn(&new);
	} else if (is_kprobe_on_ftrace(&old)) {
		/*
		 * If we find a breakpoint instruction, a kprobe has been
		 * placed at the beginning of the function. We write the
		 * constant KPROBE_ON_FTRACE_NOP into the remaining four
		 * bytes of the original instruction so that the kprobes
		 * handler can execute a nop, if it reaches this breakpoint.
		 */
		ftrace_generate_kprobe_call_insn(&orig);
		ftrace_generate_kprobe_nop_insn(&new);
	} else {
		/* Replace ftrace call with a nop. */
		ftrace_generate_call_insn(&orig, rec->ip);
		ftrace_generate_nop_insn(&new);
	}
	/* Verify that the to be replaced code matches what we expect. */
	if (memcmp(&orig, &old, sizeof(old)))
		return -EINVAL;
	s390_kernel_write((void *) rec->ip, &new, sizeof(new));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr)
{
<<<<<<< HEAD
	if (probe_kernel_write((void *) rec->ip, ftrace_enable_insn,
			       FTRACE_INSN_SIZE))
		return -EPERM;
=======
	struct ftrace_insn orig, new, old;

	if (probe_kernel_read(&old, (void *) rec->ip, sizeof(old)))
		return -EFAULT;
	if (is_kprobe_on_ftrace(&old)) {
		/*
		 * If we find a breakpoint instruction, a kprobe has been
		 * placed at the beginning of the function. We write the
		 * constant KPROBE_ON_FTRACE_CALL into the remaining four
		 * bytes of the original instruction so that the kprobes
		 * handler can execute a brasl if it reaches this breakpoint.
		 */
		ftrace_generate_kprobe_nop_insn(&orig);
		ftrace_generate_kprobe_call_insn(&new);
	} else {
		/* Replace nop with an ftrace call. */
		ftrace_generate_nop_insn(&orig);
		ftrace_generate_call_insn(&new, rec->ip);
	}
	/* Verify that the to be replaced code matches what we expect. */
	if (memcmp(&orig, &old, sizeof(old)))
		return -EINVAL;
	s390_kernel_write((void *) rec->ip, &new, sizeof(new));
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return 0;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
<<<<<<< HEAD
	return 0;
}

int __init ftrace_dyn_arch_init(void *data)
{
	*(unsigned long *) data = 0;
	return 0;
}

#endif /* CONFIG_DYNAMIC_FTRACE */
=======
	ftrace_func = func;
	return 0;
}

int __init ftrace_dyn_arch_init(void)
{
	return 0;
}

static int __init ftrace_plt_init(void)
{
	unsigned int *ip;

	ftrace_plt = (unsigned long) module_alloc(PAGE_SIZE);
	if (!ftrace_plt)
		panic("cannot allocate ftrace plt\n");
	ip = (unsigned int *) ftrace_plt;
	ip[0] = 0x0d10e310; /* basr 1,0; lg 1,10(1); br 1 */
	ip[1] = 0x100a0004;
	ip[2] = 0x07f10000;
	ip[3] = FTRACE_ADDR >> 32;
	ip[4] = FTRACE_ADDR & 0xffffffff;
	set_memory_ro(ftrace_plt, 1);
	return 0;
}
device_initcall(ftrace_plt_init);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
/*
 * Hook the return address and push it in the stack of return addresses
 * in current thread info.
 */
<<<<<<< HEAD
unsigned long __kprobes prepare_ftrace_return(unsigned long parent,
					      unsigned long ip)
{
	struct ftrace_graph_ent trace;

	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		goto out;
	ip = (ip & PSW_ADDR_INSN) - MCOUNT_INSN_SIZE;
	if (ftrace_push_return_trace(parent, ip, &trace.depth, 0) == -EBUSY)
		goto out;
	trace.func = ip;
	/* Only trace if the calling function expects to. */
	if (!ftrace_graph_entry(&trace)) {
		current->curr_ret_stack--;
		goto out;
	}
=======
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long ip)
{
	struct ftrace_graph_ent trace;

	if (unlikely(ftrace_graph_is_dead()))
		goto out;
	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		goto out;
	ip -= MCOUNT_INSN_SIZE;
	trace.func = ip;
	trace.depth = current->curr_ret_stack + 1;
	/* Only trace if the calling function expects to. */
	if (!ftrace_graph_entry(&trace))
		goto out;
	if (ftrace_push_return_trace(parent, ip, &trace.depth, 0,
				     NULL) == -EBUSY)
		goto out;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	parent = (unsigned long) return_to_handler;
out:
	return parent;
}
<<<<<<< HEAD

#ifdef CONFIG_DYNAMIC_FTRACE
/*
 * Patch the kernel code at ftrace_graph_caller location. The instruction
 * there is branch relative and save to prepare_ftrace_return. To disable
 * the call to prepare_ftrace_return we patch the bras offset to point
 * directly after the instructions. To enable the call we calculate
 * the original offset to prepare_ftrace_return and put it back.
 */
int ftrace_enable_ftrace_graph_caller(void)
{
	unsigned short offset;

	offset = ((void *) prepare_ftrace_return -
		  (void *) ftrace_graph_caller) / 2;
	return probe_kernel_write(ftrace_graph_caller + 2,
				  &offset, sizeof(offset));
=======
NOKPROBE_SYMBOL(prepare_ftrace_return);

/*
 * Patch the kernel code at ftrace_graph_caller location. The instruction
 * there is branch relative on condition. To enable the ftrace graph code
 * block, we simply patch the mask field of the instruction to zero and
 * turn the instruction into a nop.
 * To disable the ftrace graph code the mask field will be patched to
 * all ones, which turns the instruction into an unconditional branch.
 */
int ftrace_enable_ftrace_graph_caller(void)
{
	u8 op = 0x04; /* set mask field to zero */

	s390_kernel_write(__va(ftrace_graph_caller)+1, &op, sizeof(op));
	return 0;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int ftrace_disable_ftrace_graph_caller(void)
{
<<<<<<< HEAD
	static unsigned short offset = 0x0002;

	return probe_kernel_write(ftrace_graph_caller + 2,
				  &offset, sizeof(offset));
}

#endif /* CONFIG_DYNAMIC_FTRACE */
=======
	u8 op = 0xf4; /* set mask field to all ones */

	s390_kernel_write(__va(ftrace_graph_caller)+1, &op, sizeof(op));
	return 0;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* CONFIG_FUNCTION_GRAPH_TRACER */
