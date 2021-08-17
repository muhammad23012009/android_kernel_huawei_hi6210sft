/*
 * Copyright IBM Corp. 2008, 2009
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *
 */

#include <linux/linkage.h>
#include <asm/asm-offsets.h>
#include <asm/ftrace.h>
<<<<<<< HEAD
=======
#include <asm/nospec-insn.h>
#include <asm/ptrace.h>
#include <asm/export.h>

	GEN_BR_THUNK %r1
	GEN_BR_THUNK %r14
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	.section .kprobes.text, "ax"

ENTRY(ftrace_stub)
<<<<<<< HEAD
	br	%r14

ENTRY(_mcount)
#ifdef CONFIG_DYNAMIC_FTRACE
	br	%r14

ENTRY(ftrace_caller)
#endif
	stm	%r2,%r5,16(%r15)
	bras	%r1,2f
0:	.long	ftrace_trace_function
1:	.long	function_trace_stop
2:	l	%r2,1b-0b(%r1)
	icm	%r2,0xf,0(%r2)
	jnz	3f
	st	%r14,56(%r15)
	lr	%r0,%r15
	ahi	%r15,-96
	l	%r3,100(%r15)
	la	%r2,0(%r14)
	st	%r0,__SF_BACKCHAIN(%r15)
	la	%r3,0(%r3)
	ahi	%r2,-MCOUNT_INSN_SIZE
	l	%r14,0b-0b(%r1)
	l	%r14,0(%r14)
	basr	%r14,%r14
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	l	%r2,100(%r15)
	l	%r3,152(%r15)
ENTRY(ftrace_graph_caller)
# The bras instruction gets runtime patched to call prepare_ftrace_return.
# See ftrace_enable_ftrace_graph_caller. The patched instruction is:
#	bras	%r14,prepare_ftrace_return
	bras	%r14,0f
0:	st	%r2,100(%r15)
#endif
	ahi	%r15,96
	l	%r14,56(%r15)
3:	lm	%r2,%r5,16(%r15)
	br	%r14
=======
	BR_EX	%r14

#define STACK_FRAME_SIZE  (STACK_FRAME_OVERHEAD + __PT_SIZE)
#define STACK_PTREGS	  (STACK_FRAME_OVERHEAD)
#define STACK_PTREGS_GPRS (STACK_PTREGS + __PT_GPRS)
#define STACK_PTREGS_PSW  (STACK_PTREGS + __PT_PSW)
#ifdef __PACK_STACK
/* allocate just enough for r14, r15 and backchain */
#define TRACED_FUNC_FRAME_SIZE	24
#else
#define TRACED_FUNC_FRAME_SIZE	STACK_FRAME_OVERHEAD
#endif

ENTRY(_mcount)
	BR_EX	%r14

EXPORT_SYMBOL(_mcount)

ENTRY(ftrace_caller)
	.globl	ftrace_regs_caller
	.set	ftrace_regs_caller,ftrace_caller
	stg	%r14,(__SF_GPRS+8*8)(%r15)	# save traced function caller
	lgr	%r1,%r15
#ifndef CC_USING_HOTPATCH
	aghi	%r0,MCOUNT_RETURN_FIXUP
#endif
	# allocate stack frame for ftrace_caller to contain traced function
	aghi	%r15,-TRACED_FUNC_FRAME_SIZE
	stg	%r1,__SF_BACKCHAIN(%r15)
	stg	%r0,(__SF_GPRS+8*8)(%r15)
	stg	%r15,(__SF_GPRS+9*8)(%r15)
	# allocate pt_regs and stack frame for ftrace_trace_function
	aghi	%r15,-STACK_FRAME_SIZE
	stg	%r1,(STACK_PTREGS_GPRS+15*8)(%r15)
	aghi	%r1,-TRACED_FUNC_FRAME_SIZE
	stg	%r1,__SF_BACKCHAIN(%r15)
	stg	%r0,(STACK_PTREGS_PSW+8)(%r15)
	stmg	%r2,%r14,(STACK_PTREGS_GPRS+2*8)(%r15)
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	aghik	%r2,%r0,-MCOUNT_INSN_SIZE
	lgrl	%r4,function_trace_op
	lgrl	%r1,ftrace_func
#else
	lgr	%r2,%r0
	aghi	%r2,-MCOUNT_INSN_SIZE
	larl	%r4,function_trace_op
	lg	%r4,0(%r4)
	larl	%r1,ftrace_func
	lg	%r1,0(%r1)
#endif
	lgr	%r3,%r14
	la	%r5,STACK_PTREGS(%r15)
	BASR_EX	%r14,%r1
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
# The j instruction gets runtime patched to a nop instruction.
# See ftrace_enable_ftrace_graph_caller.
ENTRY(ftrace_graph_caller)
	j	ftrace_graph_caller_end
	lg	%r2,(STACK_PTREGS_GPRS+14*8)(%r15)
	lg	%r3,(STACK_PTREGS_PSW+8)(%r15)
	brasl	%r14,prepare_ftrace_return
	stg	%r2,(STACK_PTREGS_GPRS+14*8)(%r15)
ftrace_graph_caller_end:
	.globl	ftrace_graph_caller_end
#endif
	lg	%r1,(STACK_PTREGS_PSW+8)(%r15)
	lmg	%r2,%r15,(STACK_PTREGS_GPRS+2*8)(%r15)
	BR_EX	%r1
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_FUNCTION_GRAPH_TRACER

ENTRY(return_to_handler)
<<<<<<< HEAD
	stm	%r2,%r5,16(%r15)
	st	%r14,56(%r15)
	lr	%r0,%r15
	ahi	%r15,-96
	st	%r0,__SF_BACKCHAIN(%r15)
	bras	%r1,0f
	.long	ftrace_return_to_handler
0:	l	%r2,0b-0b(%r1)
	basr	%r14,%r2
	lr	%r14,%r2
	ahi	%r15,96
	lm	%r2,%r5,16(%r15)
	br	%r14
=======
	stmg	%r2,%r5,32(%r15)
	lgr	%r1,%r15
	aghi	%r15,-STACK_FRAME_OVERHEAD
	stg	%r1,__SF_BACKCHAIN(%r15)
	brasl	%r14,ftrace_return_to_handler
	aghi	%r15,STACK_FRAME_OVERHEAD
	lgr	%r14,%r2
	lmg	%r2,%r5,32(%r15)
	BR_EX	%r14
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#endif
