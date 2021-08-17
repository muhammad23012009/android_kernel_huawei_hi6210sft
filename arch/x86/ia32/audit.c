#include <asm/unistd_32.h>

unsigned ia32_dir_class[] = {
#include <asm-generic/audit_dir_write.h>
~0U
};

unsigned ia32_chattr_class[] = {
#include <asm-generic/audit_change_attr.h>
~0U
};

unsigned ia32_write_class[] = {
#include <asm-generic/audit_write.h>
~0U
};

unsigned ia32_read_class[] = {
#include <asm-generic/audit_read.h>
~0U
};

unsigned ia32_signal_class[] = {
#include <asm-generic/audit_signal.h>
~0U
};

int ia32_classify_syscall(unsigned syscall)
{
	switch (syscall) {
	case __NR_open:
		return 2;
	case __NR_openat:
		return 3;
	case __NR_socketcall:
		return 4;
	case __NR_execve:
<<<<<<< HEAD
=======
	case __NR_execveat:
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		return 5;
	default:
		return 1;
	}
}
