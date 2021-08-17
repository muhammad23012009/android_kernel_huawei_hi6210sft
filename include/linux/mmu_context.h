#ifndef _LINUX_MMU_CONTEXT_H
#define _LINUX_MMU_CONTEXT_H

<<<<<<< HEAD
=======
#include <asm/mmu_context.h>

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
struct mm_struct;

void use_mm(struct mm_struct *mm);
void unuse_mm(struct mm_struct *mm);

<<<<<<< HEAD
=======
/* Architectures that care about IRQ state in switch_mm can override this. */
#ifndef switch_mm_irqs_off
# define switch_mm_irqs_off switch_mm
#endif

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#endif
