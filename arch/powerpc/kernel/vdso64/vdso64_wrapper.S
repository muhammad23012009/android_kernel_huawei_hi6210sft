<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/linkage.h>
#include <asm/page.h>

	__PAGE_ALIGNED_DATA

	.globl vdso64_start, vdso64_end
	.balign PAGE_SIZE
vdso64_start:
<<<<<<< HEAD
	.incbin "arch/powerpc/kernel/vdso64/vdso64.so"
=======
	.incbin "arch/powerpc/kernel/vdso64/vdso64.so.dbg"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.balign PAGE_SIZE
vdso64_end:

	.previous
