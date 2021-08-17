#if defined(__i386__) || defined(__x86_64__)
#define barrier() asm volatile("" ::: "memory")
<<<<<<< HEAD
#define mb() __sync_synchronize()

#define smp_mb()	mb()
# define smp_rmb()	barrier()
# define smp_wmb()	barrier()
/* Weak barriers should be used. If not - it's a bug */
# define rmb()	abort()
# define wmb()	abort()
=======
#define virt_mb() __sync_synchronize()
#define virt_rmb() barrier()
#define virt_wmb() barrier()
/* Atomic store should be enough, but gcc generates worse code in that case. */
#define virt_store_mb(var, value)  do { \
	typeof(var) virt_store_mb_value = (value); \
	__atomic_exchange(&(var), &virt_store_mb_value, &virt_store_mb_value, \
			  __ATOMIC_SEQ_CST); \
	barrier(); \
} while (0);
/* Weak barriers should be used. If not - it's a bug */
# define mb() abort()
# define rmb() abort()
# define wmb() abort()
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#else
#error Please fill in barrier macros
#endif

