#ifndef _PERF_TARGET_H
#define _PERF_TARGET_H

#include <stdbool.h>
#include <sys/types.h>

<<<<<<< HEAD
struct perf_target {
=======
struct target {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	const char   *pid;
	const char   *tid;
	const char   *cpu_list;
	const char   *uid_str;
	uid_t	     uid;
	bool	     system_wide;
	bool	     uses_mmap;
<<<<<<< HEAD
};

enum perf_target_errno {
	PERF_ERRNO_TARGET__SUCCESS		= 0,
=======
	bool	     default_per_cpu;
	bool	     per_thread;
};

enum target_errno {
	TARGET_ERRNO__SUCCESS		= 0,
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	/*
	 * Choose an arbitrary negative big number not to clash with standard
	 * errno since SUS requires the errno has distinct positive values.
	 * See 'Issue 6' in the link below.
	 *
	 * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
	 */
<<<<<<< HEAD
	__PERF_ERRNO_TARGET__START		= -10000,


	/* for perf_target__validate() */
	PERF_ERRNO_TARGET__PID_OVERRIDE_CPU	= __PERF_ERRNO_TARGET__START,
	PERF_ERRNO_TARGET__PID_OVERRIDE_UID,
	PERF_ERRNO_TARGET__UID_OVERRIDE_CPU,
	PERF_ERRNO_TARGET__PID_OVERRIDE_SYSTEM,
	PERF_ERRNO_TARGET__UID_OVERRIDE_SYSTEM,

	/* for perf_target__parse_uid() */
	PERF_ERRNO_TARGET__INVALID_UID,
	PERF_ERRNO_TARGET__USER_NOT_FOUND,

	__PERF_ERRNO_TARGET__END,
};

enum perf_target_errno perf_target__validate(struct perf_target *target);
enum perf_target_errno perf_target__parse_uid(struct perf_target *target);

int perf_target__strerror(struct perf_target *target, int errnum, char *buf,
			  size_t buflen);

static inline bool perf_target__has_task(struct perf_target *target)
=======
	__TARGET_ERRNO__START		= -10000,

	/* for target__validate() */
	TARGET_ERRNO__PID_OVERRIDE_CPU	= __TARGET_ERRNO__START,
	TARGET_ERRNO__PID_OVERRIDE_UID,
	TARGET_ERRNO__UID_OVERRIDE_CPU,
	TARGET_ERRNO__PID_OVERRIDE_SYSTEM,
	TARGET_ERRNO__UID_OVERRIDE_SYSTEM,
	TARGET_ERRNO__SYSTEM_OVERRIDE_THREAD,

	/* for target__parse_uid() */
	TARGET_ERRNO__INVALID_UID,
	TARGET_ERRNO__USER_NOT_FOUND,

	__TARGET_ERRNO__END,
};

enum target_errno target__validate(struct target *target);
enum target_errno target__parse_uid(struct target *target);

int target__strerror(struct target *target, int errnum, char *buf, size_t buflen);

static inline bool target__has_task(struct target *target)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return target->tid || target->pid || target->uid_str;
}

<<<<<<< HEAD
static inline bool perf_target__has_cpu(struct perf_target *target)
=======
static inline bool target__has_cpu(struct target *target)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return target->system_wide || target->cpu_list;
}

<<<<<<< HEAD
static inline bool perf_target__none(struct perf_target *target)
{
	return !perf_target__has_task(target) && !perf_target__has_cpu(target);
=======
static inline bool target__none(struct target *target)
{
	return !target__has_task(target) && !target__has_cpu(target);
}

static inline bool target__uses_dummy_map(struct target *target)
{
	bool use_dummy = false;

	if (target->default_per_cpu)
		use_dummy = target->per_thread ? true : false;
	else if (target__has_task(target) ||
	         (!target__has_cpu(target) && !target->uses_mmap))
		use_dummy = true;

	return use_dummy;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* _PERF_TARGET_H */
