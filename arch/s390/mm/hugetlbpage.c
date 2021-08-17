/*
 *  IBM System z Huge TLB Page Support for Kernel.
 *
<<<<<<< HEAD
 *    Copyright IBM Corp. 2007
 *    Author(s): Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#include <linux/mm.h>
#include <linux/hugetlb.h>


void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
				   pte_t *pteptr, pte_t pteval)
{
	pmd_t *pmdp = (pmd_t *) pteptr;
	unsigned long mask;

	if (!MACHINE_HAS_HPAGE) {
		pteptr = (pte_t *) pte_page(pteval)[1].index;
		mask = pte_val(pteval) &
				(_SEGMENT_ENTRY_INV | _SEGMENT_ENTRY_RO);
		pte_val(pteval) = (_SEGMENT_ENTRY + __pa(pteptr)) | mask;
	}

	pmd_val(*pmdp) = pte_val(pteval);
}

int arch_prepare_hugepage(struct page *page)
{
	unsigned long addr = page_to_phys(page);
	pte_t pte;
	pte_t *ptep;
	int i;

	if (MACHINE_HAS_HPAGE)
		return 0;

	ptep = (pte_t *) pte_alloc_one(&init_mm, addr);
	if (!ptep)
		return -ENOMEM;

	pte_val(pte) = addr;
	for (i = 0; i < PTRS_PER_PTE; i++) {
		set_pte_at(&init_mm, addr + i * PAGE_SIZE, ptep + i, pte);
		pte_val(pte) += PAGE_SIZE;
	}
	page[1].index = (unsigned long) ptep;
	return 0;
}

void arch_release_hugepage(struct page *page)
{
	pte_t *ptep;

	if (MACHINE_HAS_HPAGE)
		return;

	ptep = (pte_t *) page[1].index;
	if (!ptep)
		return;
	clear_table((unsigned long *) ptep, _PAGE_TYPE_EMPTY,
		    PTRS_PER_PTE * sizeof(pte_t));
	page_table_free(&init_mm, (unsigned long *) ptep);
	page[1].index = 0;
=======
 *    Copyright IBM Corp. 2007,2016
 *    Author(s): Gerald Schaefer <gerald.schaefer@de.ibm.com>
 */

#define KMSG_COMPONENT "hugetlb"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/mm.h>
#include <linux/hugetlb.h>

/*
 * If the bit selected by single-bit bitmask "a" is set within "x", move
 * it to the position indicated by single-bit bitmask "b".
 */
#define move_set_bit(x, a, b)	(((x) & (a)) >> ilog2(a) << ilog2(b))

static inline unsigned long __pte_to_rste(pte_t pte)
{
	unsigned long rste;

	/*
	 * Convert encoding		  pte bits	pmd / pud bits
	 *				lIR.uswrdy.p	dy..R...I...wr
	 * empty			010.000000.0 -> 00..0...1...00
	 * prot-none, clean, old	111.000000.1 -> 00..1...1...00
	 * prot-none, clean, young	111.000001.1 -> 01..1...1...00
	 * prot-none, dirty, old	111.000010.1 -> 10..1...1...00
	 * prot-none, dirty, young	111.000011.1 -> 11..1...1...00
	 * read-only, clean, old	111.000100.1 -> 00..1...1...01
	 * read-only, clean, young	101.000101.1 -> 01..1...0...01
	 * read-only, dirty, old	111.000110.1 -> 10..1...1...01
	 * read-only, dirty, young	101.000111.1 -> 11..1...0...01
	 * read-write, clean, old	111.001100.1 -> 00..1...1...11
	 * read-write, clean, young	101.001101.1 -> 01..1...0...11
	 * read-write, dirty, old	110.001110.1 -> 10..0...1...11
	 * read-write, dirty, young	100.001111.1 -> 11..0...0...11
	 * HW-bits: R read-only, I invalid
	 * SW-bits: p present, y young, d dirty, r read, w write, s special,
	 *	    u unused, l large
	 */
	if (pte_present(pte)) {
		rste = pte_val(pte) & PAGE_MASK;
		rste |= move_set_bit(pte_val(pte), _PAGE_READ,
				     _SEGMENT_ENTRY_READ);
		rste |= move_set_bit(pte_val(pte), _PAGE_WRITE,
				     _SEGMENT_ENTRY_WRITE);
		rste |= move_set_bit(pte_val(pte), _PAGE_INVALID,
				     _SEGMENT_ENTRY_INVALID);
		rste |= move_set_bit(pte_val(pte), _PAGE_PROTECT,
				     _SEGMENT_ENTRY_PROTECT);
		rste |= move_set_bit(pte_val(pte), _PAGE_DIRTY,
				     _SEGMENT_ENTRY_DIRTY);
		rste |= move_set_bit(pte_val(pte), _PAGE_YOUNG,
				     _SEGMENT_ENTRY_YOUNG);
#ifdef CONFIG_MEM_SOFT_DIRTY
		rste |= move_set_bit(pte_val(pte), _PAGE_SOFT_DIRTY,
				     _SEGMENT_ENTRY_SOFT_DIRTY);
#endif
	} else
		rste = _SEGMENT_ENTRY_INVALID;
	return rste;
}

static inline pte_t __rste_to_pte(unsigned long rste)
{
	int present;
	pte_t pte;

	if ((rste & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		present = pud_present(__pud(rste));
	else
		present = pmd_present(__pmd(rste));

	/*
	 * Convert encoding		pmd / pud bits	    pte bits
	 *				dy..R...I...wr	  lIR.uswrdy.p
	 * empty			00..0...1...00 -> 010.000000.0
	 * prot-none, clean, old	00..1...1...00 -> 111.000000.1
	 * prot-none, clean, young	01..1...1...00 -> 111.000001.1
	 * prot-none, dirty, old	10..1...1...00 -> 111.000010.1
	 * prot-none, dirty, young	11..1...1...00 -> 111.000011.1
	 * read-only, clean, old	00..1...1...01 -> 111.000100.1
	 * read-only, clean, young	01..1...0...01 -> 101.000101.1
	 * read-only, dirty, old	10..1...1...01 -> 111.000110.1
	 * read-only, dirty, young	11..1...0...01 -> 101.000111.1
	 * read-write, clean, old	00..1...1...11 -> 111.001100.1
	 * read-write, clean, young	01..1...0...11 -> 101.001101.1
	 * read-write, dirty, old	10..0...1...11 -> 110.001110.1
	 * read-write, dirty, young	11..0...0...11 -> 100.001111.1
	 * HW-bits: R read-only, I invalid
	 * SW-bits: p present, y young, d dirty, r read, w write, s special,
	 *	    u unused, l large
	 */
	if (present) {
		pte_val(pte) = rste & _SEGMENT_ENTRY_ORIGIN_LARGE;
		pte_val(pte) |= _PAGE_LARGE | _PAGE_PRESENT;
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_READ,
					     _PAGE_READ);
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_WRITE,
					     _PAGE_WRITE);
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_INVALID,
					     _PAGE_INVALID);
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_PROTECT,
					     _PAGE_PROTECT);
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_DIRTY,
					     _PAGE_DIRTY);
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_YOUNG,
					     _PAGE_YOUNG);
#ifdef CONFIG_MEM_SOFT_DIRTY
		pte_val(pte) |= move_set_bit(rste, _SEGMENT_ENTRY_SOFT_DIRTY,
					     _PAGE_SOFT_DIRTY);
#endif
	} else
		pte_val(pte) = _PAGE_INVALID;
	return pte;
}

void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte)
{
	unsigned long rste = __pte_to_rste(pte);

	/* Set correct table type for 2G hugepages */
	if ((pte_val(*ptep) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		rste |= _REGION_ENTRY_TYPE_R3 | _REGION3_ENTRY_LARGE;
	else
		rste |= _SEGMENT_ENTRY_LARGE;
	pte_val(*ptep) = rste;
}

pte_t huge_ptep_get(pte_t *ptep)
{
	return __rste_to_pte(pte_val(*ptep));
}

pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
			      unsigned long addr, pte_t *ptep)
{
	pte_t pte = huge_ptep_get(ptep);
	pmd_t *pmdp = (pmd_t *) ptep;
	pud_t *pudp = (pud_t *) ptep;

	if ((pte_val(*ptep) & _REGION_ENTRY_TYPE_MASK) == _REGION_ENTRY_TYPE_R3)
		pudp_xchg_direct(mm, addr, pudp, __pud(_REGION3_ENTRY_EMPTY));
	else
		pmdp_xchg_direct(mm, addr, pmdp, __pmd(_SEGMENT_ENTRY_EMPTY));
	return pte;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

pte_t *huge_pte_alloc(struct mm_struct *mm,
			unsigned long addr, unsigned long sz)
{
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp = NULL;

	pgdp = pgd_offset(mm, addr);
	pudp = pud_alloc(mm, pgdp, addr);
<<<<<<< HEAD
	if (pudp)
		pmdp = pmd_alloc(mm, pudp, addr);
=======
	if (pudp) {
		if (sz == PUD_SIZE)
			return (pte_t *) pudp;
		else if (sz == PMD_SIZE)
			pmdp = pmd_alloc(mm, pudp, addr);
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	return (pte_t *) pmdp;
}

pte_t *huge_pte_offset(struct mm_struct *mm, unsigned long addr)
{
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp = NULL;

	pgdp = pgd_offset(mm, addr);
	if (pgd_present(*pgdp)) {
		pudp = pud_offset(pgdp, addr);
<<<<<<< HEAD
		if (pud_present(*pudp))
			pmdp = pmd_offset(pudp, addr);
=======
		if (pud_present(*pudp)) {
			if (pud_large(*pudp))
				return (pte_t *) pudp;
			pmdp = pmd_offset(pudp, addr);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	}
	return (pte_t *) pmdp;
}

<<<<<<< HEAD
int huge_pmd_unshare(struct mm_struct *mm, unsigned long *addr, pte_t *ptep)
{
	return 0;
}

struct page *follow_huge_addr(struct mm_struct *mm, unsigned long address,
			      int write)
{
	return ERR_PTR(-EINVAL);
}

int pmd_huge(pmd_t pmd)
{
	if (!MACHINE_HAS_HPAGE)
		return 0;

	return !!(pmd_val(pmd) & _SEGMENT_ENTRY_LARGE);
=======
int pmd_huge(pmd_t pmd)
{
	return pmd_large(pmd);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

int pud_huge(pud_t pud)
{
<<<<<<< HEAD
	return 0;
}

struct page *follow_huge_pmd(struct mm_struct *mm, unsigned long address,
			     pmd_t *pmdp, int write)
{
	struct page *page;

	if (!MACHINE_HAS_HPAGE)
		return NULL;

	page = pmd_page(*pmdp);
	if (page)
		page += ((address & ~HPAGE_MASK) >> PAGE_SHIFT);
	return page;
}
=======
	return pud_large(pud);
}

struct page *
follow_huge_pud(struct mm_struct *mm, unsigned long address,
		pud_t *pud, int flags)
{
	if (flags & FOLL_GET)
		return NULL;

	return pud_page(*pud) + ((address & ~PUD_MASK) >> PAGE_SHIFT);
}

static __init int setup_hugepagesz(char *opt)
{
	unsigned long size;
	char *string = opt;

	size = memparse(opt, &opt);
	if (MACHINE_HAS_EDAT1 && size == PMD_SIZE) {
		hugetlb_add_hstate(PMD_SHIFT - PAGE_SHIFT);
	} else if (MACHINE_HAS_EDAT2 && size == PUD_SIZE) {
		hugetlb_add_hstate(PUD_SHIFT - PAGE_SHIFT);
	} else {
		hugetlb_bad_size();
		pr_err("hugepagesz= specifies an unsupported page size %s\n",
			string);
		return 0;
	}
	return 1;
}
__setup("hugepagesz=", setup_hugepagesz);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
