#ifndef _LINUX_HUGETLB_INLINE_H
#define _LINUX_HUGETLB_INLINE_H

#ifdef CONFIG_HUGETLB_PAGE

#include <linux/mm.h>

<<<<<<< HEAD
static inline int is_vm_hugetlb_page(struct vm_area_struct *vma)
=======
static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	return !!(vma->vm_flags & VM_HUGETLB);
}

#else

<<<<<<< HEAD
static inline int is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return 0;
=======
static inline bool is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return false;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif

#endif
