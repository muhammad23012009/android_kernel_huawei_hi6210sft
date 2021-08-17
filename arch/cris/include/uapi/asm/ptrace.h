<<<<<<< HEAD
#include <arch/ptrace.h>
=======
#ifdef __arch_v32
#include <asm/ptrace_v32.h>
#else
#include <asm/ptrace_v10.h>
#endif
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
