#ifndef _LINUX_ERR_H
#define _LINUX_ERR_H

#include <linux/compiler.h>
<<<<<<< HEAD
=======
#include <linux/types.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

#include <asm/errno.h>

/*
 * Kernel pointers have redundant information, so we can use a
<<<<<<< HEAD
 * scheme where we can return either an error code or a dentry
=======
 * scheme where we can return either an error code or a normal
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
 * pointer with the same return value.
 *
 * This should be a per-architecture thing, to allow different
 * error and pointer decisions.
 */
#define MAX_ERRNO	4095

#ifndef __ASSEMBLY__

<<<<<<< HEAD
#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)
=======
#define IS_ERR_VALUE(x) unlikely((unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

static inline void * __must_check ERR_PTR(long error)
{
	return (void *) error;
}

<<<<<<< HEAD
static inline long __must_check PTR_ERR(const void *ptr)
=======
static inline long __must_check PTR_ERR(__force const void *ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return (long) ptr;
}

<<<<<<< HEAD
static inline long __must_check IS_ERR(const void *ptr)
=======
static inline bool __must_check IS_ERR(__force const void *ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return IS_ERR_VALUE((unsigned long)ptr);
}

<<<<<<< HEAD
static inline long __must_check IS_ERR_OR_NULL(const void *ptr)
{
	return !ptr || IS_ERR_VALUE((unsigned long)ptr);
=======
static inline bool __must_check IS_ERR_OR_NULL(__force const void *ptr)
{
	return unlikely(!ptr) || IS_ERR_VALUE((unsigned long)ptr);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

/**
 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 * @ptr: The pointer to cast.
 *
 * Explicitly cast an error-valued pointer to another pointer type in such a
 * way as to make it clear that's what's going on.
 */
<<<<<<< HEAD
static inline void * __must_check ERR_CAST(const void *ptr)
=======
static inline void * __must_check ERR_CAST(__force const void *ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	/* cast away the const */
	return (void *) ptr;
}

<<<<<<< HEAD
static inline int __must_check PTR_RET(const void *ptr)
=======
static inline int __must_check PTR_ERR_OR_ZERO(__force const void *ptr)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	if (IS_ERR(ptr))
		return PTR_ERR(ptr);
	else
		return 0;
}

<<<<<<< HEAD
=======
/* Deprecated */
#define PTR_RET(p) PTR_ERR_OR_ZERO(p)

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif

#endif /* _LINUX_ERR_H */
