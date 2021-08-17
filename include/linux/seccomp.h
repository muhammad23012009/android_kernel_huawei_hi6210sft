#ifndef _LINUX_SECCOMP_H
#define _LINUX_SECCOMP_H

#include <uapi/linux/seccomp.h>

<<<<<<< HEAD
#define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC)
=======
#define SECCOMP_FILTER_FLAG_MASK	(SECCOMP_FILTER_FLAG_TSYNC	| \
					 SECCOMP_FILTER_FLAG_SPEC_ALLOW)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#ifdef CONFIG_SECCOMP

#include <linux/thread_info.h>
#include <asm/seccomp.h>

struct seccomp_filter;
/**
 * struct seccomp - the state of a seccomp'ed process
 *
 * @mode:  indicates one of the valid values above for controlled
 *         system calls available to a process.
 * @filter: must always point to a valid seccomp-filter or NULL as it is
 *          accessed without locking during system call entry.
 *
 *          @filter must only be accessed from the context of current as there
 *          is no read locking.
 */
struct seccomp {
	int mode;
	struct seccomp_filter *filter;
};

<<<<<<< HEAD
extern int __secure_computing(int);
static inline int secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(this_syscall);
	return 0;
}

/* A wrapper for architectures supporting only SECCOMP_MODE_STRICT. */
static inline void secure_computing_strict(int this_syscall)
{
	BUG_ON(secure_computing(this_syscall) != 0);
}
=======
#ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
extern int __secure_computing(const struct seccomp_data *sd);
static inline int secure_computing(const struct seccomp_data *sd)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(sd);
	return 0;
}
#else
extern void secure_computing_strict(int this_syscall);
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

extern long prctl_get_seccomp(void);
extern long prctl_set_seccomp(unsigned long, char __user *);

static inline int seccomp_mode(struct seccomp *s)
{
	return s->mode;
}

#else /* CONFIG_SECCOMP */

#include <linux/errno.h>

struct seccomp { };
struct seccomp_filter { };

<<<<<<< HEAD
static inline int secure_computing(int this_syscall) { return 0; }
static inline void secure_computing_strict(int this_syscall) { return; }
=======
#ifdef CONFIG_HAVE_ARCH_SECCOMP_FILTER
static inline int secure_computing(struct seccomp_data *sd) { return 0; }
#else
static inline void secure_computing_strict(int this_syscall) { return; }
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline long prctl_get_seccomp(void)
{
	return -EINVAL;
}

static inline long prctl_set_seccomp(unsigned long arg2, char __user *arg3)
{
	return -EINVAL;
}

static inline int seccomp_mode(struct seccomp *s)
{
<<<<<<< HEAD
	return 0;
=======
	return SECCOMP_MODE_DISABLED;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}
#endif /* CONFIG_SECCOMP */

#ifdef CONFIG_SECCOMP_FILTER
extern void put_seccomp_filter(struct task_struct *tsk);
extern void get_seccomp_filter(struct task_struct *tsk);
<<<<<<< HEAD
extern u32 seccomp_bpf_load(int off);
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else  /* CONFIG_SECCOMP_FILTER */
static inline void put_seccomp_filter(struct task_struct *tsk)
{
	return;
}
static inline void get_seccomp_filter(struct task_struct *tsk)
{
	return;
}
#endif /* CONFIG_SECCOMP_FILTER */
<<<<<<< HEAD
=======

#if defined(CONFIG_SECCOMP_FILTER) && defined(CONFIG_CHECKPOINT_RESTORE)
extern long seccomp_get_filter(struct task_struct *task,
			       unsigned long filter_off, void __user *data);
#else
static inline long seccomp_get_filter(struct task_struct *task,
				      unsigned long n, void __user *data)
{
	return -EINVAL;
}
#endif /* CONFIG_SECCOMP_FILTER && CONFIG_CHECKPOINT_RESTORE */
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif /* _LINUX_SECCOMP_H */
