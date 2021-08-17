#ifndef _UAPI_ASM_X86_MSR_H
#define _UAPI_ASM_X86_MSR_H

<<<<<<< HEAD
#include <asm/msr-index.h>

=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <linux/ioctl.h>

#define X86_IOC_RDMSR_REGS	_IOWR('c', 0xA0, __u32[8])
#define X86_IOC_WRMSR_REGS	_IOWR('c', 0xA1, __u32[8])

#endif /* __ASSEMBLY__ */
#endif /* _UAPI_ASM_X86_MSR_H */
