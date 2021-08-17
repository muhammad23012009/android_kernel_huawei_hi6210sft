#define __32bit_syscall_numbers__
#include <asm/unistd.h>
<<<<<<< HEAD

unsigned sparc32_dir_class[] = {
=======
#include "kernel.h"

unsigned int sparc32_dir_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_dir_write.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_chattr_class[] = {
=======
unsigned int sparc32_chattr_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_change_attr.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_write_class[] = {
=======
unsigned int sparc32_write_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_write.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_read_class[] = {
=======
unsigned int sparc32_read_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_read.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_signal_class[] = {
=======
unsigned int sparc32_signal_class[] = {
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <asm-generic/audit_signal.h>
~0U
};

<<<<<<< HEAD
int sparc32_classify_syscall(unsigned syscall)
=======
int sparc32_classify_syscall(unsigned int syscall)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
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
		return 1;
	}
}
