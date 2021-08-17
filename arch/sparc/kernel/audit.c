#include <linux/init.h>
#include <linux/types.h>
#include <linux/audit.h>
#include <asm/unistd.h>

<<<<<<< HEAD
static unsigned dir_class[] = {
=======
#include "kernel.h"

static unsigned int dir_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_dir_write.h>
~0U
};

<<<<<<< HEAD
static unsigned read_class[] = {
=======
static unsigned int read_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_read.h>
~0U
};

<<<<<<< HEAD
static unsigned write_class[] = {
=======
static unsigned int write_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_write.h>
~0U
};

<<<<<<< HEAD
static unsigned chattr_class[] = {
=======
static unsigned int chattr_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_change_attr.h>
~0U
};

<<<<<<< HEAD
static unsigned signal_class[] = {
=======
static unsigned int signal_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_signal.h>
~0U
};

int audit_classify_arch(int arch)
{
#ifdef CONFIG_COMPAT
	if (arch == AUDIT_ARCH_SPARC)
		return 1;
#endif
	return 0;
}

<<<<<<< HEAD
int audit_classify_syscall(int abi, unsigned syscall)
{
#ifdef CONFIG_COMPAT
	extern int sparc32_classify_syscall(unsigned);
=======
int audit_classify_syscall(int abi, unsigned int syscall)
{
#ifdef CONFIG_COMPAT
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (abi == AUDIT_ARCH_SPARC)
		return sparc32_classify_syscall(syscall);
#endif
	switch(syscall) {
	case __NR_open:
		return 2;
	case __NR_openat:
		return 3;
	case __NR_socketcall:
		return 4;
	case __NR_execve:
		return 5;
	default:
		return 0;
	}
}

static int __init audit_classes_init(void)
{
#ifdef CONFIG_COMPAT
<<<<<<< HEAD
	extern __u32 sparc32_dir_class[];
	extern __u32 sparc32_write_class[];
	extern __u32 sparc32_read_class[];
	extern __u32 sparc32_chattr_class[];
	extern __u32 sparc32_signal_class[];
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	audit_register_class(AUDIT_CLASS_WRITE_32, sparc32_write_class);
	audit_register_class(AUDIT_CLASS_READ_32, sparc32_read_class);
	audit_register_class(AUDIT_CLASS_DIR_WRITE_32, sparc32_dir_class);
	audit_register_class(AUDIT_CLASS_CHATTR_32, sparc32_chattr_class);
	audit_register_class(AUDIT_CLASS_SIGNAL_32, sparc32_signal_class);
#endif
	audit_register_class(AUDIT_CLASS_WRITE, write_class);
	audit_register_class(AUDIT_CLASS_READ, read_class);
	audit_register_class(AUDIT_CLASS_DIR_WRITE, dir_class);
	audit_register_class(AUDIT_CLASS_CHATTR, chattr_class);
	audit_register_class(AUDIT_CLASS_SIGNAL, signal_class);
	return 0;
}

__initcall(audit_classes_init);
