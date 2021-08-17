/* arch/sparc64/mm/tlb.c
 *
 * Copyright (C) 2004 David S. Miller <davem@redhat.com>
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/preempt.h>

#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>
#include <asm/mmu_context.h>
#include <asm/tlb.h>

/* Heavily inspired by the ppc64 code.  */

static DEFINE_PER_CPU(struct tlb_batch, tlb_batch);

void flush_tlb_pending(void)
{
	struct tlb_batch *tb = &get_cpu_var(tlb_batch);
	struct mm_struct *mm = tb->mm;

	if (!tb->tlb_nr)
		goto out;

	flush_tsb_user(tb);

	if (CTX_VALID(mm->context)) {
		if (tb->tlb_nr == 1) {
			global_flush_tlb_page(mm, tb->vaddrs[0]);
		} else {
#ifdef CONFIG_SMP
			smp_flush_tlb_pending(tb->mm, tb->tlb_nr,
					      &tb->vaddrs[0]);
#else
			__flush_tlb_pending(CTX_HWBITS(tb->mm->context),
					    tb->tlb_nr, &tb->vaddrs[0]);
#endif
		}
	}

	tb->tlb_nr = 0;

out:
	put_cpu_var(tlb_batch);
}

void arch_enter_lazy_mmu_mode(void)
{
<<<<<<< HEAD
	struct tlb_batch *tb = &__get_cpu_var(tlb_batch);
=======
	struct tlb_batch *tb = this_cpu_ptr(&tlb_batch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tb->active = 1;
}

void arch_leave_lazy_mmu_mode(void)
{
<<<<<<< HEAD
	struct tlb_batch *tb = &__get_cpu_var(tlb_batch);
=======
	struct tlb_batch *tb = this_cpu_ptr(&tlb_batch);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	if (tb->tlb_nr)
		flush_tlb_pending();
	tb->active = 0;
}

static void tlb_batch_add_one(struct mm_struct *mm, unsigned long vaddr,
<<<<<<< HEAD
			      bool exec)
=======
			      bool exec, bool huge)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct tlb_batch *tb = &get_cpu_var(tlb_batch);
	unsigned long nr;

	vaddr &= PAGE_MASK;
	if (exec)
		vaddr |= 0x1UL;

	nr = tb->tlb_nr;

	if (unlikely(nr != 0 && mm != tb->mm)) {
		flush_tlb_pending();
		nr = 0;
	}

	if (!tb->active) {
<<<<<<< HEAD
		flush_tsb_user_page(mm, vaddr);
=======
		flush_tsb_user_page(mm, vaddr, huge);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		global_flush_tlb_page(mm, vaddr);
		goto out;
	}

<<<<<<< HEAD
	if (nr == 0)
		tb->mm = mm;
=======
	if (nr == 0) {
		tb->mm = mm;
		tb->huge = huge;
	}

	if (tb->huge != huge) {
		flush_tlb_pending();
		tb->huge = huge;
		nr = 0;
	}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

	tb->vaddrs[nr] = vaddr;
	tb->tlb_nr = ++nr;
	if (nr >= TLB_BATCH_NR)
		flush_tlb_pending();

out:
	put_cpu_var(tlb_batch);
}

void tlb_batch_add(struct mm_struct *mm, unsigned long vaddr,
		   pte_t *ptep, pte_t orig, int fullmm)
{
<<<<<<< HEAD
=======
	bool huge = is_hugetlb_pte(orig);

>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
	if (tlb_type != hypervisor &&
	    pte_dirty(orig)) {
		unsigned long paddr, pfn = pte_pfn(orig);
		struct address_space *mapping;
		struct page *page;

		if (!pfn_valid(pfn))
			goto no_cache_flush;

		page = pfn_to_page(pfn);
		if (PageReserved(page))
			goto no_cache_flush;

		/* A real file page? */
		mapping = page_mapping(page);
		if (!mapping)
			goto no_cache_flush;

		paddr = (unsigned long) page_address(page);
		if ((paddr ^ vaddr) & (1 << 13))
			flush_dcache_page_all(mm, page);
	}

no_cache_flush:
	if (!fullmm)
<<<<<<< HEAD
		tlb_batch_add_one(mm, vaddr, pte_exec(orig));
=======
		tlb_batch_add_one(mm, vaddr, pte_exec(orig), huge);
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
}

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static void tlb_batch_pmd_scan(struct mm_struct *mm, unsigned long vaddr,
<<<<<<< HEAD
			       pmd_t pmd, bool exec)
=======
			       pmd_t pmd)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	unsigned long end;
	pte_t *pte;

	pte = pte_offset_map(&pmd, vaddr);
	end = vaddr + HPAGE_SIZE;
	while (vaddr < end) {
<<<<<<< HEAD
		if (pte_val(*pte) & _PAGE_VALID)
			tlb_batch_add_one(mm, vaddr, exec);
=======
		if (pte_val(*pte) & _PAGE_VALID) {
			bool exec = pte_exec(*pte);

			tlb_batch_add_one(mm, vaddr, exec, false);
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		pte++;
		vaddr += PAGE_SIZE;
	}
	pte_unmap(pte);
}

<<<<<<< HEAD
void set_pmd_at(struct mm_struct *mm, unsigned long addr,
		pmd_t *pmdp, pmd_t pmd)
{
	pmd_t orig = *pmdp;

	*pmdp = pmd;

	if (mm == &init_mm)
		return;

	if ((pmd_val(pmd) ^ pmd_val(orig)) & PMD_ISHUGE) {
		if (pmd_val(pmd) & PMD_ISHUGE)
			mm->context.huge_pte_count++;
		else
			mm->context.huge_pte_count--;
=======

static void __set_pmd_acct(struct mm_struct *mm, unsigned long addr,
			   pmd_t orig, pmd_t pmd)
{
	if (mm == &init_mm)
		return;

	if ((pmd_val(pmd) ^ pmd_val(orig)) & _PAGE_PMD_HUGE) {
		/*
		 * Note that this routine only sets pmds for THP pages.
		 * Hugetlb pages are handled elsewhere.  We need to check
		 * for huge zero page.  Huge zero pages are like hugetlb
		 * pages in that there is no RSS, but there is the need
		 * for TSB entries.  So, huge zero page counts go into
		 * hugetlb_pte_count.
		 */
		if (pmd_val(pmd) & _PAGE_PMD_HUGE) {
			if (is_huge_zero_page(pmd_page(pmd)))
				mm->context.hugetlb_pte_count++;
			else
				mm->context.thp_pte_count++;
		} else {
			if (is_huge_zero_page(pmd_page(orig)))
				mm->context.hugetlb_pte_count--;
			else
				mm->context.thp_pte_count--;
		}
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414

		/* Do not try to allocate the TSB hash table if we
		 * don't have one already.  We have various locks held
		 * and thus we'll end up doing a GFP_KERNEL allocation
		 * in an atomic context.
		 *
		 * Instead, we let the first TLB miss on a hugepage
		 * take care of this.
		 */
	}

	if (!pmd_none(orig)) {
<<<<<<< HEAD
		bool exec = ((pmd_val(orig) & PMD_HUGE_EXEC) != 0);

		addr &= HPAGE_MASK;
		if (pmd_val(orig) & PMD_ISHUGE)
			tlb_batch_add_one(mm, addr, exec);
		else
			tlb_batch_pmd_scan(mm, addr, orig, exec);
	}
}

void pgtable_trans_huge_deposit(struct mm_struct *mm, pgtable_t pgtable)
=======
		addr &= HPAGE_MASK;
		if (pmd_trans_huge(orig)) {
			pte_t orig_pte = __pte(pmd_val(orig));
			bool exec = pte_exec(orig_pte);

			tlb_batch_add_one(mm, addr, exec, true);
			tlb_batch_add_one(mm, addr + REAL_HPAGE_SIZE, exec,
					true);
		} else {
			tlb_batch_pmd_scan(mm, addr, orig);
		}
	}
}

void set_pmd_at(struct mm_struct *mm, unsigned long addr,
		pmd_t *pmdp, pmd_t pmd)
{
	pmd_t orig = *pmdp;

	*pmdp = pmd;
	__set_pmd_acct(mm, addr, orig, pmd);
}

static inline pmd_t pmdp_establish(struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmdp, pmd_t pmd)
{
	pmd_t old;

	do {
		old = *pmdp;
	} while (cmpxchg64(&pmdp->pmd, old.pmd, pmd.pmd) != old.pmd);
	__set_pmd_acct(vma->vm_mm, address, old, pmd);

	return old;
}

/*
 * This routine is only called when splitting a THP
 */
pmd_t pmdp_invalidate(struct vm_area_struct *vma, unsigned long address,
		     pmd_t *pmdp)
{
	pmd_t old, entry;

	entry = __pmd(pmd_val(*pmdp) & ~_PAGE_VALID);
	old = pmdp_establish(vma, address, pmdp, entry);
	flush_tlb_range(vma, address, address + HPAGE_PMD_SIZE);

	/*
	 * set_pmd_at() will not be called in a way to decrement
	 * thp_pte_count when splitting a THP, so do it now.
	 * Sanity check pmd before doing the actual decrement.
	 */
	if ((pmd_val(entry) & _PAGE_PMD_HUGE) &&
	    !is_huge_zero_page(pmd_page(entry)))
		(vma->vm_mm)->context.thp_pte_count--;

	return old;
}

void pgtable_trans_huge_deposit(struct mm_struct *mm, pmd_t *pmdp,
				pgtable_t pgtable)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct list_head *lh = (struct list_head *) pgtable;

	assert_spin_locked(&mm->page_table_lock);

	/* FIFO */
<<<<<<< HEAD
	if (!mm->pmd_huge_pte)
		INIT_LIST_HEAD(lh);
	else
		list_add(lh, (struct list_head *) mm->pmd_huge_pte);
	mm->pmd_huge_pte = pgtable;
}

pgtable_t pgtable_trans_huge_withdraw(struct mm_struct *mm)
=======
	if (!pmd_huge_pte(mm, pmdp))
		INIT_LIST_HEAD(lh);
	else
		list_add(lh, (struct list_head *) pmd_huge_pte(mm, pmdp));
	pmd_huge_pte(mm, pmdp) = pgtable;
}

pgtable_t pgtable_trans_huge_withdraw(struct mm_struct *mm, pmd_t *pmdp)
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
{
	struct list_head *lh;
	pgtable_t pgtable;

	assert_spin_locked(&mm->page_table_lock);

	/* FIFO */
<<<<<<< HEAD
	pgtable = mm->pmd_huge_pte;
	lh = (struct list_head *) pgtable;
	if (list_empty(lh))
		mm->pmd_huge_pte = NULL;
	else {
		mm->pmd_huge_pte = (pgtable_t) lh->next;
=======
	pgtable = pmd_huge_pte(mm, pmdp);
	lh = (struct list_head *) pgtable;
	if (list_empty(lh))
		pmd_huge_pte(mm, pmdp) = NULL;
	else {
		pmd_huge_pte(mm, pmdp) = (pgtable_t) lh->next;
>>>>>>> cb99ff2b40d4357e990bd96b2c791860c4b0a414
		list_del(lh);
	}
	pte_val(pgtable[0]) = 0;
	pte_val(pgtable[1]) = 0;

	return pgtable;
}
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */
