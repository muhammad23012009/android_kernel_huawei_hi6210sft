#include <linux/mm.h>
<<<<<<< HEAD
#include <asm/page.h>
#include <asm/mman.h>
=======
#include <asm/elf.h>
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

const char *arch_vma_name(struct vm_area_struct *vma)
{
	if (vma->vm_mm && vma->vm_start == um_vdso_addr)
		return "[vdso]";

	return NULL;
}
<<<<<<< HEAD

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return NULL;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return 0;
}

int in_gate_area_no_mm(unsigned long addr)
{
	return 0;
}
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
