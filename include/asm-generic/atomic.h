/*
 * Generic C implementation of atomic counter operations. Usable on
 * UP systems only. Do not include in machine independent code.
 *
 * Originally implemented for MN10300.
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef __ASM_GENERIC_ATOMIC_H
#define __ASM_GENERIC_ATOMIC_H

#include <asm/cmpxchg.h>
<<<<<<< HEAD

#ifdef CONFIG_SMP
/* Force people to define core atomics */
# if !defined(atomic_add_return) || !defined(atomic_sub_return) || \
     !defined(atomic_clear_mask) || !defined(atomic_set_mask)
#  error "SMP requires a little arch-specific magic"
# endif
#endif

=======
#include <asm/barrier.h>

/*
 * atomic_$op() - $op integer to atomic variable
 * @i: integer value to $op
 * @v: pointer to the atomic variable
 *
 * Atomically $ops @i to @v. Does not strictly guarantee a memory-barrier, use
 * smp_mb__{before,after}_atomic().
 */

/*
 * atomic_$op_return() - $op interer to atomic variable and returns the result
 * @i: integer value to $op
 * @v: pointer to the atomic variable
 *
 * Atomically $ops @i to @v. Does imply a full memory barrier.
 */

#ifdef CONFIG_SMP

/* we can build all atomic primitives from cmpxchg */

#define ATOMIC_OP(op, c_op)						\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
static inline int atomic_##op##_return(int i, atomic_t *v)		\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
									\
	return c c_op i;						\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
									\
	return c;							\
}

#else

#include <linux/irqflags.h>

#define ATOMIC_OP(op, c_op)						\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	unsigned long flags;						\
									\
	raw_local_irq_save(flags);					\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
static inline int atomic_##op##_return(int i, atomic_t *v)		\
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = (v->counter = v->counter c_op i);				\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = v->counter;						\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#endif /* CONFIG_SMP */

#ifndef atomic_add_return
ATOMIC_OP_RETURN(add, +)
#endif

#ifndef atomic_sub_return
ATOMIC_OP_RETURN(sub, -)
#endif

#ifndef atomic_fetch_add
ATOMIC_FETCH_OP(add, +)
#endif

#ifndef atomic_fetch_sub
ATOMIC_FETCH_OP(sub, -)
#endif

#ifndef atomic_fetch_and
ATOMIC_FETCH_OP(and, &)
#endif

#ifndef atomic_fetch_or
ATOMIC_FETCH_OP(or, |)
#endif

#ifndef atomic_fetch_xor
ATOMIC_FETCH_OP(xor, ^)
#endif

#ifndef atomic_and
ATOMIC_OP(and, &)
#endif

#ifndef atomic_or
ATOMIC_OP(or, |)
#endif

#ifndef atomic_xor
ATOMIC_OP(xor, ^)
#endif

#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define ATOMIC_INIT(i)	{ (i) }

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
#ifndef atomic_read
<<<<<<< HEAD
#define atomic_read(v)	(*(volatile int *)&(v)->counter)
#endif

#ifndef cpu_relaxed_read_atomic
#define cpu_relaxed_read_atomic(v) atomic_read(v)
=======
#define atomic_read(v)	READ_ONCE((v)->counter)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
<<<<<<< HEAD
#define atomic_set(v, i) (((v)->counter) = (i))

#include <linux/irqflags.h>

/**
 * atomic_add_return - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v and returns the result
 */
#ifndef atomic_add_return
static inline int atomic_add_return(int i, atomic_t *v)
{
	unsigned long flags;
	int temp;

	raw_local_irq_save(flags); /* Don't trace it in an irqsoff handler */
	temp = v->counter;
	temp += i;
	v->counter = temp;
	raw_local_irq_restore(flags);

	return temp;
}
#endif

/**
 * atomic_sub_return - subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns the result
 */
#ifndef atomic_sub_return
static inline int atomic_sub_return(int i, atomic_t *v)
{
	unsigned long flags;
	int temp;

	raw_local_irq_save(flags); /* Don't trace it in an irqsoff handler */
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	raw_local_irq_restore(flags);

	return temp;
}
#endif

=======
#define atomic_set(v, i) WRITE_ONCE(((v)->counter), (i))

#include <linux/irqflags.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
static inline int atomic_add_negative(int i, atomic_t *v)
{
	return atomic_add_return(i, v) < 0;
}

static inline void atomic_add(int i, atomic_t *v)
{
	atomic_add_return(i, v);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	atomic_sub_return(i, v);
}

static inline void atomic_inc(atomic_t *v)
{
	atomic_add_return(1, v);
}

static inline void atomic_dec(atomic_t *v)
{
	atomic_sub_return(1, v);
}

#define atomic_dec_return(v)		atomic_sub_return(1, (v))
#define atomic_inc_return(v)		atomic_add_return(1, (v))

#define atomic_sub_and_test(i, v)	(atomic_sub_return((i), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_dec_return(v) == 0)
#define atomic_inc_and_test(v)		(atomic_inc_return(v) == 0)

#define atomic_xchg(ptr, v)		(xchg(&(ptr)->counter, (v)))
#define atomic_cmpxchg(v, old, new)	(cmpxchg(&((v)->counter), (old), (new)))

static inline int __atomic_add_unless(atomic_t *v, int a, int u)
{
<<<<<<< HEAD
  int c, old;
  c = atomic_read(v);
  while (c != u && (old = atomic_cmpxchg(v, c, c + a)) != c)
    c = old;
  return c;
}

/**
 * atomic_clear_mask - Atomically clear bits in atomic variable
 * @mask: Mask of the bits to be cleared
 * @v: pointer of type atomic_t
 *
 * Atomically clears the bits set in @mask from @v
 */
#ifndef atomic_clear_mask
static inline void atomic_clear_mask(unsigned long mask, atomic_t *v)
{
	unsigned long flags;

	mask = ~mask;
	raw_local_irq_save(flags); /* Don't trace it in a irqsoff handler */
	v->counter &= mask;
	raw_local_irq_restore(flags);
}
#endif

/**
 * atomic_set_mask - Atomically set bits in atomic variable
 * @mask: Mask of the bits to be set
 * @v: pointer of type atomic_t
 *
 * Atomically sets the bits set in @mask in @v
 */
#ifndef atomic_set_mask
static inline void atomic_set_mask(unsigned int mask, atomic_t *v)
{
	unsigned long flags;

	raw_local_irq_save(flags); /* Don't trace it in a irqsoff handler */
	v->counter |= mask;
	raw_local_irq_restore(flags);
}
#endif

/* Assume that atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()

#endif /* __KERNEL__ */
=======
	int c, old;
	c = atomic_read(v);
	while (c != u && (old = atomic_cmpxchg(v, c, c + a)) != c)
		c = old;
	return c;
}

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* __ASM_GENERIC_ATOMIC_H */
