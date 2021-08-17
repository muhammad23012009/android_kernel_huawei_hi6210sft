/*
 * Copyright (C) 2012 ARM Ltd.
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
#ifndef __ASM_FUTEX_H
#define __ASM_FUTEX_H

#ifdef __KERNEL__

#include <linux/futex.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <asm/errno.h>

#define __futex_atomic_op(insn, ret, oldval, uaddr, tmp, oparg)		\
	asm volatile(							\
"1:	ldxr	%w1, %2\n"						\
	insn "\n"							\
"2:	stlxr	%w3, %w0, %2\n"						\
"	cbnz	%w3, 1b\n"						\
"	dmb	ish\n"							\
"3:\n"									\
"	.pushsection .fixup,\"ax\"\n"					\
"	.align	2\n"							\
"4:	mov	%w0, %w5\n"						\
"	b	3b\n"							\
"	.popsection\n"							\
"	.pushsection __ex_table,\"a\"\n"				\
"	.align	3\n"							\
"	.quad	1b, 4b, 2b, 4b\n"					\
"	.popsection\n"							\
	: "=&r" (ret), "=&r" (oldval), "+Q" (*uaddr), "=&r" (tmp)	\
	: "r" (oparg), "Ir" (-EFAULT)					\
	: "memory")

static inline int
futex_atomic_op_inuser (int encoded_op, u32 __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval = 0, ret, tmp;

	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

	pagefault_disable();	/* implies preempt_disable() */

	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op("mov	%w0, %w4",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op("add	%w0, %w1, %w4",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op("orr	%w0, %w1, %w4",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op("and	%w0, %w1, %w4",
				  ret, oldval, uaddr, tmp, ~oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op("eor	%w0, %w1, %w4",
=======

#include <asm/errno.h>

#define FUTEX_MAX_LOOPS	128 /* What's the largest number you can think of? */

#define __futex_atomic_op(insn, ret, oldval, uaddr, tmp, oparg)		\
do {									\
	unsigned int loops = FUTEX_MAX_LOOPS;				\
	uaccess_enable();						\
									\
	asm volatile(							\
"	prfm	pstl1strm, %2\n"					\
"1:	ldxr	%w1, %2\n"						\
	insn "\n"							\
"2:	stlxr	%w0, %w3, %2\n"						\
"	cbz	%w0, 3f\n"						\
"	sub	%w4, %w4, %w0\n"					\
"	cbnz	%w4, 1b\n"						\
"	mov	%w0, %w7\n"						\
"3:\n"									\
"	dmb	ish\n"							\
"	.pushsection .fixup,\"ax\"\n"					\
"	.align	2\n"							\
"4:	mov	%w0, %w6\n"						\
"	b	3b\n"							\
"	.popsection\n"							\
	_ASM_EXTABLE(1b, 4b)						\
	_ASM_EXTABLE(2b, 4b)						\
	: "=&r" (ret), "=&r" (oldval), "+Q" (*uaddr), "=&r" (tmp),	\
	  "+r" (loops)							\
	: "r" (oparg), "Ir" (-EFAULT), "Ir" (-EAGAIN)			\
	: "memory");							\
	uaccess_disable();						\
} while (0)

static inline int
arch_futex_atomic_op_inuser(int op, int oparg, int *oval, u32 __user *uaddr)
{
	int oldval = 0, ret, tmp;

	pagefault_disable();

	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op("mov	%w3, %w5",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op("add	%w3, %w1, %w5",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op("orr	%w3, %w1, %w5",
				  ret, oldval, uaddr, tmp, oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op("and	%w3, %w1, %w5",
				  ret, oldval, uaddr, tmp, ~oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op("eor	%w3, %w1, %w5",
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
				  ret, oldval, uaddr, tmp, oparg);
		break;
	default:
		ret = -ENOSYS;
	}

<<<<<<< HEAD
	pagefault_enable();	/* subsumes preempt_enable() */

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ: ret = (oldval == cmparg); break;
		case FUTEX_OP_CMP_NE: ret = (oldval != cmparg); break;
		case FUTEX_OP_CMP_LT: ret = (oldval < cmparg); break;
		case FUTEX_OP_CMP_GE: ret = (oldval >= cmparg); break;
		case FUTEX_OP_CMP_LE: ret = (oldval <= cmparg); break;
		case FUTEX_OP_CMP_GT: ret = (oldval > cmparg); break;
		default: ret = -ENOSYS;
		}
	}
=======
	pagefault_enable();

	if (!ret)
		*oval = oldval;

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return ret;
}

static inline int
<<<<<<< HEAD
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *uaddr,
			      u32 oldval, u32 newval)
{
	int ret = 0;
	u32 val, tmp;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(u32)))
		return -EFAULT;

	asm volatile("// futex_atomic_cmpxchg_inatomic\n"
"1:	ldxr	%w1, %2\n"
"	sub	%w3, %w1, %w4\n"
"	cbnz	%w3, 3f\n"
"2:	stlxr	%w3, %w5, %2\n"
"	cbnz	%w3, 1b\n"
"	dmb	ish\n"
"3:\n"
"	.pushsection .fixup,\"ax\"\n"
"4:	mov	%w0, %w6\n"
"	b	3b\n"
"	.popsection\n"
"	.pushsection __ex_table,\"a\"\n"
"	.align	3\n"
"	.quad	1b, 4b, 2b, 4b\n"
"	.popsection\n"
	: "+r" (ret), "=&r" (val), "+Q" (*uaddr), "=&r" (tmp)
	: "r" (oldval), "r" (newval), "Ir" (-EFAULT)
	: "memory");
=======
futex_atomic_cmpxchg_inatomic(u32 *uval, u32 __user *_uaddr,
			      u32 oldval, u32 newval)
{
	int ret = 0;
	unsigned int loops = FUTEX_MAX_LOOPS;
	u32 val, tmp;
	u32 __user *uaddr;

	if (!access_ok(VERIFY_WRITE, _uaddr, sizeof(u32)))
		return -EFAULT;

	uaddr = __uaccess_mask_ptr(_uaddr);
	uaccess_enable();
	asm volatile("// futex_atomic_cmpxchg_inatomic\n"
"	prfm	pstl1strm, %2\n"
"1:	ldxr	%w1, %2\n"
"	sub	%w3, %w1, %w5\n"
"	cbnz	%w3, 4f\n"
"2:	stlxr	%w3, %w6, %2\n"
"	cbz	%w3, 3f\n"
"	sub	%w4, %w4, %w3\n"
"	cbnz	%w4, 1b\n"
"	mov	%w0, %w8\n"
"3:\n"
"	dmb	ish\n"
"4:\n"
"	.pushsection .fixup,\"ax\"\n"
"5:	mov	%w0, %w7\n"
"	b	4b\n"
"	.popsection\n"
	_ASM_EXTABLE(1b, 5b)
	_ASM_EXTABLE(2b, 5b)
	: "+r" (ret), "=&r" (val), "+Q" (*uaddr), "=&r" (tmp), "+r" (loops)
	: "r" (oldval), "r" (newval), "Ir" (-EFAULT), "Ir" (-EAGAIN)
	: "memory");
	uaccess_disable();
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	*uval = val;
	return ret;
}

#endif /* __KERNEL__ */
#endif /* __ASM_FUTEX_H */
