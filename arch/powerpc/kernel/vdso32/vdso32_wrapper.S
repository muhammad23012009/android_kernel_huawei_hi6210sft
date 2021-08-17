<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/linkage.h>
#include <asm/page.h>

	__PAGE_ALIGNED_DATA

	.globl vdso32_start, vdso32_end
	.balign PAGE_SIZE
vdso32_start:
<<<<<<< HEAD
	.incbin "arch/powerpc/kernel/vdso32/vdso32.so"
=======
	.incbin "arch/powerpc/kernel/vdso32/vdso32.so.dbg"
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	.balign PAGE_SIZE
vdso32_end:

	.previous
