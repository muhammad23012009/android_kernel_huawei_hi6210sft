/*
 *  IBM System z Huge TLB Page Support for Kernel.
 *
 *    Copyright IBM Corp. 2008
 *    Author(s): Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#ifndef _ASM_S390_HUGETLB_H
#define _ASM_S390_HUGETLB_H

#include <asm/page.h>
#include <asm/pgtable.h>


#define is_hugepage_only_range(mm, addr, len)	0
#define hugetlb_free_pgd_range			free_pgd_range
<<<<<<< HEAD

void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte);
=======
#define hugepages_supported()			(MACHINE_HAS_HPAGE)

void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte);
pte_t huge_ptep_get(pte_t *ptep);
pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
			      unsigned long addr, pte_t *ptep);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

/*
 * If the arch doesn't supply something else, assume that hugepage
 * size aligned regions are ok without further preparation.
 */
static inline int prepare_hugepage_range(struct file *file,
			unsigned long addr, unsigned long len)
{
	if (len & ~HPAGE_MASK)
		return -EINVAL;
	if (addr & ~HPAGE_MASK)
		return -EINVAL;
	return 0;
}

<<<<<<< HEAD
#define hugetlb_prefault_arch_hook(mm)		do { } while (0)
#define arch_clear_hugepage_flags(page)		do { } while (0)

int arch_prepare_hugepage(struct page *page);
void arch_release_hugepage(struct page *page);

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	pte_val(pte) |= _PAGE_RO;
	return pte;
}

static inline int huge_pte_none(pte_t pte)
{
	return (pte_val(pte) & _SEGMENT_ENTRY_INV) &&
		!(pte_val(pte) & _SEGMENT_ENTRY_RO);
}

static inline pte_t huge_ptep_get(pte_t *ptep)
{
	pte_t pte = *ptep;
	unsigned long mask;

	if (!MACHINE_HAS_HPAGE) {
		ptep = (pte_t *) (pte_val(pte) & _SEGMENT_ENTRY_ORIGIN);
		if (ptep) {
			mask = pte_val(pte) &
				(_SEGMENT_ENTRY_INV | _SEGMENT_ENTRY_RO);
			pte = pte_mkhuge(*ptep);
			pte_val(pte) |= mask;
		}
	}
	return pte;
}

static inline void __pmd_csp(pmd_t *pmdp)
{
	register unsigned long reg2 asm("2") = pmd_val(*pmdp);
	register unsigned long reg3 asm("3") = pmd_val(*pmdp) |
					       _SEGMENT_ENTRY_INV;
	register unsigned long reg4 asm("4") = ((unsigned long) pmdp) + 5;

	asm volatile(
		"	csp %1,%3"
		: "=m" (*pmdp)
		: "d" (reg2), "d" (reg3), "d" (reg4), "m" (*pmdp) : "cc");
}

static inline void huge_ptep_invalidate(struct mm_struct *mm,
					unsigned long address, pte_t *ptep)
{
	pmd_t *pmdp = (pmd_t *) ptep;

	if (MACHINE_HAS_IDTE)
		__pmd_idte(address, pmdp);
	else
		__pmd_csp(pmdp);
	pmd_val(*pmdp) = _SEGMENT_ENTRY_INV | _SEGMENT_ENTRY;
}

static inline pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
					    unsigned long addr, pte_t *ptep)
{
	pte_t pte = huge_ptep_get(ptep);

	huge_ptep_invalidate(mm, addr, ptep);
	return pte;
}

#define huge_ptep_set_access_flags(__vma, __addr, __ptep, __entry, __dirty) \
({									    \
	int __changed = !pte_same(huge_ptep_get(__ptep), __entry);	    \
	if (__changed) {						    \
		huge_ptep_invalidate((__vma)->vm_mm, __addr, __ptep);	    \
		set_huge_pte_at((__vma)->vm_mm, __addr, __ptep, __entry);   \
	}								    \
	__changed;							    \
})

#define huge_ptep_set_wrprotect(__mm, __addr, __ptep)			\
({									\
	pte_t __pte = huge_ptep_get(__ptep);				\
	if (huge_pte_write(__pte)) {					\
		huge_ptep_invalidate(__mm, __addr, __ptep);		\
		set_huge_pte_at(__mm, __addr, __ptep,			\
				huge_pte_wrprotect(__pte));		\
	}								\
})

static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long address, pte_t *ptep)
{
	huge_ptep_invalidate(vma->vm_mm, address, ptep);
}

static inline pte_t mk_huge_pte(struct page *page, pgprot_t pgprot)
{
	pte_t pte;
	pmd_t pmd;

	pmd = mk_pmd_phys(page_to_phys(page), pgprot);
	pte_val(pte) = pmd_val(pmd);
	return pte;
=======
#define arch_clear_hugepage_flags(page)		do { } while (0)

static inline void huge_pte_clear(struct mm_struct *mm, unsigned long addr,
				  pte_t *ptep)
{
	if ((pte_val(*ptep) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		pte_val(*ptep) = _REGION3_ENTRY_EMPTY;
	else
		pte_val(*ptep) = _SEGMENT_ENTRY_EMPTY;
}

static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long address, pte_t *ptep)
{
	huge_ptep_get_and_clear(vma->vm_mm, address, ptep);
}

static inline int huge_ptep_set_access_flags(struct vm_area_struct *vma,
					     unsigned long addr, pte_t *ptep,
					     pte_t pte, int dirty)
{
	int changed = !pte_same(huge_ptep_get(ptep), pte);
	if (changed) {
		huge_ptep_get_and_clear(vma->vm_mm, addr, ptep);
		set_huge_pte_at(vma->vm_mm, addr, ptep, pte);
	}
	return changed;
}

static inline void huge_ptep_set_wrprotect(struct mm_struct *mm,
					   unsigned long addr, pte_t *ptep)
{
	pte_t pte = huge_ptep_get_and_clear(mm, addr, ptep);
	set_huge_pte_at(mm, addr, ptep, pte_wrprotect(pte));
}

static inline pte_t mk_huge_pte(struct page *page, pgprot_t pgprot)
{
	return mk_pte(page, pgprot);
}

static inline int huge_pte_none(pte_t pte)
{
	return pte_none(pte);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline int huge_pte_write(pte_t pte)
{
<<<<<<< HEAD
	pmd_t pmd;

	pmd_val(pmd) = pte_val(pte);
	return pmd_write(pmd);
=======
	return pte_write(pte);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline int huge_pte_dirty(pte_t pte)
{
<<<<<<< HEAD
	/* No dirty bit in the segment table entry. */
	return 0;
=======
	return pte_dirty(pte);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline pte_t huge_pte_mkwrite(pte_t pte)
{
<<<<<<< HEAD
	pmd_t pmd;

	pmd_val(pmd) = pte_val(pte);
	pte_val(pte) = pmd_val(pmd_mkwrite(pmd));
	return pte;
=======
	return pte_mkwrite(pte);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

static inline pte_t huge_pte_mkdirty(pte_t pte)
{
<<<<<<< HEAD
	/* No dirty bit in the segment table entry. */
	return pte;
}

static inline pte_t huge_pte_modify(pte_t pte, pgprot_t newprot)
{
	pmd_t pmd;

	pmd_val(pmd) = pte_val(pte);
	pte_val(pte) = pmd_val(pmd_modify(pmd, newprot));
	return pte;
}

static inline void huge_pte_clear(struct mm_struct *mm, unsigned long addr,
				  pte_t *ptep)
{
	pmd_clear((pmd_t *) ptep);
=======
	return pte_mkdirty(pte);
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	return pte_wrprotect(pte);
}

static inline pte_t huge_pte_modify(pte_t pte, pgprot_t newprot)
{
	return pte_modify(pte, newprot);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#endif /* _ASM_S390_HUGETLB_H */
